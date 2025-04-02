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

Установка wireshark

sudo apt update
sudo apt install wireshark
sudo dpkg-reconfigure wireshark-common //без sudo
sudo usermod -aG wireshark $USER //добавить пользователя
wireshark //запуск

echo "Hello, UDP!" | nc -u -w1 34.117.188.166 443
echo "Hello, UDP!" | socat - UDP:34.117.188.166:443


Internet Protocol Version 6, Src: fe80::e73:29ff:feb7:d6e8, Dst: fe80::d6b2:9200:15bb:a0e8


SEC("sk_lookup")
int look_up(struct bpf_sk_lookup *ctx) {
    struct trace_info info = {};
    __u32 proto = ctx->protocol;

    if (ctx->family == AF_INET) {
        __u32 srcIP = bpf_ntohl(ctx->local_ip4);
        __u32 dstIP = bpf_ntohl(ctx->remote_ip4);
        __u32 srcPort = ctx->local_port;
        __u16 dstPort = bpf_ntohs(ctx->remote_port);

        info.src_ip = srcIP;
        info.sport = srcPort;
        info.dst_ip = dstIP;
        info.dport = dstPort;
        info.sysexit = 3;
        info.proto = proto;

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        bpf_printk("IPv4 lookup src=%d.%d.%d.%d:%d dst=%d.%d.%d.%d:%d protocol=%d FAMILY=%d \n", 
            (srcIP >> 24) & 0xff, (srcIP >> 16) & 0xff, (srcIP >> 8) & 0xff, srcIP & 0xff,
            srcPort,
            (dstIP >> 24) & 0xff, (dstIP >> 16) & 0xff, (dstIP >> 8) & 0xff, dstIP & 0xff,
            dstPort,
            proto, ctx->family);
    } else if (ctx->family == AF_INET6) {
        __u16 *srcIP6 = (__u16 *)ctx->local_ip6;
        __u16 *dstIP6 = (__u16 *)ctx->remote_ip6;
        __u32 srcPort = ctx->local_port;
        __u16 dstPort = bpf_ntohs(ctx->remote_port);

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

        bpf_printk("IPv6 lookup src=%x:%x::%x:%x:%x:%x:%d "
            "dst=%x:%x::%x:%x:%x:%x:%d protocol=%d FAMILY=%d \n", 
            bpf_ntohs(srcIP6[0]), bpf_ntohs(srcIP6[1]), bpf_ntohs(srcIP6[4]), bpf_ntohs(srcIP6[5]),
            bpf_ntohs(srcIP6[6]), bpf_ntohs(srcIP6[7]), srcPort,
            bpf_ntohs(dstIP6[0]), bpf_ntohs(dstIP6[1]), bpf_ntohs(dstIP6[4]), bpf_ntohs(dstIP6[5]),
            bpf_ntohs(dstIP6[6]), bpf_ntohs(dstIP6[7]), dstPort,
            proto, ctx->family);
    }
    return SK_PASS;
}

[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "109",
	"severity": 8,
	"message": "expression preceding parentheses of apparent call must have (pointer-to-) function type",
	"source": "C/C++",
	"startLineNumber": 375,
	"startColumn": 9,
	"endLineNumber": 375,
	"endColumn": 19
}]











