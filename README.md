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
#include <linux/in.h>
#include <linux/ptrace.h>
#include <linux/inet.h>
#include <linux/types.h>
#include <linux/bpf_common.h>
#include <linux/bpf_verifier.h>
#include <linux/netdevice.h>

#include <bcc/proto.h>

#define DNS_PORT 53

// Эта секция будет отвечать за вставку программы в BPF
SEC("filter_dns")
int filter_dns(struct __sk_buff *skb) {
    struct ethhdr *eth;
    struct iphdr *ip;
    struct udphdr *udp;
    unsigned short sport, dport;
    char src_ip[16], dst_ip[16];

    // Извлечение Ethernet заголовка
    eth = bpf_hdr_pointer(skb, 0);
    ip = (struct iphdr *)(eth + 1);
    
    // Проверка, что пакет — это UDP, и мы работаем с IPv4
    if (ip->protocol != IPPROTO_UDP) {
        return 0; // Пропускаем все не-UDP пакеты
    }

    udp = (struct udphdr *)((char *)ip + ip->ihl * 4);
    sport = ntohs(udp->source);
    dport = ntohs(udp->dest);

    // Если это DNS пакет (порт 53)
    if (sport == DNS_PORT || dport == DNS_PORT) {
        // Получаем исходный и целевой IP-адреса
        bpf_probe_read_kernel(&src_ip, sizeof(src_ip), &ip->saddr);
        bpf_probe_read_kernel(&dst_ip, sizeof(dst_ip), &ip->daddr);

        // Печатаем информацию в формате, схожем с выводом tcpdump
        bpf_trace_printk("DNS Request/Response: %s > %s %u -> %u\n", src_ip, dst_ip, sport, dport);
    }

    return 0;
}

// Эта секция будет отвечать за настройку программы
char _license[] SEC("license") = "GPL";



