
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________
GIT

git reset --hard HEAD
____________________________________________________



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30


"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux







// Обработка Sysexit == 2 (sys_exit_recvfrom)
if event.Sysexit == 2 {
    if event.Family == 2 {
        port := int(event.Sport)
        data, exists := eventMap[port]
        if !exists {
            // Если объекта ещё нет, создаём новый
            data = &EventData{}
            eventMap[port] = data
        }
        data.Recvmsg = Recvmsg{
            SrcIP:   srcIP,
            SrcPort: port,
            Pid:     event.Pid,
            Proto:   int(event.Proto),
            Comm:    pkg.Int8ToString(event.Comm),
        }
        data.HasRecvmsg = true

        // Если lookup уже был, и sendmsg уже был, печатаем интервью — но чаще ждём sendto/sendmsg
        if data.HasLookup && data.HasSendmsg {
            if data.Lookup.Proto == 17 {
                proto = "UDP"
            }
            if data.Lookup.DstIP.IsLoopback() {
                dsthost = "localhost"
            } else {
                dsthost = resolveIPWithCache(data.Lookup.DstIP)
            }
            if data.Lookup.SrcIP.IsLoopback() {
                srchost = "localhost"
            } else {
                srchost = resolveIPWithCache(data.Lookup.SrcIP)
            }
            fmt.Printf(
                "RECVFROM PID=%d NAME=%s %s/%s[%s]:%d -> %s[%s]:%d\n",
                data.Sendmsg.Pid,
                data.Sendmsg.Comm,
                proto,
                dsthost,
                data.Lookup.DstIP,
                data.Lookup.DstPort,
                srchost,
                data.Lookup.SrcIP,
                data.Lookup.SrcPort,
            )
            fmt.Printf(
                "RECVFROM PID=%d NAME=%s %s/%s[%s]:%d <- %s[%s]:%d\n",
                data.Recvmsg.Pid,
                data.Recvmsg.Comm,
                proto,
                dsthost,
                data.Lookup.DstIP,
                data.Lookup.DstPort,
                srchost,
                data.Lookup.SrcIP,
                data.Lookup.SrcPort,
            )
            delete(eventMap, port)
        }
    } else if event.Family == 10 {
        port := event.Sport
        pid := event.Pid
        ip6 := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
        fmt.Printf(
            "RECVFROM IPv6 PID=%d IPv6=%s:%d NAME=%s\n",
            pid,
            ip6.String(),
            port,
            pkg.Int8ToString(event.Comm),
        )
    }
}

// Обработка Sysexit == 3 (sk_lookup)
if event.Sysexit == 3 {
    family := event.Family
    if family == 2 {
        dstPort := int(event.Dport)
        srcPort := int(event.Sport)

        // Создаём один общий объект для обеих карт
        data := &EventData{}
        data.Lookup = Lookup{
            SrcIP:   srcIP,
            SrcPort: srcPort,
            DstIP:   dstIP,
            DstPort: dstPort,
            Proto:   int(event.Proto),
        }
        data.HasLookup = true

        // Сохраняем одну и ту же ссылку в двух картах
        eventMap[dstPort] = data
        eventMap_1[srcPort] = data

        // Если recvfrom уже прошёл и HasRecvmsg==true, и одновременно sendmsg==true,
        // печатаем сразу
        if data.HasRecvmsg && data.HasSendmsg {
            if data.Lookup.Proto == 17 {
                proto = "UDP"
            }
            if data.Lookup.DstIP.IsLoopback() {
                dsthost = "localhost"
            } else {
                dsthost = resolveIPWithCache(data.Lookup.DstIP)
            }
            if data.Lookup.SrcIP.IsLoopback() {
                srchost = "localhost"
            } else {
                srchost = resolveIPWithCache(data.Lookup.SrcIP)
            }
            fmt.Println("")
            fmt.Printf(
                "LOOKUP PID=%d NAME=%s %s/%s[%s]:%d<-%s[%s]:%d\n",
                data.Sendmsg.Pid,
                data.Sendmsg.Comm,
                proto,
                dsthost,
                data.Lookup.DstIP,
                data.Lookup.DstPort,
                srchost,
                data.Lookup.SrcIP,
                data.Lookup.SrcPort,
            )
            fmt.Printf(
                "LOOKUP PID=%d NAME=%s %s/%s[%s]:%d->%s[%s]:%d\n",
                data.Recvmsg.Pid,
                data.Recvmsg.Comm,
                proto,
                dsthost,
                data.Lookup.DstIP,
                data.Lookup.DstPort,
                srchost,
                data.Lookup.SrcIP,
                data.Lookup.SrcPort,
            )
            fmt.Println("")
            delete(eventMap, dstPort)
        }
    } else if family == 10 {
        ip6_s := pkg.IPv6FromLEWords(IPv6BytesToWords(event.SrcIP6.In6U.U6Addr8))
        fmt.Printf("LOOKUP SRC IPv6=%s\n", ip6_s)
        ip6_d := pkg.IPv6FromLEWords(IPv6BytesToWords(event.DstIP6.In6U.U6Addr8))
        fmt.Printf("LOOKUP DST IPv6=%s\n", ip6_d)
        fmt.Printf("LOOKUP SPORT=%d  DPORT=%d PROTO=%d\n", event.Sport, event.Dport, event.Proto)
    }
}










