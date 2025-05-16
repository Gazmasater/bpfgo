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


Если ты контролируешь vmlinux.h, можешь проверить, как определены поля:


grep saddr vmlinux.h
Если там:


__u8 saddr[4];
— то всё выше верно.

Если:


__be32 saddr;
— то дело в конфликте типов при генерации, и нужно обновить или переопределить vmlinux.h.


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ grep saddr vmlinux.h
                        __be32 skc_rcv_saddr;
        struct in6_addr skc_v6_rcv_saddr;
                __be32 saddr;
                const struct in6_addr *saddr;
                        __be32 saddr;
                        __be32 saddr[4];
        union cdrom_addr cdsc_absaddr;
                        __be32 saddr;
                        __be32 saddr;
                struct in_addr in_saddr;
                struct in6_addr in6_saddr;
        __u8 saddr[6];
        __u8 saddr[6];
        const struct in6_addr *saddr;
        __be32 nh_saddr;
        int nh_saddr_genid;
        __be32 saddr;
        struct in6_addr saddr;
        __be32 saddr;
        struct in6_addr saddr;
        struct in6_addr v6_rcv_saddr;
        struct in6_addr fast_v6_rcv_saddr;
        __be32 fast_rcv_saddr;
        __be32 inet_saddr;
        struct in6_addr saddr;
        __be32 saddr;
        int (*ipv6_dev_get_saddr)(struct net *, const struct net_device *, const struct in6_addr *, unsigned int, struct in6_addr *);
        struct in6_addr saddr;
        const struct in6_addr *saddr_cache;
struct ipv6_saddr_dst {
struct ipv6_saddr_score {
                        struct in6_addr saddr;
                        struct in6_addr saddr;
        struct in6_addr saddr;
        __be32 saddr;
                        __be32 saddr;
                        struct in6_addr saddr;
        void (*get_saddr)(struct sctp_sock *, struct sctp_transport *, struct flowi *);
        void (*to_sk_saddr)(union sctp_addr *, struct sock *);
        union sctp_addr saddr;
        __be32 saddr;
        struct in6_addr saddr;
        struct inetpeer_addr tcpm_saddr;
        __u8 saddr[28];
        __u8 saddr[4];
        __u8 saddr_v6[16];
        __u8 saddr[4];
        __u8 saddr_v6[16];
        __u8 saddr[4];
        __u8 saddr_v6[16];
        __u8 saddr[4];
        __u8 saddr_v6[16];
        __u8 saddr[4];
        __u8 saddr_v6[16];
        __u8 saddr[28];
        __u8 saddr[28];
        __u8 saddr[4];
        __u8 saddr_v6[16];
        __u8 saddr[28];
        xfrm_address_t saddr;
        xfrm_address_t old_saddr;
        xfrm_address_t new_saddr;
        xfrm_address_t saddr;
        xfrm_address_t saddr;
        int (*get_saddr)(struct net *, int, xfrm_address_t *, xfrm_address_t *, u32);
                xfrm_address_t saddr;
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 







