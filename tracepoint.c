#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct sockaddr *sock_addr;
    u32 addrlen;
    u16 src_ip6[16]; // IPv6 адрес сохраняется как массив байтов
};

struct sys_enter_accept4_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
    int __padding;
    struct sockaddr *upeer_sockaddr;
    int *upeer_addrlen;
    int flags;
};

struct sys_exit_accept4_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    long ret;
};

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_accept_four SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2
#define AF_INET6 10

static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;
    bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
    return 0;
}

static __always_inline int init_conn_info_accept4(struct sys_enter_accept4_args *ctx) {
    return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_accept_four, bpf_get_current_pid_tgid() >> 32);
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_bind_ret(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info_accept4(ctx);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("EXIT_accept Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
    if (!conn_info) {
        bpf_printk("EXIT_accept4 No connection info found for PID=%d\n", pid);
        return 0;
    }

    u8 sa_family;
    if (bpf_probe_read(&sa_family, sizeof(sa_family), conn_info->sock_addr) != 0) {
        bpf_printk("EXIT_accept4 Failed to read sa_family for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
        return 0;
    }

    if (sa_family == AF_INET) {
        struct sockaddr_in addr4;
        if (bpf_probe_read(&addr4, sizeof(addr4), conn_info->sock_addr) != 0) {
            bpf_printk("EXIT_accept4 Failed to read sockaddr_in for PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
            return 0;
        }

        conn_info->src_ip = bpf_ntohl(addr4.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr4.sin_port);

        bpf_printk("EXIT_accept4 Accepted IPv4 connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    } else if (sa_family == AF_INET6) {
        struct sockaddr_in6 addr6;
        if (bpf_probe_read(&addr6, sizeof(addr6), conn_info->sock_addr) != 0) {
            bpf_printk("EXIT_accept4 Failed to read sockaddr_in6 for PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
            return 0;
        }

bpf_printk("EXIT_accept4 addr6.sin6_addr: ");
for (int i = 0; i < 8; i++) {
    uint16_t addr = addr6.sin6_addr.in6_u.u6_addr16[i];
    bpf_printk("%02x%02x ", addr & 0xFF, (addr >> 8) & 0xFF);
}
bpf_printk("\n");


// Копирование IPv6-адреса в src_ip6
if (bpf_probe_read(conn_info->src_ip6, sizeof(conn_info->src_ip6), addr6.sin6_addr.in6_u.u6_addr8) != 0) {
    bpf_printk("EXIT_accept4 Failed to copy IPv6 address for PID=%d\n", pid);
    bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
    return 0;
}

// Выводим поле conn_info->src_ip6 после копирования
bpf_printk("EXIT_accept4 src_ip6 for PID=%d: ", pid);
for (int i = 0; i < 8; i++) {
    uint16_t addr = (conn_info->src_ip6[2*i] << 8) | conn_info->src_ip6[2*i + 1];
    bpf_printk("%02x%02x ", (addr >> 8) & 0xFF, addr & 0xFF);
}
bpf_printk("\n");



        conn_info->sport = bpf_ntohs(addr6.sin6_port);
        bpf_printk("exit ACCEPT4 IP6 PORT=%d",conn_info->sport);

    } else {
        bpf_printk("EXIT_accept4 Unknown protocol family for PID=%d, sa_family=%d\n", pid, sa_family);
    }

    bpf_map_update_elem(&conn_info_map_accept_four, &pid, conn_info, BPF_ANY);

    return 0;
}
