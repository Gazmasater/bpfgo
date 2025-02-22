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

    // Сохраняем sockaddr в карту
    bpf_map_update_elem(&sockaddr_map, &pid, ctx->upeer_sockaddr, BPF_ANY);

    bpf_printk("SERVER accept4 entry: PID=%d\n", pid);

    return 0;
}

// Обработчик выхода из sys_exit_accept4
SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("EXIT_accept4 Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&sockaddr_map, &pid);
        return 0;
    }

    // Извлекаем sockaddr из карты
    struct sockaddr *sock_addr = bpf_map_lookup_elem(&sockaddr_map, &pid);
    if (!sock_addr) {
        bpf_printk("EXIT_accept4 No sockaddr found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), sock_addr) != 0) {
        bpf_printk("EXIT_accept4 Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&sockaddr_map, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET) {
        u32 src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        u16 sport = bpf_ntohs(addr.sin_port);

        bpf_printk("EXIT_accept4 Accepted connection: PID=%d, IP=%d.%d.%d.%d, Port=%d\n",
                   pid,
                   (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF,
                   (src_ip >> 8) & 0xFF, src_ip & 0xFF, sport);
    }

    bpf_map_delete_elem(&sockaddr_map, &pid);

    return 0;
}

