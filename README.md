lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=1"
PID=9165 Comm=udp_client ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 127.0.0.1, DstPort: 33333  SYSCALL=1
PID=2926 Comm=DNS Res~ver #15 ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 127.0.0.53, DstPort: 53  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 34.120.195.249, DstPort: 443  SYSCALL=1
cccc^Cgaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ^C
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=4"
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 217.69.139.58, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.41.56, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.61.112, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.52.80, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.52.89, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.54.50, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 5.181.61.0, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 194.226.130.226, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 94.100.180.211, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.54.50, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.54.50, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 95.163.52.80, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 64.233.165.94, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 64.233.165.94, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 5.181.61.0, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 217.69.139.211, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 217.69.139.211, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 217.69.139.102, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 89.221.236.28, DstPort: 443  SYSCALL=4
PID=2926 Comm=Socket Thread ,SrcIP: 0.0.0.0, SrcPort: 0 -> DstIP: 151.236.76.33, DstPort: 443  SYSCALL=4



