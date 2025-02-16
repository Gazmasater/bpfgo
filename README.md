bpftool map clear /sys/fs/bpf/trace_events


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c





clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


package nftrace

import (
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf"
	"log"
)

func readPerfEvents(mapName string, buffLen int) error {
	// Открываем карту
	eventsMap, err := ebpf.NewMap(&ebpf.MapSpec{
		Type:       ebpf.PerfEventArray,
		Name:       mapName,
		KeySize:    4,
		ValueSize:  4,
		MaxEntries: 8,
	})
	if err != nil {
		return err
	}
	defer eventsMap.Close()

	// Читаем события из карты
	reader, err := perf.NewReader(eventsMap, buffLen)
	if err != nil {
		return err
	}
	defer reader.Close()

	for {
		record, err := reader.Read()
		if err != nil {
			log.Println("Ошибка чтения события:", err)
			break
		}
		log.Printf("Получено событие: %+v", record)
	}
	return nil
}
