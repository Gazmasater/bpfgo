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


// Структура для хранения информации о соединении
struct conn_info_t {
    struct sockaddr_in sock_addr; // Заменили указатель на структуру
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[128];
};

// Остальная часть кода остается без изменений
SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_entry(struct sys_enter_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Инициализация структуры conn_info_t напрямую
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Копирование данных о соксете в поле sock_addr
    struct sockaddr_in *peer_sockaddr = (struct sockaddr_in *)ctx->upeer_sockaddr;
    if (peer_sockaddr) {
        bpf_probe_read(&conn_info.sock_addr, sizeof(conn_info.sock_addr), peer_sockaddr);
    }

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", conn_info.pid, conn_info.comm);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("EXIT_accept4 Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("EXIT_accept4 No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr = conn_info->sock_addr; // Используем копию структуры

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("EXIT_accept4 Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);

    return 0;
}
