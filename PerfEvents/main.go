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
		log.Fatalf("failed to load eBPF collection: %v", err)
	}
	defer objs.Close()

	fmt.Println("Loaded eBPF collection:")
	for name, obj := range objs.Maps {
		fmt.Printf("Map: %s\n", name)
		fmt.Printf("Map type: %v\n", obj.Type())
	}

	traceEventsMap, exists := objs.Maps["trace_events"]
	if !exists {
		log.Fatalf("map 'trace_events' not found in eBPF object")
	}
	fmt.Println("Map 'trace_events' found")

	buffLen := 40960
	rd, err := perf.NewReader(traceEventsMap, buffLen)
	if err != nil {
		log.Fatalf("opening ringbuf reader: %s", err)
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
			log.Printf("Error reading trace from reader: %v", err)
			break
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
