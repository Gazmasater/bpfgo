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

	Comm string
}

type EventData struct {
	Lookup  *Lookup
	Sendmsg *Sendmsg
	Recvmsg *Recvmsg
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
	eventMap := make(map[int]*EventData)
	eventMap_1 := make(map[int]*EventData)

	defer objs.Close() // Закроем объекты при выходе

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", netns.Fd())

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

	// Netif_recieve, err := link.Tracepoint("net", "netif_receive_skb", objs.TraceNetifReceiveSkb, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint netif_receive_skb_entry: %s", err)
	// }
	// defer Netif_recieve.Close()

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

	// Создаем perf.Reader для чтения событий eBPF

	// Канал для завершения работы
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {

		record := new(perf.Record)

		const buffLen = 8196
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("failed to create perf reader: %s", err)
		}
		defer rd.Close()

		executableName := os.Args[0]
		if len(executableName) > 2 {
			executableName = executableName[2:]
		}

		for {

			err := rd.ReadInto(record)
			if err != nil {
				if errors.Is(err, os.ErrDeadlineExceeded) {
					continue
				}
				log.Printf("error reading from perf reader: %v", err)
				return
			}

			if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				log.Println("!!!!!!!!!!!!!!!!!!!!!!!invalid event size!!!!!!!!!!!!!!!!!!")
				continue
			}

			// Приводим прочитанные данные к структуре bpfTraceInfo
			event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

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

			if pkg.Int8ToString(event.Comm) == executableName {
				continue
			}

			if event.Sysexit == 1 {

				family := event.Family

				if family == 2 {

					port := int(event.Dport)
					data, exists := eventMap_1[port]
					if !exists {
						data = &EventData{}
						eventMap[port] = data
					}
					data.Sendmsg = &Sendmsg{
						DstIP:   dstIP,
						DstPort: port,
						Pid:     event.Pid,
						Comm:    pkg.Int8ToString(event.Comm),
					}

					if data.Lookup != nil {

						fmt.Printf("PID=%d srcIP=%s:%d -> dstIP=%s:%d\n",
							data.Sendmsg.Pid,
							data.Lookup.DstIP.String(),
							data.Lookup.DstPort,
							data.Lookup.SrcIP.String(),
							data.Lookup.SrcPort,
						)

						fmt.Printf("PID=%d srcIP=%s:%d <- dstIP=%s:%d\n",
							data.Recvmsg.Pid,
							data.Recvmsg.SrcIP,
							data.Recvmsg.SrcPort,
							data.Sendmsg.DstIP,
							data.Sendmsg.DstPort,
						)

					}

				} else if family == 10 {
					//port := event.Dport

					//pid := event.Pid
					// fmt.Printf("STATE=1 IPv6 PID=%d IPv6=%x:%x:%x:%x:%d NAME=%s\n",
					// 	pid,
					// 	event.DstIP6[0], event.DstIP6[1],
					// 	event.DstIP6[2], event.DstIP6[3],

					// 	port,
					// 	pkg.Int8ToString(event.Comm),
					// )

				}

			}

			if event.Sysexit == 11 {

				if event.Family == 2 {

					port := int(event.Dport)
					data, exists := eventMap[port]
					if !exists {
						data = &EventData{}
						eventMap[port] = data
					}
					data.Sendmsg = &Sendmsg{
						DstIP:   dstIP,
						DstPort: port,
						Pid:     event.Pid,
						Comm:    pkg.Int8ToString(event.Comm),
					}

					if data.Lookup != nil && data.Recvmsg != nil {

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

					}

				} else if event.Family == 10 {
					// port := event.Dport

					// pid := event.Pid
					// fmt.Printf("STATE=11 IPv6 PID=%d IPv6=%x:%x:%x:%x:%d  NAME=%s\n",
					// 	pid,
					// 	event.DstIP6[0], event.DstIP6[1],
					// 	event.DstIP6[2], event.DstIP6[3],

					// 	port,
					// 	pkg.Int8ToString(event.Comm),
					// )

				}

			}

			if event.Sysexit == 2 {

				if event.Family == 2 {

					port := int(event.Sport)
					data, exists := eventMap[port]
					if !exists {
						data = &EventData{}
						eventMap[port] = data
					}
					data.Recvmsg = &Recvmsg{
						SrcIP:   srcIP,
						SrcPort: port,
						Pid:     event.Pid,
						Comm:    pkg.Int8ToString(event.Comm),
					}

					// fmt.Printf("DATA_RECVFROM PID=%d srcIP=%s:%d\n",
					// 	data.Recvmsg.Pid,
					// 	data.Recvmsg.SrcIP,
					// 	data.Recvmsg.SrcPort)

				} else if event.Family == 10 {
					// port := event.Sport
					// pid := event.Pid
					// fmt.Printf("STATE2 IPv6 PID=%d IPv6=%x:%x:%x:%x:%d\n",
					// 	pid,
					// 	event.SrcIP6[0], event.SrcIP6[1],
					// 	event.SrcIP6[2], event.SrcIP6[3],
					// 	port,
					// )

				}

			}

			if event.Sysexit == 12 {

				if event.Family == 2 {

					port := int(event.Sport)
					data, exists := eventMap[port]
					if !exists {
						data = &EventData{}
						eventMap[port] = data
					}
					data.Recvmsg = &Recvmsg{
						SrcIP:   srcIP,
						SrcPort: port,
						Pid:     event.Pid,
						Comm:    pkg.Int8ToString(event.Comm),
					}
					if data.Lookup != nil && data.Sendmsg != nil {

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

					}

				} else if event.Family == 10 {
					// port := event.Sport
					// pid := event.Pid
					// fmt.Printf("STATE=12 IPv6 PID=%d srcIPv6=%x:%x:%x:%x:%d\n",
					// 	pid,
					// 	event.SrcIP6[0], event.SrcIP6[1],
					// 	event.SrcIP6[2], event.SrcIP6[3],
					// 	port,
					// )

					// eventDstIP6 := [4]uint32{
					// 	event.SrcIP6[0],
					// 	event.SrcIP6[1],
					// 	event.SrcIP6[2],
					// 	event.SrcIP6[3],
					// }

					// ipBytes := []byte{
					// 	byte(eventDstIP6[0] >> 24), byte(eventDstIP6[0] >> 16), byte(eventDstIP6[0] >> 8), byte(eventDstIP6[0]),
					// 	byte(eventDstIP6[1] >> 24), byte(eventDstIP6[1] >> 16), byte(eventDstIP6[1] >> 8), byte(eventDstIP6[1]),
					// 	byte(eventDstIP6[2] >> 24), byte(eventDstIP6[2] >> 16), byte(eventDstIP6[2] >> 8), byte(eventDstIP6[2]),
					// 	byte(eventDstIP6[3] >> 24), byte(eventDstIP6[3] >> 16), byte(eventDstIP6[3] >> 8), byte(eventDstIP6[3]),
					// }

					// ip := net.IP(ipBytes)
					// fmt.Printf("STATE=12 SHRT IPv6:=%s%d\n", ip.String(), event.Sport)

				}

			}

			if event.Sysexit == 3 {

				family := event.Family
				if family == 2 {

					// 	dstAddr := fmt.Sprintf("//%s[%s]:%d", pkg.ResolveIP(dstIP), dstIP.String(), event.Dport)
					// 	srcAddr := fmt.Sprintf("//[%s]:%d", srcIP.String(), event.Sport)
					// fmt.Printf("STATE=3 srcIP=%s dstIP=%s PROTO=%d FAMILY=%d\n", srcAddr, dstAddr, event.Proto, int(family))

					port := int(event.Dport)

					data, exists := eventMap[port]
					if !exists {
						data = &EventData{}
						eventMap[port] = data
					}

					data.Lookup = &Lookup{
						SrcIP:   srcIP,
						SrcPort: int(event.Sport),
						DstIP:   dstIP,
						DstPort: int(event.Dport),
						Proto:   int(event.Proto),
					}

					port_1 := int(event.Sport)

					data_1, exists := eventMap_1[port_1]
					if !exists {
						data_1 = &EventData{}
						eventMap_1[port_1] = data
					}

					data_1.Lookup = &Lookup{
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

					}

				} else if family == 10 {

					// fmt.Printf("STATE=3 DST IPv6=%x:%x:%x:%x\n",
					// 	event.DstIP6[0], event.DstIP6[1],
					// 	event.DstIP6[2], event.DstIP6[3])

					// fmt.Printf("STATE=3 SRC IPv6=%x:%x:%x:%x\n",
					// 	event.SrcIP6[0], event.SrcIP6[1],
					// 	event.SrcIP6[2], event.SrcIP6[3])

					// fmt.Printf("STATE=3 SPORT=%d  DPORT=%d PROTO=%d\n", event.Sport, event.Dport, event.Proto)

				}

			}

			if event.Sysexit == 6 {

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

						dsthost, err = pkg.ResolveIP_n(dstIP)
						if err != nil {
							dsthost = "unknown"

							//log.Println("Оdsthost шибка при разрешении исходного IP:", err)
						} else {
							//	fmt.Println("Исходное доменное имя для IP", dstIP, ":", dsthost)
						}

					}

					srchost := pkg.ResolveIP(srcIP)

					//	dsthost, err := pkg.ResolveIP_n(dstIP)

					srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

					if event.Proto == 6 {

						proto = "TCP"
					}

					fmt.Println("")
					fmt.Printf("PID=%d %s:%s <- %s:%s \n", event.Pid, proto, srcAddr, proto, dstAddr)

				}
				if event.State == 2 {
					mu.Lock()
					select {
					case eventChan_pid <- int(event.Pid):
					default:
						//fmt.Println("State 2: eventChan_pid заполнен, пропускаю запись PID")
					}
					mu.Unlock()
				}

				if event.State == 10 {
					mu.Lock()
					select {
					case eventChan_pid <- int(event.Pid):
					default:
						//fmt.Println("State 10: eventChan_pid заполнен, пропускаю запись PID")
					}
					mu.Unlock()
				}

				select {

				case xxx = <-eventChan_sport:

					if dstIP.IsLoopback() {
						dsthost = pkg.ResolveIP(dstIP)
					} else {

						dsthost, err = pkg.ResolveIP_n(dstIP)
						if err != nil {
							dsthost = "unknown"

							//log.Println("Оdsthost шибка при разрешении исходного IP:", err)
						} else {
							//	fmt.Println("Исходное доменное имя для IP", dstIP, ":", dsthost)
						}

					}

					srchost := pkg.ResolveIP(srcIP)

					srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), xxx)
					dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

					select {
					case xxx_pid = <-eventChan_pid:
						//fmt.Printf("State 2: получил PID %d\n", xxx_pid)
					default:
						//fmt.Println("State 2: eventChan_pid пуст, PID неизвестен")
					}

					if event.Proto == 6 {

						proto = "TCP"
					}

					fmt.Printf("PID=%d %s:%s -> %s:%s \n", xxx_pid, proto, srcAddr, proto, dstAddr)
					fmt.Println("")

				default:
					fmt.Println("")
				}

			}

		}

	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}
