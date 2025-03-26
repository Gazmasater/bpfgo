lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  // Ключ - PID процесса
    __type(value, char[256]); // Значение - строка с доменным именем
} dns_map SEC(".maps");


SEC("tracepoint/syscalls/sys_enter_getaddrinfo")
int trace_enter_getaddrinfo(struct sys_enter_getaddrinfo_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    
    char name[256] = {};
    bpf_probe_read_user_str(name, sizeof(name), (char *)ctx->name);

    bpf_map_update_elem(&dns_map, &pid, name, BPF_ANY);
    
    return 0;
}


SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) return 0;

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};
        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);
        u16 port = bpf_ntohs(addr_in.sin_port);

        struct ip_port_key key = {
            .ip = ip,
            .port = port,
        };

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.dst_ip = ip;
        info.dport = port;

        // Получаем доменное имя, если оно было запрошено
        char *domain = bpf_map_lookup_elem(&dns_map, &pid);
        if (domain) {
            bpf_printk("DNS: %s -> %d.%d.%d.%d:%d\n", domain, 
                (key.ip>>24)&0xff, (key.ip>>16)&0xff, (key.ip>>8)&0xff, key.ip&0xff, key.port);
            
            bpf_map_delete_elem(&dns_map, &pid); // Чистим карту после обработки
        } else {
            bpf_printk("IP: %d.%d.%d.%d:%d\n", 
                (key.ip>>24)&0xff, (key.ip>>16)&0xff, (key.ip>>8)&0xff, key.ip&0xff, key.port);
        }

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    bpf_map_delete_elem(&addr_map, &pid);
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}






