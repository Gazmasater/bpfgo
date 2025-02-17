package main

import (
	"log"
	"os"
	"unsafe"

	"github.com/cilium/ebpf/perf"
	"github.com/pkg/errors"
)

type bpfTraceInfo struct {
	PID   uint32
	SrcIP uint32
	DstIP uint32
	Sport uint16
	Dport uint16
	Comm  [16]byte
}

func main() {
	// Инициализация perf ридера
	rd, err := perf.NewReader(objs, buffLen)
	if err != nil {
		log.Fatalf("opening ringbuf reader: %s", err)
	}
	defer rd.Close()

	record := new(perf.Record)

	// Цикл чтения событий
Loop:
	for {
		err := rd.ReadInto(record)
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				continue
			}
			e := errors.WithMessage(err, "reading trace from reader")
			log.Printf("%v", e)
			break
		}

		// Преобразование сырых данных в структуру bpfTraceInfo
		event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

		// Обработка события (например, вывод на экран)
		log.Printf("Event received: PID: %d, SrcIP: %d, DstIP: %d, Sport: %d, Dport: %d, Comm: %s",
			event.PID, event.SrcIP, event.DstIP, event.Sport, event.Dport, string(event.Comm[:]))
	}
}
