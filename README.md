SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
        return 0;
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr)
        return 0;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid     = conn_info->pid;

    /* читаем семейство из sockaddr::sa_family */
    __u16 family;
    if (BPF_CORE_READ_USER_INTO(&family, *addr_ptr, sa_family) < 0)
        goto cleanup;

    if (family == AF_INET) {
        __u32 ip;
        __u16 port;
        /* кастим на sockaddr_in и читаем sin_addr.s_addr и sin_port */
        if (BPF_CORE_READ_USER_INTO(&ip,
                (struct sockaddr_in *)*addr_ptr,
                sin_addr.s_addr) < 0 ||
            BPF_CORE_READ_USER_INTO(&port,
                (struct sockaddr_in *)*addr_ptr,
                sin_port) < 0)
            goto cleanup;

        port = bpf_ntohs(port);
        if (port == 0)
            goto cleanup;

        info.dstIP.s_addr = ip;
        info.dport        = port;
        info.family       = AF_INET;

    } else if (family == AF_INET6) {
        __u16 port6;
        /* кастим на sockaddr_in6 и читаем sin6_port */
        if (BPF_CORE_READ_USER_INTO(&port6,
                (struct sockaddr_in6 *)*addr_ptr,
                sin6_port) < 0)
            goto cleanup;

        port6 = bpf_ntohs(port6);
        if (port6 == 0)
            goto cleanup;

        info.family = AF_INET6;
        info.dport  = port6;

        /* читаем сразу весь массив u6_addr32 */
        if (BPF_CORE_READ_USER_INTO(&info.dstIP6,
                (struct sockaddr_in6 *)*addr_ptr,
                sin6_addr.in6_u.u6_addr32) < 0)
            goto cleanup;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}



