//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_helpers.h>

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif


#define AF_INET  2
#define AF_INET6 10

#define IPPROTO_ICMP   1
#define IPPROTO_TCP    6
#define IPPROTO_UDP    17
#define IPPROTO_ICMPV6 58

#define EINPROGRESS 115
#define EALREADY    114

// syscalls events
#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_BINDOK    20
#define EV_SENDMSG   11
#define EV_RECVMSG   12
#define EV_SENDMMSG  13
#define EV_RECVMMSG  14
#define EV_READ      21
#define EV_WRITE     22
#define EV_CLOSE     30

// socket flags
#define MSG_PEEK 0x2

// file modes
#define S_IFMT   0170000
#define S_IFSOCK 0140000

// cmsg constants (Linux)
#define SOL_IP     0
#define SOL_IPV6   41
#define IP_PKTINFO 8
#define IPV6_PKTINFO 50

// bounded parsing limits
#define MAX_MMSG 16
#define MAX_IOV  4
#define MAX_CMSG_STEPS 6

#define CMSG_ALIGN(len) (((len) + sizeof(__u64) - 1) & ~(sizeof(__u64) - 1))
#define EV_SKB_OUT 40

#define EV_TLS_CHUNK  50
#define TLS_SNAP      512

struct tp_net_dev_queue {
    __u16 common_type;
    __u8  common_flags;
    __u8  common_preempt_count;
    __s32 common_pid;

    __u64 skbaddr;     // <-- то, что нам нужно
    __u32 len;
    char  name[IFNAMSIZ];
};



/* ====== types ====== */

struct conn_info_t {
    __u32 tgid;
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
    __u8  _pad0;

    __u16 lport;   // host order
    __u16 rport;   // host order

    __u32 lip;     // net order
    __u32 rip;     // net order

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct inflight_fd_t {
    __s32 fd;
};

/* EVENT (to userspace) */
struct trace_info {
    __u64 ts_ns;
    __u64 cookie;

    __u32 tgid;
    __u32 tid;

    __u32 fd;
    __s32 _pad0;

    __s64 ret;     // bytes (mmsg: best-effort sum)

    __u16 family;
    __u16 sport;
    __u16 dport;

    __u8  proto;
    __u8  event;
    __u8  state;   // mmsg: packets count (clamped to 255); connect: 0/1
    __u8  _pad1;

    __u32 src_ip4; // net order
    __u32 dst_ip4; // net order
    __u8  src_ip6[16];
    __u8  dst_ip6[16];

    __u32 src_scope; // ifindex for IPv6 link-local/mcast when known
    __u32 dst_scope; // ifindex for IPv6 link-local/mcast when known

    char  comm[32];
};

const struct trace_info *unused __attribute__((unused));



struct tls_chunk_t {
    __u64 ts_ns;
    __u64 cookie;

    __u32 tgid;
    __u32 tid;
    __u32 fd;
    __u32 _pad0;

    __u16 family;
    __u16 sport;
    __u16 dport;

    __u8  proto;
    __u8  event;
    __u16 _pad1;

    __u32 len;
    __u8  data[TLS_SNAP];
};

const struct tls_chunk_t *unused_tls __attribute__((unused));


/* ---- userspace ABI (amd64) ---- */

struct user_msghdr64 {
    __u64 msg_name;       // void*
    __u32 msg_namelen;    // socklen_t (fits)
    __u32 _pad0;

    __u64 msg_iov;        // struct iovec*
    __u64 msg_iovlen;     // size_t

    __u64 msg_control;    // void*
    __u64 msg_controllen; // size_t

