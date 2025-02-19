package main

import (
	"errors"
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

type bpfTraceInfo struct {
	Pid  uint32
	Tid  uint32
	Comm [16]byte
}

// Глобальные объекты BPF
var objs target_amd64_bpfObjects

func init() {
	// Снимаем ограничение на память
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit for process: %v", err)
	}

	// Загружаем eBPF-объекты
	if err := loadTarget_amd64_bpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

func main() {
	defer objs.Close() // Закроем объекты при выходе

	// Привязываем eBPF-программу к tracepoint
	kpEnter, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendto: %s", err)
	}
	defer kpEnter.Close()

	kpExit, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.TraceSendtoExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_sendto: %s", err)
	}
	defer kpExit.Close()

	fmt.Println("kpEnter:", kpEnter)
	fmt.Println("kpExit:", kpExit)

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
			fmt.Printf("PID: %d, TID: %d, Comm: %s\n", event.Pid, event.Tid, event.Comm)

		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}
