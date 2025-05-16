#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_endian.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

#define AF_INET 2
#define AF_INET6 10

struct conn_info_t {
    u32 pid;
    u32 fd;
    u32 addrlen;
    u16 dport;
    u16 sport;
    u16 family;
    u8 proto;
    char comm[64];
};

struct trace_info {
    struct in_addr srcIP;
    struct in_addr dstIP;
    __u32 srcIP6[4];
    __u32 dstIP6[4];
    __u16 ssrcIP6[8];
    __u16 ddstIP6[8];
    __u16 sport;
    __u16 dport;
    __u32 pid;
    __u32 proto;
    __u32 sysexit;
    __u32 state;
    __u16 family;
    char comm[32];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr *);
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
} trace_events SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    bpf_printk("[enter] trace_sendto_enter: pid=%d", bpf_get_current_pid_tgid() >> 32);
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct sockaddr *addr = (struct sockaddr *)(unsigned long)ctx->args[5];
    if (!addr) return 0;

    bpf_printk("[enter] saving addr pointer: %p", addr);
    bpf_map_update_elem(&addrSend_map, &pid, &addr, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr) return 0;

    struct trace_info info = {};
    struct sockaddr addr = {};

    if (bpf_core_read_user(&addr, sizeof(addr), *addr_ptr) < 0) return 0;

    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid = conn_info->pid;

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        if (bpf_core_read_user(&addr_in, sizeof(addr_in), *addr_ptr) < 0) return 0;

        u32 ip = addr_in.sin_addr.s_addr;
        u16 port = bpf_ntohs(addr_in.sin_port);
        if (port == 0) return 0;

        info.ddstIP.s_addr = ip;
        info.dport = port;
        info.family = AF_INET;

    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        if (bpf_core_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr) < 0) return 0;

        u16 port = bpf_ntohs(addr_in6.sin6_port);
        info.family = AF_INET6;
        info.dport = port;
        __builtin_memcpy(&info.ddstIP6, &addr_in6.sin6_addr.in6_u.u6_addr16, sizeof(info.ddstIP6));
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}


