
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git branch -m old-branch new-branch
git push origin -u new-branch


git checkout main
git pull origin main


git checkout ProcNet_monitor
git merge main

git push --force-with-lease origin ProcNet_monitor





/* ===================== sendmsg ===================== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));

    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct msghdr *msg = (struct msghdr *)ctx->args[1];
    if (!msg)
        return 0;

    bpf_map_update_elem(&addrSend_map, &id, &msg, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret;
    if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
        return 0;
    if (ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct msghdr **msgpp = bpf_map_lookup_elem(&addrSend_map, &id);
    if (!msgpp)
        goto cleanup;

    struct msghdr *msg = NULL;
    if (bpf_core_read_user(&msg, sizeof(msg), msgpp) < 0 || !msg)
        goto cleanup;

    /* msg_name is a USER POINTER to sockaddr (or NULL) */
    void *name = NULL;
    if (bpf_core_read_user(&name, sizeof(name), &msg->msg_name) < 0 || !name)
        goto cleanup;

    __u16 family = 0;
    if (bpf_core_read_user(&family, sizeof(family), name) < 0)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = 11;
    info.pid = tgid;
    info.family = family;

    if (family == AF_INET) {
        struct sockaddr_in sa = {};
        if (bpf_core_read_user(&sa, sizeof(sa), name) < 0)
            goto cleanup;

        __u16 port = bpf_ntohs(sa.sin_port);
        if (port == 0)
            goto cleanup;

        info.dport = port;
        info.dstIP.s_addr = sa.sin_addr.s_addr; /* keep as network order or convert consistently in Go */

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (family == AF_INET6) {
        struct sockaddr_in6 sa6 = {};
        if (bpf_core_read_user(&sa6, sizeof(sa6), name) < 0)
            goto cleanup;

        __u16 port6 = bpf_ntohs(sa6.sin6_port);
        if (port6 == 0)
            goto cleanup;

        info.family = AF_INET6;
        info.dport  = port6;

        /* read full IPv6 address (16 bytes) */
        if (BPF_CORE_READ_INTO(&info.dstIP6, &sa6, sin6_addr.in6_u.u6_addr32) < 0)
            goto cleanup;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}


/* ===================== recvmsg ===================== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));

    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct msghdr *msg = (struct msghdr *)ctx->args[1];
    if (!msg)
        return 0;

    bpf_map_update_elem(&addrRecv_map, &id, &msg, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret;
    if (BPF_CORE_READ_INTO(&ret, ctx, ret) < 0)
        return 0;
    if (ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct msghdr **msgpp = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (!msgpp)
        goto cleanup;

    struct msghdr *msg = NULL;
    if (bpf_core_read_user(&msg, sizeof(msg), msgpp) < 0 || !msg)
        goto cleanup;

    /* msg_name is a USER POINTER to sockaddr (or NULL) */
    void *name = NULL;
    if (bpf_core_read_user(&name, sizeof(name), &msg->msg_name) < 0 || !name)
        goto cleanup;

    __u16 family = 0;
    if (bpf_core_read_user(&family, sizeof(family), name) < 0)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = 12;
    info.pid = tgid;
    info.family = family;

    if (family == AF_INET) {
        struct sockaddr_in sa = {};
        if (bpf_core_read_user(&sa, sizeof(sa), name) < 0)
            goto cleanup;

        __u16 port = bpf_ntohs(sa.sin_port);
        if (port == 0)
            goto cleanup;

        info.sport = port;
        info.srcIP.s_addr = sa.sin_addr.s_addr; /* keep consistent with your Go conversion */

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (family == AF_INET6) {
        struct sockaddr_in6 sa6 = {};
        if (bpf_core_read_user(&sa6, sizeof(sa6), name) < 0)
            goto cleanup;

        __u16 port6 = bpf_ntohs(sa6.sin6_port);
        if (port6 == 0)
            goto cleanup;

        info.family = AF_INET6;
        info.sport  = port6;

        if (BPF_CORE_READ_INTO(&info.srcIP6, &sa6, sin6_addr.in6_u.u6_addr32) < 0)
            goto cleanup;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}








