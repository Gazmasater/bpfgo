fmt.Printf("PID: %d, Comm=%s ,SrcIP: %s, SrcPort: %d, DstIP: %s, DstPort: %d \n",
				event.Pid,
				//int8ToString(event.Comm),
				strconv.Itoa(event.Comm),
				srcIP.String(),
				event.Sport,
				dstIP.String(),
				event.Dport,
			)

   [{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "IncompatibleAssign",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "IncompatibleAssign"
		}
	},
	"severity": 8,
	"message": "cannot use event.Comm (variable of type [64]int8) as int value in argument to strconv.Itoa",
	"source": "compiler",
	"startLineNumber": 149,
	"startColumn": 18,
	"endLineNumber": 149,
	"endColumn": 28
}]
