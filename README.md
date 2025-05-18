//go:build ignore
#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <bpf/bpf_core_read.h>

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u32 fd;
    u32 sport;
    u16 dport;
    u16 family;
    u8  proto;
    char comm[64];
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
    __type(value, struct sockaddr *);
} addrRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128);
} trace_events SEC(".maps");

struct trace_info {
    struct in_addr srcIP;
    struct in_addr dstIP;
    __u32        srcIP6[4];
    __u32        dstIP6[4];
    __u32        pid;
    __u32        proto;
    __u16        sysexit;
    __u16        sport;
    __u16        dport;
    __u16        state;
    __u16        family;
    char         comm[32];
};

char LICENSE[] SEC("license") = "Dual BSD/GPL";

#define AF_INET   2
#define AF_INET6 10

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    const struct sockaddr *addr = (const struct sockaddr *)ctx->args[4];
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
    if (!conn_info)
        return 0;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr)
        return 0;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid     = conn_info->pid;

    /* Читаем сначала семейство адреса */
    __u16 family;
    if (BPF_CORE_READ_USER_INTO(&family, *addr_ptr, sa_family) < 0)
        goto cleanup;

    if (family == AF_INET) {
        __u32 ip;
        __u16 port;

        if (BPF_CORE_READ_USER_INTO(&ip,   *addr_ptr, sin_addr.s_addr) < 0 ||
            BPF_CORE_READ_USER_INTO(&port, *addr_ptr, sin_port)         < 0)
            goto cleanup;

        port = bpf_ntohs(port);
        if (port == 0)
            goto cleanup;

        info.dstIP.s_addr = ip;
        info.dport        = port;
        info.family       = AF_INET;

    } else if (family == AF_INET6) {
        __u16 port6;

        if (BPF_CORE_READ_USER_INTO(&port6, *addr_ptr, sin6_port) < 0)
            goto cleanup;

        port6 = bpf_ntohs(port6);
        if (port6 == 0)
            goto cleanup;

        info.family = AF_INET6;
        info.dport  = port6;

        /* Сразу копируем весь массив из четырёх u32 */
        if (BPF_CORE_READ_USER_INTO(&info.dstIP6, *addr_ptr, sin6_addr.in6_u.u6_addr32) < 0)
            goto cleanup;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

