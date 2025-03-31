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



WARNING: Timeout waiting for port assignment (PID=5380)
PID=5380 TCP: 192.168.1.71:0 <- 77.88.55.242:80 
WARNING: Timeout waiting for port assignment (PID=5446)
PID=5446 TCP: 192.168.1.71:0 <- 5.255.255.242:80 
WARNING: Timeout waiting for port assignment (PID=2871)
PID=2871 TCP: 192.168.1.71:0 <- 140.82.121.4:443 







