struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 16); // 64KB буфер
} events SEC(".maps");

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;
    if (ret < 0) return 0;

    struct sockaddr_in addr = {};
    if (ctx->addr) {
        bpf_probe_read(&addr, sizeof(addr), ctx->addr);
    }

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.src_ip = bpf_ntohl(addr.sin_addr.s_addr);
    conn_info.sport = bpf_ntohs(addr.sin_port);

    bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &conn_info, sizeof(conn_info));
    return 0;
}

