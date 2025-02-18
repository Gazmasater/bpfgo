package main

import (
	"errors"
	"fmt"
	"log"
	"os"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

type TraceInfo struct {
	Pid   uint32
	SrcIP uint32
	DstIP uint32
	Sport uint16
	Dport uint16
	Comm  [16]byte
}

func main() {
	// Убираем ограничения на количество таблиц, которые можно открыть
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}

	// Открываем файл eBPF объекта
	file, err := os.Open("bpf_x86_bpfel.o")
	if err != nil {
		log.Fatalf("failed to open eBPF object file: %v", err)
	}
	defer file.Close()

	// Загружаем коллекцию eBPF из файла
	objs, err := ebpf.LoadCollection("bpf_x86_bpfel.o")
	if err != nil {
		log.Fatalf("failed to load eBPF collection: %v", err)
	}
	defer objs.Close()

	// Получаем карту для перф событий
	traceEventsMap := objs.Maps["trace_events"]

	// Создаем новый перф ридер для считывания событий
	buffLen := 4096 // Размер буфера
	rd, err := perf.NewReader(traceEventsMap, buffLen)
	if err != nil {
		log.Fatalf("opening ringbuf reader: %s", err)
	}
	defer rd.Close()

	// Создаем перф рекорд для чтения данных
	record := new(perf.Record)

	// Цикл чтения перф событий
	for {
		err := rd.ReadInto(record)
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				// Пропускаем, если произошло превышение времени ожидания
				continue
			}
			// Обработка ошибок чтения
			log.Printf("Error reading trace from reader: %v", err)
			break
		}

		// Преобразуем полученные байты в структуру TraceInfo
		var info TraceInfo
		data := record.RawSample      // Получаем сырые данные из записи
		copy(info.Comm[:], data[:16]) // Копируем имя процесса в структуру
		info.Pid = uint32(data[16])   // Парсим PID
		info.SrcIP = uint32(data[20]) // Парсим SrcIP
		info.DstIP = uint32(data[24]) // Парсим DstIP
		info.Sport = uint16(data[28]) // Парсим Source Port
		info.Dport = uint16(data[30]) // Парсим Destination Port

		// Выводим полученные данные
		fmt.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, DstIP=%d.%d.%d.%d, Sport=%d, Dport=%d\n",
			info.Pid,
			info.Comm,
			(info.SrcIP>>24)&0xFF, (info.SrcIP>>16)&0xFF, (info.SrcIP>>8)&0xFF, info.SrcIP&0xFF,
			(info.DstIP>>24)&0xFF, (info.DstIP>>16)&0xFF, (info.DstIP>>8)&0xFF, info.DstIP&0xFF,
			info.Sport, info.Dport)
	}
}
