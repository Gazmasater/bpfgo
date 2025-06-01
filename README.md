
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


"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux







package main

import (
	"bpfgo/pkg"
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"time"
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

// ---------------------------------------------------------------------------
// Добавлен DNS-кэш
// ---------------------------------------------------------------------------

// cachedDNSRecord хранит хостнейм и время последнего обновления
type cachedDNSRecord struct {
	host        string
	lastUpdated time.Time
}

// TTL для кэшированных записей DNS (например, 5 минут)
const dnsCacheTTL = 5 * time.Minute

// dnsCache хранит кэш: ключ — строковое представление IP, значение — cachedDNSRecord
var dnsCache = struct {
	sync.RWMutex
	entries map[string]cachedDNSRecord
}{
	entries: make(map[string]cachedDNSRecord),
}

// resolveIPWithCache возвращает хостнейм для заданного IP, используя кэш.
// Если запись отсутствует или устарела, выполняется реальный DNS-запрос через pkg.ResolveIP или pkg.ResolveIP_n.
func resolveIPWithCache(ip net.IP) string {
	ipStr := ip.String()

	// Сначала пробуем получить из кэша
	dnsCache.RLock()
	if entry, exists := dnsCache.entries[ipStr]; exists {
		if time.Since(entry.lastUpdated) < dnsCacheTTL {
			dnsCache.RUnlock()
			return entry.host
		}
	}
	dnsCache.RUnlock()

	// Если в кэше нет или запись устарела — обновляем
	var host string
	if ip.To4() != nil {
		// IPv4: используем pkg.ResolveIP (без ошибки)
		host = pkg.ResolveIP(ip)
	} else {
		// IPv6: используем pkg.ResolveIP_n, которая возвращает (string, error)
		h, err := pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		} else {
			host = h
		}
	}

	// Сохраняем в кэш
	dnsCache.Lock()
	dnsCache.entries[ipStr] = cachedDNSRecord{
		host:        host,
		lastUpdated: time.Now(),
	}
	dnsCache.Unlock()

	return host
}

