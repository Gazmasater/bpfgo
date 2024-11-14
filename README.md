#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
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
} conn_info_map_accept SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_c SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2


struct sys_enter_accept4_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
    struct sockaddr *upeer_sockaddr;
    int *upeer_addrlen;
    int flags;
};

// struct sys_exit_accept4_args {
//     unsigned short common_type;
//     unsigned char common_flags;
//     unsigned char common_preempt_count;
//     int common_pid;
//     int __syscall_nr;
//     long ret
// };




SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept_entry(struct sys_enter_accept4_args *ctx){
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid=pid;
    conn_info.sock_addr = ctx->upeer_sockaddr;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid); 
    if (conn_info) 
    { bpf_printk("CLIENT accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm); }
    
    return 0;
}


[{
	"resource": "/home/gaz358/myprog/bpfgo/fentry.c",
	"owner": "C/C++: IntelliSense",
	"code": "41",
	"severity": 8,
	"message": "expression must have arithmetic or pointer type",
	"source": "C/C++",
	"startLineNumber": 69,
	"startColumn": 9,
	"endLineNumber": 69,
	"endColumn": 18
}]
