
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

// ─────────────────────────────────────────────────────────────
//                        Глобальные данные
// ─────────────────────────────────────────────────────────────

var objs bpfObjects

var (
	eventChan_sport = make(chan int, 1)
	eventChan_pid   = make(chan int, 1)

	mu             sync.Mutex
	xxx, xxx_pid   int
	proto          string
	srchost        string
	dsthost        string
)

// ─────────────────────────────────────────────────────────────
//                          Пользовательские типы
// ─────────────────────────────────────────────────────────────

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

// *** ИЗМЕНЁННАЯ СТРУКТУРА ***
type EventData struct {
	Lookup      Lookup
	Sendmsg     Sendmsg
	Recvmsg     Recvmsg
	haveLookup  bool
	haveSendmsg bool
	haveRecvmsg bool
}

// ─────────────────────────────────────────────────────────────
//                            init()
// ─────────────────────────────────────────────────────────────

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

// ─────────────────────────────────────────────────────────────
//                               main
// ─────────────────────────────────────────────────────────────

func main() {
	eventMap := make(map[int]*EventData)
	eventMap_1 := make(map[int]*EventData)

	defer objs.Close()

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", netns.Fd())

	// ─ Tracepoints
	SmsgEnter := mustTP("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter)
	defer SmsgEnter.Close()
	SmsgExit := mustTP("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit)
	defer SmsgExit.Close()
	SEnter := mustTP("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	defer SEnter.Close()
	SExit := mustTP("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	defer SExit.Close()
	RmsgEnter := mustTP("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter)
	defer RmsgEnter.Close()
	RmsgExit := mustTP("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit)
	defer RmsgExit.Close()
	REnter := mustTP("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	defer REnter.Close()
	RExit := mustTP("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)
	defer RExit.Close()
	InetSock := mustTP("sock", "inet_sock_set_state", objs.TraceTcpEst)
	defer InetSock.Close()

	skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("failed to attach sk_lookup: %v", err)
	}
	defer skLookupLink.Close()

	// ─ perf reader goroutine
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 4096 * 2
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("perf reader: %v", err)
		}
		defer rd.Close()

		exeName := os.Args[0]
		if len(exeName) > 2 {
			exeName = exeName[2:]
		}

		for {
			rec, err := rd.Read()
			if err != nil {
				if errors.Is(err, os.ErrDeadlineExceeded) {
					continue
				}
				log.Printf("perf read error: %v", err)
				return
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				log.Println("invalid event size")
				continue
			}

			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))

			srcIP := ip4FromUint32(ev.SrcIP.S_addr)
			dstIP := ip4FromUint32(ev.DstIP.S_addr)

			if pkg.Int8ToString(ev.Comm) == exeName {
				continue
			}

			switch ev.Sysexit {

			// ───────────────── SENDTO (exit)
			case 1:
				if ev.Family != 2 {
					printSendToIPv6(ev)
					break
				}
				port := int(ev.Dport)
				data := getEvent(port, eventMap_1, eventMap)

				data.Sendmsg = Sendmsg{
					DstIP:   dstIP,
					DstPort: port,
					Pid:     ev.Pid,
					Comm:    pkg.Int8ToString(ev.Comm),
				}
				data.haveSendmsg = true

				if data.haveLookup && data.haveRecvmsg {
					printSendTo(data)
					delete(eventMap, port)
				}

			// ───────────────── SENDMSG (exit)
			case 11:
				if ev.Family != 2 {
					printSendMsgIPv6(ev)
					break
				}
				port := int(ev.Dport)
				data := getEvent(port, eventMap, nil)

				data.Sendmsg = Sendmsg{
					DstIP:   dstIP,
					DstPort: port,
					Pid:     ev.Pid,
					Comm:    pkg.Int8ToString(ev.Comm),
				}
				data.haveSendmsg = true

				if data.haveLookup && data.haveRecvmsg {
					printSendMsg(data)
					delete(eventMap, port)
				}

			// ───────────────── RECVFROM (exit)
			case 2:
				if ev.Family != 2 {
					printRecvFromIPv6(ev)
					break
				}
				port := int(ev.Sport)
				data := getEvent(port, eventMap, nil)

				data.Recvmsg = Recvmsg{
					SrcIP:   srcIP,
					SrcPort: port,
					Pid:     ev.Pid,
					Comm:    pkg.Int8ToString(ev.Comm),
				}
				data.haveRecvmsg = true

				if data.haveLookup && data.haveSendmsg {
					printRecvFrom(data, dstIP, srcIP)
					delete(eventMap, port)
				}

			// ───────────────── RECVMSG (exit)
			case 12:
				if ev.Family != 2 {
					printRecvMsgIPv6(ev)
					break
				}
				port := int(ev.Sport)
				data := getEvent(port, eventMap, nil)

				data.Recvmsg = Recvmsg{
					SrcIP:   srcIP,
					SrcPort: port,
					Pid:     ev.Pid,
					Comm:    pkg.Int8ToString(ev.Comm),
				}
				data.haveRecvmsg = true

				if data.haveLookup && data.haveSendmsg {
					printRecvMsg(data)
					delete(eventMap, port)
				}

			// ───────────────── LOOKUP verdict
			case 3:
				if ev.Family != 2 {
					printLookupIPv6(ev)
					break
				}
				port := int(ev.Dport)
				data := getEvent(port, eventMap, nil)

				data.Lookup = Lookup{
					SrcIP:   srcIP,
					SrcPort: int(ev.Sport),
					DstIP:   dstIP,
					DstPort: int(ev.Dport),
					Proto:   int(ev.Proto),
				}
				data.haveLookup = true

				portRev := int(ev.Sport)
				dataRev := getEvent(portRev, eventMap_1, nil)
				dataRev.Lookup = data.Lookup
				dataRev.haveLookup = true

				if data.haveSendmsg && data.haveRecvmsg {
					printLookup(data)
					delete(eventMap, port)
				}

			// ───────────────── TCP state changes
			case 6:
				handleTCPState(ev, dstIP, srcIP)
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

// ─────────────────────────────────────────────────────────────
//                       Вспомогательные функции
// ─────────────────────────────────────────────────────────────

func mustTP(cat, name string, prog *ebpf.Program) link.Link {
	l, err := link.Tracepoint(cat, name, prog, nil)
	if err != nil {
		log.Fatalf("tracepoint %s/%s: %v", cat, name, err)
	}
	return l
}

func ip4FromUint32(v uint32) net.IP {
	return net.IPv4(byte(v), byte(v>>8), byte(v>>16), byte(v>>24))
}

func getEvent(port int, m map[int]*EventData, alt map[int]*EventData) *EventData {
	if e, ok := m[port]; ok {
		return e
	}
	e := &EventData{}
	m[port] = e
	if alt != nil {
		alt[port] = e
	}
	return e
}

// ─── Печать/форматирование (оставлены как были, только убраны указатели)

func printSendTo(data *EventData) {
	if data.Lookup.Proto == 17 {
		proto = "UDP"
	}
	resolveHosts(data)
	fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
		data.Sendmsg.Pid, data.Sendmsg.Comm,
		proto, dsthost, data.Lookup.DstIP, data.Lookup.DstPort,
		srchost, data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
		data.Recvmsg.Pid, data.Recvmsg.Comm,
		proto, dsthost, data.Lookup.DstIP, data.Lookup.DstPort,
		srchost, data.Lookup.SrcIP, data.Lookup.SrcPort)
}

func printSendMsg(data *EventData) {
	if data.Lookup.Proto == 17 {
		proto = "UDP"
	}
	fmt.Println()
	fmt.Printf("SENDMSG PID=%d NAME=%s %s/%s:%d->%s:%d\n",
		data.Sendmsg.Pid, data.Sendmsg.Comm, proto,
		data.Lookup.DstIP, data.Lookup.DstPort,
		data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Printf("SENDMSG PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
		data.Recvmsg.Pid, data.Recvmsg.Comm, proto,
		data.Lookup.DstIP, data.Lookup.DstPort,
		data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Println()
}

func printRecvFrom(data *EventData, dstIP, srcIP net.IP) {
	if data.Lookup.Proto == 17 {
		proto = "UDP"
	}
	resolveHosts(data)
	fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
		data.Sendmsg.Pid, data.Sendmsg.Comm,
		proto, dsthost, data.Lookup.DstIP, data.Lookup.DstPort,
		srchost, data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
		data.Recvmsg.Pid, data.Recvmsg.Comm,
		proto, dsthost, data.Lookup.DstIP, data.Lookup.DstPort,
		srchost, data.Lookup.SrcIP, data.Lookup.SrcPort)
}

func printRecvMsg(data *EventData) {
	if data.Lookup.Proto == 17 {
		proto = "UDP"
	}
	fmt.Println()
	fmt.Printf("RECVMSG PID=%d NAME=%s %s/%s:%d->%s:%d\n",
		data.Sendmsg.Pid, data.Sendmsg.Comm, proto,
		data.Lookup.DstIP, data.Lookup.DstPort,
		data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Printf("RECVMSG PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
		data.Recvmsg.Pid, data.Recvmsg.Comm, proto,
		data.Lookup.DstIP, data.Lookup.DstPort,
		data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Println()
}

func printLookup(data *EventData) {
	if data.Lookup.Proto == 17 {
		proto = "UDP"
	}
	resolveHosts(data)
	fmt.Println()
	fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
		data.Sendmsg.Pid, data.Sendmsg.Comm, proto,
		dsthost, data.Lookup.DstIP, data.Lookup.DstPort,
		srchost, data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
		data.Recvmsg.Pid, data.Recvmsg.Comm, proto,
		dsthost, data.Lookup.DstIP, data.Lookup.DstPort,
		srchost, data.Lookup.SrcIP, data.Lookup.SrcPort)
	fmt.Println()
}

func resolveHosts(data *EventData) {
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
}

// IPv6 print helpers (оставлены без изменений логики)

func printSendToIPv6(ev bpfTraceInfo) {
	port := ev.Dport
	pid := ev.Pid
	ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.DstIP6.In6U.U6Addr8))
	fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
		pid, ip6.String(), port, pkg.Int8ToString(ev.Comm))
}

func printSendMsgIPv6(ev bpfTraceInfo) {
	port := ev.Dport
	pid := ev.Pid
	ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.DstIP6.In6U.U6Addr8))
	fmt.Printf("SENDMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
		pid, ip6.String(), port, pkg.Int8ToString(ev.Comm))
}

func printRecvFromIPv6(ev bpfTraceInfo) {
	port := ev.Sport
	pid := ev.Pid
	ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.SrcIP6.In6U.U6Addr8))
	fmt.Printf("RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
		pid, ip6.String(), port, pkg.Int8ToString(ev.Comm))
}

func printRecvMsgIPv6(ev bpfTraceInfo) {
	port := ev.Sport
	pid := ev.Pid
	ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.SrcIP6.In6U.U6Addr8))
	fmt.Printf("RECVMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
		pid, ip6.String(), port, pkg.Int8ToString(ev.Comm))
}

