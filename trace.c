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
    u32 fd;
    u16 sport;
    u16 dport;
    u8  proto;

    char comm[64];
};



struct sys_enter_getsockname_args {

    unsigned short common_type;      
    unsigned char common_flags;    
    unsigned char common_preempt_count;  
    int common_pid;  
    int __syscall_nr;
    int fd;  
    struct sockaddr * usockaddr;     
    int * usockaddr_len;    
    

};


struct sys_exit_getsockname_args {

    unsigned short common_type;       
    unsigned char common_flags;      
    unsigned char common_preempt_count;     
    int common_pid;  
    int __syscall_nr;
    int pad1;
    long ret; 

};



struct sys_enter_socket_args {


    unsigned short common_type;    
    unsigned char common_flags;       
    unsigned char common_preempt_count;    
    int common_pid;  
    int __syscall_nr;
    int family;     
    int type;
    int protocol;    


};

struct sys_exit_socket_args {


    unsigned short common_type;       
    unsigned char common_flags;    
    unsigned char common_preempt_count; 
    int common_pid;   
    int __syscall_nr;
    int pad;
    long ret; 

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
    __type(value, struct sockaddr);
} addr_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr);
} addrBind_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr);
} addrConnect_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr);
} addrSend_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr);
} addrRecv_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr);
} addrSockName_map SEC(".maps");








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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  
    __type(value, struct sockaddr_in6);
} bind6_map SEC(".maps");


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
    u32 dport;
    u32 proto;
    u32  sysexit;
    u32 fd;
    u32 state;
    char comm[64];

};





// Размещение переменной с атрибутом unused
const struct trace_info *unused __attribute__((unused));
#define AF_INET 2
#define AF_INET6 10
#define TCP_ESTABLISHED 1
#define TCP_SYN_SENT 2



char LICENSE[] SEC("license") = "Dual BSD/GPL";

// SEC("tracepoint/syscalls/sys_enter_accept4")
// int trace_accept4_enter(struct sys_enter_accept4_args *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     struct conn_info_t conn_info = {};

//     conn_info.pid = pid;
//     bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

//     struct sockaddr *addr = (struct sockaddr *)ctx->upeer_sockaddr;  

    

//     bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

//     bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);


//     return 0;
// }

// SEC("tracepoint/syscalls/sys_exit_accept4")
// int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = ctx->ret;

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
//     if (!conn_info) return 0;

//     if (ret < 0) {
//         bpf_printk("EXIT_accept Accept4 failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map, &pid);
//         return 0;
//     }

//     struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
//     if (!addr_ptr) {
//         return 0;
//     }

//     struct sockaddr addr = {};
//     bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  


//     if (addr.sa_family == AF_INET) {
//         struct sockaddr_in addr_in = {};
//         bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

//         u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

//         u16 port = bpf_ntohs(addr_in.sin_port);

//         struct ip_port_key key = {
//             .ip = ip,
//             .port = port,
//         };



//         bpf_printk("!!!lookup sys_exit_accept4 FAMILY=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s ",
//             addr.sa_family,
//             (key.ip>>24)&0xff,
//             (key.ip>>16)&0xff,
//             (key.ip>>8)&0xff,
//             (key.ip)&0xff,
//             key.port,
//             conn_info->comm);



//         struct trace_info info = {};
//         __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

//         info.pid = pid;
//         info.src_ip=ip;
//         info.sport = port;

//         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
//     }

//     bpf_map_delete_elem(&addr_map, &pid);  
//     bpf_map_delete_elem(&conn_info_map, &pid);



//     return 0;

