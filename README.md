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


SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Проверяем успешность соединения
    if (ret < 0) {
        return 0;
    }

    // Получаем информацию о соединении
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_accept4: No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Получаем указатель на структуру сокета
    struct sock *sk = sockfd_lookup(ret, NULL);
    if (!sk) {
        bpf_printk("Failed to lookup sock for FD=%ld\n", ret);
        return 0;
    }

    // Заполняем информацию о соединении
    bpf_probe_read_kernel(&conn_info->src_ip, sizeof(conn_info->src_ip), &sk->__sk_common.skc_rcv_saddr);
    bpf_probe_read_kernel(&conn_info->dst_ip, sizeof(conn_info->dst_ip), &sk->__sk_common.skc_daddr);
    bpf_probe_read_kernel(&conn_info->src_port, sizeof(conn_info->src_port), &sk->__sk_common.skc_num);
    bpf_probe_read_kernel(&conn_info->dst_port, sizeof(conn_info->dst_port), &sk->__sk_common.skc_dport);

    // Преобразуем IP и порты в читаемый формат
    u32 src_ip = bpf_ntohl(conn_info->src_ip);
    u32 dst_ip = bpf_ntohl(conn_info->dst_ip);
    u16 src_port = bpf_ntohs(conn_info->src_port);
    u16 dst_port = bpf_ntohs(conn_info->dst_port);

    // Вывод в читаемом формате
    bpf_printk("Connection Established:\n");
    bpf_printk("    PID: %d\n", pid);
    bpf_printk("    Process: %s\n", conn_info->comm);
    bpf_printk("    Source IP: %d.%d.%d.%d\n",
               (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF,
               (src_ip >> 8) & 0xFF, src_ip & 0xFF);
    bpf_printk("    Destination IP: %d.%d.%d.%d\n",
               (dst_ip >> 24) & 0xFF, (dst_ip >> 16) & 0xFF,
               (dst_ip >> 8) & 0xFF, dst_ip & 0xFF);
    bpf_printk("    Source Port: %d\n", src_port);
    bpf_printk("    Destination Port: %d\n", dst_port);

    // Удаляем элемент из карты после обработки
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}



