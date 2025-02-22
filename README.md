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


SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Инициализация структуры conn_info_t напрямую
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct conn_info_t *existing_conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!existing_conn_info) {
        bpf_map_update_elem(&conn_info_map_sc, &pid, &conn_info, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Если произошла ошибка, удаляем элемент из карты
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("UDP sys_exit_sendto: Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    // Обновляем информацию о соединении
    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);

    return 0;
}
