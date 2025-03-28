lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


SEC("tracepoint/syscalls/sys_enter_getsockname")
int trace_enter_getsockname(struct sys_enter_getsockname_args *ctx) {
    struct conn_info_t conn_info = {};

    u32 pid = bpf_get_current_pid_tgid() >> 32;
    conn_info.pid = pid;

    // Получаем имя процесса
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.fd = ctx->fd;

    // Получаем указатель на sockaddr
    struct sockaddr *addr = (struct sockaddr *)ctx->usockaddr;
    bpf_printk("sys_enter_getsockname PID=%d NAME=%s addr=%p", conn_info.pid, conn_info.comm, addr);

    // Обновляем карту addrSockName_map с addr
    bpf_map_update_elem(&addrSockName_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_getsockname")
int trace_exit_getsockname(struct sys_exit_getsockname_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    if (ret < 0) {
        bpf_printk("sys_exit_getsockname failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addrSockName_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    bpf_printk("11111sys_exit_getsockname ADDRptr=%p ",*addr_ptr);


    struct sockaddr addr = {};
    bpf_probe_read_kernel(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("22222sys_exit_getsockname ADDR=%p FAMILY=%d ",addr,addr.sa_family);


    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        if (port==0) {

            return 0;
        }
        


        struct trace_info info = {};
        info.pid = pid;
        info.src_ip=ip;
        info.sport = port;
        info.sysexit=5;
      



         bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));


        bpf_map_update_elem(&bind_map, &pid, &addr_in, BPF_ANY);

        bpf_printk("sys_exit_getsockname FAMILY=%d PID=%d Comm=%s IP=%d.%d.%d.%d PORT=%d\n",
            addr.sa_family,
            conn_info.pid,
            conn_info.comm,
            (ip >> 24) & 0xff,
            (ip >> 16) & 0xff,
            (ip >> 8) & 0xff,
            (ip) & 0xff,
            port);  
    } else if (addr.sa_family == AF_INET6) {
        struct sockaddr_in6 addr_in6 = {};
        bpf_probe_read_user(&addr_in6, sizeof(addr_in6), *addr_ptr);

        u16 port = bpf_ntohs(addr_in6.sin6_port);

        bpf_map_update_elem(&bind6_map, &pid, &addr_in6, BPF_ANY);

        bpf_printk("sys_exit_getsockname IP6 FAMILY=%d PID=%d Comm=%s PORT=%d\n",
            addr.sa_family,
            conn_info.pid,
            conn_info.comm,
            port);  
    }

  
    return 0;
}


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_syn(struct trace_event_raw_inet_sock_set_state *ctx) {
    struct conn_info_t conn_info = {};

    // Проверяем, что это состояние TCP_SYN_SENT (начало соединения)
    if (ctx->newstate == TCP_SYN_SENT) {
    conn_info.src_ip = bpf_ntohl(ctx->saddr);
    conn_info.dst_ip = bpf_ntohl(ctx->daddr);
    conn_info.sport = bpf_ntohs(ctx->sport);
    conn_info.dport = bpf_ntohs(ctx->dport);

    }

    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir "$(pwd)" -tags linux -type trace_info -go-package=main -target amd64 bpf "$(pwd)/trace.c" -- -I"$(pwd)"
/home/gaz358/myprog/bpfgo/trace.c:965:24: warning: cast to smaller integer type '__u32' (aka 'unsigned int') from '__u8 *' (aka 'unsigned char *') [-Wpointer-to-int-cast]
  965 |     conn_info.src_ip = bpf_ntohl(ctx->saddr);
      |                        ^~~~~~~~~~~~~~~~~~~~~
/usr/include/bpf/bpf_endian.h:91:3: note: expanded from macro 'bpf_ntohl'
   91 |          __bpf_constant_ntohl(x) : __bpf_ntohl(x))
      |          ^~~~~~~~~~~~~~~~~~~~~~~

__u32 ip;
bpf_probe_read_kernel(&ip, sizeof(ip), ctx->saddr);
conn_info.src_ip = bpf_ntohl(ip);

