clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u16 sport;
    u16 dport;
    char comm[64];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sockaddr_in);
} addr_map SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем указатель на структуру sockaddr в карте
    struct sockaddr_in *addr = (struct sockaddr_in *)ctx->addr;
    bpf_map_update_elem(&addr_map, &pid, addr, BPF_ANY);

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in *addr = bpf_map_lookup_elem(&addr_map, &pid);
    if (addr && addr->sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr->sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr->sin_port);
    }

    bpf_printk("UDP sys_exit_sendto: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
               conn_info->pid, conn_info->comm,
               (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
               (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

    bpf_map_delete_elem(&conn_info_map, &pid);
    bpf_map_delete_elem(&addr_map, &pid);
    return 0;
}
