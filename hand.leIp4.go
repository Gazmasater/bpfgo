package main

import (
	"bpfgo/pkg"
	"fmt"
	"net"
	"sync"
)

func HandleIPv4Event(
	event bpfTraceInfo,
	srcIP, dstIP net.IP,
	mu *sync.Mutex,
	eventChan_sport chan int,
	eventChan_pid chan int,
) {
	var (
		proto   string
		xxx     int
		xxx_pid int
		dsthost string
		err     error
	)

	if event.State == 1 {
		mu.Lock()
		select {
		case eventChan_sport <- int(event.Sport):
		default:
			eventChan_sport <- int(event.Sport)
			fmt.Printf("State 1: заменен порт %d\n", event.Sport)
		}
		mu.Unlock()

		if dstIP.IsLoopback() {
			dsthost = pkg.ResolveIP(dstIP)
		} else {
			dsthost, err = pkg.ResolveIP_n(dstIP)
			if err != nil {
				dsthost = "unknown"
			}
		}

		srchost := pkg.ResolveIP(srcIP)

		srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
		dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

		if event.Proto == 6 {
			proto = "TCP"
		}

		fmt.Println("")
		fmt.Printf("PID=%d %s:%s <- %s:%s \n", event.Pid, proto, srcAddr, proto, dstAddr)
	}

	if event.State == 2 || event.State == 10 {
		mu.Lock()
		select {
		case eventChan_pid <- int(event.Pid):
		default:
			// пропускаем, если канал заполнен
		}
		mu.Unlock()
	}

	select {
	case xxx = <-eventChan_sport:
		if dstIP.IsLoopback() {
			dsthost = pkg.ResolveIP(dstIP)
		} else {
			dsthost, err = pkg.ResolveIP_n(dstIP)
			if err != nil {
				dsthost = "unknown"
			}
		}

		srchost := pkg.ResolveIP(srcIP)

		srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), xxx)
		dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

		select {
		case xxx_pid = <-eventChan_pid:
			// получили PID
		default:
			// PID неизвестен
		}

		if event.Proto == 6 {
			proto = "TCP"
		}

		fmt.Printf("PID=%d %s:%s -> %s:%s \n", xxx_pid, proto, srcAddr, proto, dstAddr)
		fmt.Println("")

	default:
		fmt.Println("")
	}
}
