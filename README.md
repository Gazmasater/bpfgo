nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("xdp")
int xdp_prog(struct __sk_buff *skb) {
    void *data_end = (void *)(long)skb->data_end;
    void *data = (void *)(long)skb->data;
    
    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    if (eth->h_proto != __constant_htons(ETH_P_IP))
        return XDP_PASS;

    struct iphdr *ip = (struct iphdr *)(eth + 1);
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    if (ip->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp = (struct tcphdr *)(ip + 1);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;

        bpf_printk("TCP: Src IP=%x, Src Port=%d, Dst IP=%x, Dst Port=%d\n",
            ip->saddr, ntohs(tcp->source), ip->daddr, ntohs(tcp->dest));
    }

    if (ip->protocol == IPPROTO_UDP) {
        struct udphdr *udp = (struct udphdr *)(ip + 1);
        if ((void *)(udp + 1) > data_end)
            return XDP_PASS;

        bpf_printk("UDP: Src IP=%x, Src Port=%d, Dst IP=%x, Dst Port=%d\n",
            ip->saddr, ntohs(udp->source), ip->daddr, ntohs(udp->dest));
    }

    return XDP_PASS;
}




      |                    ^     ~~~~~~~~~~~~~~~~~~~~~~~

