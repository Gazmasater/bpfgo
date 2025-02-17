package main

import (
	load "load/generated"
	"log"
	"os"
	"os/signal"
	"syscall"

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
	if err := load.LoadBpfObjects(&objs, nil); err != nil {
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
