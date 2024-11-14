SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept_entry(struct sys_enter_accept4_args *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    
    // Создаём структуру для хранения информации о соединении
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    conn_info.sock_addr = ctx->upeer_sockaddr;
    
    // Записываем данные о процессе в карту
    struct conn_info_t *conn_info_ptr = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
    
    if (conn_info_ptr) {
        // Если данные есть в карте, выводим информацию о процессе
        bpf_printk("CLIENT accept4 entry: PID=%d, Comm=%s\n", pid, conn_info_ptr->comm);
    } else {
        // Если данных нет в карте, выводим сообщение
        bpf_printk("No connection info for PID=%d\n", pid);
    }
    
    // Для отладки выводим указатель на структуру conn_info_ptr
    bpf_printk("conn_info_ptr=%p\n", conn_info_ptr);

    return 0;
}
