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

	var xxx_pid int

	if event.State == 2 {
		mu.Lock()
		select {
		case eventChan_pid <- int(event.Pid):
			fmt.Printf("State 2: записал PID %d\n", event.Pid)
		default:
			fmt.Println("State 2: eventChan_pid заполнен, пропускаю запись PID")
		}
		mu.Unlock()
	}

	// Читаем PID из eventChan_pid
	select {
	case xxx_pid = <-eventChan_pid:
		fmt.Printf("PID=%d PROTO=%d \n", xxx_pid, event.Proto)
	default:
		fmt.Println("State 2: eventChan_pid пуст, PID неизвестен")
	}
}









