lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    __u32 proto = ctx->protocol;
    __u32 dstIP = bpf_ntohl(ctx->local_ip4);
    __u32 srcIP = bpf_ntohl(ctx->remote_ip4);
    __u32 dstPort = ctx->local_port;
    __u16 srcPort = bpf_ntohs(ctx->remote_port);

    struct ip_port_key key = {
        .ip = dstIP,
        .port = dstPort,
    };

    struct ip_port_value value={

        .ip=srcIP,
        .port=srcPort,
        .protocol=proto,
        


    };

    bpf_map_update_elem(&sk_lookup_map, &key, &value, BPF_ANY);

    bpf_printk("sk_lookup src=%d.%d.%d.%d:%d dst=%d.%d.%d.%d:%d protocol=%d\n", 
        (srcIP >> 24) & 0xff, (srcIP >> 16) & 0xff, (srcIP >> 8) & 0xff, srcIP & 0xff, srcPort,
        (dstIP >> 24) & 0xff, (dstIP >> 16) & 0xff, (dstIP >> 8) & 0xff, dstIP & 0xff, dstPort,
        proto);
    
    return SK_PASS;
}




