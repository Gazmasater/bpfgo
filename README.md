
import (
    // ...
    "github.com/cilium/ebpf/ringbuf"
    // ...
)

// ...

go func() {
    rd, err := ringbuf.NewReader(objs.TraceEvents)
    if err != nil {
        log.Fatalf("failed to create ringbuf reader: %s", err)
    }
    defer rd.Close()

    executableName := os.Args[0]
    if len(executableName) > 2 {
        executableName = executableName[2:]
    }

    for {
        record, err := rd.Read()
        if err != nil {
            if errors.Is(err, os.ErrClosed) {
                break
            }
            log.Printf("error reading from ringbuf: %v", err)
            continue
        }

        if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
            log.Println("invalid event size")
            continue
        }
        event := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
        // ... твой обработчик событий ...
    }
}()

