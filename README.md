#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/socket.h>

char LICENSE[] SEC("license") = "Dual BSD/GPL";

struct conn_info_t {
    u32 pid;
    char comm[16];
};

struct trace_info {
    u32 pid;
    u32 family;
    u16 dport;
    union {
        struct {
            u32 saddr;
            u32 daddr;
        } ddstIP;
        struct in6_addr dstIP6;
    };
    char comm[16];
    int sysexit;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);
    __type(value, struct conn_info_t);
    __uint(max_entries, 1024);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);
    __type(value, const struct sockaddr *);
    __uint(max_entries, 1024);
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
} trace_events SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    const struct sockaddr *addr;
    bpf_core_read(&addr, sizeof(addr), &ctx->args[4]);
    if (!addr)
        return 0;

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

    const struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr || !*addr_ptr) return 0;

    struct trace_info info = {};
    struct sockaddr addr = {};

    if (bpf_core_read(&addr, sizeof(addr), *addr_ptr) < 0) return 0;

    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid = conn_info->pid;

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        if (bpf_core_read(&addr_in, sizeof(addr_in), *addr_ptr) < 0) return 0;

        u32 ip = addr_in.sin_addr.s_addr;
        u16 port = bpf_ntohs(addr_in.sin_port);
        if (port == 0) return 0;

        info.ddstIP.saddr = 0; // можно заполнить src IP, если нужно
        info.ddstIP.daddr = ip;
        info.dport = port;
        info.family = AF_INET;
    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        if (bpf_core_read(&addr_in6, sizeof(addr_in6), *addr_ptr) < 0) return 0;

        u16 port = bpf_ntohs(addr_in6.sin6_port);
        info.dport = port;
        info.family = AF_INET6;
        __builtin_memcpy(&info.dstIP6, &addr_in6.sin6_addr, sizeof(info.dstIP6));
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}





