
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



// go:build linux
// +build linux

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

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

/* -------------------------------------------------------------------------- */
/*                             Глобальные объекты                              */
/* -------------------------------------------------------------------------- */

var objs bpfObjects

/* ссылки-трейспоинты с вашими названиями */
var (
	SmsgEnter link.Link
	SmsgExit  link.Link
	SEnter    link.Link
	SExit     link.Link
	RmsgEnter link.Link
	RmsgExit  link.Link
	REnter    link.Link
	RExit     link.Link
	InetSock  link.Link
	skLookupLink link.Link
)

/* каналы и синхронизация — без изменений */
var (
	eventChan_sport = make(chan int, 1)
	eventChan_pid   = make(chan int, 1)
	mu              sync.Mutex
)

/* глобальные переменные, которые использовались в исходнике */
var (
	xxx, xxx_pid int
	proto        string
	srchost      string
	dsthost      string
)

/* -------------------------------------------------------------------------- */
/*                                 C-структуры                                 */
/* -------------------------------------------------------------------------- */

/* тип bpfTraceInfo создан на стороне eBPF (c-struct); объявление здесь должно
   совпадать по полям и размерам. Приведён минимальный пример; дополните при
   необходимости полями, которые есть в вашем object-файле. */
type bpfTraceInfo struct {
	/* IPv4 */
	SrcIP pkg.InAddr
	DstIP pkg.InAddr
	/* IPv6 */
	SrcIP6 pkg.In6Addr
	DstIP6 pkg.In6Addr

	Sport  uint16
	Dport  uint16
	Family uint16
	Proto  uint8
	State  uint8

	Sysexit uint8

	Pid  uint32
	Comm [16]int8
}

/* -------------------------------------------------------------------------- */
/*                               Пользовательские                              */
/* -------------------------------------------------------------------------- */

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
	Lookup  *Lookup
	Sendmsg *Sendmsg
	Recvmsg *Recvmsg
}

/* -------------------------------------------------------------------------- */
/*                                    init                                    */
/* -------------------------------------------------------------------------- */

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock limit: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load eBPF objects: %v", err)
	}
}

/* -------------------------------------------------------------------------- */
/*                                    main                                    */
/* -------------------------------------------------------------------------- */

func main() {
	defer objs.Close()

	/* подключаем все трейспоинты одним циклом */
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

	/* подключаем sk_lookup к namespace текущего процесса */
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

	/* читаем perf-события */
	go readEvents()

	fmt.Println("Press Ctrl+C to exit")
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)
	<-stop
	fmt.Println("Exiting...")
}

/* -------------------------------------------------------------------------- */
/*                            attachTracepoints                               */
/* -------------------------------------------------------------------------- */

func attachTracepoints() error {
	type tp struct {
		cat, name string
		prog      *ebpf.Program
		linkPtr   *link.Link
	}
	tps := []tp{
		{"syscalls", "sys_enter_sendmsg",  objs.TraceSendmsgEnter, &SmsgEnter},
		{"syscalls", "sys_exit_sendmsg",   objs.TraceSendmsgExit,  &SmsgExit},
		{"syscalls", "sys_enter_sendto",   objs.TraceSendtoEnter,  &SEnter},
		{"syscalls", "sys_exit_sendto",    objs.TraceSendtoExit,   &SExit},
		{"syscalls", "sys_enter_recvmsg",  objs.TraceRecvmsgEnter, &RmsgEnter},
		{"syscalls", "sys_exit_recvmsg",   objs.TraceRecvmsgExit,  &RmsgExit},
		{"syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter,&REnter},
		{"syscalls", "sys_exit_recvfrom",  objs.TraceRecvfromExit, &RExit},
		{"sock",     "inet_sock_set_state",objs.TraceTcpEst,       &InetSock},
	}
	for _, p := range tps {
		l, err := link.Tracepoint(p.cat, p.name, p.prog, nil)
		if err != nil {
			for _, q := range tps {
				if *q.linkPtr != nil { (*q.linkPtr).Close() }
			}
			return fmt.Errorf("attach %s/%s: %w", p.cat, p.name, err)
		}
		*p.linkPtr = l
	}
	return nil
}

/* -------------------------------------------------------------------------- */
/*                                readEvents                                  */
/* -------------------------------------------------------------------------- */

func readEvents() {
	const buffLen = 4096 * 2
	rd, err := perf.NewReader(objs.TraceEvents, buffLen)
	if err != nil {
		log.Fatalf("perf reader: %v", err)
	}
	defer rd.Close()

	executableName := os.Args[0]
	if len(executableName) > 2 { executableName = executableName[2:] }

	eventMap   := make(map[int]*EventData)
	eventMap_1 := make(map[int]*EventData)

	for {
		record, err := rd.Read()
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) { continue }
			log.Printf("perf read error: %v", err); return
		}
		if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			log.Println("invalid sample size"); continue
		}

		event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
		if pkg.Int8ToString(event.Comm) == executableName { continue }

		processEvent(event, eventMap, eventMap_1)
	}
}

