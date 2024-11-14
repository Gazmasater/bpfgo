SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept_entry(struct sys_enter_accept4_args *ctx){
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid=pid;
    conn_info.sock_addr = ctx->upeer_sockaddr;

    struct conn_info_t *conn_info_ptr = bpf_map_lookup_elem(&conn_info_map_accept, &pid); 
    bpf_printk("conn_info_ptr=%d\n",conn_info_ptr);
    if (conn_info_ptr) 
    { bpf_printk("CLIENT accept4 entry: PID=%d, Comm=%s\n", pid, conn_info.comm); }
    
    return 0;
}
