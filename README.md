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


#include <linux/sock.h>
#include <linux/net.h>
#include <linux/in.h>

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // Проверяем успешность вызова
    if (ret < 0) {
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Используем файловый дескриптор для извлечения сокетной информации
    struct sockaddr_in addr = {};
    struct sock *sk = sock_from_file(ret);
    if (!sk) {
        bpf_printk("Failed to retrieve socket for PID=%d\n", pid);
        return 0;
    }

    // Читаем IP и порт из сокета
    bpf_probe_read_kernel(&addr, sizeof(addr), &sk->sk_daddr);
    addr.sin_port = sk->sk_dport;

    // Преобразуем IP и порт в читаемый вид
    u32 src_ip = bpf_ntohl(addr.sin_addr.s_addr);
    u16 src_port = bpf_ntohs(addr.sin_port);

    bpf_printk("New Connection:\n");
    bpf_printk("    PID: %d\n", pid);
    bpf_printk("    Process: %s\n", conn_info->comm);
    bpf_printk("    IP: %d.%d.%d.%d\n",
               (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF,
               (src_ip >> 8) & 0xFF, src_ip & 0xFF);
    bpf_printk("    Port: %d\n", src_port);

    // Удаляем данные из карты
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}
