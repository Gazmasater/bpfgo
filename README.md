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


if event.State == 2 {
    eventChan <- event.Sport // Сохраняем значение Sport в канал
    fmt.Printf("State 2: отправлено значение Sport = %d\n", event.Sport)
}

if event.State == 1 {
    select {
    case xxx = <-eventChan: // Извлекаем значение Sport из канала и сохраняем его в xxx
        fmt.Printf("State 1: получено значение Sport = %d\n", xxx)
    default:
        // Канал пуст, ничего не делаем
    }
}



















