struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 4096); // Размер буфера в байтах
} events SEC(".maps");

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("exit_connect Connect failed for PID=%d\n", pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_printk("exit_connect No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("exit_connect Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    struct event_t *event = bpf_ringbuf_reserve(&events, sizeof(struct event_t), 0);
    if (!event) {
        return 0; // Если недостаточно места
    }

    event->pid = conn_info->pid;
    bpf_probe_read_str(event->comm, sizeof(event->comm), conn_info->comm);
    event->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
    event->sport = bpf_ntohs(addr.sin_port);

    bpf_ringbuf_submit(event, 0); // Отправляем событие

    return 0;
}
