
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

package main

import (
	"log"
	"os"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

func main() {
	// Шаг 1: Загружаем программу типа BPF_SK_LOOKUP
	spec, err := ebpf.LoadCollectionSpec("prog.o") // Путь к объектному файлу eBPF
	if err != nil {
		log.Fatalf("failed to load eBPF program: %v", err)
	}

	var objs struct {
		SocketLookup *ebpf.Program `ebpf:"sk_lookup_prog"`
	}

	err = spec.LoadAndAssign(&objs, nil)
	if err != nil {
		log.Fatalf("failed to assign eBPF program: %v", err)
	}

	// Шаг 2: Открываем cgroup для получения файлового дескриптора
	cgroupPath := "/sys/fs/cgroup/net_cls/my_cgroup"
	f, err := os.Open(cgroupPath)
	if err != nil {
		log.Fatalf("failed to open cgroup path: %v", err)
	}
	defer f.Close()

	// Получаем файловый дескриптор для cgroup
	cgroupFd := int(f.Fd())

	// Получаем файловый дескриптор для программы
	programFd := objs.SocketLookup.FD()

	// Шаг 3: Создаем ссылку с attach_type BPF_SK_LOOKUP
	_, err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Program: objs.SocketLookup,  // Программа типа BPF_SK_LOOKUP
		Attach:  ebpf.AttachSkLookup, // Тип привязки sk_lookup
		Target:  cgroupFd,            // Файловый дескриптор cgroup
	})
	if err != nil {
		log.Fatalf("failed to attach BPF_SK_LOOKUP to cgroup: %v", err)
	}

	log.Println("Successfully attached eBPF program to cgroup")
}


