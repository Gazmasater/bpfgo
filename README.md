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



package main

import (
	"fmt"
	"log"
	"os"
	"path/filepath"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
)

type TraceInfo struct {
	Pid   uint32
	SrcIP uint32
	Sport uint16
	Dport uint16
	Comm  [16]byte
}

func main() {
	// Получаем текущую рабочую директорию
	wd, err := os.Getwd()
	if err != nil {
		log.Fatalf("failed to get current working directory: %v", err)
	}

	// Путь к скомпилированному eBPF объекту
	eBpfFilePath := filepath.Join(filepath.Dir(wd), "generated", "bpf_x86_bpfel.o")
	fmt.Println("Loading eBPF object...")

	// Загружаем eBPF коллекцию
	objs, err := ebpf.LoadCollection(eBpfFilePath)
	if err != nil {
		log.Fatalf("failed to load eBPF collection: %v", err)
	}
	defer objs.Close()

	// Проверка загруженных карт и вывод информации
	fmt.Println("Loaded eBPF collection:")
	for name, obj := range objs.Maps {
		fmt.Printf("Map: %s\n", name)
		fmt.Printf("Map type: %v\n", obj.Type())
	}

	// Линковка программы через Tracepoint для sys_enter_sendto
	kpEnter, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.Program("trace_sendto_enter"), nil)
	if err != nil {
		log.Fatalf("failed to open sys_enter_sendto tracepoint: %v", err)
	}
	defer kpEnter.Close()

	// Линковка программы через Tracepoint для sys_exit_sendto
	kpExit, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.Program("trace_sendto_exit"), nil)
	if err != nil {
		log.Fatalf("failed to open sys_exit_sendto tracepoint: %v", err)
	}
	defer kpExit.Close()

	// Прочитаем данные с перфоманс-буфера
	rd, err := perf.NewReader(objs, 4096)
	if err != nil {
		log.Fatalf("failed to create perf reader: %v", err)
	}
	defer rd.Close()

	record := new(perf.Record)

	// Чтение данных с перфоманс-буфера
	for {
		err := rd.ReadInto(record)
		if err != nil {
			if err == os.ErrDeadlineExceeded {
				continue
			}
			log.Printf("error reading trace from reader: %v", err)
			break
		}

		// Разбор полученного события
		event := *(*TraceInfo)(unsafe.Pointer(&record.RawSample[0]))
		fmt.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, Sport=%d, Dport=%d\n",
			event.Pid,
			event.Comm,
			(event.SrcIP>>24)&0xFF, (event.SrcIP>>16)&0xFF, (event.SrcIP>>8)&0xFF, event.SrcIP&0xFF,
			event.Sport, event.Dport)
	}
}



objs, err := ebpf.LoadCollection(eBpfFilePath)
if err != nil {
    log.Fatalf("failed to load eBPF collection: %v", err)
}
defer objs.Close()

// Печатаем имена всех программ в коллекции для проверки
fmt.Println("Loaded eBPF collection programs:")
for name, program := range objs.Programs {
    fmt.Printf("Program: %s, Type: %v\n", name, program.Type())
}

