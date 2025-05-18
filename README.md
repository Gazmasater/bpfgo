SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;


    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No conn_info for pid=%d", pid);
        goto cleanup;
    }

    if (ret < 0) {
        bpf_printk("recvmsg failed for PID=%d", pid);
        goto cleanup;

    }

    // Получаем указатель на msghdr
    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr) {
        bpf_printk("No addr_ptr for pid=%d", pid);
        goto cleanup;
    }

    struct msghdr *msg;
    bpf_core_read_user(&msg, sizeof(msg), *addr_ptr);

    if (!msg) {
        bpf_printk("msg is NULL for pid=%d", pid);
        goto cleanup;
    }



    struct sockaddr_in sa = {};
    struct sockaddr_in6 sa6 = {};
    struct trace_info info = {};
   
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit=11;


    bpf_core_read_user(&sa, sizeof(sa), &msg->msg_name);
    bpf_core_read_user(&sa6, sizeof(sa6), &msg->msg_name);

    __u16 family;
    if (BPF_CORE_READ_USER_INTO(&family, sa, sa_family) < 0)
    goto cleanup;

    if (sa.sin_family==AF_INET) {

     u32   port=bpf_ntohs(sa.sin_port);
     u32   ip=bpf_ntohl(sa.sin_addr.s_addr);
     info.pid=conn_info->pid;
     info.dstIP.s_addr=ip;
     info.dport = port;
     info.family=AF_INET;  
     info.proto=conn_info->proto;
     bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


    } else if (sa.sin_family==AF_INET6) {

        u32 port=bpf_ntohs(sa6.sin6_port);
        

        if (port==0) {
         goto cleanup;

        }

        bpf_printk("sys_exit_recvmsg IP6 PORT=%d SYSEXIT=%d",port,info.sysexit);



        info.family=AF_INET6;
        info.dport=port;
        info.pid=pid;

        __builtin_memcpy(&info.dstIP6, &sa6.sin6_addr.in6_u.u6_addr32, sizeof(info.dstIP6));


         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    }



cleanup:

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "3364",
	"severity": 8,
	"message": "operator -> or ->* applied to \"struct sockaddr_in\" instead of to a pointer type",
	"source": "C/C++",
	"startLineNumber": 367,
	"startColumn": 9,
	"endLineNumber": 367,
	"endColumn": 32
}]




