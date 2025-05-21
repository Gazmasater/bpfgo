#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>

SEC("tracepoint/net/netif_receive_skb")
int on_receive_skb(struct trace_event_raw_net_dev_template *ctx) {
    // Получаем указатель на sk_buff
    struct sk_buff *skb = (struct sk_buff *)ctx->skbaddr;

    // Получаем указатель на начало данных (skb->data)
    void *data = (void *)skb->data;
    void *data_end = (void *)skb->data_end;

    // Проверяем, что влезает Ethernet-заголовок
    if (data + sizeof(struct ethhdr) > data_end)
        return 0;

    struct ethhdr *eth = data;

    // Пропускаем только IPv4-пакеты
    if (eth->h_proto != bpf_htons(ETH_P_IP))
        return 0;

    // Смещаемся на IP-заголовок
    struct iphdr *ip = data + sizeof(struct ethhdr);
    if ((void*)ip + sizeof(struct iphdr) > data_end)
        return 0;

    // Пропускаем только UDP
    if (ip->protocol != IPPROTO_UDP)
        return 0;

    // Смещаемся на UDP-заголовок
    int ip_header_len = ip->ihl * 4;
    struct udphdr *udp = (void*)ip + ip_header_len;
    if ((void*)udp + sizeof(struct udphdr) > data_end)
        return 0;

    // Получаем IP и порты
    __u32 src_ip = ip->saddr;
    __u32 dst_ip = ip->daddr;
    __u16 src_port = bpf_ntohs(udp->source);
    __u16 dst_port = bpf_ntohs(udp->dest);

    // ... логика, например, отправка в perf buffer

    return 0;
}
char _license[] SEC("license") = "GPL";
