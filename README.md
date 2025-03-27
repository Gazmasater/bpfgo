lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


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
        bpf_printk("sys_exit_getsockname: addr_ptr is NULL for PID=%d", pid);
        return 0;
    }

    bpf_printk("sys_exit_getsockname: addr_ptr=%p", addr_ptr);

    struct sockaddr *user_addr_ptr;
    if (bpf_probe_read_user(&user_addr_ptr, sizeof(user_addr_ptr), addr_ptr)) {
        bpf_printk("sys_exit_getsockname: Failed to read user_addr_ptr for PID=%d", pid);
        return 0;
    }

    bpf_printk("sys_exit_getsockname: user_addr_ptr=%p", user_addr_ptr);

    struct sockaddr addr = {};
    if (bpf_probe_read_user(&addr, sizeof(addr), user_addr_ptr)) {
        bpf_printk("sys_exit_getsockname: Failed to read sockaddr struct for PID=%d", pid);
        return 0;
    }

    bpf_printk("sys_exit_getsockname: pid=%d family=%d", pid, addr.sa_family);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        if (bpf_probe_read_user(&addr_in, sizeof(addr_in), user_addr_ptr)) {
            bpf_printk("sys_exit_getsockname: Failed to read sockaddr_in struct for PID=%d", pid);
            return 0;
        }

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port == 0) return 0;

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
        info.dst_ip = ip;
        info.dport = port;
        info.sysexit = 6;

        bpf_printk("!!!sys_exit_getsockname PID=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s FD=%d",
            pid,
            (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8) & 0xff,
            ip & 0xff,
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

 Chrome_ChildIOT-4176    [005] ...21   126.175563: bpf_trace_printk: sys_enter_getsockname PID=4150 NAME=Chrome_ChildIOT
 Chrome_ChildIOT-4176    [005] ...21   126.175570: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000aac8c0d4
 Chrome_ChildIOT-4176    [005] ...21   126.175572: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=4150






