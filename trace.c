//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_helpers.h>

/* ====== types ====== */

#define AF_INET  2
#define AF_INET6 10
#define EINPROGRESS 115
#define EALREADY    114

struct conn_info_t {
    __u32 pid;
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

    __u32 lip;     // local IPv4 (network order)
    __u32 rip;     // remote IPv4 (network order)

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct inflight_fd_t {
    __s32 fd;
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
    __u16 sport;
    __u16 dport;
    __u16 family;

    __u8  sysexit; // event code
    __u8  state;

    char  comm[32];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user pointer to sockaddr
} addrConnect_map SEC(".maps");


const struct trace_info *unused __attribute__((unused));

/* user msghdr head (enough for msg_name + msg_namelen) */
struct user_msghdr_head {
    void *msg_name;     // 8 bytes on amd64
    __u32 msg_namelen;  // 4 bytes
    __u32 _pad;         // align
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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user pointer
} addrBind_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user pointer
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user pointer
} addrRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64); // pid_tgid
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

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

static __always_inline int fill_from_sockaddr(struct trace_info *info, const void *uaddr, int is_dst)
{
    __u16 family = 0;

    if (!uaddr)
        return -1;

    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0)
        return -1;

    if (family == AF_INET) {
        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa.sin_port);
        if (port == 0)
            return -1;

        info->family = AF_INET;

        if (is_dst) {
            info->dport = port;
            info->dstIP.s_addr = sa.sin_addr.s_addr; // keep network order
        } else {
            info->sport = port;
            info->srcIP.s_addr = sa.sin_addr.s_addr; // keep network order
        }
        return 0;
    }

    if (family == AF_INET6) {
        struct sockaddr_in6 sa6 = {};
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa6.sin6_port);
        if (port == 0)
            return -1;

        info->family = AF_INET6;

        if (is_dst) {
            info->dport = port;
            __builtin_memcpy(&info->dstIP6, &sa6.sin6_addr, sizeof(info->dstIP6));
        } else {
            info->sport = port;
            __builtin_memcpy(&info->srcIP6, &sa6.sin6_addr, sizeof(info->srcIP6));
        }
        return 0;
    }

    return -1;
}

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

static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t *st = bpf_map_lookup_elem(&fd_state_map, &k);
    if (!st)
        return -1;

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

/* fallbacks when fd_state_map has no entry */
static __always_inline void fill_local_src_from_fd(struct trace_info *info, int fd)
{
    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) < 0)
        return;

    info->proto  = st.proto;
    info->family = st.family;

    if (st.family == AF_INET) {
        info->srcIP.s_addr = st.lip;
        info->sport        = st.lport;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info->srcIP6, &st.lip6, sizeof(info->srcIP6));
        info->sport = st.lport;
    }
}

static __always_inline void fill_local_dst_from_fd(struct trace_info *info, int fd)
{
    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) < 0)
        return;

    info->proto  = st.proto;
    info->family = st.family;

    if (st.family == AF_INET) {
        info->dstIP.s_addr = st.lip;
        info->dport        = st.lport;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info->dstIP6, &st.lip6, sizeof(info->dstIP6));
        info->dport = st.lport;
    }
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
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct inflight_fd_t in = {};
    in.fd = (int)ctx->args[0];
    bpf_map_update_elem(&connect_fd_map, &id, &in, BPF_ANY);

    // sockaddr* аргумент connect(fd, sockaddr*, addrlen)
    __u64 uaddr = (__u64)ctx->args[1];
    if (uaddr)
        bpf_map_update_elem(&addrConnect_map, &id, &uaddr, BPF_ANY);

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

    // allow non-blocking connect
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
    info.sysexit = 3;          // CONNECT
    info.pid     = tgid;
    info.proto   = st.proto;
    info.family  = st.family;
    info.sport   = st.lport;
    info.dport   = st.rport;

    // 0 = ok, 1 = in-progress
    info.state   = (ret < 0) ? 1 : 0;

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm(info.comm, sizeof(info.comm));

    // базово из sock
    if (st.family == AF_INET) {
        info.srcIP.s_addr = st.lip;
        info.dstIP.s_addr = st.rip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info.srcIP6, &st.lip6, sizeof(info.srcIP6));
        __builtin_memcpy(&info.dstIP6, &st.rip6, sizeof(info.dstIP6));
    } else {
        goto cleanup;
    }

    // override dst из connect(sockaddr*), если есть
    __u64 *uaddrp = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (uaddrp && *uaddrp)
        (void)fill_from_sockaddr(&info, (void *)*uaddrp, /*is_dst=*/1);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== accept/accept4 (common inline, no calling other programs) ====== */

