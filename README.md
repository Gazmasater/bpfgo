



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


______________________________________________________________________________________________
TG

✅ Чеклист — пройди всё:
1. Inline Mode включён в BotFather?
Перейди в @BotFather

Введи: /mybots → выбери своего бота

Bot Settings → Inline Mode → должно быть Enabled

📌 Если не включён — @Dom_Mechty48_Bot в чатах не будет работать вообще.

2. У бота установлено публичное имя?
Если бот приватный (не имеет @username) — инлайн работать не будет.

У тебя он называется @Dom_Mechty48_Bot, значит — ✅ OK.



Inline mode is currently enabled for dom_mechty @Dom_Mechty48_Bot.

Current placeholder is: Search...
Please send me the placeholder message for queries to your bot. People will see this placeholder whenever they type @Dom_Mechty48_Bot in the message field – in any chat. Tell them what they can get from your bot (e.g., Search GIFs...).


https://afystatic.ru/files/vpbb/full/0/07/07ef43605f8c8d24f63705001396103200.webp

https://avatars.mds.yandex.net/get-verba/3587101/2a0000017ff4c8092f5b4087c32235c40ef4/realty_large

https://57.img.avito.st/image/1/1.zLhjera5yFDXWXbT2nvOsVWPYlfT3eJHVdZiVd_balM.hhlWwMfPR-IIWYI6aTO7kCqIYB375RjQAowhu8vJyrs

https://www.lipetskmedia.ru/images/news_/135/005_big2.png

https://github.com/user-attachments/assets/4b68cdab-acff-4cf4-adb5-d363dc48f630








