
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force




_______________________________________________________________________________________________



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

var objs bpfObjects

var (
	SmsgEnter    link.Link
	SmsgExit     link.Link
	SEnter       link.Link
	SExit        link.Link
	RmsgEnter    link.Link
	RmsgExit     link.Link
	REnter       link.Link
	RExit        link.Link
	InetSock     link.Link
	skLookupLink link.Link
)

var (
	eventChan_sport = make(chan int, 1)
	eventChan_pid   = make(chan int, 1)
	mu              sync.Mutex
)

var (
	xxx, xxx_pid int
	proto        string
	srchost      string
	dsthost      string
)

// —————————————————————————————————————————————————————
// Структуры теперь без указателей: value-типы вместо *Lookup, *Sendmsg, *Recvmsg
type Lookup struct {
	DstIP   net.IP
	DstPort int
	SrcIP   net.IP
	SrcPort int
	Proto   int
}

type Sendmsg struct {
	DstIP   net.IP
	DstPort int
	Pid     uint32
	Proto   int
	Comm    string
}

type Recvmsg struct {
	SrcIP   net.IP
	SrcPort int
	Pid     uint32
	Proto   int
	Comm    string
}

// EventData хранит вложенные структуры как значения
type EventData struct {
	Lookup  Lookup
	Sendmsg Sendmsg
	Recvmsg Recvmsg
}

// Предварительно объявляем сигнатуру ProcessEvent
func ProcessEvent(e bpfTraceInfo,
	eventMap, eventMap1 map[int]EventData,
) {
	// TODO: перенести логику разбора bpfTraceInfo и заполнения EventData сюда.
	// Пример паттерна обновления:
	//
	// id := int(e.SomeId)
	// var ed EventData
	// ed.Lookup = Lookup{ DstIP: …, DstPort: …, /* … */ }
	// if условие для sendmsg {
	//     ed.Sendmsg = Sendmsg{ DstIP: …, /* … */ }
	//     eventMap[id] = ed
	// }
	// if условие для recvmsg {
	//     existing := eventMap[id]   // копия
	//     existing.Recvmsg = Recvmsg{ /* … */ }
	//     eventMap[id] = existing
	// }
}

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock limit: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load eBPF objects: %v", err)
	}
}

func main() {
	defer objs.Close()

	if err := attachTracepoints(); err != nil {
		log.Fatalf("attach tracepoint error: %v", err)
	}
	defer func() {
		SmsgEnter.Close()
		SmsgExit.Close()
		SEnter.Close()
		SExit.Close()
		RmsgEnter.Close()
		RmsgExit.Close()
		REnter.Close()
		RExit.Close()
		InetSock.Close()
	}()

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("open self netns: %v", err)
	}
	defer netns.Close()

	skLookupLink, err = link.AttachNetNs(int(netns.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("attach sk_lookup: %v", err)
	}
	defer skLookupLink.Close()

	go readEvents()

	fmt.Println("Press Ctrl+C to exit")
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)
	<-stop
	fmt.Println("Exiting...")
}

func readEvents() {
	const buffLen = 4096 * 2
	rd, err := perf.NewReader(objs.TraceEvents, buffLen)
	if err != nil {
		log.Fatalf("perf reader: %v", err)
	}
	defer rd.Close()

	executableName := os.Args[0]
	if len(executableName) > 2 {
		executableName = executableName[2:]
	}

	// Карты теперь хранят value-типы EventData
	eventMap := make(map[int]EventData)
	eventMap_1 := make(map[int]EventData)

	for {
		record, err := rd.Read()
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				continue
			}
			log.Printf("perf read error: %v", err)
			return
		}
		if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			log.Println("invalid sample size")
			continue
		}

		event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
		if pkg.Int8ToString(event.Comm) == executableName {
			continue
		}

		// Передача карт по value-типу EventData
		ProcessEvent(event, eventMap, eventMap_1)
	}
}








