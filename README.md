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

	// Загружаем eBPF-программу и загружаем карты
	program, err := bpf.LoadAndAssignProgram("trace_events", goebpf.EbpfType("tracepoint"))
	if err != nil {
		log.Fatalf("Ошибка загрузки программы: %v", err)
	}

	// Получаем ссылку на карту, которая хранит perf_event
	perfMap, err := program.GetPerfMapByName("trace_events")
	if err != nil {
		log.Fatalf("Не удалось получить карту по имени 'trace_events': %v", err)
	}

	// Ожидаем сигнала завершения
	sigCh := make(chan os.Signal, 1)
	signal.Notify(sigCh, syscall.SIGINT, syscall.SIGTERM)
	defer signal.Stop(sigCh)

	// Основной цикл для получения событий из карты
	for {
		select {
		case data := <-perfMap:
			// Данные события приходят как срез байтов, распаковываем их в структуру
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
			// Обработка сигнала завершения
			fmt.Println("\nПолучен сигнал завершения. Завершение работы...")
			return
		}
	}
}




