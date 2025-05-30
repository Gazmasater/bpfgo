
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
	"sync/atomic"
	"syscall"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

// ─────────────────────────────────────────────────────────────
//              eBPF-объекты, подготовка окружения
// ─────────────────────────────────────────────────────────────

var objs bpfObjects

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("rlimit: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("load bpf: %v", err)
	}
}

// ─────────────────────────────────────────────────────────────
//                          Типы данных
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

// value-структура без внутренних указателей
type EventData struct {
	Lookup      Lookup
	Sendmsg     Sendmsg
	Recvmsg     Recvmsg
	haveLookup  bool
	haveSendmsg bool
	haveRecvmsg bool
}

// ─────────────────────────────────────────────────────────────
//                 Пул для EventData и вспом. функции
// ─────────────────────────────────────────────────────────────

var eventPool = sync.Pool{
	New: func() any { return new(EventData) },
}

func getEvent(port int, m map[int]*EventData) *EventData {
	if e, ok := m[port]; ok {
		return e
	}
	e := eventPool.Get().(*EventData)
	*e = EventData{}      // обнуляем поля
	m[port] = e
	return e
}

func releaseEvent(port int, m map[int]*EventData) {
	if e, ok := m[port]; ok {
		delete(m, port)
		eventPool.Put(e)
	}
}

// ─────────────────────────────────────────────────────────────
//          Пэдинг-обёртки для атомиков (false-sharing)
// ─────────────────────────────────────────────────────────────

type paddedUint32 struct {
	v atomic.Uint32
	_ [60]byte // 64-байтная cache-line
}

var sportAtomic paddedUint32
var pidAtomic  paddedUint32

// ─────────────────────────────────────────────────────────────

