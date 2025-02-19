package main

import (
	"fmt"
	"log"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
	"github.com/pkg/errors"
)

var objs = target_amd64_bpfObjects{}

func init() {
	var loadOpts = &ebpf.CollectionOptions{}

	// Снимаем ограничение на память
	if err := rlimit.RemoveMemlock(); err != nil {
		panic(errors.WithMessage(err, "failed to remove memory limit for process"))
	}

	// Инициализируем объекты eBPF

	if err := loadTarget_amd64_bpfObjects(&objs, loadOpts); err != nil {
		panic(errors.WithMessage(err, "failed to load bpf objects"))
	}

}

func main() {

	kpEnter, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter, nil)

	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendto: %s", err)
	}
	defer kpEnter.Close()

	fmt.Println(" kpEnter", kpEnter)

	kpExit, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.TraceSendtoExit, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_sendto: %s", err)
	}
	defer kpExit.Close()

	fmt.Println(" kpExit", kpExit)

	// Печатаем сообщение о привязке
	fmt.Println(" !!!!!")

}
