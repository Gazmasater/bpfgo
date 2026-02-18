//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_helpers.h>

#define AF_INET  2
#define AF_INET6 10

#define EINPROGRESS 115
#define EALREADY    114

// events
#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_BINDOK    20
#define EV_SENDMSG   11
#define EV_RECVMSG   12

// tcp states (linux)
#define TCP_ESTABLISHED 1
#define TCP_SYN_SENT    2
#define TCP_CLOSE       7

// owner kinds
#define OWN_KIND_NONE    0
#define OWN_KIND_CONNECT 1
#define OWN_KIND_ACCEPT  2

/* ====== types ====== */

struct conn_info_t {
    __u32 pid;     // tgid
    __u32 fd;
    char  comm[64];
};

struct fd_key_t {
    __u32 tgid;
    __s32 fd;
};

struct fd_state_t {
    __u16 family;
    __u8  proto;

    __u16 lport;   // local
    __u16 rport;   // remote

    __u32 lip;     // local IPv4 (net order)
    __u32 rip;     // remote IPv4 (net order)

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct inflight_fd_t {
    __s32 fd;
};

// owner by skaddr (needed because inet_sock_set_state can run in softirq context)
struct sk_owner_t {
    __u32 pid;       // tgid
    __u32 fd;
    __s64 conn_ret;  // from sys_exit_connect (0 / -115 / ...)
    __u8  kind;      // OWN_KIND_*
    __u8  emitted;   // EV_CONNECT already emitted?
    __u8  _pad[6];
    char  comm[32];
};

struct trace_info {
    // IPv4
    struct in_addr  srcIP;
    struct in_addr  dstIP;

    // IPv6
    struct in6_addr srcIP6;
    struct in6_addr dstIP6;

    __u32 pid;
    __u32 proto;

    __u32 fd;
    __s32 _pad0;   // align ret
    __s64 ret;     // sys_exit return / bytes / etc.

    __u16 sport;
    __u16 dport;
    __u16 family;

    __u8  sysexit; // event code
    __u8  state;   // for TCP: newstate

    char  comm[32];
};

const struct trace_info *unused __attribute__((unused));

/* user msghdr head (enough for msg_name + msg_namelen) */
struct user_msghdr_head {
    void *msg_name;     // 8 bytes on amd64
    __u32 msg_namelen;  // 4 bytes
    __u32 _pad;         // align
};

struct addr_ptrlen_t {
    __u64 addr;   // user sockaddr*
    __u32 len;    // sockaddr len
    __u32 _pad;
};

struct addr_recv_meta_t {
    __u64 addr;   // user sockaddr* (kernel fills)
    __u64 lenp;   // user socklen_t* (kernel fills)
};

/* ====== maps ====== */

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, struct fd_state_t);
} fd_state_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);   // pid_tgid
    __type(value, struct inflight_fd_t);
} connect_fd_map SEC(".maps");

/* connect: (sockaddr*, len) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrConnect_map SEC(".maps");

/* bind: (sockaddr*, len) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrBind_map SEC(".maps");

/* sendto: (sockaddr*, len) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrSend_map SEC(".maps");

/* recvfrom: (sockaddr*, socklen_t*) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_recv_meta_t);
} addrRecv_map SEC(".maps");

/* stack-safe: separate maps for msghdr* */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);    // pid_tgid
    __type(value, __u64);  // user msghdr*
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);    // pid_tgid
    __type(value, __u64);  // user msghdr*
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64); // pid_tgid
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

// skaddr -> owner (LRU to avoid leaks)
struct {
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 65536);
    __type(key, __u64);                 // skaddr
    __type(value, struct sk_owner_t);
} sk_owner_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ====== helpers ====== */

static __always_inline int read_sys_exit_ret(struct trace_event_raw_sys_exit *ctx, __s64 *ret)
{
    if (BPF_CORE_READ_INTO(ret, ctx, ret) < 0)
        return -1;
    return 0;
}

static __always_inline int read_msghdr_head(__u64 msg_u, struct user_msghdr_head *h)
{
    if (!msg_u)
        return -1;
    if (bpf_probe_read_user(h, sizeof(*h), (void *)msg_u) < 0)
        return -1;
    return 0;
}

// IMPORTANT: do NOT fail when port==0.
// Just "don't overwrite" ports if they are already known from fd_state.
static __always_inline int fill_from_sockaddr_user(struct trace_info *info,
                                                   const void *uaddr,
                                                   __u32 addrlen,
                                                   int is_dst)
{
    __u16 family = 0;

    if (!uaddr || addrlen < sizeof(__u16))
        return -1;

    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0)
        return -1;

    if (family == AF_INET) {
        if (addrlen < sizeof(struct sockaddr_in))
            return -1;

        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa.sin_port);

        info->family = AF_INET;
        if (is_dst) {
            info->dstIP.s_addr = sa.sin_addr.s_addr; // net order
            if (port) info->dport = port;
        } else {
            info->srcIP.s_addr = sa.sin_addr.s_addr; // net order
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

        info->family = AF_INET6;
        if (is_dst) {
            __builtin_memcpy(&info->dstIP6, &sa6.sin6_addr, sizeof(info->dstIP6));
            if (port) info->dport = port;
        } else {
            __builtin_memcpy(&info->srcIP6, &sa6.sin6_addr, sizeof(info->srcIP6));
            if (port) info->sport = port;
        }
        return 0;
    }

    return -1;
}

