

bpftool map pin id <map_id> /sys/fs/bpf/trace_events


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
	"time"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
)

// Размер буфера для чтения событий
const bufferLen = 4096

// Функция для чтения событий из карты
func readPerfEvents() error {
	// Открытие карты, которая уже была загружена в ядро
	eventsMap, err := ebpf.LoadPinnedMap("/sys/fs/bpf/trace_events", nil)
	if err != nil {
		return fmt.Errorf("failed to open pinned map: %w", err)
	}
	defer eventsMap.Close()

	// Создаем perf.Reader для чтения событий из карты
	reader, err := perf.NewReader(eventsMap, bufferLen)
	if err != nil {
		return fmt.Errorf("failed to create reader: %w", err)
	}
	defer reader.Close()

	// Чтение событий в бесконечном цикле
	for {
		// Ожидаем событие
		record, err := reader.Read()
		if err != nil {
			// Если произошла ошибка, выводим и завершаем
			if err.Error() == "perf ring buffer timeout" {
				// Ожидаем следующее событие
				continue
			}
			return fmt.Errorf("failed to read event: %w", err)
		}

		// Обрабатываем событие
		log.Printf("Received event: %+v\n", record)
	}
}

func main() {
	// Чтение событий из карты
	if err := readPerfEvents(); err != nil {
		log.Fatalf("Error reading perf events: %v", err)
	}
}
