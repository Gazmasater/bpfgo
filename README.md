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


PID=7493 192.168.1.71:0 -> 77.88.55.242:80
PID=535 192.168.1.71:35908 <- 77.88.55.242:80
State 1: получено значение Sport = 35908
PID=988 192.168.1.71:35908 -> 185.125.190.49:80
PID=537 192.168.1.71:55356 <- 185.125.190.49:80
State 1: получено значение Sport = 55356
PID=7600 192.168.1.71:55356 -> 173.194.73.141:443
PID=536 192.168.1.71:41522 <- 173.194.73.141:443
State 1: получено значение Sport = 41522
PID=4107 192.168.1.71:41522 -> 13.107.246.45:443
PID=534 192.168.1.71:45552 <- 13.107.246.45:443



















