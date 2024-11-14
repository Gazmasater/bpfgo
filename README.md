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

// Функция для инициализации информации о соединении
static __always_inline int init_conn_info(u32 pid, struct sockaddr *sock_addr,struct conn_info_t *conn_info) {
    conn_info->pid = pid;
    bpf_get_current_comm(&conn_info->comm, sizeof(conn_info->comm));
    conn_info->sock_addr = sock_addr;
    return 0;
}

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



// Tracepoint для accept4
SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_entry(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sockaddr *sock_addr = (struct sockaddr *)upeer_sockaddr(ctx);
    struct conn_info_t conn_info = {};
    init_conn_info(pid, sock_addr, &conn_info);

    bpf_map_update_elem(&conn_info_map_ab, &pid, &conn_info, BPF_ANY);

    bpf_printk("CLIENT accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc fentry.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpJD98Bb/include", "-I/tmp/.tmpJD98Bb/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "fentry.c", "-o", "fentry.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] fentry.c:61:53: error: call to undeclared function 'upeer_sockaddr'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
   61 |     struct sockaddr *sock_addr = (struct sockaddr *)upeer_sockaddr(ctx);
      |                                                     ^
fentry.c:61:34: warning: cast to 'struct sockaddr *' from smaller integer type 'int' [-Wint-to-pointer-cast]
   61 |     struct sockaddr *sock_addr = (struct sockaddr *)upeer_sockaddr(ctx);
      |                                  ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
1 warning and 1 error generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
