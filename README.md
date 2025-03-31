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


var lastPid *int // Указатель на PID

if event.Sysexit == 6 {

	var xxx int

	if event.State == 1 {
		mu.Lock()
		select {
		case eventChan <- int(event.Sport):
		default:
			eventChan <- int(event.Sport)
			fmt.Printf("State 1: заменен порт %d\n", event.Sport)
		}
		mu.Unlock()
		srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
		dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
		fmt.Printf("PID=%d PROTO=%d %s <- %s \n", event.Pid, event.Proto, srcAddr, dstAddr)
	}

	if event.State == 2 {
		pid := int(event.Pid) // Локальная переменная
		lastPid = &pid        // Запоминаем PID через указатель
		fmt.Println("Запомнил PID:", *lastPid)
	}

	select {
	case xxx = <-eventChan:
		srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), xxx)
		dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

		// Если lastPid не nil, используем его значение
		if lastPid != nil {
			fmt.Printf("PID=%d PROTO=%d %s -> %s \n", *lastPid, event.Proto, srcAddr, dstAddr)
		} else {
			fmt.Println("PID не установлен, пропускаю вывод")
		}

	default:
		fmt.Println("State 2: канал пуст, пропускаю чтение")
	}

}