func main() {

	// карты: ключ — порт (sport или dport), значение — *EventData
	eventMap := make(map[int]*EventData)
	eventMapRev := make(map[int]*EventData) // «зеркальная» для sport ↔ dport

	defer objs.Close()

	// открываем собственный network-namespace
	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("open netns: %v", err)
	}
	defer netns.Close()
	fmt.Printf("NS fd=%d\n", netns.Fd())

	// ───── привязываем eBPF-программы к tracepoint-ам ─────
	traceOrDie := func(cat, name string, prog *ebpf.Program) link.Link {
		l, e := link.Tracepoint(cat, name, prog, nil)
		if e != nil {
			log.Fatalf("tracepoint %s/%s: %v", cat, name, e)
		}
		return l
	}
	defer traceOrDie("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter).Close()
	defer traceOrDie("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit).Close()
	defer traceOrDie("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter).Close()
	defer traceOrDie("syscalls", "sys_exit_sendto", objs.TraceSendtoExit).Close()
	defer traceOrDie("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter).Close()
	defer traceOrDie("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit).Close()
	defer traceOrDie("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter).Close()
	defer traceOrDie("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit).Close()
	defer traceOrDie("sock", "inet_sock_set_state", objs.TraceTcpEst).Close()

	skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("attach sk_lookup: %v", err)
	}
	defer skLookupLink.Close()

	// ─────────────────────────────────────────────────────────
	//                  Чтение perf-событий
	// ─────────────────────────────────────────────────────────
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 8192
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("perf reader: %v", err)
		}
		defer rd.Close()

		selfExe := os.Args[0]
		if len(selfExe) > 2 {
			selfExe = selfExe[2:]
		}

		for {
			rec, err := rd.Read()
			if err != nil {
				if errors.Is(err, os.ErrDeadlineExceeded) {
					continue
				}
				log.Printf("perf read: %v", err)
				return
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}

			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))

			// отфильтровали свои же пакеты
			if pkg.Int8ToString(ev.Comm) == selfExe {
				continue
			}

			srcIP := net.IPv4(
				byte(ev.SrcIP.S_addr),
				byte(ev.SrcIP.S_addr>>8),
				byte(ev.SrcIP.S_addr>>16),
				byte(ev.SrcIP.S_addr>>24),
			)
			dstIP := net.IPv4(
				byte(ev.DstIP.S_addr),
				byte(ev.DstIP.S_addr>>8),
				byte(ev.DstIP.S_addr>>16),
				byte(ev.DstIP.S_addr>>24),
			)

			switch ev.Sysexit {

			// ───────────────────── LOOKUP ──────────────────────
			case 3: // sk_lookup verdict
				if ev.Family != 2 { break }
				port := int(ev.Dport)
				revPort := int(ev.Sport)

				d := getEvent(port, eventMap)
				d.Lookup = Lookup{
					SrcIP:   srcIP,
					SrcPort: int(ev.Sport),
					DstIP:   dstIP,
					DstPort: int(ev.Dport),
					Proto:   int(ev.Proto),
				}
				d.haveLookup = true

				d2 := getEvent(revPort, eventMapRev)
				*d2 = *d         // зеркало
				d2.haveLookup = true

			// ───────────────────── SENDMSG / SENDTO ────────────
			case 11, 1: // sendmsg exit / sendto exit
				if ev.Family != 2 { break }
				port := int(ev.Dport)
				d := getEvent(port, eventMap)
				d.Sendmsg = Sendmsg{
					DstIP:   dstIP,
					DstPort: port,
					Pid:     ev.Pid,
					Comm:    pkg.Int8ToString(ev.Comm),
					Proto:   int(ev.Proto),
				}
				d.haveSendmsg = true

			// ───────────────────── RECVMSG / RECVFROM ──────────
			case 12, 2: // recvmsg exit / recvfrom exit
				if ev.Family != 2 { break }
				port := int(ev.Sport)
				d := getEvent(port, eventMap)
				d.Recvmsg = Recvmsg{
					SrcIP:   srcIP,
					SrcPort: port,
					Pid:     ev.Pid,
					Comm:    pkg.Int8ToString(ev.Comm),
					Proto:   int(ev.Proto),
				}
				d.haveRecvmsg = true

			// ───────────────────── TCP state change ────────────
			case 6: // inet_sock_set_state
				if ev.State == 1 { // established
					sportAtomic.v.Store(uint32(ev.Sport))
				} else if ev.State == 2 || ev.State == 10 {
					pidAtomic.v.Store(ev.Pid)
				}
			}

			// ───── если собрали все три части события ─────
			if ev.Family == 2 {
				var port int
				if ev.Sysexit == 3 {
					port = int(ev.Dport)
				} else if ev.Sysexit == 2 || ev.Sysexit == 12 {
					port = int(ev.Sport)
				} else {
					port = int(ev.Dport)
				}

				if d, ok := eventMap[port]; ok &&
					d.haveLookup && d.haveSendmsg && d.haveRecvmsg {

					proto := "UDP"
					if d.Lookup.Proto == 6 {
						proto = "TCP"
					}
					fmt.Printf("[%s] PID=%d %s:%d ⇆ %s:%d  (%s)\n",
						d.Sendmsg.Comm,
						d.Sendmsg.Pid,
						d.Lookup.SrcIP, d.Lookup.SrcPort,
						d.Lookup.DstIP, d.Lookup.DstPort,
						proto,
					)

					releaseEvent(port, eventMap)
				}
			}
		}
	}()

	fmt.Println("Ctrl+C → exit")
	<-stop
}

// ─────────────────────────────────────────────────────────────
//                  Вспомогательная функция
// ─────────────────────────────────────────────────────────────

func IPv6BytesToWords(addr [16]uint8) [4]uint32 {
	var w [4]uint32
	for i := 0; i < 4; i++ {
		w[i] = uint32(addr[i*4]) |
			uint32(addr[i*4+1])<<8 |
			uint32(addr[i*4+2])<<16 |
			uint32(addr[i*4+3])<<24
	}
	return w
}
















