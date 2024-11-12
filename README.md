struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, struct conn_key_t);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");


SEC("kretprobe/__sys_accept4")
int trace_accept4_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    if (ret < 0) {
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid);
    if (!conn_info) {
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) == 0) {
        if (addr.sin_family == AF_INET) {
            conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info->sport = bpf_ntohs(addr.sin_port);

            struct conn_key_t key = {
                .src_ip = (conn_info->src_ip == 0) ? (127 << 24 | 1) : conn_info->src_ip, // Замена 0.0.0.0 на 127.0.0.1 для соответствия
                .dst_ip = conn_info->dst_ip,
                .src_port = conn_info->sport,
                .dst_port = conn_info->dport,
            };

            struct conn_info_t *connect_info = bpf_map_lookup_elem(&conn_info_map, &key);
            if (connect_info) {
                bpf_printk("Matched connection: Client %d.%d.%d.%d:%d to Server %d.%d.%d.%d:%d\n",
                           (key.src_ip >> 24) & 0xFF, (key.src_ip >> 16) & 0xFF,
                           (key.src_ip >> 8) & 0xFF, key.src_ip & 0xFF, key.src_port,
                           (key.dst_ip >> 24) & 0xFF, (key.dst_ip >> 16) & 0xFF,
                           (key.dst_ip >> 8) & 0xFF, key.dst_ip & 0xFF, key.dst_port);

                bpf_map_delete_elem(&conn_info_map, &key);
            }
        }
    }

    return 0;
}

SEC("kprobe/__sys_connect")
int trace_connect_entry(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    struct conn_info_t conn_info = {};
    init_conn_info(&conn_info, pid, ctx);

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info.sock_addr) == 0) {
        if (addr.sin_family == AF_INET) {
            conn_info.dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info.dport = bpf_ntohs(addr.sin_port);

            struct conn_key_t key = {
                .src_ip = conn_info.src_ip,
                .dst_ip = conn_info.dst_ip,
                .src_port = conn_info.sport,
                .dst_port = conn_info.dport,
            };

            bpf_map_update_elem(&conn_info_map, &key, &conn_info, BPF_ANY);
        }
    }

    return 0;
}
