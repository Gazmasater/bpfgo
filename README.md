lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i wlp0s20f3 udp -nn

sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


5:14:33.646892 IP 192.168.1.71.50430 > 34.117.188.166.443: UDP, length 1252
15:14:33.646928 IP 192.168.1.71.50430 > 34.117.188.166.443: UDP, length 1252
15:14:33.671326 IP 34.117.188.166.443 > 192.168.1.71.50430: UDP, length 43
15:14:33.671659 IP 34.117.188.166.443 > 192.168.1.71.50430: UDP, length 1252
