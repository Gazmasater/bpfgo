lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


srcAddr=94.100.180.59:443 dstAddr=192.168.1.71:53858  SYSCALL=3  PROTO=6




