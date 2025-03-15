

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

package main

import (
    "fmt"
    "log"
    "os"
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

    // Шаг 3: Открыть cgroup для получения дескриптора
    f, err := os.Open(cgroupPath)
    if err != nil {
        log.Fatalf("failed to open cgroup path: %v", err)
    }
    defer f.Close()

    // Получаем дескриптор cgroup (это поле 'Target' в RawAttachProgramOptions)
    cgroupFd := int(f.Fd())

    // Шаг 4: Привязать программу BPF
    err = link.RawAttachProgram(link.RawAttachProgramOptions{
        Program: objs.EchoDispatch,   // Программа BPF
        Attach:  ebpf.AttachSkLookup, // Тип привязки sk_lookup
        Target:  cgroupFd,            // Дескриптор cgroup
    })
    if err != nil {
        log.Fatalf("failed to attach sk_lookup: %v", err)
    }

    fmt.Println("Successfully attached BPF program to cgroup")
}

































