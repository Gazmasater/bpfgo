clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

package main

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -tags linux trace trace.c


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ go generate
no required module provides package github.com/cilium/ebpf/cmd/bpf2go; to add it:
        go get github.com/cilium/ebpf/cmd/bpf2go
gen.go:3: running "go": exit status 1
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ 






package main

import (
	"bytes"
	"context"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/dropbox/goebpf"
)

func main() {
	// Инициализация системы eBPF
	bpf := goebpf.NewDefaultEbpfSystem()

	// Получение карты perf по имени
	perfMap := bpf.GetMapByName("events")
	if perfMap == nil {
		log.Fatal("Не удалось найти карту perf с именем 'events'")
	}

	// Создание perf-событий
	perfEvents, err := goebpf.NewPerfEvents(perfMap)
	if err != nil {
		log.Fatalf("Ошибка создания perf-событий: %v", err)
	}
	defer perfEvents.Stop()

	// Запуск чтения событий
	events, err := perfEvents.StartForAllProcessesAndCPUs(4096)
	if err != nil {
		log.Fatalf("Ошибка запуска чтения событий: %v", err)
	}
	defer perfEvents.Stop()

	// Создание канала для обработки сигналов
	sigCh := make(chan os.Signal, 1)
	signal.Notify(sigCh, syscall.SIGINT, syscall.SIGTERM)
	defer signal.Stop(sigCh)

	// Обработка событий
	for {
		select {
		case data := <-events:
			// Преобразование данных в структуру event_t
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

			// Вывод информации
			fmt.Printf("PID: %d, Comm: %s, SrcIP: %d.%d.%d.%d, SrcPort: %d\n",
				event.Pid,
				string(event.Comm[:]),
				(event.SrcIP>>24)&0xFF, (event.SrcIP>>16)&0xFF,
				(event.SrcIP>>8)&0xFF, event.SrcIP&0xFF,
				event.Sport)
		case <-sigCh:
			// Обработка сигнала завершения
			fmt.Println("\nПолучен сигнал завершения. Завершение работы...")
			return
		}
	}
}
