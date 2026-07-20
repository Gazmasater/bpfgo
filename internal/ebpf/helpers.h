/* ====== small helpers ====== */

#define MAX_CMSG_ITERS 6



static __always_inline void parse_pktinfo_cmsg(struct trace_info *info,
                                               __u64 cmsg_u,
                                               __u64 cmsg_len,
                                               int set_dst_local)
{
    if (!cmsg_u || cmsg_len < sizeof(struct user_cmsghdr64))
        return;

    __u64 off = 0;

#pragma clang loop unroll(full)
    for (int it = 0; it < MAX_CMSG_ITERS; it++) {
        if (off + sizeof(struct user_cmsghdr64) > cmsg_len)
            break;

        struct user_cmsghdr64 ch = {};
        if (bpf_probe_read_user(&ch, sizeof(ch), (void *)(unsigned long)(cmsg_u + off)) != 0)
            break;

        // sanity
        if (ch.cmsg_len < sizeof(struct user_cmsghdr64))
            break;

        __u64 this_len = ch.cmsg_len;
        if (off + this_len > cmsg_len)
            break;

        __u64 data_off = off + sizeof(struct user_cmsghdr64);

        // IPv4 pktinfo -> local dst for recvmsg / specified src for sendmsg
        if (ch.cmsg_level == SOL_IP && ch.cmsg_type == IP_PKTINFO) {
            struct user_in_pktinfo pi = {};
            if (data_off + sizeof(pi) <= cmsg_len &&
                bpf_probe_read_user(&pi, sizeof(pi), (void *)(unsigned long)(cmsg_u + data_off)) == 0) {

                info->family = AF_INET;
                if (set_dst_local) {
                    // local address (dst) for receive path
                    info->dst_ip4 = pi.ipi_spec_dst ? pi.ipi_spec_dst : pi.ipi_addr;
                } else {
                    // src hint for send path
                    info->src_ip4 = pi.ipi_spec_dst ? pi.ipi_spec_dst : pi.ipi_addr;
                }
            }
        }

        // IPv6 pktinfo
        if (ch.cmsg_level == SOL_IPV6 && ch.cmsg_type == IPV6_PKTINFO) {
            struct user_in6_pktinfo pi6 = {};
            if (data_off + sizeof(pi6) <= cmsg_len &&
                bpf_probe_read_user(&pi6, sizeof(pi6), (void *)(unsigned long)(cmsg_u + data_off)) == 0) {

                info->family = AF_INET6;
                if (set_dst_local) {
                    __builtin_memcpy(info->dst_ip6, pi6.ipi6_addr, 16);
                } else {
                    __builtin_memcpy(info->src_ip6, pi6.ipi6_addr, 16);
                }
            }
        }

        // move to next cmsg (aligned)
        off += CMSG_ALIGN(this_len);
        if (off >= cmsg_len)
            break;
    }
}



static __always_inline __u32 min_u32(__u32 a, __u32 b) { return a < b ? a : b; }

static __always_inline int read_sys_exit_ret(struct trace_event_raw_sys_exit *ctx, __s64 *ret)
{
    if (BPF_CORE_READ_INTO(ret, ctx, ret) < 0)
        return -1;
    return 0;
}

static __always_inline int read_msghdr64(__u64 msg_u, struct user_msghdr64 *h)
{
    if (!msg_u)
        return -1;
    if (bpf_probe_read_user(h, sizeof(*h), (void *)msg_u) < 0)
        return -1;
    return 0;
}

static __always_inline int read_mmsghdr0(__u64 vec_u, struct user_mmsghdr64 *out)
{
    if (!vec_u)
        return -1;
    if (bpf_probe_read_user(out, sizeof(*out), (void *)vec_u) < 0)
        return -1;
    return 0;
}

/* recv: sum msg_len (kernel fills it) */
static __always_inline __s64 sum_mmsg_len(__u64 vec_u, __u32 n)
{
    __s64 total = 0;

#pragma clang loop unroll(full)
    for (int i = 0; i < MAX_MMSG; i++) {
        if ((__u32)i >= n)
            continue;
        struct user_mmsghdr64 mh = {};
        __u64 p = vec_u + (__u64)i * (__u64)sizeof(struct user_mmsghdr64);
        if (bpf_probe_read_user(&mh, sizeof(mh), (void *)p) == 0)
            total += (__s64)mh.msg_len;
    }
    return total;
}

