SEC("kprobe/udp_sendmsg")
int kprobe_udp_sendmsg(struct pt_regs *ctx)
{
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    struct msghdr *msg = (struct msghdr *)PT_REGS_PARM2(ctx);

    struct trace_info info = {};
    info.pid = bpf_get_current_pid_tgid() >> 32;
    info.proto = IPPROTO_UDP;
    info.sysexit = 0;
    info.state = 0;
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    __u16 family = 0;
    BPF_CORE_READ_INTO(&family, sk, __sk_common.skc_family);
    info.family = family;

    if (family == AF_INET) {
        BPF_CORE_READ_INTO(&info.srcIP.s_addr, sk, __sk_common.skc_rcv_saddr);
        BPF_CORE_READ_INTO(&info.sport, sk, __sk_common.skc_num);

        struct sockaddr_in addr = {};
        bpf_probe_read_user(&addr, sizeof(addr), msg->msg_name);
        info.dport = bpf_ntohs(addr.sin_port);
        info.dstIP.s_addr = addr.sin_addr.s_addr;
    } else if (family == AF_INET6) {
        BPF_CORE_READ_INTO(&info.srcIP6, sk, __sk_common.skc_v6_rcv_saddr);
        BPF_CORE_READ_INTO(&info.sport, sk, __sk_common.skc_num);

        struct sockaddr_in6 addr6 = {};
        bpf_probe_read_user(&addr6, sizeof(addr6), msg->msg_name);
        info.dport = bpf_ntohs(addr6.sin6_port);
        __builtin_memcpy(&info.dstIP6, &addr6.sin6_addr, sizeof(struct in6_addr));
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    return 0;
}
