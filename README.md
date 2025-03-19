
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
	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

// Глобальная переменная для хранения объектов eBPF
var objs struct {
	LookUp *ebpf.Program `ebpf:"sk_lookup"` // Пример загрузки программы sk_lookup
}

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
	// Шаг 1: Создание нового сетевого пространства
	if err := syscall.Unshare(syscall.CLONE_NEWNET); err != nil {
		log.Fatalf("Ошибка создания нового network namespace: %v", err)
	}
	fmt.Println("Создано новое сетевое пространство")

	// Шаг 2: Открываем дескриптор нового namespace
	newNS, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("Ошибка открытия дескриптора нового namespace: %v", err)
	}
	defer newNS.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", newNS.Fd())

	// Шаг 3: Создание интерфейса в новом сетевом пространстве
	// Создаем виртуальную пару интерфейсов (veth)
	if err := createVethPair(newNS.Fd()); err != nil {
		log.Fatalf("Ошибка создания интерфейса: %v", err)
	}

	// Шаг 4: Привязываем BPF-программу к сетевому неймспейсу
	skLookupLink, err := link.AttachNetNs(int(newNS.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("Ошибка привязки программы sk_lookup: %v", err)
	}
	defer skLookupLink.Close()

	fmt.Println("Программа sk_lookup успешно привязана")
}

// Функция для создания виртуальной пары интерфейсов (veth)
func createVethPair(namespaceFD uintptr) error {
	// Используем netlink для создания пары интерфейсов veth
	linkAttrs := netlink.NewLinkAttrs()
	linkAttrs.Name = "veth0"
	veth0 := &netlink.Veth{
		LinkAttrs: linkAttrs,
		PeerName:  "veth1",
	}

	// Создаем интерфейс veth в текущем неймспейсе
	if err := netlink.LinkAdd(veth0); err != nil {
		return fmt.Errorf("не удалось создать veth интерфейс: %w", err)
	}

	// Включаем интерфейсы
	if err := netlink.LinkSetUp(veth0); err != nil {
		return fmt.Errorf("не удалось включить интерфейс veth0: %w", err)
	}

	// Включаем интерфейс veth1
	peerLink, err := netlink.LinkByName("veth1")
	if err != nil {
		return fmt.Errorf("не удалось найти интерфейс veth1: %w", err)
	}

	if err := netlink.LinkSetUp(peerLink); err != nil {
		return fmt.Errorf("не удалось включить интерфейс veth1: %w", err)
	}

	return nil
}

// Загрузка объектов eBPF (предположим, что ваши объекты загружаются здесь)
func loadBpfObjects(objs *struct{ LookUp *ebpf.Program }, opts interface{}) error {
	// Здесь нужно загрузить ваши eBPF-объекты
	// Это пример, вам нужно указать путь к файлу или структуру с объектами eBPF
	return nil
}















