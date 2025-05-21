
SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct trace_info info = {};

    __u32 proto = ctx->protocol;
    info.sysexit = 3;
    info.family=ctx->family;



    if (ctx->family == AF_INET) {
        struct in_addr srcIP={};
        struct in_addr dstIP={};
    

        info.srcIP.s_addr = ctx->local_ip4;
        info.sport = ctx->local_port;
        info.dstIP.s_addr = ctx->remote_ip4;
        info.dport = bpf_ntohs(ctx->remote_port);

        info.proto = proto;

       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (ctx->family == AF_INET6) {

        info.srcIP6=ctx->local_ip6;
        info.srcIP6=ctx->local_ip6;
        info.srcIP6=ctx->local_ip6;
        info.srcIP6=ctx->local_ip6;



