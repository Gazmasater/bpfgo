clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


package main

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -tags linux trace trace.c

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Копируем адрес сокета в структуру
    if (ctx->addr) {
        bpf_probe_read(&conn_info.sock_addr, sizeof(conn_info.sock_addr), ctx->addr);
    }

    // Проверяем, что семейство адресов - IPv4
    if (conn_info.sock_addr.sa_family == AF_INET) {
        struct sockaddr_in *addr_in = &conn_info.sock_addr.in;
        conn_info.src_ip = bpf_ntohl(addr_in->sin_addr.s_addr);
        conn_info.sport = bpf_ntohs(addr_in->sin_port);

        // Выводим информацию
        bpf_printk("UDP sys_enter_sendto: PID=%d, Comm=%s, Src_IP=%d.%d.%d.%d, Src_Port=%d\n",
            conn_info.pid, conn_info.comm,
            (conn_info.src_ip >> 24) & 0xFF, (conn_info.src_ip >> 16) & 0xFF,
            (conn_info.src_ip >> 8) & 0xFF, conn_info.src_ip & 0xFF, conn_info.sport);
    } else {
        bpf_printk("UDP sys_enter_sendto: PID=%d, Comm=%s, Unknown address family\n",
            conn_info.pid, conn_info.comm);
    }

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}
