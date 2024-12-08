package main

import (
	"fmt"
	"log"
	"github.com/cilium/ebpf"
)

func main() {
	// Имя карты
	mapName := "conn_info_map_sc"

	// Найти карту по имени
	mapFD, err := ebpf.LoadMap(mapName)
	if err != nil {
		log.Fatalf("Failed to find map by name %s: %v", mapName, err)
	}
	defer mapFD.Close()

	fmt.Printf("Found map: %s\n", mapName)

	// Чтение данных из карты
	var key uint32 = 0
	var value ConnInfo
	err = mapFD.Lookup(key, &value)
	if err != nil {
		log.Fatalf("Failed to lookup map element: %v", err)
	}

	// Вывод результата
	fmt.Printf("PID: %d, Source IP: %d, Destination IP: %d\n", value.Pid, value.SrcIP, value.DstIP)
}
