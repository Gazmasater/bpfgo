gcc udp_server.c -o udp_server

event.Comm= [117 100 112 95 115 101 114 118 101 114 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]


package main

import (
	"fmt"
	"strings"
)

func main() {
	// Пример массива, аналогичного вашему
	eventComm := [64]int8{117, 100, 112, 95, 115, 101, 114, 118, 101, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

	// Преобразуем массив в срез байтов и используем strings.TrimRight для удаления нулевых байтов
	str := strings.TrimRight(string(eventComm[:]), "\x00")

	fmt.Println(str) // Выведет "udp_server"
}

[{
	"resource": "/home/gaz358/myprog/test/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "InvalidConversion",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "InvalidConversion"
		}
	},
	"severity": 8,
	"message": "cannot convert eventComm[:] (value of type []int8) to type string",
	"source": "compiler",
	"startLineNumber": 13,
	"startColumn": 34,
	"endLineNumber": 13,
	"endColumn": 46
}]

