clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");


SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Прямой доступ к sockaddr_in
    void *addr_ptr = (void *)ctx->addr;
    bpf_probe_read(&conn_info.addr, sizeof(conn_info.addr), addr_ptr); // записываем всю структуру sockaddr_in

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}



SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
    {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        return 0;
    }

    if (conn_info->addr.sin_family == AF_INET)
    {
        __be32 ip_addr = conn_info->addr.sin_addr.s_addr;
        __be16 port = conn_info->addr.sin_port;

        conn_info->src_ip = bpf_ntohl(ip_addr);
        conn_info->sport = bpf_ntohs(port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

        bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);
    }

    // TODO: отправка через perf_event_output в юзерспейс
    return 0;
}

