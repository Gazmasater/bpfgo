bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

sudo bpftool map dump id 11




11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


bpf2go -output-dir $(pwd) \
  -tags linux \
  -type trace_info \
  -go-package main \
  target_amd64_bpf \
  $(pwd)/trace.c -- -I$(pwd)


  struct status_t {
    bool in_progress;
};



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
	Pid     uint32
	Comm    [128]byte
	SrcIP   uint32
	SrcPort uint16
	DstIP   uint32
	DstPort uint16
}

// Глобальные объекты BPF
var objs target_amd64_bpfObjects

// Инициализация, включая снятие ограничения по памяти и загрузку eBPF объектов
func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit for process: %v", err)
	}

	if err := loadTarget_amd64_bpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

func main() {
	defer objs.Close() // Закрытие объектов при завершении работы программы

	// Привязка eBPF-программы к tracepoint'ам
	if err := setupTracepoints(); err != nil {
		log.Fatalf("failed to setup tracepoints: %v", err)
	}

	// Чтение событий eBPF через perf.Reader
	rd, err := createPerfReader()
	if err != nil {
		log.Fatalf("failed to create perf reader: %s", err)
	}
	defer rd.Close()

	// Канал для обработки сигналов
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	// Асинхронное чтение и обработка событий
	go readPerfEvents(rd)

	// Ожидаем сигнала завершения
	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

// Настройка tracepoint'ов для отслеживания системных вызовов
func setupTracepoints() error {
	tracepoints := []struct {
		name      string
		prog      interface{}
		tracepoint string
	}{
		{"sys_enter_sendto", objs.TraceSendtoEnter, "syscalls", "sys_enter_sendto"},
		{"sys_exit_sendto", objs.TraceSendtoExit, "syscalls", "sys_exit_sendto"},
		{"sys_enter_recvfrom", objs.TraceRecvfromEnter, "syscalls", "sys_enter_recvfrom"},
		{"sys_exit_recvfrom", objs.TraceRecvfromExit, "syscalls", "sys_exit_recvfrom"},
	}

	for _, tp := range tracepoints {
		if err := linkTracepoint(tp.name, tp.tracepoint, tp.prog); err != nil {
			return err
		}
	}

	return nil
}

// Привязка eBPF-программы к tracepoint
func linkTracepoint(name, tracepoint, prog string) error {
	_, err := link.Tracepoint(tracepoint, prog, nil)
	if err != nil {
		return fmt.Errorf("opening tracepoint %s: %w", name, err)
	}
	return nil
}

// Создание perf.Reader для чтения событий из eBPF
func createPerfReader() (*perf.Reader, error) {
	const buffLen = 4096
	return perf.NewReader(objs.TraceEvents, buffLen)
}

// Чтение и обработка событий из perf.Reader
func readPerfEvents(rd *perf.Reader) {
	for {
		record := new(perf.Record)
		if err := rd.ReadInto(record); err != nil {
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

		// Выводим информацию о событии
		fmt.Printf("PID: %d, Comm: %s\n", event.Pid, event.Comm[:])
		fmt.Printf("Src IP: %v, Src Port: %d\n", event.SrcIP, event.SrcPort)
		fmt.Printf("Dst IP: %v, Dst Port: %d\n", event.DstIP, event.DstPort)
	}
}
