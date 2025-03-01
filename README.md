fmt.Printf("PID: %d, Comm=%s ,SrcIP: %s, SrcPort: %d, DstIP: %s, DstPort: %d \n",
				event.Pid,
				//int8ToString(event.Comm),
				strings.TrimRight(string(event.Comm[:]), "\x00"),
				srcIP.String(),
				event.Sport,
				dstIP.String(),
				event.Dport,
			)

   [{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
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
	"message": "cannot convert event.Comm[:] (value of type []int8) to type string",
	"source": "compiler",
	"startLineNumber": 149,
	"startColumn": 30,
	"endLineNumber": 149,
	"endColumn": 43
}]
