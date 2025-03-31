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


var eventChan = make(chan int, 1)


package main

import (
	"fmt"
	"sync"
)

var mu sync.Mutex

// Канал для передачи значения Sport, буферизированный для хранения нескольких значений
var eventChan = make(chan int, 10)

func handleEvent(event Event) {
	var xxx int // Переменная для хранения значения из состояния 2

	if event.Sysexit == 6 {

		if event.State == 1 {
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
			// Передаем значение Sport в канал для использования в состоянии 1
			eventChan <- event.Sport
			fmt.Printf("State 2: отправлено значение Sport = %d\n", event.Sport)

			// Формируем адреса и выводим информацию
			srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
			dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

			fmt.Printf("PID=%d %s -> %s\n", event.Pid, srcAddr, dstAddr)
		}
	}
}





[{
	"resource": "/home/gaz358/myprog/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "IncompatibleAssign",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "IncompatibleAssign"
		}
	},
	"severity": 8,
	"message": "cannot use event.Sport (variable of type uint16) as int value in send",
	"source": "compiler",
	"startLineNumber": 186,
	"startColumn": 19,
	"endLineNumber": 186,
	"endColumn": 30
}]















