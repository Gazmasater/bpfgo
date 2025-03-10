nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int fd = ctx->args[0];
    long ret = ctx->ret;

    if (ret < 0) return 0;  // Ошибка соединения, ничего не делаем

    struct socket *sock = sockfd_lookup(fd, NULL); // Получаем socket по fd
    if (!sock) return 0;

    struct sock *sk = sock->sk;  // Получаем struct sock
    if (!sk) return 0;

    u32 local_ip = sk->__sk_common.skc_rcv_saddr;  // Локальный IP
    u16 local_port = sk->__sk_common.skc_num;  // Локальный порт

    bpf_printk("PID=%d Local IP=%pI4 Port=%d\n", pid, &local_ip, ntohs(local_port));

    return 0;
}




