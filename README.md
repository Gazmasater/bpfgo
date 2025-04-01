lsns -t net


sudo ip netns list
ls /sys/kernel/tracing/events/udp/



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i wlp0s20f3 udp -nn

sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
Дескриптор нового namespace: 6
2025/04/01 15:50:20 opening tracepoint udp_rcv: reading file "/sys/kernel/tracing/events/udp/udp_rcv/id": open /sys/kernel/tracing/events/udp/udp_rcv/id: no such file or directory
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$