    __u32 msg_flags;      // int
    __u32 _pad1;
};

struct user_cmsghdr64 {
    __u64 cmsg_len;   // size_t
    __s32 cmsg_level; // int
    __s32 cmsg_type;  // int
};

struct user_in6_pktinfo {
    __u8  ipi6_addr[16];
    __u32 ipi6_ifindex;
};

struct user_in_pktinfo {
    __s32 ipi_ifindex;
    __u32 ipi_spec_dst; // local address of packet
    __u32 ipi_addr;     // dst address in IP header (see ip(7))
};

/* ---- mmsg (amd64) ---- */

struct user_iovec64 {
    __u64 iov_base;
    __u64 iov_len;
};

struct user_mmsghdr64 {
    struct user_msghdr64 msg_hdr;
    __u32 msg_len;  // recv: filled by kernel; send: often 0
    __u32 _pad;
};

struct addr_ptrlen_t {
    __u64 addr;   // user sockaddr*
    __u32 len;    // sockaddr len
    __u32 _pad;
};

struct addr_recv_meta_t {
    __u64 addr;   // user sockaddr*
    __u64 lenp;   // user socklen_t*
    __u32 flags;
    __u32 _pad;
};

struct msg_ptrflags_t {
    __u64 msg;    // user msghdr*
    __u32 flags;
    __u32 _pad;
};

struct mmsg_ptrvlen_t {
    __u64 vec;    // user mmsghdr*
    __u32 vlen;
    __u32 flags;
};

/* ====== maps ====== */

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, struct trace_info);
} scratch_info SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, struct tls_chunk_t);
} scratch_tls SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} tls_events SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, struct fd_state_t);
} fd_state_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct inflight_fd_t);
} connect_fd_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_ptrlen_t);
} addrConnect_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_ptrlen_t);
} addrBind_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_ptrlen_t);
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_recv_meta_t);
} addrRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, __u64); // user msghdr*
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct msg_ptrflags_t);
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct mmsg_ptrvlen_t);
} mmsgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct mmsg_ptrvlen_t);
} mmsgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ====== small helpers ====== */

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
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file)
        return 0;

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

    /* don't allow family to "jump" once fixed (except v4-mapped on v6 socket) */
    if (info->family != 0 && family != info->family) {
        if (info->family == AF_INET6 && family == AF_INET) {
            if (addrlen < sizeof(struct sockaddr_in))
                return -1;

            struct sockaddr_in sa = {};
            if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
                return -1;

            __u16 port = bpf_ntohs(sa.sin_port);

            __u8 v6[16] = {};
            v6[10] = 0xff;
            v6[11] = 0xff;
            __builtin_memcpy(&v6[12], &sa.sin_addr.s_addr, 4);

            if (fill_dst) {
                __builtin_memcpy(info->dst_ip6, v6, 16);
                if (port) info->dport = port;
            } else {
                __builtin_memcpy(info->src_ip6, v6, 16);
                if (port) info->sport = port;
            }
            return 0;
        }
        return -1;
    }

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
                info->src_ip4 = bpf_htonl(0x7f000001); // 127.0.0.1
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

/* cache + self-heal */
static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send_dir)
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

/* ---- parse pktinfo cmsg (NO LOOPS) ---- */

