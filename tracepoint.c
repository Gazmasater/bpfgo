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
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long fd;
	struct sockaddr *upeer_sockaddr;
	int *upeer_addrlen;
	int flags;
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

struct sys_enter_bind_args
{

	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long fd;
	struct sockaddr *umyaddr;
	int addrlen;
};

struct sys_exit_bind_args
{

	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long ret;
};

struct sys_enter_connect_args
{

	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long fd;
	struct sockaddr *uservaddr;
	int addrlen;
	//char padding[4];
};

struct sys_exit_connect_args
{

unsigned short common_type;      
unsigned char common_flags;      
unsigned char common_preempt_count;    
int common_pid; 
int __syscall_nr; 
int ret; 

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
} conn_info_map_bind SEC(".maps");

struct
{
	__uint(type, BPF_MAP_TYPE_PERCPU_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_connect SEC(".maps");

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


static __always_inline int init_conn_info_accept(struct sys_enter_accept4_args *ctx)
{

	u32 pid = bpf_get_current_pid_tgid() >> 32;

	struct conn_info_t conn_info = {};
	conn_info.pid = pid;
	bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
	conn_info.sock_addr = (struct sockaddr *)ctx->upeer_sockaddr;
	bpf_map_update_elem(&conn_info_map_accept, &pid, &conn_info, BPF_ANY);
	return 0;
}

static __always_inline int init_conn_info_bind(struct sys_enter_bind_args *ctx)
{
	u64 current_pid_tgid = bpf_get_current_pid_tgid();
	u32 pid = current_pid_tgid >> 32;

	struct conn_info_t conn_info = {};
	conn_info.pid = pid;
	bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
	conn_info.sock_addr = (struct sockaddr *)ctx->umyaddr;
	bpf_map_update_elem(&conn_info_map_bind, &pid, &conn_info, BPF_ANY);
	return 0;
}

// static __always_inline int init_conn_info_connect(struct sys_enter_connect_args *ctx)
// {
// 	u32 pid = bpf_get_current_pid_tgid() >> 32;
// 	struct conn_info_t conn_info = {};
// 	conn_info.pid = pid;
// 	bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
// 	conn_info.sock_addr = (struct sockaddr *)ctx->uservaddr;
// 	bpf_map_update_elem(&conn_info_map_connect, &pid, &conn_info, BPF_ANY);
// 	return 0;
// }

static __always_inline int init_conn_info_connect(struct sys_enter_connect_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map_connect, bpf_get_current_pid_tgid() >> 32);
}


SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_bind_ret(struct sys_enter_accept4_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_accept(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
	if (conn_info&&( conn_info->comm!="dockerd"))
	{
//		bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);
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
		bpf_map_delete_elem(&conn_info_map_accept, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
	if (!conn_info)
	{
		bpf_printk("EXIT_accept No connection info found for PID=%d\n", pid);
		return 0;
	}

	bpf_printk("EXIT sockaddr=%p",conn_info->sock_addr);


	struct sockaddr_in addr;

	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("EXIT_accept Failed to read sockaddr for PID=%d\n", pid);
		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->sport = bpf_ntohs(addr.sin_port);

		bpf_printk("EXIT_accept Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
				   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
	}

	return 0;
}

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct sys_enter_bind_args *ctx)
{
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_bind(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
	if (conn_info)
	{
//		bpf_printk("SERVER Bind entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
	}

	return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct sys_exit_bind_args *ctx)
{
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	long ret = ctx->ret;

	if (ret < 0)
	{
		bpf_printk("EXIT_bind  failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_bind, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
	if (!conn_info)
	{
		bpf_printk("EXIT_bind No connection info found for PID=%d\n", pid);
		return 0;
	}

	struct sockaddr_in addr;

	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("EXIT_bind Failed to read sockaddr for PID=%d\n", pid);
		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->dport = bpf_ntohs(addr.sin_port);

		bpf_printk("EXIT_bind SERVER  connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
				   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
	}

	return 0;
}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx)
{
	u32 pid = bpf_get_current_pid_tgid() >> 32;

	init_conn_info_connect(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_connect, &pid);
	if (conn_info&&( conn_info->comm!="dockerd"))
	{
		bpf_printk("enter_connect CLIENT Connect entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
	}

	return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx)
{
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	int ret = ctx->ret;


	if (ret < 0)
	{
		bpf_printk("exit_connect Connect failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_connect, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_connect, &pid);
	if (!conn_info)
	{
		bpf_printk("exit_connect No connection info found for PID=%d\n", pid);
		return 0;
	}

	struct sockaddr_in addr;
	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("exit_connect Failed to read sockaddr for PID=%d\n", pid);
		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->dport = bpf_ntohs(addr.sin_port);

		bpf_printk("exit_connect CLIENT Connected to server: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
				   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
	}

	return 0;
}
