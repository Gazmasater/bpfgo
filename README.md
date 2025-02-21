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



struct sockaddr_any {
    __u16 family;
    __u32 src_ip[4];  // Подходит и для IPv4, и для IPv6
    __u16 port;
};


if (ctx->upeer_sockaddr) {
    struct sockaddr_any addr = {};
    bpf_probe_read_user(&addr.family, sizeof(addr.family), ctx->upeer_sockaddr);

    if (addr.family == AF_INET) {
        // Для IPv4 читаем адрес и порт
        struct sockaddr_in ipv4_addr = {};
        if (bpf_probe_read_user(&ipv4_addr, sizeof(ipv4_addr), ctx->upeer_sockaddr) == 0) {
            addr.src_ip[0] = ipv4_addr.sin_addr.s_addr;
            addr.port = ipv4_addr.sin_port;
        }
    } else if (addr.family == AF_INET6) {
        // Для IPv6 читаем адрес и порт
        struct sockaddr_in6 ipv6_addr = {};
        if (bpf_probe_read_user(&ipv6_addr, sizeof(ipv6_addr), ctx->upeer_sockaddr) == 0) {
            bpf_probe_read_kernel(&addr.src_ip, sizeof(ipv6_addr.sin6_addr.s6_addr), &ipv6_addr.sin6_addr.s6_addr);
            addr.port = ipv6_addr.sin6_port;
        }
    }

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
}


struct sockaddr_any *addr = bpf_map_lookup_elem(&addr_map, &pid);
if (!addr) {
    bpf_printk("UDP sys_exit_accept4: No sockaddr found for PID=%d\n", pid);
    return 0;
}

if (addr->family == AF_INET) {
    __u32 ip = addr->src_ip[0];
    bpf_printk("IPv4: %d.%d.%d.%d:%d", 
               (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF,
               bpf_ntohs(addr->port));
} else if (addr->family == AF_INET6) {
    bpf_printk("IPv6: Port %d", bpf_ntohs(addr->port));
} else {
    bpf_printk("Unknown family: %d", addr->family);
}
