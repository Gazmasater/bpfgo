#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/sched.h>
#include <linux/in.h>
#include <linux/ip.h>

struct event {
    __be32 saddr;           // Исходный адрес
    __be32 daddr;           // Целевой адрес
    __be16 dport;           // Целевой порт
    __be16 sport;           // Исходный порт
    char comm[TASK_COMM_LEN]; // Название команды
    pid_t pid;              // Идентификатор процесса
};

struct bpf_ringbuf **events; // Предполагается, что это инициализировано в другом месте

SEC("tracepoint/sock/sock_create")
int BPF_PROG(sock_create, struct sock *sk)
{
    struct event *tcp_info;

    // Резервируем место в кольцевом буфере
    tcp_info = bpf_ringbuf_reserve(events, sizeof(struct event), 0);
    if (!tcp_info) {
        return 0; // Нет места в кольцевом буфере
    }

    // Проверяем семью сокетов для IPv4
    if (sk->__sk_common.skc_family == AF_INET) {
        tcp_info->saddr = sk->__sk_common.skc_rcv_saddr;
        tcp_info->daddr = sk->__sk_common.skc_daddr;
        tcp_info->dport = sk->__sk_common.skc_dport;
        tcp_info->sport = bpf_htons(sk->__sk_common.skc_num);
    }
    // Опционально: Обработка IPv6
    // else if (sk->__sk_common.skc_family == AF_INET6) {
    //     // Добавьте логику для обработки IPv6
    // }

    // Получаем информацию о текущем процессе
    bpf_get_current_comm(&tcp_info->comm, TASK_COMM_LEN);
    tcp_info->pid = bpf_get_current_pid_tgid() >> 32; // Получаем PID

    // Отправляем событие в кольцевой буфер
    bpf_ringbuf_submit(tcp_info, 0);

    return 0;
}

