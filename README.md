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
	"os"
	"os/signal"
	"syscall"

	"bpfgo/generated"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
)

// Определяем структуру для хранения объектов
type bpfObjects struct {
	SendtoEnter *ebpf.Program `ebpf:"trace_sendto_enter"`
	SendtoExit  *ebpf.Program `ebpf:"trace_sendto_exit"`
}

func main() {
	// Увеличиваем лимит на количество блокировок памяти для загрузки eBPF-программы
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("Не удалось снять ограничение на блокировку памяти: %v", err)
	}

	// Загружаем eBPF-объектный файл в структуру bpfObjects
	var objs bpfObjects
	if err := generated.LoadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("Не удалось загрузить eBPF-объекты: %v", err)
	}
	defer objs.Close()

	// Прикрепляем программы к соответствующим tracepoint'ам
	sendtoEnterLink, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.SendtoEnter, nil)
	if err != nil {
		log.Fatalf("Не удалось прикрепить trace_sendto_enter: %v", err)
	}
	defer sendtoEnterLink.Close()

	sendtoExitLink, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.SendtoExit, nil)
	if err != nil {
		log.Fatalf("Не удалось прикрепить trace_sendto_exit: %v", err)
	}
	defer sendtoExitLink.Close()

	log.Println("eBPF-программы успешно загружены и прикреплены.")

	// Ожидаем сигнала завершения (Ctrl+C)
	sig := make(chan os.Signal, 1)
	signal.Notify(sig, syscall.SIGINT, syscall.SIGTERM)
	<-sig

	log.Println("Завершение работы.")
}

// loadBpfObjects загружает все объекты eBPF в переданную структуру

// Close закрывает все ресурсы, связанные с объектами eBPF
func (objs *bpfObjects) Close() {
	if objs.SendtoEnter != nil {
		objs.SendtoEnter.Close()
	}
	if objs.SendtoExit != nil {
		objs.SendtoExit.Close()
	}
}

[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredImportedName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredImportedName"
		}
	},
	"severity": 8,
	"message": "undefined: generated.LoadBpfObjects",
	"source": "compiler",
	"startLineNumber": 30,
	"startColumn": 22,
	"endLineNumber": 30,
	"endColumn": 36
}]
