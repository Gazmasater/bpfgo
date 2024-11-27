#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t
{
	u32 pid;
	u32 src_ip;
	u32 dst_ip;

	u16 sport;
	u16 dport;
	char comm[16];
	struct sockaddr *sock_addr;
	u32 addrlen;
};



struct sys_enter_accept4_args
{
	unsigned short common_type; //2
	unsigned char common_flags;//1
	unsigned char common_preempt_count;//1
	int common_pid;//4
	int __syscall_nr;//4
	int fd;//4
	int __padding;//4

	struct sockaddr *upeer_sockaddr;//8
	int *upeer_addrlen;//8
	int flags;//4
};

struct sys_exit_accept4_args
{
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long ret;
};


struct
{
	__uint(type, BPF_MAP_TYPE_PERCPU_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_accept SEC(".maps");

struct
{
	__uint(type, BPF_MAP_TYPE_PERCPU_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_accept_four SEC(".maps");


char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2


static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;
    bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
    return 0;
}



static __always_inline int init_conn_info_accept4(struct sys_enter_accept4_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_accept_four, bpf_get_current_pid_tgid() >> 32);
}




SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_bind_ret(struct sys_enter_accept4_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_accept4(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
	if (conn_info)
	{
		
	//	bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);

	}

	return 0;
}


SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	long ret = ctx->ret;

	if (ret < 0)
	{
		bpf_printk("EXIT_accept Accept4 failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
	if (!conn_info)
	{
		bpf_printk("EXIT_accept4 No connection info found for PID=%d\n", pid);
		return 0;
	}

	u16 family;
    if (bpf_probe_read(&family, sizeof(family), conn_info->sock_addr) != 0) {
        bpf_printk("EXIT_accept4 Failed to read address family for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
        return 0;
    }

	struct sockaddr_in6 addr;

	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("EXIT_accept4 Failed to read sockaddr for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_accept_four, &pid);

		return 0;
	}

	bpf_printk("EXIT_ACCEPT4 PROTOCOL=%d",addr.sin6_family);



	return 0;
}

