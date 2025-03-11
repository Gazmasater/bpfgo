nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct sys_enter_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;  

    struct conn_info_t conn_info = {};
    conn_info.pid=pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr *addr = (struct sockaddr *)ctx->umyaddr;  

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
  
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct sys_exit_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t conn_info = {};
    conn_info.pid=pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));


    if (ret < 0) {
        bpf_printk("sys_exit_bind failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    struct sockaddr_in addr_in = {};
    bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

    u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

    u16 port = bpf_ntohs(addr_in.sin_port);

    bpf_map_update_elem(&bind_map, &pid, &addr_in, BPF_ANY);


    bpf_printk("sys_exit_bind  FAMILY=%d  PID=%d Comm=%s IP=%d.%d.%d.%d PORT=%d ",
        addr.sa_family,
        conn_info.pid,
        conn_info.comm,
        (ip>>24)&0xff,
        (ip>>16)&0xff,
        (ip>>8)&0xff,
        (ip)&0xff,
         port);  
   
  
    return 0;
}
