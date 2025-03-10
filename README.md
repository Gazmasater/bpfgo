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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, int);
} fd_map SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;  // Получаем PID
    int fd = ctx->fd;

    bpf_map_update_elem(&fd_map, &pid, &fd, BPF_ANY);
    
    bpf_printk("Saved FD=%d for PID=%d\n", fd, pid);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&fd_map, &pid);
        return 0;
    }

    int *fd_ptr = bpf_map_lookup_elem(&fd_map, &pid);
    if (!fd_ptr) return 0;

    int fd = *fd_ptr;  // Получили fd

    bpf_printk("FD=%d used by PID=%d\n", fd, pid);

    bpf_map_delete_elem(&fd_map, &pid);  // Удаляем fd после использования
    return 0;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir "$(pwd)" -tags linux -type trace_info -go-package=main -target amd64 bpf "$(pwd)/trace.c" -- -I"$(pwd)"
/home/gaz358/myprog/bpfgo/trace.c:397:27: error: call to undeclared function 'sockfd_lookup'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  397 |     struct socket *sock = sockfd_lookup(fd, NULL); // Получаем socket по fd
      |                           ^
/home/gaz358/myprog/bpfgo/trace.c:397:20: error: incompatible integer to pointer conversion initializing 'struct socket *' with an expression of type 'int' [-Wint-conversion]
  397 |     struct socket *sock = sockfd_lookup(fd, NULL); // Получаем socket по fd
      |                    ^      ~~~~~~~~~~~~~~~~~~~~~~~
2 errors generated.
Error: compile: exit status 1





