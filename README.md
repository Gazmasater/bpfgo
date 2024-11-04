SEC("fentry/inet_accept")
int BPF_PROG(inet_accept, struct sock *sk)
{
	struct event tcp_info = {}; // Инициализируем структуру

	if (sk->__sk_common.skc_family == AF_INET)
	{
		tcp_info.daddr = sk->__sk_common.skc_daddr;
		tcp_info.dport = sk->__sk_common.skc_dport;
		tcp_info.sport = bpf_htons(sk->__sk_common.skc_num);
		tcp_info.saddr = sk->__sk_common.skc_rcv_saddr; // Добавлено для получения адреса отправителя
	}

	bpf_get_current_comm(&tcp_info.comm, TASK_COMM_LEN);
	tcp_info.pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

	bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &tcp_info, sizeof(tcp_info));

	return 0;
}
И
