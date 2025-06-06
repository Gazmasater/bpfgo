package main

import (
	"bpfgo/pkg"
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
	var (
		proto   string
		dsthost string
		err     error
	)

	if event.Family == 6 {
		proto = "TCP"
	}

	if dstIP.IsLoopback() {
		dsthost = pkg.ResolveIP(dstIP)
	} else {
		dsthost, err = pkg.ResolveIP_n(dstIP)
		if err != nil {
			dsthost = "unknown"
		}
	}
	srchost := resolveHost(srcIP)

	srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
	dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

	key := makeConnectionKey(srcIP, uint16(event.Sport), dstIP, event.Dport)

	switch event.State {
	case 1:
		muConn.Lock()
		_, exists := connections[key]
		if !exists {
			connections[key] = true
		}
		muConn.Unlock()

		if !exists {
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
		}

	case 2, 10:
		select {
		case eventChan_pid <- int(event.Pid):
		default:
		}
	}

	select {
	case sport := <-eventChan_sport:
		select {
		case pid := <-eventChan_pid:
			if pid > 0 {
				srcAddr = fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), sport)
				dstAddr = fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

				name := cachedComm(event.Comm)

				fmt.Printf("PID=%d NAME=%s %s:%s -> %s:%s \n",
					pid,
					name,
					proto,
					srcAddr,
					proto,
					dstAddr)
			}
		default:
		}
	default:
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
