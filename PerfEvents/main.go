package main

import (
	gener "bpfgo/generated"
	"fmt"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/rlimit"
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
	if err := gener.LoadBpfObjects(&objs, loadOpts); err != nil {
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
