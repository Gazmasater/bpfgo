gcc udp_server.c -o udp_server

package main

import (
	"fmt"
	"strings"
)

func int8ToString(arr [64]int8) string {
	// Преобразуем массив в срез байтов и удаляем нулевые байты
	return strings.TrimRight(string(arr[:]), "\x00")
}

func main() {
	arr := [64]int8{72, 101, 108, 108, 111, 0, 0, 0}
	fmt.Println(int8ToString(arr)) // Выведет "Hello"
}
