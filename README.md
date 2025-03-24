lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


SEC("sockops")
int bpf_sock_ops(struct bpf_sock_ops *ctx) {
    struct bpf_sock *sk = ctx->sk;
    if (!sk) {
        return 0;
    }

    __u32 state = sk->state;  // Получаем состояние TCP-сокета

    bpf_printk("TCP STATE: %d", state);

    return 0;
}



