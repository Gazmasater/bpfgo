#define ETH_P_IP    0x0800  // Интернет-протокол (IPv4)
ETH_P_IPV6 = 0x86DD


#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>

SEC("tracepoint/net/netif_receive_skb")
int trace_udp(struct trace_event_raw_net_dev_template *ctx)
{
    struct ethhdr eth = {};
    struct iphdr ip = {};
    struct udphdr udp = {};
    int ret;

    // Сначала Ethernet-заголовок
    ret = bpf_skb_load_bytes(ctx->skbaddr, 0, &eth, sizeof(eth));
    if (ret < 0)
        return 0;

    if (eth.h_proto != __bpf_htons(ETH_P_IP))
        return 0;

    // IP-заголовок
    ret = bpf_skb_load_bytes(ctx->skbaddr, sizeof(eth), &ip, sizeof(ip));
    if (ret < 0)
        return 0;

    if (ip.protocol != IPPROTO_UDP)
        return 0;

    // UDP-заголовок (ip.ihl — длина IP заголовка в 32-битных словах)
    int ip_header_len = ip.ihl * 4;
    ret = bpf_skb_load_bytes(ctx->skbaddr, sizeof(eth) + ip_header_len, &udp, sizeof(udp));
    if (ret < 0)
        return 0;

    // Готово! Вот твои данные:
    __u32 src_ip = ip.saddr;
    __u32 dst_ip = ip.daddr;
    __u16 src_port = __bpf_ntohs(udp.source);
    __u16 dst_port = __bpf_ntohs(udp.dest);

    // ...дальнейшая логика

    return 0;
}

char _license[] SEC("license") = "GPL";

