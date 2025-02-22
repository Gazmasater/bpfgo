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


#include <linux/tcp.h>
#include <linux/sched.h>

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);
    __type(value, struct conn_info_t);
    __uint(max_entries, 1024);
} conn_info_map SEC(".maps");

struct conn_info_t {
    u32 pid;
    char comm[16];
    u16 port;
};

SEC("tracepoint/tcp/tcp_set_state")
int trace_tcp_listen(struct trace_event_raw_tcp_event_sk *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sock *sk = (struct sock *)ctx->sk;

    // Проверяем, что новое состояние - это TCP_LISTEN
    if (ctx->state != TCP_LISTEN) {
        return 0;
    }

    // Читаем порт из структуры сокета
    u16 port = 0;
    bpf_probe_read_kernel(&port, sizeof(port), &sk->__sk_common.skc_num);

    // Преобразуем порт в хостовый порядок
    u16 port_host = bpf_ntohs(port);

    // Заполняем информацию о соединении
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    conn_info.port = port_host;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем информацию о слушающем сокете
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    // Логируем событие
    bpf_printk("Process %s (PID: %d) is listening on port %d\n", conn_info.comm, pid, port_host);

    return 0;
}

int type;
socklen_t len = sizeof(type);
if (getsockopt(sock_fd, SOL_SOCKET, SO_TYPE, &type, &len) == 0) {
    if (type == SOCK_STREAM) {
        // Это TCP-сокет
    } else if (type == SOCK_DGRAM) {
        // Это UDP-сокет
    }
}


