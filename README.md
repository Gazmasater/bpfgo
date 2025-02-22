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


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  // PID в качестве ключа
    __type(value, struct sockaddr_in); // Хранение копии sockaddr
} sockaddr_map SEC(".maps");


static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Копируем данные сокета в локальную структуру
    struct sockaddr_in addr = {};
    if (bpf_probe_read(&addr, sizeof(addr), sock_addr) != 0) {
        bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
        return -1;
    }

    // Сохраняем скопированные данные в отдельную карту
    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);

    bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
    return 0;
}
