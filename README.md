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
            sudo-4938    [002] ...21   106.421808: bpf_trace_printk: sys_enter_getsockname PID=4938 NAME=sudo addr=000000007803979e
            sudo-4938    [002] ...21   106.421811: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000a75752a4 for PID=4938
            sudo-4938    [002] ...21   106.421812: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=4938, error=-14
           <...>-370     [000] ...21   118.872862: bpf_trace_printk: sys_enter_getsockname PID=370 NAME=systemd-udevd addr=00000000431eec02
           <...>-370     [000] ...21   118.872867: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000bec7499e for PID=370
           <...>-370     [000] ...21   118.872869: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=370, error=-14
           <...>-975     [001] ...21   133.737171: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737184: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000080810b94 for PID=975
           <...>-975     [001] ...21   133.737186: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737247: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737250: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000784de4c8 for PID=975
           <...>-975     [001] ...21   133.737252: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737282: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737284: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000080810b94 for PID=975
           <...>-975     [001] ...21   133.737286: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737315: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737318: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000784de4c8 for PID=975
           <...>-975     [001] ...21   133.737319: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737347: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737349: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000080810b94 for PID=975
           <...>-975     [001] ...21   133.737351: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737378: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737381: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000784de4c8 for PID=975
           <...>-975     [001] ...21   133.737383: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737420: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737422: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000080810b94 for PID=975
           <...>-975     [001] ...21   133.737424: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737455: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737457: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000784de4c8 for PID=975
           <...>-975     [001] ...21   133.737459: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737491: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737493: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000080810b94 for PID=975
           <...>-975     [001] ...21   133.737496: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737534: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737537: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000784de4c8 for PID=975
           <...>-975     [001] ...21   133.737539: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737588: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737591: bpf_trace_printk: sys_exit_getsockname: addr_ptr=0000000080810b94 for PID=975
           <...>-975     [001] ...21   133.737592: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14
           <...>-975     [001] ...21   133.737626: bpf_trace_printk: sys_enter_getsockname PID=975 NAME=NetworkManager addr=00000000a05171dc
           <...>-975     [001] ...21   133.737629: bpf_trace_printk: sys_exit_getsockname: addr_ptr=00000000784de4c8 for PID=975
           <...>-975     [001] ...21   133.737648: bpf_trace_printk: sys_exit_getsockname: Failed to read user_addr_ptr for PID=975, error=-14

