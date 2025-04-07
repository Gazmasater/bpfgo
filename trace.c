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
    u32 sport;
    u16 dport;
    u16 family;
    u8  proto;

    char comm[64];
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

struct sys_enter_sendmsg {

    unsigned short common_type;      
    unsigned char common_flags;      
    unsigned char common_preempt_count;     
    int common_pid;   

    int __syscall_nr; 
    int fd;
    int  pad;   
    struct user_msghdr * msg; 
    unsigned int flags;      


};

struct sys_exit_sendmsg {

    unsigned short common_type;       
    unsigned char common_flags;    
    unsigned char common_preempt_count;    
    int common_pid;   

    int __syscall_nr; 
    int pad;
    long ret; 


};

struct sys_enter_recvmsg_args {
        unsigned short common_type;      
        unsigned char common_flags;      
        unsigned char common_preempt_count;     
        int common_pid;   

        int __syscall_nr; 
        int fd;
        int  pad;   
        struct user_msghdr * msg; 
        unsigned int flags;      

};


struct sys_exit_recvmsg_args{

        unsigned short common_type;       
        unsigned char common_flags;    
        unsigned char common_preempt_count;    
        int common_pid;   

        int __syscall_nr; 
        int pad;
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
    __type(value, struct sockaddr);
} addr_map SEC(".maps");


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

struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); 
} trace_events SEC(".maps");

struct trace_info {
    u32 src_ip;
    u32 dst_ip;
    u32 sport;
    u32 pid;
    u32 proto;
    u32  sysexit;
    u32 fd;
    u32 state;
    u16 family;
    __u32 srcIP6[4];  // IPv6-адрес состоит из 8 блоков по 16 бит
    __u32 dstIP6[4];
   
    u16 dport;
   
    char comm[64];

};

// Размещение переменной с атрибутом unused
const struct trace_info *unused __attribute__((unused));
#define AF_INET 2
#define AF_INET6 10
#define TCP_ESTABLISHED 1
#define TCP_SYN_SENT 2
#define INET6_ADDRSTRLEN 46
#define ETH_P_IPV6 0x86DD

#define ETH_P_IP_BE   0x0008   // htons(0x0800) -> big-endian IP proto
#define IPPROTO_UDP   17       // Протокол UDP в IP заголовке
#define BLOCK_PORT    53       // Порт, который блокируем (например, DNS)

#define TC_OK         0        // Пропустить пакет
#define TC_SHOT       2        // Отбросить пакет






char LICENSE[] SEC("license") = "Dual BSD/GPL";

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

    struct trace_info info = {};

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
            bpf_printk("!!lookup sys_exit_sendto FAMILY=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s ",
                addr.sa_family,
                (ip>>24)&0xff,
                (ip>>16)&0xff,
                (ip>>8)&0xff,
                (ip)&0xff,
                port,
                conn_info->comm);


                info.pid = pid;
                __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
        
                info.pid=conn_info->pid;
                info.dst_ip=ip;
                info.dport = port;
                info.family=AF_INET;  
                info.sysexit=1;
                      
                bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
           
    } else if (addr.sa_family==AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr);

        __u32 ip6[4];

        info.dstIP6[0] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[0]);
        info.dstIP6[1] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[4]);
        info.dstIP6[2] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[8]);
        info.dstIP6[3] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[12]);
        u16 port = bpf_ntohs(addr_in6.sin6_port);
        info.dport=port;
        
        info.pid=pid;
        info.sysexit=1;
        info.family=AF_INET6;
        info.dport=port;
        
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&addrSend_map, &pid);  
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

    bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);


    return 0;
}


// SEC("tracepoint/syscalls/sys_exit_recvfrom")
// int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = ctx->ret;

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
//     if (!conn_info) return 0;

//     if (ret < 0) {
//         bpf_printk("sys_exit_recvfrom failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map, &pid);
//         return 0;
//     }

//     struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
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
               
//         struct trace_info info = {};
//         info.pid = pid;
//         __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));


//             info.src_ip=ip;
//             info.sport = port;
//             info.family=AF_INET;
//             info.dst_ip=0;
//             info.dport=0;
//             info.sysexit=2;
//             info.pid=pid;

//          bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        
//     } else if (addr.sa_family==AF_INET6) {

//         struct trace_info info = {};

//         struct sockaddr_in6 addr_in6 = {};

//         info.sysexit=2;
//         info.family=AF_INET6;

//         info.dstIP6[0] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[0]);
//         info.dstIP6[1] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[1]);
//         info.dstIP6[2] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[2]);
//         info.dstIP6[3] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.in6_u.u6_addr8[3]);

//         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


//     }

//     bpf_map_delete_elem(&addrRecv_map, &pid);
//     bpf_map_delete_elem(&conn_info_map, &pid);
//     bpf_map_delete_elem(&addr_map, &pid);

 
//     return 0;

// }


SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct sys_enter_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct msghdr *addr = (struct msghdr *)ctx->msg;  

    bpf_printk("sys_enter_recvmsg addr=%p",addr);
    bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct sys_exit_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

  //  bpf_printk("sys_exit_recvmsg pid=%d ret=%ld", pid, ret);

    // Проверяем, есть ли данные для PID
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No conn_info for pid=%d", pid);
        return 0;
    }

    if (ret < 0) {
        bpf_printk("recvmsg failed for PID=%d", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }



    // Получаем указатель на msghdr
    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr) {
        bpf_printk("No addr_ptr for pid=%d", pid);
        return 0;
    }



    struct msghdr *msg;
    bpf_probe_read_user(&msg, sizeof(msg), *addr_ptr);

    if (!msg) {
        bpf_printk("msg is NULL for pid=%d", pid);
        return 0;
    }

    struct sockaddr_in sa = {};
    struct sockaddr_in6 sa6 = {};
    struct trace_info info = {};

    bpf_probe_read_user(&sa, sizeof(sa), &msg->msg_name);
    bpf_probe_read_user(&sa6, sizeof(sa6), &msg->msg_name);


    if (sa.sin_family==AF_INET) {

     u32   port=bpf_ntohs(sa.sin_port);
     u32   ip=bpf_ntohl(sa.sin_addr.s_addr);
     info.pid=conn_info->pid;
     info.dst_ip=ip;
     info.dport = port;
     info.family=AF_INET;  
     info.sysexit=2;
           
     bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    // bpf_printk("sys_exit_recvmsg name_ptr FAMIY=%d IP=%d:%d:%d:%d:%d",sa.sin_family,
    //     (ip>>24)&0xff,
    //     (ip>>16)&0xff,
    //     (ip>>8)&0xff,
    //     ip&0xff,   
    //     port);
    } else if (sa.sin_family==AF_INET6) {

        u32 port=bpf_ntohs(sa6.sin6_port);
        

        if (port==0) {
            return 0;
        }

        bpf_printk("sys_exit_recvmsg IP6 PORT=%d",port);



        info.sysexit=2;
        info.family=AF_INET6;
        info.dport=port;
        info.pid=pid;

        info.dstIP6[0] = bpf_ntohl(*(__u32 *)&sa6.sin6_addr.in6_u.u6_addr8[0]);
        info.dstIP6[1] = bpf_ntohl(*(__u32 *)&sa6.sin6_addr.in6_u.u6_addr8[1]);
        info.dstIP6[2] = bpf_ntohl(*(__u32 *)&sa6.sin6_addr.in6_u.u6_addr8[2]);
        info.dstIP6[3] = bpf_ntohl(*(__u32 *)&sa6.sin6_addr.in6_u.u6_addr8[3]);
         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        // bpf_printk("sys_exit_recvmsg IP6=%d:%d:%d:%d",
        //     info.dstIP6[0],
        //     info.dstIP6[1],
        //     info.dstIP6[2],
        //     info.dstIP6[3]);
    }

    return 0;
}



SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct trace_info info = {};
    __u32 proto = ctx->protocol;

    if (ctx->family == AF_INET) {
        __u32 srcIP = bpf_ntohl(ctx->local_ip4);
        __u32 dstIP = bpf_ntohl(ctx->remote_ip4);
        __u32 srcPort = ctx->local_port;
        __u16 dstPort = bpf_ntohs(ctx->remote_port);

        info.src_ip = srcIP;
        info.sport = srcPort;
        info.dst_ip = dstIP;
        info.dport = dstPort;
        info.sysexit = 3;
        info.proto = proto;
        info.family=AF_INET;


        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        bpf_printk("IPv4 lookup src=%d.%d.%d.%d:%d dst=%d.%d.%d.%d:%d protocol=%d FAMILY=%d \n", 
            (srcIP >> 24) & 0xff, (srcIP >> 16) & 0xff, (srcIP >> 8) & 0xff, srcIP & 0xff,
            srcPort,
            (dstIP >> 24) & 0xff, (dstIP >> 16) & 0xff, (dstIP >> 8) & 0xff, dstIP & 0xff,
            dstPort,
            proto, ctx->family);
    } else if (ctx->family == AF_INET6) {

        info.srcIP6[0]=bpf_ntohl(ctx->local_ip6[0]);
        info.srcIP6[1]=bpf_ntohl(ctx->local_ip6[1]);
        info.srcIP6[2]=bpf_ntohl(ctx->local_ip6[2]);
        info.srcIP6[3]=bpf_ntohl(ctx->local_ip6[3]);

        info.dstIP6[0]=bpf_ntohl(ctx->remote_ip6[0]);
        info.dstIP6[1]=bpf_ntohl(ctx->remote_ip6[1]);
        info.dstIP6[2]=bpf_ntohl(ctx->remote_ip6[2]);
        info.dstIP6[3]=bpf_ntohl(ctx->remote_ip6[3]);

        info.sport = ctx->local_port;
        info.dport = bpf_ntohs(ctx->remote_port);
        info.family=AF_INET6;
        info.sysexit = 3;
        info.proto=ctx->protocol;


        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

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

    if (ctx->family==AF_INET) {
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

    } else if (ctx->family==AF_INET6) {

        bpf_printk("IPV6!!!!!!!inet_sock_set_state");


    }

    return 0;
}

