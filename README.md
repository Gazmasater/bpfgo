
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"os/exec"
	"syscall"
)

func main() {
	// Создаем новый network namespace
	if err := syscall.Unshare(syscall.CLONE_NEWNET); err != nil {
		log.Fatalf("failed to create network namespace: %v", err)
	}
	fmt.Println("Создано новое сетевое пространство")

	// Поднимаем loopback-интерфейс (иначе не будет DNS-запросов)
	if err := exec.Command("ip", "link", "set", "lo", "up").Run(); err != nil {
		log.Fatalf("failed to bring up loopback: %v", err)
	}
	fmt.Println("Loopback-интерфейс поднят")

	// Выполняем DNS-запрос в новом пространстве
	host := "google.com"
	ips, err := net.LookupIP(host)
	if err != nil {
		log.Fatalf("Ошибка LookupIP: %v", err)
	}
	fmt.Printf("IP-адреса для %s: %v\n", host, ips)
}