/* send: best-effort sum iov_len (clamped) */
static __always_inline __s64 sum_mmsg_iov_bytes(__u64 vec_u, __u32 n)
{
    __s64 total = 0;

#pragma clang loop unroll(full)
    for (int i = 0; i < MAX_MMSG; i++) {
        if ((__u32)i >= n)
            continue;

        struct user_mmsghdr64 mh = {};
        __u64 p = vec_u + (__u64)i * (__u64)sizeof(struct user_mmsghdr64);
        if (bpf_probe_read_user(&mh, sizeof(mh), (void *)p) != 0)
            continue;

        if (!mh.msg_hdr.msg_iov || mh.msg_hdr.msg_iovlen == 0)
            continue;

        __u32 iovcnt = (mh.msg_hdr.msg_iovlen > 0xffffffffULL) ? 0xffffffffU : (__u32)mh.msg_hdr.msg_iovlen;
        iovcnt = min_u32(iovcnt, MAX_IOV);

#pragma clang loop unroll(full)
        for (int j = 0; j < MAX_IOV; j++) {
            if ((__u32)j >= iovcnt)
                continue;
            struct user_iovec64 iv = {};
            __u64 ip = mh.msg_hdr.msg_iov + (__u64)j * (__u64)sizeof(struct user_iovec64);
            if (bpf_probe_read_user(&iv, sizeof(iv), (void *)ip) == 0)
                total += (__s64)iv.iov_len;
        }
    }
    return total;
}

/* ---- fd->file helpers ---- */

static __always_inline struct file *file_from_fd(int fd)
{
    if (fd < 0)
        return 0;

    struct task_struct *task = (struct task_struct *)bpf_get_current_task_btf();
    if (!task)
        return 0;

    struct files_struct *files = BPF_CORE_READ(task, files);
    if (!files)
        return 0;

    struct fdtable *fdt = BPF_CORE_READ(files, fdt);
    if (!fdt)
        return 0;

    int max_fds = BPF_CORE_READ(fdt, max_fds);
    if (fd >= max_fds)
        return 0;

    struct file **fd_array = BPF_CORE_READ(fdt, fd);
    if (!fd_array)
        return 0;

    struct file *file = 0;
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd] < 0 ? 0 : &fd_array[fd]) < 0 || !file)
        ; // ignore
    // the above line is a bit defensive; typical kernels are fine with plain &fd_array[fd]
    // fallback to direct read:
    if (!file) {
        if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file)
            return 0;
    }
    return file;
}

static __always_inline int is_socket_fd(int fd)
{
    struct file *file = file_from_fd(fd);
    if (!file) return 0;

    struct inode *inode = BPF_CORE_READ(file, f_inode);
    if (!inode) return 0;

    __u16 mode = BPF_CORE_READ(inode, i_mode);
    return (mode & S_IFMT) == S_IFSOCK;
}

/* cookie = socket inode number */
static __always_inline __u64 cookie_from_fd(int fd)
{
    struct file *file = file_from_fd(fd);
    if (!file)
        return 0;

    struct inode *inode = BPF_CORE_READ(file, f_inode);
    if (!inode)
        return 0;

    return (__u64)BPF_CORE_READ(inode, i_ino);
}

static __always_inline struct sock *sock_from_fd(int fd)
{
    struct file *file = file_from_fd(fd);
    if (!file)
        return 0;

    void *pd = BPF_CORE_READ(file, private_data);
    struct socket *sock = (struct socket *)pd;
    if (!sock)
        return 0;

    return BPF_CORE_READ(sock, sk);
}

/* ---- sockaddr from user (AF_INET/AF_INET6) ---- */

