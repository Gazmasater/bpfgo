nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/ptrace.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <bpf/bpf_helpers.h>

struct sock_info_t {
    __be32 local_ip;
    __be16 local_port;
    __be32 remote_ip;
    __be16 remote_port;
};

// Мапа для хранения данных о соединениях по PID процесса
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct sock_info_t);
} sock_map SEC(".maps");

// Перехватываем вход в sys_connect (fix: не используем user-space адрес напрямую)
SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect(struct trace_event_raw_sys_enter *ctx) {
    int sockfd = ctx->args[0];  
    struct sockaddr *addr_ptr;
    bpf_probe_read_user(&addr_ptr, sizeof(addr_ptr), (void *)ctx->args[1]);
    
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sock_info_t info = {};

    struct sockaddr_in addr;
    bpf_probe_read_user(&addr, sizeof(addr), addr_ptr);

    if (addr.sin_family == AF_INET) {
        info.remote_ip = addr.sin_addr.s_addr;
        info.remote_port = addr.sin_port;
        bpf_map_update_elem(&sock_map, &pid, &info, BPF_ANY);
    }

    return 0;
}

// Перехватываем выход из sys_connect
SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sock_info_t *info = bpf_map_lookup_elem(&sock_map, &pid);
    if (!info) return 0;

    // Получаем локальный IP и порт
    struct task_struct *task = (struct task_struct *)bpf_get_current_task();
    struct file *file = bpf_task_fd_get(task, ctx->ret);  
    if (!file) return 0;

    struct socket *sock = (struct socket *)file->private_data;
    if (!sock) return 0;

    struct sock *sk = sock->sk;
    if (!sk) return 0;

    info->local_ip = sk->sk_rcv_saddr;
    info->local_port = sk->sk_num;

    // Вывод через bpf_printk()
    bpf_printk("Local IP: %pI4, Local Port: %d", &info->local_ip, bpf_ntohs(info->local_port));

    // Удаляем запись из мапы
    bpf_map_delete_elem(&sock_map, &pid);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
