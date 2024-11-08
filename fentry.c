
#include "vmlinux.h"
#include "stdbool.h"
#include "bpf/bpf_core_read.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_helpers.h"
#include "bpf/bpf_tracing.h"
#include "common_new.h"

struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct file *file;
};


struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);
  __type(value, struct accept_args_t);
  __uint(max_entries, 1024*128);
} active_accept4_args_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/inet_csk_accept")
int trace_accept(struct pt_regs *ctx) {
    
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    // if (!should_intercept()) {
    //     return 0;
    // }

    bpf_printk("kprobe/accept entry: PID: %d\n", pid);
    struct pt_regs *ctx2 = (struct pt_regs *)PT_REGS_PARM1(ctx);

    struct sockaddr *saddr;
    bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));

    // Build the connect_event and save it to the map
    struct accept_args_t accept_args = {};
    accept_args.addr = (struct sockaddr_in *)saddr;
    bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);

    return 0;
}

SEC("kretprobe/inet_accept")
int trace_accept_ret(struct pt_regs *ctx) {

    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

        struct accept_args_t* accept_args = bpf_map_lookup_elem(&active_accept4_args_map, &current_pid_tgid);
    if (accept_args == NULL) {
        return 0;
    }

    // Get the FD and check the call to accept4() was successful
    int fd = (int)PT_REGS_RC(ctx);
    if (fd < 0) {
        bpf_printk("kprobe/accept return: failed: PID: %d, FD: %d\n", pid, fd);
        return 0;
    }

    // Get the source IP & port
    struct addr_t src_addr = {};
    parse_address(&src_addr, accept_args);

    
     // Преобразование IP и портов из сетевого порядка байтов
     u32 ip = bpf_ntohl(src_addr.ip);
    // u16 src_port = bpf_ntohs(src_addr.port);
    u16 src_port = src_addr.port;

    // Выводим результаты
    bpf_printk("Accept returned: PID: %d, FD: %d, Source IP: %d.%d.%d.%d, Source Port: %d\n",
               pid, fd, (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF, src_port);

    // Удаление записи из карты после обработки
    bpf_map_delete_elem(&active_accept4_args_map, &current_pid_tgid);
    





  

    return 0;
}

