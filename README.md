gcc udp_server.c -o udp_server

package main

import (
	"fmt"
	"log"
	"net"
	"syscall"
	"unsafe"
)

const (
	NETLINK_INET_DIAG = 4
	IPPROTO_TCP       = 6
)

type InetDiagReqV2 struct {
	Family   uint8
	Protocol uint8
	Pad      uint16
	Scope    uint32
	Filter   uint32
	Flags    uint32
	Timeout  uint32
	RecvQ    uint32
	SendQ    uint32
	IDs      [4]uint32
}

type InetDiagMsg struct {
	State    uint8
	Pad      uint8
	Flags    uint16
	Family   uint8
	Protocol uint8
	AddrLen  uint16
	Port     uint16
	DstPort  uint16
}

func getNetlinkSocket() (int, error) {
	fd, err := syscall.Socket(syscall.AF_NETLINK, syscall.SOCK_RAW, syscall.NETLINK_INET_DIAG)
	if err != nil {
		return -1, fmt.Errorf("failed to create netlink socket: %v", err)
	}
	return fd, nil
}

func getLocalConnections() ([]InetDiagMsg, error) {
	fd, err := getNetlinkSocket()
	if err != nil {
		return nil, err
	}
	defer syscall.Close(fd)

	// Создание запроса
	req := InetDiagReqV2{
		Family:   syscall.AF_INET,
		Protocol: IPPROTO_TCP,
	}

	// Отправка запроса
	err = syscall.Sendto(fd, (*[unsafe.Sizeof(req)]byte)(unsafe.Pointer(&req))[:], 0, &syscall.SockaddrNetlink{Family: syscall.AF_NETLINK})
	if err != nil {
		return nil, fmt.Errorf("failed to send netlink message: %v", err)
	}

	// Чтение ответа
	buf := make([]byte, 4096) // Попробуем с более меньшим размером
	n, _, err := syscall.Recvfrom(fd, buf, 0)
	if err != nil {
		return nil, fmt.Errorf("failed to read netlink response: %v", err)
	}

	if n == 0 {
		return nil, fmt.Errorf("no data received from netlink")
	}

	// Преобразуем данные ответа в структуру
	var connections []InetDiagMsg
	for i := 0; i < n; i += int(unsafe.Sizeof(InetDiagMsg{})) {
		conn := *(*InetDiagMsg)(unsafe.Pointer(&buf[i]))
		connections = append(connections, conn)
	}

	return connections, nil
}

func main() {
	conns, err := getLocalConnections()
	if err != nil {
		log.Fatalf("Error: %v", err)
	}

	// Если соединений нет, выводим информацию
	if len(conns) == 0 {
		fmt.Println("No active TCP connections found.")
	} else {
		for _, conn := range conns {
			// Используем поля DstPort и AddrLen для более точного вывода
			ip := net.IPv4(byte(conn.State), byte(conn.Port), byte(conn.DstPort), byte(conn.Flags))
			fmt.Printf("Local IP: %s Port: %d State: %d\n", ip, conn.Port, conn.State)
		}
	}
}
