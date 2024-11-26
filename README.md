static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;
    bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
    return 0;
}
