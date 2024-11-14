#include <uapi/linux/ptrace.h>
#include <net/sock.h>
#include <linux/inet.h>
#include <linux/bpf.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u16 sport;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);
    __type(value, struct conn_info_t);
    __uint(max_entries, 1024);
} conn_info_map_accept SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Получаем указатель на sockaddr_in
    struct sockaddr_in addr;
    struct sockaddr *sock_addr = (struct sockaddr *)ctx->args[1];

    // Считываем структуру sockaddr_in из пользовательской памяти
    if (bpf_probe_read_user(&addr, sizeof(addr), sock_addr) == 0) {
        if (addr.sin_family == AF_INET) {
            conn_info.src_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info.sport = bpf_ntohs(addr.sin_port);
            bpf_printk("Accept enter: PID=%d, IP=%d.%d.%d.%d, Port=%d\n",
                pid,
                (conn_info.src_ip >> 24) & 0xFF, (conn_info.src_ip >> 16) & 0xFF,
                (conn_info.src_ip >> 8) & 0xFF, conn_info.src_ip & 0xFF, conn_info.sport);
        }
    } else {
        bpf_printk("Failed to read sockaddr at enter_accept for PID=%d\n", pid);
        return 0;
    }

    // Сохраняем conn_info в карту, используя PID как ключ
    bpf_map_update_elem(&conn_info_map_accept, &pid, &conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    int ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept, &pid);
        return 0;
    }

    // Ищем сохраненную информацию в карте по PID
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
    if (!conn_info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    bpf_printk("SERVER Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
        conn_info->pid, conn_info->comm,
        (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
        (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

    // Удаляем запись из карты после успешного использования
    bpf_map_delete_elem(&conn_info_map_accept, &pid);

    return 0;
}

char LICENSE[] SEC("license") = "GPL";
