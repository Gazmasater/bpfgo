package main

import (
	"fmt"
	"net"
)

func main() {

	listener, err := net.Listen("tcp", ":9090")
	if err != nil {
		fmt.Println("Error starting server:", err)
		return
	}
	defer listener.Close()

	fmt.Println("Server listening on port 9090...")

	for {

		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("Error accepting connection:", err)
			continue
		}

		// Выводим информацию о принятом соединении
		fmt.Printf("Connection accepted from %s\n", conn.RemoteAddr().String())

		// Закрываем соединение
		conn.Close()
	}
}
