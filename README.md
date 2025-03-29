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
    __u32 srcip, dstip;
    __u16 sport, dport;
    __u32 pid;

    bpf_probe_read_kernel(&srcip, sizeof(srcip), ctx->saddr);
    srcip = bpf_ntohl(srcip);

    bpf_probe_read_kernel(&dstip, sizeof(dstip), ctx->daddr);
    dstip = bpf_ntohl(dstip);

    sport = ctx->sport;
    dport = ctx->dport;
    pid = bpf_get_current_pid_tgid() >> 32;

    if (ctx->newstate == TCP_ESTABLISHED || ctx->newstate == TCP_SYN_SENT) {
        bpf_printk("inet_sock_STATE_CHANGE PID=%d srcip=%d.%d.%d.%d:%d dstip=%d.%d.%d.%d:%d PROTO=%d STATE=%d",
            pid,
            (srcip >> 24) & 0xff, (srcip >> 16) & 0xff, (srcip >> 8) & 0xff, (srcip) & 0xff,
            sport,
            (dstip >> 24) & 0xff, (dstip >> 16) & 0xff, (dstip >> 8) & 0xff, (dstip) & 0xff,
            dport,
            ctx->protocol,
            ctx->newstate);
    }

    return 0;
}



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=6"
PID=2887 srcAddr=192.168.1.71:0 -> dstAddr=140.82.121.3:443  SYSCALL=6
PID=530 srcAddr=192.168.1.71:34262 <- dstAddr=140.82.121.3:443  SYSCALL=6
PID=2887 srcAddr=192.168.1.71:0 -> dstAddr=140.82.114.21:443  SYSCALL=6
PID=2887 srcAddr=192.168.1.71:0 -> dstAddr=140.82.112.25:443  SYSCALL=6
PID=533 srcAddr=192.168.1.71:45948 <- dstAddr=140.82.114.21:443  SYSCALL=6
PID=533 srcAddr=192.168.1.71:38784 <- dstAddr=140.82.112.25:443  SYSCALL=6
PID=2887 srcAddr=192.168.1.71:0 -> dstAddr=140.82.121.5:443  SYSCALL=6
PID=527 srcAddr=192.168.1.71:51974 <- dstAddr=140.82.121.5:443  SYSCALL=6
PID=991 srcAddr=192.168.1.71:0 -> dstAddr=91.189.91.97:80  SYSCALL=6
PID=530 srcAddr=192.168.1.71:55828 <- dstAddr=91.189.91.97:80  SYSCALL=6
PID=18221 srcAddr=192.168.1.71:0 -> dstAddr=151.101.246.49:443  SYSCALL=6
PID=529 srcAddr=192.168.1.71:38630 <- dstAddr=151.101.246.49:443  SYSCALL=6
PID=4149 srcAddr=192.168.1.71:0 -> dstAddr=13.107.246.45:443  SYSCALL=6
PID=529 srcAddr=192.168.1.71:55512 <- dstAddr=13.107.246.45:443  SYSCALL=6
PID=991 srcAddr=192.168.1.71:0 -> dstAddr=185.125.190.48:80  SYSCALL=6
PID=533 srcAddr=192.168.1.71:49914 <- dstAddr=185.125.190.48:80  SYSCALL=6
PID=4308 srcAddr=192.168.1.71:0 -> dstAddr=13.107.5.93:443  SYSCALL=6
PID=529 srcAddr=192.168.1.71:44174 <- dstAddr=13.107.5.93:443  SYSCALL=6
PID=4290 srcAddr=192.168.1.71:0 -> dstAddr=13.107.5.93:443  SYSCALL=6
PID=528 srcAddr=192.168.1.71:44176 <- dstAddr=13.107.5.93:443  SYSCALL=6
PID=4149 srcAddr=192.168.1.71:0 -> dstAddr=13.107.246.45:443  SYSCALL=6
PID=533 srcAddr=192.168.1.71:60484 <- dstAddr=13.107.246.45:443  SYSCALL=6
PID=4308 srcAddr=192.168.1.71:0 -> dstAddr=13.107.5.93:443  SYSCALL=6
PID=526 srcAddr=192.168.1.71:44184 <- dstAddr=13.107.5.93:443  SYSCALL=6
PID=4149 srcAddr=192.168.1.71:0 -> dstAddr=13.107.246.45:443  SYSCALL=6
PID=530 srcAddr=192.168.1.71:60368 <- dstAddr=13.107.246.45:443  SYSCALL=6
PID=991 srcAddr=192.168.1.71:0 -> dstAddr=185.125.190.96:80  SYSCALL=6
PID=528 srcAddr=192.168.1.71:54916 <- dstAddr=185.125.190.96:80  SYSCALL=6
