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
	"strconv"
	"strings"
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
	*ed = EventData{}
	eventDataPool.Put(ed)
}

var (
	resolveCache = make(map[string]string)
	cacheMu      sync.RWMutex

	// Кеш для конвертации [32]int8 в string
	commCache = make(map[[32]int8]string)
	commMu    sync.RWMutex
)

// cachedComm возвращает имя процесса из кеша или конвертирует и сохраняет
func cachedComm(c [32]int8) string {
	commMu.RLock()
	if s, ok := commCache[c]; ok {
		commMu.RUnlock()
		return s
	}
	commMu.RUnlock()

	s := pkg.Int8ToString(c)

	commMu.Lock()
	commCache[c] = s
	commMu.Unlock()
	return s
}

func resolveHost(ip net.IP) string {
	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.To4() != nil {
		host = pkg.ResolveIP(ip)
	} else {
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

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit for process: %v", err)
	}
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
		const buffLen = 1024
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
		batch := make([]perf.Record, 0, batchSize)

		for {
			// Собираем до batchSize записей в слайс
			batch = batch[:0] // обнуляем длину, оставляем capacity = batchSize
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
				if cachedComm(event.Comm) == executableName {
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
							Comm:    cachedComm(event.Comm),
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
							fmt.Print("SENDTO PID=", data.Sendmsg.Pid, " NAME=", data.Sendmsg.Comm, " ", proto, "/",
								dstHost, "[", data.Lookup.DstIP.String(), "]", ":", data.Lookup.DstPort, " -> ",
								srcHost, "[", data.Lookup.SrcIP.String(), "]", ":", data.Lookup.SrcPort, "\n")

							fmt.Print("SENDTO PID=", data.Recvmsg.Pid, " NAME=", data.Recvmsg.Comm, " ", proto, "/",
								dstHost, "[", data.Lookup.DstIP.String(), "]", ":", data.Lookup.DstPort, " <- ",
								srcHost, "[", data.Lookup.SrcIP.String(), "]", ":", data.Lookup.SrcPort, "\n")

							delete(eventMap1, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Dport
						pid := event.Pid

						var ip6Buf = make(net.IP, net.IPv6len)
						words := IPv6BytesToWords(event.DstIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6Buf[i*4+0] = byte(words[i])
							ip6Buf[i*4+1] = byte(words[i] >> 8)
							ip6Buf[i*4+2] = byte(words[i] >> 16)
							ip6Buf[i*4+3] = byte(words[i] >> 24)
						}
						fmt.Print("SENDTO IPv6 PID=", pid, " IPv6=", ip6Buf.String(), ":", port, " NAME=", cachedComm(event.Comm), "\n")
					}
				}

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
							Comm:    cachedComm(event.Comm),
						}
						data.HasSendmsg = true

						if data.HasLookup && data.HasRecvmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Print("\n")
							fmt.Print("SENDMSG PID=", data.Sendmsg.Pid, " NAME=", data.Sendmsg.Comm, " ", proto, "/",
								data.Lookup.DstIP.String(), ":", data.Lookup.DstPort, "->", data.Lookup.SrcIP.String(), ":", data.Lookup.SrcPort, "\n")

							fmt.Print("SENDMSG PID=", data.Recvmsg.Pid, " NAME=", data.Recvmsg.Comm, " ", proto, "/",
								data.Lookup.DstIP.String(), ":", data.Lookup.DstPort, "<-", data.Lookup.SrcIP.String(), ":", data.Lookup.SrcPort, "\n")
							fmt.Print("\n")

							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Dport
						pid := event.Pid

						var ip6Buf = make(net.IP, net.IPv6len)
						words := IPv6BytesToWords(event.DstIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6Buf[i*4+0] = byte(words[i])
							ip6Buf[i*4+1] = byte(words[i] >> 8)
							ip6Buf[i*4+2] = byte(words[i] >> 16)
							ip6Buf[i*4+3] = byte(words[i] >> 24)
						}
						fmt.Print("SENDMSG IPv6 PID=", pid, " IPv6=", ip6Buf.String(), ":", port, " NAME=", cachedComm(event.Comm), "\n")
					}
				}

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
							Comm:    cachedComm(event.Comm),
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
							fmt.Print("RECVFROM PID=", data.Sendmsg.Pid, " NAME=", data.Sendmsg.Comm, " ", proto, "/",
								dstHost, "[", data.Lookup.DstIP.String(), "]", ":", data.Lookup.DstPort, " -> ",
								srcHost, "[", data.Lookup.SrcIP.String(), "]", ":", data.Lookup.SrcPort, "\n")

							fmt.Print("RECVFROM PID=", data.Recvmsg.Pid, " NAME=", data.Recvmsg.Comm, " ", proto, "/",
								dstHost, "[", data.Lookup.DstIP.String(), "]", ":", data.Lookup.DstPort, " <- ",
								srcHost, "[", data.Lookup.SrcIP.String(), "]", ":", data.Lookup.SrcPort, "\n")

							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Sport
						pid := event.Pid

						var ip6Buf = make(net.IP, net.IPv6len)
						words := IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6Buf[i*4+0] = byte(words[i])
							ip6Buf[i*4+1] = byte(words[i] >> 8)
							ip6Buf[i*4+2] = byte(words[i] >> 16)
							ip6Buf[i*4+3] = byte(words[i] >> 24)
						}
						fmt.Print("RECVFROM IPv6 PID=", pid, " IPv6=", ip6Buf.String(), ":", port, " NAME=", cachedComm(event.Comm), "\n")
					}
				}

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
							Comm:    cachedComm(event.Comm),
						}
						data.HasRecvmsg = true

						if data.HasLookup && data.HasSendmsg {
							if data.Lookup.Proto == 17 {
								proto = "UDP"
							}
							fmt.Print("\n")
							fmt.Print(proto, "/", data.Lookup.DstIP.String(), ":", data.Lookup.DstPort, "->", data.Lookup.SrcIP.String(), ":", data.Lookup.SrcPort, "\n")
							fmt.Print(proto, "/", data.Lookup.DstIP.String(), ":", data.Lookup.DstPort, "<-", data.Lookup.SrcIP.String(), ":", data.Lookup.SrcPort, "\n")
							fmt.Print("\n")
							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						port := event.Sport
						pid := event.Pid

						var ip6Buf = make(net.IP, net.IPv6len)
						words := IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6Buf[i*4+0] = byte(words[i])
							ip6Buf[i*4+1] = byte(words[i] >> 8)
							ip6Buf[i*4+2] = byte(words[i] >> 16)
							ip6Buf[i*4+3] = byte(words[i] >> 24)
						}
						fmt.Print("RECVMSG IPv6 PID=", pid, " IPv6=", ip6Buf.String(), ":", port, " NAME=", cachedComm(event.Comm), "\n")
					}
				}

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
							fmt.Print("\n")
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
							fmt.Print("LOOKUP PID=", data.Sendmsg.Pid, " NAME=", data.Sendmsg.Comm, " ", proto, "/",
								dstHost, "[", data.Lookup.DstIP.String(), "]", ":", data.Lookup.DstPort, "<-",
								srcHost, "[", data.Lookup.SrcIP.String(), "]", ":", data.Lookup.SrcPort, "\n")
							fmt.Print("LOOKUP PID=", data.Recvmsg.Pid, " NAME=", data.Recvmsg.Comm, " ", proto, "/",
								dstHost, "[", data.Lookup.DstIP.String(), "]", ":", data.Lookup.DstPort, "->",
								srcHost, "[", data.Lookup.SrcIP.String(), "]", ":", data.Lookup.SrcPort, "\n")
							fmt.Print("\n")
							delete(eventMap, port)
							putEventData(data)
						}
					} else if event.Family == 10 {
						var ip6Buf1 = make(net.IP, net.IPv6len)
						words1 := IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6Buf1[i*4+0] = byte(words1[i])
							ip6Buf1[i*4+1] = byte(words1[i] >> 8)
							ip6Buf1[i*4+2] = byte(words1[i] >> 16)
							ip6Buf1[i*4+3] = byte(words1[i] >> 24)
						}
						fmt.Print("LOOKUP SRC IPv6=", ip6Buf1.String(), "\n")

						var ip6Buf2 = make(net.IP, net.IPv6len)
						words2 := IPv6BytesToWords(event.DstIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6Buf2[i*4+0] = byte(words2[i])
							ip6Buf2[i*4+1] = byte(words2[i] >> 8)
							ip6Buf2[i*4+2] = byte(words2[i] >> 16)
							ip6Buf2[i*4+3] = byte(words2[i] >> 24)
						}
						fmt.Print("LOOKUP DST IPv6=", ip6Buf2.String(), "\n")
						fmt.Print("LOOKUP SPORT=", event.Sport, "  DPORT=", event.Dport, " PROТО=", event.Proto, "\n")
					}
				}

				if event.Sysexit == 6 {
					if event.Family == 10 {
						sport := event.Sport
						dport := event.Dport
						pid := event.Pid

						var ip6BufS = make(net.IP, net.IPv6len)
						ws := IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6BufS[i*4+0] = byte(ws[i])
							ip6BufS[i*4+1] = byte(ws[i] >> 8)
							ip6BufS[i*4+2] = byte(ws[i] >> 16)
							ip6BufS[i*4+3] = byte(ws[i] >> 24)
						}
						fmt.Print("TCP IPv6 PID=", pid, " IPv6=", ip6BufS.String(), ":", sport, " NAME=", cachedComm(event.Comm), "\n")

						var ip6BufD = make(net.IP, net.IPv6len)
						wd := IPv6BytesToWords(event.DstIP6.In6U.U6Addr8)
						for i := 0; i < 4; i++ {
							ip6BufD[i*4+0] = byte(wd[i])
							ip6BufD[i*4+1] = byte(wd[i] >> 8)
							ip6BufD[i*4+2] = byte(wd[i] >> 16)
							ip6BufD[i*4+3] = byte(wd[i] >> 24)
						}
						fmt.Print("TCP IPv6 PID=", pid, " IPv6=", ip6BufD.String(), ":", dport, " NAME=", cachedComm(event.Comm), "\n")

						fmt.Print("!!!!!TCP IPv6 !!!!!!PID=", event.Pid, "  NAME=", cachedComm(event.Comm), "\n")
					}

					if event.State == 1 {
						mu.Lock()
						select {
						case eventChanSport <- int(event.Sport):
						default:
							eventChanSport <- int(event.Sport)
							fmt.Print("State 1: заменен порт ", event.Sport, "\n")
						}
						mu.Unlock()

						dstHost = resolveHost(dstIP)
						srcHostResolved := resolveHost(srcIP)

						var sb strings.Builder
						sb.Grow(2 + len(srcHostResolved) + len(srcIP.String()) + 20)
						sb.WriteString("//")
						sb.WriteString(srcHostResolved)
						sb.WriteByte('[')
						sb.WriteString(srcIP.String())
						sb.WriteString("]:")
						sb.WriteString(strconv.Itoa(int(event.Sport)))
						srcAddr := sb.String()

						sb.Reset()
						sb.Grow(2 + len(dstHost) + len(dstIP.String()) + 20)
						sb.WriteString("//")
						sb.WriteString(dstHost)
						sb.WriteByte('[')
						sb.WriteString(dstIP.String())
						sb.WriteString("]:")
						sb.WriteString(strconv.Itoa(int(event.Dport)))
						dstAddr := sb.String()

						if event.Proto == 6 {
							proto = "TCP"
						}

						fmt.Print("\n")
						fmt.Print("PID=", event.Pid, " ", proto, ":", srcAddr, " <- ", proto, ":", dstAddr, " \n")
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
						dstHost = resolveHost(dstIP)
						srcHostResolved2 := resolveHost(srcIP)

						var sb1 strings.Builder
						sb1.Grow(2 + len(srcHostResolved2) + len(srcIP.String()) + 20)
						sb1.WriteString("//")
						sb1.WriteString(srcHostResolved2)
						sb1.WriteByte('[')
						sb1.WriteString(srcIP.String())
						sb1.WriteString("]:")
						sb1.WriteString(strconv.Itoa(int(lastSport)))
						srcAddr := sb1.String()

						sb1.Reset()
						sb1.Grow(2 + len(dstHost) + len(dstIP.String()) + 20)
						sb1.WriteString("//")
						sb1.WriteString(dstHost)
						sb1.WriteByte('[')
						sb1.WriteString(dstIP.String())
						sb1.WriteString("]:")
						sb1.WriteString(strconv.Itoa(int(event.Dport)))
						dstAddr := sb1.String()

						select {
						case lastPID = <-eventChanPID:
						default:
						}

						if event.Proto == 6 {
							proto = "TCP"
						}

						fmt.Print("PID=", lastPID, " ", proto, ":", srcAddr, " -> ", proto, ":", dstAddr, " \n")
						fmt.Print("\n")

					default:
						fmt.Print("\n")
					}
				}

			} // конец цикла обработки batch
		} // конец главного цикла
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

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
