lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


package main

import (
	"fmt"
	"os"
	"log"
	"os/exec"
	"syscall"
	"time"
)

func main() {
	// Чтение символической ссылки, указывающей на неймспейс
	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		panic(err)
	}
	defer netns.Close()

	// Применяем пространство имен для текущего процесса
	err = syscall.Setns(int(netns.Fd()), syscall.CLONE_NEWNET)
	if err != nil {
		log.Fatalf("Ошибка при применении нового сетевого пространства имен: %v", err)
	}

	// Запуск сервера nc
	serverCmd := exec.Command("nc", "-l", "127.0.0.1", "12345")
	serverCmd.Stdout = os.Stdout
	serverCmd.Stderr = os.Stderr

	// Запуск серверной части в новом пространстве имен
	err = serverCmd.Start()
	if err != nil {
		log.Fatalf("Ошибка при запуске сервера nc: %v", err)
	}
	fmt.Println("Сервер nc запущен на порту 12345.")

	// Даем серверу немного времени на запуск
	time.Sleep(1 * time.Second)

	// Запуск клиента nc, который подключается к серверу
	clientCmd := exec.Command("nc", "127.0.0.1", "12345")
	clientCmd.Stdout = os.Stdout
	clientCmd.Stderr = os.Stderr

	// Запуск клиентской части
	err = clientCmd.Start()
	if err != nil {
		log.Fatalf("Ошибка при запуске клиента nc: %v", err)
	}
	fmt.Println("Клиент nc подключается к серверу.")

	// Ожидаем завершения обеих команд
	err = serverCmd.Wait()
	if err != nil {
		log.Fatalf("Ошибка при завершении сервера nc: %v", err)
	}

	err = clientCmd.Wait()
	if err != nil {
		log.Fatalf("Ошибка при завершении клиента nc: %v", err)
	}

	fmt.Println("Сервер и клиент завершили работу.")
}

