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


package main

import (
	"fmt"
	"sync"
)

// Канал с буфером 1 для хранения порта
var eventChan = make(chan int, 1)
var mu sync.Mutex

func handleEvent(event Event) {
	if event.State == 2 {
		mu.Lock()
		select {
		case eventChan <- event.Sport:
			fmt.Printf("State 2: сохранен порт %d\n", event.Sport)
		default:
			// Если канал уже содержит значение, заменяем его
			<-eventChan
			eventChan <- event.Sport
			fmt.Printf("State 2: заменен порт %d\n", event.Sport)
		}
		mu.Unlock()
	}

	if event.State == 1 {
		mu.Lock()
		select {
		case xxx := <-eventChan:
			fmt.Printf("State 1: получен порт %d\n", xxx)
			srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), xxx)
			dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
			fmt.Printf("PID=%d %s <- %s\n", event.Pid, srcAddr, dstAddr)
		default:
			fmt.Println("State 1: канал пуст, порт не получен")
		}
		mu.Unlock()
	}
}
