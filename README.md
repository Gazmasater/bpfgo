sudo ip netns list



nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


package main

import (
	"fmt"
	"log"
	"os"
	"syscall"

	"github.com/vishvananda/netlink"
)

func CreateVethPair(namespaceFD uintptr) error {
	// Создаем veth-пару
	linkAttrs := netlink.NewLinkAttrs()
	linkAttrs.Name = "veth0"
	veth0 := &netlink.Veth{
		LinkAttrs: linkAttrs,
		PeerName:  "veth1",
	}

	// Добавляем veth-пару в текущий namespace
	if err := netlink.LinkAdd(veth0); err != nil {
		return fmt.Errorf("не удалось создать veth интерфейс: %w", err)
	}

	// Включаем veth0
	if err := netlink.LinkSetUp(veth0); err != nil {
		return fmt.Errorf("не удалось включить veth0: %w", err)
	}

	// Получаем ссылку на veth1
	peerLink, err := netlink.LinkByName("veth1")
	if err != nil {
		return fmt.Errorf("не удалось найти интерфейс veth1: %w", err)
	}

	// Переносим veth1 в другой namespace
	if err := netlink.LinkSetNsFd(peerLink, int(namespaceFD)); err != nil {
		return fmt.Errorf("не удалось переместить veth1 в namespace: %w", err)
	}

	return nil
}

func main() {
	// Создаем новый network namespace
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

	// Создаем veth-пару и переносим veth1 в новый namespace
	if err := CreateVethPair(newNS.Fd()); err != nil {
		log.Fatalf("Ошибка создания veth-пары: %v", err)
	}

	// Теперь можно привязать sk_lookup к veth1 в новом namespace
	fmt.Println("Теперь настройте sk_lookup в новом namespace")
}

















