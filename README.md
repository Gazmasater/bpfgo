lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


    struct sock *sk=ctx->sk;

    __u32 state=sk->sk_state_change;

    bpf_printk("STATE sk_lookup=%d",state);

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir "$(pwd)" -tags linux -type trace_info -go-package=main -target amd64 bpf "$(pwd)/trace.c" -- -I"$(pwd)"
/home/gaz358/myprog/bpfgo/trace.c:711:18: warning: incompatible pointer types initializing 'struct sock *' with an expression of type 'struct bpf_sock *' [-Wincompatible-pointer-types]
  711 |     struct sock *sk=ctx->sk;
      |                  ^  ~~~~~~~
/home/gaz358/myprog/bpfgo/trace.c:713:11: error: incompatible pointer to integer conversion initializing '__u32' (aka 'unsigned int') with an expression of type 'void (*)(struct sock *)' [-Wint-conversion]
  713 |     __u32 state=sk->sk_state_change;
      |           ^     ~~~~~~~~~~~~~~~~~~~
1 warning and 1 error generated.
Error: compile: exit status 1



