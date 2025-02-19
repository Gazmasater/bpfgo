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



package main

import (
	"fmt"
	"log"
	"github.com/pkg/errors"
)

type bpfObjects struct {
	Programs map[string]*ebpf.Program
	Maps     map[string]*ebpf.Map
}

func init() {
	// Снимаем ограничение на память
	if err := rlimit.RemoveMemlock(); err != nil {
		panic(errors.WithMessage(err, "failed to remove memory limit for process"))
	}

	// Инициализируем объекты eBPF
	objs := bpfObjects{}
	if err := loadBpfObjects(&objs, loadOpts); err != nil {
		panic(errors.WithMessage(err, "failed to load bpf objects"))
	}

	// Печатаем все программы из коллекции
	fmt.Println("Loaded eBPF collection programs:")
	for name, program := range objs.Programs {
		fmt.Printf("Program: %s, Type: %v\n", name, program.Type())
	}
}

func main() {
	// Основная логика программы
	// init() уже выполнена, и eBPF объекты были загружены и напечатаны.
	fmt.Println("Main function started.")
}


[{
	"resource": "/home/gaz358/myprog/bpfgo/PerfEvents/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredName"
		}
	},
	"severity": 8,
	"message": "undefined: loadOpts",
	"source": "compiler",
	"startLineNumber": 25,
	"startColumn": 40,
	"endLineNumber": 25,
	"endColumn": 48
}]
