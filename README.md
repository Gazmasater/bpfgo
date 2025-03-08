gcc udp_server.c -o udp_server

SEC("cgroup/connect4")
int trace_connect_cgroup(struct bpf_sock_addr *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    struct sockaddr_in local_addr = {};
    int err = bpf_getsockname(ctx->sk, (struct sockaddr *)&local_addr, sizeof(local_addr));
    
    if (err == 0 && local_addr.sin_family == AF_INET) {
        conn_info.local_ip = bpf_ntohl(local_addr.sin_addr.s_addr);
        conn_info.local_port = bpf_ntohs(local_addr.sin_port);
        bpf_map_update_elem(&conn_map, &pid, &conn_info, BPF_ANY);
    }

    return 0;
}
