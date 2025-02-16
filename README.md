

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
	"os"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
	"github.com/pkg/errors"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
)

// Структура для хранения информации о событии
type trace_info struct {
	PID    uint32
	SrcIP  uint32
	DstIP  uint32
	Sport  uint16
	Dport  uint16
	Comm   [16]byte
}

// Структура для хранения BPF объектов
type bpfObjects struct {
	SendToEnter *ebpf.Program
	SendToExit  *ebpf.Program
}

func init() {
	// Убираем ограничения на память
	if err := rlimit.RemoveMemlock(); err != nil {
		panic(errors.WithMessage(err, "failed to remove memory limit for process"))
	}
}

// Загружаем BPF объекты
func loadBpfObjects(objs *bpfObjects, loadOpts ebpf.LoadOptions) error {
	// Открываем файл с BPF программой
	prog, err := ebpf.NewProgram(&ebpf.ProgramSpec{
		Type:       ebpf.TracePoint,
		AttachType: ebpf.AttachTracepoint,
	})
	if err != nil {
		return errors.Wrap(err, "failed to create BPF program")
	}

	// Загружаем BPF программы для sys_enter_sendto и sys_exit_sendto
	objs.SendToEnter, err = prog.Load(loadOpts)
	if err != nil {
		return errors.Wrap(err, "failed to load SendToEnter program")
	}

	objs.SendToExit, err = prog.Load(loadOpts)
	if err != nil {
		return errors.Wrap(err, "failed to load SendToExit program")
	}

	return nil
}

func main() {
	// Инициализация BPF объектов
	var objs bpfObjects
	if err := loadBpfObjects(&objs, ebpf.LoadOptions{}); err != nil {
		log.Fatalf("failed to load BPF objects: %v", err)
	}
	defer func() {
		if err := objs.SendToEnter.Close(); err != nil {
			log.Fatalf("failed to close SendToEnter: %v", err)
		}
		if err := objs.SendToExit.Close(); err != nil {
			log.Fatalf("failed to close SendToExit: %v", err)
		}
	}()

	// Линковка tracepoint для sys_enter_sendto
	kp, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.SendToEnter, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendto: %s", err)
	}
	defer kp.Close()

	// Линковка tracepoint для sys_exit_sendto
	kpExit, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.SendToExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_exit_sendto: %s", err)
	}
	defer kpExit.Close()

	// Создание perf reader для чтения событий
	rd, err := perf.NewReader(objs.SendToEnter, 4096) // Размер буфера
	if err != nil {
		log.Fatalf("opening ringbuf reader: %s", err)
	}
	defer rd.Close()

	// Прочитаем данные из perf событий
	record := new(perf.Record)
	for {
		err := rd.ReadInto(record)
		if err != nil {
			if errors.Is(err, os.ErrDeadlineExceeded) {
				continue
			}
			log.Fatalf("reading trace from reader: %s", err)
		}

		// Преобразуем прочитанное событие в нужный тип
		var event trace_info
		event = *(*trace_info)(unsafe.Pointer(&record.RawSample[0]))

		// Обработка события
		log.Printf("Received event: PID=%d, Comm=%s, SrcIP=%d.%d.%d.%d, Sport=%d\n",
			event.PID,
			string(event.Comm[:]),
			(event.SrcIP>>24)&0xFF, (event.SrcIP>>16)&0xFF, (event.SrcIP>>8)&0xFF, event.SrcIP&0xFF,
			event.Sport)
	}
}
