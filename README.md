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

	// Открываем файл eBPF объекта
	file, err := os.Open("your_bpf_object.o")
	if err != nil {
		log.Fatalf("failed to open eBPF object file: %v", err)
	}
	defer file.Close()

	// Загружаем спецификацию eBPF коллекции из файла
	spec, err := ebpf.LoadCollectionSpecFromReader(file)
	if err != nil {
		log.Fatalf("failed to load eBPF collection spec from reader: %v", err)
	}

	// Загружаем коллекцию eBPF
	objs, err := ebpf.LoadCollection(spec)
	if err != nil {
		log.Fatalf("failed to load eBPF collection: %v", err)
	}
	defer objs.Close()

	// Создаем новый перф ридер для считывания событий
	buffLen := 4096 // Размер буфера
	rd, err := perf.NewReader(objs, buffLen)
	if err != nil {
		log.Fatalf("opening ringbuf reader: %s", err)
	}
	defer rd.Close()

	// Создаем перф рекорд для чтения данных
	record := new(perf.Record)

	// Цикл чтения перф событий
	for {
		err := rd.ReadInto(record)
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				// Пропускаем, если произошло превышение времени ожидания
				continue
			}
			// Обработка ошибок чтения
			log.Printf("Error reading trace from reader: %v", err)
			break
		}

		// Преобразуем полученные байты в структуру TraceInfo
		var info TraceInfo
		// Чтение и копирование данных из record в структуру
		copy(info.Comm[:], record.Raw[:16]) // Копируем имя процесса в структуру
		info.Pid = uint32(record.Raw[16])   // Парсим PID
		info.SrcIP = uint32(record.Raw[20]) // Парсим SrcIP
		info.DstIP = uint32(record.Raw[24]) // Парсим DstIP
		info.Sport = uint16(record.Raw[28]) // Парсим Source Port
		info.Dport = uint16(record.Raw[30]) // Парсим Destination Port

		// Выводим полученные данные
		fmt.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, DstIP=%d.%d.%d.%d, Sport=%d, Dport=%d\n",
			info.Pid,
			info.Comm,
			(info.SrcIP>>24)&0xFF, (info.SrcIP>>16)&0xFF, (info.SrcIP>>8)&0xFF, info.SrcIP&0xFF,
			(info.DstIP>>24)&0xFF, (info.DstIP>>16)&0xFF, (info.DstIP>>8)&0xFF, info.DstIP&0xFF,
			info.Sport, info.Dport)
	}
}
