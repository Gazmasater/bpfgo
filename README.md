lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


// Создаем мапу с ключом "ip:port" и значением "ip:port"
portsMap := make(map[string]string)

// Преобразуем IP-адреса в строковый формат
srcIP := net.IPv4(
    byte(event.SrcIp>>24),
    byte(event.SrcIp>>16),
    byte(event.SrcIp>>8),
    byte(event.SrcIp),
)

dstIP := net.IPv4(
    byte(event.DstIp>>24),
    byte(event.DstIp>>16),
    byte(event.DstIp>>8),
    byte(event.DstIp),
)

// Формируем строковые представления адресов с портами
srcAddr := fmt.Sprintf("%s:%d", srcIP.String(), event.Sport)
dstAddr := fmt.Sprintf("%s:%d", dstIP.String(), event.Dport)

// Проверяем, если Sysexit == 4, добавляем в мапу
if event.Sysexit == 4 {
    portsMap[dstAddr] = srcAddr
}

// Выводим данные в нужном формате
fmt.Printf("%s -> %s (PROTO: %d)\n", srcAddr, dstAddr, event.Proto)

// Выводим всю мапу (если нужно)
fmt.Println("Текущая мапа соединений:", portsMap)





