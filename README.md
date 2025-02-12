SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr addr;
    void *addr_ptr = (void *)ctx->args[5];  // 6-й аргумент - addr

    if (addr_ptr) {
        bpf_probe_read(&addr, sizeof(addr), addr_ptr);
        bpf_map_update_elem(&fd_map, &pid, &addr, BPF_ANY);
    }

    bpf_map_update_elem(&conn_info_map_sc, &pid, &conn_info, BPF_ANY);

    struct conn_info_t *stored_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (stored_info) {
        bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", stored_info->pid, stored_info->comm);
    }
    
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->args[0]; // 1-й аргумент - возвращаемое значение

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        bpf_map_delete_elem(&fd_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        bpf_map_delete_elem(&fd_map, &pid);
        return 0;
    }

    struct sockaddr_in *addr = bpf_map_lookup_elem(&fd_map, &pid);
    if (addr && addr->sin_family == AF_INET) {
        __be32 ip_addr = 0;
        __be16 port;

        ip_addr = BPF_CORE_READ(addr, sin_addr.s_addr);
        port = BPF_CORE_READ(addr, sin_port);

        conn_info->src_ip = bpf_ntohl(ip_addr);
        conn_info->sport = bpf_ntohs(port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);
    bpf_map_delete_elem(&fd_map, &pid);
    return 0;
}
