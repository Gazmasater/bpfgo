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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// kprobe: Фиксируем PID, имя процесса и указатель на структуру file
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

// kretprobe: Завершаем извлечение информации о соединении
SEC("kretprobe/__sys_accept4")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_printk("Accept failed, removed connection info for PID=%d\n", pid);
        return 0;
    }

    struct socket *sock;
    if (bpf_core_read(&sock, sizeof(sock), &info->file->private_data) != 0 || !sock) {
        bpf_printk("Failed to read file->private_data for PID=%d\n", pid);
        return 0;
    }

    struct sock *sk;
    if (bpf_core_read(&sk, sizeof(sk), &sock->sk) != 0 || !sk) {
        bpf_printk("Failed to read sock->sk for PID=%d\n", pid);
        return 0;
    }

    u32 dip;
    u32 portpair;
    if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) != 0) {
        bpf_printk("Failed to read skc_rcv_saddr for PID=%d\n", pid);
        return 0;
    }
    if (bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) != 0) {
        bpf_printk("Failed to read skc_portpair for PID=%d\n", pid);
        return 0;
    }

    info->ip = dip;
    info->sport = bpf_ntohs(portpair >> 16);
    info->dport = bpf_ntohs(portpair & 0xFFFF);

    bpf_printk("Connection accepted: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u\n",
               info->pid, info->comm,
               (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
               (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
               info->sport, info->dport);

    // Удаляем запись после завершения обработки
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/sock.h>
#include <linux/tcp.h>
#include <net/sock.h>
#include <bpf/bpf_helpers.h>

struct conn_info_t {
    u32 pid;
    u16 sport;  // Исходный порт
    u16 dport;  // Целевой порт
    char comm[TASK_COMM_LEN];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

SEC("kprobe/__sys_accept4")
int trace_accept(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t info = {};
    struct sock *sk;

    info.pid = pid;
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    // Получаем указатель на структуру sock (например, в PT_REGS_PARM1)
    sk = (struct sock *)PT_REGS_PARM1(ctx);

    if (sk) {
        // Чтение skc_portpair
        u32 portpair = 0;
        bpf_probe_read_kernel(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair);

        // Извлекаем целевой (локальный) и исходный (удалённый) порты
        info.dport = (portpair >> 16);  // Старшие 16 бит — целевой порт
        info.sport = (portpair & 0xFFFF);  // Младшие 16 бит — исходный порт

        // Сохраняем информацию в карте
        int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
        if (res == 0) {
            bpf_printk("Connection initiated: PID=%d, Comm=%s, sport=%d, dport=%d\n",
                       pid, info.comm, ntohs(info.sport), ntohs(info.dport));
        } else {
            bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
        }
    } else {
        bpf_printk("sock is NULL\n");
    }

    return 0;
}

char _license[] SEC("license") = "GPL";

