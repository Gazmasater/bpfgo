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

    // Получаем имя процесса
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.fd = ctx->fd;

    // Получаем указатель на sockaddr
    struct sockaddr *addr = (struct sockaddr *)ctx->usockaddr;
    bpf_printk("sys_enter_getsockname PID=%d NAME=%s addr=%p", conn_info.pid, conn_info.comm, addr);

    // Обновляем карту addrSockName_map с addr
    bpf_map_update_elem(&addrSockName_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

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
    if (!conn_info) {
        bpf_printk("sys_exit_getsockname: no conn_info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSockName_map, &pid);
    if (!addr_ptr) {
        bpf_printk("sys_exit_getsockname: addr_ptr is NULL for PID=%d\n", pid);
        return 0;
    }

    // Печатаем значение addr_ptr для диагностики
    bpf_printk("sys_exit_getsockname: addr_ptr=%p for PID=%d", addr_ptr, pid);

    if (!addr_ptr || *addr_ptr == NULL) {
        bpf_printk("sys_exit_getsockname: addr_ptr is NULL or points to NULL for PID=%d", pid);
        return 0;
    }

    // Попробуем прочитать указатель на sockaddr
    struct sockaddr *user_addr_ptr;
    int read_result = bpf_probe_read_user(&user_addr_ptr, sizeof(user_addr_ptr), addr_ptr);

    if (read_result < 0) {
        bpf_printk("sys_exit_getsockname: Failed to read user_addr_ptr for PID=%d, error=%d", pid, read_result);
        return 0;
    }

    bpf_printk("sys_exit_getsockname: user_addr_ptr=%p for PID=%d", user_addr_ptr, pid);

    if (!user_addr_ptr) {
        bpf_printk("sys_exit_getsockname: user_addr_ptr is NULL for PID=%d", pid);
        return 0;
    }

    struct sockaddr addr = {};
    read_result = bpf_probe_read_user(&addr, sizeof(addr), user_addr_ptr);

    if (read_result < 0) {
        bpf_printk("sys_exit_getsockname: Failed to read sockaddr for PID=%d, error=%d", pid, read_result);
        return 0;
    }

    // Печатаем результат чтения
    bpf_printk("sys_exit_getsockname: addr_ptr read: FAMILY=%d for PID=%d", addr.sa_family, pid);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        read_result = bpf_probe_read_user(&addr_in, sizeof(addr_in), user_addr_ptr);

        if (read_result < 0) {
            bpf_printk("sys_exit_getsockname: Failed to read sockaddr_in for PID=%d, error=%d", pid, read_result);
            return 0;
        }

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port == 0) {
            return 0;
        }

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
        info.dst_ip = ip;
        info.dport = port;
        info.sysexit = 6;

        bpf_printk("sys_exit_getsockname PID=%d ADDRESS=%d.%d.%d.%d:%d Comm=%s FD=%d",
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


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/trace_pipe|grep "getsockname"
[sudo] password for gaz358: 
 DNS Resolver #3-3529    [002] ...21    84.200362: bpf_trace_printk: sys_enter_getsockname PID=2896 NAME=DNS Resolver #3 addr=000000006ec58499
 DNS Resolver #3-3529    [002] ...21    84.200369: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000c7f5a6b3 for PID=2896
 DNS Resolver #3-3529    [002] ...21    84.200371: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=2896, error=-14
 DNS Resolver #8-3534    [004] ...21    84.200876: bpf_trace_printk: sys_enter_getsockname PID=2896 NAME=DNS Resolver #8 addr=000000002d7aca84
 DNS Resolver #8-3534    [004] ...21    84.200884: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000044ea294c for PID=2896
 DNS Resolver #8-3534    [004] ...21    84.200886: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=2896, error=-14
 DNS Resolver #3-3529    [002] ...21    84.215381: bpf_trace_printk: sys_enter_getsockname PID=2896 NAME=DNS Resolver #3 addr=000000006ec58499
 DNS Resolver #3-3529    [002] ...21    84.215385: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000e6460ae0 for PID=2896
 DNS Resolver #3-3529    [002] ...21    84.215385: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=2896, error=-14
   Socket Thread-3015    [005] ...21    84.236400: bpf_trace_printk: sys_enter_getsockname PID=2896 NAME=Socket Thread addr=000000002d7aca84
   Socket Thread-3015    [005] ...21    84.236404: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000050dddbaf for PID=2896
   Socket Thread-3015    [005] ...21    84.236405: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=2896, error=-14

