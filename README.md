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

				var xxx, xxx_pid int
				var proto string

				if event.State == 1 {

					mu.Lock()
					select {
					case eventChan <- int(event.Sport):
					default:
						// Если канал уже содержит значение, заменяем его
						//	<-eventChan
						eventChan <- int(event.Sport)
						fmt.Printf("State 1: заменен порт %d\n", event.Sport)
					}
					mu.Unlock()
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

					if event.Proto == 6 {

						proto = "TCP"
					}

					fmt.Printf("PID=%d %s %s <- %s \n", event.Pid, proto, srcAddr, dstAddr)

				}
				if event.State == 2 {
					mu.Lock()
					select {
					case eventChan_pid <- int(event.Pid):
					default:
						fmt.Println("State 2: eventChan_pid заполнен, пропускаю запись PID")
					}
					mu.Unlock()
				}

				select {

				case xxx = <-eventChan:
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), xxx)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

					//xxx_pid = <-eventChan_pid
					if event.Proto == 6 {

						proto = "TCP"
					}

					fmt.Printf("PID=%d %s %s <- %s \n", xxx_pid, proto, srcAddr, dstAddr)

				default:
					fmt.Println("")
				}

			}



		select {
		case xxx_pid = <-eventChan_pid:
			fmt.Printf("State 2: получил PID %d\n", xxx_pid)
		default:
			fmt.Println("State 2: eventChan_pid пуст, PID неизвестен")
		}








