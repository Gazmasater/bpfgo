package main

import (
	"fmt"
	"log"
	"os"
	"syscall"

	"github.com/cilium/ebpf"
)

type ConnInfo struct {
	SockAddr syscall.Sockaddr // В Go используем Sockaddr для представления сокет-адресов
	Pid      uint32
	SrcIP    uint32
	DstIP    uint32
	Addrlen  uint32
	Sport    uint16
	Dport    uint16
	Call     uint8
	Protocol uint8
	Comm     [16]byte
}

func main() {
	// Путь к карте BPF
	mapPathSC := "/sys/fs/bpf/my_map_sc" // Путь к карте conn_info_map_sc
	mapPathRA := "/sys/fs/bpf/my_map_ra" // Путь к карте conn_info_map_ra

	// Открытие карты BPF (для conn_info_map_sc)
	mapFD, err := ebpf.LoadPinnedMap(mapPathSC, nil)
	if err != nil {
		log.Fatalf("failed to open BPF map: %v", err)
		os.Exit(1)
	}
	defer mapFD.Close()

	// Чтение данных из карты conn_info_map_sc
	var keySC uint32 = 0 // Пример ключа, используйте подходящий для вашего случая
	var connInfoSC ConnInfo
	err = mapFD.Lookup(keySC, &connInfoSC)
	if err != nil {
		log.Fatalf("failed to lookup map element (conn_info_map_sc): %v", err)
	}
	printConnInfo(&connInfoSC)

	// Открытие карты BPF (для conn_info_map_ra)
	mapFD, err = ebpf.LoadPinnedMap(mapPathRA, nil)
	if err != nil {
		log.Fatalf("failed to open BPF map: %v", err)
		os.Exit(1)
	}
	defer mapFD.Close()

	// Чтение данных из карты conn_info_map_ra
	var keyRA uint32 = 1 // Пример ключа, используйте подходящий для вашего случая
	var connInfoRA ConnInfo
	err = mapFD.Lookup(keyRA, &connInfoRA)
	if err != nil {
		log.Fatalf("failed to lookup map element (conn_info_map_ra): %v", err)
	}
	printConnInfo(&connInfoRA)
}

// Функция для вывода данных из структуры ConnInfo
func printConnInfo(connInfo *ConnInfo) {
	fmt.Printf("PID: %d\n", connInfo.Pid)
	fmt.Printf("Source IP: %d.%d.%d.%d\n",
		(connInfo.SrcIP>>24)&0xFF, (connInfo.SrcIP>>16)&0xFF,
		(connInfo.SrcIP>>8)&0xFF, connInfo.SrcIP&0xFF)
	fmt.Printf("Destination IP: %d.%d.%d.%d\n",
		(connInfo.DstIP>>24)&0xFF, (connInfo.DstIP>>16)&0xFF,
		(connInfo.DstIP>>8)&0xFF, connInfo.DstIP&0xFF)
	fmt.Printf("Source Port: %d\n", connInfo.Sport)
	fmt.Printf("Destination Port: %d\n", connInfo.Dport)
	fmt.Printf("Call: %d\n", connInfo.Call)
	fmt.Printf("Protocol: %d\n", connInfo.Protocol)
	fmt.Printf("Comm: %s\n", connInfo.Comm[:])
}