static __always_inline void parse_pktinfo_cmsg(struct trace_info *info, __u64 ctrl, __u64 controllen, int set_dst)
{
    if (!ctrl || controllen < sizeof(struct user_cmsghdr64))
        return;

    __u64 p = ctrl;
    __u64 left = controllen;

#define CMSG_TRY_STEP() do { \
    if (left < sizeof(struct user_cmsghdr64)) return; \
    struct user_cmsghdr64 ch = {}; \
    if (bpf_probe_read_user(&ch, sizeof(ch), (void *)p) != 0) return; \
    if (ch.cmsg_len < sizeof(struct user_cmsghdr64) || ch.cmsg_len > left) return; \
    __u64 data = p + sizeof(struct user_cmsghdr64); \
    __u64 datalen = ch.cmsg_len - sizeof(struct user_cmsghdr64); \
    if (info->family == AF_INET6 && ch.cmsg_level == SOL_IPV6 && ch.cmsg_type == IPV6_PKTINFO && datalen >= sizeof(struct user_in6_pktinfo)) { \
        struct user_in6_pktinfo pi6 = {}; \
        if (bpf_probe_read_user(&pi6, sizeof(pi6), (void *)data) == 0) { \
            if (set_dst) { \
                __builtin_memcpy(info->dst_ip6, &pi6.ipi6_addr, 16); \
                if (pi6.ipi6_ifindex) info->dst_scope = pi6.ipi6_ifindex; \
            } else { \
                __builtin_memcpy(info->src_ip6, &pi6.ipi6_addr, 16); \
                if (pi6.ipi6_ifindex) info->src_scope = pi6.ipi6_ifindex; \
            } \
        } \
        return; \
    } \
    if (info->family == AF_INET && ch.cmsg_level == SOL_IP && ch.cmsg_type == IP_PKTINFO && datalen >= sizeof(struct user_in_pktinfo)) { \
        struct user_in_pktinfo pi4 = {}; \
        if (bpf_probe_read_user(&pi4, sizeof(pi4), (void *)data) == 0) { \
            __u32 local = pi4.ipi_spec_dst; /* ip(7): local address of packet */ \
            if (set_dst) info->dst_ip4 = local; else info->src_ip4 = local; \
        } \
        return; \
    } \
    __u64 step = CMSG_ALIGN(ch.cmsg_len); \
    if (step == 0 || step > left) return; \
    p += step; \
    left -= step; \
} while (0)

    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();

#undef CMSG_TRY_STEP
}

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

/* ====== bind ====== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
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

    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct trace_info info = {};
    info.event = EV_BINDOK;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);

    info.proto  = st.proto;
    info.family = st.family;

    // bind = local -> put in src
    info.sport = st.lport;
    if (st.family == AF_INET) {
        info.src_ip4 = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, &st.lip6, 16);
    }

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrBind_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 0);

    loopback_fallback(&info, 1);
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
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
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

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
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
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    __u32 zero = 0;
    struct trace_info *info = bpf_map_lookup_elem(&scratch_info, &zero);
    if (!info)
        goto cleanup;

    __builtin_memset(info, 0, sizeof(*info));
    info->event = EV_SENDMSG;
    info->fd    = ci->fd;
    info->ret   = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1 /* send */) < 0)
        goto cleanup;

    // msg pointer
    __u64 msg_u = 0;
    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp)
        msg_u = *msgp;

    // parse msghdr: dst from msg_name, src from pktinfo cmsg
    struct user_msghdr64 mh = {};
    int mh_ok = 0;
    if (msg_u && read_msghdr64(msg_u, &mh) == 0)
        mh_ok = 1;

    if (mh_ok) {
        if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(info, (void *)mh.msg_name, mh.msg_namelen, 1 /* dst */);

        if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(info, mh.msg_control, mh.msg_controllen, 0 /* set SRC */);
    }

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

    // ===== TLS chunk (для SNI в Go) =====
    int want_tls = (info->proto == IPPROTO_TCP) &&
                   (info->dport == 443 || info->sport == 443 || info->dport == 853 || info->sport == 853);

    if (want_tls && mh_ok && mh.msg_iov && mh.msg_iovlen) {
        struct user_iovec64 iv0 = {};
        if (bpf_probe_read_user(&iv0, sizeof(iv0), (void *)mh.msg_iov) == 0) {
            __u64 base = iv0.iov_base;
            __u64 blen = iv0.iov_len;

            if (base && blen >= 5) {
                // быстрый TLS record header check: 0x16 0x03 0x01..0x04
                __u8 typ=0, v1=0, v2=0;
                if (bpf_probe_read_user(&typ, 1, (void *)base) == 0 &&
                    bpf_probe_read_user(&v1,  1, (void *)(base + 1)) == 0 &&
                    bpf_probe_read_user(&v2,  1, (void *)(base + 2)) == 0) {

                    if (typ == 0x16 && v1 == 0x03 && (v2 >= 0x01 && v2 <= 0x04)) {
                        struct tls_chunk_t *ch = bpf_map_lookup_elem(&scratch_tls, &zero);
                        if (ch) {
                            __builtin_memset(ch, 0, sizeof(*ch));
                            ch->ts_ns  = bpf_ktime_get_ns();
                            ch->cookie = info->cookie;
                            ch->tgid   = info->tgid;
                            ch->tid    = info->tid;
                            ch->fd     = info->fd;
                            ch->family = info->family;
                            ch->proto  = info->proto;
                            ch->sport  = info->sport;
                            ch->dport  = info->dport;
                            ch->event  = EV_TLS_CHUNK;

                            // verifier-friendly: только константные размеры
                            if (blen >= TLS_SNAP) {
                                if (bpf_probe_read_user(ch->data, TLS_SNAP, (void *)base) == 0)
                                    ch->len = TLS_SNAP;
                            } else if (blen >= 128) {
                                if (bpf_probe_read_user(ch->data, 128, (void *)base) == 0)
                                    ch->len = 128;
                            } else if (blen >= 64) {
                                if (bpf_probe_read_user(ch->data, 64, (void *)base) == 0)
                                    ch->len = 64;
                            } else if (blen >= 32) {
                                if (bpf_probe_read_user(ch->data, 32, (void *)base) == 0)
                                    ch->len = 32;
                            } else if (blen >= 16) {
                                if (bpf_probe_read_user(ch->data, 16, (void *)base) == 0)
                                    ch->len = 16;
                            } else {
                                if (bpf_probe_read_user(ch->data, 8, (void *)base) == 0)
                                    ch->len = 8;
                            }

                            if (ch->len)
                                bpf_perf_event_output(ctx, &tls_events, BPF_F_CURRENT_CPU, ch, sizeof(*ch));
                        }
                    }
                }
            }
        }
    }

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

