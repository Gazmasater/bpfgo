//go:build ignore

#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

struct conn_info_t
{
    struct sockaddr sock_addr;
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[64];
};

struct event_t {
    u32 pid;
    u32 src_ip;
    u16 sport;
    u16 pad;
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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");



struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(__u32));
    __uint(value_size, sizeof(__u32));
   // __uint(max_entries, 128); 
} events SEC(".maps");


char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    
    void *addr_ptr = (void *)ctx->addr;    
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        return 0;
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)&conn_info->sock_addr;
    if (addr->sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr->sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr->sin_port);
    }

    bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
        conn_info->pid, conn_info->comm,
        (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
        (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

   struct event_t event = {
    .pid = conn_info->pid,
    .src_ip = conn_info->src_ip,
    .sport = conn_info->sport,
  };

    __builtin_memcpy(event.comm, conn_info->comm, sizeof(event.comm));
    bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &event, sizeof(event));


        return 0;
}