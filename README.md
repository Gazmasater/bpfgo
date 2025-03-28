lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_syn(struct trace_event_raw_inet_sock_set_state *ctx) {



    // Проверяем, что это состояние TCP_SYN_SENT (начало соединения)
    if (ctx->newstate == TCP_SYN_SENT) {


__u32 srcip;
bpf_probe_read_kernel(&srcip, sizeof(srcip), ctx->saddr);
srcip = bpf_ntohl(srcip);

__u32 dstip;
bpf_probe_read_kernel(&dstip, sizeof(dstip), ctx->daddr);
dstip = bpf_ntohl(dstip);


__u16 sport;

sport = ctx->sport;

__u16 dport;
dport=ctx->dport;


bpf_printk("inet_sock_set_state srcip=%d.%d.%d.%d:%d   dstip=%d.%d.%d.%d:%d ",

    (srcip >> 24) & 0xff,
    (srcip >> 16) & 0xff,
    (srcip >> 8) & 0xff,
    (srcip) & 0xff,
    sport,

    (dstip >> 24) & 0xff,
    (dstip >> 16) & 0xff,
    (dstip >> 8) & 0xff,
    (dstip) & 0xff,
    dport


);



    }

    return 0;
}