/* ---------- fd -> sock (best effort) ---------- */

static __always_inline struct sock *sock_from_fd(int fd)
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
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file)
        return 0;

    struct socket *sock = 0;
    if (bpf_probe_read_kernel(&sock, sizeof(sock), &file->private_data) < 0 || !sock)
        return 0;

    return BPF_CORE_READ(sock, sk);
}

static __always_inline int fill_fd_state_from_sock(struct sock *sk, struct fd_state_t *st)
{
    if (!sk)
        return -1;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);

    // local port in host order
    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);

    // remote port in network order -> host
    __u16 dport_be = BPF_CORE_READ(sk, __sk_common.skc_dport);
    st->rport = bpf_ntohs(dport_be);

    if (st->family == AF_INET) {
        st->lip = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
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

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st)
{
    struct sock *sk = sock_from_fd(fd);
    return fill_fd_state_from_sock(sk, st);
}

/* ---------- fd_state_map helpers ---------- */

static __always_inline int apply_state_to_info(struct trace_info *info,
                                               const struct fd_state_t *st,
                                               int is_send)
{
    info->proto  = st->proto;
    info->family = st->family;

    if (st->family == AF_INET) {
        if (is_send) {
            info->srcIP.s_addr = st->lip;
            info->dstIP.s_addr = st->rip;
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            info->srcIP.s_addr = st->rip;
            info->dstIP.s_addr = st->lip;
            info->sport = st->rport;
            info->dport = st->lport;
        }
        return 0;
    }

    if (st->family == AF_INET6) {
        if (is_send) {
            __builtin_memcpy(&info->srcIP6, &st->lip6, sizeof(info->srcIP6));
            __builtin_memcpy(&info->dstIP6, &st->rip6, sizeof(info->dstIP6));
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            __builtin_memcpy(&info->srcIP6, &st->rip6, sizeof(info->srcIP6));
            __builtin_memcpy(&info->dstIP6, &st->lip6, sizeof(info->dstIP6));
            info->sport = st->rport;
            info->dport = st->lport;
        }
        return 0;
    }

    return -1;
}

// try map, else snapshot from fd and cache
static __always_inline int fill_from_fd_state_any(struct trace_info *info, __u32 tgid, int fd, int is_send)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t *stp = bpf_map_lookup_elem(&fd_state_map, &k);
    if (stp)
        return apply_state_to_info(info, stp, is_send);

    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) < 0)
        return -1;

    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
    return apply_state_to_info(info, &st, is_send);
}

/* ---------- sk_owner_map ---------- */

static __always_inline void remember_owner_for_fd(__u32 tgid, int fd, __s64 conn_ret, __u8 kind)
{
    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return;

    __u64 skk = (__u64)(unsigned long)(const void *)sk; // ✅ same cast as inet_sock_set_state

    struct sk_owner_t own = {};
    own.pid = tgid;
    own.fd  = (__u32)fd;
    own.conn_ret = conn_ret;
    own.kind = kind;
    own.emitted = 0;

    bpf_get_current_comm(own.comm, sizeof(own.comm));
    bpf_map_update_elem(&sk_owner_map, &skk, &own, BPF_ANY);
}

/* ====== connect ====== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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

// NOTE: we do NOT emit EV_CONNECT here.
// We only populate fd_state_map + sk_owner_map.
// Real "TCP CONNECT" print comes from inet_sock_set_state when local port is assigned.
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

    // best-effort snapshot
    struct fd_state_t st = {};
    if (fill_fd_state(in->fd, &st) == 0) {
        struct fd_key_t k = { .tgid = tgid, .fd = in->fd };
        bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
    }

    // bind skaddr -> owner for inet_sock_set_state
    remember_owner_for_fd(tgid, in->fd, ret, OWN_KIND_CONNECT);

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== inet_sock_set_state (TCP port becomes non-zero) ====== */

