SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    // Сохраняем только PID в карту, так как IP и порт пока недоступны
    bpf_map_update_elem(&conn_info_map_accept, &pid, &pid, BPF_ANY);
    bpf_printk("Enter: PID=%d, connection initiated.\n", pid);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    int ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept, &pid);
        return 0;
    }

    // Получаем conn_info по PID после успешного завершения accept4
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = (struct sockaddr *)ctx->args[1];

    struct sockaddr_in addr;
    if (bpf_probe_read_user(&addr, sizeof(addr), conn_info.sock_addr) == 0) {
        if (addr.sin_family == AF_INET) {
            conn_info.src_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info.sport = bpf_ntohs(addr.sin_port);
            bpf_printk("Exit: PID=%d, IP=%d.%d.%d.%d, Port=%d\n",
                pid,
                (conn_info.src_ip >> 24) & 0xFF, (conn_info.src_ip >> 16) & 0xFF,
                (conn_info.src_ip >> 8) & 0xFF, conn_info.src_ip & 0xFF, conn_info.sport);
        } else {
            bpf_printk("Exit: Unsupported family for PID=%d\n", pid);
        }
    } else {
        bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
    }

    // Удаляем запись из карты после успешного завершения соединения
    bpf_map_delete_elem(&conn_info_map_accept, &pid);

    return 0;
}
