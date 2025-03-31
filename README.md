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



var portMap = make(map[string]uint16)
var mu sync.Mutex

if event.Sysexit == 6 {
    if event.State == 2 {
        mu.Lock()
        key := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
        if port, ok := portMap[key]; ok {
            event.Sport = port // Берем порт из состояния 1
            delete(portMap, key) // Удаляем после использования
        } else if event.Sport == 0 {
            mu.Unlock()
            return // Пропускаем, если порт не найден и он нулевой
        }
        mu.Unlock()

        srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
        dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

        fmt.Printf("PID=%d %s <- %s\n", event.Pid, srcAddr, dstAddr)
    }

    if event.State == 1 {
        mu.Lock()
        key := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)
        if _, exists := portMap[key]; !exists {
            portMap[key] = event.Sport // Сохраняем порт для состояния 2
        }
        mu.Unlock()

        srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
        dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

        fmt.Printf("PID=%d %s -> %s\n", event.Pid, srcAddr, dstAddr)
    }
}











