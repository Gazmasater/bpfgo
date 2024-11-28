SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct sys_enter_bind_args *ctx)
{
    // Получаем PID текущего процесса
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Инициализируем информацию о соединении
    init_conn_info_bind(ctx);

    // Извлекаем информацию из карты по PID
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
    if (conn_info)
    {
        // Проверяем семейство адресов (IPv4 или IPv6)
        if (conn_info->sock_family == AF_INET || conn_info->sock_family == AF_INET6)
        {
            // Проверяем тип сокета (TCP или UDP)
            if (conn_info->sock_type == SOCK_STREAM) // TCP сокеты
            {
                bpf_printk("TCP Bind entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
            }
            else if (conn_info->sock_type == SOCK_DGRAM) // UDP сокеты
            {
                bpf_printk("UDP Bind entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
            }
        }
    }

    return 0;
}
