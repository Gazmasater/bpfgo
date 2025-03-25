lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


^Cgaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=1"
PID=8511 Comm=udp_client ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.1(localhost), DstPort: 33333 PROTO: 0 SYSCALL=1
2025/03/25 20:23:58 invalid event size
2025/03/25 20:23:58 invalid event size

 


