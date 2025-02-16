bpftool map clear /sys/fs/bpf/trace_events


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c





clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


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
	// Создаем новый экземпляр eBPF-системы
	bpf := goebpf.NewDefaultEbpfSystem()

	// Получаем perf_event по его имени
	perfEvent := bpf.GetPerfEventByName("trace_events")
	if perfEvent == nil {
		log.Fatal("Не удалось найти perf_event с именем 'trace_events'")
	}

	// Создаем объект для работы с perf событиями
	events, err := goebpf.NewPerfEvents(perfEvent)
	if err != nil {
		log.Fatalf("Ошибка создания perf-событий: %v", err)
	}
	defer events.Stop()

	// Ожидаем сигнала завершения
	sigCh := make(chan os.Signal, 1)
	signal.Notify(sigCh, syscall.SIGINT, syscall.SIGTERM)
	defer signal.Stop(sigCh)

	// Запускаем чтение всех событий для всех процессов и CPU
	err = events.StartForAllProcessesAndCPUs(4096)
	if err != nil {
		log.Fatalf("Ошибка запуска чтения событий: %v", err)
	}
	defer events.Stop()

	// Основной цикл для обработки событий
	for {
		select {
		case data := <-events:
			var event struct {
				Pid   uint32
				Comm  [64]byte
				SrcIP uint32
				Sport uint16
			}

			// Распаковываем данные события в структуру
			err := binary.Read(bytes.NewReader(data), binary.LittleEndian, &event)
			if err != nil {
				log.Printf("Ошибка распаковки данных события: %v", err)
				continue
			}

			// Выводим данные о событии
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
