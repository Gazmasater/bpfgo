package main

import (
	gener "bpfgo/generated"
	"fmt"
	"log"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
	"github.com/pkg/errors"
)

type bpfObjects struct {
	Programs map[string]*ebpf.Program
	Maps     map[string]*ebpf.Map
}

func init() {

	var loadOpts = &ebpf.CollectionOptions{}

	// Снимаем ограничение на память
	if err := rlimit.RemoveMemlock(); err != nil {
		panic(errors.WithMessage(err, "failed to remove memory limit for process"))
	}

	// Инициализируем объекты eBPF
	objs := bpfObjects{}
	if err := gener.LoadBpfObjects(&objs, loadOpts); err != nil {
		panic(errors.WithMessage(err, "failed to load bpf objects"))
	}
	defer objs.Close()

	kpEnter, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.Programs["trace_sendto_enter"], nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendto: %s", err)
	}
	defer kpEnter.Close()

	// Печатаем все программы из коллекции
	fmt.Println("Loaded eBPF collection programs:")
	for name, program := range objs.Programs {
		fmt.Printf("Program: %s, Type: %v\n", name, program.Type())
	}
}

func main() {
	fmt.Println("Main function started.")

}
