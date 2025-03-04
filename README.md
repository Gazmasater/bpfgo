gcc udp_server.c -o udp_server

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_accept/format


SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind(struct sys_enter_bind_args *ctx) {
    int fd = ctx->fd;
    struct sockaddr_in *addr = (struct sockaddr_in *)ctx->addr;
    
    u16 port = bpf_ntohs(addr->sin_port);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Сохраняем информацию о сокете и порте в карту
    struct bind_info bind = {};
    bind.fd = fd;
    bind.port = port;

    bpf_map_update_elem(&bind_map, &fd, &bind, BPF_ANY);
    
    return 0;
}



