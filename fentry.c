
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
    info.pid = pid;  // Инициализация только один раз
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    // Обновляем карту с информацией о соединении
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
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);  // Получаем указатель на struct sock
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    bpf_printk("kretprobe PID=%d\n", pid);

    // Получаем сохраненную информацию о процессе
    struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!info) {
        bpf_printk("No conn_info found for PID=%d\n", pid);
        return 0;
    }

    u32 dip;
    u32 skc_portpair;

    // Чтение IP-адреса и порта из структуры sock
    if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) != 0 ||
        bpf_core_read(&skc_portpair, sizeof(skc_portpair), &sk->__sk_common.skc_portpair) != 0) {
        return 0;
    }

    info->ip = dip;

    // Извлечение локального и удаленного портов
    info->sport = skc_portpair >> 16;  // Локальный порт (старшие 16 бит)
    info->dport = skc_portpair & 0xFFFF;  // Удаленный порт (младшие 16 бит)

    bpf_printk("Connection accepted: PID=%d, Sport=%u, Dport=%u\n", info->pid, info->sport, info->dport);

    // Обновляем карту с информацией о соединении
    bpf_map_update_elem(&conn_info_map, &pid, info, BPF_ANY);
    bpf_printk("Connection started: PID=%d, Comm=%s\n", info->pid, info->comm);

    return 0;
}

SEC("kprobe/tcp_v4_connect")
int trace_tcp_connect(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;

    // Чтение IP-адресов и портов из структуры sock
    if (bpf_core_read(&conn_info.ip, sizeof(conn_info.ip), &sk->__sk_common.skc_rcv_saddr) != 0)
    {
        bpf_printk("!!!!!!!!!!!!!!!!!!!!!!TCP!!!!!!!!!!!!!!!!!!!!!!!!!");

        return 0;
    }
    
    u32 portpair;
    if (bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) != 0)

    {
       
        bpf_printk("!!!!!!!!!!!!!!!!!!!!!!PORTPAIR!!!!!!!!!!!!!!!!!!!!!!!!!");

        return 0;
    }

    // Извлечение локального и удаленного портов
    conn_info.sport = portpair >> 16;   // Локальный порт (старшие 16 бит)
    conn_info.dport = portpair & 0xFFFF; // Удаленный порт (младшие 16 бит)

    // Обновление карты с информацией о подключении
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    bpf_printk("TCP connect: PID=%d, IP=%x, Sport=%u, Dport=%u\n",
               pid, conn_info.ip, conn_info.sport, conn_info.dport);

    return 0;
}
