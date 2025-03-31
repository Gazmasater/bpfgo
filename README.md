lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


			if event.Sysexit == 6 {

				if event.State == 1 {
					key := fmt.Sprintf("%s->%s:%d", srcIP.String(), dstIP.String(), event.Dport)

					portMap[key] = int(event.Sport)

					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
					fmt.Printf("PID=%d srcAddr=%s <- dstAddr=%s  STATE=%d\n", event.Pid, srcAddr, dstAddr, event.State)

				}

				if event.State == 2 {

					key := fmt.Sprintf("%s->%s:%d", srcIP.String(), dstIP.String(), event.Dport)
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
					fmt.Printf("!!!!!!!PID=%d srcAddr=%s -> dstAddr=%s  STATE=%d\n", event.Pid, srcAddr, dstAddr, event.State)

					if port, found := portMap[key]; found {

						event.Sport = uint16(port)

					}

				}

			}






