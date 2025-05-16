SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr) return 0;

    struct trace_info info = {};
    struct sockaddr addr = {};

    if (bpf_core_read_user(&addr, sizeof(addr), *addr_ptr) < 0) return 0;

    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid = conn_info->pid;

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        if (bpf_core_read_user(&addr_in, sizeof(addr_in), *addr_ptr) < 0) return 0;

        u32 ip = addr_in.sin_addr.s_addr;
        u16 port = bpf_ntohs(addr_in.sin_port);
        if (port == 0) return 0;

        info.ddstIP.sin_addr.s_addr = ip;
        info.dport = port;
        info.family = AF_INET;

    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        if (bpf_core_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr) < 0) return 0;

        u16 port = bpf_ntohs(addr_in6.sin6_port);
        info.family = AF_INET6;
        info.dport = port;
        __builtin_memcpy(&info.ddstIP6, &addr_in6.sin6_addr.in6_u.u6_addr16, sizeof(info.ddstIP6));
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}


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

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct sys_exit_sendmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;


    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No conn_info for pid=%d", pid);
        return 0;
    }

    if (ret < 0) {
        bpf_printk("recvmsg failed for PID=%d", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    // Получаем указатель на msghdr
    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr) {
        bpf_printk("No addr_ptr for pid=%d", pid);
        return 0;
    }

    struct msghdr *msg;
    bpf_probe_read_user(&msg, sizeof(msg), *addr_ptr);

    if (!msg) {
        bpf_printk("msg is NULL for pid=%d", pid);
        return 0;
    }

    

    struct sockaddr_in sa = {};
    struct sockaddr_in6 sa6 = {};
    struct trace_info info = {};
   
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit=11;


    bpf_probe_read_user(&sa, sizeof(sa), &msg->msg_name);
    bpf_probe_read_user(&sa6, sizeof(sa6), &msg->msg_name);

    if (sa.sin_family==AF_INET) {

     u32   port=bpf_ntohs(sa.sin_port);
     u32   ip=bpf_ntohl(sa.sin_addr.s_addr);
     info.pid=conn_info->pid;
     info.ddstIP.sin_addr.s_addr=ip;
     info.dport = port;
     info.family=AF_INET;  
     info.proto=conn_info->proto;
     bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    } else if (sa.sin_family==AF_INET6) {

        u32 port=bpf_ntohs(sa6.sin6_port);
        

        if (port==0) {
            return 0;
        }

        bpf_printk("sys_exit_recvmsg IP6 PORT=%d SYSEXIT=%d",port,info.sysexit);



        info.family=AF_INET6;
        info.dport=port;
        info.pid=pid;

        __builtin_memcpy(&info.ddstIP6, &sa6.sin6_addr.in6_u.u6_addr16, sizeof(info.ddstIP6));


         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}


SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct sys_exit_recvmsg_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No conn_info for pid=%d", pid);
        return 0;
    }

    if (ret < 0) {
        bpf_printk("recvmsg failed for PID=%d", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &pid);
    if (!addr_ptr) {
        bpf_printk("No addr_ptr for pid=%d", pid);
        return 0;
    }

    struct msghdr *msg;
    bpf_probe_read_user(&msg, sizeof(msg), *addr_ptr);

    if (!msg) {
        bpf_printk("msg is NULL for pid=%d", pid);
        return 0;
    }

    struct sockaddr_in sa = {};
    struct sockaddr_in6 sa6 = {};
    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));


    bpf_probe_read_user(&sa, sizeof(sa), &msg->msg_name);
    bpf_probe_read_user(&sa6, sizeof(sa6), &msg->msg_name);
    info.sysexit=12;



    if (sa.sin_family==AF_INET) {

     u32   port=bpf_ntohs(sa.sin_port);
     u32   ip=bpf_ntohl(sa.sin_addr.s_addr);
     info.pid=conn_info->pid;
     info.ssrcIP.sin_addr.s_addr=ip;
     info.sport = port;
     info.family=AF_INET;  
           
     bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    } else if (sa.sin_family==AF_INET6) {

        u32 port=bpf_ntohs(sa6.sin6_port);
        

        if (port==0) {
            return 0;
        }

        bpf_printk("sys_exit_recvmsg IP6 PORT=%d SYSEXIT=%d",port,info.sysexit);

        info.family=AF_INET6;
        info.sport=port;
        info.pid=pid;



        __builtin_memcpy(&info.ssrcIP6, &sa6.sin6_addr.in6_u.u6_addr16, sizeof(info.ssrcIP6));
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }

    bpf_map_delete_elem(&addrRecv_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}



