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



SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Если операция завершилась с ошибкой
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_accept4: No connection info found for PID=%d\n", pid);
        return 0;
    }

    bpf_printk("222222222222222222Comm=%s", conn_info->comm);

    // Читаем sockaddr напрямую из аргументов системного вызова
    struct sockaddr_in addr = {};
    if (ctx->upeer_sockaddr != NULL) {
        if (bpf_probe_read_user(&addr, sizeof(addr), ctx->upeer_sockaddr) != 0) {
            bpf_printk("UDP sys_exit_accept4: Failed to read sockaddr for PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map, &pid);
            return 0;
        }
    } else {
        bpf_printk("UDP sys_exit_accept4: upeer_sockaddr is NULL for PID=%d\n", pid);
        return 0;
    }

    bpf_printk("4444444444444444444 Comm=%s FAMILY=%d", conn_info->comm, addr.sin_family);

    // Если это IPv4, обновляем информацию
    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        struct trace_info info = {};
        info.pid = conn_info->pid;
        info.src_ip = conn_info->src_ip;
        info.sport = conn_info->sport;
        bpf_probe_read_str(&info.comm, sizeof(info.comm), conn_info->comm);

        // Выводим информацию о соединении
        bpf_printk("UDP sys_exit_accept4: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   info.pid, info.comm,
                   (info.src_ip >> 24) & 0xFF, (info.src_ip >> 16) & 0xFF,
                   (info.src_ip >> 8) & 0xFF, info.src_ip & 0xFF, info.sport);

        // Отправляем информацию в пользовательское пространство
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    // Обновляем статус на false (отсутствие активности)
    struct status_t status = {.in_progress = false};
    bpf_map_update_elem(&status_map, &pid, &status, BPF_ANY);

    // Очистка карт после завершения
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}
