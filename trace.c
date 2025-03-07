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
    u32 comm_hash;
    u16 sport;
    u16 dport;
    char comm[64];
};

struct sockaddr_info_t {
    struct sockaddr_in local_addr;  // Локальный адрес
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

struct  sys_enter_bind_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    char pad1[1];
    int common_pid;
    int __syscall_nr;
    int fd;
    int addrlen;
    struct sockaddr *umyaddr;
};

struct  sys_exit_bind_args {

    unsigned short common_type;    
    unsigned char common_flags;  
    unsigned char common_preempt_count; 
    char pad1[1];   
    int common_pid;  

    int __syscall_nr; 
    long ret;

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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  
    __type(value, struct sockaddr_in);
} bind_map SEC(".maps");


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
#define AF_INET6 10

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr *addr = (struct sockaddr *)ctx->upeer_sockaddr;  

    

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

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

    bpf_printk("sys_exit_accept4 PID=%d  FAMILY=%d ",conn_info->pid,addr.sa_family);



    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

      //  bpf_printk("sys_exit_accept4 PID=%d  FAMILY=%d PORT=%d Comm=%s",conn_info->pid,addr.sa_family,port,conn_info->comm);

        struct sockaddr_in *addr_in_b = bpf_map_lookup_elem(&bind_map, &pid);
        if (!addr_in_b) {
            bpf_printk("PID=%d not found in bind_map\n", pid);
            return 0;
        }
    
        u32 ip_b = bpf_ntohl(addr_in_b->sin_addr.s_addr);
        u16 port_b = bpf_ntohs(addr_in_b->sin_port);
    
        struct trace_info info = {};
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid = pid;
        info.src_ip=ip;
        info.sport = port;
        info.dst_ip=ip_b;
        info.dport=port_b;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&bind_map, &pid);



    return 0;

}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    int fd=ctx->fd;

    conn_info.pid = pid;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    
    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr;  
    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

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

    bpf_printk("sys_exit_connect PID=%d  FAMILY=%d ",conn_info->pid,addr.sa_family);


    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};

        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);


       // bpf_printk("sys_exit_connect FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        // struct trace_info info = {};
        // info.pid = pid;
        // __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        // info.pid=conn_info->pid;
        // info.dst_ip=ip;
        // info.dport = port;
       

      //  bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
       
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

    bpf_printk("sys_exit_sendto FAMILY=%d Comm=%s", addr.sa_family, conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid = conn_info->pid;
        info.dst_ip = ip;
        info.dport = port;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr);

        u8 *ip6 = (u8 *) addr_in6.sin6_addr.in6_u.u6_addr32;
        u16 port = bpf_ntohs(addr_in6.sin6_port);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid = conn_info->pid;
        // Для IPv6 записываем в info.dst_ip первые 4 байта IPv6 (например, для логирования)
        info.dst_ip = *(u32 *)ip6;  // Можно сделать и более детальное представление IP
        info.dport = port;

       // bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        // Логирование для IPv6
        bpf_printk("IPv6 detected: port %d, address %x:%x:%x:%x:%x:%x:%x:%x\n",
            port,
            ip6[0], ip6[1], ip6[2], ip6[3], ip6[4], ip6[5], ip6[6], ip6[7]
        );
    }

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

  //  bpf_printk("sys_exit_recvfrom FAMILY=%d Comm=%s",addr.sa_family,conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        struct sockaddr_in *addr_in_b = bpf_map_lookup_elem(&bind_map, &pid);
        if (!addr_in_b) {
            bpf_printk("PID=%d not found in bind_map\n", pid);
            return 0;
        }
    
        u32 ip_b = bpf_ntohl(addr_in_b->sin_addr.s_addr);
        u16 port_b = bpf_ntohs(addr_in_b->sin_port);



       // bpf_printk("sys_exit_recvfrom FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));


        info.src_ip=ip;
        info.sport = port;
        info.dst_ip=ip_b;
        info.dport=port_b;


         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
 
    return 0;

}

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct sys_enter_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;  

    struct conn_info_t conn_info = {};
    conn_info.pid=pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr *addr = (struct sockaddr *)ctx->umyaddr;  

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
  
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct sys_exit_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t conn_info = {};
    conn_info.pid=pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));


    if (ret < 0) {
        bpf_printk("sys_exit_bind failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    struct sockaddr_in addr_in = {};
    bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

    u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

    u16 port = bpf_ntohs(addr_in.sin_port);

    bpf_map_update_elem(&bind_map, &pid, &addr_in, BPF_ANY);


    bpf_printk("sys_exit_bind  PID=%d Comm=%s IP=%d.%d.%d.%d PORT=%d ",
        conn_info.pid,
        conn_info.comm,
        (ip>>24)&0xff,
        (ip>>16)&0xff,
        (ip>>8)&0xff,
        (ip)&0xff,
         port);  
   
  
    return 0;
}

// SEC("sockops")
// int bpf_sockops_prog(struct bpf_sock_ops *skops) {
//     // Проверяем, что соединение установлено
//     if (skops->op == BPF_SOCK_OPS_STATE_CB && skops->args[1] == BPF_TCP_ESTABLISHED) {
//         bpf_printk("TCP connection established: local port=%d, remote port=%d\n",
//                    skops->local_port, skops->remote_port);
//     }
//     return 0;
// }






