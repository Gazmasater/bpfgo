struct sockaddr_in addr;
void *addr_ptr = (void *)ctx->args[4];

bpf_probe_read(&addr, sizeof(addr), addr_ptr);
if (addr.sin_family == AF_INET) {
    bpf_map_update_elem(&fd_map, &pid, &addr, BPF_ANY);
}



struct sockaddr_in *addr = bpf_map_lookup_elem(&fd_map, &pid);
if (addr && addr->sin_family == AF_INET) {
    conn_info->src_ip = bpf_ntohl(addr->sin_addr.s_addr);
    conn_info->sport = bpf_ntohs(addr->sin_port);
}

