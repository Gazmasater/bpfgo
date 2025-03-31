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



			if event.Sysexit == 6 {

				if event.State == 2 {
					// Формируем ключ для входящего соединения
					//key := fmt.Sprintf("%s:%s:%d:in", dstIP.String(), srcIP.String(), event.Dport)

					// Формируем адреса для вывода
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

					fmt.Printf("PID=%d %s <- %s  \n", event.Pid, srcAddr, dstAddr)

				}
			}

			if event.State == 1 {
				// Формируем ключ для исходящего соединения
				//key := fmt.Sprintf("%s:%s:%d:out", srcIP.String(), dstIP.String(), event.Dport)

				srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
				dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

				fmt.Printf("PID=%d %s: -> %s \n", event.Pid, srcAddr, dstAddr)

			}











