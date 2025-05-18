SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
        return 0;
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct msghdr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr)
        return 0;

    /* читаем указатель на msghdr из userspace */
    struct msghdr *msg = NULL;
    if (bpf_core_read_user(&msg, sizeof(msg), *addr_ptr) < 0)
        goto cleanup;

    /* читаем указатель на sockaddr из поля msg_name */
    struct sockaddr *namep = NULL;
    if (bpf_core_read_user(&namep, sizeof(namep), &msg->msg_name) < 0)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 11;
    info.pid     = conn_info->pid;
    info.proto   = conn_info->proto;

    /* читаем семейство адреса */
    __u16 family;
    if (BPF_CORE_READ_USER_INTO(&family, namep, sa_family) < 0)
        goto cleanup;

    if (family == AF_INET) {
        __u32 ip;
        __u16 port;
        /* IPv4: читаем sin_addr.s_addr и sin_port */
        if (BPF_CORE_READ_USER_INTO(&ip,
                (struct sockaddr_in *)namep,
                sin_addr.s_addr) < 0 ||
            BPF_CORE_READ_USER_INTO(&port,
                (struct sockaddr_in *)namep,
                sin_port) < 0)
            goto cleanup;

        ip   = bpf_ntohl(ip);
        port = bpf_ntohs(port);
        if (port == 0)
            goto cleanup;

        info.dstIP.s_addr = ip;
        info.dport        = port;
        info.family       = AF_INET;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (family == AF_INET6) {
        __u16 port6;
        /* IPv6: читаем sin6_port */
        if (BPF_CORE_READ_USER_INTO(&port6,
                (struct sockaddr_in6 *)namep,
                sin6_port) < 0)
            goto cleanup;

        port6 = bpf_ntohs(port6);
        if (port6 == 0)
            goto cleanup;

        info.family = AF_INET6;
        info.dport  = port6;

        /* читаем весь массив u6_addr32 */
        if (BPF_CORE_READ_USER_INTO(&info.dstIP6,
                (struct sockaddr_in6 *)namep,
                sin6_addr.in6_u.u6_addr32) < 0)
            goto cleanup;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

cleanup:
    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}






