#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include <linux/inet.h>
#include <linux/socket.h>

struct conn_info_t {
    struct sockaddr *sock_addr;
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    u8 call;
    u8 protocol;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 1024);
} events SEC(".maps");

struct sys_exit_sendto_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    long ret;
};

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        return 0;
    }

    struct conn_info_t conn_info = {};
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.pid = pid;

    // Отправляем данные через perf event
    bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &conn_info, sizeof(conn_info));
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
