#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct file *file;
};

// Карта для хранения информации о соединении по PID
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

// Карта для хранения дескриптора сокета по PID
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, int);
} sockfd_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/__sys_accept4")
int trace_accept(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    bpf_get_current_comm(&info.comm, sizeof(info.comm));
    info.file = (struct file *)PT_REGS_PARM1(ctx);

    int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    if (res == 0) {
        bpf_printk("Connection initiated: PID=%d, Comm=%s\n", pid, info.comm);
    } else {
        bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
    }

    return 0;
}


SEC("kretprobe/__sys_accept4")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int sockfd = PT_REGS_RC(ctx);

    if (sockfd < 0) {
        // Если прием соединения не удался, удаляем запись
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_printk("Accept failed, removing connection info for PID=%d\n", pid);
    } else {
        // Соединение успешно принято, сохраняем дескриптор сокета
        bpf_map_update_elem(&sockfd_map, &pid, &sockfd, BPF_ANY);
        
        // Получаем сохраненную информацию о процессе
        struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);

        if (info) {
        bpf_printk("!!!!!!!!!!!!!!!!!!1 (info)=%d\n",info);

        struct sock *sk = bpf_sock_from_fd(sockfd);
       //     if (sk) {
       //         u32 dip;
       //         u32 portpair;
       //         // Чтение IP и портов
       //         if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) == 0 &&
       //             bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) == 0) {
       //             
       //             info->ip = dip;
       //             info->sport = bpf_ntohs(portpair >> 16);
       //             info->dport = bpf_ntohs(portpair & 0xFFFF);
//
       //             bpf_printk("Connection accepted: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u\n",
       //                        info->pid, info->comm,
       //                        (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
       //                        (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
       //                        info->sport, info->dport);
       //         }
       //     }
        }

        // Удаляем запись из sockfd_map после использования
        bpf_map_delete_elem(&sockfd_map, &pid);
    }

    return 0;
}

