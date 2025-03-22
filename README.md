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




SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) return 0;

    if (ret < 0) {
        bpf_printk("sys_exit_connect failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct sockaddr **addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        return 0;
    }

   

    struct sockaddr addr = {};
    bpf_probe_read_user(&addr, sizeof(addr), *addr_ptr);  

    if (addr.sa_family == AF_INET) {
        struct sockaddr_in addr_in = {};

        bpf_probe_read_user(&addr_in, sizeof(addr_in), *addr_ptr);

        u32 ip = bpf_ntohl(addr_in.sin_addr.s_addr);

        u16 port = bpf_ntohs(addr_in.sin_port);

        struct trace_info info = {};
        info.pid = pid;
        __builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

        info.pid=conn_info->pid;
        info.dst_ip=ip;
        info.dport = port;
        
        bpf_printk("sys_exit_connect FAMILY=%d PORT=%d Comm=%s",addr.sa_family,port,conn_info->comm);


       bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
       
    }

    bpf_map_delete_elem(&addr_map, &pid);  
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;

}



