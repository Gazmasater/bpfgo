lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=2"
PID=6968 Comm=udp_server ,SrcIP: 127.0.0.1(localhost), SrcPort: 34075 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=DNS Res~ver #13 ,SrcIP: 127.0.0.53(_localdnsstub), SrcPort: 53 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=DNS Res~ver #13 ,SrcIP: 127.0.0.53(_localdnsstub), SrcPort: 53 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
2025/03/25 20:47:36 invalid event size
2025/03/25 20:47:36 invalid event size
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.73.94(lq-in-f94.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
PID=3993 Comm=Socket Thread ,SrcIP: 173.194.222.97(lo-in-f97.1e100.net.), SrcPort: 443 -> DstIP: 0.0.0.0(Unknown), DstPort: 0 PROTO: 0 SYSCALL=2
2025/03/25 20:47:36 invalid event size
2025/03/25 20:47:36 invalid event size
2025/03/25 20:47:36 invalid event size
2025/03/25 20:47:37 invalid event size
2025/03/25 20:47:37 invalid event size
2025/03/25 20:47:37 invalid event size

 


