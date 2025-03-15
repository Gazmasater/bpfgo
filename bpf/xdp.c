//go:build ignore

#include "vmlinux.h"


#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

#define ETH_P_IP 0x0800
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17


char LICENSE[] SEC("license") = "Dual BSD/GPL";



SEC("xdp")
int xdp_drop_non_tcp_udp(struct xdp_md *ctx) {
    void *data_end = (void *)(unsigned long)ctx->data_end;
    void *data = (void *)(unsigned long)ctx->data;

    // Получаем указатель на заголовок Ethernet
    struct ethhdr *eth = data;
    if ((void *)eth + sizeof(*eth) > data_end)
        return XDP_PASS;  // Если пакет меньше Ethernet заголовка, пропускаем его

    // Проверяем, содержит ли Ethernet-фрейм IP-пакет
    if (eth->h_proto != bpf_htons(ETH_P_IP))
        return XDP_PASS;  // Если не IP-пакет, пропускаем его

    // Получаем указатель на заголовок IP
    struct iphdr *ip = data + sizeof(*eth);
    if ((void *)ip + sizeof(*ip) > data_end)
        return XDP_PASS;  // Если IP-заголовок выходит за пределы пакета, пропускаем его

    // Печатаем IP-адреса источника и назначения
    // bpf_printk("Source IP: %d.%d.%d.%d, Dest IP: %d.%d.%d.%d\n",
    //                  (ip->saddr >> 0) & 0xFF, (ip->saddr >> 8) & 0xFF,
    //                  (ip->saddr >> 16) & 0xFF, (ip->saddr >> 24) & 0xFF,
    //                  (ip->daddr >> 0) & 0xFF, (ip->daddr >> 8) & 0xFF,
    //                  (ip->daddr )>> 16) & 0xFF, (ip->daddr >> 24) & 0xFF);


    // Проверяем, является ли протокол TCP или UDP
    if (ip->protocol == IPPROTO_TCP) {
        // Печатаем, что это TCP-пакет
        bpf_printk("Protocol: TCP=%d\n",ip->protocol);
        return XDP_PASS;
    }
    else if (ip->protocol == IPPROTO_UDP) {
        // Печатаем, что это UDP-пакет
        bpf_printk("Protocol: UDP=%d\n",ip->protocol);
        return XDP_PASS;
    }

    // Отбрасываем все остальные пакеты (не TCP и не UDP)
    return XDP_DROP;
}