nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

SEC("xdp_prog")
int xdp_monitor(struct __sk_buff *skb) {
    // Указатель на Ethernet-заголовок
    struct ethhdr *eth = bpf_hdr_pointer(skb, 0);
    
    // Проверяем, что это IP-пакет
    if (eth->h_proto == htons(ETH_P_IP)) {
        struct iphdr *ip = (struct iphdr *)(eth + 1);

        // Если это TCP или UDP, то извлекаем нужную информацию
        if (ip->protocol == IPPROTO_TCP) {
            struct tcphdr *tcp = (struct tcphdr *)(ip + 1);

            // Локальный IP и порт
            __be32 local_ip = ip->saddr;
            __be16 local_port = tcp->source;

            // Удалённый IP и порт
            __be32 remote_ip = ip->daddr;
            __be16 remote_port = tcp->dest;

            // Логирование или другой механизм мониторинга
            bpf_printk("TCP Monitor: Local IP: %pI4, Local Port: %u, Remote IP: %pI4, Remote Port: %u\n",
                &local_ip, ntohs(local_port), &remote_ip, ntohs(remote_port));
        } 
        else if (ip->protocol == IPPROTO_UDP) {
            struct udphdr *udp = (struct udphdr *)(ip + 1);

            // Локальный IP и порт
            __be32 local_ip = ip->saddr;
            __be16 local_port = udp->source;

            // Удалённый IP и порт
            __be32 remote_ip = ip->daddr;
            __be16 remote_port = udp->dest;

            // Логирование или другой механизм мониторинга
            bpf_printk("UDP Monitor: Local IP: %pI4, Local Port: %u, Remote IP: %pI4, Remote Port: %u\n",
                &local_ip, ntohs(local_port), &remote_ip, ntohs(remote_port));
        }
    }
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";

/home/gaz358/myprog/bpfgo/trace.c:598:20: error: incompatible integer to pointer conversion initializing 'struct ethhdr *' with an expression of type 'int' [-Wint-conversion]
  598 |     struct ethhdr *eth = bpf_hdr_pointer(skb, 0);
      |                    ^     ~~~~~~~~~~~~~~~~~~~~~~~

