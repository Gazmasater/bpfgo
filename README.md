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


#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

struct conn_info_t {
    u32 pid;
    u16 sport;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

// Трейспоинт для sys_exit_accept4
SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    long ret = ctx->ret;

    // Проверка на ошибку
    if (ret < 0) {
        return 0;
    }

    // Преобразуем файловый дескриптор в указатель на сокет
    struct sock *sk = bpf_sock_from_fd(ret);
    if (!sk) {
        bpf_printk("Failed to get sock from fd\n");
        return 0;
    }

    // Считываем порт с помощью BPF-helpers
    u16 sport = 0;
    bpf_probe_read_kernel(&sport, sizeof(sport), &sk->__sk_common.skc_num);

    // Преобразуем порт в хостовый порядок
    u16 port_host = bpf_ntohs(sport);
    conn_info.sport = port_host;

    // Логируем информацию о процессе и порте
    bpf_printk("sys_exit_accept4: Comm=%s, Src Port=%u\n", conn_info.comm, conn_info.sport);

    // Сохраняем информацию о соединении в карте
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir $(pwd)   -tags linux   -type trace_info   -go-package main   target_amd64_bpf   $(pwd)/trace.c -- -I$(pwd)
/home/gaz358/myprog/bpfgo/trace.c:221:23: error: call to undeclared function 'bpf_sock_from_fd'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  221 |     struct sock *sk = bpf_sock_from_fd(ret);
      |                       ^
/home/gaz358/myprog/bpfgo/trace.c:221:18: error: incompatible integer to pointer conversion initializing 'struct sock *' with an expression of type 'int' [-Wint-conversion]
  221 |     struct sock *sk = bpf_sock_from_fd(ret);
      |                  ^    ~~~~~~~~~~~~~~~~~~~~~
2 errors generated.
