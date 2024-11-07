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

// kprobe: Фиксируем информацию о процессе
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

    // Сохраняем минимальную информацию в карту
    int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    if (res == 0) {
        bpf_printk("Connection started: PID=%d, Comm=%s\n", info.pid, info.comm);
    } else {
        bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
    }

    return 0;
}

// kretprobe: Извлекаем и выводим полную информацию о соединении
// Убедитесь, что определение функции trace_accept_ret есть только одно.
SEC("kprobe/__sys_accept4")
int trace_accept(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct file *file = (struct file *)PT_REGS_PARM1(ctx);

    struct conn_info_t info = {};
    info.pid = pid;
    info.file = file;
    bpf_get_current_comm(info.comm, sizeof(info.comm));

    bpf_printk("trace_accept: PID=%d, Comm=%s\n", pid, info.comm);

    // Сохраняем информацию в карту
    int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    if (res == 0) {
        bpf_printk("Connection start detected: PID=%d, Comm=%s\n", info.pid, info.comm);
    } else {
        bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
    }

    return 0;
}

// kretprobe: Извлекаем и выводим полную информацию о соединении
SEC("kretprobe/__sys_accept4")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    if (ret < 0) {
        // Соединение не установлено, удаляем информацию из карты
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_printk("Accept failed for PID=%d\n", pid);
        return 0;
    }

    struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (info && info->file) {
        struct socket *sock;
        struct sock *sk;

        // Используем сохранённый указатель на file
        if (bpf_core_read(&sock, sizeof(sock), &info->file->private_data) != 0 || !sock) {
            bpf_printk("Failed to read file->private_data\n");
            return 0;
        }

        if (bpf_core_read(&sk, sizeof(sk), &sock->sk) != 0 || !sk) {
            bpf_printk("Failed to read sock->sk\n");
            return 0;
        }

        // Читаем IP-адрес и порты
        bpf_core_read(&info->ip, sizeof(info->ip), &sk->__sk_common.skc_rcv_saddr);
        u32 portpair;
        bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair);
        info->sport = bpf_ntohs(portpair >> 16);
        info->dport = bpf_ntohs(portpair & 0xFFFF);

        bpf_printk("Connection accepted: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u\n",
                   info->pid, info->comm,
                   (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
                   (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
                   info->sport, info->dport);
    } else {
        bpf_printk("No connection info found for PID=%d after accept\n", pid);
    }

    return 0;
}
