gcc udp_server.c -o udp_server

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_sendto failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);

    bpf_printk("sys_exit_sendto FAMILY=%d Comm=%s", addr.sa_family, conn_info->comm);

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid = conn_info->pid;
        info.dst_ip = ip;
        info.dport = port;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr);

        u8 *ip6 = addr_in6.sin6_addr.s6_addr;
        u16 port = bpf_ntohs(addr_in6.sin6_port);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid = conn_info->pid;
        // Для IPv6 записываем в info.dst_ip первые 4 байта IPv6 (например, для логирования)
        info.dst_ip = *(u32 *)ip6;  // Можно сделать и более детальное представление IP
        info.dport = port;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        // Логирование для IPv6
        bpf_printk("IPv6 detected: port %d, address %x:%x:%x:%x:%x:%x:%x:%x\n",
            port,
            ip6[0], ip6[1], ip6[2], ip6[3], ip6[4], ip6[5], ip6[6], ip6[7]
        );
    }

    return 0;
}

