package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
)

// Event структура должна совпадать с event_t в BPF-коде
type Event struct {
	PID   uint32
	SrcIP uint32
	Sport uint16
	Comm  [16]byte
}

func main() {
	// Загружаем скомпилированную BPF-программу
	objs := struct {
		TraceEvents *ebpf.Map
	}{}

	opts := &ebpf.CollectionOptions{}
	if err := loadTraceObjects(&objs, opts); err != nil {
		log.Fatalf("loading BPF objects: %v", err)
	}
	defer objs.TraceEvents.Close()

	// Открываем канал для чтения perf-событий
	reader, err := perf.NewReader(objs.TraceEvents, 4096)
	if err != nil {
		log.Fatalf("creating perf event reader: %v", err)
	}
	defer reader.Close()

	fmt.Println("Listening for UDP sys_exit_sendto events...")

	// Обработка прерываний (Ctrl+C)
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt)

	go func() {
		for {
			record, err := reader.Read()
			if err != nil {
				log.Printf("reading perf event: %v", err)
				continue
			}

			// Декодируем данные
			var event Event
			if err := binary.Read(bytes.NewBuffer(record.RawSample), binary.LittleEndian, &event); err != nil {
				log.Printf("parsing perf event: %v", err)
				continue
			}

			// Вывод в консоль
			fmt.Printf("UDP sys_exit_sendto: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				event.PID, bytes.Trim(event.Comm[:], "\x00"),
				(event.SrcIP>>24)&0xFF, (event.SrcIP>>16)&0xFF, (event.SrcIP>>8)&0xFF, event.SrcIP&0xFF,
				event.Sport)
		}
	}()

	<-sigChan
	fmt.Println("\nExiting...")
}
