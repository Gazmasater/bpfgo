//go:build ignore
#include "vmlinux.h"


#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_helpers.h>

struct conn_info_t
{
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u32 fd;
    u32 sport;
    u16 dport;
    u16 family;
    u8  proto;

    char comm[64];
};

struct fd_key_t {
    __u32 tgid;
    __s32 fd;
};

struct fd_state_t {
    __u16 family;
    __u8  proto;

    __u16 lport;
    __u16 rport;

    __u32 lip;
    __u32 rip;

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, struct fd_state_t);
} fd_state_map SEC(".maps");

struct inflight_fd_t {
    __s32 fd;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);              // pid_tgid
    __type(value, struct inflight_fd_t);
} connect_fd_map SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);
    __type(value, __u64); // user pointer to sockaddr
} addrBind_map SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);
    __type(value, __u64); // user pointer (sockaddr* or msghdr*)
} addrSend_map SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);
    __type(value, __u64); // user pointer (sockaddr* or msghdr*)
} addrRecv_map SEC(".maps");



struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); 
} trace_events SEC(".maps");



struct trace_info {

    // IPv4
    struct in_addr srcIP;
    struct in_addr dstIP;

     // IPv6
    struct in6_addr srcIP6;
    struct in6_addr dstIP6;

    
    __u32 pid;
    __u32 proto;
    __u16 sport;
    __u16 dport;
    __u16 family;
    __u8 sysexit;
    __u8 state;
   __s8 comm[32];
} ;




const struct trace_info *unused __attribute__((unused));
#define AF_INET 2
#define AF_INET6 10
#define TCP_ESTABLISHED 1
#define IPPROTO_UDP   17     
#define IPPROTO_TCP    6
#define  BLOCK_PORT    53  
#define ETH_P_IP    0x0800    
#define TC_ACT_OK 0

char LICENSE[] SEC("license") = "Dual BSD/GPL";


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
            info->dstIP.s_addr = sa.sin_addr.s_addr; // ВАЖНО: без ntohl
        } else {
            info->sport = port;
            info->srcIP.s_addr = sa.sin_addr.s_addr; // ВАЖНО: без ntohl
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
        // ip оставляем как есть (network order) — ты уже где-то то так, то так делаешь.
        // Если хочешь в host order — скажи, приведём единообразно.
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


SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = tgid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    bpf_map_update_elem(&conn_info_map, &id, &conn_info, BPF_ANY);

    struct inflight_fd_t in = {};
    in.fd = (int)ctx->args[0];
    bpf_map_update_elem(&connect_fd_map, &id, &in, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = ctx->ret;
    if (ret < 0)
        goto cleanup;

    struct inflight_fd_t *in = bpf_map_lookup_elem(&connect_fd_map, &id);
    if (!in)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state(in->fd, &st) < 0)
        goto cleanup;

    // сохраняем fd -> endpoints
    struct fd_key_t k = { .tgid = tgid, .fd = in->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    // событие (src=local, dst=remote)
    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.sysexit = 3;
    info.pid     = tgid;
    info.proto   = st.proto;
    info.family  = st.family;
    info.sport   = st.lport;
    info.dport   = st.rport;

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm((char *)info.comm, sizeof(info.comm));

    if (st.family == AF_INET) {
        info.srcIP.s_addr = st.lip;
        info.dstIP.s_addr = st.rip;
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info.srcIP6, &st.lip6, sizeof(info.srcIP6));
        __builtin_memcpy(&info.dstIP6, &st.rip6, sizeof(info.dstIP6));
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

cleanup:
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}



SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = tgid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    bpf_map_update_elem(&conn_info_map, &id, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 newfd = ctx->ret;
    if (newfd < 0)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)newfd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)newfd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    // событие (src=remote, dst=local) — входящее соединение
    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.sysexit = 4;
    info.pid     = tgid;
    info.proto   = st.proto;
    info.family  = st.family;

    // src=клиент
    info.sport = st.rport;
    // dst=сервер
    info.dport = st.lport;

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm((char *)info.comm, sizeof(info.comm));

    if (st.family == AF_INET) {
        info.srcIP.s_addr = st.rip;
        info.dstIP.s_addr = st.lip;
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(&info.srcIP6, &st.rip6, sizeof(info.srcIP6));
        __builtin_memcpy(&info.dstIP6, &st.lip6, sizeof(info.dstIP6));
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

// accept (не accept4) — на всякий
SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx)
{
    return trace_accept4_enter(ctx);
}

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx)
{
    return trace_accept4_exit(ctx);
}



SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));

    // args: fd, sockaddr*, addrlen
    ci.fd = (__u32)ctx->args[0];
    ci.addrlen = (__u32)ctx->args[2];

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

    __s64 ret;
    if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
        goto cleanup;
    if (ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    __u64 *uaddrp = bpf_map_lookup_elem(&addrBind_map, &id);
    if (!uaddrp)
        goto cleanup;

    void *uaddr = (void *)(*uaddrp);
    if (!uaddr)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.pid = ci->pid;
    info.sysexit = 20; // <- код события "BIND OK"

    __u16 family = 0;
    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0)
        goto cleanup;

    if (family == AF_INET) {
        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            goto cleanup;

        info.family = AF_INET;
        info.dport  = bpf_ntohs(sa.sin_port);
        info.dstIP.s_addr = sa.sin_addr.s_addr; // BE, БЕЗ ntohl

    } else if (family == AF_INET6) {
        struct sockaddr_in6 sa6 = {};
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0)
            goto cleanup;

        info.family = AF_INET6;
        info.dport  = bpf_ntohs(sa6.sin6_port);
        __builtin_memcpy(&info.dstIP6, &sa6.sin6_addr, sizeof(info.dstIP6));
    } else {
        goto cleanup;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid=id>>32;
    struct conn_info_t conn_info = {};
    

    conn_info.pid = tgid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &id, &conn_info, BPF_ANY);

    const struct sockaddr *addr = (const struct sockaddr *)ctx->args[4];
    if (!addr)
        return 0;

    bpf_map_update_elem(&addrSend_map, &id, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
     __u64 id = bpf_get_current_pid_tgid();

    __s64 ret;
if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
    return 0;
if (ret < 0)
    goto cleanup;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!conn_info)
        return 0;
    if (ret < 0) {
        goto cleanup;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSend_map, &id);
    if (!addr_ptr)
        goto cleanup;


    struct trace_info info = {};

   __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

  
    info.sysexit = 1;
    info.pid     = conn_info->pid;

    __u16 family;
    if (BPF_CORE_READ_USER_INTO(&family, *addr_ptr, sa_family) < 0)
        goto cleanup;

    if (family == AF_INET) {
        __u32 ip;
        __u16 port;
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
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx) {
   
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid=id>>32;

    struct conn_info_t conn_info = {};

    conn_info.pid = tgid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &id, &conn_info, BPF_ANY);

    if (!ctx->args[4]) return 0;


    struct sockaddr *addr = (struct sockaddr *)ctx->args[4];  

    bpf_map_update_elem(&addrRecv_map, &id, &addr, BPF_ANY);


    return 0;
}


SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id = bpf_get_current_pid_tgid();

__s64 ret;
if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
    return 0;
if (ret < 0)
    goto cleanup;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!conn_info)
        return 0;
    if (ret < 0) {
        bpf_printk("sys_exit_recvfrom failed for PID=%d\n", conn_info->pid);
        bpf_map_delete_elem(&conn_info_map, &id);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (!addr_ptr)
    goto cleanup;


    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 2;
    info.pid     =conn_info->pid;

    /* читаем семейство адреса */
    __u16 family;
    if (BPF_CORE_READ_USER_INTO(&family, *addr_ptr, sa_family) < 0)
        goto cleanup;

    if (family == AF_INET) {
        __u32 ip;
        __u16 port;
        /* кастим на sockaddr_in и читаем поля */
        if (BPF_CORE_READ_USER_INTO(&ip,
                (struct sockaddr_in *)*addr_ptr,
                sin_addr.s_addr) < 0 ||
            BPF_CORE_READ_USER_INTO(&port,
                (struct sockaddr_in *)*addr_ptr,
                sin_port) < 0)
            goto cleanup;

        /* приводим к хост-байт-ордру */
        ip   = bpf_ntohl(ip);
        port = bpf_ntohs(port);
        if (port == 0)
            goto cleanup;

        info.srcIP.s_addr = ip;
        info.sport        = port;
        info.family       = AF_INET;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (family == AF_INET6) {
        __u16 port6;
        /* кастим на sockaddr_in6 и читаем порт */
        if (BPF_CORE_READ_USER_INTO(&port6,
                (struct sockaddr_in6 *)*addr_ptr,
                sin6_port) < 0)
            goto cleanup;

        port6 = bpf_ntohs(port6);
        if (port6 == 0)
            goto cleanup;

        info.family = AF_INET6;
        info.sport  = port6;

        /* читаем весь массив u6_addr32 за один вызов */
        if (BPF_CORE_READ_USER_INTO(&info.srcIP6,
                (struct sockaddr_in6 *)*addr_ptr,
                sin6_addr.in6_u.u6_addr32) < 0)
            goto cleanup;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = tgid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &id, &conn_info, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1]; // struct msghdr * (user)
    if (msg_u)
        bpf_map_update_elem(&addrSend_map, &id, &msg_u, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();

    __s64 ret;
    if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
        goto cleanup;
    if (ret < 0)
        goto cleanup;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!conn_info)
        goto cleanup;

    __u64 *msg_u_ptr = bpf_map_lookup_elem(&addrSend_map, &id);
    if (!msg_u_ptr)
        goto cleanup;

    struct msghdr msg = {};
    if (bpf_probe_read_user(&msg, sizeof(msg), (const void *)*msg_u_ptr) < 0)
        goto cleanup;

    void *name = msg.msg_name; // user sockaddr*
    if (!name)
        goto cleanup; // connected socket -> тут адреса нет

    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 11;
    info.pid = conn_info->pid;

    if (fill_from_sockaddr(&info, name, /*is_dst=*/1) < 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}
SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = tgid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    bpf_map_update_elem(&conn_info_map, &id, &conn_info, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1]; // struct msghdr * (user)
    if (msg_u)
        bpf_map_update_elem(&addrRecv_map, &id, &msg_u, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();

    __s64 ret;
    if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
        goto cleanup;
    if (ret < 0)
        goto cleanup;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!conn_info)
        goto cleanup;

    __u64 *msg_u_ptr = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (!msg_u_ptr)
        goto cleanup;

    struct msghdr msg = {};
    if (bpf_probe_read_user(&msg, sizeof(msg), (const void *)*msg_u_ptr) < 0)
        goto cleanup;

    void *name = msg.msg_name; // user sockaddr*
    if (!name)
        goto cleanup; // часто бывает, зависит от сокета/флагов

    struct trace_info info = {};
    __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));
    info.sysexit = 12;
    info.pid = conn_info->pid;

    if (fill_from_sockaddr(&info, name, /*is_dst=*/0) < 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}























