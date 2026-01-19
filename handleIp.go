package main

import (
	"fmt"
	"net"
	"sync"
)

var (
	connections = make(map[string]bool)
	muConn      sync.Mutex
)

func HandleIPEvent(
	event bpfTraceInfo,
	srcIP, dstIP net.IP,
	eventChan_sport chan int,
	eventChan_pid chan int,
) {
	if event.Family != 6 {
		return
	}

	var (
		proto   = "TCP"
		dsthost = resolveHost(dstIP)
		srchost = resolveHost(srcIP)
	)

	srcAddr := formatAddr(srchost, srcIP, event.Sport)
	dstAddr := formatAddr(dsthost, dstIP, event.Dport)

	key := makeConnectionKey(srcIP, uint16(event.Sport), dstIP, event.Dport)

	switch event.State {
	case 1:
		muConn.Lock()
		if _, exists := connections[key]; exists {
			muConn.Unlock()
			return
		}
		connections[key] = true
		muConn.Unlock()

		select {
		case eventChan_sport <- int(event.Sport):
			fmt.Printf("State 1: заменен порт %d\n", event.Sport)
		default:
		}

		name := cachedComm(event.Comm)
		fmt.Printf("PID=%d NAME=%s %s:%s <- %s:%s \n",
			event.Pid,
			name,
			proto,
			srcAddr,
			proto,
			dstAddr)

	case 2, 10:
		select {
		case eventChan_pid <- int(event.Pid):
		default:
		}
	}
}

func makeConnectionKey(srcIP net.IP, srcPort uint16, dstIP net.IP, dstPort uint16) string {
	src := fmt.Sprintf("%s:%d", srcIP.String(), srcPort)
	dst := fmt.Sprintf("%s:%d", dstIP.String(), dstPort)
	if src < dst {
		return fmt.Sprintf("%s-%s", src, dst)
	}
	return fmt.Sprintf("%s-%s", dst, src)
}

func formatAddr(name string, ip net.IP, port uint16) string {
	return fmt.Sprintf("//%s[%s]:%d", name, ip.String(), port)
}
