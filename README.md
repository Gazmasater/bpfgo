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


				if event.State == 1 {

					eventChan <- int(event.Sport)

					mu.Lock()

					// Извлекаем значение из канала, если оно есть
					select {

					case xxx = <-eventChan: // Сохраняем значение в переменной xxx
						fmt.Printf("State 1: получено значение Sport = %d\n", xxx)
					default:
						// Канал пуст, ничего не делаем
					}
					mu.Unlock()

					// Формируем адреса и выводим информацию
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

					fmt.Printf("PID=%d %s <- %s, xxx=%d\n", event.Pid, srcAddr, dstAddr, xxx)
				}

				if event.State == 2 {
					select {
					case xxx = <-eventChan: // Извлекаем значение Sport из канала и сохраняем его в xxx
						fmt.Printf("State 1: получено значение Sport = %d\n", xxx)

					default:
						// Канал пуст, ничего не делаем
					}
					srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), xxx)
					dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

					fmt.Printf("PID=%d %s -> %s\n", event.Pid, srcAddr, dstAddr)

				}















