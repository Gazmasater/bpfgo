lsns -t net


sudo ip netns list
ls /sys/kernel/tracing/events
ls /sys/kernel/tracing/events/net/




bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i wlp0s20f3 udp -nn

sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ls /sys/kernel/tracing/events
alarmtimer        fs_dax          iwlwifi_msg    nvme            sof
amd_cpu           ftrace          iwlwifi_ucode  oom             sof_intel
asoc              gpio            jbd2           osnoise         spi
avc               gpu_scheduler   kmem           page_isolation  swiotlb
block             handshake       ksm            pagemap         sync_trace
bpf_test_run      hda             kvm            page_pool       syscalls
bpf_trace         hda_controller  kvmmmu         percpu          task
bridge            hda_intel       libata         power           tcp
cfg80211          header_event    lock           printk          thermal
cgroup            header_page     mac80211       pwm             thermal_power_allocator
clk               huge_memory     mac80211_msg   qdisc           thp
compaction        hwmon           maple_tree     qrtr            timer
context_tracking  hyperv          mce            ras             tlb
cpuhp             i2c             mctp           raw_syscalls    udp
cros_ec           i915            mdio           rcu             v4l2
csd               initcall        mei            regmap          vb2
dev               intel_iommu     migrate        regulator       vmalloc
devfreq           interconnect    mmap           resctrl         vmscan
devlink           iocost          mmap_lock      rpm             vsyscall
dma_fence         iomap           mmc            rseq            watchdog
drm               iommu           module         rtc             wbt
enable            io_uring        mptcp          rv              workqueue
error_report      ipi             msr            sched           writeback
exceptions        irq             napi           scsi            x86_fpu
ext4              irq_matrix      neigh          sd              xdp
fib               irq_vectors     net            signal          xe
fib6              iwlwifi         netlink        skb             xen
filelock          iwlwifi_data    nmi            smbus           xhci-hcd
filemap           iwlwifi_io      notifier       sock
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 


systemctl is-active systemd-resolved

sudo systemctl stop systemd-resolved
sudo systemctl start systemd-resolved

sudo systemctl disable systemd-resolved

ip route

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/socket.h>
#include <linux/in.h>

struct ip_port_key {
    __u32 dst_ip;
    __u16 dst_port;
};

struct ip_port_value {
    __u32 src_ip;
    __u16 src_port;
};

// Определение мапы
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct ip_port_key);  // Ключ (dst_ip:dst_port)
    __type(value, struct ip_port_value);  // Значение (src_ip:src_port)
} addrBind_map SEC(".maps");

SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    __u32 proto = ctx->protocol;
    __u32 srcIP = bpf_ntohl(ctx->local_ip4);
    __u32 dstIP = bpf_ntohl(ctx->remote_ip4);
    __u32 srcPort = ctx->local_port;
    __u16 dstPort = bpf_ntohs(ctx->remote_port);

    // Заполнение ключа (dst_ip, dst_port)
    struct ip_port_key key = {};
    key.dst_ip = dstIP;
    key.dst_port = dstPort;

    // Заполнение значения (src_ip, src_port)
    struct ip_port_value value = {};
    value.src_ip = srcIP;
    value.src_port = srcPort;

    // Сохранение в мапу
    bpf_map_update_elem(&addrBind_map, &key, &value, BPF_ANY);

    if ((((srcIP >> 24) & 0xff) != 127) && (ctx->protocol == 17)) {
        bpf_printk("lookup src=%d.%d.%d.%d:%d dst=%d.%d.%d.%d:%d protocol=%d FAMILY=%d \n", 
            (srcIP >> 24) & 0xff,
            (srcIP >> 16) & 0xff, 
            (srcIP >> 8) & 0xff, 
            srcIP & 0xff,
            srcPort,
            (dstIP >> 24) & 0xff, 
            (dstIP >> 16) & 0xff, 
            (dstIP >> 8) & 0xff,
            dstIP & 0xff,
            dstPort,
            proto,
            ctx->family        
        );
    }

    return SK_PASS;
}


