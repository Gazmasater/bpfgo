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


SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_entry(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Создаём структуру sockaddr_in для хранения данных
    struct sockaddr_in addr;
    
    // Копируем данные из указателя в структуру
    if (bpf_probe_read(&addr, sizeof(addr), ctx->upeer_sockaddr) != 0) {
        bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    // Сохраняем sockaddr в карту, вместо указателя на структуру
    bpf_map_update_elem(&sockaddr_map, &pid, &addr, BPF_ANY);

    bpf_printk("SERVER accept4 entry: PID=%d\n", pid);

    return 0;
}
