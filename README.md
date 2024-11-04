#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include  "bpf/bpf_endian.h"

struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 port;
};

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/inet_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    
    // Получаем IP-адрес и порт из структуры sock
    u16 dport = sk->__sk_common.skc_dport;
    u32 dip = sk->__sk_common.skc_daddr;
    
    info.ip = dip;
    info.port = __bpf_ntohs(dport);
        //bpf_printk("Connection accepted: PID=%d, IP=%x, PORT=%d\n", info.pid, info.ip, info.port);
      bpf_printk("Connection accepted: PID=%d\n", pid);



    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecli run package.json
INFO [faerie::elf] strtab: 0x3c3 symtab 0x400 relocs 0x448 sh_offset 0x448
libbpf: Failed to bump RLIMIT_MEMLOCK (err = -1), you might need to do it explicitly!
libbpf: Error in bpf_object__probe_loading():Operation not permitted(1). Couldn't load trivial BPF program. Make sure your kernel supports BPF (CONFIG_BPF_SYSCALL=y) and/or that RLIMIT_MEMLOCK is set to big enough value.
libbpf: failed to load object 'fentry_bpf�p'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 1"), RecvError



    BebraAppimage
