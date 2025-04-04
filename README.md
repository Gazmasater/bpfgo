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


#define ETH_P_IPV6 0x86DD
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ipv6.h>
#include <linux/udp.h>
#include <bpf/bpf_helpers.h>

SEC("socket/udp")
int udp_filter(struct __sk_buff *skb) {
    void *data = (void *)(long)skb->data;
    void *data_end = (void *)(long)skb->data_end;

    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return 0;

    if (eth->h_proto != __bpf_constant_htons(ETH_P_IPV6))
        return 0;

    struct ipv6hdr *ip6h = (void *)(eth + 1);
    if ((void *)(ip6h + 1) > data_end)
        return 0;

    if (ip6h->nexthdr != IPPROTO_UDP)
        return 0;

    struct udphdr *udph = (void *)(ip6h + 1);
    if ((void *)(udph + 1) > data_end)
        return 0;

    bpf_printk("IPv6 UDP packet: sport=%d dport=%d\n", 
        bpf_ntohs(udph->source), bpf_ntohs(udph->dest));

    return 0;
}

char LICENSE[] SEC("license") = "GPL";


package main

import (
	"log"
	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
	"os"
)

func main() {
	// Повышаем лимит памяти для eBPF
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("remove memlock: %v", err)
	}

	// Загружаем BPF объект
	spec, err := ebpf.LoadCollectionSpec("udp_filter.o")
	if err != nil {
		log.Fatalf("failed to load spec: %v", err)
	}

	objs := struct {
		UdpFilter *ebpf.Program `ebpf:"udp_filter"`
	}{}
	if err := spec.LoadAndAssign(&objs, nil); err != nil {
		log.Fatalf("failed to load objs: %v", err)
	}

	// Создаем raw socket
	ifaceName := "eth0" // замени на свой интерфейс!
	iface, err := net.InterfaceByName(ifaceName)
	if err != nil {
		log.Fatalf("failed to get iface: %v", err)
	}

	// Привязываем eBPF-программу к сокету
	sock, err := link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  uint32(iface.Index),
		Program: objs.UdpFilter,
		Attach:  ebpf.AttachSkSKBStreamParser, // или другой AttachType
	})
	if err != nil {
		log.Fatalf("failed to attach: %v", err)
	}
	defer sock.Close()

	log.Println("eBPF UDP фильтр загружен. Читай dmesg или trace_pipe.")
	select {}
}


	skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("failed to attach sk_lookup program: %v", err)
	}
	defer skLookupLink.Close()

 	sock, err := link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  uint32(iface.Index),
		Program: objs.
		Attach:  ebpf.AttachSkSKBStreamParser,
	})
	if err != nil {
		log.Fatalf("failed to attach: %v", err)
	}
	defer sock.Close()


	sock, err := link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  uint32(iface.Index),
		Program: objs.UdpFilter,
		Attach:  ebpf.AttachSkSKBStreamParser,
	})


 

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "WrongAssignCount",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "WrongAssignCount"
		}
	},
	"severity": 8,
	"message": "assignment mismatch: 2 variables but link.RawAttachProgram returns 1 value",
	"source": "compiler",
	"startLineNumber": 55,
	"startColumn": 15,
	"endLineNumber": 59,
	"endColumn": 4
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "IncompatibleAssign",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "IncompatibleAssign"
		}
	},
	"severity": 8,
	"message": "cannot use uint32(iface.Index) (value of type uint32) as int value in struct literal",
	"source": "compiler",
	"startLineNumber": 56,
	"startColumn": 12,
	"endLineNumber": 56,
	"endColumn": 31
}]



