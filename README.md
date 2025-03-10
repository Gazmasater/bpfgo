nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_connect failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    bpf_printk("sys_exit_connect PID=%d  FAMILY=%d ", conn_info->pid, addr.sa_family);

    // Формируем tuple для поиска сокета
    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 remote_ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 remote_port = bpf_ntohs(addr_in.sin_port);

        struct bpf_sock_tuple tuple = {};
        tuple.ipv4.saddr = conn_info->local_ip;   // Локальный IP
        tuple.ipv4.daddr = remote_ip;             // Удалённый IP
        tuple.ipv4.sport = bpf_htons(conn_info->local_port);  // Локальный порт
        tuple.ipv4.dport = bpf_htons(remote_port); // Удалённый порт

        // Получаем сокет
        struct bpf_sock *sk = bpf_sk_lookup_tcp(ctx, &tuple, sizeof(tuple.ipv4), BPF_F_CURRENT_NETNS, 0);
        if (sk) {
            // Извлекаем локальный IP и порт
            u32 local_ip = sk->src_ip4;
            u16 local_port = bpf_ntohs(sk->src_port);
            bpf_printk("Local IP: %u, Local Port: %d\n", local_ip, local_port);

            bpf_sk_release(sk); // Освобождаем ресурс
        }
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}


SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct sys_enter_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int fd = ctx->fd;
    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr;
    u32 addr_len = ctx->addrlen;

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        u32 local_ip = bpf_ntohl(addr_in->sin_addr.s_addr);
        u16 local_port = bpf_ntohs(addr_in->sin_port);

        bpf_printk("PID=%d Local IP: %u, Local Port: %d, FD: %d", pid, local_ip, local_port, fd);
    }

    return 0;
}







