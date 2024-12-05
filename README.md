struct conn_info_key {
    u32 pid;          // Идентификатор процесса (PID)
    u8 op_type;       // Тип операции: 0 — для sendto, 1 — для connect
};

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct conn_info_key); // Используем новую структуру ключа
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 op_type)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;

    struct conn_info_key key = {};
    key.pid = pid;
    key.op_type = op_type;

    bpf_map_update_elem(map, &key, &conn_info, BPF_ANY);
    return 0;
}

static __always_inline int init_conn_info_sendto(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map, pid, 0); // 0 для sendto
}

static __always_inline int init_conn_info_connect(struct sys_enter_connect_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map, pid, 1); // 1 для connect
}

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info_sendto(ctx);

    struct conn_info_key key = { .pid = pid, .op_type = 0 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (conn_info && conn_info->comm[0] == 'u')
    {
        bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);
    }

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

    struct conn_info_key key = { .pid = pid, .op_type = 0 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (!conn_info)
    {
        bpf_printk("UDP sys_exit_sendto No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        bpf_printk("UDP sys_exit_sendto Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("UDP sys_exit_sendto  connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    bpf_map_update_elem(&conn_info_map, &key, conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info_connect(ctx);

    struct conn_info_key key = { .pid = pid, .op_type = 1 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (conn_info && conn_info->comm[0] == 'n')
    {
        bpf_printk("enter_connect CLIENT Connect entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int ret = ctx->ret;

    if (ret < 0)
    {
        bpf_printk("exit_connect Connect failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_key key = { .pid = pid, .op_type = 1 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (!conn_info)
    {
        bpf_printk("exit_connect No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        bpf_printk("exit_connect Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->dport = bpf_ntohs(addr.sin_port);

        bpf_printk("exit_connect CLIENT Connected to server: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
                   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
    }

    bpf_map_update_elem(&conn_info_map, &key, conn_info, BPF_ANY);
    return 0;
}
