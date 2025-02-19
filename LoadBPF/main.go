package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"

	gener "bpfgo/generated"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
)

type bpfObjects struct {
	SendtoEnter *ebpf.Program `ebpf:"trace_sendto_enter"`
	SendtoExit  *ebpf.Program `ebpf:"trace_sendto_exit"`
}

func main() {

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("Не удалось снять ограничение на блокировку памяти: %v", err)
	}

	var objs bpfObjects
	if err := gener.LoadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("Не удалось загрузить eBPF-объекты: %v", err)
	}
	defer objs.Close()

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

	sig := make(chan os.Signal, 1)
	signal.Notify(sig, syscall.SIGINT, syscall.SIGTERM)
	<-sig

	log.Println("Завершение работы.")
}

func (objs *bpfObjects) Close() {
	if objs.SendtoEnter != nil {
		objs.SendtoEnter.Close()
	}
	if objs.SendtoExit != nil {
		objs.SendtoExit.Close()
	}
}
