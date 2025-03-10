nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    int fd=ctx->fd;

    bpf_printk("sys_enter_connect FD=%d   PID=%d", fd,pid);

    conn_info.pid = pid;
    conn_info.fd=fd;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    
    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr;  
    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    struct sockaddr_in *addrdst = (struct sockaddr_in *)ctx->uservaddr;  
    bpf_map_update_elem(&addrdst_map, &pid, &addrdst, BPF_ANY);



    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_connect failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }




    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("sys_exit_connect PID=%d  FAMILY=%d ",conn_info->pid,addr.sa_family);


    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};

        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);


       // bpf_printk("sys_exit_connect FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid=conn_info->pid;
        info.dst_ip=ip;
        info.dport = port;
       

       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
       
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;

}



