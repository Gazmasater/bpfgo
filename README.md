bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

BPF_PROG_TYPE_FLOW_DISSECTOR — анализирует содержимое пакета, чтобы извлечь такие данные, как IP-адрес или порт, что помогает в маршрутизации и фильтрации.

nc -l -p 12345
nc 127.0.0.1 12345


SEC("kprobe/inet_accept")
int trace_accept(struct pt_regs *ctx)

    bpf_printk("Family: %d\n", sk->skc_family);

    	struct event tcp_info = {};
event.Pid = binary.LittleEndian.Uint32(record.RawSample[28:32]) // Извлечение Pid
		//event.Dport = binary.LittleEndian.Uint16(record.RawSample[34:36]) // Извлечение dport
		//event.Sport = binary.LittleEndian.Uint16(record.RawSample[32:34]) // Извлечение sport
		event.Sport = binary.BigEndian.Uint16(record.RawSample[32:34])
		event.Dport = binary.BigEndian.Uint16(record.RawSample[34:36])s


record := Record{
		RawSample: []byte{0x00, 0x50, 0x01, 0xBB}, // Пример данных для sport и dport
	}

	var event Event
	event.Sport = binary.BigEndian.Uint16(record.RawSample[0:2]) // Извлечение source port
	event.Dport = binary.BigEndian.Uint16(record.RawSample[2:4]) // Извлечение destination port

	fmt.Printf("Source Port: %d, Destination Port: %d\n", event.Sport, event.Dport)



+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SEC("fentry/inet_accept")
int BPF_PROG(inet_accept, struct socket *sock, struct sockaddr *address, int *address_len)
{
    struct event *tcp_info;
    struct sock *sk = sock->sk; // Получаем структуру sock

    tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
    if (!tcp_info)
    {
        return 0;
    }

    if (sk->__sk_common.skc_family == AF_INET)
    {
        tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
        tcp_info->daddr = sk->__sk_common.skc_daddr;
        tcp_info->dport = sk->__sk_common.skc_dport; // Это может быть не инициализировано
        tcp_info->sport = bpf_htons(sk->__sk_common.skc_num); // Проверьте, правильно ли заполняется skc_num

        // Получение информации о порте из sockaddr
        struct sockaddr_in *addr = (struct sockaddr_in *)address;
        tcp_info->dport = addr->sin_port; // Устанавливаем порт назначения
    }
    else if (sk->__sk_common.skc_family == AF_INET6)
    {
        // Обработка для IPv6
        return 0;
    }

    bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
    tcp_info->pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

    bpf_ringbuf_submit(tcp_info, 0);

    return 0;
}
