lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct sock *sk = ctx->sk;  // Получаем указатель на сокет
    __u32 proto = sk->sk_protocol;  // Получаем протокол из сокета

    // Выводим протокол для отладки
    bpf_printk("Protocol: %u\n", proto);

    return SK_PASS;  // Пропускаем пакет
}


