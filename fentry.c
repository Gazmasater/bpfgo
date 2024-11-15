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
};

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_accept SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_c SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

// Функция для инициализации информации о соединении
static __always_inline int init_conn_info(u32 pid, struct sockaddr *sock_addr,struct conn_info_t *conn_info) {
    conn_info->pid = pid;
    bpf_get_current_comm(&conn_info->comm, sizeof(conn_info->comm));
    conn_info->sock_addr = sock_addr;
    return 0;
}

struct sys_enter_accept4_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
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
    long ret
};




SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_entry(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sockaddr *sock_addr = ctx->upeer_sockaddr; 
    struct conn_info_t conn_info = {};
    init_conn_info(pid, sock_addr, &conn_info);

    bpf_map_update_elem(&conn_info_map_accept, &pid, &conn_info, BPF_ANY);

    bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

    return 0;
}
// Tracepoint для завершения accept4
SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_ret(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
    if (!conn_info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("CLIENT Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
            conn_info->pid, conn_info->comm,
            (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
            (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    return 0;
}

// // Tracepoint для bind
// SEC("tracepoint/syscalls/sys_enter_bind")
// int trace_bind_entry(struct trace_event_raw_sys_enter *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     struct sockaddr *sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
//     u32 addrlen = PT_REGS_PARM3(ctx);

//     struct conn_info_t conn_info = {};
//     init_conn_info(pid, sock_addr, addrlen, &conn_info);

//     bpf_map_update_elem(&conn_info_map_ab, &pid, &conn_info, BPF_ANY);

//     bpf_printk("SERVER Bind entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

//     return 0;
// }

// // Tracepoint для завершения bind
// SEC("tracepoint/syscalls/sys_exit_bind")
// int trace_bind_ret(struct trace_event_raw_sys_exit *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = PT_REGS_RC(ctx);

//     if (ret < 0) {
//         bpf_printk("Bind failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map_ab, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for PID=%d\n", pid);
//         return 0;
//     }

//     struct sockaddr_in addr;
//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     if (addr.sin_family == AF_INET) {
//         conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
//         conn_info->dport = bpf_ntohs(addr.sin_port);

//         bpf_printk("SERVER Bind: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
//             (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
//     }

//     return 0;
// }

// // Tracepoint для connect
// SEC("tracepoint/syscalls/sys_enter_connect")
// int trace_connect_entry(struct trace_event_raw_sys_enter *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     struct sockaddr *sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
//     u32 addrlen = PT_REGS_PARM3(ctx);

//     struct conn_info_t conn_info = {};
//     init_conn_info(pid, sock_addr, addrlen, &conn_info);

//     bpf_map_update_elem(&conn_info_map_c, &pid, &conn_info, BPF_ANY);

//     bpf_printk("CLIENT Connect entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

//     return 0;
// }

// // Tracepoint для завершения connect
// SEC("tracepoint/syscalls/sys_exit_connect")
// int trace_connect_ret(struct trace_event_raw_sys_exit *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = PT_REGS_RC(ctx);

//     if (ret < 0) {
//         bpf_printk("Connect failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map_c, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_c, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for PID=%d\n", pid);
//         return 0;
//     }

//     struct sockaddr_in addr;
//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     if (addr.sin_family == AF_INET) {
//         conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
//         conn_info->dport = bpf_ntohs(addr.sin_port);

//         bpf_printk("CLIENT Connected to server: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
//             (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
//     }

//     return 0;
// }










