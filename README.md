struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key,   u32);
    __type(value, struct msghdr *);
} msgSend_map SEC(".maps");

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#define AF_INET   2
#define AF_INET6 10

struct conn_info_t {
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

struct trace_info {
    struct in_addr  srcIP;
    struct in_addr  dstIP;
    __u32           srcIP6[4];
    __u32           dstIP6[4];
    __u32           pid;
    __u32           proto;
    __u16           sysexit;
    __u16           sport;
    __u16           dport;
    __u16           state;
    __u16           family;
    char            comm[32];
};

/* карта для conn_info */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key,   u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

/* карта для указателя на msghdr при sendmsg */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key,   u32);
    __type(value, struct msghdr *);
} msgSend_map SEC(".maps");

/* perf-массив для передачи данных в userspace */
struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
} trace_events SEC(".maps");


SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    /* сохраняем базовую информацию о процессе */
    struct conn_info_t ci = {};
    ci.pid = pid;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &pid, &ci, BPF_ANY);

    /* сохраняем указатель на msghdr */
    struct msghdr *msgp = (struct msghdr *)ctx->args[1];
    if (msgp)
        bpf_map_update_elem(&msgSend_map, &pid, &msgp, BPF_ANY);

    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    /* lookup conn_info и удаляем при ошибке */
    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!ci || ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    /* lookup pointer на msghdr */
    struct msghdr **msgptr = bpf_map_lookup_elem(&msgSend_map, &pid);
    if (!msgptr) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    /* читаем всю структуру msghdr из пространства пользователя */
    struct msghdr msg = {};
    if (bpf_core_read_user(&msg, sizeof(msg), *msgptr) < 0)
        goto cleanup;

    /* сначала читаем общий sockaddr только для sa_family */
    struct sockaddr sa = {};
    if (bpf_core_read_user(&sa, sizeof(sa), &msg.msg_name) < 0)
        goto cleanup;

    /* готовим trace_info */
    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = 11;
    info.pid     = ci->pid;
    info.proto   = ci->proto;

    if (sa.sa_family == AF_INET) {
        /* читаем полный sockaddr_in */
        struct sockaddr_in sin = {};
        if (bpf_core_read_user(&sin, sizeof(sin), &msg.msg_name) < 0)
            goto cleanup;

        u16 port = bpf_ntohs(sin.sin_port);
        if (port == 0) goto cleanup;

        info.family       = AF_INET;
        info.dport        = port;
        info.dstIP.s_addr = bpf_ntohl(sin.sin_addr.s_addr);

    } else if (sa.sa_family == AF_INET6) {
        /* читаем полный sockaddr_in6 */
        struct sockaddr_in6 sin6 = {};
        if (bpf_core_read_user(&sin6, sizeof(sin6), &msg.msg_name) < 0)
            goto cleanup;

        u16 port6 = bpf_ntohs(sin6.sin6_port);
        if (port6 == 0) goto cleanup;

        info.family = AF_INET6;
        info.dport  = port6;
        __builtin_memcpy(&info.dstIP6,
                         sin6.sin6_addr.in6_u.u6_addr32,
                         sizeof(info.dstIP6));
    } else {
        /* неизвестное семейство */
        goto cleanup;
    }

    /* отправляем событие в userspace */
    bpf_perf_event_output(ctx, &trace_events,
                          BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgSend_map,  &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

