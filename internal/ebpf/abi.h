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

#define EV_SKB_OUT   40

// NEW: TLS chunk event
#define EV_TLS_CHUNK 100

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

#define CMSG_ALIGN(len) (((len) + sizeof(__u64) - 1) & ~(sizeof(__u64) - 1))

/* ===== TLS chunk config ===== */
#define TLS_CHUNK_MAX 512
#define TLS_MAX_BYTES_PER_CONN (16 * 1024) /* userspace only */
#define TLS_FROM_SENDMSG 1


struct tp_net_dev_queue {
    __u16 common_type;
    __u8  common_flags;
    __u8  common_preempt_count;
    __s32 common_pid;

    __u64 skbaddr;
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

/* EVENT (to userspace): your existing */
struct trace_info {
    __u64 ts_ns;
    __u64 cookie;

    __u32 tgid;
    __u32 tid;

    __u32 fd;
    __s32 _pad0;

    __s64 ret;

    __u16 family;
    __u16 sport;
    __u16 dport;

    __u8  proto;
    __u8  event;
    __u8  state;
    __u8  _pad1;

    __u32 src_ip4;
    __u32 dst_ip4;
    __u8  src_ip6[16];
    __u8  dst_ip6[16];

    __u32 src_scope;
    __u32 dst_scope;

    char  comm[32];
};

const struct trace_info *unused __attribute__((unused));

/* ===== TLS chunk event (to userspace) ===== */
struct tls_chunk_event {
    __u64 cookie;
    __u64 ts_ns;

    __u32 tgid;
    __u32 tid;

    __s32 fd;
    __u32 seq;

    __u8  proto;
    __u8  event;  // = EV_TLS_CHUNK
    __u16 sport;
    __u16 dport;

    __u32 len;
    __u8  data[TLS_CHUNK_MAX];
};

/* ---- userspace ABI (amd64) ---- */

struct user_msghdr64 {
    __u64 msg_name;
    __u32 msg_namelen;
    __u32 _pad0;

    __u64 msg_iov;
    __u64 msg_iovlen;

    __u64 msg_control;
    __u64 msg_controllen;

    __u32 msg_flags;
    __u32 _pad1;
};

struct user_cmsghdr64 {
    __u64 cmsg_len;
    __s32 cmsg_level;
    __s32 cmsg_type;
};

struct user_in6_pktinfo {
    __u8  ipi6_addr[16];
    __u32 ipi6_ifindex;
};

struct user_in_pktinfo {
    __s32 ipi_ifindex;
    __u32 ipi_spec_dst;
    __u32 ipi_addr;
};

/* ---- mmsg (amd64) ---- */

struct user_iovec64 {
    __u64 iov_base;
    __u64 iov_len;
};

struct user_mmsghdr64 {
    struct user_msghdr64 msg_hdr;
    __u32 msg_len;
    __u32 _pad;
};

struct addr_ptrlen_t {
    __u64 addr;
    __u32 len;
    __u32 _pad;
};

struct addr_recv_meta_t {
    __u64 addr;
    __u64 lenp;
    __u32 flags;
    __u32 _pad;
};

struct msg_ptrflags_t {
    __u64 msg;
    __u32 flags;
    __u32 _pad;
};

struct mmsg_ptrvlen_t {
    __u64 vec;
    __u32 vlen;
    __u32 flags;
};

/* NEW: write args */
struct write_args_t {
    __u64 buf;
    __u64 count;
};