static __always_inline int accept_enter_common(void)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
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
    info.sysexit = 4;
    info.pid     = tgid;
    info.proto   = st.proto;
    info.family  = st.family;

    // incoming: src=remote, dst=local
    info.sport = st.rport;
    info.dport = st.lport;

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm(info.comm, sizeof(info.comm));

    if (st.family == AF_INET) {
        info.srcIP.s_addr = st.rip;
        info.dstIP.s_addr = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info.srcIP6, &st.rip6, sizeof(info.srcIP6));
        __builtin_memcpy(&info.dstIP6, &st.lip6, sizeof(info.dstIP6));
    } else {
        goto cleanup;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx)
{
    return accept_enter_common();
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx)
{
    return accept_exit_common(ctx);
}

SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx)
{
    return accept_enter_common();
}

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx)
{
    return accept_exit_common(ctx);
}

/* ====== close ====== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd     = (int)ctx->args[0];

    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);
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
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr = (__u64)ctx->args[1];
    if (uaddr)
        bpf_map_update_elem(&addrBind_map, &id, &uaddr, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    __u64 *uaddrp = bpf_map_lookup_elem(&addrBind_map, &id);
    if (!uaddrp || !*uaddrp)
        goto cleanup;

    void *uaddr = (void *)(*uaddrp);

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.pid     = ci->pid;
    info.sysexit = 20;

    // bind: treat as dst(local) endpoint in dst fields for convenience
    if (fill_from_sockaddr(&info, uaddr, /*is_dst=*/1) < 0)
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
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr = (__u64)ctx->args[4]; // dest sockaddr*
    if (uaddr)
        bpf_map_update_elem(&addrSend_map, &id, &uaddr, BPF_ANY);

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
    info.sysexit = 1;
    info.pid     = ci->pid;

    // prefer full tuple from map, else fallback local src
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, /*is_send=*/1) < 0)
        fill_local_src_from_fd(&info, (int)ci->fd);

    // override dst from sendto sockaddr
    __u64 *uaddrp = bpf_map_lookup_elem(&addrSend_map, &id);
    if (uaddrp && *uaddrp)
        (void)fill_from_sockaddr(&info, (void *)*uaddrp, /*is_dst=*/1);

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
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr = (__u64)ctx->args[4]; // src sockaddr* (kernel fills it)
    if (uaddr)
        bpf_map_update_elem(&addrRecv_map, &id, &uaddr, BPF_ANY);

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
    info.sysexit = 2;
    info.pid     = ci->pid;

    // prefer full tuple from map, else fallback local dst
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, /*is_send=*/0) < 0)
        fill_local_dst_from_fd(&info, (int)ci->fd);

    // override src from recvfrom sockaddr
    __u64 *uaddrp = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (uaddrp && *uaddrp)
        (void)fill_from_sockaddr(&info, (void *)*uaddrp, /*is_dst=*/0);

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
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1]; // user msghdr*
    if (msg_u)
        bpf_map_update_elem(&addrSend_map, &id, &msg_u, BPF_ANY);

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
    info.sysexit = 11;
    info.pid     = ci->pid;

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, /*is_send=*/1) < 0)
        fill_local_src_from_fd(&info, (int)ci->fd);

    // override dst if msg_name != NULL
    __u64 *msgp = bpf_map_lookup_elem(&addrSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (bpf_probe_read_user(&h, sizeof(h), (void *)*msgp) == 0) {
            if (h.msg_name)
                (void)fill_from_sockaddr(&info, h.msg_name, /*is_dst=*/1);
        }
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
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
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1]; // user msghdr*
    if (msg_u)
        bpf_map_update_elem(&addrRecv_map, &id, &msg_u, BPF_ANY);

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
    info.sysexit = 12;
    info.pid     = ci->pid;

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, /*is_send=*/0) < 0)
        fill_local_dst_from_fd(&info, (int)ci->fd);

    // override src if msg_name != NULL
    __u64 *msgp = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (bpf_probe_read_user(&h, sizeof(h), (void *)*msgp) == 0) {
            if (h.msg_name)
                (void)fill_from_sockaddr(&info, h.msg_name, /*is_dst=*/0);
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
























