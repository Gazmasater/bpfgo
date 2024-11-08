#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;

    u16 sport;
    u16 dport;
    char comm[16];
    struct sockaddr *sock_addr;   
    u32 addrlen;           
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2


SEC("kprobe/__sys_accept4")
int trace_accept4_entry(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем указатель на sockaddr и длину
    conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
    conn_info.addrlen = PT_REGS_PARM3(ctx);

    // Сохраняем начальную информацию по PID
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    bpf_printk("CLIENT Accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

    return 0;
}

// kretprobe для завершения извлечения информации о соединении
SEC("kretprobe/__sys_accept4")
int trace_accept4_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx); // Получаем результат вызова

    // Если результат отрицательный, значит произошла ошибка
    if (ret < 0) {
        bpf_printk("Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Получаем IP и порт клиента из sockaddr, используя сохраненный указатель
    struct sockaddr_in addr;

    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем IP и порт из sockaddr_in, если это IPv4-соединение
    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr); // Преобразуем IP к порядку хоста
        conn_info->sport = bpf_ntohs(addr.sin_port);      // Преобразуем порт к порядку хоста
        
        bpf_printk("CLIENT Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
            conn_info->pid, conn_info->comm,
            (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
            (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    // Удаляем запись из карты после завершения обработки
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}


// kprobe для фиксации начальных данных процесса и дескриптора файла
SEC("kprobe/__sys_bind")
int trace_bind_entry(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем указатель на sockaddr и длину
    conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
    conn_info.addrlen = PT_REGS_PARM3(ctx);

    // Сохраняем начальную информацию по PID
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    bpf_printk("SERVER Accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

    return 0;
}

// kretprobe для завершения извлечения информации о соединении
SEC("kretprobe/__sys_bind")
int trace_bind_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx); // Получаем результат вызова

    // Если результат отрицательный, значит произошла ошибка
    if (ret < 0) {
        bpf_printk("Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Получаем IP и порт клиента из sockaddr, используя сохраненный указатель
    struct sockaddr_in addr;

    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем IP и порт из sockaddr_in, если это IPv4-соединение
    if (addr.sin_family == AF_INET) {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr); // Преобразуем IP к порядку хоста
        conn_info->dport = bpf_ntohs(addr.sin_port);      // Преобразуем порт к порядку хоста
        
        bpf_printk("SERVER Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
            conn_info->pid, conn_info->comm,
            (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
            (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
    }

    // Удаляем запись из карты после завершения обработки
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}





