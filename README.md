clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


package main

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -tags linux trace trace.c

struct sockaddr_storage {
    union {
        struct sockaddr_in  in;
        struct sockaddr_in6 in6;
        struct sockaddr_un  un;
        // Добавьте другие типы адресов, если необходимо
    };
    unsigned short sa_family;
};


struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[64];
    struct sockaddr_storage sock_addr;
};

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Копируем адрес сокета в структуру
    if (ctx->addr) {
        bpf_probe_read(&conn_info.sock_addr, sizeof(conn_info.sock_addr), ctx->addr);
    }

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Обрабатываем адрес сокета
    if (conn_info->sock_addr.sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&conn_info->sock_addr;
        conn_info->src_ip = bpf_ntohl(addr_in->sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr_in->sin_port);
    } else if (conn_info->sock_addr.sa_family == AF_INET6) {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&conn_info->sock_addr;
        // Обработка IPv6 адреса
        // ...
    } else if (conn_info->sock_addr.sa_family == AF_UNIX) {
        struct sockaddr_un *addr_un = (struct sockaddr_un *)&conn_info->sock_addr;
        // Обработка Unix-сокета
        // ...
    }

    // Создаем и заполняем структуру события
    struct event_t event = {};
    event.pid = conn_info->pid;
    event.src_ip = conn_info->src_ip;
    event.sport = conn_info->sport;
    __builtin_memcpy(&event.comm, conn_info->comm, sizeof(event.comm));

    // Отправляем событие в пространство пользователя
    bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &event, sizeof(event));

    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}











