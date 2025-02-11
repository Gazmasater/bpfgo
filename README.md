struct conn_info_t {
    struct sockaddr *sock_addr;
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    u8 call; // 0- accept 1-accept4 2-connect 3-recvfrom 4-sendto
    u8 protocol; 
    char comm[16];
};

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    conn_info.call = 4;
    conn_info.sock_addr = (struct sockaddr *)ctx->addr;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.protocol = 2;

    bpf_map_update_elem(&conn_info_map_sc, &pid, &conn_info, BPF_ANY);

    struct conn_info_t *stored_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (stored_info) {
        bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", stored_info->pid, stored_info->comm);
    }
    
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("UDP sys_exit_sendto: Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);
    return 0;
}