static __always_inline int fill_from_sockaddr_user(struct trace_info *info,
                                                   const void *uaddr,
                                                   __u32 addrlen,
                                                   int fill_dst)
{
    __u16 family = 0;
    if (!uaddr || addrlen < sizeof(__u16))
        return -1;

    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0)
        return -1;

    if (info->family == 0)
        info->family = family;

    if (family == AF_INET) {
        if (addrlen < sizeof(struct sockaddr_in))
            return -1;
        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa.sin_port);

        if (fill_dst) {
            info->dst_ip4 = sa.sin_addr.s_addr;
            if (port) info->dport = port;
        } else {
            info->src_ip4 = sa.sin_addr.s_addr;
            if (port) info->sport = port;
        }
        return 0;
    }

    if (family == AF_INET6) {
        if (addrlen < sizeof(struct sockaddr_in6))
            return -1;
        struct sockaddr_in6 sa6 = {};
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa6.sin6_port);
        __u32 scope = sa6.sin6_scope_id;

        if (fill_dst) {
            __builtin_memcpy(info->dst_ip6, &sa6.sin6_addr, 16);
            if (port) info->dport = port;
            if (scope) info->dst_scope = scope;
        } else {
            __builtin_memcpy(info->src_ip6, &sa6.sin6_addr, 16);
            if (port) info->sport = port;
            if (scope) info->src_scope = scope;
        }
        return 0;
    }

    return -1;
}

/* ---- local addr helpers (IPv4) ---- */

static __always_inline __u32 ipv4_local_addr(struct sock *sk)
{
    __u32 a = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
    if (a == 0) {
        struct inet_sock *inet = (struct inet_sock *)sk;
        a = BPF_CORE_READ(inet, inet_saddr);
    }
    return a;
}

static __always_inline int is_ipv4_loopback(__u32 addr_be)
{
    if (addr_be == 0)
        return 0;
    __u32 h = bpf_ntohl(addr_be);
    return ((h >> 24) == 127);
}

static __always_inline int is_ipv6_loopback(const __u8 a[16])
{
#pragma clang loop unroll(full)
    for (int i = 0; i < 15; i++) {
        if (a[i] != 0)
            return 0;
    }
    return a[15] == 1;
}

static __always_inline void loopback_fallback(struct trace_info *info, int is_send_dir)
{
    if (info->family == AF_INET) {
        if (is_send_dir) {
            if (info->src_ip4 == 0 && is_ipv4_loopback(info->dst_ip4))
                info->src_ip4 = bpf_htonl(0x7f000001);
        } else {
            if (info->dst_ip4 == 0 && is_ipv4_loopback(info->src_ip4))
                info->dst_ip4 = bpf_htonl(0x7f000001);
        }
    } else if (info->family == AF_INET6) {
        if (is_send_dir) {
            if (is_ipv6_loopback(info->dst_ip6)) {
                int all0 = 1;
#pragma clang loop unroll(full)
                for (int i = 0; i < 16; i++) if (info->src_ip6[i] != 0) all0 = 0;
                if (all0) {
#pragma clang loop unroll(full)
                    for (int i = 0; i < 15; i++) info->src_ip6[i] = 0;
                    info->src_ip6[15] = 1;
                }
            }
        } else {
            int all0 = 1;
#pragma clang loop unroll(full)
            for (int i = 0; i < 16; i++) if (info->dst_ip6[i] != 0) all0 = 0;
            if (all0 && is_ipv6_loopback(info->src_ip6)) {
#pragma clang loop unroll(full)
                for (int i = 0; i < 15; i++) info->dst_ip6[i] = 0;
                info->dst_ip6[15] = 1;
            }
        }
    }
}

/* ---- fd state ---- */

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st)
{
    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return -1;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);

    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);

    __u16 dport_be = BPF_CORE_READ(sk, __sk_common.skc_dport);
    st->rport = bpf_ntohs(dport_be);

    if (st->family == AF_INET) {
        st->lip = ipv4_local_addr(sk);
        st->rip = BPF_CORE_READ(sk, __sk_common.skc_daddr);
        return 0;
    }

    if (st->family == AF_INET6) {
        if (BPF_CORE_READ_INTO(&st->lip6, sk, __sk_common.skc_v6_rcv_saddr) < 0)
            return -1;
        if (BPF_CORE_READ_INTO(&st->rip6, sk, __sk_common.skc_v6_daddr) < 0)
            return -1;
        return 0;
    }

    return -1;
}

