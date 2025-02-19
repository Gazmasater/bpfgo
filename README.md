bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

sudo bpftool map dump id 11




11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

[{
	"resource": "/home/gaz358/myprog/bpfgo/PerfEvents/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredName"
		}
	},
	"severity": 8,
	"message": "undefined: program",
	"source": "compiler",
	"startLineNumber": 34,
	"startColumn": 66,
	"endLineNumber": 34,
	"endColumn": 73
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/PerfEvents/main.go",
	"owner": "go-staticcheck",
	"severity": 4,
	"message": "undefined: gener.BpfObjects",
	"source": "go-staticcheck",
	"startLineNumber": 28,
	"startColumn": 16,
	"endLineNumber": 28,
	"endColumn": 28
}]



