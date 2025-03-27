lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


SEC("tracepoint/syscalls/sys_enter_getsockname")
int trace_enter_getsockname(struct sys_enter_getsockname_args *ctx) {
    struct conn_info_t conn_info = {};

    u32 pid = bpf_get_current_pid_tgid() >> 32;

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.fd=ctx->fd;

    struct sockaddr *addr = (struct sockaddr *)ctx->usockaddr; 
    bpf_map_update_elem(&addrSockName_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    bpf_printk("sys_enter_getsockname PID=%d NAME=%s",conn_info.pid,conn_info.comm);


    return 0;
}

SEC("tracepoint/syscalls/sys_exit_getsockname")
int trace_exit_getsockname(struct sys_exit_getsockname_args *ctx) {

    u32 pid = bpf_get_current_pid_tgid() >> 32;

    long ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("sys_exit_getsockname failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }
  

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;


    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSockName_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    bpf_printk("sys_exit_getsockname PID=%d RET=%d",conn_info->pid,ret);

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("33333sys_exit_getsockname FAMILY=%d ",addr.sa_family);


    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};

        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port==0) {

            return 0;
        }

        
        struct trace_info info = {};

        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid=conn_info->pid;
        info.dst_ip=ip;
        info.dport = port;
        info.sysexit=6;
        
    
        
        bpf_printk("!!!lookup sys_exit_getsockname FAMILY=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s FD=%d",
            addr.sa_family,
            (ip>>24)&0xff,
            (ip>>16)&0xff,
            (ip>>8)&0xff,
            (ip)&0xff,
            port,
            conn_info->comm,
            conn_info->fd
        );

       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
       
    }

    bpf_map_delete_elem(&addrSockName_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}


if (!addr_ptr) {
    bpf_printk("addr_ptr is NULL");
    return 0;
}
bpf_printk("addr_ptr=%p", *addr_ptr);

Socket Thread-2919    [002] ...21  3153.156173: bpf_trace_printk: sys_exit_getsockname  PID=2799 RET=0 addr_ptr=00000000eeafd000


if (bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr)) {
    bpf_printk("bpf_probe_read_user (sockaddr) failed! addr_ptr=%p", *addr_ptr);
    return 0;
}
bpf_printk("bpf_probe_read_user success FAMILY=%d", addr.sa_family);






