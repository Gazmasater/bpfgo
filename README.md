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


State == 1 {

					mu.Lock()
					select {
					case eventChan <- int(event.Sport):
						fmt.Printf("State 1: сохранен порт %d\n", event.Sport)
					default:
						// Если канал уже содержит значение, заменяем его
						<-eventChan
						eventChan <- int(event.Sport)
						fmt.Printf("State 1: заменен порт %d\n", event.Sport)
					}
					mu.Unlock()
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
					fmt.Printf("PID=%d %s <- %s STATE=%d\n", event.Pid, srcAddr, dstAddr, event.State)

				}
				if event.State == 2 {

					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
					fmt.Printf("PID=%d %s -> %s STATE=%d\n", event.Pid, srcAddr, dstAddr, event.State)
					eventChan <- int(event.Sport)
					mu.Lock()
					var xxx int
					select {
					case xxx = <-eventChan:
						fmt.Printf("State 2: сохранен порт %d\n", xxx)
					default:
						// Если канал уже содержит значение, заменяем его
						<-eventChan
						eventChan <- int(event.Sport)
						fmt.Printf("State 2: заменен порт %d\n", event.Sport)
					}
					mu.Unlock()
				}