// }

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    conn_info.fd=ctx->fd;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
  
    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr; 

    
    bpf_printk("sys_enter_connect PID=%d FD=%d",conn_info.pid,conn_info.fd);


    bpf_map_update_elem(&addrConnect_map, &pid, &addr, BPF_ANY);

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

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrConnect_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

   

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};

        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port==0) {

            return 0;
        }

        
        struct trace_info info = {};

        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid=conn_info->pid;
        info.dst_ip=ip;
        info.dport = port;
        info.sysexit=4;
        info.fd=conn_info->fd;
        
    
        
        bpf_printk("!!!lookup sys_exit_connect FAMILY=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s FD=%d",
            addr.sa_family,
            (ip>>24)&0xff,
            (ip>>16)&0xff,
            (ip>>8)&0xff,
            (ip)&0xff,
            port,
            conn_info->comm,
            conn_info->fd
        );

       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
       
    }

    bpf_map_delete_elem(&addrConnect_map, &pid);  
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

    bpf_map_update_elem(&addrSend_map, &pid, &addr, BPF_ANY);


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

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  


    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port==0) {

            return 0;
        }

      

       


        bpf_printk("!!! lookup sys_exit_sendto FAMILY=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s ",
            addr.sa_family,
            (ip>>24)&0xff,
            (ip>>16)&0xff,
            (ip>>8)&0xff,
            (ip)&0xff,
            port,
            conn_info->comm);


      



      //  bpf_printk("sys_exit_sendto FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid=conn_info->pid;
        info.dst_ip=ip;
        info.dport = port;
        
        info.sysexit=1;
       

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
    }

    bpf_map_delete_elem(&addr_map, &pid);  
  //  bpf_map_delete_elem(&conn_info_map, &pid);

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

    bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);


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

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  


    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

       


        bpf_printk("#####lookup sys_exit_recvfrom ADDRESS=%d.%d.%d.%d:%d Comm=%s ",
            (ip>>24)&0xff,
            (ip>>16)&0xff,
            (ip>>8)&0xff,
            (ip)&0xff,
            port,
            conn_info->comm);




      // bpf_printk("#######lookup sys_exit_recvfrom FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));


        info.src_ip=ip;
        info.sport = port;
        info.dst_ip=0;
        info.dport=0;
        info.sysexit=2;
        info.pid=pid;
      



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
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr *addr = (struct sockaddr *)ctx->umyaddr;  

    bpf_map_update_elem(&addrBind_map, &pid, &addr, BPF_ANY);
  
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct sys_exit_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    if (ret < 0) {
        bpf_printk("sys_exit_bind failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrBind_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  



    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port==0) {

            return 0;
        }
        


        struct trace_info info = {};
        info.pid = pid;
        info.src_ip=ip;
        info.sport = port;
        info.sysexit=5;
      



         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


        bpf_map_update_elem(&bind_map, &pid, &addr_in, BPF_ANY);

        bpf_printk("sys_exit_bind FAMILY=%d PID=%d Comm=%s IP=%d.%d.%d.%d PORT=%d\n",
            addr.sa_family,
            conn_info.pid,
            conn_info.comm,
            (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8) & 0xff,
            (ip) & 0xff,
            port);  
    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr);

        u16 port = bpf_ntohs(addr_in6.sin6_port);

        bpf_map_update_elem(&bind6_map, &pid, &addr_in6, BPF_ANY);

        bpf_printk("sys_exit_bind IP6 FAMILY=%d PID=%d Comm=%s PORT=%d\n",
            addr.sa_family,
            conn_info.pid,
            conn_info.comm,
            port);  
    }

  
    return 0;
}


SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    __u32 proto = ctx->protocol;
    __u32 srcIP = bpf_ntohl(ctx->local_ip4);
    __u32 dstIP = bpf_ntohl(ctx->remote_ip4);
    __u32 srcPort = ctx->local_port;
    __u16 dstPort = bpf_ntohs(ctx->remote_port);

    

    struct trace_info info = {};

    info.src_ip=srcIP;
    info.sport=srcPort;
    info.dst_ip=dstIP;
    info.dport=dstPort;
    info.sysexit=3;
    info.proto=ctx->protocol;



    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    bpf_printk("lookup src=%d.%d.%d.%d:%d dst=%d.%d.%d.%d:%d protocol=%d FAMILY=%d \n", 
        (srcIP >> 24) & 0xff,
        (srcIP >> 16) & 0xff, 
        (srcIP >> 8) & 0xff, 
        srcIP & 0xff,
        srcPort,
        (dstIP >> 24) & 0xff, 
        (dstIP >> 16) & 0xff, 
        (dstIP >> 8) & 0xff,
        dstIP & 0xff,
        dstPort,
        proto,
        ctx->family        
    );
    
    return SK_PASS;
}



SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_est(struct trace_event_raw_inet_sock_set_state *ctx) {


    __u32 pid_tcp = bpf_get_current_pid_tgid() >> 32;

    __u32 srcip;
    bpf_probe_read_kernel(&srcip, sizeof(srcip), ctx->saddr);
    srcip = bpf_ntohl(srcip);
    
    __u32 dstip;
    bpf_probe_read_kernel(&dstip, sizeof(dstip), ctx->daddr);
    dstip = bpf_ntohl(dstip);
       
    __u16 sport=0;
    
    sport=ctx->sport;
       
    __u16 dport;
    dport=ctx->dport;

   __u8 state=ctx->newstate;

    
    if (ctx->newstate == TCP_ESTABLISHED||ctx->newstate == TCP_SYN_SENT||ctx->newstate==TCP_LISTEN) {

bpf_printk("inet_sock_set_state PID=%d srcip=%d.%d.%d.%d:%d   dstip=%d.%d.%d.%d:%d PROTO=%d ",
    
    pid_tcp,
    (srcip >> 24) & 0xff,
    (srcip >> 16) & 0xff,
    (srcip >> 8) & 0xff,
    (srcip) & 0xff,
    sport,

    (dstip >> 24) & 0xff,
    (dstip >> 16) & 0xff,
    (dstip >> 8) & 0xff,
    (dstip) & 0xff,
    dport,
    ctx->protocol

);

struct trace_info info = {};

info.src_ip=srcip;
info.sport=sport;
info.dst_ip=dstip;
info.dport=dport;
info.sysexit=6;
info.proto=ctx->protocol;
info.pid=pid_tcp;
info.state=ctx->newstate;
bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));





    }

    return 0;
}

