lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


sudo apt update && sudo apt install -y tcpdump
sudo tcpdump -i any -nn 'tcp[tcpflags] & (tcp-syn) != 0'

ls /sys/kernel/debug/tracing/events/sock/udp_sendmsg


package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

// Event — структура события
type Event struct {
	Pid     int
	SrcIP   string
	Sport   int
	DstIP   string
	DstPort int
	Sysexit int
	State   int
}

func parseEvent(line string) (Event, bool) {
	var event Event
	_, err := fmt.Sscanf(line, "PID=%d srcAddr=%s -> dstAddr=%s SYSCALL=%d STATE=%d",
		&event.Pid, &event.SrcIP, &event.DstIP, &event.Sysexit, &event.State)
	if err == nil {
		parts := strings.Split(event.SrcIP, ":")
		if len(parts) == 2 {
			event.SrcIP = parts[0]
			fmt.Sscanf(parts[1], "%d", &event.Sport)
		}
		parts = strings.Split(event.DstIP, ":")
		if len(parts) == 2 {
			event.DstIP = parts[0]
			fmt.Sscanf(parts[1], "%d", &event.DstPort)
		}
		return event, true
	}
	return event, false
}

func main() {
	portMap := make(map[string]int) // Карта для хранения портов STATE=1

	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		line := scanner.Text()
		event, ok := parseEvent(line)
		if !ok {
			continue
		}

		// Запоминаем порт, если STATE=1
		if event.State == 1 {
			key := fmt.Sprintf("%s->%s:%d", event.SrcIP, event.DstIP, event.DstPort)
			portMap[key] = event.Sport
		}

		// Если SPORT=0 и есть соответствие в portMap — заменяем
		if event.Sport == 0 {
			key := fmt.Sprintf("%s->%s:%d", event.SrcIP, event.DstIP, event.DstPort)
			if port, found := portMap[key]; found {
				event.Sport = port
			}
		}

		// Вывод обновлённых данных
		srcAddr := fmt.Sprintf("%s:%d", event.SrcIP, event.Sport)
		dstAddr := fmt.Sprintf("%s:%d", event.DstIP, event.DstPort)

		if event.Sport == 0 {
			fmt.Printf("PID=%d srcAddr=%s -> dstAddr=%s  SYSCALL=%d STATE=%d\n", event.Pid, srcAddr, dstAddr, event.Sysexit, event.State)
		} else {
			fmt.Printf("PID=%d srcAddr=%s <- dstAddr=%s  SYSCALL=%d STATE=%d\n", event.Pid, srcAddr, dstAddr, event.Sysexit, event.State)
		}
	}
}



