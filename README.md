type bpfTraceInfo struct {
	SrcIP   struct{ S_addr uint32 }
	DstIP   struct{ S_addr uint32 }
	SrcIP6  struct{ In6U struct{ U6Addr8 [16]uint8 } }
	DstIP6  struct{ In6U struct{ U6Addr8 [16]uint8 } }
	Pid     uint32
	Proto   uint32
	Sport   uint16
	Dport   uint16
	Family  uint16
	Sysexit uint8
	State   uint8
	Comm    [32]int8
}

func IPv6FromLEWords(words [4]uint32) net.IP {
	ip := make(net.IP, 16)
	for i := 0; i < 4; i++ {
		ip[i*4+0] = byte(words[i])
		ip[i*4+1] = byte(words[i] >> 8)
		ip[i*4+2] = byte(words[i] >> 16)
		ip[i*4+3] = byte(words[i] >> 24)
	}
	return ip
}

else if family == 10 {
					port := event.Dport

					pid := event.Pid
					fmt.Printf("SENDTO IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
						pid,
						pkg.IPv6FromLEWords(event.DstIP6).String(),
						port,
						pkg.Int8ToString(event.Comm),
					)

				}

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
	"message": "cannot use event.DstIP6 (variable of type struct{In6U struct{U6Addr8 [16]uint8}}) as [4]uint32 value in argument to pkg.IPv6FromLEWords",
	"source": "compiler",
	"startLineNumber": 281,
	"startColumn": 27,
	"endLineNumber": 281,
	"endColumn": 39
}]

