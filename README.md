clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o



package main

import (
    "fmt"
    "log"

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
    defer events.Stop()

    // Обработка событий
    for {
        select {
        case data := <-events:
            // Преобразование данных в структуру event_t
            var event struct {
                Pid    uint32
                Comm   [64]byte
                SrcIP  uint32
                Sport  uint16
            }
            err := goebpf.Unmarshal(data, &event)
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
        }
    }
}


