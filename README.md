SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_est(struct trace_event_raw_inet_sock_set_state *ctx) {
    struct trace_info info = {};

    __u32 pid_tcp = bpf_get_current_pid_tgid() >> 32;
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    if (ctx->family == AF_INET) {
        if (ctx->newstate == TCP_ESTABLISHED || ctx->newstate == TCP_SYN_SENT || ctx->newstate == TCP_LISTEN) {
            info.srcIP.s_addr = ctx->saddr;
            info.dstIP.s_addr = ctx->daddr;
            info.sport = ctx->sport;
            info.dport = ctx->dport;
            info.sysexit = 6;
            info.proto = ctx->protocol;
            info.pid = pid_tcp;
            info.state = ctx->newstate;
            info.family = AF_INET;

            bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
        }
    } else if (ctx->family == AF_INET6) {
        info.sysexit = 6;
        info.family = AF_INET6;
        info.pid = pid_tcp;
        info.proto = ctx->protocol;
        info.state = ctx->newstate;
        info.sport = ctx->sport;
        info.dport = ctx->dport;

        // IPv6 — здесь действительно нужно использовать bpf_core_read
        bpf_core_read(&info.ddstIP6, sizeof(info.ddstIP6), &ctx->daddr_v6);
        bpf_core_read(&info.ssrcIP6, sizeof(info.ssrcIP6), &ctx->saddr_v6);

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    return 0;
}







