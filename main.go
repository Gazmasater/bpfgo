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

// func resolveIP(ip net.IP) string {
// 	names, err := net.LookupAddr(ip.String())
// 	if err != nil || len(names) == 0 {
// 		return "Unknown"
// 	}
// 	return names[0] // Берем первый найденный домен
// }

// func int8ToString(arr [64]int8) string {
// 	// Преобразуем []int8 в []byte
// 	byteArr := make([]byte, len(arr))
// 	for i, v := range arr {
// 		byteArr[i] = byte(v)
// 	}

//		// Убираем NULL-байты в конце и преобразуем в строку
//		return string(bytes.Trim(byteArr, "\x00"))
//	}
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

	// Привязываем eBPF-программу к tracepoint
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

	Accept4Enter, err := link.Tracepoint("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_accept4: %s", err)
	}
	defer Accept4Enter.Close()

	Accept4Exit, err := link.Tracepoint("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_accept4: %s", err)
	}
	defer Accept4Exit.Close()

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

			// Выводим все данные
			fmt.Printf("PID: %d, Comm=%s ,SrcIP: %s, SrcPort: %d, DstIP: %s(%s), DstPort: %d \n",
				event.Pid,
				pkg.Int8ToString(event.Comm),
				srcIP.String(),
				event.Sport,
				dstIP.String(),
				pkg.ResolveIP(dstIP),
				event.Dport,
			)
		}

	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}