/* -------------------------------------------------------------------------- */
/*                                processEvent                                */
/* -------------------------------------------------------------------------- */

func processEvent(
	event bpfTraceInfo,
	eventMap map[int]*EventData,
	eventMap_1 map[int]*EventData,
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

	/* --------------------  Sysexit == 1  (sys_exit_sendto) ------------------- */

	if event.Sysexit == 1 {
		family := event.Family
		if family == 2 { /* IPv4 */
			port := int(event.Dport)
			data, ok := eventMap_1[port]
			if !ok { data = &EventData{}; eventMap[port] = data }

			data.Sendmsg = &Sendmsg{
				DstIP:   dstIP,
				DstPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Recvmsg != nil {
				if data.Lookup.Proto == 17 { proto = "UDP" }

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

	/* --------------------  Sysexit == 11 (sys_exit_sendmsg) ------------------ */

	if event.Sysexit == 11 {
		if event.Family == 2 {
			port := int(event.Dport)
			data, ok := eventMap[port]
			if !ok { data = &EventData{}; eventMap[port] = data }

			data.Sendmsg = &Sendmsg{
				DstIP:   dstIP,
				DstPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Recvmsg != nil {
				if data.Lookup.Proto == 17 { proto = "UDP" }

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

	/* --------------------  Sysexit == 2  (sys_exit_recvfrom) ----------------- */

	if event.Sysexit == 2 {
		if event.Family == 2 {
			port := int(event.Sport)
			data, ok := eventMap[port]
			if !ok { data = &EventData{}; eventMap[port] = data }

			data.Recvmsg = &Recvmsg{
				SrcIP:   srcIP,
				SrcPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Sendmsg != nil {
				if data.Lookup.Proto == 17 { proto = "UDP" }

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

	/* --------------------  Sysexit == 12 (sys_exit_recvmsg) ------------------ */

	if event.Sysexit == 12 {
		if event.Family == 2 {
			port := int(event.Sport)
			data, ok := eventMap[port]
			if !ok { data = &EventData{}; eventMap[port] = data }

			data.Recvmsg = &Recvmsg{
				SrcIP:   srcIP,
				SrcPort: port,
				Pid:     event.Pid,
				Comm:    pkg.Int8ToString(event.Comm),
			}

			if data.Lookup != nil && data.Sendmsg != nil {
				if data.Lookup.Proto == 17 { proto = "UDP" }

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

	/* --------------------  Sysexit == 3  (sk_lookup) ------------------------- */

	if event.Sysexit == 3 {
		if event.Family == 2 {
			port := int(event.Dport)
			data, ok := eventMap[port]
			if !ok { data = &EventData{}; eventMap[port] = data }

			data.Lookup = &Lookup{
				SrcIP:   srcIP,
				SrcPort: int(event.Sport),
				DstIP:   dstIP,
				DstPort: int(event.Dport),
				Proto:   int(event.Proto),
			}

			port1 := int(event.Sport)
			data1, ok := eventMap_1[port1]
			if !ok { data1 = &EventData{}; eventMap_1[port1] = data1 }

			data1.Lookup = &Lookup{
				SrcIP:   srcIP,
				SrcPort: int(event.Sport),
				DstIP:   dstIP,
				DstPort: int(event.Dport),
				Proto:   int(event.Proto),
			}

			if data.Recvmsg != nil && data.Sendmsg != nil {
				if data.Lookup.Proto == 17 { proto = "UDP" }

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

	/* --------------------  Sysexit == 6  (inet_sock_set_state) --------------- */

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

		if event.State == 1 { /* established */
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
				if err != nil { dsthost = "unknown" }
			}
			srchost = pkg.ResolveIP(srcIP)
			srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
			dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)
			if event.Proto == 6 { proto = "TCP" }
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
				if err != nil { dsthost = "unknown" }
			}
			srchost = pkg.ResolveIP(srcIP)
			srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), xxx)
			dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

			select {
			case xxx_pid = <-eventChan_pid:
			default:
			}
			if event.Proto == 6 { proto = "TCP" }
			fmt.Printf("PID=%d %s:%s -> %s:%s \n",
				xxx_pid, proto, srcAddr, proto, dstAddr)
			fmt.Println()
		default:
			fmt.Println()
		}
		return
	}
}

/* -------------------------------------------------------------------------- */
/*                              IPv6BytesToWords                              */
/* -------------------------------------------------------------------------- */

func IPv6BytesToWords(addr [16]uint8) [4]uint32 {
	var words [4]uint32
	for i := 0; i < 4; i++ {
		words[i] = uint32(addr[i*4]) |
			uint32(addr[i*4+1])<<8 |
			uint32(addr[i*4+2])<<16 |
			uint32(addr[i*4+3])<<24
	}
	return words
}









