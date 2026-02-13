
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








gcc -O2 -Wall -o recvmsg_test recvmsg_test.c
gcc -O2 -Wall -o send_udp send_udp.c


./recvmsg_test 9999
./send_udp 127.0.0.1 9999 "ping1"



1) Замени определения addrSend_map и addrRecv_map
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

2) Добавь helper для парсинга sockaddr (один раз, где удобно)
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

3) Перепиши sendmsg enter/exit (вставь вместо твоих)
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

4) Перепиши recvmsg enter/exit (вставь вместо твоих)
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




// recvmsg_test.c
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>   // atoi

static pid_t gettid_linux(void) {
    return (pid_t)syscall(SYS_gettid);
}

int main(int argc, char **argv) {
    int port = 9999;
    if (argc > 1) port = atoi(argv[1]);

    pid_t pid = getpid();
    pid_t tid = gettid_linux();

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    printf("[pid=%d tid=%d] recvmsg UDP server listening on 0.0.0.0:%d\n",
           (int)pid, (int)tid, port);

    for (;;) {
        char buf[2048];

        struct sockaddr_storage peer;
        socklen_t peerlen = sizeof(peer);

        struct iovec iov = {
            .iov_base = buf,
            .iov_len  = sizeof(buf),
        };

        struct msghdr msg;
        memset(&msg, 0, sizeof(msg));
        msg.msg_name = &peer;
        msg.msg_namelen = peerlen;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        ssize_t n = recvmsg(fd, &msg, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("recvmsg");
            break;
        }

        // распечатаем src ip:port
        char ipstr[INET6_ADDRSTRLEN] = {0};
        int pport = 0;

        if (peer.ss_family == AF_INET) {
            struct sockaddr_in *sin = (struct sockaddr_in*)&peer;
            inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr));
            pport = ntohs(sin->sin_port);
        } else if (peer.ss_family == AF_INET6) {
            struct sockaddr_in6 *sin6 = (struct sockaddr_in6*)&peer;
            inet_ntop(AF_INET6, &sin6->sin6_addr, ipstr, sizeof(ipstr));
            pport = ntohs(sin6->sin6_port);
        } else {
            strcpy(ipstr, "unknown_family");
        }

        printf("[pid=%d tid=%d] got %zd bytes from %s:%d: %.*s\n",
               (int)pid, (int)tid, n, ipstr, pport, (int)n, buf);
        fflush(stdout);
    }

    close(fd);
    return 0;
}




SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct trace_info info = {};
    info.sysexit = 3;
    info.family  = ctx->family;

    if (ctx->protocol != IPPROTO_TCP && ctx->protocol != IPPROTO_UDP)
        return SK_PASS;

    info.proto = ctx->protocol;

    if (ctx->family == AF_INET) {
        // src = remote (клиент), dst = local (сервер)
        info.srcIP.s_addr = ctx->remote_ip4;
        info.sport        = bpf_ntohs(ctx->remote_port); // remote_port обычно в network order
        info.dstIP.s_addr = ctx->local_ip4;
        info.dport        = ctx->local_port;             // local_port обычно host order

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    } else if (ctx->family == AF_INET6) {
        info.srcIP6.in6_u.u6_addr32[0] = ctx->remote_ip6[0];
        info.srcIP6.in6_u.u6_addr32[1] = ctx->remote_ip6[1];
        info.srcIP6.in6_u.u6_addr32[2] = ctx->remote_ip6[2];
        info.srcIP6.in6_u.u6_addr32[3] = ctx->remote_ip6[3];

        info.dstIP6.in6_u.u6_addr32[0] = ctx->local_ip6[0];
        info.dstIP6.in6_u.u6_addr32[1] = ctx->local_ip6[1];
        info.dstIP6.in6_u.u6_addr32[2] = ctx->local_ip6[2];
        info.dstIP6.in6_u.u6_addr32[3] = ctx->local_ip6[3];

        info.sport = bpf_ntohs(ctx->remote_port);
        info.dport = ctx->local_port;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    return SK_PASS;
}












