bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=nftrace -target amd64 bpf $(pwd)/nftrace_perf.c -- -I$(pwd)

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


bpftool map pin id <map_id> /sys/fs/bpf/trace_events

bpftool map pin id 11 /sys/fs/bpf/trace_events


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
	"errors"
	"fmt"
	"log"
	"os"
	"path/filepath"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

type TraceInfo struct {
	Pid   uint32
	SrcIP uint32
	DstIP uint32
	Sport uint16
	Dport uint16
	Comm  [16]byte
}

func main() {
	// Убираем ограничения на количество таблиц, которые можно открыть
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}

	// Получаем текущую рабочую директорию
	wd, err := os.Getwd()
	if err != nil {
		log.Fatalf("failed to get current working directory: %v", err)
	}

	// Строим путь к файлу eBPF объекта относительно текущей директории
	eBpfFilePath := filepath.Join(wd, "generated", "bpf_x86_bpfel.o")

	// Загружаем коллекцию eBPF напрямую из файла
	fmt.Println("Loading eBPF object...")
	objs, err := ebpf.LoadCollection(eBpfFilePath)
	if err != nil {
		log.Fatalf("failed to load eBPF collection: %v", err)
	}
	defer objs.Close()

	fmt.Println("Loaded eBPF collection:")
	for name, obj := range objs.Maps {
		fmt.Printf("Map: %s\n", name)
		fmt.Printf("Map type: %v\n", obj.Type())
	}

	// Получаем карту для перф событий
	traceEventsMap, exists := objs.Maps["trace_events"]
	if !exists {
		log.Fatalf("map 'trace_events' not found in eBPF object")
	}
	fmt.Println("Map 'trace_events' found")

	// Создаем новый перф ридер для считывания событий
	buffLen := 40960 // Размер буфера
	rd, err := perf.NewReader(traceEventsMap, buffLen)
	if err != nil {
		log.Fatalf("opening ringbuf reader: %s", err)
	}
	defer rd.Close()

	// Создаем перф рекорд для чтения данных
	record := new(perf.Record)

	// Цикл чтения перф событий
	fmt.Println("Start reading events from trace_events map")
Loop:
	for {
		// Отладочный вывод
		fmt.Println("Waiting for event...")
		err := rd.ReadInto(record)
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				// Время ожидания истекло
				fmt.Println("Timeout, no data available")
				continue
			}
			// Добавляем контекст к ошибке и переходим к следующей итерации
			e := errors.WithMessage(err, "reading trace from reader")
			log.Printf("Error: %v", e)
			continue // Переходим к следующему циклу
		}

		// Успешно прочитано событие
		fmt.Println("Event read successfully!")

		// Преобразуем полученные байты в структуру
		var info TraceInfo
		data := record.RawSample
		copy(info.Comm[:], data[:16]) // Копируем имя процесса в структуру
		info.Pid = uint32(data[16])   // Парсим PID
		info.SrcIP = uint32(data[20]) // Парсим SrcIP
		info.DstIP = uint32(data[24]) // Парсим DstIP
		info.Sport = uint16(data[28]) // Парсим Source Port
		info.Dport = uint16(data[30]) // Парсим Destination Port

		// Выводим полученные данные
		fmt.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, DstIP=%d.%d.%d.%d, Sport=%d, Dport=%d\n",
			info.Pid,
			info.Comm,
			(info.SrcIP>>24)&0xFF, (info.SrcIP>>16)&0xFF, (info.SrcIP>>8)&0xFF, info.SrcIP&0xFF,
			(info.DstIP>>24)&0xFF, (info.DstIP>>16)&0xFF, (info.DstIP>>8)&0xFF, info.DstIP&0xFF,
			info.Sport, info.Dport)
	}
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./perf
[sudo] password for gaz358: 
Loading eBPF object...
Loaded eBPF collection:
Map: conn_info_map
Map type: Hash
Map: trace_events
Map type: PerfEventArray
Map: .bss
Map type: Array
Map: .rodata
Map type: Array
Map: addr_map
Map type: Hash
Map 'trace_events' found
Start reading events from trace_events map
Waiting for event...
