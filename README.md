static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;
    bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
    return 0;
}

static __always_inline int init_conn_info_accept(struct sys_enter_accept4_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_accept, bpf_get_current_pid_tgid() >> 32);
}

static __always_inline int init_conn_info_bind(struct sys_enter_bind_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->umyaddr, &conn_info_map_bind, bpf_get_current_pid_tgid() >> 32);
}

static __always_inline int init_conn_info_connect(struct sys_enter_connect_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map_connect, bpf_get_current_pid_tgid() >> 32);
}

