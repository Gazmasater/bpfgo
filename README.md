lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  // Ключ - PID процесса
    __type(value, char[256]); // Значение - строка с доменным именем
} dns_map SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  
    __type(value, struct sockaddr_in);
} addr_map SEC(".maps");

// Перехватываем sendto (syscall enter)
SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct sockaddr_in addr = {};
    struct sockaddr *addr_ptr;

    // Читаем указатель на sockaddr
    bpf_probe_read_user(&addr_ptr, sizeof(addr_ptr), (void *)ctx->dest_addr);

    // Если указатель есть, читаем содержимое
    if (addr_ptr) {
        bpf_probe_read_user(&addr, sizeof(addr), addr_ptr);
        
        // Сохраняем, если это DNS (порт 53)
        if (addr.sin_family == AF_INET && bpf_ntohs(addr.sin_port) == 53) {
            bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
        }
    }

    return 0;
}

// Перехватываем sendto (syscall exit)
SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Достаем сохраненный адрес
    struct sockaddr_in *addr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr) {
        return 0;
    }

    // Читаем DNS-запрос (он идёт в буфере sendto)
    char query[256] = {};
    bpf_probe_read_user(&query, sizeof(query), (void *)ctx->buf);

    // Выводим результат
    bpf_printk("DNS Query: PID=%d, Dest=%d.%d.%d.%d, Query=%s",
        pid,
        (addr->sin_addr.s_addr >> 24) & 0xFF,
        (addr->sin_addr.s_addr >> 16) & 0xFF,
        (addr->sin_addr.s_addr >> 8) & 0xFF,
        (addr->sin_addr.s_addr) & 0xFF,
        query);

    // Удаляем запись
    bpf_map_delete_elem(&addr_map, &pid);

    return 0;
}










