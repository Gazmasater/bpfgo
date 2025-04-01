lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i eth0 udp -nn

sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


srcAddr := fmt.Sprintf("%s:%d (%s)", srcIP.String(), event.Sport, ResolveIP(srcIP))
dstAddr := fmt.Sprintf("%s:%d (%s)", dstIP.String(), event.Dport, ResolveIP(dstIP))


tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on wlp0s20f3, link-type EN10MB (Ethernet), snapshot length 262144 bytes
13:45:43.664185 IP 192.168.1.71.51550 > 192.168.1.1.53: 22424+ [1au] A? mozilla.cloudflare-dns.com. (55)
13:45:43.664274 IP 192.168.1.71.56569 > 192.168.1.1.53: 23042+ [1au] AAAA? mozilla.cloudflare-dns.com. (55)
13:45:43.676382 IP 192.168.1.1.53 > 192.168.1.71.51550: 22424 2/0/1 A 162.159.61.4, A 172.64.41.4 (87)
13:45:43.676982 IP 192.168.1.1.53 > 192.168.1.71.56569: 23042 2/0/1 AAAA 2803:f800:53::4, AAAA 2a06:98c1:52::4 (111)
13:45:43.890284 IP 192.168.1.71.57377 > 192.168.1.1.53: 57180+ [1au] A? detectportal.firefox.com. (53)
13:45:43.890350 IP 192.168.1.71.37299 > 192.168.1.1.53: 50891+ [1au] AAAA? detectportal.firefox.com. (53)

