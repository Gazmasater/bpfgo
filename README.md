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


SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;
    struct conn_info_t conn_info = {};

    // Проверка на ошибку
    if (ret < 0) {
        bpf_printk("EXIT_accept Accept4 failed for PID=%d\n", pid);
        return 0;
    }

    // Используем bpf_probe_read для безопасного чтения структуры sockaddr_in
    struct sockaddr_in client_addr;
    if (ctx->addr) {
        bpf_probe_read(&client_addr, sizeof(client_addr), (void *)ctx->addr);
        conn_info.client_ip = client_addr.sin_addr.s_addr;
        conn_info.client_port = ntohs(client_addr.sin_port);
    }

    // Получаем имя процесса
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Логируем информацию о процессе и соединении
    bpf_printk("Accept4: PID=%d, Comm=%s, Client IP=%u.%u.%u.%u, Client Port=%d\n",
               pid,
               conn_info.comm,
               (conn_info.client_ip >> 24) & 0xFF, 
               (conn_info.client_ip >> 16) & 0xFF, 
               (conn_info.client_ip >> 8) & 0xFF, 
               conn_info.client_ip & 0xFF,
               conn_info.client_port);

    // Сохраняем информацию в карту
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}
