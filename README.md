#define TC_ACT_OK 0
#define IPPROTO_UDP 17


#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>

SEC("tc")
int udp_monitor(struct __sk_buff *skb) {
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    // Проверка на размер ethernet header
    if (data + sizeof(struct ethhdr) > data_end)
        return TC_ACT_OK;

    struct ethhdr *eth = data;

    // Только IPv4
    if (eth->h_proto != __bpf_htons(ETH_P_IP))
        return TC_ACT_OK;

    // Проверка на размер IP header
    struct iphdr *ip = data + sizeof(struct ethhdr);
    if ((void *)ip + sizeof(struct iphdr) > data_end)
        return TC_ACT_OK;

    // Только UDP
    if (ip->protocol != IPPROTO_UDP)
        return TC_ACT_OK;

    int ip_header_len = ip->ihl * 4;
    struct udphdr *udp = data + sizeof(struct ethhdr) + ip_header_len;
    if ((void *)udp + sizeof(struct udphdr) > data_end)
        return TC_ACT_OK;

    // Получаем адреса и порты
    __u32 src_ip = ip->saddr;
    __u32 dst_ip = ip->daddr;
    __u16 src_port = __bpf_ntohs(udp->source);
    __u16 dst_port = __bpf_ntohs(udp->dest);

    bpf_printk("UDP: %x:%d -> %x:%d\n", src_ip, src_port, dst_ip, dst_port);

    return TC_ACT_OK;
}

