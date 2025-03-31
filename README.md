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



var portMap = make(map[string]uint16) // Хранилище портов по IP+порт
var mu sync.Mutex                     // Мьютекс для защиты portMap

if event.State == 2 {
    select {
    case port := <-portChan:
        event.Sport = uint16(port)
        mu.Lock()
        // Формируем ключ прямо в коде, учитывая направление потока
        key := fmt.Sprintf("%s:%s:%d:in", dstIP.String(), srcIP.String(), event.Dport)
        portMap[key] = event.Sport
        mu.Unlock()
    case <-time.After(15 * time.Second):
        fmt.Printf("WARNING: Timeout waiting for port assignment (PID=%d)\n", event.Pid)
    }

    srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
    dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

    if event.Proto == 6 {
        proto = "TCP"
    }

    fmt.Printf("PID=%d %s: %s <- %s \n", event.Pid, proto, srcAddr, dstAddr)
}

if event.State == 1 {
    mu.Lock()
    // Формируем ключ для исходящего соединения
    key := fmt.Sprintf("%s:%s:%d:out", srcIP.String(), dstIP.String(), event.Dport)
    if port, ok := portMap[key]; ok {
        event.Sport = port
        delete(portMap, key) // Удаляем порт после использования
    }
    mu.Unlock()

    srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
    dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

    if event.Proto == 6 {
        proto = "TCP"
    }

    fmt.Printf("PID=%d %s: %s -> %s \n", event.Pid, proto, srcAddr, dstAddr)

    // Отправляем порт в канал с задержкой 2 сек
    go func(port int) {
        time.Sleep(2 * time.Second)
        portChan <- port
    }(int(event.Sport))
}


