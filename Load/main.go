package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
)

func main() {
	// Увеличиваем лимит на количество блокировок памяти для загрузки eBPF-программы
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("Не удалось снять ограничение на блокировку памяти: %v", err)
	}

	// Загружаем eBPF-объектный файл
	obj, err := ebpf.LoadCollection("trace_bpfel.o")
	if err != nil {
		log.Fatalf("Не удалось загрузить eBPF-коллекцию: %v", err)
	}
	defer obj.Close()

	// Получаем eBPF-программы из загруженной коллекции
	sendtoEnterProg, ok := obj.Programs["trace_sendto_enter"]
	if !ok {
		log.Fatalf("Программа trace_sendto_enter не найдена")
	}

	sendtoExitProg, ok := obj.Programs["trace_sendto_exit"]
	if !ok {
		log.Fatalf("Программа trace_sendto_exit не найдена")
	}

	// Прикрепляем программы к соответствующим tracepoint'ам
	sendtoEnterLink, err := link.Tracepoint("syscalls", "sys_enter_sendto", sendtoEnterProg, nil)
	if err != nil {
		log.Fatalf("Не удалось прикрепить trace_sendto_enter: %v", err)
	}
	defer sendtoEnterLink.Close()

	sendtoExitLink, err := link.Tracepoint("syscalls", "sys_exit_sendto", sendtoExitProg, nil)
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
