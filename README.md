lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


				if event.State == 1 { // SYN_SENT
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
					fmt.Printf("PID=%d srcAddr=%s -> dstAddr=%s  STATE=%d\n", event.Pid, srcAddr, dstAddr, event.State)

					// Отправляем порт в канал с задержкой 2 сек
					go func(port int) {
						time.Sleep(2 * time.Second)
						portChan <- port
					}(int(event.Sport))
				}

				if event.State == 2 { // ESTABLISHED
					// Ожидание порта из канала (не более 10 секунд)
					select {
					case port := <-portChan:
						event.Sport = uint16(port)
					case <-time.After(15 * time.Second):
						fmt.Printf("WARNING: Timeout waiting for port assignment (PID=%d)\n", event.Pid)
					}

					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
					fmt.Printf("PID=%d srcAddr=%s <- dstAddr=%s  STATE=%d\n", event.Pid, srcAddr, dstAddr, event.State)
				}







