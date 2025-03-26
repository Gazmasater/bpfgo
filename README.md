lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    int32 fd=ctx->fd;

    

    conn_info.pid = pid;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    
    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr;  
    bpf_map_update_elem(&fd_map, &pid, &fd, BPF_ANY);

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    int32 *fd = bpf_map_lookup_elem(&fd_map, &pid);
    if (!fd) {
        return 0;
    }
  

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
        info.sysexit=4;
        info.fd_connect=fd;
        
    
        
        bpf_printk("!!!lookup sys_exit_connect FAMILY=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s ",
            addr.sa_family,
            (ip>>24)&0xff,
            (ip>>16)&0xff,
            (ip>>8)&0xff,
            (ip)&0xff,
            port,
            conn_info->comm);

       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
       
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&fd_map, &pid);


    return 0;

}



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir "$(pwd)" -tags linux -type trace_info -go-package=main -target amd64 bpf "$(pwd)/trace.c" -- -I"$(pwd)"
/home/gaz358/myprog/bpfgo/trace.c:426:14: warning: expression result unused [-Wunused-value]
  426 |         info.fd_connect;
      |         ~~~~ ^~~~~~~~~~
1 warning generated.
Compiled /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.go





