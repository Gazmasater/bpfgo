bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=nftrace -target amd64 bpf $(pwd)/nftrace_perf.c -- -I$(pwd)

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


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
    "log"
    "unsafe"
    "github.com/cilium/ebpf/perf"
    "github.com/pkg/errors"
    "os"
)

type bpfTraceInfo struct {
    PID    uint32
    SrcIP  uint32
    DstIP  uint32
    Sport  uint16
    Dport  uint16
    Comm   [16]byte
}

func main() {
    // Инициализация perf ридера
    rd, err := perf.NewReader(objs, buffLen)
    if err != nil {
        log.Fatalf("opening ringbuf reader: %s", err)
    }
    defer rd.Close()

    record := new(perf.Record)

    // Цикл чтения событий
Loop:
    for {
        err := rd.ReadInto(record)
        if err != nil {
            if errors.Is(err, os.ErrDeadlineExceeded) {
                continue
            }
            e := errors.WithMessage(err, "reading trace from reader")
            log.Printf("%v", e)
            break
        }

        // Преобразование сырых данных в структуру bpfTraceInfo
        event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))

        // Обработка события (например, вывод на экран)
        log.Printf("Event received: PID: %d, SrcIP: %d, DstIP: %d, Sport: %d, Dport: %d, Comm: %s",
            event.PID, event.SrcIP, event.DstIP, event.Sport, event.Dport, string(event.Comm[:]))
    }
}

