
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

	//	Создаем perf.Reader для чтения событий eBPF

	// Канал для завершения работы
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

		for {
			record, err := rd.Read()
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

				} else if family == 10 {
					port := event.Dport
					ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
					pid := event.Pid
					fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
						pid,
						ip6.String(),
						port,
						pkg.Int8ToString(event.Comm),
					)

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

			if event.Sysexit == 2 {

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
						Comm:    pkg.Int8ToString(event.Comm),
					}

					data.HasRecvmsg = true

					if data.HasLookup && data.HasSendmsg {

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
						Comm:    pkg.Int8ToString(event.Comm),
					}

					data.HasRecvmsg = true

					if data.HasLookup && data.HasSendmsg {

						if data.Lookup.Proto == 17 {

							proto = "UDP"
						}

						fmt.Println("")

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

						fmt.Println("")
						delete(eventMap, port)

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

			if event.Sysexit == 3 {

				family := event.Family
				if family == 2 {

					// dstAddr := fmt.Sprintf("//%s[%s]:%d", pkg.ResolveIP(dstIP), dstIP.String(), event.Dport)
					// srcAddr := fmt.Sprintf("//[%s]:%d", srcIP.String(), event.Sport)

					port := int(event.Dport)

					data, exists := eventMap[port]
					if !exists {
						data = &EventData{}
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

					port_1 := int(event.Sport)

					data_1, exists := eventMap_1[port_1]
					if !exists {
						data_1 = &EventData{}
						eventMap_1[port_1] = data
					}

					data_1.Lookup = Lookup{
						SrcIP:   srcIP,
						SrcPort: int(event.Sport),
						DstIP:   dstIP,
						DstPort: int(event.Dport),
						Proto:   int(event.Proto),
					}

					if data.HasRecvmsg && data.HasSendmsg {

						if data.Lookup.Proto == 17 {

							proto = "UDP"
						}

						fmt.Println("")

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

						fmt.Println("")
						delete(eventMap, port)

					}

				} else if family == 10 {
					ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))

					fmt.Printf("LOOKUP SRC IPv6=%s\n",
						ip6,
					)

					ip6_d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))

					fmt.Printf("LOOKUP DST IPv6=%s\n",
						ip6_d,
					)
					fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROTO=%d\n", event.Sport, event.Dport, event.Proto)

				}

			}

			if event.Sysexit == 6 {

				if event.Family == 10 {

					sport := event.Sport
					dport := event.Dport
					pid := event.Pid
					ip6_s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))

					fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
						pid,
						ip6_s.String(),
						sport,
						pkg.Int8ToString(event.Comm),
					)

					ip6_d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))

					fmt.Printf("TCP IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
						pid,
						ip6_d,
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













