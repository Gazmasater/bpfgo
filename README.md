SEC("tracepoint/sock/sock_create")
int BPF_PROG(sock_create, struct sock *sk)
{
	struct event *tcp_info;

	//if (sk->__sk_common.skc_family == AF_INET)
	//{
		tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
		if (!tcp_info)
		{
			return 0;
		}

		tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info->daddr = sk->__sk_common.skc_daddr;
		tcp_info->dport = sk->__sk_common.skc_dport;
		tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
	//}
	// else if (sk->__sk_common.skc_family == AF_INET6)
	// {
	// 	// Обработка IPv6, если необходимо
	// 	// Здесь добавьте аналогичную логику для IPv6
	// 	return 0; // Здесь вы можете также вернуть данные для IPv6
	// }

	bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
	tcp_info->pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

	bpf_ringbuf_submit(tcp_info, 0);

	return 0;
}


SEC("tracepoint/sock/sock_create")
int BPF_PROG(sock_create, struct sock *sk)
{
	struct event *tcp_info;

	//if (sk->__sk_common.skc_family == AF_INET)
	//{
		tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
		if (!tcp_info)
		{
			return 0;
		}

		tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info->daddr = sk->__sk_common.skc_daddr;
		tcp_info->dport = sk->__sk_common.skc_dport;
		tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
	//}
	// else if (sk->__sk_common.skc_family == AF_INET6)
	// {
	// 	// Обработка IPv6, если необходимо
	// 	// Здесь добавьте аналогичную логику для IPv6
	// 	return 0; // Здесь вы можете также вернуть данные для IPv6
	// }

	bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
	tcp_info->pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

	bpf_ringbuf_submit(tcp_info, 0);

	return 0;
}
