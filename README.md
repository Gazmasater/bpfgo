static __always_inline int init_conn_info_bind(struct sys_enter_bind_args *ctx) {
    struct conn_info_t conn_info = {};
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    // Получаем PID и имя процесса
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Копируем данные sockaddr сразу в conn_info
    struct sockaddr_in addr;
    if (bpf_probe_read_user(&addr, sizeof(addr), (void *)ctx->umyaddr) == 0) {
        conn_info.sock_addr_in = addr; // Сохраняем копию структуры
        bpf_printk("COPY sockaddr_in");
    } else {
        bpf_printk("Failed to read sockaddr in sys_enter_bind for PID=%d\n", pid);
        return -1;  // Если не удалось прочитать указатель, выходим с ошибкой
    }

    // Обновляем карту с информацией о соединении
    bpf_map_update_elem(&conn_info_map_bind, &pid, &conn_info, BPF_ANY);

    return 0;
}


SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_ret(struct sys_exit_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Если результат отрицательный, значит произошла ошибка
    if (ret < 0) {
        bpf_printk("trace __sys_bind: Bind failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_bind, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
    if (!conn_info) {
        bpf_printk("trace __sys_bind: No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем IP и порт из сохраненной структуры sockaddr_in
    if (conn_info->sock_addr_in.sin_family == AF_INET) {
        conn_info->dst_ip = bpf_ntohl(conn_info->sock_addr_in.sin_addr.s_addr);
        conn_info->dport = bpf_ntohs(conn_info->sock_addr_in.sin_port);

        bpf_printk("trace __sys_bind: SERVER Bound address: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
            conn_info->pid, conn_info->comm,
            (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
            (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
    }

    // Удаляем запись из карты, если больше не нужна
    bpf_map_delete_elem(&conn_info_map_bind, &pid);

    return 0;
}
