//go:build ignore

#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

struct conn_info_t
{
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[64];
};


struct sys_enter_accept4_args
{
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	int fd;
	int pad1;
	struct sockaddr *upeer_sockaddr;
	int *upeer_addrlen;
	int flags;
};

struct sys_exit_accept4_args
{
	unsigned short common_type;//2
	unsigned char common_flags;//1
	unsigned char common_preempt_count;//1
	int common_pid;//4
	int __syscall_nr;//4
	int pad1;
	long ret;//8
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


struct sys_enter_sendto_args
{
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
    int pad1;
    void *buff;
    size_t len;
    unsigned int flags;
    struct sockaddr *addr;
    int addr_len;
};

struct sys_exit_sendto_args
{
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    long ret;
};

struct sys_enter_recvfrom_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    int fd;
    void *ubuf;
    size_t size;
    unsigned int flags;
    struct sockaddr *addr;
    int *addr_len;
};

struct sys_exit_recvfrom_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    int ret;
};


struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr *);
} addr_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");


struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); 
} trace_events SEC(".maps");



struct trace_info {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u16 sport;
    u16 dport;
    char comm[64];

};

// Размещение переменной с атрибутом unused
const struct trace_info *unused __attribute__((unused));


char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_printk("sys_enter_accept4 Comm=%s  PID=%d\n",conn_info.comm,conn_info.pid);

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct sockaddr *addr = (struct sockaddr *)ctx->upeer_sockaddr;  

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);


    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("EXIT_accept Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("sys_exit_accept4 FAMILY=%d Comm=%s",addr.sa_family,conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);


        bpf_printk("sys_exit_accept4  FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid = pid;
        info.src_ip=ip;
        info.sport = port;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;

}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr;  

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);


    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_connect failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("sys_exit_connect FAMILY=%d Comm=%s",addr.sa_family,conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);


        bpf_printk("sys_exit_connect FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.dst_ip=ip;
        info.dport = port;


        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;

}

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct sockaddr *addr = (struct sockaddr *)ctx->addr;  

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_sendto failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("sys_exit_sendto FAMILY=%d Comm=%s",addr.sa_family,conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);


        bpf_printk("sys_exit_sendto FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.dst_ip=ip;
        info.dport = port;


        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;

}

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct sys_enter_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct sockaddr *addr = (struct sockaddr *)ctx->addr;  

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_recvfrom failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("sys_exit_recvfrom FAMILY=%d Comm=%s",addr.sa_family,conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);


        bpf_printk("sys_exit_recvfrom FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));


        info.src_ip=ip;
        info.sport = port;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
 
    return 0;

}




