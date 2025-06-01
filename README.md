
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




