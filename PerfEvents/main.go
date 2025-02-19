package main

import (
	"fmt"
	"log"
	"os"
	"path/filepath"

	"github.com/cilium/ebpf"
)

type TraceInfo struct {
	Pid   uint32
	SrcIP uint32
	Sport uint16
	Dport uint16
	Comm  [16]byte
}

func main() {
	// Получаем текущую рабочую директорию
	wd, err := os.Getwd()
	if err != nil {
		log.Fatalf("failed to get current working directory: %v", err)
	}

	// Путь к скомпилированному eBPF объекту
	eBpfFilePath := filepath.Join(filepath.Dir(wd), "generated", "bpf_x86_bpfel.o")
	fmt.Println("Loading eBPF object...")

	// Загружаем eBPF коллекцию
	objs, err := ebpf.LoadCollection(eBpfFilePath)
	if err != nil {
		log.Fatalf("failed to load eBPF collection: %v", err)
	}
	defer objs.Close()

	// Печатаем имена всех программ в коллекции для проверки
	fmt.Println("Loaded eBPF collection programs:")
	for name, program := range objs.Programs {
		fmt.Printf("Program: %s, Type: %v\n", name, program.Type())
	}

}
