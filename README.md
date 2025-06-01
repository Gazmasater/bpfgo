
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



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


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



Time: 2025-06-01 18:44:04 MSK
Duration: 30.01s, Total samples = 80ms ( 0.27%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 80ms, 100% of 80ms total
Showing top 10 nodes out of 79
      flat  flat%   sum%        cum   cum%
      20ms 25.00% 25.00%       20ms 25.00%  internal/runtime/syscall.Syscall6
      20ms 25.00% 50.00%       20ms 25.00%  runtime.cgocall
      10ms 12.50% 62.50%       10ms 12.50%  fmt.(*buffer).write (inline)
      10ms 12.50% 75.00%       10ms 12.50%  runtime.futex
      10ms 12.50% 87.50%       10ms 12.50%  runtime.memmove
      10ms 12.50%   100%       10ms 12.50%  runtime.pcdatavalue1
         0     0%   100%       20ms 25.00%  bpfgo/pkg.ResolveIP
         0     0%   100%       40ms 50.00%  bpfgo/pkg.ResolveIP_n
         0     0%   100%       10ms 12.50%  fmt.(*buffer).writeString
         0     0%   100%       10ms 12.50%  fmt.(*fmt).fmtInteger
(pprof) 


package main

import (
	"bpfgo/pkg"
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	_ "net/http/pprof"
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

var eventChanSport = make(chan int, 1)
var eventChanPID = make(chan int, 1)
var mu sync.Mutex
var lastSport, lastPID int
var proto, srcHost, dstHost string

// Пул для переиспользования структур EventData
var eventDataPool = sync.Pool{
	New: func() interface{} {
		return &EventData{}
	},
}

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
	Lookup     Lookup
	Sendmsg    Sendmsg
	Recvmsg    Recvmsg
	HasLookup  bool
	HasSendmsg bool
	HasRecvmsg bool
}

func getEventData() *EventData {
	return eventDataPool.Get().(*EventData)
}

func putEventData(ed *EventData) {
	// Сбрасываем флаги перед возвратом в пул
	*ed = EventData{}
	eventDataPool.Put(ed)
}

// ------ Начало исправленного кода для кэша резолвинга ------
var (
	// resolveCache хранит результат резолва ip -> hostname
	resolveCache = make(map[string]string)
	cacheMu      sync.RWMutex
)

// resolveHost сначала проверяет кэш, а затем вызывает pkg.ResolveIP (для IPv4) или pkg.ResolveIP_n (для IPv6)
func resolveHost(ip net.IP) string {
	key := ip.String()

	// Чтение из кэша
	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	// Вычисляем хост и записываем в кэш
	var host string
	if ip.To4() != nil {
		// IPv4: используем pkg.ResolveIP, он никогда не возвращает ошибку, а в случае отсутствия PTR просто отдаст строку IP
		host = pkg.ResolveIP(ip)
	} else {
		// IPv6: используем pkg.ResolveIP_n, который возвращает (string, error)
		var err error
		host, err = pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()

	return host
}
// ------ Конец исправленного кода для кэша резолвинга ------

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

	go func() {
		log.Println("Starting pprof HTTP server on :6060")
		if err := http.ListenAndServe(":6060", nil); err != nil {
			log.Fatalf("pprof ListenAndServe error: %v", err)
		}
	}()

	eventMap := make(map[int]*EventData)
	eventMap1 := make(map[int]*EventData)
	defer objs.Close()

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", netns.Fd())
	fmt.Printf("Go sizeof(traceInfo) = %d\n", unsafe.Sizeof(bpfTraceInfo{}))

	SmsgEnter, err := link.Tracepoint("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendmsg: %s", err)
	}
	defer SmsgEnter.Close()

	SmsgExit, err := link.Tracepoint("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_sendmsg: %s", err)
	}
	defer SmsgExit.Close()

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

	RmsgEnter, err := link.Tracepoint("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_recvmsg: %s", err)
	}
	defer RmsgEnter.Close()

	RmsgExit, err := link.Tracepoint("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_recvmsg: %s", err)
	}
	defer RmsgExit.Close()

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
	// Канал для graceful shutdown
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 4096 * 2 // если кольцо мелкое — увеличить до 4096*16 и т.д.
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("failed to create perf reader: %s", err)
		}
		defer rd.Close()

		executableName := os.Args[0]
		if len(executableName) > 2 {
			executableName = executableName[2:]
		}

		const batchSize = 8

		for {
			// Собираем до batchSize записей в слайс
			var batch []perf.Record
			for i := 0; i < batchSize; i++ {
				record, err := rd.Read()
				if err != nil {
					if errors.Is(err, os.ErrDeadlineExceeded) {
						break
					}
					log.Fatalf("error reading from perf reader: %v", err)
				}
				batch = append(batch, record)
				if len(batch) >= batchSize {
					break
				}
			}
			if len(batch) == 0 {
				continue
			}

			for _, record := range batch {
				if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
					log.Println("!!!!!!!!!!!!!!!!!!!! invalid event size !!!!!!!!!!!!!!!!!!!")
					continue
				}
				event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

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

				// Пропускаем события от нашего демона
				if pkg.Int8ToString(event.Comm) == executableName {
					continue
				}

				// Обработка Sysexit == 1 (sys_exit_sendto или sendmsg)
				if event.Sysexit == 1 {
					if event.Family == 2 {
						port := int(event.Dport)
						data, exists := eventMap1[port]
						if !exists {
							data = getEventData()
							eventMap1[port] = data
						}
						data.Sendmsg = Sendmsg{
							DstIP:   dstIP,
							DstPort: port,
							Pid:     event.Pid,
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasSendmsg = true

						if data.HasLookup && data.HasRecvmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							if data.Lookup.DstIP.IsLoopback() {
								dstHost = "localhost"
							} else {
								dstHost = resolveHost(data.Lookup.DstIP)
							}
							if data.Lookup.SrcIP.IsLoopback() {
								srcHost = "localhost"
							} else {
								srcHost = resolveHost(data.Lookup.SrcIP)
							}
							fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
								data.Sendmsg.Pid,
								data.Sendmsg.Comm,
								proto,
								dstHost,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								srcHost,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Printf("SENDTO PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
								data.Recvmsg.Pid,
								data.Recvmsg.Comm,
								proto,
								dstHost,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								srcHost,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							delete(eventMap1, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Dport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Обработка Sysexit == 11 (sys_exit_sendmsg)
				if event.Sysexit == 11 {
					if event.Family == 2 {
						port := int(event.Dport)
						data, exists := eventMap[port]
						if !exists {
							data = getEventData()
							eventMap[port] = data
						}
						data.Sendmsg = Sendmsg{
							DstIP:   dstIP,
							DstPort: port,
							Pid:     event.Pid,
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasSendmsg = true

						if data.HasLookup && data.HasRecvmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Println("")
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
							fmt.Println("")
							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Dport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("SENDMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Обработка Sysexit == 2 (sys_exit_recvfrom)
				if event.Sysexit == 2 {
					if event.Family == 2 {
						port := int(event.Sport)
						data, exists := eventMap[port]
						if !exists {
							data = getEventData()
							eventMap[port] = data
						}
						data.Recvmsg = Recvmsg{
							SrcIP:   srcIP,
							SrcPort: port,
							Pid:     event.Pid,
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasRecvmsg = true

						if data.HasLookup && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							if data.Lookup.DstIP.IsLoopback() {
								dstHost = "localhost"
							} else {
								dstHost = resolveHost(dstIP)
							}
							if data.Lookup.SrcIP.IsLoopback() {
								srcHost = "localhost"
							} else {
								srcHost = resolveHost(srcIP)
							}
							fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
								data.Sendmsg.Pid,
								data.Sendmsg.Comm,
								proto,
								dstHost,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								srcHost,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Printf("RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
								data.Recvmsg.Pid,
								data.Recvmsg.Comm,
								proto,
								dstHost,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								srcHost,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Sport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6,
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Обработка Sysexit == 12 (sys_exit_recvmsg)
				if event.Sysexit == 12 {
					if event.Family == 2 {
						port := int(event.Sport)
						data, exists := eventMap[port]
						if !exists {
							data = getEventData()
							eventMap[port] = data
						}
						data.Recvmsg = Recvmsg{
							SrcIP:   srcIP,
							SrcPort: port,
							Pid:     event.Pid,
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasRecvmsg = true

						if data.HasLookup && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Println("")
							fmt.Printf("%s/%s:%d->%s:%d\n",
								proto,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Printf("%s/%s:%d<-%s:%d\n",
								proto,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Println("")
							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Sport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("RECVMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// Обработка Sysexit == 3 (sk_lookup)
				if event.Sysexit == 3 {
					if event.Family == 2 {
						port := int(event.Dport)
						data, exists := eventMap[port]
						if !exists {
							data = getEventData()
							eventMap[port] = data
						}
						data.Lookup = Lookup{
							SrcIP:   srcIP,
							SrcPort: int(event.Sport),
							DstIP:   dstIP,
							DstPort: int(event.Dport),
							Proto:   int(event.Proto),
						}
						data.HasLookup = true

						port1 := int(event.Sport)
						data1, exists1 := eventMap1[port1]
						if !exists1 {
							data1 = data
							eventMap1[port1] = data1
						} else {
							data1.Lookup = data.Lookup
						}

						if data.HasRecvmsg && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Println("")
							if data.Lookup.DstIP.IsLoopback() {
								dstHost = "localhost"
							} else {
								dstHost = resolveHost(data.Lookup.DstIP)
							}
							if data.Lookup.SrcIP.IsLoopback() {
								srcHost = "localhost"
							} else {
								srcHost = resolveHost(data.Lookup.SrcIP)
							}
							fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
								data.Sendmsg.Pid,
								data.Sendmsg.Comm,
								proto,
								dstHost,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								srcHost,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Printf("LOOKUP PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
								data.Recvmsg.Pid,
								data.Recvmsg.Comm,
								proto,
								dstHost,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								srcHost,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Println("")
							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						ip6s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("LOOKUP SRC IPv6=%s\n", ip6s)
						ip6d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("LOOKUP DST IPv6=%s\n", ip6d)
						fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROТО=%d\n", event.Sport, event.Dport, event.Proto)
					}
				}

				// Обработка Sysexit == 6 (inet_sock_set_state)
				if event.Sysexit == 6 {
					if event.Family == 10 {
						sport := event.Sport
						dport := event.Dport
						pid := event.Pid
						ip6s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6s.String(),
							sport,
							pkg.Int8ToString(event.Comm),
						)
						ip6d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6d,
							dport,
							pkg.Int8ToString(event.Comm),
						)
						fmt.Printf("!!!!!TCP IPv6 !!!!!!PID=%d  NAME=%s\n",
							event.Pid,
							pkg.Int8ToString(event.Comm),
						)
					}

					if event.State == 1 {
						mu.Lock()
						select {
						case eventChanSport <- int(event.Sport):
						default:
							eventChanSport <- int(event.Sport)
							fmt.Printf("State 1: заменен порт %d\n", event.Sport)
						}
						mu.Unlock()

						if dstIP.IsLoopback() {
							dstHost = resolveHost(dstIP)
						} else {
							dstHost = resolveHost(dstIP)
						}

						srcHostResolved := resolveHost(srcIP)
						srcAddr := fmt.Sprintf("//%s[%s]:%d", srcHostResolved, srcIP.String(), event.Sport)
						dstAddr := fmt.Sprintf("//%s[%s]:%d", dstHost, dstIP.String(), event.Dport)

						if event.Proto == 6 {
							proto = "TCP"
						}

						fmt.Println("")
						fmt.Printf("PID=%d %s:%s <- %s:%s \n",
							event.Pid,
							proto,
							srcAddr,
							proto,
							dstAddr,
						)
					}

					if event.State == 2 {
						mu.Lock()
						select {
						case eventChanPID <- int(event.Pid):
						default:
						}
						mu.Unlock()
					}

					if event.State == 10 {
						mu.Lock()
						select {
						case eventChanPID <- int(event.Pid):
						default:
						}
						mu.Unlock()
					}

					select {
					case lastSport = <-eventChanSport:
						if dstIP.IsLoopback() {
							dstHost = resolveHost(dstIP)
						} else {
							dstHost = resolveHost(dstIP)
						}

						srcHostResolved2 := resolveHost(srcIP)
						srcAddr := fmt.Sprintf("//%s[%s]:%d", srcHostResolved2, srcIP.String(), lastSport)
						dstAddr := fmt.Sprintf("//%s[%s]:%d", dstHost, dstIP.String(), event.Dport)

						select {
						case lastPID = <-eventChanPID:
						default:
						}

						if event.Proto == 6 {
							proto = "TCP"
						}

						fmt.Printf("PID=%d %s:%s -> %s:%s \n",
							lastPID,
							proto,
							srcAddr,
							proto,
							dstAddr,
						)
						fmt.Println("")

					default:
						fmt.Println("")
					}
				}

			} // конец цикла обработки batch
		} // конец главного цикла
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

// Вспомогательная функция для IPv6
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


Type: cpu
Time: 2025-06-01 20:48:13 MSK
Duration: 30.01s, Total samples = 10ms (0.033%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 10ms, 100% of 10ms total
      flat  flat%   sum%        cum   cum%
      10ms   100%   100%       10ms   100%  syscall.RawSyscall6
         0     0%   100%       10ms   100%  github.com/cilium/ebpf/internal/epoll.(*Poller).Wait
         0     0%   100%       10ms   100%  github.com/cilium/ebpf/internal/unix.EpollWait (inline)
         0     0%   100%       10ms   100%  github.com/cilium/ebpf/perf.(*Reader).Read (inline)
         0     0%   100%       10ms   100%  github.com/cilium/ebpf/perf.(*Reader).ReadInto
         0     0%   100%       10ms   100%  golang.org/x/sys/unix.EpollWait
         0     0%   100%       10ms   100%  main.main.func2
         0     0%   100%       10ms   100%  syscall.Syscall6


  pprof) top
Showing nodes accounting for 20160.79kB, 100% of 20160.79kB total
Showing top 10 nodes out of 38
      flat  flat%   sum%        cum   cum%
 7458.81kB 37.00% 37.00% 12705.92kB 63.02%  github.com/cilium/ebpf/btf.readAndInflateTypes
 4100.97kB 20.34% 57.34%  4733.11kB 23.48%  github.com/cilium/ebpf/btf.readAndInflateTypes.func2
 3356.76kB 16.65% 73.99%  5916.82kB 29.35%  github.com/cilium/ebpf/btf.readStringTable
 2560.06kB 12.70% 86.69%  2560.06kB 12.70%  bufio.(*Scanner).Text (inline)
    1026kB  5.09% 91.78%     1026kB  5.09%  runtime.allocm
  632.14kB  3.14% 94.91%   632.14kB  3.14%  github.com/cilium/ebpf/btf.readAndInflateTypes.func1
     514kB  2.55% 97.46%      514kB  2.55%  slices.Grow[go.shape.[]github.com/cilium/ebpf/btf.btfEnum,go.shape.struct { NameOff uint32; Val uint32 }] (inline)
  512.05kB  2.54%   100%   512.05kB  2.54%  runtime.acquireSudog
         0     0%   100% 18110.68kB 89.83%  github.com/cilium/ebpf.(*CollectionSpec).LoadAndAssign
         0     0%   100% 18110.68kB 89.83%  github.com/cilium/ebpf.(*CollectionSpec).LoadAndAssign.func1
(pprof) 


(pprof) list github.com/cilium/ebpf/btf.readAndInflateTypes

File: bpfgo
Build ID: 691a8045ad0b7b61b208865d6487496b0de466bf
Type: inuse_space
Time: 2025-06-01 21:53:46 MSK
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) list github.com/cilium/ebpf/btf.readAndInflateTypes
Total: 20.09MB
ROUTINE ======================== github.com/cilium/ebpf/btf.readAndInflateTypes in /home/gaz358/go/pkg/mod/github.com/cilium/ebpf@v0.18.0/btf/types.go
    8.28MB    11.29MB (flat, cum) 56.19% of Total
         .          .    761:func readAndInflateTypes(r io.Reader, bo binary.ByteOrder, typeLen uint32, rawStrings *stringTable, base *Spec) ([]Type, error) {
         .          .    762:   // because of the interleaving between types and struct members it is difficult to
         .          .    763:   // precompute the numbers of raw types this will parse
         .          .    764:   // this "guess" is a good first estimation
         .          .    765:   sizeOfbtfType := uintptr(btfTypeLen)
         .          .    766:   tyMaxCount := uintptr(typeLen) / sizeOfbtfType / 2
    2.28MB     2.28MB    767:   types := make([]Type, 0, tyMaxCount)
         .          .    768:
         .          .    769:   // Void is defined to always be type ID 0, and is thus omitted from BTF.
         .          .    770:   types = append(types, (*Void)(nil))
         .          .    771:
         .          .    772:   firstTypeID := TypeID(0)
         .          .    773:   if base != nil {
         .          .    774:           var err error
         .          .    775:           firstTypeID, err = base.nextTypeID()
         .          .    776:           if err != nil {
         .          .    777:                   return nil, err
         .          .    778:           }
         .          .    779:
         .          .    780:           // Split BTF doesn't contain Void.
         .          .    781:           types = types[:0]
         .          .    782:   }
         .          .    783:
         .          .    784:   type fixupDef struct {
         .          .    785:           id  TypeID
         .          .    786:           typ *Type
         .          .    787:   }
         .          .    788:
         .          .    789:   var fixups []fixupDef
         .          .    790:   fixup := func(id TypeID, typ *Type) {
         .          .    791:           if id < firstTypeID {
         .          .    792:                   if baseType, err := base.TypeByID(id); err == nil {
         .          .    793:                           *typ = baseType
         .          .    794:                           return
         .          .    795:                   }
         .          .    796:           }
         .          .    797:
         .          .    798:           idx := int(id - firstTypeID)
         .          .    799:           if idx < len(types) {
         .          .    800:                   // We've already inflated this type, fix it up immediately.
         .          .    801:                   *typ = types[idx]
         .          .    802:                   return
         .          .    803:           }
         .          .    804:
         .          .    805:           fixups = append(fixups, fixupDef{id, typ})
         .          .    806:   }
         .          .    807:
         .          .    808:   type bitfieldFixupDef struct {
         .          .    809:           id TypeID
         .          .    810:           m  *Member
         .          .    811:   }
         .          .    812:
         .          .    813:   var (
         .          .    814:           legacyBitfields = make(map[TypeID][2]Bits) // offset, size
         .          .    815:           bitfieldFixups  []bitfieldFixupDef
         .          .    816:   )
         .          .    817:   convertMembers := func(raw []btfMember, kindFlag bool) ([]Member, error) {
         .          .    818:           // NB: The fixup below relies on pre-allocating this array to
         .          .    819:           // work, since otherwise append might re-allocate members.
         .          .    820:           members := make([]Member, 0, len(raw))
         .          .    821:           for i, btfMember := range raw {
         .          .    822:                   name, err := rawStrings.Lookup(btfMember.NameOff)
         .          .    823:                   if err != nil {
         .          .    824:                           return nil, fmt.Errorf("can't get name for member %d: %w", i, err)
         .          .    825:                   }
         .          .    826:
         .          .    827:                   members = append(members, Member{
         .          .    828:                           Name:   name,
         .          .    829:                           Offset: Bits(btfMember.Offset),
         .          .    830:                   })
         .          .    831:
         .          .    832:                   m := &members[i]
         .          .    833:                   fixup(raw[i].Type, &m.Type)
         .          .    834:
         .          .    835:                   if kindFlag {
         .          .    836:                           m.BitfieldSize = Bits(btfMember.Offset >> 24)
         .          .    837:                           m.Offset &= 0xffffff
         .          .    838:                           // We ignore legacy bitfield definitions if the current composite
         .          .    839:                           // is a new-style bitfield. This is kind of safe since offset and
         .          .    840:                           // size on the type of the member must be zero if kindFlat is set
         .          .    841:                           // according to spec.
         .          .    842:                           continue
         .          .    843:                   }
         .          .    844:
         .          .    845:                   // This may be a legacy bitfield, try to fix it up.
         .          .    846:                   data, ok := legacyBitfields[raw[i].Type]
         .          .    847:                   if ok {
         .          .    848:                           // Bingo!
         .          .    849:                           m.Offset += data[0]
         .          .    850:                           m.BitfieldSize = data[1]
         .          .    851:                           continue
         .          .    852:                   }
         .          .    853:
         .          .    854:                   if m.Type != nil {
         .          .    855:                           // We couldn't find a legacy bitfield, but we know that the member's
         .          .    856:                           // type has already been inflated. Hence we know that it can't be
         .          .    857:                           // a legacy bitfield and there is nothing left to do.
         .          .    858:                           continue
         .          .    859:                   }
         .          .    860:
         .          .    861:                   // We don't have fixup data, and the type we're pointing
         .          .    862:                   // at hasn't been inflated yet. No choice but to defer
         .          .    863:                   // the fixup.
         .          .    864:                   bitfieldFixups = append(bitfieldFixups, bitfieldFixupDef{
         .          .    865:                           raw[i].Type,
         .          .    866:                           m,
         .          .    867:                   })
         .          .    868:           }
         .          .    869:           return members, nil
         .          .    870:   }
         .          .    871:
         .          .    872:   var (
         .          .    873:           buf       = make([]byte, 1024)
         .          .    874:           header    btfType
         .          .    875:           bInt      btfInt
         .          .    876:           bArr      btfArray
         .          .    877:           bMembers  []btfMember
         .          .    878:           bEnums    []btfEnum
         .          .    879:           bParams   []btfParam
         .          .    880:           bVariable btfVariable
         .          .    881:           bSecInfos []btfVarSecinfo
         .          .    882:           bDeclTag  btfDeclTag
         .          .    883:           bEnums64  []btfEnum64
         .          .    884:   )
         .          .    885:
         .          .    886:   var declTags []*declTag
         .          .    887:   for {
         .          .    888:           var (
         .          .    889:                   id  = firstTypeID + TypeID(len(types))
         .          .    890:                   typ Type
         .          .    891:           )
         .          .    892:
         .          .    893:           if _, err := io.ReadFull(r, buf[:btfTypeLen]); err == io.EOF {
         .          .    894:                   break
         .          .    895:           } else if err != nil {
         .          .    896:                   return nil, fmt.Errorf("can't read type info for id %v: %v", id, err)
         .          .    897:           }
         .          .    898:
         .          .    899:           if _, err := unmarshalBtfType(&header, buf[:btfTypeLen], bo); err != nil {
         .          .    900:                   return nil, fmt.Errorf("can't unmarshal type info for id %v: %v", id, err)
         .          .    901:           }
         .          .    902:
         .          .    903:           if id < firstTypeID {
         .          .    904:                   return nil, fmt.Errorf("no more type IDs")
         .          .    905:           }
         .          .    906:
         .          .    907:           name, err := rawStrings.Lookup(header.NameOff)
         .          .    908:           if err != nil {
         .          .    909:                   return nil, fmt.Errorf("get name for type id %d: %w", id, err)
         .          .    910:           }
         .          .    911:
         .          .    912:           switch header.Kind() {
         .          .    913:           case kindInt:
         .          .    914:                   size := header.Size()
         .          .    915:                   buf = buf[:btfIntLen]
         .          .    916:                   if _, err := io.ReadFull(r, buf); err != nil {
         .          .    917:                           return nil, fmt.Errorf("can't read btfInt, id: %d: %w", id, err)
         .          .    918:                   }
         .          .    919:                   if _, err := unmarshalBtfInt(&bInt, buf, bo); err != nil {
         .          .    920:                           return nil, fmt.Errorf("can't unmarshal btfInt, id: %d: %w", id, err)
         .          .    921:                   }
         .          .    922:                   if bInt.Offset() > 0 || bInt.Bits().Bytes() != size {
         .          .    923:                           legacyBitfields[id] = [2]Bits{bInt.Offset(), bInt.Bits()}
         .          .    924:                   }
         .          .    925:                   typ = &Int{name, header.Size(), bInt.Encoding()}
         .          .    926:
         .          .    927:           case kindPointer:
  512.01kB   512.01kB    928:                   ptr := &Pointer{nil}
         .          .    929:                   fixup(header.Type(), &ptr.Target)
         .          .    930:                   typ = ptr
         .          .    931:
         .          .    932:           case kindArray:
         .          .    933:                   buf = buf[:btfArrayLen]
         .          .    934:                   if _, err := io.ReadFull(r, buf); err != nil {
         .          .    935:                           return nil, fmt.Errorf("can't read btfArray, id: %d: %w", id, err)
         .          .    936:                   }
         .          .    937:                   if _, err := unmarshalBtfArray(&bArr, buf, bo); err != nil {
         .          .    938:                           return nil, fmt.Errorf("can't unmarshal btfArray, id: %d: %w", id, err)
         .          .    939:                   }
         .          .    940:
         .          .    941:                   arr := &Array{nil, nil, bArr.Nelems}
         .          .    942:                   fixup(bArr.IndexType, &arr.Index)
         .          .    943:                   fixup(bArr.Type, &arr.Type)
         .          .    944:                   typ = arr
         .          .    945:
         .          .    946:           case kindStruct:
         .          .    947:                   vlen := header.Vlen()
         .          .    948:                   bMembers = slices.Grow(bMembers[:0], vlen)[:vlen]
         .          .    949:                   buf = slices.Grow(buf[:0], vlen*btfMemberLen)[:vlen*btfMemberLen]
         .          .    950:                   if _, err := io.ReadFull(r, buf); err != nil {
         .          .    951:                           return nil, fmt.Errorf("can't read btfMembers, id: %d: %w", id, err)
         .          .    952:                   }
         .          .    953:                   if _, err := unmarshalBtfMembers(bMembers, buf, bo); err != nil {
         .          .    954:                           return nil, fmt.Errorf("can't unmarshal btfMembers, id: %d: %w", id, err)
         .          .    955:                   }
         .          .    956:
         .        2MB    957:                   members, err := convertMembers(bMembers, header.Bitfield())
         .          .    958:                   if err != nil {
         .          .    959:                           return nil, fmt.Errorf("struct %s (id %d): %w", name, id, err)
         .          .    960:                   }
       1MB        1MB    961:                   typ = &Struct{name, header.Size(), members, nil}
         .          .    962:
         .          .    963:           case kindUnion:
         .          .    964:                   vlen := header.Vlen()
         .          .    965:                   bMembers = slices.Grow(bMembers[:0], vlen)[:vlen]
         .          .    966:                   buf = slices.Grow(buf[:0], vlen*btfMemberLen)[:vlen*btfMemberLen]
         .          .    967:                   if _, err := io.ReadFull(r, buf); err != nil {
         .          .    968:                           return nil, fmt.Errorf("can't read btfMembers, id: %d: %w", id, err)
         .          .    969:                   }
         .          .    970:                   if _, err := unmarshalBtfMembers(bMembers, buf, bo); err != nil {
         .          .    971:                           return nil, fmt.Errorf("can't unmarshal btfMembers, id: %d: %w", id, err)
         .          .    972:                   }
         .          .    973:
         .        1MB    974:                   members, err := convertMembers(bMembers, header.Bitfield())
         .          .    975:                   if err != nil {
         .          .    976:                           return nil, fmt.Errorf("union %s (id %d): %w", name, id, err)
         .          .    977:                   }
  512.04kB   512.04kB    978:                   typ = &Union{name, header.Size(), members, nil}
         .          .    979:
         .          .    980:           case kindEnum:
         .          .    981:                   vlen := header.Vlen()
         .          .    982:                   bEnums = slices.Grow(bEnums[:0], vlen)[:vlen]
         .          .    983:                   buf = slices.Grow(buf[:0], vlen*btfEnumLen)[:vlen*btfEnumLen]
         .          .    984:                   if _, err := io.ReadFull(r, buf); err != nil {
         .          .    985:                           return nil, fmt.Errorf("can't read btfEnums, id: %d: %w", id, err)
         .          .    986:                   }
         .          .    987:                   if _, err := unmarshalBtfEnums(bEnums, buf, bo); err != nil {
         .          .    988:                           return nil, fmt.Errorf("can't unmarshal btfEnums, id: %d: %w", id, err)
         .          .    989:                   }
         .          .    990:
       1MB        1MB    991:                   vals := make([]EnumValue, 0, vlen)
         .          .    992:                   signed := header.Signed()
         .          .    993:                   for i, btfVal := range bEnums {
         .          .    994:                           name, err := rawStrings.Lookup(btfVal.NameOff)
         .          .    995:                           if err != nil {
         .          .    996:                                   return nil, fmt.Errorf("get name for enum value %d: %s", i, err)
         .          .    997:                           }
         .          .    998:                           value := uint64(btfVal.Val)
         .          .    999:                           if signed {
         .          .   1000:                                   // Sign extend values to 64 bit.
         .          .   1001:                                   value = uint64(int32(btfVal.Val))
         .          .   1002:                           }
         .          .   1003:                           vals = append(vals, EnumValue{name, value})
         .          .   1004:                   }
  512.02kB   512.02kB   1005:                   typ = &Enum{name, header.Size(), signed, vals}
         .          .   1006:
         .          .   1007:           case kindForward:
         .          .   1008:                   typ = &Fwd{name, header.FwdKind()}
         .          .   1009:
         .          .   1010:           case kindTypedef:
         .          .   1011:                   typedef := &Typedef{name, nil, nil}
         .          .   1012:                   fixup(header.Type(), &typedef.Type)
         .          .   1013:                   typ = typedef
         .          .   1014:
         .          .   1015:           case kindVolatile:
         .          .   1016:                   volatile := &Volatile{nil}
         .          .   1017:                   fixup(header.Type(), &volatile.Type)
         .          .   1018:                   typ = volatile
         .          .   1019:
         .          .   1020:           case kindConst:
         .          .   1021:                   cnst := &Const{nil}
         .          .   1022:                   fixup(header.Type(), &cnst.Type)
         .          .   1023:                   typ = cnst
         .          .   1024:
         .          .   1025:           case kindRestrict:
         .          .   1026:                   restrict := &Restrict{nil}
         .          .   1027:                   fixup(header.Type(), &restrict.Type)
         .          .   1028:                   typ = restrict
         .          .   1029:
         .          .   1030:           case kindFunc:
  512.05kB   512.05kB   1031:                   fn := &Func{name, nil, header.Linkage(), nil, nil}
         .          .   1032:                   fixup(header.Type(), &fn.Type)
         .          .   1033:                   typ = fn
         .          .   1034:
         .          .   1035:           case kindFuncProto:
         .          .   1036:                   vlen := header.Vlen()
         .          .   1037:                   bParams = slices.Grow(bParams[:0], vlen)[:vlen]
         .          .   1038:                   buf = slices.Grow(buf[:0], vlen*btfParamLen)[:vlen*btfParamLen]
         .          .   1039:                   if _, err := io.ReadFull(r, buf); err != nil {
         .          .   1040:                           return nil, fmt.Errorf("can't read btfParams, id: %d: %w", id, err)
         .          .   1041:                   }
         .          .   1042:                   if _, err := unmarshalBtfParams(bParams, buf, bo); err != nil {
         .          .   1043:                           return nil, fmt.Errorf("can't unmarshal btfParams, id: %d: %w", id, err)
         .          .   1044:                   }
         .          .   1045:
       1MB        1MB   1046:                   params := make([]FuncParam, 0, vlen)
         .          .   1047:                   for i, param := range bParams {
         .          .   1048:                           name, err := rawStrings.Lookup(param.NameOff)
         .          .   1049:                           if err != nil {
         .          .   1050:                                   return nil, fmt.Errorf("get name for func proto parameter %d: %s", i, err)
         .          .   1051:                           }
         .          .   1052:                           params = append(params, FuncParam{
         .          .   1053:                                   Name: name,
         .          .   1054:                           })
         .          .   1055:                   }
         .          .   1056:                   for i := range params {
         .          .   1057:                           fixup(bParams[i].Type, &params[i].Type)
         .          .   1058:                   }
         .          .   1059:
       1MB        1MB   1060:                   fp := &FuncProto{nil, params}
         .          .   1061:                   fixup(header.Type(), &fp.Return)
         .          .   1062:                   typ = fp
         .          .   1063:
         .          .   1064:           case kindVar:
         .          .   1065:                   buf = buf[:btfVariableLen]
ROUTINE ======================== github.com/cilium/ebpf/btf.readAndInflateTypes.func2 in /home/gaz358/go/pkg/mod/github.com/cilium/ebpf@v0.18.0/btf/types.go
       3MB        3MB (flat, cum) 14.95% of Total
         .          .    817:   convertMembers := func(raw []btfMember, kindFlag bool) ([]Member, error) {
         .          .    818:           // NB: The fixup below relies on pre-allocating this array to
         .          .    819:           // work, since otherwise append might re-allocate members.
       3MB        3MB    820:           members := make([]Member, 0, len(raw))
         .          .    821:           for i, btfMember := range raw {
         .          .    822:                   name, err := rawStrings.Lookup(btfMember.NameOff)
         .          .    823:                   if err != nil {
         .          .    824:                           return nil, fmt.Errorf("can't get name for member %d: %w", i, err)
         .          .    825:                   }
(pprof) 


