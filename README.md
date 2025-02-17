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
	"fmt"
	"log"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/rlimit"
	"golang.org/x/sys/unix"
)

type TraceInfo struct {
	Pid    uint32
	SrcIP  uint32
	DstIP  uint32
	Sport  uint16
	Dport  uint16
	Comm   [16]byte
}

func main() {
	// Поднимаем лимит на количество таблиц, которые можно открыть
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}

	// Открываем eBPF объект, который уже загружен
	// Предполагаем, что объект eBPF уже загружен и привязан
	prog, err := ebpf.NewProgramFromReader("your_bpf_object.o") // если уже загружен, можно пропустить этот шаг
	if err != nil {
		log.Fatalf("failed to load eBPF program: %v", err)
	}
	defer prog.Close()

	// Читаем данные через perf_event
	// Создаем структуру для чтения данных
	event := make([]byte, unsafe.Sizeof(TraceInfo{}))

	// Ожидаем и обрабатываем события
	for {
		// Ожидаем события от perf_event
		// Здесь мы указываем PID или что-то другое для определения событий
		// Ожидаем данные от программы через perf_event
		err := unix.PerfRead(0, event) // Это заглушка для получения данных, замените на правильную логику для perf_event
		if err != nil {
			log.Fatalf("failed to read event: %v", err)
		}

		// Преобразуем байты в структуру TraceInfo
		var info TraceInfo
		// Преобразуем байты в структуру
		// Это важно, чтобы корректно обработать структуру и не потерять данные
		copy(info.Comm[:], event[:16]) // Пример копирования в строку для comm, с учетом размера
		info.Pid = uint32(event[16]) // Пример парсинга других данных, нужно указать правильный порядок байтов

		// Печатаем информацию о событии
		fmt.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, Sport=%d\n",
			info.Pid,
			info.Comm,
			(info.SrcIP>>24)&0xFF, (info.SrcIP>>16)&0xFF, (info.SrcIP>>8)&0xFF, info.SrcIP&0xFF,
			info.Sport)
	}
}
