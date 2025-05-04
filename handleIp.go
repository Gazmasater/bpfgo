package main

import (
	"bpfgo/pkg"
	"fmt"
	"net"
	"sync"
)

// Глобальная мапа соединений и мьютекс
var (
	connections = make(map[string]bool)
	muConn      sync.Mutex
)

func HandleIPEvent(
	event bpfTraceInfo,
	srcIP, dstIP net.IP,
	mu *sync.Mutex,
	eventChan_sport chan int,
	eventChan_pid chan int,
) {
	var (
		proto   string
		dsthost string
		err     error
	)

	//fmt.Printf("FAMIY FUNC =%d STATE=%d\n", event.Family, event.State)
	// fmt.Printf("PID=%d SPORT=%d DPORT=%d STATE=%d NAME=%s\n",
	// 	event.Pid,
	// 	event.Sport,
	// 	event.Dport,
	// 	event.State,
	// 	pkg.Int8ToString(event.Comm))

	// Определяем протокол
	if event.Family == 6 {
		proto = "TCP"
	}

	// Определяем имена хостов
	if dstIP.IsLoopback() {
		dsthost = pkg.ResolveIP(dstIP)
	} else {
		dsthost, err = pkg.ResolveIP_n(dstIP)
		if err != nil {
			dsthost = "unknown"
		}
	}
	srchost := pkg.ResolveIP(srcIP)

	// Формируем адреса
	srcAddr := fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), event.Sport)
	dstAddr := fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

	// Формируем ключ соединения
	key := makeConnectionKey(srcIP, uint16(event.Sport), dstIP, event.Dport)

	// Обработка состояния 1 (новое соединение)
	if event.State == 1 {
		muConn.Lock()
		if _, exists := connections[key]; !exists {
			connections[key] = true
			muConn.Unlock()

			mu.Lock()
			select {
			case eventChan_sport <- int(event.Sport):
			default:
				eventChan_sport <- int(event.Sport)
				fmt.Printf("State 1: заменен порт %d\n", event.Sport)
			}
			mu.Unlock()

			fmt.Println("")
			fmt.Printf("PID=%d NAME=%s %s:%s <- %s:%s \n",
				event.Pid,
				pkg.Int8ToString(event.Comm),
				proto,
				srcAddr,
				proto,
				dstAddr)
		} else {
			muConn.Unlock()
		}
	}

	// Обработка состояний 2 и 10
	if event.State == 2 || event.State == 10 {
		//fmt.Printf("POSLE IF STATE=%d PID=%d\n", event.State, event.Pid)
		mu.Lock()
		select {
		case eventChan_pid <- int(event.Pid):
		default:
		}
		mu.Unlock()
	}

	// Чтение данных из каналов (если есть)
	select {
	case sport := <-eventChan_sport:
		select {
		case pid := <-eventChan_pid:
			if pid > 0 {
				srcAddr = fmt.Sprintf("//%s[%s]:%d", srchost, srcIP.String(), sport)
				dstAddr = fmt.Sprintf("//%s[%s]:%d", dsthost, dstIP.String(), event.Dport)

				fmt.Printf("PID=%d NAME=%s %s:%s -> %s:%s \n",
					pid,
					pkg.Int8ToString(event.Comm),
					proto,
					srcAddr,
					proto,
					dstAddr)
				fmt.Println("")
			}
		default:
			return
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
