SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Удаление информации о соединении в случае ошибки вызова
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct sock *sk;
    struct sockaddr_in addr;
    struct task_struct *task;

    // Получение текущего процесса
    task = (struct task_struct *)bpf_get_current_task();

    // Чтение структуры сокета
    sk = BPF_CORE_READ(task, files->fdt->fd[ctx->fd]->private_data);
    if (!sk) {
        bpf_printk("UDP sys_exit_sendto: Failed to read socket for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    // Чтение адреса и порта из сокета
    addr.sin_family = BPF_CORE_READ(sk, __sk_common.skc_family);
    addr.sin_addr.s_addr = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
    addr.sin_port = BPF_CORE_READ(sk, __sk_common.skc_num);

    if (addr.sin_family == AF_INET) {
        u32 src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        u16 sport = bpf_ntohs(addr.sin_port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, IP=%d.%d.%d.%d, Port=%d\n",
                   pid,
                   (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF,
                   (src_ip >> 8) & 0xFF, src_ip & 0xFF, sport);
    }

    return 0;
}
