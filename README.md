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


struct conn_info_t {
    __be32 ip;  // IPv4 address
    __be16 port;  // Port
};

#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <net/sock.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>

struct conn_info_t {
    __be32 ip;  // IPv4 address
    __be16 port;  // Port
};

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;  // Дескриптор сокета

    if (ret < 0) {
        bpf_printk("UDP sys_exit_accept4: Accept failed\n");
        return 0;
    }

    struct sock *sk = (struct sock *)ret;  // Преобразуем дескриптор сокета в указатель на сокет

    if (sk == NULL) {
        bpf_printk("UDP sys_exit_accept4: Invalid socket\n");
        return 0;
    }

    // Используем BPF-хелперы для получения информации о сокете
    struct inet_sock *inet = (struct inet_sock *)sk;  // Преобразуем сокет в inet_sock (IPv4)

    if (inet == NULL) {
        bpf_printk("UDP sys_exit_accept4: No inet_sock found\n");
        return 0;
    }

    __be32 ip = inet->inet_daddr;  // Удалённый IP-адрес
    __be16 port = inet->inet_dport;  // Удалённый порт

    // Преобразуем IP в строковый формат
    char ip_str[16];  // Строка для хранения IP
    bpf_probe_read_str(ip_str, sizeof(ip_str), &ip);  // Преобразуем IP в строку

    // Преобразуем порт в строковый формат
    u16 port_host = ntohs(port);  // Преобразуем порт из сетевого порядка в хостовый

    // Логируем информацию о соединении в формате IP:PORT
    bpf_printk("UDP sys_exit_accept4: Remote IP: %s, Remote Port: %u\n", ip_str, port_host);

    // Сохраняем информацию о соединении в карте
    struct conn_info_t conn_info = {
        .ip = ip,
        .port = port,
    };

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

