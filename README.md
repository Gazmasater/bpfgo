struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);   // PID процесса
    __type(value, u64); // указатель на struct sockaddr (user-space), сохранённый как целое число
} addrSend_map SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    const struct sockaddr *addr = (const struct sockaddr *)ctx->args[4];
    if (!addr)
        return 0;

    u64 addr_ptr = (u64)addr;
    bpf_map_update_elem(&addrSend_map, &pid, &addr_ptr, BPF_ANY);

    return 0;
}

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

    u64 *addr_ptr = bpf_map_lookup_elem(&addrSend_map, &pid);
    if (!addr_ptr)
        return 0;

    struct trace_info info = {};
    struct sockaddr addr = {};
    if (bpf_core_read_user(&addr, sizeof(addr), (void *)*addr_ptr) < 0)
        return 0;

    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 1;
    info.pid = conn_info->pid;

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        if (bpf_core_read_user(&addr_in, sizeof(addr_in), (void *)*addr_ptr) < 0)
            return 0;

        u32 ip = addr_in.sin_addr.s_addr;
        u16 port = bpf_ntohs(addr_in.sin_port);
        if (port == 0)
            return 0;

        info.dstIP.s_addr = ip;
        info.dport = port;
        info.family = AF_INET;

    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        if (bpf_core_read_user(&addr_in6, sizeof(addr_in6), (void *)*addr_ptr) < 0)
            return 0;

        u16 port = bpf_ntohs(addr_in6.sin6_port);
        if (port == 0)
            return 0;

        info.family = AF_INET6;
        info.dport = port;
        __builtin_memcpy(&info.dstIP6, &addr_in6.sin6_addr.in6_u.u6_addr32, sizeof(info.dstIP6));
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    bpf_map_delete_elem(&addrSend_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}







