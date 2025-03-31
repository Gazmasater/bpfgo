lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "InvalidSend",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "InvalidSend"
		}
	},
	"severity": 8,
	"message": "invalid operation: cannot send to non-channel xxx (variable of type int)",
	"source": "compiler",
	"startLineNumber": 205,
	"startColumn": 11,
	"endLineNumber": 205,
	"endColumn": 27
}]
