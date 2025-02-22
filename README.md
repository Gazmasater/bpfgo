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


struct sockaddr_in *saved_addr = bpf_map_lookup_elem(&addr_map, &pid);
if (!saved_addr) {
    bpf_printk("EXIT_accept4 No saved sockaddr for PID=%d\n", pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

if (saved_addr->sin_family == AF_INET) {
    conn_info->src_ip = bpf_ntohl(saved_addr->sin_addr.s_addr);
    conn_info->sport = bpf_ntohs(saved_addr->sin_port);

    bpf_printk("EXIT_accept4 Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
               conn_info->pid, conn_info->comm,
               (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
               (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
}

// Обновляем conn_info
bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);
bpf_map_delete_elem(&addr_map, &pid); // Удаляем сохранённый адрес после использования
