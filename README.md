

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("sk_lookup")
int echo_dispatch(struct bpf_sk_lookup *ctx)
{
    __u8 proto ; 


bpf_probe_read_user(&proto,sizeof(proto),&ctx->protocol);
     __u32 srcIP=bpf_ntohl(ctx->local_ip4);
     __u16 srcPort=bpf_ntohs(ctx->local_port);


    const char *proto_str;
    if (proto == IPPROTO_TCP) {
        proto_str = "TCP";
    } else if (proto == IPPROTO_UDP) {
        proto_str = "UDP";
    } else {
        proto_str = "UNKNOWN";
    }

 bpf_printk("sk_lookup srcIP=%d.%d.%d.%d    srcPORT=%d: Protocol: %d\n", 
    (srcIP>>24)&0xff,
    (srcIP>>16)&0xff,
    (srcIP>>8)&0xff,
    (srcIP)&0xff,
     srcPort,
     proto);
    return SK_PASS;
}



























