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



 curl-3950    [005] ...21    90.326815: bpf_trace_printk: inet_sock_set_state PID=3950 srcip=192.168.1.71:0   dstip=77.88.44.242:80 PROTO=6 
           <...>-534     [002] ..s41    90.340175: bpf_trace_printk: inet_sock_set_state PID=534 srcip=192.168.1.71:35350   dstip=77.88.44.242:80 PROTO=6 









