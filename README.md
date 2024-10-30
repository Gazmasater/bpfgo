SEC("fentry/tcp_connect") 
int BPF_PROG(tcp_connect, struct sock *sk) { struct event *tcp_info;

tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
if (!tcp_info) {
    return 0;
}

tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info->daddr = sk->__sk_common.skc_daddr;
		tcp_info->dport = sk->__sk_common.skc_dport;
		tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
tcp_info->pid = bpf_get_current_pid_tgid() >> 32;

bpf_ringbuf_submit(tcp_info, 0);

return 0;

}
