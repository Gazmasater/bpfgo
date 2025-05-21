
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

        info.srcIP6[0]=(ctx->local_ip6[0]);
        info.srcIP6[1]=(ctx->local_ip6[1]);
        info.srcIP6[2]=(ctx->local_ip6[2]);
        info.srcIP6[3]=(ctx->local_ip6[3]);

        info.dstIP6[0]=(ctx->remote_ip6[0]);
        info.dstIP6[1]=(ctx->remote_ip6[1]);
        info.dstIP6[2]=(ctx->remote_ip6[2]);
        info.dstIP6[3]=(ctx->remote_ip6[3]);

        info.sport = ctx->local_port;
        info.dport = bpf_ntohs(ctx->remote_port);
        info.proto=ctx->protocol;


bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    return SK_PASS;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
2025/05/21 03:49:09 failed to load bpf objects: field LookUp: program look_up: load program: invalid argument: cannot pass map_type 27 into func bpf_perf_event_output#25 (62 line(s) omitted)
