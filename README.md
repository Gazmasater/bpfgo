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


#include <linux/bpf.h>
#include <linux/inet.h>
#include <linux/if_ether.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/ipv6.h>

#define MAX_COMM_LEN 128

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[MAX_COMM_LEN];
};

// Определяем карту для хранения информации о соединении
struct bpf_map_def SEC("maps") conn_info_map = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u32),  // Ключ - PID процесса
    .value_size = sizeof(struct conn_info_t),  // Значение - информация о соединении
    .max_entries = 1024,
};

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    long ret = ctx->ret;

    // Если операция завершена с ошибкой
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    // Преобразуем дескриптор сокета в указатель на сокет
    struct sock *sk = (struct sock *)ret;

    // Если сокет не существует
    if (sk == NULL) {
        bpf_printk("Invalid socket descriptor\n");
        return 0;
    }

    // Читаем только необходимые данные из inet_sock
    u16 port = 0;
    if (bpf_probe_read(&port, sizeof(port), &sk->sk_num)) {
        bpf_printk("Failed to read port\n");
        return 0;
    }

    // Преобразуем порт в хостовый порядок
    u16 port_host = bpf_ntohs(port);
    conn_info.sport = port_host;

    // Логируем информацию о процессе и порте
    bpf_printk("sys_exit_accept4: Comm=%s, Src Port=%u\n", conn_info.comm, conn_info.sport);

    // Сохраняем информацию о соединении в карту
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