/* ===== write meta ===== */

struct write_meta_t {
    __u64 buf;   // user void*
    __u64 cnt;   // size_t
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64); // pid_tgid
    __type(value, struct write_meta_t);
} write_meta_map SEC(".maps");

/* ===== TLS pending per cookie (to reassemble one TLS record) ===== */

struct tls_need_t {
    __u32 left;   // bytes remaining to emit from this TLS record
    __u32 total;  // total bytes for this TLS record (record_len + 5)
};

struct {
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 65536);
    __type(key, __u64);             // cookie
    __type(value, struct tls_need_t);
} tls_need_map SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_write")
int trace_write_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    int fd = (int)ctx->args[0];
    if (!is_socket_fd(fd))
        return 0;

    // conn_info (как раньше)
    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    // save buf + count for TLS chunk on exit
    struct write_meta_t wm = {};
    wm.buf = (__u64)ctx->args[1];
    wm.cnt = (__u64)ctx->args[2];
    if (wm.buf && wm.cnt)
        bpf_map_update_elem(&write_meta_map, &id, &wm, BPF_ANY);

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

    __u32 zero = 0;
    struct trace_info *info = bpf_map_lookup_elem(&scratch_info, &zero);
    if (!info)
        goto cleanup;

    __builtin_memset(info, 0, sizeof(*info));
    info->event = EV_WRITE;
    info->fd    = ci->fd;
    info->ret   = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1 /* send */) < 0)
        goto cleanup;

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

    /* ===== TLS chunk from write(buf): pending logic ===== */
    if (info->proto == IPPROTO_TCP && info->cookie) {
        struct write_meta_t *wm = bpf_map_lookup_elem(&write_meta_map, &id);
        if (wm && wm->buf) {
            __u64 base = wm->buf;

            // how many bytes were actually written
            __u64 blen = (__u64)ret;
            if (wm->cnt && blen > wm->cnt)
                blen = wm->cnt;

            if (blen > 0) {
                __u64 c = info->cookie;

                // do we already have a pending TLS record for this cookie?
                struct tls_need_t *need = bpf_map_lookup_elem(&tls_need_map, &c);

                // if no pending, detect start of TLS ClientHello record
                if (!need && blen >= 6) {
                    // read: record hdr(5) + handshake_type(1)
                    __u8 h[6] = {};
                    if (bpf_probe_read_user(h, sizeof(h), (void *)base) == 0) {
                        __u8 typ = h[0];
                        __u8 v1  = h[1];
                        __u8 v2  = h[2];
                        __u16 rec_len = ((__u16)h[3] << 8) | (__u16)h[4];
                        __u8 hs_type  = h[5];

                        // TLS handshake record + ClientHello only
                        if (typ == 0x16 && v1 == 0x03 && (v2 >= 0x01 && v2 <= 0x04) && hs_type == 0x01) {
                            __u32 total = (__u32)rec_len + 5;
                            // sanity cap: we only need first record for SNI
                            if (total > 2048)
                                total = 2048;

                            struct tls_need_t init = {.left = total, .total = total};
                            bpf_map_update_elem(&tls_need_map, &c, &init, BPF_ANY);
                            need = bpf_map_lookup_elem(&tls_need_map, &c);
                        }
                    }
                }

                // if pending exists: emit continuation chunks EVEN if buf doesn't start with 16 03 ...
                if (need && need->left) {
                    __u64 want = need->left;
                    if (want > blen)
                        want = blen;

                    // emit at least 1 byte if possible
                    if (want > 0) {
                        struct tls_chunk_t *ch = bpf_map_lookup_elem(&scratch_tls, &zero);
                        if (ch) {
                            __builtin_memset(ch, 0, sizeof(*ch));

                            ch->ts_ns  = bpf_ktime_get_ns();
                            ch->cookie = c;
                            ch->tgid   = info->tgid;
                            ch->tid    = info->tid;
                            ch->fd     = info->fd;

                            ch->family = info->family;
                            ch->proto  = info->proto;
                            ch->sport  = info->sport;
                            ch->dport  = info->dport;
                            ch->event  = EV_TLS_CHUNK;

                            // verifier-friendly reads: constant sizes only
                            if (want >= TLS_SNAP) {
                                if (bpf_probe_read_user(ch->data, TLS_SNAP, (void *)base) == 0) ch->len = TLS_SNAP;
                            } else if (want >= 256) {
                                if (bpf_probe_read_user(ch->data, 256, (void *)base) == 0) ch->len = 256;
                            } else if (want >= 128) {
                                if (bpf_probe_read_user(ch->data, 128, (void *)base) == 0) ch->len = 128;
                            } else if (want >= 64) {
                                if (bpf_probe_read_user(ch->data, 64, (void *)base) == 0) ch->len = 64;
                            } else if (want >= 32) {
                                if (bpf_probe_read_user(ch->data, 32, (void *)base) == 0) ch->len = 32;
                            } else if (want >= 16) {
                                if (bpf_probe_read_user(ch->data, 16, (void *)base) == 0) ch->len = 16;
                            } else if (want >= 8) {
                                if (bpf_probe_read_user(ch->data, 8, (void *)base) == 0) ch->len = 8;
                            } else if (want >= 4) {
                                if (bpf_probe_read_user(ch->data, 4, (void *)base) == 0) ch->len = 4;
                            } else if (want >= 2) {
                                if (bpf_probe_read_user(ch->data, 2, (void *)base) == 0) ch->len = 2;
                            } else {
                                if (bpf_probe_read_user(ch->data, 1, (void *)base) == 0) ch->len = 1;
                            }

                            if (ch->len) {
                                // decrease remaining bytes for this TLS record
                                if (need->left <= ch->len) {
                                    bpf_map_delete_elem(&tls_need_map, &c);
                                } else {
                                    need->left -= ch->len;
                                }

                                bpf_perf_event_output(ctx, &tls_events, BPF_F_CURRENT_CPU, ch, sizeof(*ch));
                            }
                        }
                    }
                }
            }
        }
    }

