lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i wlp0s20f3 udp -nn

sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


#include <linux/inet.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/tracepoint.h>

SEC("tracepoint/udp/udp_rcv")
int trace_udp_recv(struct __sk_buff *skb) {
    struct iphdr *ip = bpf_hdr_pointer(skb, 0);
    if (ip == NULL) {
        return 0;
    }

    struct udphdr *udp = (struct udphdr *)((char *)ip + (ip->ihl << 2));
    if (ip->protocol == IPPROTO_UDP) {
        bpf_trace_printk("Captured incoming UDP packet: src_ip=%d.%d.%d.%d dst_ip=%d.%d.%d.%d src_port=%d dst_port=%d length=%d\n",
                         ((ip->saddr >> 0) & 0xFF), ((ip->saddr >> 8) & 0xFF),
                         ((ip->saddr >> 16) & 0xFF), ((ip->saddr >> 24) & 0xFF),
                         ((ip->daddr >> 0) & 0xFF), ((ip->daddr >> 8) & 0xFF),
                         ((ip->daddr >> 16) & 0xFF), ((ip->daddr >> 24) & 0xFF),
                         ntohs(udp->source), ntohs(udp->dest), skb->len);
    }
    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir "$(pwd)" -tags linux -type trace_info -go-package=main -target amd64 bpf "$(pwd)/trace.c" -- -I"$(pwd)"
/home/gaz358/myprog/bpfgo/trace.c:474:24: error: call to undeclared function 'bpf_hdr_pointer'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  474 |     struct iphdr *ip = bpf_hdr_pointer(skb, 0);
      |                        ^
/home/gaz358/myprog/bpfgo/trace.c:474:19: error: incompatible integer to pointer conversion initializing 'struct iphdr *' with an expression of type 'int' [-Wint-conversion]
  474 |     struct iphdr *ip = bpf_hdr_pointer(skb, 0);
      |                   ^    ~~~~~~~~~~~~~~~~~~~~~~~
2 errors generated.
Error: compile: exit status 1
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

SEC("tracepoint/udp/udp_send")
int trace_udp_send(struct __sk_buff *skb) {
    struct iphdr *ip = bpf_hdr_pointer(skb, 0);
    if (ip == NULL) {
        return 0;
    }

    struct udphdr *udp = (struct udphdr *)((char *)ip + (ip->ihl << 2));
    if (ip->protocol == IPPROTO_UDP) {
        bpf_trace_printk("Captured outgoing UDP packet: src_ip=%d.%d.%d.%d dst_ip=%d.%d.%d.%d src_port=%d dst_port=%d length=%d\n",
                         ((ip->saddr >> 0) & 0xFF), ((ip->saddr >> 8) & 0xFF),
                         ((ip->saddr >> 16) & 0xFF), ((ip->saddr >> 24) & 0xFF),
                         ((ip->daddr >> 0) & 0xFF), ((ip->daddr >> 8) & 0xFF),
                         ((ip->daddr >> 16) & 0xFF), ((ip->daddr >> 24) & 0xFF),
                         ntohs(udp->source), ntohs(udp->dest), skb->len);
    }
    return 0;
}

char _license[] SEC("license") = "GPL";
