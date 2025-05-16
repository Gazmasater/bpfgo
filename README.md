SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_recvfrom failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct trace_info info = {};

    struct sockaddr addr = {};

    struct sockaddr_in addr_in = {};

   if (bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr)<0){
    return 0;
   }


   if (bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr)<0){
    return 0;
   }  



    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
        
       
    info.sysexit=2;


    if (addr.sa_family == AF_INET) {

        struct sockaddr_in addr_in = {};
       if (bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr)<0){
        return 0;
       }

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port== 0) {
        return 0;
        }
               
        info.pid = pid;

      

        info.ssrcIP.sin_addr.s_addr=ip;
        info.sport = port;
        info.family=AF_INET;           
        info.pid=pid;
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

                    
    } else if (addr.sa_family==AF_INET6) {



    struct sockaddr_in6 addr_in6 = {};

    if (bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr) < 0) {
        return 0;
    }



    u16 port6 = bpf_ntohs(addr_in6.sin6_port);
    info.family = AF_INET6;
    info.pid = conn_info->pid;
    info.sport = port6;


     __builtin_memcpy(&info.ssrcIP6, &addr_in6.sin6_addr.in6_u.u6_addr16, sizeof(info.ssrcIP6));
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }
    bpf_map_delete_elem(&addrRecv_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);


 
    return 0;

}



