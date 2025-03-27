lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


ls /sys/kernel/debug/tracing/events/syscalls/ | grep getaddrinfo
ls /sys/kernel/debug/tracing/events/syscalls/ | grep socket


#include <linux/ptrace.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/tcp.h>

struct conn_info_t {
    u32 pid;
    char comm[16];
    __be32 src_ip;  // Локальный IP
    __be32 dst_ip;  // Целевой IP
    u16 dst_port;   // Целевой порт
};

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;  // Получаем PID процесса
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));  // Получаем имя процесса

    // Извлекаем sockaddr (адрес) из аргументов системного вызова
    struct sockaddr_in *addr = (struct sockaddr_in *)ctx->uservaddr;
    
    // Проверяем, что это IPv4-адрес
    if (addr->sin_family == AF_INET) {
        // Извлекаем целевой IP и порт
        conn_info.dst_ip = addr->sin_addr.s_addr;
        conn_info.dst_port = ntohs(addr->sin_port);  // Преобразуем порт из сети в хост-байт порядок

        // Записываем в карту с ключом pid
        bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    }

    return 0;
}


sys_enter_getsockname
sys_enter_getsockopt



