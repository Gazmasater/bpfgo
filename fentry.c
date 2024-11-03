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
      bpf_printk("Connection accepted: PID=%d\n", info.pid);
      //bpf_printk("Connection accepted: PID=%d, IP=%x\n", info.pid, info.ip);



    return 0;
}
