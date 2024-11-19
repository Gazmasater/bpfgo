static __always_inline int init_conn_info_bind(struct sys_enter_bind_args *ctx) {
    struct conn_info_t conn_info = {};
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    // Получаем PID и имя процесса
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Чтение указателя на sockaddr
    struct sockaddr *sock_addr;
    if (bpf_probe_read(&sock_addr, sizeof(sock_addr), &ctx->umyaddr) != 0) {
        bpf_printk("Failed to read sock_addr pointer at address %p", ctx->umyaddr);
        return -1;  // Ошибка чтения указателя
    }

    // Проверка на NULL для sock_addr
    if (!sock_addr) {
        bpf_printk("sock_addr is NULL");
        return -1;  // Указатель на сокет NULL
    }

    // Чтение структуры sockaddr из пользовательского пространства
    struct sockaddr_in addr;
    if (bpf_probe_read_user(&addr, sizeof(addr), (void *)sock_addr) != sizeof(addr)) {
        bpf_printk("Failed to read sockaddr from user space at address %p", sock_addr);
        return -1;  // Ошибка чтения из пользовательского пространства
    }

    // Сохранение адреса сокета
    conn_info.sock_addr = sock_addr;

    // Обновляем карту с информацией о соединении
    bpf_map_update_elem(&conn_info_map_bind, &pid, &conn_info, BPF_ANY);

    return 0;
}