cleanup:
    bpf_map_delete_elem(&write_meta_map, &id);
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

/* ====== close (ONLY SOCKETS) ====== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    // always drop cached fd state
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);

    if (!is_socket_fd(fd))
        return 0;

    struct trace_info info = {};
    info.event = EV_CLOSE;
    info.fd    = (__u32)fd;
    info.ret   = 0;

    fill_ids_comm_cookie(&info, id, fd, 0);

    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) == 0) {
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
        }

        loopback_fallback(&info, 1);
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    return 0;
}


static __always_inline __u64 inode_cookie_from_sock(struct sock *sk)
{
    if (!sk) return 0;

    struct socket *sock = BPF_CORE_READ(sk, sk_socket);
    if (!sock) return 0;

    struct file *file = BPF_CORE_READ(sock, file);
    if (!file) return 0;

    struct inode *ino = BPF_CORE_READ(file, f_inode);
    if (!ino) return 0;

    return (__u64)BPF_CORE_READ(ino, i_ino);
}



SEC("tracepoint/net/net_dev_queue")
int trace_net_dev_queue(struct tp_net_dev_queue *ctx)
{
    struct sk_buff *skb = (struct sk_buff *)(unsigned long)ctx->skbaddr;
    if (!skb) return 0;


    struct sock *sk = BPF_CORE_READ(skb, sk);
    if (!sk) return 0;

__u64 cookie = inode_cookie_from_sock(sk);
if (!cookie) return 0;


     
    // family from sock
    __u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);

    struct trace_info e = {};
    e.event  = EV_SKB_OUT;
    e.cookie = cookie;
    e.family = family;

    void *head = (void *)BPF_CORE_READ(skb, head);
    __u16 nh   = BPF_CORE_READ(skb, network_header);
    __u16 th   = BPF_CORE_READ(skb, transport_header);

    if (family == AF_INET) {
        struct iphdr iph = {};
        if (!head) return 0;
        bpf_probe_read_kernel(&iph, sizeof(iph), head + nh);
        if (iph.version != 4) return 0;

        e.proto   = iph.protocol;
        e.src_ip4 = iph.saddr; // network order
        e.dst_ip4 = iph.daddr; // network order

        if (e.proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e.sport = bpf_ntohs(uh.source);
            e.dport = bpf_ntohs(uh.dest);
        } else if (e.proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e.sport = bpf_ntohs(thh.source);
            e.dport = bpf_ntohs(thh.dest);
        }
    } else if (family == AF_INET6) {
        struct ipv6hdr ip6h = {};
        if (!head) return 0;
        bpf_probe_read_kernel(&ip6h, sizeof(ip6h), head + nh);

        e.proto = ip6h.nexthdr;
        __builtin_memcpy(e.src_ip6, &ip6h.saddr, sizeof(e.src_ip6));
        __builtin_memcpy(e.dst_ip6, &ip6h.daddr, sizeof(e.dst_ip6));


        if (e.proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e.sport = bpf_ntohs(uh.source);
            e.dport = bpf_ntohs(uh.dest);
        } else if (e.proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e.sport = bpf_ntohs(thh.source);
            e.dport = bpf_ntohs(thh.dest);
        }
    } else {
        return 0;
    }

    // we only care TCP/UDP/ICMP(v6) like in user space
    // (ICMP has no ports; you can still emit src/dst if you want)
    if (e.proto != IPPROTO_TCP && e.proto != IPPROTO_UDP && e.proto != IPPROTO_ICMP && e.proto != IPPROTO_ICMPV6) {
        return 0;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &e, sizeof(e));
    return 0;
}