SEC("tracepoint/sock/inet_sock_set_state")
int trace_inet_sock_set_state(struct trace_event_raw_inet_sock_set_state *ctx)
{
    const void *skp = BPF_CORE_READ(ctx, skaddr);
    if (!skp)
        return 0;

    __u64 skaddr = (__u64)(unsigned long)skp; // ✅ key for maps, no clang warning
    __u32 newstate = BPF_CORE_READ(ctx, newstate);

    struct sk_owner_t *own = bpf_map_lookup_elem(&sk_owner_map, &skaddr);
    if (!own)
        return 0;

    if (newstate != TCP_SYN_SENT && newstate != TCP_ESTABLISHED && newstate != TCP_CLOSE)
        return 0;

    struct sock *sk = (struct sock *)skp;

    // update fd_state_map from sock (most accurate)
    struct fd_state_t st = {};
    if (fill_fd_state_from_sock(sk, &st) < 0)
        return 0;

    struct fd_key_t fk = { .tgid = own->pid, .fd = (__s32)own->fd };
    bpf_map_update_elem(&fd_state_map, &fk, &st, BPF_ANY);

    // cleanup on CLOSE
    if (newstate == TCP_CLOSE) {
        bpf_map_delete_elem(&sk_owner_map, &skaddr);
        return 0;
    }

    // emit CONNECT only for sockets that came from sys_exit_connect
    if (own->kind != OWN_KIND_CONNECT)
        return 0;

    // emit once when local port is known
    // prefer SYN_SENT, but if lport==0 there, emit later on ESTABLISHED
    if (!own->emitted && st.lport != 0 &&
        (newstate == TCP_SYN_SENT || newstate == TCP_ESTABLISHED))
    {
        struct trace_info info = {};
        info.sysexit = EV_CONNECT;
        info.state   = (__u8)newstate;

        info.pid = own->pid;
        info.fd  = own->fd;
        info.ret = own->conn_ret;

        __builtin_memcpy(info.comm, own->comm, sizeof(info.comm));

        // fill addrs/ports from st
        info.proto  = st.proto;
        info.family = st.family;
        info.sport  = st.lport;
        info.dport  = st.rport;

        if (st.family == AF_INET) {
            info.srcIP.s_addr = st.lip;
            info.dstIP.s_addr = st.rip;
        } else if (st.family == AF_INET6) {
            __builtin_memcpy(&info.srcIP6, &st.lip6, sizeof(info.srcIP6));
            __builtin_memcpy(&info.dstIP6, &st.rip6, sizeof(info.dstIP6));
        } else {
            return 0;
        }

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
        own->emitted = 1;
    }

    return 0;
}

/* ====== accept/accept4 ====== */

static __always_inline int accept_enter_common(void)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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

    // snapshot state
    struct fd_state_t st = {};
    if (fill_fd_state((int)newfd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)newfd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.sysexit = EV_ACCEPT;
    info.state   = 0;

    info.pid     = tgid;
    info.fd      = (__u32)newfd;
    info.ret     = newfd; // accept() return = new fd

    // accept: src=remote, dst=local
    if (apply_state_to_info(&info, &st, /*is_send=*/0) < 0)
        goto cleanup;

    if (conn) __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else bpf_get_current_comm(info.comm, sizeof(info.comm));

    // remember owner to keep fd_state fresh on state changes (but DO NOT emit EV_CONNECT)
    remember_owner_for_fd(tgid, (int)newfd, 0, OWN_KIND_ACCEPT);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(); }

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(); }

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

/* ====== close ====== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u32 tgid = bpf_get_current_pid_tgid() >> 32;
    int fd     = (int)ctx->args[0];

    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);

    // best-effort remove sk_owner
    struct sock *sk = sock_from_fd(fd);
    if (sk) {
        __u64 skk = (__u64)(unsigned long)(const void *)sk;
        bpf_map_delete_elem(&sk_owner_map, &skk);
    }

    return 0;
}

/* ====== bind ====== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[1];
    __u32 addrlen = (__u32)ctx->args[2];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = {.addr = uaddr, .len = addrlen};
        bpf_map_update_elem(&addrBind_map, &id, &v, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrBind_map, &id);
    if (!ap || !ap->addr || !ap->len)
        goto cleanup;

    // cache fd state after successful bind (helps show real local ip/port for UDP)
    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) == 0) {
        struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
        bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
    }

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.pid     = ci->pid;
    info.sysexit = EV_BINDOK;

    info.fd  = ci->fd;
    info.ret = ret;

    // for bind we put bound address into dst fields (server side)
    (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    if (info.family == 0) {
        // fallback: if sockaddr parsing failed, try at least to set family from fd
        (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 1);
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== sendto ====== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

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
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_SENDTO;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // try to fill src/dst from fd state (TCP connected sockets will work)
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 1);

    // override dst from user sockaddr if provided (UDP sendto)
    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvfrom ====== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];
    if (uaddr && lenp_u) {
        struct addr_recv_meta_t m = {.addr = uaddr, .lenp = lenp_u};
        bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx)
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
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_RECVFROM;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // for recv: src=remote, dst=local
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 0);

    // override src from user sockaddr if kernel filled it
    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && m->addr && m->lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void *)m->lenp) == 0) {
            if (addrlen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)m->addr, addrlen, 0);
        }
    }

    if (info.family == 0)
        goto cleanup;

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
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    if (msg_u)
        bpf_map_update_elem(&msgSend_map, &id, &msg_u, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx)
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
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_SENDMSG;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // try fd_state for connected sockets
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 1);

    // if msg_name present -> dst from it (often UDP)
    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(*msgp, &h) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 1);
        }
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvmsg ====== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    if (msg_u)
        bpf_map_update_elem(&msgRecv_map, &id, &msg_u, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx)
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
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_RECVMSG;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // recv: src=remote, dst=local
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 0);

    // if msg_name present -> src from it (depends on caller setup)
    __u64 *msgp = bpf_map_lookup_elem(&msgRecv_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(*msgp, &h) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 0);
        }
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}