SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct sys_enter_sendmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct msghdr *addr = (struct msghdr *)ctx->msg;  

    bpf_printk("sys_enter_sendmsg addr=%p",addr);
    bpf_map_update_elem(&addrSend_map, &pid, &addr, BPF_ANY);


    return 0;
}




