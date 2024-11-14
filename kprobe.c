// go:build ignore
#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t
{
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct sockaddr *sock_addr;
    u32 addrlen;
};

struct
{
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_accept SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_bind SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_connect SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

static __always_inline int init_conn_info(void *conn_map, u32 pid, struct pt_regs *ctx)
{
    // Проверяем, есть ли уже запись для данного PID
    struct conn_info_t *conn_info = bpf_map_lookup_elem(conn_map, &pid);
    if (conn_info)
    {
        // Если запись существует, ничего не делаем
        return 0;
    }

    // Инициализируем структуру для нового PID
    struct conn_info_t new_conn_info = {};
    new_conn_info.pid = pid;
    bpf_get_current_comm(&new_conn_info.comm, sizeof(new_conn_info.comm));
    new_conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);

    // Добавляем новую запись в карту
    bpf_map_update_elem(conn_map, &pid, &new_conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_entry(struct trace_event_raw_sys_enter *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info(&conn_info_map_accept, pid, (struct pt_regs *)ctx);

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
    if (conn_info)
    {
        bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_ret(struct trace_event_raw_sys_exit *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int ret = ctx->ret;

    if (ret < 0)
    {
        // Если соединение не установлено, удаляем информацию из карты
        bpf_map_delete_elem(&conn_info_map_accept, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
    if (!conn_info)
    {
        bpf_printk("No connection info found for ACCEPT4 PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("ACCEPTED connection: PID=%d, Comm=%s, Client IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

        // Обновляем карту с новой информацией
        bpf_map_update_elem(&conn_info_map_accept, &pid, conn_info, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_entry(struct trace_event_raw_sys_enter *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info(&conn_info_map_bind, pid, (struct pt_regs *)ctx);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_ret(struct trace_event_raw_sys_exit *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int ret = ctx->ret;

    if (ret < 0)
    {
        // Если bind не удался, удаляем информацию из карты
        bpf_map_delete_elem(&conn_info_map_bind, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
    if (!conn_info)
    {
        bpf_printk("No connection info found for BIND PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->dport = bpf_ntohs(addr.sin_port);

        bpf_printk("SERVER Bind: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
                   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);

        // Обновляем карту с новой информацией
        bpf_map_update_elem(&conn_info_map_bind, &pid, conn_info, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_entry(struct trace_event_raw_sys_enter *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info(&conn_info_map_connect, pid, (struct pt_regs *)ctx);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_ret(struct trace_event_raw_sys_exit *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int ret = ctx->ret;

    if (ret < 0)
    {
        // Если connect не удался, удаляем информацию из карты
        bpf_map_delete_elem(&conn_info_map_connect, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_connect, &pid);
    if (!conn_info)
    {
        bpf_printk("No connection info found for CONNECT PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->dport = bpf_ntohs(addr.sin_port);

        bpf_printk("CLIENT Connected to server: PID=%d, Comm=%s, Server IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
                   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);

        // Обновляем карту с новой информацией
        bpf_map_update_elem(&conn_info_map_connect, &pid, conn_info, BPF_ANY);
    }

    return 0;
}
