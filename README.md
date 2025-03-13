

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

PID=767 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 

r



