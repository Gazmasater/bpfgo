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
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("kprobe/inet_csk_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    bpf_get_current_comm(info.comm, sizeof(info.comm));

    // Чтение IP и портов
    u32 dip;
    u32 portpair;
    if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) != 0 ||
        bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) != 0) {
        return 0;
    }

    info.ip = dip;
    info.sport = portpair >> 16;        // Старшие 16 бит — локальный порт
    info.dport = portpair & 0xFFFF;     // Младшие 16 бит — удаленный порт

    // Сохраняем информацию в карту
    bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    bpf_printk("Connection started: PID=%d, Comm=%s", info.pid, info.comm);

    return 0;
}

SEC("kretprobe/inet_csk_accept")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    if (ret < 0) {
        // Соединение не установлено, удаляем запись
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_printk("Accept failed, removing connection info from map\n");
    } else {
        // Подключение успешно, можно вывести информацию
        struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
        if (info) {
            bpf_printk("Connection accepted: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u",
                       info->pid, info->comm,
                       (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
                       (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
                       info->sport, info->dport);
        }
    }

    return 0;
}
