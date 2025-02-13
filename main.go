package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/cilium/ebpf/perf"
)

//go:generate bpf2go -target native bpf ./bpf/trace.c -- -I/usr/include

type ConnInfo struct {
	Pid   uint32
	SrcIP uint32
	DstIP uint32
	Sport uint16
	Dport uint16
	Comm  [64]byte
}

func main() {
	// Загружаем eBPF-программу
	objs := bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("Ошибка загрузки BPF: %v", err)
	}
	defer objs.Close()

	// Открываем канал событий
	rd, err := perf.NewReader(objs.TraceEvents, 4096)
	if err != nil {
		log.Fatalf("Ошибка создания перф-ридера: %v", err)
	}
	defer rd.Close()

	log.Println("eBPF-программа успешно загружена! Ожидание событий...")

	// Обработка SIGINT (Ctrl+C)
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	// Читаем события в цикле
	go func() {
		for {
			record, err := rd.Read()
			if err != nil {
				log.Printf("Ошибка чтения события: %v", err)
				continue
			}
			if record.LostSamples > 0 {
				log.Printf("Потеряно %d событий", record.LostSamples)
				continue
			}

			var info ConnInfo
			if err := binary.Read(bytes.NewBuffer(record.RawSample), binary.LittleEndian, &info); err != nil {
				log.Printf("Ошибка парсинга данных: %v", err)
				continue
			}

			fmt.Printf("PID: %d, SrcIP: %d, DstIP: %d, Sport: %d, Dport: %d, Comm: %s\n",
				info.Pid, info.SrcIP, info.DstIP, info.Sport, info.Dport, info.Comm)
		}
	}()

	<-stop
	log.Println("Завершаем работу.")
}
