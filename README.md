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
#include <linux/bpf.h>

SEC("tracepoint/udp/udp_rcv")
int trace_udp_recv(struct __sk_buff *skb) {
    // Буфер для хранения данных из пакета
    unsigned char data[128];

    // Читаем первые 128 байт данных
    int len = bpf_skb_load_bytes(skb, 0, data, sizeof(data));
    if (len < sizeof(struct iphdr)) {
        return 0;  // Не хватает данных для чтения заголовка IP
    }

    struct iphdr *ip = (struct iphdr *)data;

    // Проверка протокола UDP
    if (ip->protocol == IPPROTO_UDP) {
        // Смещение до UDP заголовка
        struct udphdr *udp = (struct udphdr *)((char *)ip + (ip->ihl << 2));
        bpf_trace_printk("Captured incoming UDP packet: src_ip=%d.%d.%d.%d dst_ip=%d.%d.%d.%d src_port=%d dst_port=%d length=%d\n",
                         ((ip->saddr >> 0) & 0xFF), ((ip->saddr >> 8) & 0xFF),
                         ((ip->saddr >> 16) & 0xFF), ((ip->saddr >> 24) & 0xFF),
                         ((ip->daddr >> 0) & 0xFF), ((ip->daddr >> 8) & 0xFF),
                         ((ip->daddr >> 16) & 0xFF), ((ip->daddr >> 24) & 0xFF),
                         ntohs(udp->source), ntohs(udp->dest), skb->len);
    }
    return 0;
}

SEC("tracepoint/udp/udp_send")
int trace_udp_send(struct __sk_buff *skb) {
    // Буфер для хранения данных из пакета
    unsigned char data[128];

    // Читаем первые 128 байт данных
    int len = bpf_skb_load_bytes(skb, 0, data, sizeof(data));
    if (len < sizeof(struct iphdr)) {
        return 0;  // Не хватает данных для чтения заголовка IP
    }

    struct iphdr *ip = (struct iphdr *)data;

    // Проверка протокола UDP
    if (ip->protocol == IPPROTO_UDP) {
        // Смещение до UDP заголовка
        struct udphdr *udp = (struct udphdr *)((char *)ip + (ip->ihl << 2));
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


