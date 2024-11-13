package main

import (
	"fmt"
	"log"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

func main() {
	// Загружаем eBPF-программу из файла
	spec, err := ebpf.LoadCollectionSpec("program.o")
	if err != nil {
		log.Fatalf("Ошибка загрузки eBPF программы: %v", err)
	}

	// Создаем коллекцию программ и карт
	collection, err := ebpf.NewCollection(spec)
	if err != nil {
		log.Fatalf("Ошибка создания коллекции: %v", err)
	}
	defer collection.Close()

	// Привязываем программы к нужным системным вызовам (например, sys_accept4, sys_bind, sys_connect)
	for name, program := range collection.Programs {
		fmt.Printf("Программа: %s\n", name)

		// Привязываем программу к событию kprobe для sys_accept4
		err := attachKprobe(program, "sys_accept4")
		if err != nil {
			log.Fatalf("Ошибка привязки программы %s к sys_accept4: %v", name, err)
		}

		// Привязываем программу к событию kprobe для sys_bind
		err = attachKprobe(program, "sys_bind")
		if err != nil {
			log.Fatalf("Ошибка привязки программы %s к sys_bind: %v", name, err)
		}

		// Привязываем программу к событию kprobe для sys_connect
		err = attachKprobe(program, "sys_connect")
		if err != nil {
			log.Fatalf("Ошибка привязки программы %s к sys_connect: %v", name, err)
		}

		// Прерываем цикл, если привязка прошла успешно
		break
	}

	fmt.Println("Программа привязана к событиям. Используйте 'bpftool prog show' для проверки.")
	select {} // Блокируем программу, чтобы привязка оставалась активной
}

// attachKprobe привязывает eBPF-программу к событию kprobe
func attachKprobe(program *ebpf.Program, event string) error {
	// Привязываем программу kprobe к событию ядра
	kprobeLink, err := link.Kprobe(link.KprobeOptions{
		Program: program,
		Event:   event, // Указываем точку события (например, "sys_accept4")
	})
	if err != nil {
		return fmt.Errorf("ошибка привязки kprobe к %s: %v", event, err)
	}
	defer kprobeLink.Close()

	fmt.Printf("Программа успешно привязана к событию %s\n", event)
	return nil
}
