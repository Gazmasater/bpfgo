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



SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct sys_enter_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    // Получение имени процесса
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    
    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), ctx->addr) != 0) {
        return 0; 
    }

    // Проверка, если процесс уже в процессе
    struct status_t *status = bpf_map_lookup_elem(&status_map, &pid);
    if (status && status->in_progress) {
        return 0; // Пропустить, если уже в процессе
    }

    // Пометить процесс как в процессе
    struct status_t new_status = {.in_progress = true};
    bpf_map_update_elem(&status_map, &pid, &new_status, BPF_ANY);

    // Записать информацию о соединении и адресе
    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Проверка на успешный выход
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    // Получить информацию о соединении
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        return 0;
    }

    // Получение адреса
    void *addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

    struct sockaddr_in addr;
    bpf_probe_read(&addr, sizeof(addr), addr_ptr);

    // Обновить информацию о соединении
    conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
    conn_info->sport = bpf_ntohs(addr.sin_port);

    // Передача данных через bpf_perf_event_output
    struct trace_info info = {};
    info.pid = conn_info->pid;
    info.src_ip = conn_info->src_ip;
    info.sport = conn_info->sport;
    bpf_probe_read_str(&info.comm, sizeof(info.comm), conn_info->comm);

    // Вывод информации в kernel лог
    bpf_printk("UDP sys_exit_recvfrom: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
               info.pid, info.comm,
               (info.src_ip >> 24) & 0xFF, (info.src_ip  >> 16) & 0xFF,
               (info.src_ip >> 8) & 0xFF, info.src_ip  & 0xFF, info.sport);

    // Отправка данных в пользовательское пространство
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

    // Удаление данных о процессе
    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addr_map, &pid);

    // Обновление флага статуса
    struct status_t status = {.in_progress = false};
    bpf_map_update_elem(&status_map, &pid, &status, BPF_ANY);

    return 0;
}
