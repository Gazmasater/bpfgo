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

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "vmlinux.h"
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 sport;
    u16 dport;
    char comm[16];
};

BPF_HASH(conn_info_map, u32, struct conn_info_t);  // Хэш-таблица для хранения информации

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// kprobe на sys_accept для сохранения информации при начале соединения
SEC("kprobe/sys_accept")
int trace_sys_accept(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    
    struct conn_info_t info = {};
    info.pid = pid;
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    // Сохраняем базовую информацию в карту
    conn_info_map.update(&pid, &info);
    bpf_printk("sys_accept called: PID=%d, Comm=%s", info.pid, info.comm);
    
    return 0;
}

// kretprobe на sys_accept для получения результата соединения
SEC("kretprobe/sys_accept")
int trace_sys_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    // Проверка, успешен ли вызов sys_accept
    if (ret < 0) {
        bpf_printk("Accept failed for PID=%d", pid);
        conn_info_map.delete(&pid);  // Удаляем из карты, если соединение не установлено
        return 0;
    }

    struct conn_info_t *info = conn_info_map.lookup(&pid);
    if (!info) {
        return 0; // Если информации нет, выходим
    }

    info->dport = ret;  // Пример использования результата (добавьте необходимые поля)
    bpf_printk("Connection accepted: PID=%d, Comm=%s, Dport=%u", info->pid, info->comm, info->dport);

    return 0;
}

