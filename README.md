lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


package main

import (
	"bpfgo/pkg"
	"errors"
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
	"syscall"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

// Глобальные объекты BPF
var objs bpfObjects

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

			//	portsMap := make(map[string]string)

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

			portMap := make(map[string]int)

			if event.Sysexit == 6 {

				if event.State == 1 {
					key := fmt.Sprintf("%s->%s:%d", srcIP.String(), dstIP.String(), event.Dport)

					portMap[key] = int(event.Sport)
				}
				
				fmt.Println("!!!!!!!!!!!")
				// Вывод каждого ключа и значения отдельно
				for key, value := range portMap {
					fmt.Printf("Key: %s, Value: %d\n", key, value)
				}

				if event.State == 2 {

					key := fmt.Sprintf("%s->%s:%d", srcIP.String(), dstIP.String(), event.Dport)

					if port, found := portMap[key]; found {

						event.Sport = uint16(port)
					}
				}

				srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
				dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

				if event.Sport == 0 {
					fmt.Printf("PID=%d srcAddr=%s -> dstAddr=%s  SYSCALL=%d STATE=%d\n", event.Pid, srcAddr, dstAddr, event.Sysexit, event.State)
				} else {
					fmt.Printf("PID=%d srcAddr=%s <- dstAddr=%s  SYSCALL=%d STATE=%d\n", event.Pid, srcAddr, dstAddr, event.Sysexit, event.State)

				}
			}

		}

	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}





