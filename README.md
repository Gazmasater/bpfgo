#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 s_ip;
    u32 d_ip;
    u16 s_port;
    u16 d_port;
    char comm[16]; // Для хранения имени процесса
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);
    __type(value, struct sock *);
    __uint(max_entries, 1024);
} sock_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);
    __type(value, struct conn_info_t);
    __uint(max_entries, 1024);
} conn_info_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// kprobe для сохранения указателя на sock
SEC("kprobe/inet_csk_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Сохраняем указатель на sock в карте sock_map
    bpf_map_update_elem(&sock_map, &pid, &sk, BPF_ANY);
    return 0;
}

// kretprobe для обработки результата функции
SEC("kretprobe/inet_csk_accept")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sock **sk_ptr = bpf_map_lookup_elem(&sock_map, &pid);

    // Проверяем, что указатель на sock сохранён в карте
    if (!sk_ptr) {
        bpf_printk("Failed to find socket in sock_map\n");
        return 0;
    }

    struct sock *sk = *sk_ptr;

    // Проверка возвращаемого значения функции
    long ret = PT_REGS_RC(ctx);
    if (ret < 0) {
        // Ошибка, удаляем запись из sock_map и завершаем
        bpf_map_delete_elem(&sock_map, &pid);
        bpf_printk("Accept failed, removing socket from map\n");
        return 0;
    }

    struct conn_info_t info = {};
    info.pid = pid;

    // Получаем имя процесса
    bpf_get_current_comm(&info.comm, sizeof(info.comm));

    // Чтение IP-адресов и портов с помощью bpf_core_read
    u32 s_ip, d_ip, portpair;

    if (bpf_core_read(&s_ip, sizeof(s_ip), &sk->__sk_common.skc_rcv_saddr) != 0) {
        bpf_printk("Failed to read source IP\n");
        bpf_map_delete_elem(&sock_map, &pid);
        return 0;
    }
    if (bpf_core_read(&d_ip, sizeof(d_ip), &sk->__sk_common.skc_daddr) != 0) {
        bpf_printk("Failed to read destination IP\n");
        bpf_map_delete_elem(&sock_map, &pid);
        return 0;
    }
    if (bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) != 0) {
        bpf_printk("Failed to read port pair\n");
        bpf_map_delete_elem(&sock_map, &pid);
        return 0;
    }

    // Извлечение исходного и целевого портов из portpair
    info.s_port = portpair >> 16;       // Старшие 16 бит — исходный порт
    info.d_port = portpair & 0xFFFF;    // Младшие 16 бит — целевой порт
    info.s_ip = s_ip;
    info.d_ip = d_ip;

    // Сохраняем информацию о подключении в conn_info_map
    bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);

    // Удаляем запись из sock_map, так как она больше не нужна
    bpf_map_delete_elem(&sock_map, &pid);

    // Вывод информации об успешном подключении
    bpf_printk("Connection accepted: PID=%d, Comm=%s", info.pid, info.comm);
    bpf_printk("Source IP=%u.%u.%u.%u, Source Port=%u",
               (info.s_ip >> 0) & 0xFF,
               (info.s_ip >> 8) & 0xFF,
               (info.s_ip >> 16) & 0xFF,
               (info.s_ip >> 24) & 0xFF,
               info.s_port);
    bpf_printk("Destination IP=%u.%u.%u.%u, Destination Port=%u",
               (info.d_ip >> 0) & 0xFF,
               (info.d_ip >> 8) & 0xFF,
               (info.d_ip >> 16) & 0xFF,
               (info.d_ip >> 24) & 0xFF,
               info.d_port);

    return 0;
}
