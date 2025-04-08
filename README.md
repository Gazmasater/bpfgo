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


STATE=1 IPv6 PID=945 IPv6=ff020000:0:0:10002:547 NAME=
STATE=3 DST IPv6=fe800000:0:e7329ff:feb7d6e8
STATE=3 SRC IPv6=fe800000:0:d6b29200:15bba0e8
STATE=3 SPORT=546  DPORT=52645 PROTO=17
STATE=12 IPv6 PID=945 srcIPv6=fe800000:80000000:0:0:52645
STATE=1-sendto
STATE=3-lookup
STATE=12-recvmsg

package main

import (
	"fmt"
	"net"
)

func main() {
	raw := "ff02:0000:0000:0000:0001:0000:0000:0002"
	ip := net.ParseIP(raw)
	if ip == nil {
		fmt.Println("Invalid IP")
		return
	}
	fmt.Println(ip.String()) // Выведет: ff02::1:2
}


package main

import (
	"fmt"
	"net"
)

func main() {
	// Пример данных из event.DstIP6
	eventDstIP6 := [4]uint32{
		0xff020000, // часть 1
		0x00000000, // часть 2
		0x00010000, // часть 3
		0x00000002, // часть 4
	}

	// Преобразуем данные в 16-байтовый массив
	ipBytes := []byte{
		byte(eventDstIP6[0] >> 24), byte(eventDstIP6[0] >> 16), byte(eventDstIP6[0] >> 8), byte(eventDstIP6[0]),
		byte(eventDstIP6[1] >> 24), byte(eventDstIP6[1] >> 16), byte(eventDstIP6[1] >> 8), byte(eventDstIP6[1]),
		byte(eventDstIP6[2] >> 24), byte(eventDstIP6[2] >> 16), byte(eventDstIP6[2] >> 8), byte(eventDstIP6[2]),
		byte(eventDstIP6[3] >> 24), byte(eventDstIP6[3] >> 16), byte(eventDstIP6[3] >> 8), byte(eventDstIP6[3]),
	}

	// Создаем объект net.IP из массива байтов
	ip := net.IP(ipBytes)

	// Получаем строковое представление IPv6-адреса
	fmt.Println("IPv6:", ip.String()) // Выведет: ff02::1:2
}








