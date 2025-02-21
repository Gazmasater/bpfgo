nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

sudo bpftool map dump id 11




11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


bpf2go -output-dir $(pwd) \
  -tags linux \
  -type trace_info \
  -go-package main \
  target_amd64_bpf \
  $(pwd)/trace.c -- -I$(pwd)


  struct status_t {
    bool in_progress;
};

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем указатель на структуру sockaddr
    struct sockaddr_in *addr_ptr = (struct sockaddr_in *)ctx->upeer_sockaddr;
    if (!addr_ptr) {
        return 0;
    }

    // Добавляем статус в status_map
    struct status_t *status = bpf_map_lookup_elem(&status_map, &pid);
    if (status && status->in_progress) {
        return 0;  // Если соединение уже в процессе, ничего не делаем
    }

    struct status_t new_status = {.in_progress = true};
    bpf_map_update_elem(&status_map, &pid, &new_status, BPF_ANY);

    // Сохраняем указатель на sockaddr в карту
    bpf_map_update_elem(&addr_map, &pid, &addr_ptr, BPF_ANY);

    // Сохраняем информацию о соединении
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    bpf_printk("SERVER sys_enter_accept4: PID=%d, Comm=%s\n", conn_info.pid, conn_info.comm);

    return 0;
}



SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;  // Дескриптор нового соединения

    // Если произошла ошибка при вызове accept4()
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        bpf_map_delete_elem(&status_map, &pid);
        return 0;
    }

    // Извлекаем информацию о соединении
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_accept4: No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    bpf_printk("Comm=%s", conn_info->comm);

    // Извлекаем указатель на sockaddr
    struct sockaddr_in **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr || !*addr_ptr) {
        bpf_printk("UDP sys_exit_accept4: No sockaddr found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    // Считываем данные о клиенте
    struct sockaddr_in addr = {};
    if (bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr) != 0) {
        bpf_printk("UDP sys_exit_accept4: Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    // Проверяем, что это IPv4
    if (addr.sin_family == AF_INET) {
        u32 src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        u16 src_port = bpf_ntohs(addr.sin_port);

        bpf_printk("Accepted connection from %d.%d.%d.%d:%d (PID=%d)\n",
                   (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF,
                   (src_ip >> 8) & 0xFF, src_ip & 0xFF, src_port, pid);
    }

    // Обновляем статус на false (отсутствие активности)
    struct status_t status = {.in_progress = false};
    bpf_map_update_elem(&status_map, &pid, &status, BPF_ANY);

    // Удаляем временные данные из карт
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addr_map, &pid);

    return 0;
}

