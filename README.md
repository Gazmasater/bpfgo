SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_entry(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sockaddr *sock_addr = ctx->upeer_sockaddr;  // Прямой доступ к полю upeer_sockaddr из структуры
    struct conn_info_t conn_info = {};
    init_conn_info(pid, sock_addr, &conn_info);

    bpf_map_update_elem(&conn_info_map_ab, &pid, &conn_info, BPF_ANY);

    bpf_printk("CLIENT accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm);

    return 0;
}
