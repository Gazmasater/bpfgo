lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <linux/in.h>

struct ip_port_key {
    __u32 ip;
    __u16 port;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct ip_port_key);
    __type(value, __u32);
} sk_lookup_map SEC(".maps");

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <linux/in.h>

struct ip_port_key {
    __u32 ip;
    __u32 port;
};

struct ip_port_value {
    __u32 ip;
    __u16 port;
    __u32 protocol;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct ip_port_key);
    __type(value, struct ip_port_value);
} sk_lookup_map SEC(".maps");

SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    __u32 proto = ctx->protocol;
    __u32 dstIP = bpf_ntohl(ctx->local_ip4);
    __u32 srcIP = bpf_ntohl(ctx->remote_ip4);
    __u32 dstPort = ctx->local_port;
    __u16 srcPort = bpf_ntohs(ctx->remote_port);

    struct ip_port_key key = {
        .ip = dstIP,
        .port = dstPort,
    };

    struct ip_port_value value = {
        .ip =srcIP,
        .port = srcPort,
        .protocol = proto,
    };

    bpf_map_update_elem(&sk_lookup_map, &key, &value, BPF_ANY);

    bpf_printk("sk_lookup src=%d.%d.%d.%d:%d dst=%d.%d.%d.%d:%d protocol=%d\n", 
        (srcIP >> 24) & 0xff, (srcIP >> 16) & 0xff, (srcIP >> 8) & 0xff, srcIP & 0xff, srcPort,
        (dstIP >> 24) & 0xff, (dstIP >> 16) & 0xff, (dstIP >> 8) & 0xff, dstIP & 0xff, dstPort,
        proto);
    
    return SK_PASS;
}

char _license[] SEC("license") = "GPL";



        struct ip_port_key key = {
            .ip = ip,
            .port = port,
        };

        struct ip_port_value *value = bpf_map_lookup_elem(&sk_lookup_map, &key);
        if (value) {
            bpf_printk("trace_connect_exit: src=%d.%d.%d.%d:%d protocol=%d\n", 
                (value->ip >> 24) & 0xff, (value->ip >> 16) & 0xff, (value->ip >> 8) & 0xff, value->ip & 0xff, value->port,
                value->protocol);
        }



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
2025/03/22 20:28:05 failed to load bpf objects: field TraceConnectExit: program trace_connect_exit: load program: permission denied: invalid access to map value, value_size=4 off=4 size=2: R0 min value is outside of the allowed memory range (102 line(s) omitted)
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 


