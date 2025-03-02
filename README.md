gcc udp_server.c -o udp_server

event.Comm= [117 100 112 95 115 101 114 118 101 114 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]


import (
	"bytes"
)

func int8ArrayToString(arr [64]int8) string {
	byteArr := make([]byte, len(arr))
	for i, v := range arr {
		byteArr[i] = byte(v) // Преобразуем int8 в byte
	}
	return string(bytes.Trim(byteArr, "\x00")) // Убираем \x00 в конце
}
