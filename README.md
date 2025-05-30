
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
	"fmt"
	"net"
)

func ProcessEvent(event bpfTraceInfo,
	eventMap, eventMap_1 map[int]EventData,
) {
	srcIP := net.IPv4(
		byte(event.SrcIP.S_addr),
		byte(event.SrcIP.S_addr>>8),
		byte(event.SrcIP.S_addr>>16),
		byte(event.SrcIP.S_addr>>24),
	)
	dstIP := net.IPv4(
		byte(event.DstIP.S_addr),
		byte(event.DstIP.S_addr>>8),
		byte(event.DstIP.S_addr>>16),
		byte(event.DstIP.S_addr>>24),
	)

	if event.Sysexit == 1 {
		family := event.Family
		if family == 2 { /* IPv4 */
			port := int(event.Dport)
			data, ok := eventMap_1[port]
			if !ok {
				data = EventData{}
				eventMap[port] = data
			}

			data.Sendmsg = Sendmsg{
				DstIP:   dstIP,
				DstPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Recvmsg != nil {
				if data.Lookup.Proto == 17 {
					proto = "UDP"
				}

				if data.Lookup.DstIP.IsLoopback() {
					dsthost = "localhost"
				} else {
					dsthost = pkg.ResolveIP(data.Lookup.DstIP)
				}
				if data.Lookup.SrcIP.IsLoopback() {
					srchost = "localhost"
				} else {
					srchost = pkg.ResolveIP(data.Lookup.SrcIP)
				}

				fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
					data.Sendmsg.Pid,
					data.Sendmsg.Comm,
					proto,
					dsthost,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					srchost,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
					data.Recvmsg.Pid,
					data.Recvmsg.Comm,
					proto,
					dsthost,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					srchost,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				delete(eventMap, port)
			}
		} else if family == 10 { /* IPv6 */
			ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
			fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
				event.Pid, ip6.String(), event.Dport, pkg.Int8ToString(event.Comm))
		}
		return
	}

	if event.Sysexit == 11 {
		if event.Family == 2 {
			port := int(event.Dport)
			data, ok := eventMap[port]
			if !ok {
				data = EventData{}
				eventMap[port] = data
			}

			data.Sendmsg = Sendmsg{
				DstIP:   dstIP,
				DstPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Recvmsg != nil {
				if data.Lookup.Proto == 17 {
					proto = "UDP"
				}

				fmt.Println()
				fmt.Printf("SENDMSG PID=%d NAME=%s %s/%s:%d->%s:%d\n",
					data.Sendmsg.Pid,
					data.Sendmsg.Comm,
					proto,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Printf("SENDMSG PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
					data.Recvmsg.Pid,
					data.Recvmsg.Comm,
					proto,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Println()
				delete(eventMap, port)
			}
		} else if event.Family == 10 {
			ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
			fmt.Printf("SENDMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
				event.Pid, ip6.String(), event.Dport, pkg.Int8ToString(event.Comm))
		}
		return
	}

	if event.Sysexit == 2 {
		if event.Family == 2 {
			port := int(event.Sport)
			data, ok := eventMap[port]
			if !ok {
				data = EventData{}
				eventMap[port] = data
			}

			data.Recvmsg = Recvmsg{
				SrcIP:   srcIP,
				SrcPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Sendmsg != nil {
				if data.Lookup.Proto == 17 {
					proto = "UDP"
				}

				if data.Lookup.DstIP.IsLoopback() {
					dsthost = "localhost"
				} else {
					dsthost = pkg.ResolveIP(dstIP)
				}
				if data.Lookup.SrcIP.IsLoopback() {
					srchost = "localhost"
				} else {
					srchost = pkg.ResolveIP(srcIP)
				}

				fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
					data.Sendmsg.Pid,
					data.Sendmsg.Comm,
					proto,
					dsthost,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					srchost,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
					data.Recvmsg.Pid,
					data.Recvmsg.Comm,
					proto,
					dsthost,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					srchost,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				delete(eventMap, port)
			}
		} else if event.Family == 10 {
			ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
			fmt.Printf("RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
				event.Pid, ip6.String(), event.Sport, pkg.Int8ToString(event.Comm))
		}
		return
	}

	if event.Sysexit == 12 {
		if event.Family == 2 {
			port := int(event.Sport)
			data, ok := eventMap[port]
			if !ok {
				data = EventData{}
				eventMap[port] = data
			}

			data.Recvmsg = Recvmsg{
				SrcIP:   srcIP,
				SrcPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Sendmsg != nil {
				if data.Lookup.Proto == 17 {
					proto = "UDP"
				}

				fmt.Println()
				fmt.Printf("RECVMSG PID=%d NAME=%s %s/%s:%d->%s:%d\n",
					data.Sendmsg.Pid,
					data.Sendmsg.Comm,
					proto,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Printf("RECVMSG PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
					data.Recvmsg.Pid,
					data.Recvmsg.Comm,
					proto,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Println()
				delete(eventMap, port)
			}
		} else if event.Family == 10 {
			ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
			fmt.Printf("RECVMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
				event.Pid, ip6.String(), event.Sport, pkg.Int8ToString(event.Comm))
		}
		return
	}

	if event.Sysexit == 3 {
		if event.Family == 2 {
			port := int(event.Dport)
			data, ok := eventMap[port]
			if !ok {
				data = EventData{}
				eventMap[port] = data
			}

			data.Lookup = Lookup{
				SrcIP:   srcIP,
				SrcPort: int(event.Sport),
				DstIP:   dstIP,
				DstPort: int(event.Dport),
				Proto:   int(event.Proto),
			}

			port1 := int(event.Sport)
			data1, ok := eventMap_1[port1]
			if !ok {
				data1 = EventData{}
				eventMap_1[port1] = data1
			}

			data1.Lookup = Lookup{
				SrcIP:   srcIP,
				SrcPort: int(event.Sport),
				DstIP:   dstIP,
				DstPort: int(event.Dport),
				Proto:   int(event.Proto),
			}

			if data.Recvmsg != nil && data.Sendmsg != nil {
				if data.Lookup.Proto == 17 {
					proto = "UDP"
				}

				fmt.Println()
				if data.Lookup.DstIP.IsLoopback() {
					dsthost = "localhost"
				} else {
					dsthost = pkg.ResolveIP(data.Lookup.DstIP)
				}
				if data.Lookup.SrcIP.IsLoopback() {
					srchost = "localhost"
				} else {
					srchost = pkg.ResolveIP(data.Lookup.SrcIP)
				}

				fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
					data.Sendmsg.Pid,
					data.Sendmsg.Comm,
					proto,
					dsthost,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					srchost,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
					data.Recvmsg.Pid,
					data.Recvmsg.Comm,
					proto,
					dsthost,
					data.Lookup.DstIP,
					data.Lookup.DstPort,
					srchost,
					data.Lookup.SrcIP,
					data.Lookup.SrcPort,
				)
				fmt.Println()
				delete(eventMap, port)
			}
		} else if event.Family == 10 {
			ip6s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
			ip6d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
			fmt.Printf("LOOKUP SRC IPv6=%s\n", ip6s)
			fmt.Printf("LOOKUP DST IPv6=%s\n", ip6d)
			fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROTO=%d\n", event.Sport, event.Dport, event.Proto)
		}
		return
	}

	if event.Sysexit == 6 {
		if event.Family == 10 {
			ip6s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
			ip6d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
			fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
				event.Pid, ip6s.String(), event.Sport, pkg.Int8ToString(event.Comm))
			fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
				event.Pid, ip6d.String(), event.Dport, pkg.Int8ToString(event.Comm))
			fmt.Printf("!!!!!TCP IPv6 !!!!!!PID=%d  NAME=%s\n",
				event.Pid, pkg.Int8ToString(event.Comm))
		}

		if event.State == 1 {
			mu.Lock()
			select {
			case eventChan_sport <- int(event.Sport):
			default:
				eventChan_sport <- int(event.Sport)
				fmt.Printf("State 1: заменен порт %d\n", event.Sport)
			}
			mu.Unlock()

			if dstIP.IsLoopback() {
				dsthost = pkg.ResolveIP(dstIP)
			} else {
				var err error
				dsthost, err = pkg.ResolveIP_n(dstIP)
				if err != nil {
					dsthost = "unknown"
				}
			}
			srchost = pkg.ResolveIP(srcIP)
			srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
			dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)
			if event.Proto == 6 {
				proto = "TCP"
			}
			fmt.Println()
			fmt.Printf("PID=%d %s:%s <- %s:%s \n",
				event.Pid, proto, srcAddr, proto, dstAddr)
		}

		if event.State == 2 || event.State == 10 {
			mu.Lock()
			select {
			case eventChan_pid <- int(event.Pid):
			default:
			}
			mu.Unlock()
		}

		select {
		case xxx = <-eventChan_sport:
			if dstIP.IsLoopback() {
				dsthost = pkg.ResolveIP(dstIP)
			} else {
				var err error
				dsthost, err = pkg.ResolveIP_n(dstIP)
				if err != nil {
					dsthost = "unknown"
				}
			}
			srchost = pkg.ResolveIP(srcIP)
			srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), xxx)
			dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

			select {
			case xxx_pid = <-eventChan_pid:
			default:
			}
			if event.Proto == 6 {
				proto = "TCP"
			}
			fmt.Printf("PID=%d %s:%s -> %s:%s \n",
				xxx_pid, proto, srcAddr, proto, dstAddr)
			fmt.Println()
		default:
			fmt.Println()
		}
		return
	}
}


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

type EventData struct {
	Lookup  Lookup
	Sendmsg Sendmsg
	Recvmsg Recvmsg
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

	eventMap := make(map[int]*EventData)
	eventMap_1 := make(map[int]*EventData)

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

		ProcessEvent(event, eventMap, eventMap_1)
	}
}


[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
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
	"message": "cannot use eventMap (variable of type map[int]*EventData) as map[int]EventData value in argument to ProcessEvent",
	"source": "compiler",
	"startLineNumber": 161,
	"startColumn": 23,
	"endLineNumber": 161,
	"endColumn": 31
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
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
	"message": "cannot use eventMap_1 (variable of type map[int]*EventData) as map[int]EventData value in argument to ProcessEvent",
	"source": "compiler",
	"startLineNumber": 161,
	"startColumn": 33,
	"endLineNumber": 161,
	"endColumn": 43
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/prcessevent.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "MismatchedTypes",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "MismatchedTypes"
		}
	},
	"severity": 8,
	"message": "invalid operation: data.Lookup != nil (mismatched types Lookup and untyped nil)",
	"source": "compiler",
	"startLineNumber": 42,
	"startColumn": 22,
	"endLineNumber": 42,
	"endColumn": 25
}]








