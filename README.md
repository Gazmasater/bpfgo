lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg

SEC("tracepoint/sock/inet_sock_set_state")
int trace_tcp_est(struct trace_event_raw_inet_sock_set_state *ctx) {


    if (ctx->newstate == TCP_ESTABLISHED) {

    __u32 pid2 = bpf_get_current_pid_tgid() >> 32;
        


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


bpf_printk("inet_sock_ESTABLISHED  PID2=%d srcip=%d.%d.%d.%d:%d   dstip=%d.%d.%d.%d:%d PROTO=%d ",
    
    pid2,
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
    ctx->protocol);

    } 

    if (ctx->newstate == TCP_SYN_SENT){

        __u32 pid1 = bpf_get_current_pid_tgid() >> 32;
        


        __u32 srcip1;
        bpf_probe_read_kernel(&srcip1, sizeof(srcip1), ctx->saddr);
        srcip1 = bpf_ntohl(srcip1);
    
        __u32 dstip1;
        bpf_probe_read_kernel(&dstip1, sizeof(dstip1), ctx->daddr);
        dstip1 = bpf_ntohl(dstip1);
    
    
        __u16 sport1=0;
    
        sport1=ctx->sport;
    
    
        __u16 dport1;
        dport1=ctx->dport;
    
    
    bpf_printk("inet_sock_ESTABLISHED  PID1=%d srcip=%d.%d.%d.%d:%d   dstip=%d.%d.%d.%d:%d PROTO=%d ",
        
        pid1,
        (srcip1 >> 24) & 0xff,
        (srcip1 >> 16) & 0xff,
        (srcip1 >> 8) & 0xff,
        (srcip1) & 0xff,
        sport1,
    
        (dstip1 >> 24) & 0xff,
        (dstip1 >> 16) & 0xff,
        (dstip1 >> 8) & 0xff,
        (dstip1) & 0xff,
        dport1,
        ctx->protocol);
    
        
    } 

    return 0;

    }
