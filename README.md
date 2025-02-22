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
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    long ret = ctx->ret;

    // Если операция завершена с ошибкой
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    // Преобразуем дескриптор сокета в указатель на сокет
    struct sock *sk = (struct sock *)ret;

    // Если сокет не существует
    if (sk == NULL) {
        bpf_printk("Invalid socket descriptor\n");
        return 0;
    }

    // Преобразуем сокет в структуру inet_sock для получения информации о порте
    struct inet_sock inet;
    if (bpf_probe_read(&inet, sizeof(inet), sk)) {
        bpf_printk("Failed to read inet_sock\n");
        return 0;
    }

    // Получаем исходный порт
    u16 port = inet.inet_sport;

    // Преобразуем порт в хостовый порядок
    u16 port_host = bpf_ntohs(port);
    conn_info.sport = port_host;

    // Логируем информацию о процессе и порте
    bpf_printk("sys_exit_accept4: Comm=%s, Src Port=%u\n", conn_info.comm, conn_info.sport);

    return 0;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir $(pwd)   -tags linux   -type trace_info   -go-package main   target_amd64_bpf   $(pwd)/trace.c -- -I$(pwd)
/home/gaz358/myprog/bpfgo/trace.c:206:5: error: Looks like the BPF stack limit is exceeded. Please move large on stack variables into BPF per-cpu array map. For non-kernel uses, the stack can be increased using -mllvm -bpf-stack-size.

  206 | int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
      |     ^
/home/gaz358/myprog/bpfgo/trace.c:206:5: note: could not determine the original source location for ./trace.c:0:0
/home/gaz358/myprog/bpfgo/trace.c:238:21: error: Looks like the BPF stack limit is exceeded. Please move large on stack variables into BPF per-cpu array map. For non-kernel uses, the stack can be increased using -mllvm -bpf-stack-size.

  238 |     u16 port = inet.inet_sport;
      |                     ^
2 errors generated.
Error: compile: exit status 1
