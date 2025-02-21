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


#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <net/sock.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>

SEC("tracepoint/tcp/tcp_connect")
int bpf_prog(struct trace_event_raw_tcp_connect *ctx) {
    struct sock *sk = (struct sock *)ctx->sk;  // Получаем указатель на сокет

    if (sk == NULL) {
        return 0; // Проверка на null
    }

    // Получаем IPv4 адрес и порт
    struct inet_sock *inet = inet_sk(sk);
    __be32 ip = inet->inet_daddr;  // Удалённый IP
    __be16 port = inet->inet_dport; // Удалённый порт

    // Логирование IP и порта
    bpf_printk("Remote IP: %pI4, Remote Port: %u\n", &ip, ntohs(port));

    return 0;
}