static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send_dir)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t tmp = {};

    /*
     * A numeric fd can be reused without us observing close(2) (for example
     * when libc or the runtime uses close_range).  Never trust a cached
     * protocol/address for a new syscall: refresh it from the live socket.
     * The map remains useful to the connect/accept paths, but correctness must
     * not depend on receiving every close event.
     */
    if (fill_fd_state(fd, &tmp) < 0)
        return -1;
    bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
    struct fd_state_t *st = &tmp;

    info->proto  = st->proto;
    info->family = st->family;

    if (st->family == AF_INET) {
        if (is_send_dir) {
            info->src_ip4 = st->lip;
            info->dst_ip4 = st->rip;
            info->sport   = st->lport;
            info->dport   = st->rport;
        } else {
            info->src_ip4 = st->rip;
            info->dst_ip4 = st->lip;
            info->sport   = st->rport;
            info->dport   = st->lport;
        }
        loopback_fallback(info, is_send_dir);
        return 0;
    }

    if (st->family == AF_INET6) {
        if (is_send_dir) {
            __builtin_memcpy(info->src_ip6, &st->lip6, 16);
            __builtin_memcpy(info->dst_ip6, &st->rip6, 16);
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            __builtin_memcpy(info->src_ip6, &st->rip6, 16);
            __builtin_memcpy(info->dst_ip6, &st->lip6, 16);
            info->sport = st->rport;
            info->dport = st->lport;
        }
        loopback_fallback(info, is_send_dir);
        return 0;
    }

    return -1;
}

static __always_inline void fill_ids_comm_cookie(struct trace_info *info, __u64 pid_tgid, int fd, const char *comm64_opt)
{
    info->ts_ns  = bpf_ktime_get_ns();
    info->tgid   = pid_tgid >> 32;
    info->tid    = (__u32)pid_tgid;
    info->cookie = cookie_from_fd(fd);

    if (comm64_opt) {
        __builtin_memcpy(info->comm, comm64_opt, sizeof(info->comm));
    } else {
        bpf_get_current_comm(info->comm, sizeof(info->comm));
    }
}

/* ===== NEW: emit TLS chunk (perf) using per-cpu scratch ===== */

static __always_inline int emit_tls_chunk(void *ctx,
                                          __u64 id,
                                          __u32 tgid,
                                          struct conn_info_t *ci,
                                          struct trace_info *info,
                                          const void *user_ptr,
                                          __s64 ret_bytes)
{
    if (!user_ptr)
        return 0;

    if (!info->cookie || info->proto != IPPROTO_TCP)
        return 0;

    if (!(info->dport == 443 || info->sport == 443))
        return 0;

    __u64 cookie = info->cookie;

    __u8 *done = bpf_map_lookup_elem(&tls_done_map, &cookie);
    if (done && *done)
        return 0;

    __u32 key0 = 0;
    struct tls_chunk_event *ev = bpf_map_lookup_elem(&tls_chunk_scratch, &key0);
    if (!ev)
        return 0;

    __builtin_memset(ev, 0, sizeof(*ev));

    __u32 n = (__u32)ret_bytes;
    if (n > TLS_CHUNK_MAX)
        n = TLS_CHUNK_MAX;
    if (n == 0)
        return 0;

    ev->cookie = cookie;
    ev->ts_ns  = bpf_ktime_get_ns();
    ev->tgid   = tgid;
    ev->tid    = (__u32)id;
    ev->fd     = (__s32)ci->fd;
    ev->proto  = info->proto;
    ev->event  = EV_TLS_CHUNK;
    ev->sport  = info->sport;
    ev->dport  = info->dport;
    ev->len    = n;

    __u32 *seqp = bpf_map_lookup_elem(&tls_seq_map, &cookie);
    __u32 seq = seqp ? *seqp : 0;
    ev->seq = seq;
    seq++;
    bpf_map_update_elem(&tls_seq_map, &cookie, &seq, BPF_ANY);

    if (bpf_probe_read_user(ev->data, n, user_ptr) != 0)
        return 0;

    bpf_perf_event_output(ctx, &tls_events, BPF_F_CURRENT_CPU, ev, sizeof(*ev));
    return 0;
}



            

