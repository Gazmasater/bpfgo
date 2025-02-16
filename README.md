

bpftool map pin id <map_id> /sys/fs/bpf/trace_events

bpftool map pin id 11 /sys/fs/bpf/trace_events


11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


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

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
)

// Размер буфера для чтения событий
const bufferLen = 4096

// Структура, соответствующая данным из BPF события
type ConnInfo struct {
	PID   uint32
	Comm  [16]byte
	SrcIP uint32
	Sport uint16
}

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

		// Преобразуем данные из perf record в структуру ConnInfo
		connInfo := ConnInfo{}
		// Чтение данных из record в connInfo
		copy(connInfo.Comm[:], record.RawSample[0:16]) // Копируем имя процесса (Comm)
		connInfo.PID = uint32(record.RawSample[16])   // PID
		connInfo.SrcIP = uint32(record.RawSample[20]) // IP (предполагается, что IP записан с 20-го байта)
		connInfo.Sport = uint16(record.RawSample[24]) // Порт (предполагается, что порт записан с 24-го байта)

		// Выводим полученные данные
		log.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, Sport=%d\n",
			connInfo.PID,
			connInfo.Comm,
			(connInfo.SrcIP>>24)&0xFF, (connInfo.SrcIP>>16)&0xFF, (connInfo.SrcIP>>8)&0xFF, connInfo.SrcIP&0xFF,
			connInfo.Sport)
	}
}

func main() {
	// Чтение событий из карты
	if err := readPerfEvents(); err != nil {
		log.Fatalf("Error reading perf events: %v", err)
	}
}
