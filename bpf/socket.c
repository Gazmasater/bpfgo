//go:build ignore

#include "vmlinux.h"


#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

#define ETH_P_IP 0x0800
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define TC_ACT_OK 0
#define ETH_P_IPV6 10






char LICENSE[] SEC("license") = "Dual BSD/GPL";

SEC("socket")
int sock_filter_prog(struct __sk_buff *skb) {
    struct ethhdr *eth = bpf_hdr_pointer(skb, 0, sizeof(*eth));
    if (!eth) return 0;

    struct iphdr *ip = (void *)(eth + 1);
    if ((void *)(ip + 1) > (void *)(long)skb->data_end) return 0;

    if (ip->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp = (void *)(ip + 1);
        if ((void *)(tcp + 1) > (void *)(long)skb->data_end) return 0;

        bpf_printk("SOCK_FILTER: TCP src_port=%d, dest_port=%d\n",
                   bpf_ntohs(tcp->source), bpf_ntohs(tcp->dest));
    }
    return 1; // Пропускаем пакет дальше
}