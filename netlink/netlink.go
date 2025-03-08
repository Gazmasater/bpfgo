package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"net"

	"github.com/mdlayher/netlink"
	"golang.org/x/sys/unix"
)

// inetDiagMsg структура для декодирования ответа
type inetDiagMsg struct {
	Family  uint8
	State   uint8
	Timer   uint8
	Retrans uint8
	ID      inetDiagSockID
}

// inetDiagSockID содержит информацию о соединении
type inetDiagSockID struct {
	SPort  uint16
	DPort  uint16
	Src    [4]byte // IPv4
	Dst    [4]byte // IPv4
	Iface  uint32
	Cookie [8]uint8
}

func main() {
	// Открытие соединения с netlink
	conn, err := netlink.Dial(unix.NETLINK_INET_DIAG, nil)
	if err != nil {
		log.Fatalf("Ошибка подключения к netlink: %v", err)
	}
	defer conn.Close()

	// PID процесса, для которого ищем соединение
	const pid = 5198

	// Формируем запрос для получения всех соединений (TCP)
	req := netlink.Message{
		Header: netlink.Header{
			Type:  netlink.HeaderType(unix.NETLINK_INET_DIAG), // Прямой запрос на получение соединений
			Flags: netlink.Request | netlink.Dump,
		},
		// Пример запроса для получения всех соединений (IPv4, TCP)
		Data: []byte{
			2,                // AF_INET (IPv4)
			unix.IPPROTO_TCP, // Протокол TCP
			0, 0, 0, 0, 0, 0, // Пустые данные
		},
	}

	fmt.Println("Отправляем запрос в netlink...")
	fmt.Println("DATA", req.Data)

	// Отправляем запрос
	_, err = conn.Send(req)
	if err != nil {
		log.Fatalf("Ошибка отправки запроса: %v", err)
	}

	fmt.Println("Запрос отправлен, ожидаем ответ...")

	// Читаем ответ
	msgs, err := conn.Receive()
	if err != nil {
		log.Fatalf("Ошибка чтения ответа: %v", err)
	}

	fmt.Println("Ответ получен!")

	// Разбираем ответ
	for _, m := range msgs {
		if len(m.Data) < 20 {
			continue
		}

		var diag inetDiagMsg
		reader := bytes.NewReader(m.Data)
		err := binary.Read(reader, binary.LittleEndian, &diag)
		if err != nil {
			log.Printf("Ошибка декодирования сообщения: %v", err)
			continue
		}

		// Преобразуем данные в читаемый формат
		srcIP := net.IP(diag.ID.Src[:])
		srcPort := binary.LittleEndian.Uint16(m.Data[8:10])

		// Выводим информацию о процессе и соединении
		fmt.Printf("Процесс %d использует IP: %s, Port: %d\n", pid, srcIP, srcPort)
		fmt.Printf("Диагностика соединения: %+v\n", diag)
	}
}
