
clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

struct event_t {
    u32 pid;
    u32 src_ip;
    u16 sport;
    char comm[64];
};


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

    struct sockaddr_in addr = {};
    if (conn_info->sock_addr) {
        bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr);
        if (addr.sin_family == AF_INET) {
            conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info->sport = bpf_ntohs(addr.sin_port);
        }
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














