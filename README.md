bpftool map clear /sys/fs/bpf/trace_events


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c





clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


package main

import (
	"fmt"
	"os"
	"log"

	"github.com/cilium/ebpf"
)

func main() {
	// Загружаем BPF-программу и карту
	programPath := "./bpf_program.o"
	spec, err := ebpf.LoadCollectionSpec(programPath)
	if err != nil {
		log.Fatalf("failed to load BPF program: %s", err)
	}

	// Загружаем коллекцию
	collection, err := ebpf.NewCollection(spec)
	if err != nil {
		log.Fatalf("failed to load BPF collection: %s", err)
	}

	// Получаем карту trace_events
	traceEventsMap := collection.Maps["trace_events"]
	if traceEventsMap == nil {
		log.Fatalf("trace_events map not found in BPF program")
	}

	// Открываем перфоманс-событие для чтения
	perfReader, err := ebpf.NewPerfEventReader(traceEventsMap)
	if err != nil {
		log.Fatalf("failed to create perf event reader: %s", err)
	}
	defer perfReader.Close()

	// Чтение событий из карты
	for {
		record, err := perfReader.Read()
		if err != nil {
			if err.Error() == "EOF" {
				break
			}
			log.Fatalf("failed to read perf event: %s", err)
		}

		// Обработка прочитанного события
		fmt.Printf("Received event: %v\n", record)
	}
}
