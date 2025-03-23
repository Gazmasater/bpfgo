lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct sock *sk = ctx->sk;  // Получаем сокет

    // Получаем состояние сокета
    __u32 state = sk->sk_state;

    // Выводим состояние сокета
    bpf_printk("Socket state: %u\n", state);

    return SK_PASS;
}


