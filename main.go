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

var xxx int
var xxx_pid int
var proto string
var srchost string
var dsthost string

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
	defer objs.Close() // Закроем объекты при выходе

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", netns.Fd())

	skLookupLink, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("failed to attach sk_lookup program: %v", err)
	}
	defer skLookupLink.Close()

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

	// Accept4Enter, err := link.Tracepoint("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_enter_accept4: %s", err)
	// }
	// defer Accept4Enter.Close()

	// Accept4Exit, err := link.Tracepoint("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit, nil)
	// if err != nil {
	// 	log.Fatalf("opening tracepoint sys_exit_accept4: %s", err)
	// }
	// defer Accept4Exit.Close()

	ConnectEnter, err := link.Tracepoint("syscalls", "sys_enter_connect", objs.TraceConnectEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_connect: %s", err)
	}
	defer ConnectEnter.Close()

	ConnectExit, err := link.Tracepoint("syscalls", "sys_exit_connect", objs.TraceConnectExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_connect: %s", err)
	}
	defer ConnectExit.Close()

	BindEnter, err := link.Tracepoint("syscalls", "sys_enter_bind", objs.TraceBindEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_bind: %s", err)
	}
	defer BindEnter.Close()

	BindExit, err := link.Tracepoint("syscalls", "sys_exit_bind", objs.TraceBindExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_bind: %s", err)
	}
	defer BindExit.Close()

	InetSock, err := link.Tracepoint("sock", "inet_sock_set_state", objs.TraceTcpEst, nil)
	if err != nil {
		log.Fatalf("opening tracepoint inet_sock_set_state: %s", err)
	}
	defer InetSock.Close()

	// Создаем perf.Reader для чтения событий eBPF
	const buffLen = 4096
	rd, err := perf.NewReader(objs.TraceEvents, buffLen)
	if err != nil {
		log.Fatalf("failed to create perf reader: %s", err)
	}
	defer rd.Close()

	// Канал для завершения работы
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)
	//portChan := make(chan int, 1)

	// Запускаем цикл чтения событий eBPF
	go func() {

		executableName := os.Args[0]
		if len(executableName) > 2 {
			executableName = executableName[2:]
		}

		for {
			record := new(perf.Record)
			err := rd.ReadInto(record)
			if err != nil {
				if errors.Is(err, os.ErrDeadlineExceeded) {
					continue
				}
				log.Printf("error reading from perf reader: %v", err)
				return
			}

			if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				log.Println("invalid event size")
				continue
			}

			// Приводим прочитанные данные к структуре bpfTraceInfo
			event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

			// Преобразуем IP-адреса в строковый формат
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

			if event.Sysexit == 6 {

				if event.State == 1 {

					mu.Lock()
					select {
					case eventChan_sport <- int(event.Sport):
					default:
						// Если канал уже содержит значение, заменяем его
						//	<-eventChan
						eventChan_sport <- int(event.Sport)
						fmt.Printf("State 1: заменен порт %d\n", event.Sport)
					}
					mu.Unlock()

					// var srchost string
					// var dsthost string

					srchost = pkg.ResolveIP(srcIP)
					dsthost = pkg.ResolveIP(dstIP)

					srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

					if event.Proto == 6 {

						proto = "TCP:"
					}

					fmt.Printf("PID=%d %s%s <- %s%s \n", event.Pid, proto, srcAddr, proto, dstAddr)

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

				select {

				case xxx = <-eventChan_sport:
					var srchost string
					var dsthost string

					srchost = pkg.ResolveIP(srcIP)
					dsthost = pkg.ResolveIP(dstIP)

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

					fmt.Printf("PID=%d %s%s -> %s%s \n", xxx_pid, proto, srcAddr, proto, dstAddr)

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
