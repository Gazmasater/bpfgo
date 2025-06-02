
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


func resolveHost(ip net.IP) string {

	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.To4() != nil {
		if ip.IsLoopback() {
			host = "localhost"
		} else {
			host = pkg.ResolveIP(ip)

		}
	} else {
		var err error
		host, err = pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()

	return host
}







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git checkout ProcNet_monitor
git push --force origin ProcNet_monitor



func resolveHost(ip net.IP) string {
    // 1) Если ip == nil, сразу возвращаем «unknown»
    if ip == nil {
        return "unknown"
    }

    // 2) Убираем возможную зону (%lo0, %eth0 и т.п.)
    if s := ip.String(); strings.Contains(s, "%") {
        // если, например, ip.String() == "127.0.0.1%lo0", обрезаем «%lo0»
        noZone := s[:strings.LastIndex(s, "%")]
        ip = net.ParseIP(noZone)
        if ip == nil {
            return "unknown"
        }
    }

    // 3) Нормализуем «IPv4-mapped IPv6» (например ::ffff:127.0.0.1) в чистое 4-байтовое presentation
    if v4 := ip.To4(); v4 != nil {
        ip = v4
    }

    // 4) Сразу проверяем loopback (127.0.0.1 или ::1)
    //    после To4() IsLoopback() вернёт true и для «::ffff:127.0.0.1»
    if ip.IsLoopback() {
        return "localhost"
    }

    // 5) Больше не loopback-адрес, используем строковое представление IP как ключ кеша
    key := ip.String()

    cacheMu.RLock()
    if host, ok := resolveCache[key]; ok {
        cacheMu.RUnlock()
        return host
    }
    cacheMu.RUnlock()

    // 6) Если в кеше нет, зовём pkg.ResolveIP(...) или pkg.ResolveIP_n(...)
    var host string
    if ip4 := ip.To4(); ip4 != nil {
        host = pkg.ResolveIP(ip4)
    } else {
        var err error
        host, err = pkg.ResolveIP_n(ip)
        if err != nil {
            host = "unknown"
        }
    }

    // 7) Записываем в кеш и возвращаем
    cacheMu.Lock()
    resolveCache[key] = host
    cacheMu.Unlock()

    return host
}












