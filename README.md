#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

char LICENSE[] SEC("license") = "GPL";

struct conn_info_t {
    u32 pid;
    char comm[16];
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
    __type(value, struct sockaddr);
    __uint(max_entries, 1024);
} addrSend_map SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    // читаем ctx->args[4] — это аргумент "addr"
    const struct sockaddr *addr = (const struct sockaddr *)ctx->args[4];
    if (!addr)
        return 0;

    struct sockaddr sa = {};
    bpf_core_read(&sa, sizeof(sa), addr);

    bpf_map_update_elem(&addrSend_map, &pid, &sa, BPF_ANY);

    return 0;
}



