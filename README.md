clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
    {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in *addr = bpf_map_lookup_elem(&addr_map, &pid);
    if (addr && addr->sin_family == AF_INET)
    {
        __be32 ip_addr = addr->sin_addr.s_addr;  // без использования BPF_CORE_READ
        __be16 port = addr->sin_port;            // без использования BPF_CORE_READ

        conn_info->src_ip = bpf_ntohl(ip_addr);
        conn_info->sport = bpf_ntohs(port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

        bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);
    }

    // TODO вот тут должна быть отправка через perf_event_output conn_info в юзерспейс
    return 0;
}
