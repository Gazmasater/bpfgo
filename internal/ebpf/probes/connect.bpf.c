/* ====== connect ====== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct inflight_fd_t in = {};
    in.fd = (int)ctx->args[0];
    bpf_map_update_elem(&connect_fd_map, &id, &in, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[1];
    __u32 addrlen = (__u32)ctx->args[2];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = {.addr = uaddr, .len = addrlen};
        bpf_map_update_elem(&addrConnect_map, &id, &v, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0)
        goto cleanup;

    if (ret < 0 && ret != -EINPROGRESS && ret != -EALREADY)
        goto cleanup;

    struct inflight_fd_t *in = bpf_map_lookup_elem(&connect_fd_map, &id);
    if (!in)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state(in->fd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = in->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.event = EV_CONNECT;
    info.state = (ret < 0) ? 1 : 0;
    info.fd    = (__u32)in->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)info.fd, conn ? conn->comm : 0);

    info.proto  = st.proto;
    info.family = st.family;
    info.sport  = st.lport;
    info.dport  = st.rport;

    if (st.family == AF_INET) {
        info.src_ip4 = st.lip;
        info.dst_ip4 = st.rip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, &st.lip6, 16);
        __builtin_memcpy(info.dst_ip6, &st.rip6, 16);
    } else {
        goto cleanup;
    }

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== accept/accept4 ====== */

static __always_inline int accept_enter_common(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0]; // listen fd
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);
    return 0;
}

static __always_inline int accept_exit_common(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 newfd = 0;
    if (read_sys_exit_ret(ctx, &newfd) < 0 || newfd < 0)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)newfd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)newfd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.event = EV_ACCEPT;

    info.fd  = conn ? conn->fd : 0; // listen fd
    info.ret = newfd;

    info.ts_ns  = bpf_ktime_get_ns();
    info.tgid   = tgid;
    info.tid    = (__u32)id;
    info.cookie = cookie_from_fd((int)newfd);

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm(info.comm, sizeof(info.comm));

    info.proto  = st.proto;
    info.family = st.family;

    // accept: peer -> local
    info.sport = st.rport;
    info.dport = st.lport;

    if (st.family == AF_INET) {
        info.src_ip4 = st.rip;
        info.dst_ip4 = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, &st.rip6, 16);
        __builtin_memcpy(info.dst_ip6, &st.lip6, 16);
    } else {
        goto cleanup;
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(ctx); }

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(ctx); }

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

