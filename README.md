//go:build ignore

#include "common.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_tracing.h"

#define AF_INET 2
#define AF_INET6 10 // Для поддержки IPv6

#define TASK_COMM_LEN 16

char __license[] SEC("license") = "Dual MIT/GPL";

// Структура sock_common, дополненная для поддержки IPv6
struct sock_common
{
	union
	{
		struct
		{
			__be32 skc_daddr;
			__be32 skc_rcv_saddr;
		};
	};
	union
	{
		// Padding out union skc_hash.
		__u32 _;
	};
	union
	{
		struct
		{
			__be16 skc_dport;
			__u16 skc_num;
		};
	};
	short unsigned int skc_family;
};

// Структура sock отражает начало структуры sock из ядра
struct sock
{
	struct sock_common __sk_common;
};

struct
{
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 1 << 24);
} events SEC(".maps");

// Структура события, дополненная для хранения PID
struct event
{
	u8 comm[16];
	__u16 sport;
	__be16 dport;
	__be32 saddr;
	__be32 daddr;
	__u32 pid; // Добавлено поле для PID
};
struct event *unused __attribute__((unused));

SEC("KProbe/sys_accept")
int BPF_PROG(sys_accept, struct sock *sk)
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

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ go build && sudo ./ebpf-test
[sudo] password for gaz358: 
2024/10/30 19:35:23 loading objects: field SysAccept: program sys_accept: load program: permission denied: 8: (61) r1 = *(u32 *)(r7 +4): R7 invalid mem access 'scalar' (13 line(s) omitted)
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ^C
