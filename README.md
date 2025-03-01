gcc udp_server.c -o udp_server

event.Comm= [117 100 112 95 115 101 114 118 101 114 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]


import "bytes"

func int8ToString(arr [64]int8) string {
	// Преобразуем массив int8 в срез байтов, обрезаем нулевые байты и возвращаем строку
	return string(bytes.TrimRight([]byte(arr[:]), "\x00"))
}
