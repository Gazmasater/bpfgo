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


		if event.State == 1 {
			mu.Lock()
			xxx := event.Sport // Присваиваем xxx значение event.Sport
			xxxChan <- xxx     // Передаём xxx в канал
			mu.Unlock()

			srcAddr := fmt.Sprintf("%s:%d", event.SrcIP.String(), event.Sport)
			dstAddr := fmt.Sprintf("%s:%d", event.DstIP.String(), event.Dport)

			fmt.Printf("PID=%d %s <- %s\n", event.Pid, srcAddr, dstAddr)
		}

		if event.State == 2 {
			mu.Lock()
			xxx := <-xxxChan // Получаем xxx из канала
			mu.Unlock()

			srcAddr := fmt.Sprintf("%s:%d", event.SrcIP.String(), xxx)
			dstAddr := fmt.Sprintf("%s:%d", event.DstIP.String(), event.Dport)

			fmt.Printf("PID=%d %s -> %s\n", event.Pid, srcAddr, dstAddr)
		}















