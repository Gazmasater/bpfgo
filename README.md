bpftool map clear /sys/fs/bpf/trace_events


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c





clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


package main

import (
	"log"
	"os"
	"time"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/pkg/errors"
)

// Пример структуры события, которая используется в вашей программе
type bpfTraceInfo struct {
	Counter uint64
	EventID uint32
	Timestamp uint64
}

func main() {
	// Открываем ранее загруженную коллекцию eBPF (предположим, что она уже была загружена)
	collection, err := ebpf.LoadCollectionFromReader(os.Stdin) // Замените на реальный источник
	if err != nil {
		log.Fatalf("failed to load BPF collection: %s", err)
	}

	// Получаем карту типа PERF_EVENT_ARRAY
	traceEventsMap := collection.Maps["trace_events"]
	if traceEventsMap == nil {
		log.Fatalf("trace_events map not found in BPF program")
	}

	// Создаем ридер для карты
	perfReader, err := ebpf.NewPerfEventReader(traceEventsMap)
	if err != nil {
		log.Fatalf("failed to create perf event reader: %s", err)
	}
	defer perfReader.Close()

	// Чтение событий из карты
	log.Println("Waiting for events...")
	for {
		// Устанавливаем тайм-аут на 1 секунду
		perfReader.SetDeadline(time.Now().Add(time.Second))

		record, err := perfReader.Read()
		if err != nil {
			if err == os.ErrDeadlineExceeded {
				// Если тайм-аут истек, продолжаем ждать событий
				continue
			}
			log.Fatalf("failed to read perf event: %s", err)
		}

		// Преобразуем байты события в нужную структуру
		event := *(*bpfTraceInfo)(unsafe.Pointer(&record[0]))

		// Обрабатываем событие
		processEvent(event)
	}
}

// Обработчик для получения и вывода информации о событии
func processEvent(event bpfTraceInfo) {
	// Например, выводим информацию о событии
	log.Printf("Received event: Counter=%d, EventID=%d, Timestamp=%d\n", event.Counter, event.EventID, event.Timestamp)
}
