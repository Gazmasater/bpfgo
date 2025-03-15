

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

package main

import (
    "fmt"
    "log"
    "os/exec"
    "golang.org/x/net/ebpf"
    "github.com/cilium/ebpf/link"
)

func main() {
    // Путь к cgroup
    cgroupPath := "/sys/fs/cgroup/net_cls/my_cgroup"

    // Шаг 1: Создайте cgroup (если его нет)
    err := exec.Command("mkdir", "-p", cgroupPath).Run()
    if err != nil {
        log.Fatalf("failed to create cgroup: %v", err)
    }

    // Шаг 2: Установите classid для cgroup
    err = exec.Command("sh", "-c", "echo \"1:1\" > "+cgroupPath+"/net_cls.classid").Run()
    if err != nil {
        log.Fatalf("failed to set classid for cgroup: %v", err)
    }

    // Шаг 3: Привязываем программу BPF
    err = link.RawAttachProgram(link.RawAttachProgramOptions{
        Program: objs.EchoDispatch,  // Программа BPF
        Attach:  ebpf.AttachSkLookup, // Тип привязки sk_lookup
        Cgroup:  cgroupPath,         // Путь к cgroup
    })
    if err != nil {
        log.Fatalf("failed to attach sk_lookup: %v", err)
    }

    fmt.Println("Successfully attached BPF program to cgroup")
}

type RawAttachProgramOptions struct {
	// Target to query. This is usually a file descriptor but may refer to
	// something else based on the attach type.
	Target int
	// Program to attach.
	Program *ebpf.Program
	// Attach must match the attach type of Program.
	Attach ebpf.AttachType
	// Attach relative to an anchor. Optional.
	Anchor Anchor
	// Flags control the attach behaviour. Specify an Anchor instead of
	// F_LINK, F_ID, F_BEFORE, F_AFTER and F_REPLACE. Optional.
	Flags uint32
	// Only attach if the internal revision matches the given value.
	ExpectedRevision uint64
}
































