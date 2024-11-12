#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct sockaddr *sock_addr;   
    u32 addrlen;  
};

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_ab SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_c SEC(".maps");



char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2


static __always_inline int init_conn_info(void *conn_map, u32 pid, struct pt_regs *ctx) {
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
    bpf_map_update_elem(conn_map, &pid, &conn_info, BPF_ANY);
    return 0;
}


int BPF_KPROBE(__sys_accept4)
{
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

bpf_printk("PID=%d",pid);

    init_conn_info(&conn_info_map_ab, pid, ctx);

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid); 
    return 0;
}

// int BPF_KRETPROBE(sys_accept4)
//  {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     int ret = PT_REGS_RC(ctx);

//     if (ret < 0) {
//     //    bpf_printk("ACCEPT4 failed: PID=%d, Error=%ld\n", pid, ret);
//         bpf_map_delete_elem(&conn_info_map_ab, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for ACCEPT4 PID=%d\n", pid);
//         return 0;
//     }

//     struct sockaddr_in addr;
//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("kprobe/__sys_accept4 Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     if (addr.sin_family == AF_INET) {
//         conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
//         conn_info->sport = bpf_ntohs(addr.sin_port);

//         bpf_printk("ACCEPTED connection: PID=%d, Comm=%s, Client IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
//             (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

        
//         // Successful connection: Add to map
//         bpf_map_update_elem(&conn_info_map_ab, &pid, conn_info, BPF_ANY);
//     }

//     return 0;
// }



// int BPF_KPROBE(__sys_bind)
//  {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;


//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid);

//     init_conn_info(&conn_info_map_ab, pid, ctx);


//         return 0;
// }

// int BPF_KRETPROBE(__sys_bind)
// {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     int ret = PT_REGS_RC(ctx);

//     if (ret < 0) {
//      //   bpf_printk("BIND failed: PID=%d, Error=%ld\n", pid, ret);
//         bpf_map_delete_elem(&conn_info_map_ab, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for BIND PID=%d\n", pid);
//         return 0;
//     }

//     struct sockaddr_in addr;
//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("kretprobe/__sys_bind Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     if (addr.sin_family == AF_INET) {
//         conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
//         conn_info->dport = bpf_ntohs(addr.sin_port);

//         bpf_printk("SERVER Bind: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
//             (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);

//         // Successful bind: Add to map
//         bpf_map_update_elem(&conn_info_map_ab, &pid, conn_info, BPF_ANY);
//     }

//     return 0;
// }

// int BPF_KPROBE(__sys_connect)
//  {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;

//     init_conn_info(&conn_info_map_c, pid, ctx);

//     return 0;
// }

// int BPF_KRETPROBE(__sys_connect)
//  {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
   
//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_c, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for CONNECT PID=%d\n", pid);
//         return 0;
//     }

//     struct sockaddr_in addr;
//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("kretprobe/__sys_connect Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     if (addr.sin_family == AF_INET) {
//         conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
//         conn_info->dport = bpf_ntohs(addr.sin_port);

//         bpf_printk("CLIENT Connected to server: PID=%d, Comm=%s, Server IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
//             (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
        
//         // Successful connection: Add to map
//         bpf_map_update_elem(&conn_info_map_c, &pid, conn_info, BPF_ANY);
//     }

//     return 0;
// }