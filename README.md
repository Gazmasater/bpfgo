
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


1️⃣ Проверка, вызывается ли sk_lookup
Попробуй добавить простейший отладочный вывод в начало eBPF-функции:


bpf_printk("sk_lookup вызван!");
Заново загрузи программу и посмотри trace_pipe:


sudo cat /sys/kernel/debug/tracing/trace_pipe
Если даже это не печатается, значит sk_lookup не вызывается.

2️⃣ Проверка трафика в netns
Ты создал новый network namespace, но в нём может не быть трафика.

✅ Выведи список интерфейсов в новом netns:


ip netns exec testns ip link show
Если там только lo (loopback), значит eBPF просто не видит никакого трафика.

✅ Добавь виртуальный интерфейс и отправь трафик:


ip link add veth0 type veth peer name veth1
ip link set veth1 netns testns
ip netns exec testns ip addr add 192.168.1.2/24 dev veth1
ip addr add 192.168.1.1/24 dev veth0
ip link set veth0 up
ip netns exec testns ip link set veth1 up
Теперь в testns есть интерфейс.

✅ Сгенерируй трафик:


ip netns exec testns nc -u 192.168.1.1 12345
Если sk_lookup работает, он должен сработать на этот пакет.

3️⃣ Проверка фильтрации по протоколу
В коде есть:


if (proto == IPPROTO_TCP) {
    proto_str = "TCP";
} else if (proto == IPPROTO_UDP) {
    proto_str = "UDP";
} else {
    proto_str = "UNKNOWN";
}
Но если пакет, например, ICMP (ping), он не попадёт в этот код.
Добавь отладку:


bpf_printk("sk_lookup вызван: протокол=%d", proto);
Заново загрузи eBPF и проверь trace_pipe.

4️⃣ Проверка привязки к netns
Ты используешь:

skLookupLink, err := link.AttachNetNs(int(newNS.Fd()), objs.LookUp)
Но можно проверить, правильно ли программа привязана:


sudo bpftool net show






