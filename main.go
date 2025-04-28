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

	// SmsgEnter, err := link.Tracepoint("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_enter_sendmsg: %s", err)
	// }
	// defer SmsgEnter.Close()

	// SmsgExit, err := link.Tracepoint("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_exit_sendmsg: %s", err)
	// }
	// defer SmsgExit.Close()

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

	// RmsgEnter, err := link.Tracepoint("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_enter_recvmsg: %s", err)
	// }
	// defer RmsgEnter.Close()

	// RmsgExit, err := link.Tracepoint("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_exit_recvmsg: %s", err)
	// }
	// defer RmsgExit.Close()

	// REnter, err := link.Tracepoint("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_enter_recvfrom: %s", err)
	// }
	// defer REnter.Close()

	// RExit, err := link.Tracepoint("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_exit_recvfrom: %s", err)
	// }
	// defer RExit.Close()

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

		time.Sleep(1 * time.Nanosecond)

		record := new(perf.Record)

		const buffLen = 4096
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
			event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

			var srcIP, dstIP, ddstIP net.IP

			if event.Family == 2 {
				srcIP = net.IPv4(
					byte(event.Saddr4.S_addr),
					byte(event.Saddr4.S_addr>>8),
					byte(event.Saddr4.S_addr>>16),
					byte(event.Saddr4.S_addr>>24),
				)

				// ssrcIP = net.IPv4(
				// 	byte(event.Saddr4.S_addr),
				// 	byte(event.Saddr4.S_addr>>8),
				// 	byte(event.Saddr4.S_addr>>16),
				// 	byte(event.Saddr4.S_addr>>24),
				// )

				dstIP = net.IPv4(
					byte(event.Daddr4.S_addr),
					byte(event.Daddr4.S_addr>>8),
					byte(event.Daddr4.S_addr>>16),
					byte(event.Daddr4.S_addr>>24),
				)

				ddstIP = net.IPv4(
					byte(event.Ddaddr4.SinAddr.S_addr),
					byte(event.Ddaddr4.SinAddr.S_addr>>8),
					byte(event.Ddaddr4.SinAddr.S_addr>>16),
					byte(event.Ddaddr4.SinAddr.S_addr>>24),
				)
			}

			var dstIP6, srcIP6 net.IP
			if event.Family == 10 {
				dstIP6 = net.IP(event.Daddr6.In6U.U6Addr8[:])
				srcIP6 = net.IP(event.Saddr6.In6U.U6Addr8[:])

				//	fmt.Printf("Destination IPv6: %s\n", dstIP6)
			}

			if pkg.Int8ToString(event.Comm) == executableName {
				continue
			}

			if event.Sysexit == 1 {

				family := event.Family
				fmt.Printf("!!!!!SENDTO FAMILY=%d DSTIP=%s:%d\n",
					event.Family,
					ddstIP.String(),
					event.Dport)

				if family == 2 {

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

					fmt.Printf("!!!!!!!!!SENDTO  DST6=%s[%s]:%d\n",
						pkg.ResolveIP(dstIP6),
						dstIP6,
						event.Dport)

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

						fmt.Printf("PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
							data.Sendmsg.Pid,
							data.Sendmsg.Comm,
							proto,
							pkg.ResolveIP(dstIP),
							data.Lookup.DstIP,
							data.Lookup.DstPort,
							pkg.ResolveIP(srcIP),
							data.Lookup.SrcIP,
							data.Lookup.SrcPort,
						)

						fmt.Printf("PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
							data.Recvmsg.Pid,
							data.Recvmsg.Comm,
							proto,
							pkg.ResolveIP(dstIP),

							data.Lookup.DstIP,
							data.Lookup.DstPort,
							pkg.ResolveIP(srcIP),
							data.Lookup.SrcIP,
							data.Lookup.SrcPort,
						)

						fmt.Println("")

					}

				} else if event.Family == 10 {
					fmt.Printf("!!!!!!!!!SENDMSG SRC6=%s:%d DST6=%s:%d\n",
						srcIP6,
						event.Sport,
						dstIP6,
						event.Dport)

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

				} else if event.Family == 10 {
					fmt.Printf("!!!!!!!!!RECVFROM SRC6=%s:%d DST6=%s:%d\n",
						srcIP6,
						event.Sport,
						dstIP6,
						event.Dport)

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

						fmt.Printf("PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
							data.Sendmsg.Pid,
							data.Sendmsg.Comm,
							proto,
							data.Lookup.DstIP,
							data.Lookup.DstPort,
							data.Lookup.SrcIP,
							data.Lookup.SrcPort,
						)

						fmt.Printf("PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
							data.Recvmsg.Pid,
							data.Recvmsg.Comm,
							proto,
							data.Lookup.DstIP,
							data.Lookup.DstPort,
							data.Lookup.SrcIP,
							data.Lookup.SrcPort,
						)

						fmt.Println("")

					}

				} else if event.Family == 10 {
					fmt.Printf("!!!!!!!!!RECVMSG SRC6=%s:%d DST6=%s:%d\n",
						srcIP6,
						event.Sport,
						dstIP6,
						event.Dport)

				}

			}

			if event.Sysexit == 3 {

				family := event.Family

				if family == 2 {

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
						eventMap_1[port_1] = data_1
					}

					data_1.Lookup = &Lookup{
						SrcIP:   srcIP,
						SrcPort: int(event.Sport),
						DstIP:   dstIP,
						DstPort: int(event.Dport),
						Proto:   int(event.Proto),
					}

					//	fmt.Printf("LOOKUP srcIP=%s:%d\n", data_1.Lookup.SrcIP, data_1.Lookup.SrcPort)

					if data.Recvmsg != nil && data.Sendmsg != nil {

						proto := "TCP"
						if data.Lookup.Proto == 17 {
							proto = "UDP"
						}

						fmt.Println()

						fmt.Printf("PID=%d NAME=%s %s/%s:%d<-%s:%d\n",
							data.Recvmsg.Pid,
							data.Recvmsg.Comm,
							proto,
							data.Lookup.DstIP,
							data.Lookup.DstPort,
							data.Lookup.SrcIP,
							data.Lookup.SrcPort,
						)

						fmt.Printf("PID=%d NAME=%s %s/%s:%d->%s:%d\n",
							data.Sendmsg.Pid,
							data.Sendmsg.Comm,
							proto,
							data.Lookup.DstIP,
							data.Lookup.DstPort,
							data.Lookup.SrcIP,
							data.Lookup.SrcPort,
						)

						fmt.Println()
					}
				}
				if family == 10 {
					fmt.Printf("SRCIP6=%s:%d\n", srcIP6.String(), event.Sport)
					fmt.Printf("DSTIP6=%s:%d\n", dstIP6.String(), event.Dport)

				}
			}

			if event.Sysexit == 6 {

				switch event.Family {
				case 2:
					HandleIPEvent(event, srcIP, dstIP, &mu, eventChan_sport, eventChan_pid)

				case 10:

					HandleIPEvent(event, srcIP6, dstIP6, &mu, eventChan_sport, eventChan_pid)

				default:
					continue
				}

			}
		}

	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}
