package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/dropbox/goebpf"
)

func main() {
	bpf := goebpf.NewDefaultEbpfSystem()

	perfMap := bpf.GetMapByName("trace_events")
	if perfMap == nil {
		log.Fatal("Не удалось найти карту perf с именем 'trace_events'")
	}

	perfEvents, err := goebpf.NewPerfEvents(perfMap)
	if err != nil {
		log.Fatalf("Ошибка создания perf-событий: %v", err)
	}
	defer perfEvents.Stop()

	events, err := perfEvents.StartForAllProcessesAndCPUs(4096)
	if err != nil {
		log.Fatalf("Ошибка запуска чтения событий: %v", err)
	}
	defer perfEvents.Stop()

	sigCh := make(chan os.Signal, 1)
	signal.Notify(sigCh, syscall.SIGINT, syscall.SIGTERM)
	defer signal.Stop(sigCh)

	for {
		select {
		case data := <-events:
			var event struct {
				Pid   uint32
				Comm  [64]byte
				SrcIP uint32
				Sport uint16
			}
			err := binary.Read(bytes.NewReader(data), binary.LittleEndian, &event)
			if err != nil {
				log.Printf("Ошибка распаковки данных события: %v", err)
				continue
			}

			fmt.Printf("PID: %d, Comm: %s, SrcIP: %d.%d.%d.%d, SrcPort: %d\n",
				event.Pid,
				string(event.Comm[:]),
				(event.SrcIP>>24)&0xFF, (event.SrcIP>>16)&0xFF,
				(event.SrcIP>>8)&0xFF, event.SrcIP&0xFF,
				event.Sport)
		case <-sigCh:
			fmt.Println("\nПолучен сигнал завершения. Завершение работы...")
			return
		}
	}
}
