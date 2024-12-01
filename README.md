sudo bpftrace -l


package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	// Определяем адрес для сервера
	address := ":8080" // Сервер будет слушать на всех интерфейсах на порту 8080

	// Создаем UDP-адрес
	udpAddr, err := net.ResolveUDPAddr("udp", address)
	if err != nil {
		fmt.Printf("Ошибка при разрешении адреса: %v\n", err)
		os.Exit(1)
	}

	// Создаем UDP-сокет
	conn, err := net.ListenUDP("udp", udpAddr)
	if err != nil {
		fmt.Printf("Ошибка при создании сокета: %v\n", err)
		os.Exit(1)
	}
	defer conn.Close()

	fmt.Printf("UDP-сервер запущен на %s\n", address)

	// Буфер для приема данных
	buffer := make([]byte, 1024)

	for {
		// Читаем данные из сокета
		n, clientAddr, err := conn.ReadFromUDP(buffer)
		if err != nil {
			fmt.Printf("Ошибка при чтении данных: %v\n", err)
			continue
		}

		// Логируем полученные данные
		fmt.Printf("Получено сообщение от %s: %s\n", clientAddr.String(), string(buffer[:n]))

		// Отправляем ответ клиенту
		response := []byte("Сообщение получено")
		_, err = conn.WriteToUDP(response, clientAddr)
		if err != nil {
			fmt.Printf("Ошибка при отправке данных: %v\n", err)
		}
	}
}


package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	// Адрес сервера
	serverAddr := "127.0.0.1:8080"

	// Создаем UDP-адрес для сервера
	udpAddr, err := net.ResolveUDPAddr("udp", serverAddr)
	if err != nil {
		fmt.Printf("Ошибка при разрешении адреса: %v\n", err)
		os.Exit(1)
	}

	// Создаем UDP-соединение
	conn, err := net.DialUDP("udp", nil, udpAddr)
	if err != nil {
		fmt.Printf("Ошибка при создании соединения: %v\n", err)
		os.Exit(1)
	}
	defer conn.Close()

	fmt.Printf("Соединение с сервером %s установлено\n", serverAddr)

	// Отправляем сообщение серверу
	message := "Привет, сервер!"
	_, err = conn.Write([]byte(message))
	if err != nil {
		fmt.Printf("Ошибка при отправке сообщения: %v\n", err)
		return
	}
	fmt.Printf("Отправлено сообщение: %s\n", message)

	// Получаем ответ от сервера
	buffer := make([]byte, 1024)
	n, err := conn.Read(buffer)
	if err != nil {
		fmt.Printf("Ошибка при получении ответа: %v\n", err)
		return
	}

	fmt.Printf("Получен ответ от сервера: %s\n", string(buffer[:n]))
}
