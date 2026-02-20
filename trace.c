//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_helpers.h>

#define AF_INET  2
#define AF_INET6 10

#define IPPROTO_ICMP   1
#define IPPROTO_TCP    6
#define IPPROTO_UDP    17
#define IPPROTO_ICMPV6 58

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

// socket flags
#define MSG_PEEK 0x2

/* ====== types ====== */

struct conn_info_t {
    __u32 tgid;
    __u32 fd;            // syscall fd (enter arg0)
    char  comm[64];
};

struct fd_key_t {
    __u32 tgid;
    __s32 fd;
};

struct fd_state_t {
    __u16 family;
    __u8  proto;
    __u8  _pad0;

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

/* EVENT */
struct trace_info {
    // IPv4
    struct in_addr  srcIP;
    struct in_addr  dstIP;

    // IPv6
    struct in6_addr srcIP6;
    struct in6_addr dstIP6;

    __u32 tgid;     // process id
    __u32 tid;      // thread id
    __u32 proto;    // IPPROTO_*

    __u32 fd;       // syscall fd
    __s32 _pad0;
    __s64 ret;      // sys_exit return

    __u16 sport;
    __u16 dport;
    __u16 family;

    __u8  sysexit;  // event code
    __u8  state;    // misc (CONNECT: 1 if inprogress)
    __u16 _pad1;

    __u64 cookie;   // socket inode i_ino (stable per socket lifetime)

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

/* recvfrom meta: include flags */
struct addr_recv_meta_t {
    __u64 addr;    // user sockaddr* (kernel fills)
    __u64 lenp;    // user socklen_t*  (kernel fills)
    __u32 flags;
    __u32 _pad;
};

/* recvmsg meta: msg* + flags */
struct msg_ptrflags_t {
    __u64 msg;     // user msghdr*
    __u32 flags;   // recvmsg flags
    __u32 _pad;
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
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrConnect_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrBind_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_recv_meta_t);
} addrRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);    // pid_tgid
    __type(value, __u64);  // user msghdr*
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                      // pid_tgid
    __type(value, struct msg_ptrflags_t);
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
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

static __always_inline int read_msghdr_head(__u64 msg_u, struct user_msghdr_head *h)
{
    if (!msg_u)
        return -1;
    if (bpf_probe_read_user(h, sizeof(*h), (void *)msg_u) < 0)
        return -1;
    return 0;
}

/* sockaddr from user (AF_INET/AF_INET6) */
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
            info->dstIP.s_addr = sa.sin_addr.s_addr;
            if (port) info->dport = port;
        } else {
            info->srcIP.s_addr = sa.sin_addr.s_addr;
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

/* ---- fd -> file/sock helpers ---- */

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
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file)
        return 0;

    return file;
}

/* cookie = socket inode number (works in tracepoints, unlike bpf_get_socket_cookie on your kernel) */
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

/* ---- fd state ---- */

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st)
{
    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return -1;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);

    // local port (host order already)
    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);

    // remote port is BE
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

/* cache + self-heal:
   - if no cached state -> read from sock and cache
   - if TCP and lport/rport==0 -> re-read and refresh (EINPROGRESS, fd reuse, etc.)
*/
static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };

    struct fd_state_t tmp = {};
    struct fd_state_t *st = bpf_map_lookup_elem(&fd_state_map, &k);

    if (!st) {
        if (fill_fd_state(fd, &tmp) < 0)
            return -1;
        bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
        st = &tmp;
    } else {
        if (st->proto == IPPROTO_TCP && (st->lport == 0 || st->rport == 0)) {
            if (fill_fd_state(fd, &tmp) == 0) {
                bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
                st = &tmp;
            }
        }
    }

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

static __always_inline void fill_ids_comm_cookie(struct trace_info *info, __u64 pid_tgid, int fd, const char *comm64_opt)
{
    info->tgid = pid_tgid >> 32;
    info->tid  = (__u32)pid_tgid;
    info->cookie = cookie_from_fd(fd);

    if (comm64_opt) {
        // copy first 32 bytes
        __builtin_memcpy(info->comm, comm64_opt, sizeof(info->comm));
    } else {
        bpf_get_current_comm(info->comm, sizeof(info->comm));
    }
}

/* ====== connect ====== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd  = (__u32)ctx->args[0];
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
    info.sysexit = EV_CONNECT;
    info.state   = (ret < 0) ? 1 : 0;

    info.fd  = (__u32)in->fd;
    info.ret = ret;

    fill_ids_comm_cookie(&info, id, (int)info.fd, conn ? conn->comm : 0);

    info.proto   = st.proto;
    info.family  = st.family;
    info.sport   = st.lport;
    info.dport   = st.rport;

    if (st.family == AF_INET) {
        info.srcIP.s_addr = st.lip;
        info.dstIP.s_addr = st.rip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info.srcIP6, &st.lip6, sizeof(info.srcIP6));
        __builtin_memcpy(&info.dstIP6, &st.rip6, sizeof(info.dstIP6));
    } else {
        goto cleanup;
    }

    // prefer user-provided dst sockaddr (more accurate on connect)
    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

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
    ci.fd  = (__u32)ctx->args[0]; // listen fd
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
    info.sysexit = EV_ACCEPT;

    // for ACCEPT, fd=listen fd, ret=newfd
    info.fd  = conn ? conn->fd : 0;
    info.ret = newfd;

    // cookie should identify the *new* socket
    info.tgid = tgid;
    info.tid  = (__u32)id;
    info.cookie = cookie_from_fd((int)newfd);

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm(info.comm, sizeof(info.comm));

    info.proto  = st.proto;
    info.family = st.family;

    // accept: remote -> local
    info.sport = st.rport;
    info.dport = st.lport;

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
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(ctx); }

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(ctx); }

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

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
    ci.tgid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
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
    __u64 id   = bpf_get_current_pid_tgid();
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

    // cache fd_state on successful bind (helps UDP before connect)
    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) == 0) {
        struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
        bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
    }

    struct trace_info info = {};
    info.sysexit = EV_BINDOK;
    info.fd      = ci->fd;
    info.ret     = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);
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
    ci.tgid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
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
    info.sysexit = EV_SENDTO;
    info.fd      = ci->fd;
    info.ret     = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        fill_local_src_from_fd(&info, (int)ci->fd);

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

/* ====== recvfrom (FILTER MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u32 flags  = (__u32)ctx->args[3];
    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];

    if ((flags & MSG_PEEK) || (uaddr && lenp_u)) {
        struct addr_recv_meta_t m = {.addr = uaddr, .lenp = lenp_u, .flags = flags};
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

    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && (m->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.sysexit = EV_RECVFROM;
    info.fd      = ci->fd;
    info.ret     = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        fill_local_dst_from_fd(&info, (int)ci->fd);

    // if kernel wrote peer addr
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
    ci.tgid = tgid;
    ci.fd  = (__u32)ctx->args[0];
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
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.sysexit = EV_SENDMSG;
    info.fd      = ci->fd;
    info.ret     = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        fill_local_src_from_fd(&info, (int)ci->fd);

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

/* ====== recvmsg (FILTER MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd  = (__u32)ctx->args[0];
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
    __u64 id   = bpf_get_current_pid_tgid();
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
    info.sysexit = EV_RECVMSG;
    info.fd      = ci->fd;
    info.ret     = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        fill_local_dst_from_fd(&info, (int)ci->fd);

    if (pv && pv->msg) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(pv->msg, &h) == 0) {
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
