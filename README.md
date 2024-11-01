bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

package main

import (
	"fmt"
	"net"
)

func main() {
	// Привязываем сервер к адресу и порту
	listener, err := net.Listen("tcp", ":9090")
	if err != nil {
		fmt.Println("Error starting server:", err)
		return
	}
	defer listener.Close()

	fmt.Println("Server listening on port 9090...")

	for {
		// Принимаем входящее соединение
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
nc 127.0.0.1 9090
