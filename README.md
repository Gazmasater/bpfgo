lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


#include <linux/ptrace.h>
#include <linux/net.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/bpf.h>
#include <linux/if_packet.h>
#include <linux/filter.h>

struct dns_query {
    u32 pid;
    u32 tgid;
    char comm[16];
    u8 data[512];
};

BPF_PERF_OUTPUT(dns_events);

SEC("tracepoint/net/net_dev_queue")
int trace_dns(struct __sk_buff *skb) {
    // Анализируйте пакет и извлекайте DNS-запросы/ответы
    // Пример: проверка на порт 53 (DNS)
    struct ethhdr *eth = bpf_hdr_pointer(skb, 0);
    if (eth->h_proto == htons(ETH_P_IP)) {
        struct iphdr *ip = bpf_hdr_pointer(skb, sizeof(*eth));
        if (ip->protocol == IPPROTO_UDP) {
            struct udphdr *udp = bpf_hdr_pointer(skb, sizeof(*eth) + sizeof(*ip));
            if (bpf_ntohs(udp->dest) == 53 || bpf_ntohs(udp->source) == 53) {
                // Извлекаем данные DNS из пакета
                struct dns_query query = {};
                query.pid = bpf_get_current_pid_tgid() >> 32;
                query.tgid = bpf_get_current_pid_tgid();
                bpf_get_current_comm(&query.comm, sizeof(query.comm));
                // Копируем данные DNS из пакета
                u32 data_len = skb->len - (sizeof(*eth) + sizeof(*ip) + sizeof(*udp));
                if (data_len > sizeof(query.data)) {
                    data_len = sizeof(query.data);
                }
                bpf_skb_load_bytes(skb, sizeof(*eth) + sizeof(*ip) + sizeof(*udp), &query.data, data_len);
                dns_events.perf_submit(skb, &query, sizeof(query));
            }
        }
    }
    return 0;
}

char _license[] SEC("license") = "GPL";










