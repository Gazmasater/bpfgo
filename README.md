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

SEC("kprobe/__sys_accept4")
int trace_accept(struct pt_regs *ctx) {
    struct file *file = (struct file *)PT_REGS_PARM1(ctx);
    if (!file) {
        bpf_printk("Failed: file is NULL\n");
        return 0;
    }

    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t info = {};
    info.pid = pid;
    bpf_get_current_comm(info.comm, sizeof(info.comm));

    bpf_printk("trace_accept: PID=%d, Comm=%s\n", pid, info.comm);

    // Сохраняем информацию в карту
    int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    if (res == 0) {
        bpf_printk("Connection started: PID=%d, Comm=%s\n", info.pid, info.comm);
    } else {
        bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
    }

    return 0;
}

SEC("kretprobe/__sys_accept4")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    if (ret < 0) {
        // Соединение не установлено, удаляем запись
        int res = bpf_map_delete_elem(&conn_info_map, &pid);
        if (res == 0) {
            bpf_printk("Accept failed, removed connection info for PID=%d\n", pid);
        } else {
            bpf_printk("Failed to delete connection info from map for PID=%d\n", pid);
        }
    } else {
        // Подключение успешно, можно вывести информацию
        struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
        if (info) {
            bpf_printk("Connection accepted: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u\n",
                       info->pid, info->comm,
                       (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
                       (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
                       info->sport, info->dport);
        } else {
            bpf_printk("No connection info found for PID=%d after accept\n", pid);
        }
    }

    return 0;
}
