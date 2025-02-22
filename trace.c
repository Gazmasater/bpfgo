//go:build ignore

#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

struct conn_info_t
{
   
    
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[128];
};

struct trace_info {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u16 sport;
    u16 dport;
    char comm[128];
} ;  

// Размещение переменной с атрибутом unused
const struct trace_info *unused __attribute__((unused));

struct status_t {
    bool in_progress;
};

struct sockaddr_any {
    __u16 family;
    __u32 src_ip[4];  // Подходит и для IPv4, и для IPv6
    __u16 port;
};



struct sys_enter_accept4_args
{
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	int fd;
	int pad1;
	struct sockaddr *upeer_sockaddr;
	int *upeer_addrlen;
	int flags;
};

struct sys_enter_accept_args
{
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	int fd;
	int pad1;
	struct sockaddr *upeer_sockaddr;
	int *upeer_addrlen;
};

struct sys_exit_accept4_args
{
	unsigned short common_type;//2
	unsigned char common_flags;//1
	unsigned char common_preempt_count;//1
	int common_pid;//4
	int __syscall_nr;//4
	int pad1;
	long ret;//8
};

struct sys_enter_connect_args
{

	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long fd;
	struct sockaddr *uservaddr;
	int addrlen;
};

struct sys_exit_connect_args
{

unsigned short common_type;      
unsigned char common_flags;      
unsigned char common_preempt_count;    
int common_pid; 
int __syscall_nr; 
int ret; 

};

struct sys_enter_sendto_args
{
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
    int pad1;
    void *buff;
    size_t len;
    unsigned int flags;
    struct sockaddr *addr;
    int addr_len;
};

struct sys_exit_sendto_args
{
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    long ret;
};

struct sys_enter_recvfrom_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    int fd;
    void *ubuf;
    size_t size;
    unsigned int flags;
    struct sockaddr *addr;
    int *addr_len;
};

struct sys_exit_recvfrom_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    int ret;
};


struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, int);
} addr_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} status_map SEC(".maps");



struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); 
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2






SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    long ret = ctx->ret;

    // Если операция завершена с ошибкой
    if (ret < 0) {
        return 0;
    }

    // Преобразуем дескриптор сокета в указатель на сокет
    struct sock *sk = (struct sock *)ret;

    // Проверяем валидность сокета
    if (!sk) {
        bpf_printk("Invalid socket descriptor\n");
        return 0;
    }

    // Читаем порт из структуры inet_sock
    u16 sport = 0;
    bpf_probe_read(&sport, sizeof(sport), &((struct inet_sock *)sk)->inet_sport);

    // Преобразуем порт в хостовый порядок
    u16 port_host = bpf_ntohs(sport);
    conn_info.sport = port_host;

    // Логируем информацию о процессе и порте
    bpf_printk("sys_exit_accept4: Comm=%s, Src Port=%u\n", conn_info.comm, conn_info.sport);

    // Сохраняем информацию о соединении в карте
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}



SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Считываем информацию о sockaddr
    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), ctx->addr) != 0) {
        return 0; 
    }

    // Добавляем статус в status_map
    struct status_t *status = bpf_map_lookup_elem(&status_map, &pid);
    if (status && status->in_progress) {
        return 0; 
    }

    struct status_t new_status = {.in_progress = true};
    bpf_map_update_elem(&status_map, &pid, &new_status, BPF_ANY);

    // Обновляем карты с адресом и информацией о соединении
    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", conn_info.pid, conn_info.comm);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Если операция завершена с ошибкой
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    // Получаем информацию об адресе
    void *addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        bpf_printk("UDP sys_exit_sendto: No sockaddr found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), addr_ptr) != 0) {
        bpf_printk("UDP sys_exit_sendto: Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    // Если это IPv4, обновляем информацию
    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        struct trace_info info = {};
        info.pid = conn_info->pid;
        info.src_ip = conn_info->src_ip;
        info.sport = conn_info->sport;
        bpf_probe_read_str(&info.comm, sizeof(info.comm), conn_info->comm);

        // Выводим информацию о соединении
        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   info.pid, info.comm,
                   (info.src_ip >> 24) & 0xFF, (info.src_ip  >> 16) & 0xFF,
                   (info.src_ip >> 8) & 0xFF, info.src_ip  & 0xFF, info.sport);

        // Отправляем информацию в пользовательское пространство
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    // Обновляем статус на false (отсутствие активности)
    struct status_t status = {.in_progress = false};
    bpf_map_update_elem(&status_map, &pid, &status, BPF_ANY);

    // Очистка карт после завершения
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addr_map, &pid);

    return 0;
}


SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct sys_enter_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid=pid;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    
    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), ctx->addr) != 0) {
        return 0; 
    }

    struct status_t *status = bpf_map_lookup_elem(&status_map, &pid);
    if (status && status->in_progress) {
        return 0; 
    }

    struct status_t new_status = {.in_progress = true};
    bpf_map_update_elem(&status_map, &pid, &new_status, BPF_ANY);

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        return 0;
    }

    void *addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr_in addr;
    bpf_probe_read(&addr, sizeof(addr), addr_ptr);

    conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
    conn_info->dport = bpf_ntohs(addr.sin_port);

    struct trace_info info = {};
    info.pid = conn_info->pid;
    info.dst_ip = conn_info->dst_ip;
    info.dport = conn_info->dport;
    bpf_probe_read_str(&info.comm, sizeof(info.comm), conn_info->comm);

    bpf_printk("UDP sys_exit_recvfrom: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
               info.pid, info.comm,
               (info.dst_ip >> 24) & 0xFF, (info.dst_ip  >> 16) & 0xFF,
               (info.dst_ip >> 8) & 0xFF, info.dst_ip  & 0xFF, info.dport);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addr_map, &pid);

    struct status_t status = {.in_progress = false};
    bpf_map_update_elem(&status_map, &pid, &status, BPF_ANY);

    return 0;
}
