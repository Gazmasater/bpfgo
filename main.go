package main

import (
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

func main() {
	spec, err := ebpf.LoadCollectionSpec("kprobe.o")
	if err != nil {
		log.Fatalf("Ошибка загрузки eBPF программы: %v", err)
	}

	// коллекцию программ и карт
	collection, err := ebpf.NewCollection(spec)
	if err != nil {
		log.Fatalf("Ошибка создания коллекции: %v", err)
	}
	defer collection.Close()

	for name, program := range collection.Programs {
		fmt.Printf("Программа: %s\n", name)

		err := attachKprobe(program, "sys_accept4", false)
		if err != nil {
			log.Fatalf("Ошибка привязки kprobe: %v", err)
		}

		err = attachKprobe(program, "sys_accept4", true)
		if err != nil {
			log.Fatalf("Ошибка привязки kretprobe: %v", err)
		}

		// Привязываем программу к событию kprobe для sys_bind
		err = attachKprobe(program, "sys_bind", false)
		if err != nil {
			log.Fatalf("Ошибка привязки kprobe: %v", err)
		}

		err = attachKprobe(program, "sys_bind", true)
		if err != nil {
			log.Fatalf("Ошибка привязки kretprobe: %v", err)
		}

		// Привязываем программу к событию kprobe для sys_connect
		err = attachKprobe(program, "sys_connect", false)
		if err != nil {
			log.Fatalf("Ошибка привязки kprobe: %v", err)
		}

		err = attachKprobe(program, "sys_connect", true)
		if err != nil {
			log.Fatalf("Ошибка привязки kretprobe: %v", err)
		}

		// Прерываем цикл, если привязка прошла успешно
		break
	}

	fmt.Println("Программа привязана к событиям. Используйте 'bpftool prog show' для проверки.")
	// Канал для захвата сигналов
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)

	// Ожидание сигнала
	sig := <-sigs
	fmt.Printf("Получен сигнал %s, завершение работы.\n", sig)
}

// attachKprobe привязывает eBPF-программу к событию kprobe
func attachKprobe(program *ebpf.Program, event string, isRetprobe bool) error {
	var kprobeLink link.Link
	var err error

	if isRetprobe {
		// Привязываем программу к kretprobe событию
		kprobeLink, err = link.Kretprobe(event, program, nil)
		if err != nil {
			return fmt.Errorf("ошибка привязки kretprobe к %s: %v", event, err)
		}
		fmt.Printf("Программа успешно привязана к событию kretprobe %s\n", event)
	} else {
		// Привязываем программу к kprobe событию
		kprobeLink, err = link.Kprobe(event, program, nil)
		if err != nil {
			return fmt.Errorf("ошибка привязки kprobe к %s: %v", event, err)
		}
		fmt.Printf("Программа успешно привязана к событию kprobe %s\n", event)
	}

	// Закрытие ссылки будет выполнено после завершения работы программы
	defer kprobeLink.Close()

	return nil
}
