
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



package main

import (
	"fmt"
	"log"
	"os"
	"syscall"
)

func main() {
	// Создаем новое сетевое пространство
	if err := syscall.Unshare(syscall.CLONE_NEWNET); err != nil {
		log.Fatalf("Ошибка создания нового network namespace: %v", err)
	}
	fmt.Println("Создано новое сетевое пространство")

	// Открываем дескриптор нового namespace
	newNS, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("Ошибка открытия дескриптора нового namespace: %v", err)
	}
	defer newNS.Close()
	fmt.Printf("Дескриптор нового namespace: %d\n", newNS.Fd())

	// --- Если нужно, можно вернуться в исходное пространство ---
	origNS, err := os.Open("/proc/1/ns/net") // Открываем оригинальный namespace (PID 1)
	if err != nil {
		log.Fatalf("Ошибка открытия оригинального namespace: %v", err)
	}
	defer origNS.Close()

	// Переключаемся обратно в оригинальный namespace
	if err := syscall.Setns(int(origNS.Fd()), syscall.CLONE_NEWNET); err != nil {
		log.Fatalf("Ошибка возврата в оригинальный namespace: %v", err)
	}
	fmt.Println("Вернулись в исходное сетевое пространство")
}