// ---------------------------------------------------------------------------
// Конец добавленного DNS-кэша
// ---------------------------------------------------------------------------

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
	// Запуск pprof-сервера
	go func() {
		log.Println("Starting pprof HTTP server on :6060")
		if err := http.ListenAndServe(":6060", nil); err != nil {
			log.Fatalf("pprof ListenAndServe error: %v", err)
		}
	}()

	eventMap := make(map[int]*EventData)
	eventMap_1 := make(map[int]*EventData)
	defer objs.Close()

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", netns.Fd())
	fmt.Printf("Go sizeof(traceInfo) = %d\n", unsafe.Sizeof(bpfTraceInfo{}))

	// Привязка tracepoint-ов
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
		const buffLen = 4096 * 2
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("failed to create perf reader: %s", err)
		}
		defer rd.Close()

		executableName := os.Args[0]
		if len(executableName) > 2 {
			executableName = executableName[2:]
		}

		const batchSize = 4

		for {
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
					log.Println("!!!!!!!!!!!!!!!!!!!!!!!invalid event size!!!!!!!!!!!!!!!!!!")
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

				// Пропускаем события от нашего демона, чтобы не заваливать логи
				if pkg.Int8ToString(event.Comm) == executableName {
					continue
				}

				// ---------------------------
				// Sysexit == 2: sys_exit_recvfrom
				// ---------------------------
				if event.Sysexit == 2 {
					if event.Family == 2 {
						port := int(event.Sport)
						data, exists := eventMap[port]
						if !exists {
							// Если объекта ещё нет, создаём новый
							data = &EventData{}
							eventMap[port] = data
						}
						data.Recvmsg = Recvmsg{
							SrcIP:   srcIP,
							SrcPort: port,
							Pid:     event.Pid,
							Proto:   int(event.Proto),
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasRecvmsg = true

						// Если lookup и sendmsg уже есть, печатаем сразу
						if data.HasLookup && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							if data.Lookup.DstIP.IsLoopback() {
								dsthost = "localhost"
							} else {
								dsthost = resolveIPWithCache(dstIP)
							}
							if data.Lookup.SrcIP.IsLoopback() {
								srchost = "localhost"
							} else {
								srchost = resolveIPWithCache(srcIP)
							}
							fmt.Printf(
								"RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
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
							fmt.Printf(
								"RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
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
						port := event.Sport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf(
							"RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// ---------------------------
				// Sysexit == 3: sk_lookup
				// ---------------------------
				if event.Sysexit == 3 {
					family := event.Family
					if family == 2 {
						dstPort := int(event.Dport)
						srcPort := int(event.Sport)

						// Создаём один общий объект для обеих карт
						data := &EventData{}
						data.Lookup = Lookup{
							SrcIP:   srcIP,
							SrcPort: srcPort,
							DstIP:   dstIP,
							DstPort: dstPort,
							Proto:   int(event.Proto),
						}
						data.HasLookup = true

						// Сохраняем одну и ту же ссылку в двух картах
						eventMap[dstPort] = data   // затем recvfrom смотрит сюда
						eventMap_1[srcPort] = data // sendto смотрит сюда

						// Если recvfrom и sendmsg уже были, можно сразу распечатать
						if data.HasRecvmsg && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							if data.Lookup.DstIP.IsLoopback() {
								dsthost = "localhost"
							} else {
								dsthost = resolveIPWithCache(data.Lookup.DstIP)
							}
							if data.Lookup.SrcIP.IsLoopback() {
								srchost = "localhost"
							} else {
								srchost = resolveIPWithCache(data.Lookup.SrcIP)
							}
							fmt.Println("")
							fmt.Printf(
								"LOOKUP PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
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
							fmt.Printf(
								"LOOKUP PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
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
							fmt.Println("")
							delete(eventMap, dstPort)
						}
					} else if family == 10 {
						ip6_s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf("LOOKUP SRC IPv6=%s\n", ip6_s)
						ip6_d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf("LOOKUP DST IPv6=%s\n", ip6_d)
						fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROТО=%d\n", event.Sport, event.Dport, event.Proto)
					}
				}

				// ---------------------------
				// Sysexit == 1: sys_exit_sendto
				// ---------------------------
				if event.Sysexit == 1 {
					if event.Family == 2 {
						port := int(event.Dport)
						data, exists := eventMap_1[port]
						if !exists {
							data = &EventData{}
							eventMap_1[port] = data
						}
						data.Sendmsg = Sendmsg{
							DstIP:   dstIP,
							DstPort: port,
							Pid:     event.Pid,
							Proto:   int(event.Proto),
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasSendmsg = true

						if data.HasLookup && data.HasRecvmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							if data.Lookup.DstIP.IsLoopback() {
								dsthost = "localhost"
							} else {
								dsthost = resolveIPWithCache(data.Lookup.DstIP)
							}
							if data.Lookup.SrcIP.IsLoopback() {
								srchost = "localhost"
							} else {
								srchost = resolveIPWithCache(data.Lookup.SrcIP)
							}
							fmt.Printf(
								"SENDTO PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
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
							fmt.Printf(
								"SENDTO PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
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
							delete(eventMap_1, port)
						}
					} else if event.Family == 10 {
						port := event.Dport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf(
							"SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// ---------------------------
				// Sysexit == 11: sys_exit_sendmsg
				// ---------------------------
				if event.Sysexit == 11 {
					if event.Family == 2 {
						port := int(event.Dport)
						data, exists := eventMap[port]
						if !exists {
							data = &EventData{}
							eventMap[port] = data
						}
						data.Sendmsg = Sendmsg{
							DstIP:   dstIP,
							DstPort: port,
							Pid:     event.Pid,
							Proto:   int(event.Proto),
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasSendmsg = true

						if data.HasLookup && data.HasRecvmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Println("")
							fmt.Printf(
								"SENDMSG PID=%d NAME=%s %s/%s:%d->%s:%d\n",
								data.Sendmsg.Pid,
								data.Sendmsg.Comm,
								proto,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Printf(
								"SENDMSG PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
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
						}
					} else if event.Family == 10 {
						port := event.Dport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf(
							"SENDMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// ---------------------------
				// Sysexit == 12: sys_exit_recvmsg
				// ---------------------------
				if event.Sysexit == 12 {
					if event.Family == 2 {
						port := int(event.Sport)
						data, exists := eventMap[port]
						if !exists {
							data = &EventData{}
							eventMap[port] = data
						}
						data.Recvmsg = Recvmsg{
							SrcIP:   srcIP,
							SrcPort: port,
							Pid:     event.Pid,
							Proto:   int(event.Proto),
							Comm:    pkg.Int8ToString(event.Comm),
						}
						data.HasRecvmsg = true

						if data.HasLookup && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Println("")
							fmt.Printf(
								"%s/%s:%d->%s:%d\n",
								proto,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Printf(
								"%s/%s:%d<-%s:%d\n",
								proto,
								data.Lookup.DstIP,
								data.Lookup.DstPort,
								data.Lookup.SrcIP,
								data.Lookup.SrcPort,
							)
							fmt.Println("")
							delete(eventMap, port)
						}
					} else if event.Family == 10 {
						port := event.Sport
						pid := event.Pid
						ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf(
							"RECVMSG IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6.String(),
							port,
							pkg.Int8ToString(event.Comm),
						)
					}
				}

				// ---------------------------
				// Sysexit == 6: inet_sock_set_state
				// ---------------------------
				if event.Sysexit == 6 {
					if event.Family == 10 {
						sport := event.Sport
						dport := event.Dport
						pid := event.Pid
						ip6_s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
						fmt.Printf(
							"TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6_s.String(),
							sport,
							pkg.Int8ToString(event.Comm),
						)
						ip6_d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
						fmt.Printf(
							"TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
							pid,
							ip6_d,
							dport,
							pkg.Int8ToString(event.Comm),
						)
						fmt.Printf(
							"!!!!!TCP IPv6 !!!!!!PID=%d  NAME=%s\n",
							event.Pid,
							pkg.Int8ToString(event.Comm),
						)
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
							dsthost = "localhost"
						} else {
							dsthost = resolveIPWithCache(dstIP)
						}

						srchost := resolveIPWithCache(srcIP)
						srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
						dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

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
						case eventChan_pid <- int(event.Pid):
						default:
						}
						mu.Unlock()
					}

					if event.State == 10 {
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
							dsthost = resolveIPWithCache(dstIP)
						} else {
							dsthost = resolveIPWithCache(dstIP)
						}

						srchost := resolveIPWithCache(srcIP)
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
							xxx_pid,
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



 10ms 33.33% 33.33%       10ms 33.33%  runtime.(*mcache).prepareForSweep
      10ms 33.33% 66.67%       10ms 33.33%  runtime.entersyscall
      10ms 33.33%   100%       10ms 33.33%  runtime.futex
         0     0%   100%       10ms 33.33%  fmt.Fprintln
         0     0%   100%       10ms 33.33%  fmt.Println
         0     0%   100%       20ms 66.67%  github.com/cilium/ebpf/internal/epoll.(*Poller).Wait
         0     0%   100%       20ms 66.67%  github.com/cilium/ebpf/internal/unix.EpollWait (inline)
         0     0%   100%       20ms 66.67%  github.com/cilium/ebpf/perf.(*Reader).Read
         0     0%   100%       20ms 66.67%  github.com/cilium/ebpf/perf.(*Reader).ReadInto
         0     0%   100%       20ms 66.67%  golang.org/x/sys/unix.EpollWait
(pprof) 

