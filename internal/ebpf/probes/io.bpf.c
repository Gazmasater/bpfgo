/* ====== sendto ====== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct write_args_t wa = {};
    wa.buf = (__u64)ctx->args[1];
    wa.count = (__u64)ctx->args[2];
    if (wa.buf)
        bpf_map_update_elem(&write_args_map, &id, &wa, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[4];
    __u32 addrlen = (__u32)ctx->args[5];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = {.addr = uaddr, .len = addrlen};
        bpf_map_update_elem(&addrSend_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_SENDTO;
    info.fd = ci->fd;
    info.ret = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    struct write_args_t *wa = bpf_map_lookup_elem(&write_args_map, &id);
    if (wa && wa->buf)
        (void)emit_tls_chunk(ctx, id, tgid, ci, &info,
                             (void *)(unsigned long)wa->buf, ret);

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&write_args_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvfrom (filter MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u32 flags  = (__u32)ctx->args[3];
    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];

    if (uaddr && lenp_u) {
        struct addr_recv_meta_t m = {.addr = uaddr, .lenp = lenp_u, .flags = flags};
        bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);
    } else if (flags & MSG_PEEK) {
        struct addr_recv_meta_t m = {.addr = 0, .lenp = 0, .flags = flags};
        bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && (m->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_RECVFROM;
    info.fd = ci->fd;
    info.ret = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    // peer addr written by kernel
    if (m && m->addr && m->lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void *)m->lenp) == 0) {
            if (addrlen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)m->addr, addrlen, 0);
        }
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== sendmsg ====== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    if (msg_u)
        bpf_map_update_elem(&msgSend_map, &id, &msg_u, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_SENDMSG;
    info.fd = ci->fd;
    info.ret = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    /*
     * Unconnected UDP sockets keep no peer in struct sock.  sendmsg carries
     * that peer in msg_name, and IPv4/IPv6 pktinfo may select the local source
     * address.  Parse both so userspace can correlate this send with the
     * corresponding receive flow.
     */
    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    struct user_msghdr64 mh = {};
    int have_mh = msgp && *msgp && read_msghdr64(*msgp, &mh) == 0;
    if (have_mh) {
        if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(&info, (void *)mh.msg_name,
                                          mh.msg_namelen, 1);
        if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(&info, mh.msg_control, mh.msg_controllen,
                               0 /* set local source */);
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

#if TLS_FROM_SENDMSG
    // TLS chunk from first iovec of msghdr
    if (have_mh && mh.msg_iov && mh.msg_iovlen) {
        struct user_iovec64 iv0 = {};
        if (bpf_probe_read_user(&iv0, sizeof(iv0), (void *)(unsigned long)mh.msg_iov) == 0) {
            if (iv0.iov_base) {
                (void)emit_tls_chunk(ctx, id, tgid, ci, &info,
                                     (void *)(unsigned long)iv0.iov_base, ret);
                }
        }
    }
#endif

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvmsg (filter MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    __u32 flags = (__u32)ctx->args[2];

    if (msg_u) {
        struct msg_ptrflags_t v = {.msg = msg_u, .flags = flags};
        bpf_map_update_elem(&msgRecv_map, &id, &v, BPF_ANY);
    }
    return 0;
}



SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct msg_ptrflags_t *pv = bpf_map_lookup_elem(&msgRecv_map, &id);
    if (pv && (pv->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_RECVMSG;
    info.fd = ci->fd;
    info.ret = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    if (pv && pv->msg) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(pv->msg, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)mh.msg_name, mh.msg_namelen, 0);

            if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
                parse_pktinfo_cmsg(&info, mh.msg_control, mh.msg_controllen, 1 /* set DST (local) */);
        }
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== sendmmsg ====== */

SEC("tracepoint/syscalls/sys_enter_sendmmsg")
int trace_sendmmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct mmsg_ptrvlen_t v = {};
    v.vec   = (__u64)ctx->args[1];
    v.vlen  = (__u32)ctx->args[2];
    v.flags = (__u32)ctx->args[3];

    if (v.vec && v.vlen)
        bpf_map_update_elem(&mmsgSend_map, &id, &v, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmmsg")
int trace_sendmmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    struct mmsg_ptrvlen_t *pv = bpf_map_lookup_elem(&mmsgSend_map, &id);
    if (!ci || !pv || !pv->vec)
        goto cleanup;

    __u32 cnt = (__u32)ret;
    cnt = min_u32(cnt, pv->vlen);
    cnt = min_u32(cnt, MAX_MMSG);

    struct trace_info info = {};
    info.event = EV_SENDMMSG;
    info.fd    = ci->fd;
    info.state = (ret > 255) ? 255 : (__u8)ret;
    info.ret   = sum_mmsg_iov_bytes(pv->vec, cnt);

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    // prefer dst from first mmsghdr
    struct user_mmsghdr64 m0 = {};
    if (read_mmsghdr0(pv->vec, &m0) == 0) {
        if (m0.msg_hdr.msg_name && m0.msg_hdr.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(&info, (void *)m0.msg_hdr.msg_name, m0.msg_hdr.msg_namelen, 1);

        if (m0.msg_hdr.msg_control && m0.msg_hdr.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(&info, m0.msg_hdr.msg_control, m0.msg_hdr.msg_controllen, 0 /* set SRC */);
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&mmsgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvmmsg (filter MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvmmsg")
int trace_recvmmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct mmsg_ptrvlen_t v = {};
    v.vec   = (__u64)ctx->args[1];
    v.vlen  = (__u32)ctx->args[2];
    v.flags = (__u32)ctx->args[3];

    if (v.vec && v.vlen)
        bpf_map_update_elem(&mmsgRecv_map, &id, &v, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmmsg")
int trace_recvmmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct mmsg_ptrvlen_t *pv = bpf_map_lookup_elem(&mmsgRecv_map, &id);
    if (pv && (pv->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci || !pv || !pv->vec)
        goto cleanup;

    __u32 cnt = (__u32)ret;
    cnt = min_u32(cnt, pv->vlen);
    cnt = min_u32(cnt, MAX_MMSG);

    struct trace_info info = {};
    info.event = EV_RECVMMSG;
    info.fd    = ci->fd;
    info.state = (ret > 255) ? 255 : (__u8)ret;
    info.ret   = sum_mmsg_len(pv->vec, cnt);

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    // peer + pktinfo from first mmsghdr
    struct user_mmsghdr64 m0 = {};
    if (read_mmsghdr0(pv->vec, &m0) == 0) {
        if (m0.msg_hdr.msg_name && m0.msg_hdr.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(&info, (void *)m0.msg_hdr.msg_name, m0.msg_hdr.msg_namelen, 0);

        if (m0.msg_hdr.msg_control && m0.msg_hdr.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(&info, m0.msg_hdr.msg_control, m0.msg_hdr.msg_controllen, 1 /* set DST(local) */);
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&mmsgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== write/read (ONLY SOCKETS) ====== */

SEC("tracepoint/syscalls/sys_enter_write")
int trace_write_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    if (!is_socket_fd(fd))
        return 0;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    // NEW: save write buf/count so we can read buf on exit
    struct write_args_t wa = {};
    wa.buf   = (__u64)ctx->args[1];
    wa.count = (__u64)ctx->args[2];
    if (wa.buf)
        bpf_map_update_elem(&write_args_map, &id, &wa, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_write")
int trace_write_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_WRITE;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    // NEW: TLS chunk from write(buf)
    struct write_args_t *wa = bpf_map_lookup_elem(&write_args_map, &id);
    if (wa && wa->buf) {
        (void)emit_tls_chunk(ctx, id, tgid, ci, &info, (void *)(unsigned long)wa->buf, ret);
    }

cleanup:
    bpf_map_delete_elem(&write_args_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}


SEC("tracepoint/syscalls/sys_enter_read")
int trace_read_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    if (!is_socket_fd(fd))
        return 0;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_read")
int trace_read_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_READ;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

