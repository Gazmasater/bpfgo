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


static __always_inline int init_conn_info(struct sockaddr *sock_addr, u32 pid)
{
    struct sockaddr_in addr = {};
    bpf_probe_read(&addr, sizeof(addr), sock_addr);

    // Сохраняем sockaddr в новую карту
    bpf_map_update_elem(&sockaddr_map, &pid, &addr, BPF_ANY);

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}

struct sockaddr_in *addr = bpf_map_lookup_elem(&sockaddr_map, &pid);
if (!addr)
{
    bpf_printk("EXIT_accept4 No sockaddr found for PID=%d\n", pid);
    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

if (addr->sin_family == AF_INET)
{
    conn_info->src_ip = bpf_ntohl(addr->sin_addr.s_addr);
    conn_info->sport = bpf_ntohs(addr->sin_port);

    bpf_printk("EXIT_accept4 Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
               conn_info->pid, conn_info->comm,
               (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
               (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
}

// Удаляем sockaddr после использования
bpf_map_delete_elem(&sockaddr_map, &pid);
