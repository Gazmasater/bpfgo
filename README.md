
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

package main

import (
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/rlimit"
)

// Глобальные объекты BPF
var objs bpfObjects

func init() {
	// Снимаем ограничение на память
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit for process: %v", err)
	}

	// Загружаем eBPF-объекты
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

func main() {
	defer objs.Close() // Закроем объекты при выходе

	// Получаем файловый дескриптор программы BPF (prog_fd)
	progFD := objs.EchoDispatch.FD()

	// Получаем файловый дескриптор сетевого пространства имен (target_fd)
	// Для примера используем сетевое пространство по умолчанию
	targetFD, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("failed to open network namespace: %v", err)
	}
	defer targetFD.Close()

	// Создаем ссылку для привязки программы с типом BPF_SK_LOOKUP
	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		ProgFD:    progFD,      // Файловый дескриптор программы BPF
		TargetFD:  int(targetFD.Fd()), // Файловый дескриптор целевого сетевого пространства
		Attach:    ebpf.AttachSkLookup, // Тип привязки для sk_lookup
		AttachType: ebpf.AttachSkLookup,
	})
	if err != nil {
		log.Fatalf("failed to attach sk_lookup: %v", err)
	}

	// Канал для завершения работы
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	// Запускаем цикл, который будет работать до получения сигнала завершения
	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}



