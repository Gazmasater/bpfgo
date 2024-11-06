#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 sport;
    u16 dport;
};

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/inet_csk_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    
    // Чтение IP-адреса и порта с помощью bpf_core_read
    u16 dport;
    u32 dip;
    u32 portpair;

    if (bpf_core_read(&dport, sizeof(dport), &sk->__sk_common.skc_daddr) != 0) {
        bpf_printk("Failed to read port\n");
        return 0;
    }
    if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) != 0) {
        bpf_printk("Failed to read IP\n");
        return 0;
    }


     if  ( bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair)!=0) {

        bpf_printk("Failed to read pairport\n");
        return 0;

     }

        u16 local_port = portpair >> 16;  // Старшие 16 бит — это исходный порт
        u16 remote_port = portpair & 0xFFFF;  // Младшие 16 бит — это целевой порт

    bpf_printk("Portpair: 0x%x Local port: %u Remote port: %u", 
               portpair, local_port, remote_port);


    info.ip = dip;  // Преобразование IP из сетевого порядка
    info.sport=local_port;
    info.dport=remote_port;

    bpf_printk("Connection accepted: PID=%d", info.pid);
    bpf_printk("IP=%u.%u.%u.%u", 
               (info.ip >> 0) & 0xFF, 
               (info.ip >> 8) & 0xFF, 
               (info.ip >> 16) & 0xFF, 
               (info.ip >> 24) & 0xFF);


    return 0;
}

