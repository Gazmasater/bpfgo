//go:build ignore

#include "common.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_tracing.h"
#include "linux/ptrace.h"

#define AF_INET 2
#define AF_INET6 10
#define TASK_COMM_LEN 16

char __license[] SEC("license") = "Dual MIT/GPL";

// Структура sock_common для IPv4/IPv6
struct sock_common {
	union {
		struct {
			__be32 skc_daddr;
			__be32 skc_rcv_saddr;
		};
	};
	union {
		__u32 _;
	};
	union {
		struct {
			__be16 skc_dport;
			__u16 skc_num;
		};
	};
	short unsigned int skc_family;
};

// Структура sock
struct sock {
	struct sock_common __sk_common;
};

// Определение карты для отправки событий через perf
struct {
	__uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
} events SEC(".maps");

// Структура события с PID
struct event {
	u8 comm[16];
	__u16 sport;
	__be16 dport;
	__be32 saddr;
	__be32 daddr;
	__u32 pid;
};
// Основной обработчик, использующий kprobe для совместимости со старыми ядрами
SEC("kprobe/__x64_sys_accept")
int bpf_prob(struct pt_regs *ctx,struct sock *sk) {
	  struct event tcp_info = {};

	// // Проверка, что IPv4 используется
	 if (sk->__sk_common.skc_family == AF_INET) {
	 	tcp_info.saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info.daddr = sk->__sk_common.skc_daddr;
	 	tcp_info.dport = sk->__sk_common.skc_dport;
	 	tcp_info.sport = bpf_htons(sk->__sk_common.skc_num);
	 } else {
	// // IPv6 можно обработать аналогично
	  	return 0;
	}

	// Получение имени процесса и PID
	 bpf_get_current_comm(&tcp_info.comm, TASK_COMM_LEN);
	 tcp_info.pid = bpf_get_current_pid_tgid() >> 32;

	// Отправка события в пространство пользователя
	bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &tcp_info, sizeof(tcp_info));

	return 0;
}