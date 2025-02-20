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



SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct sys_enter_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr_in addr = {};
    if (ctx->addr) {
        if (bpf_probe_read(&addr, sizeof(addr), ctx->addr) != 0) {
            bpf_printk("ENTER: Failed to read sockaddr for PID=%d\n", pid);
            return 0;
        }
    }

    bpf_printk("ENTER: PID=%d, Comm=%s, Family=%d\n", pid, conn_info.comm, addr.sin_family);

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("EXIT: No conn_info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in *addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        bpf_printk("EXIT: No addr_map found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr_in addr = {};
    if (bpf_probe_read(&addr, sizeof(addr), addr_ptr) != 0) {
        bpf_printk("EXIT: Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);
        return 0;
    }

    bpf_printk("EXIT: PID=%d, Family=%d\n", pid, addr.sin_family);

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        struct trace_info info = {};
        info.pid = conn_info->pid;
        info.src_ip = conn_info->src_ip;
        info.sport = conn_info->sport;
        bpf_probe_read_str(&info.comm, sizeof(info.comm), conn_info->comm);

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        bpf_printk("EXIT: Connection PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   info.pid, info.comm,
                   (info.src_ip >> 24) & 0xFF, (info.src_ip >> 16) & 0xFF,
                   (info.src_ip >> 8) & 0xFF, info.src_ip & 0xFF, info.sport);
    }

    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/trace_pipe|grep "Comm=udp"
      udp_server-5531    [005] ...21   262.815290: bpf_trace_printk: sys_enter_recvfrom: PID=5531, Comm=udp_server, Family=49725
           <...>-5572    [007] ...21   267.985092: bpf_trace_printk: SERVER sys_enter_sendto: PID=5572, Comm=udp_client
           <...>-5572    [007] ...21   267.985176: bpf_trace_printk: UDP sys_exit_sendto: Connection: PID=5572, Comm=udp_client, IP=0.0.0.0, Port=33333
      udp_server-5531    [000] ...21   267.985218: bpf_trace_printk: sys_enter_recvfrom: PID=5531, Comm=udp_server, Family=2
