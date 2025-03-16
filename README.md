
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

package main

import (
	"fmt"
	"log"
	"os"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

func main() {
	// Открываем файловый дескриптор для сетевого пространства имен
	targetFD, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("failed to open network namespace: %v", err)
	}
	defer targetFD.Close()
	fmt.Println("Opened network namespace successfully.")

	// Проверяем, загружена ли программа BPF
	if objs.EchoDispatch == nil {
		log.Fatal("BPF program EchoDispatch is nil")
	}

	// Получаем дескриптор программы
	progFD := objs.EchoDispatch.FD()
	fmt.Printf("Program FD: %d\n", progFD)
	if progFD <= 0 {
		log.Fatalf("invalid program FD: %d", progFD)
	}

	// Проверяем тип загруженной программы BPF
	info, err := objs.EchoDispatch.Info()
	if err != nil {
		log.Fatalf("failed to get program info: %v", err)
	}
	fmt.Printf("Program Type: %v\n", info.Type)

	// Привязываем программу BPF к пространству имен сети
	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  int(targetFD.Fd()),
		Program: objs.EchoDispatch,
		Attach:  ebpf.AttachSkLookup,
	})

	if err != nil {
		log.Fatalf("failed to attach program: %v", err)
	}

	fmt.Println("BPF program attached successfully.")
}