func printLookupIPv6(ev bpfTraceInfo) {
	ip6s := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.SrcIP6.In6U.U6Addr8))
	ip6d := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.DstIP6.In6U.U6Addr8))
	fmt.Printf("LOOKUP SRC IPv6=%s\n", ip6s)
	fmt.Printf("LOOKUP DST IPv6=%s\n", ip6d)
	fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROTO=%d\n", ev.Sport, ev.Dport, ev.Proto)
}

// TCP state handling (оставлена прежняя логика)

func handleTCPState(ev bpfTraceInfo, dstIP, srcIP net.IP) {
	if ev.Family == 10 {
		printTCPIPv6(ev)
	}

	switch ev.State {
	case 1: // ESTABLISHED
		mu.Lock()
		select {
		case eventChan_sport <- int(ev.Sport):
		default:
			eventChan_sport <- int(ev.Sport)
		}
		mu.Unlock()

		setHostsForTCP(dstIP, srcIP)
		printTCPFlow(ev)

	case 2, 10:
		mu.Lock()
		select {
		case eventChan_pid <- int(ev.Pid):
		default:
		}
		mu.Unlock()
	}

	select {
	case xxx = <-eventChan_sport:
		setHostsForTCP(dstIP, srcIP)
		srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), xxx)
		dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), ev.Dport)
		select {
		case xxx_pid = <-eventChan_pid:
		default:
		}
		if ev.Proto == 6 {
			proto = "TCP"
		}
		fmt.Printf("PID=%d %s:%s -> %s:%s \n", xxx_pid, proto, srcAddr, proto, dstAddr)
		fmt.Println()
	default:
	}
}

func printTCPIPv6(ev bpfTraceInfo) {
	sport := ev.Sport
	dport := ev.Dport
	pid := ev.Pid
	ip6s := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.SrcIP6.In6U.U6Addr8))
	ip6d := pkg.IPv6FromLEWords(IPv6BytesToWords(ev.DstIP6.In6U.U6Addr8))

	fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n", pid, ip6s.String(), sport, pkg.Int8ToString(ev.Comm))
	fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n", pid, ip6d.String(), dport, pkg.Int8ToString(ev.Comm))
}

func setHostsForTCP(dstIP, srcIP net.IP) {
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
}

func printTCPFlow(ev bpfTraceInfo) {
	srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, ev.SrcIP.S_addr, ev.Sport)
	dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, ev.DstIP.S_addr, ev.Dport)
	if ev.Proto == 6 {
		proto = "TCP"
	}
	fmt.Println()
	fmt.Printf("PID=%d %s:%s <- %s:%s \n", ev.Pid, proto, srcAddr, proto, dstAddr)
}

// IPv6 helpers

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











