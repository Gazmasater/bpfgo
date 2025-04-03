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


__u16 *srcIP6 = (__u16 *)ctx->local_ip6;
        __u16 *dstIP6 = (__u16 *)ctx->remote_ip6;
        __u32 srcPort = ctx->local_port;
        __u16 dstPort = bpf_ntohs(ctx->remote_port);





struct event_t {
    __u16 srcIP6[8];  // IPv6-адрес состоит из 8 блоков по 16 бит
    __u16 dstIP6[8];
    __u32 srcPort;
    __u16 dstPort;
};

bpf_probe_read_kernel(event->srcIP6, sizeof(event->srcIP6), ctx->local_ip6);
bpf_probe_read_kernel(event->dstIP6, sizeof(event->dstIP6), ctx->remote_ip6);

u32 ip = bpf_ntohl(addr_in6.sin6_addr.in6_u);


__u32 ip6[4];

ip6[0] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.s6_addr[0]);
ip6[1] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.s6_addr[4]);
ip6[2] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.s6_addr[8]);
ip6[3] = bpf_ntohl(*(__u32 *)&addr_in6.sin6_addr.s6_addr[12]);









