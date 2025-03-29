lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


#define TCP_ESTABLISHED 1
#define TCP_SYN_SENT 2

SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_est(struct trace_event_raw_inet_sock_set_state *ctx) {


    __u32 pid_tcp = bpf_get_current_pid_tgid() >> 32;

    __u32 srcip;
    bpf_probe_read_kernel(&srcip, sizeof(srcip), ctx->saddr);
    srcip = bpf_ntohl(srcip);
    
    __u32 dstip;
    bpf_probe_read_kernel(&dstip, sizeof(dstip), ctx->daddr);
    dstip = bpf_ntohl(dstip);
       
    __u16 sport=0;
    
    sport=ctx->sport;
       
    __u16 dport;
    dport=ctx->dport;
    
    if (ctx->newstate == TCP_ESTABLISHED||ctx->newstate == TCP_SYN_SENT||ctx->newstate==TCP_LISTEN) {

bpf_printk("inet_sock_set_state PID=%d srcip=%d.%d.%d.%d:%d   dstip=%d.%d.%d.%d:%d PROTO=%d ",
    
    pid_tcp,
    (srcip >> 24) & 0xff,
    (srcip >> 16) & 0xff,
    (srcip >> 8) & 0xff,
    (srcip) & 0xff,
    sport,

    (dstip >> 24) & 0xff,
    (dstip >> 16) & 0xff,
    (dstip >> 8) & 0xff,
    (dstip) & 0xff,
    dport,
    ctx->protocol


);

struct trace_info info = {};

info.src_ip=srcip;
info.sport=sport;
info.dst_ip=dstip;
info.dport=dport;
info.sysexit=6;
info.proto=ctx->protocol;
info.pid=pid_tcp;
bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));





    }

    return 0;
}

