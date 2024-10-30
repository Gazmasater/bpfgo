#include <linux/bpf.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/bpf_helpers.h>
#include <linux/ip.h>
#include <linux/ptrace.h>

SEC("fentry/__x64_sys_recvfrom")
int BPF_PROG(recvfrom_hook, int sockfd, void __user *buf, size_t len, unsigned int flags, struct sockaddr __user *src_addr, int __user *addrlen) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)src_addr;

    if (addr_in) {
        bpf_printk("UDP packet received from %pI4:%u\n", &addr_in->sin_addr, ntohs(addr_in->sin_port));
    }

    return 0; // возвращаем 0 для продолжения выполнения функции
}

char _license[] SEC("license") = "GPL";



bpftool btf dump file /sys/kernel/btf/vmlinux | grep sys_accept

#include <linux/bpf.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/bpf_helpers.h>

SEC("fentry/__x64_sys_accept4") 
int BPF_PROG(accept_hook, int sockfd, struct sockaddr __user *addr, int __user *addrlen, int flags) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;

    // Проверяем, что указатель адреса валиден
    if (addr_in) {
        bpf_printk("New TCP connection accepted from %pI4:%u\n", &addr_in->sin_addr, ntohs(addr_in->sin_port));
    }

    return 0; // возвращаем 0 для продолжения выполнения функции
}

char _license[] SEC("license") = "GPL";



#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);    // UID процесса
    __type(value, u32);  // ID сокета
    __uint(max_entries, 1024);
} socket_map SEC(".maps");

SEC("fentry/__sys_socket")
int handle_socket_creation(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    u32 socket_type = PT_REGS_PARM1(ctx);  // Тип сокета, например SOCK_STREAM
    u32 protocol = PT_REGS_PARM3(ctx);     // Протокол, например IPPROTO_TCP

    // Логика фильтрации: захватываем только TCP-сокеты
    if (protocol == IPPROTO_TCP) {
        bpf_map_update_elem(&socket_map, &pid, &socket_type, BPF_ANY);
    }






    // go:build ignore

#include "common.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_tracing.h"

#define AF_INET 2
#define AF_INET6 10 // Для поддержки IPv6

#define TASK_COMM_LEN 16

char __license[] SEC("license") = "Dual MIT/GPL";

// Структура sock_common для поддержки IPv6
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

// Структура sock
struct sock
{
	struct sock_common __sk_common;
};

// Кольцевой буфер для событий
struct
{
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 1 << 24);
} events SEC(".maps");

// Структура события с полем для PID
struct event
{
	u8 comm[TASK_COMM_LEN];
	__u16 sport;
	__be16 dport;
	__be32 saddr;
	__be32 daddr;
	__u32 pid;
};
struct event *unused __attribute__((unused));

// Хук для отслеживания создания сокетов
SEC("fentry/__sys_socket")
int BPF_PROG(handle_socket_creation, struct pt_regs *ctx)
{
	struct event *tcp_info;
	struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);

	if (sk->__sk_common.skc_family == AF_INET)
	{
		tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
		if (!tcp_info)
		{
			return 0;
		}

		// Заполнение структуры event для IPv4
		tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info->daddr = sk->__sk_common.skc_daddr;
		tcp_info->dport = sk->__sk_common.skc_dport;
		tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
	}
	else if (sk->__sk_common.skc_family == AF_INET6)
	{
		// Обработка IPv6
		return 0;
	}

	// Получение имени процесса и PID
	bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
	tcp_info->pid = bpf_get_current_pid_tgid() >> 32;

	// Отправка события в кольцевой буфер
	bpf_ringbuf_submit(tcp_info, 0);

	return 0;
}


    return 0;
}

char _license[] SEC("license") = "GPL";


