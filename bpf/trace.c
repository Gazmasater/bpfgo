// go:build ignore

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
    //      ^^^^^^ 16 мало. сделай 64 хотя бы
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
        void * buff;     
        size_t len;      
        unsigned int flags;       
        struct sockaddr * addr;   
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


struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");
//^^^^^^^^^^^^^^^ зачем такие сложные названия? можно же просто назвать conn_info_map


struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    //                  ^^^^^^ 1024?
    __type(key, u32);
    __type(value, int);
} addr_map SEC(".maps");
//^^^^^^ название странное! по сути оно хранит sockaddr, ну так и назови типа sock_addr_map

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

// TODO trace_event_raw_sys_enter и sys_enter_sendto выглядят странно. Лучше задать самописные структуры из описания точки и примеров cilium
SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr_in addr;
    void *addr_ptr = (void *)ctx->addr;

    bpf_probe_read(&addr, sizeof(addr), addr_ptr);
    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    //  ^^^^^^^^^^^^^^^^^^^^^^^^^^^ Зачем использовать дополнительную мапу для хранения addr? можно ведь все хранить в одной conn_info_map_sc
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
    {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in *addr = bpf_map_lookup_elem(&addr_map, &pid);
    //      ^^^^^^^^^^ в мапе же в sys_enter_sendto сохраняешь тип sockaddr, а тут используешь sockaddr_in
    if (addr && addr->sin_family == AF_INET)
    {
        __be32 ip_addr = 0;
        __be16 port=addr->sin_port;

        ip_addr = BPF_CORE_READ(addr, sin_addr.s_addr); // TODO BPF_CORE_READ тут лишнее
      //  port = BPF_CORE_READ(addr, sin_port);           // TODO BPF_CORE_READ тут лишнее

        conn_info->src_ip = bpf_ntohl(ip_addr);
        conn_info->sport = bpf_ntohs(port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

        bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);
    }

    // TODO вот тут должна быть отправка через perf_event_output conn_info в юзерспейс
    return 0;
}