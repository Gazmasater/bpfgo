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

SEC("kprobe/sys_accept") 
int syscall__probe_entry_accept(struct pt_regs *ctx, int sockfd, struct sockaddr *addr)
{ struct event *tcp_info;

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

[{
	"resource": "/home/gaz358/myprog/bpfgo/fentry.c",
	"owner": "C/C++: IntelliSense",
	"code": "20",
	"severity": 8,
	"message": "identifier \"sk\" is undefined",
	"source": "C/C++",
	"startLineNumber": 75,
	"startColumn": 19,
	"endLineNumber": 75,
	"endColumn": 21
}]
