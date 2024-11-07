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
            // Теперь получаем информацию о сокете через sockfd
            struct sock *sk = (struct sock *)bpf_map_lookup_elem(&sockfd_map, &pid);
            if (sk) {
                // Извлекаем локальный и удаленный порты через skc_portpair
                u16 local_port = 0;
                u16 remote_port = 0;

                // Получаем порты из skc_portpair
                struct sk_buff *skb = (struct sk_buff *)sk;
                if (skb) {
                    struct skc_portpair *port_pair = &skb->skc_portpair;

                    // Локальный порт
                    local_port = bpf_ntohs(port_pair->local);
                    // Удаленный порт
                    remote_port = bpf_ntohs(port_pair->remote);

                    bpf_printk("Accepted connection: PID=%d, Comm=%s, Local Port=%d, Remote Port=%d\n",
                               pid, info->comm, local_port, remote_port);
                }
            }
        }
    }

    return 0;
}
