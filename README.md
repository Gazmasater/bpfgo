bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

BPF_PROG_TYPE_FLOW_DISSECTOR — анализирует содержимое пакета, чтобы извлечь такие данные, как IP-адрес или порт, что помогает в маршрутизации и фильтрации.

nc -l -p 12345
nc 127.0.0.1 12345


SEC("kprobe/inet_accept")
int trace_accept(struct pt_regs *ctx)

    bpf_printk("Family: %d\n", sk->skc_family);

    	struct event tcp_info = {};




+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include  "bpf/bpf_endian.h"



struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 port;
};

// Создаем BPF-карту для хранения информации о соединениях
//BPF_HASH(conn_map, u32, struct conn_info_t);

// Привязываем функцию к kprobe для inet_accept
SEC("kprobe/inet_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    
    // Получаем IP-адрес и порт из структуры sock
    u16 dport = sk->__sk_common.skc_dport;
    u32 dip = sk->__sk_common.skc_daddr;
    
    info.ip = dip;
    info.port = __bpf_ntohs(dport);
  //bpf_printk("Connection accepted: PID=%d, IP=%x, PORT=%d\n", info.pid, info.ip, info.port);
   //   bpf_printk("Connection accepted: PID=%d\n", pid);
         bpf_printk("Connection accepted: PID=%d, IP=%x\n", info.pid, info.ip);

    // Сохраняем информацию о соединении в BPF-карте
    //conn_map.update(&pid, &info);

    return 0;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x8491 symtab 0x84d0 relocs 0x8518 sh_offset 0x8518
libbpf: prog 'trace_accept': BPF program load failed: Permission denied
libbpf: prog 'trace_accept': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
0: (79) r6 = *(u64 *)(r1 +112)        ; R1=ctx() R6_w=scalar()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
1: (85) call bpf_get_current_pid_tgid#14      ; R0_w=scalar()
; bpf_printk("Connection accepted: PID=%d, IP=%x\n", info.pid, info.ip);
2: (61) r4 = *(u32 *)(r6 +0)
R6 invalid mem access 'scalar'
processed 3 insns (limit 1000000) max_states_per_insn 0 total_states 0 peak_states 0 mark_read 0
-- END PROG LOAD LOG --
libbpf: prog 'trace_accept': failed to load: -13
libbpf: failed to load object 'fentry_bpf�b�s'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 13"), RecvError
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
 	 __u16 family; 
 
};
struct event *unused __attribute__((unused));

SEC("fentry/inet_accept")
int BPF_PROG(inet_accept, struct sock *sk){


	struct event *tcp_info;
	//struct event tcp_info = {};s


	if (sk->__sk_common.skc_family == AF_INET)
	{
		tcp_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
		if (!tcp_info)
		{
			return 0;
		}

		
  		tcp_info->family = sk->__sk_common.skc_family;  // Сохраняем семейство протоколов
    		tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
		tcp_info->daddr = sk->__sk_common.skc_daddr;
		tcp_info->dport = sk->__sk_common.skc_dport;
		tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
	}
	//  else if (sk->__sk_common.skc_family == AF_INET6)
	//   {
	
	//   	return 0; 
	//    }

	bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
	tcp_info->pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

	bpf_ringbuf_submit(tcp_info, 0);

	return 0;
}s
