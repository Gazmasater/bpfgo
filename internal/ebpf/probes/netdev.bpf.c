static __always_inline __u64 inode_cookie_from_sock(struct sock *sk)
{
    if (!sk) return 0;

    struct socket *sock = BPF_CORE_READ(sk, sk_socket);
    if (!sock) return 0;

    struct file *file = BPF_CORE_READ(sock, file);
    if (!file) return 0;

    struct inode *ino = BPF_CORE_READ(file, f_inode);
    if (!ino) return 0;

    return (__u64)BPF_CORE_READ(ino, i_ino);
}



SEC("tracepoint/net/net_dev_queue")
int trace_net_dev_queue(struct tp_net_dev_queue *ctx)
{
    struct sk_buff *skb = (struct sk_buff *)(unsigned long)ctx->skbaddr;
    if (!skb) return 0;


    struct sock *sk = BPF_CORE_READ(skb, sk);
    if (!sk) return 0;

__u64 cookie = inode_cookie_from_sock(sk);
if (!cookie) return 0;


     
    // family from sock
    __u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);

    struct trace_info e = {};
    e.event  = EV_SKB_OUT;
    e.cookie = cookie;
    e.family = family;

    void *head = (void *)BPF_CORE_READ(skb, head);
    __u16 nh   = BPF_CORE_READ(skb, network_header);
    __u16 th   = BPF_CORE_READ(skb, transport_header);

    if (family == AF_INET) {
        struct iphdr iph = {};
        if (!head) return 0;
        bpf_probe_read_kernel(&iph, sizeof(iph), head + nh);
        if (iph.version != 4) return 0;

        e.proto   = iph.protocol;
        e.src_ip4 = iph.saddr; // network order
        e.dst_ip4 = iph.daddr; // network order

        if (e.proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e.sport = bpf_ntohs(uh.source);
            e.dport = bpf_ntohs(uh.dest);
        } else if (e.proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e.sport = bpf_ntohs(thh.source);
            e.dport = bpf_ntohs(thh.dest);
        }
    } else if (family == AF_INET6) {
        struct ipv6hdr ip6h = {};
        if (!head) return 0;
        bpf_probe_read_kernel(&ip6h, sizeof(ip6h), head + nh);

        e.proto = ip6h.nexthdr;
        __builtin_memcpy(e.src_ip6, &ip6h.saddr, sizeof(e.src_ip6));
        __builtin_memcpy(e.dst_ip6, &ip6h.daddr, sizeof(e.dst_ip6));


        if (e.proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e.sport = bpf_ntohs(uh.source);
            e.dport = bpf_ntohs(uh.dest);
        } else if (e.proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e.sport = bpf_ntohs(thh.source);
            e.dport = bpf_ntohs(thh.dest);
        }
    } else {
        return 0;
    }

    // we only care TCP/UDP/ICMP(v6) like in user space
    // (ICMP has no ports; you can still emit src/dst if you want)
    if (e.proto != IPPROTO_TCP && e.proto != IPPROTO_UDP && e.proto != IPPROTO_ICMP && e.proto != IPPROTO_ICMPV6) {
        return 0;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &e, sizeof(e));
    return 0;
}
