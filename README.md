bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

SEC("fentry/tcp_connect")
int BPF_PROG(tcp_connect, struct sock *sk)
{
	struct event *tcp_info;

	if (sk->__sk_common.skc_family == AF_INET)
	{
		tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
		if (!tcp_info)
		{
			return 0;
		}

		tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info->daddr = sk->__sk_common.skc_daddr;
		tcp_info->dport = sk->__sk_common.skc_dport;
		tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
	}
	else if (sk->__sk_common.skc_family == AF_INET6)
	{
		// Обработка IPv6, если необходимо
		// Здесь добавьте аналогичную логику для IPv6
		return 0; // Здесь вы можете также вернуть данные для IPv6
	}

	bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
	tcp_info->pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

	bpf_ringbuf_submit(tcp_info, 0);

	return 0;
}

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//go:build ignore

#include "common.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_tracing.h"

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
SEC("kprobe/tcp_connect")
int BPF_KPROBE(tcp_connect, struct sock *sk) {
	struct event tcp_info = {};

	// Проверка, что IPv4 используется
	if (sk->__sk_common.skc_family == AF_INET) {
		tcp_info.saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info.daddr = sk->__sk_common.skc_daddr;
		tcp_info.dport = sk->__sk_common.skc_dport;
		tcp_info.sport = bpf_htons(sk->__sk_common.skc_num);
	} else {
		// IPv6 можно обработать аналогично
		return 0;
	}

	// Получение имени процесса и PID
	bpf_get_current_comm(&tcp_info.comm, TASK_COMM_LEN);
	tcp_info.pid = bpf_get_current_pid_tgid() >> 32;

	// Отправка события в пространство пользователя
	bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &tcp_info, sizeof(tcp_info));

	return 0;
}

