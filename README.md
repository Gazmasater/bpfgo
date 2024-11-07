#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

// Структура для хранения информации о соединении
struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct file *file;
};

// Хеш-карта для хранения информации о соединениях
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// kprobe для фиксации PID и указателя на file при вызове accept
SEC("kprobe/__sys_accept4")
int trace_accept(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct file *file = (struct file *)PT_REGS_PARM1(ctx);
    if (!file) {
        bpf_printk("trace_accept: file is NULL\n");
        return 0;
    }

    // Создаем и заполняем структуру conn_info_t
    struct conn_info_t info = {};
    info.pid = pid;
    info.file = file;
    bpf_get_current_comm(info.comm, sizeof(info.comm));

    // Сохраняем информацию о процессе в карту
    int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    if (res == 0) {
        bpf_printk("trace_accept: PID=%d, Comm=%s\n", pid, info.comm);
    } else {
        bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
    }

    return 0;
}

// kretprobe для извлечения информации о соединении после успешного вызова accept
SEC("kretprobe/__sys_accept4")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    // Проверяем результат вызова accept
    if (ret < 0) {
        // Ошибка соединения, удаляем запись из карты
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_printk("Accept failed, removed connection info for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем сохраненную информацию из карты
    struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!info || !info->file) {
        bpf_printk("No connection info found for PID=%d or file is NULL\n", pid);
        return 0;
    }

    // Получаем указатель на socket и структуру sock
    struct socket *sock;
    if (bpf_core_read(&sock, sizeof(sock), &info->file->private_data) != 0 || !sock) {
        bpf_printk("Failed to read file->private_data in kretprobe\n");
        return 0;
    }

    struct sock *sk;
    if (bpf_core_read(&sk, sizeof(sk), &sock->sk) != 0 || !sk) {
        bpf_printk("Failed to read sock->sk in kretprobe\n");
        return 0;
    }

    // Извлекаем IP и порты
    u32 dip;
    u32 portpair;
    if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) != 0) {
        bpf_printk("Failed to read sk->__sk_common.skc_rcv_saddr in kretprobe\n");
        return 0;
    }
    if (bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) != 0) {
        bpf_printk("Failed to read sk->__sk_common.skc_portpair in kretprobe\n");
        return 0;
    }

    // Заполняем информацию о IP и портах
    info->ip = dip;
    info->sport = bpf_ntohs(portpair >> 16);
    info->dport = bpf_ntohs(portpair & 0xFFFF);

    // Выводим информацию о соединении
    bpf_printk("Connection established: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u\n",
               info->pid, info->comm,
               (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
               (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
               info->sport, info->dport);

    // Удаляем информацию из карты после использования
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}
