bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

sudo bpftool map dump id 11




11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map dump name trace_events
key:
00 00 00 00
value:
Unknown error 524
key:
01 00 00 00
value:
Unknown error 524
key:
02 00 00 00
value:
Unknown error 524
key:
03 00 00 00
value:
Unknown error 524
key:
04 00 00 00
value:
Unknown error 524
key:
05 00 00 00
value:
Unknown error 524
key:
06 00 00 00
value:
Unknown error 524
key:
07 00 00 00
value:
Unknown error 524
Found 0 elements



package main

import (
	"errors"
	"fmt"
	"log"
	"os"
	"path/filepath"

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
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}

	wd, err := os.Getwd()
	if err != nil {
		log.Fatalf("failed to get current working directory: %v", err)
	}

	eBpfFilePath := filepath.Join(filepath.Dir(wd), "generated", "bpf_x86_bpfel.o")

	fmt.Println("Loading eBPF object...")
	objs, err := ebpf.LoadCollection(eBpfFilePath)
	if err != nil {
		panic(fmt.Sprintf("failed to load eBPF collection: %v", err)) // Паника при ошибке загрузки eBPF объекта
	}
	defer objs.Close()

	fmt.Println("Loaded eBPF collection:")
	for name, obj := range objs.Maps {
		fmt.Printf("Map: %s\n", name)
		fmt.Printf("Map type: %v\n", obj.Type())
	}

	traceEventsMap, exists := objs.Maps["trace_events"]
	if !exists {
		panic("map 'trace_events' not found in eBPF object") // Паника, если не найдена карта 'trace_events'
	}
	fmt.Println("Map 'trace_events' found")

	buffLen := 40960
	rd, err := perf.NewReader(traceEventsMap, buffLen)
	if err != nil {
		panic(fmt.Sprintf("opening ringbuf reader: %s", err)) // Паника при ошибке открытия reader
	}
	defer rd.Close()
	record := new(perf.Record)

	fmt.Println("Start reading events from trace_events map")
	for {
		fmt.Println("Waiting for event...")
		err := rd.ReadInto(record)
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				fmt.Println("Timeout, no data available")
				continue
			}
			panic(fmt.Sprintf("Error reading trace from reader: %v", err)) // Паника при ошибке чтения
		}

		fmt.Println("Event read successfully!")

		var info TraceInfo
		data := record.RawSample
		copy(info.Comm[:], data[:16])
		info.Pid = uint32(data[16])
		info.SrcIP = uint32(data[20])
		info.DstIP = uint32(data[24])
		info.Sport = uint16(data[28])
		info.Dport = uint16(data[30])

		fmt.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, DstIP=%d.%d.%d.%d, Sport=%d, Dport=%d\n",
			info.Pid,
			info.Comm,
			(info.SrcIP>>24)&0xFF, (info.SrcIP>>16)&0xFF, (info.SrcIP>>8)&0xFF, info.SrcIP&0xFF,
			(info.DstIP>>24)&0xFF, (info.DstIP>>16)&0xFF, (info.DstIP>>8)&0xFF, info.DstIP&0xFF,
			info.Sport, info.Dport)
	}
}

