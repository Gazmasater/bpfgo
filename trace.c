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


struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr*);
} addrSend_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr*);
} addrRecv_map SEC(".maps");



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

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
} ipv6_events SEC(".maps");

struct trace_info {
    // IPv4
    struct sockaddr_in ssrcIP;
    struct sockaddr_in ddstIP; 
    struct in_addr srcIP;
    struct in_addr dstIP;
    __u32 srcIP6[4];    
    __u32 dstIP6[4];   
    
    __u32 pid;
    __u32 proto;
    __u16 sysexit;
    __u16 sport;
    __u16 dport;
    __u16 state;
    __u16 family;
    char comm[32];
} ;



// Размещение переменной с атрибутом unused
const struct trace_info *unused __attribute__((unused));
#define AF_INET 2
#define AF_INET6 10
#define TCP_ESTABLISHED 1
#define IPPROTO_UDP   17     
#define IPPROTO_TCP    6
#define  BLOCK_PORT    53      

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    const struct sockaddr *addr = (const struct sockaddr *)ctx->args[4];
    if (!addr)
        return 0;

    bpf_map_update_elem(&addrSend_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr) return 0;

    struct trace_info info = {};
    struct sockaddr addr = {};

    if (bpf_core_read_user(&addr, sizeof(addr), *addr_ptr) < 0) return 0;

    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid = conn_info->pid;

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        if (bpf_core_read_user(&addr_in, sizeof(addr_in), *addr_ptr) < 0) return 0;

        u32 ip = addr_in.sin_addr.s_addr;
        u16 port = bpf_ntohs(addr_in.sin_port);
        if (port == 0) return 0;

        info.ddstIP.sin_addr.s_addr = ip;
        info.dport = port;
        info.family = AF_INET;

    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        if (bpf_core_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr) < 0) return 0;

        u16 port = bpf_ntohs(addr_in6.sin6_port);
        info.family = AF_INET6;
        info.dport = port;
        __builtin_memcpy(&info.dstIP6, &addr_in6.sin6_addr.in6_u.u6_addr32, sizeof(info.dstIP6));
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    if (!ctx->args[4]) return 0;


    struct sockaddr *addr = (struct sockaddr *)ctx->args[4];  

    bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx) {
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

    struct trace_info info = {};

    struct sockaddr addr = {};

    struct sockaddr_in addr_in = {};

   if (bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr)<0){
    return 0;
   }


   if (bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr)<0){
    return 0;
   }  



    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
        
       
    info.sysexit=2;


    if (addr.sa_family == AF_INET) {

        struct sockaddr_in addr_in = {};
       if (bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr)<0){
        return 0;
       }

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port== 0) {
        return 0;
        }
               
        info.pid = pid;

      

        info.ssrcIP.sin_addr.s_addr=ip;
        info.sport = port;
        info.family=AF_INET;           
        info.pid=pid;
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

                    
    } else if (addr.sa_family==AF_INET6) {



    struct sockaddr_in6 addr_in6 = {};

    if (bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr) < 0) {
        return 0;
    }



    u16 port6 = bpf_ntohs(addr_in6.sin6_port);
    info.family = AF_INET6;
    info.pid = conn_info->pid;
    info.sport = port6;


     __builtin_memcpy(&info.srcIP6, &addr_in6.sin6_addr.in6_u.u6_addr32, sizeof(info.srcIP6));
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }
    bpf_map_delete_elem(&addrRecv_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);


 
    return 0;

}

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct msghdr *addr = (struct msghdr *)ctx->args[1];  

    bpf_printk("sys_enter_sendmsg addr=%p",addr);
    bpf_map_update_elem(&addrSend_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;


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
    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
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
   
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit=11;


    bpf_probe_read_user(&sa, sizeof(sa), &msg->msg_name);
    bpf_probe_read_user(&sa6, sizeof(sa6), &msg->msg_name);

    if (sa.sin_family==AF_INET) {

     u32   port=bpf_ntohs(sa.sin_port);
     u32   ip=bpf_ntohl(sa.sin_addr.s_addr);
     info.pid=conn_info->pid;
     info.ddstIP.sin_addr.s_addr=ip;
     info.dport = port;
     info.family=AF_INET;  
     info.proto=conn_info->proto;
     bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    } else if (sa.sin_family==AF_INET6) {

        u32 port=bpf_ntohs(sa6.sin6_port);
        

        if (port==0) {
            return 0;
        }

        bpf_printk("sys_exit_recvmsg IP6 PORT=%d SYSEXIT=%d",port,info.sysexit);



        info.family=AF_INET6;
        info.dport=port;
        info.pid=pid;

        __builtin_memcpy(&info.dstIP6, &sa6.sin6_addr.in6_u.u6_addr32, sizeof(info.dstIP6));


         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct msghdr *addr = (struct msghdr *)ctx->args[1];  

    bpf_printk("sys_enter_recvmsg addr=%p",addr);
    bpf_map_update_elem(&addrRecv_map, &pid, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    

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
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));


    bpf_probe_read_user(&sa, sizeof(sa), &msg->msg_name);
    bpf_probe_read_user(&sa6, sizeof(sa6), &msg->msg_name);
    info.sysexit=12;



    if (sa.sin_family==AF_INET) {

     u32   port=bpf_ntohs(sa.sin_port);
     u32   ip=bpf_ntohl(sa.sin_addr.s_addr);
     info.pid=conn_info->pid;
     info.ssrcIP.sin_addr.s_addr=ip;
     info.sport = port;
     info.family=AF_INET;  
           
     bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    } else if (sa.sin_family==AF_INET6) {

        u32 port=bpf_ntohs(sa6.sin6_port);
        

        if (port==0) {
            return 0;
        }

        bpf_printk("sys_exit_recvmsg IP6 PORT=%d SYSEXIT=%d",port,info.sysexit);

        info.family=AF_INET6;
        info.sport=port;
        info.pid=pid;



        __builtin_memcpy(&info.srcIP6, &sa6.sin6_addr.in6_u.u6_addr32, sizeof(info.srcIP6));
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    bpf_map_delete_elem(&addrRecv_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}



SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct trace_info info = {};

    __u32 proto = ctx->protocol;

    if (ctx->family == AF_INET) {
        struct in_addr srcIP={};
        struct in_addr dstIP={};
    

        info.srcIP.s_addr = ctx->local_ip4;
        info.sport = ctx->local_port;
        info.dstIP.s_addr = ctx->remote_ip4;
        info.dport = bpf_ntohs(ctx->remote_port);

        info.sysexit = 3;
        info.proto = proto;
        info.family=AF_INET;

       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (ctx->family == AF_INET6) {

        info.srcIP6[0]=bpf_ntohl(ctx->local_ip6[0]);
        info.srcIP6[1]=bpf_ntohl(ctx->local_ip6[1]);
        info.srcIP6[2]=bpf_ntohl(ctx->local_ip6[2]);
        info.srcIP6[3]=bpf_ntohl(ctx->local_ip6[3]);

        info.dstIP6[0]=bpf_ntohl(ctx->remote_ip6[0]);
        info.dstIP6[1]=bpf_ntohl(ctx->remote_ip6[1]);
        info.dstIP6[2]=bpf_ntohl(ctx->remote_ip6[2]);
        info.dstIP6[3]=bpf_ntohl(ctx->remote_ip6[3]);

      //  bpf_core_read(&info.dstIP6, sizeof(info.dstIP6), ctx->remote_ip6);


        info.sport = ctx->local_port;
        info.dport = bpf_ntohs(ctx->remote_port);
        info.family=ctx->family;
        info.sysexit = 3;
        info.proto=ctx->protocol;


        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    return SK_PASS;
}

SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_est(struct trace_event_raw_inet_sock_set_state *ctx) {
    struct trace_info info = {};

    __u32 pid_tcp = bpf_get_current_pid_tgid() >> 32;
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    if (ctx->family == AF_INET) {
        if (ctx->newstate == TCP_ESTABLISHED ||
            ctx->newstate == TCP_SYN_SENT ||
            ctx->newstate == TCP_LISTEN) {

            __u32 ip_src = 0, ip_dst = 0;
            __builtin_memcpy(&ip_src, ctx->saddr, sizeof(ip_src));
            __builtin_memcpy(&ip_dst, ctx->daddr, sizeof(ip_dst));

            info.srcIP.s_addr = ip_src;
            info.dstIP.s_addr = ip_dst;
            info.sport = ctx->sport;
            info.dport = ctx->dport;
            info.sysexit = 6;
            info.proto = ctx->protocol;
            info.pid = pid_tcp;
            info.state = ctx->newstate;
            info.family = AF_INET;

            bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
        }
    } else if (ctx->family == AF_INET6) {
        info.sysexit = 6;
        info.family = AF_INET6;
        info.pid = pid_tcp;
        info.proto = ctx->protocol;
        info.state = ctx->newstate;
        info.sport = ctx->sport;
        info.dport = ctx->dport;

        bpf_core_read(&info.dstIP6, sizeof(info.dstIP6), &ctx->daddr_v6);
        bpf_core_read(&info.srcIP6, sizeof(info.srcIP6), &ctx->saddr_v6);

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    return 0;
}





















