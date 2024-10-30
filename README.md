SEC("tracepoint/sock/sock_create")
int BPF_PROG(sock_create, struct sock *sk) {
    struct event *tcp_info;

    tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
    if (!tcp_info) {
        return 0;
    }

    tcp_info->saddr = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
    tcp_info->daddr = BPF_CORE_READ(sk, __sk_common.skc_daddr);
    tcp_info->dport = BPF_CORE_READ(sk, __sk_common.skc_dport);
    tcp_info->sport = bpf_htons(BPF_CORE_READ(sk, __sk_common.skc_num));

    bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
    tcp_info->pid = bpf_get_current_pid_tgid() >> 32;

    bpf_ringbuf_submit(tcp_info, 0);

    return 0;
}

