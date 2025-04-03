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


package main

import (
	"bpfgo/pkg"
	"errors"
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

// Глобальные объекты BPF
var objs bpfObjects

var eventChan_sport = make(chan int, 1)
var eventChan_pid = make(chan int, 1)

var mu sync.Mutex
var xxx, xxx_pid int

var proto, srchost, dsthost string

type bpfTraceInfo struct {
	Pid    uint32
	Family uint16
	Proto  uint8
	State  uint8
	Sysexit uint8
	Sport  uint16
	Dport  uint16
	SrcIp  uint32 // IPv4
	DstIp  uint32 // IPv4
	SrcIp6 [16]byte // IPv6
	DstIp6 [16]byte // IPv6
	Comm   [16]byte
}

func init() {
	// Снимаем ограничение на память
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit for process: %v", err)
	}

	// Загружаем eBPF-объекты
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

func main() {
	defer objs.Close() // Закроем объекты при выходе

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", netns.Fd())

	SEnter, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendto: %s", err)
	}
	defer SEnter.Close()

	SExit, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.TraceSendtoExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_sendto: %s", err)
	}
	defer SExit.Close()

	REnter, err := link.Tracepoint("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_recvfrom: %s", err)
	}
	defer REnter.Close()

	RExit, err := link.Tracepoint("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_recvfrom: %s", err)
	}
	defer RExit.Close()

	InetSock, err := link.Tracepoint("sock", "inet_sock_set_state", objs.TraceTcpEst, nil)
	if err != nil {
		log.Fatalf("opening tracepoint inet_sock_set_state: %s", err)
	}
	defer InetSock.Close()

	skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("failed to attach sk_lookup program: %v", err)
	}
	defer skLookupLink.Close()

	const buffLen = 4096
	rd, err := perf.NewReader(objs.TraceEvents, buffLen)
	if err != nil {
		log.Fatalf("failed to create perf reader: %s", err)
	}
	defer rd.Close()

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		for {
			record := new(perf.Record)
			err := rd.ReadInto(record)
			if err != nil {
				if errors.Is(err, os.ErrDeadlineExceeded) {
					continue
				}
				log.Printf("error reading from perf reader: %v", err)
				return
			}

			if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				log.Println("invalid event size")
				continue
			}

			event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

			var srcAddr, dstAddr string
			if event.Family == 2 { // IPv4
				srcIP := net.IPv4(
					byte(event.SrcIp>>24),
					byte(event.SrcIp>>16),
					byte(event.SrcIp>>8),
					byte(event.SrcIp),
				)
				dstIP := net.IPv4(
					byte(event.DstIp>>24),
					byte(event.DstIp>>16),
					byte(event.DstIp>>8),
					byte(event.DstIp),
				)

				srcAddr = fmt.Sprintf("[%s]:%d", srcIP.String(), event.Sport)
				dstAddr = fmt.Sprintf("[%s]:%d", dstIP.String(), event.Dport)

			} else if event.Family == 10 { // IPv6
				srcIP := net.IP(event.SrcIp6[:])
				dstIP := net.IP(event.DstIp6[:])

				srcAddr = fmt.Sprintf("[%s]:%d", srcIP.String(), event.Sport)
				dstAddr = fmt.Sprintf("[%s]:%d", dstIP.String(), event.Dport)
			}

			if event.Sysexit == 1 {
				fmt.Printf("STATE=1 PID=%d dstIP=%s FAMILY=%d \n", event.Pid, dstAddr, event.Family)
			}

			if event.Sysexit == 2 {
				fmt.Printf("STATE=2 PID=%d srcIP=%s \n", event.Pid, srcAddr)
			}

			if event.Sysexit == 3 {
				fmt.Printf("STATE=3 srcIP=%s dstIP=%s PROTO=%d FAMILY=%d\n", srcAddr, dstAddr, event.Proto, event.Family)
			}

			if event.Sysexit == 5 {
				if event.State == 1 {
					fmt.Printf("PID=%d %s <- %s \n", event.Pid, dstAddr, srcAddr)
				}
				if event.State == 2 {
					fmt.Printf("State 2 detected for PID=%d\n", event.Pid)
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}












