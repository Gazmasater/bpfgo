package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"
	"reflect"

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

func loadTraceObject(objs interface{}) error {
	// Загружаем скомпилированную BPF-программу
	spec, err := ebpf.LoadCollectionSpec("trace_x86_bpfel.o")
	if err != nil {
		return fmt.Errorf("loading BPF program: %v", err)
	}

	// Создаем коллекцию объектов из BPF-спецификации
	coll, err := ebpf.NewCollection(spec)
	if err != nil {
		return fmt.Errorf("creating BPF collection: %v", err)
	}

	// Привязываем карту trace_events
	traceEventsMap, found := coll.Maps["trace_events"]
	if !found {
		return fmt.Errorf("trace_events map not found in BPF program")
	}

	// Записываем карту в структуру
	v := reflect.ValueOf(objs).Elem()
	v.FieldByName("TraceEvents").Set(reflect.ValueOf(traceEventsMap))

	return nil
}

func main() {
	// Загружаем скомпилированную BPF-программу
	objs := struct {
		TraceEvents *ebpf.Map
	}{}

	if err := loadTraceObject(&objs); err != nil {
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
