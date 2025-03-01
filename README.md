gcc udp_server.c -o udp_server

event.Comm= [117 100 112 95 115 101 114 118 101 114 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]


import "bytes"

func int8ToString(arr [64]int8) string {
	// Преобразуем массив int8 в срез байтов, обрезаем нулевые байты и возвращаем строку
	return string(bytes.TrimRight([]byte(arr[:]), "\x00"))
}

[{
	"resource": "/home/gaz358/myprog/test/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
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
	"message": "cannot convert arr[:] (value of type []int8) to type []byte",
	"source": "compiler",
	"startLineNumber": 13,
	"startColumn": 39,
	"endLineNumber": 13,
	"endColumn": 45
}]
