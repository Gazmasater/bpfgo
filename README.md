https://github.com/Morwran/nft-go/tree/main/internal/expr-encoders



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

sudo apt install mitmproxy

mitmproxy
http://mitm.it




✅ Цель:
Зайти на https://ozon.ru в браузере.

Перехватить и увидеть все отправленные запросы и то, что отправилось сайту: заголовки, cookies, IP и прочее.

🧰 Что нужно:
mitmproxy (или mitmweb)

Сертификат установлен в систему или браузер

Браузер настроен на использование прокси

🔧 Шаг 1: Запусти mitmproxy (или mitmweb)
В терминале:

mitmweb
Это откроет веб-интерфейс: http://127.0.0.1:8081

Прокси работает на 127.0.0.1:8080

🔧 Шаг 2: Настрой браузер на использование прокси
В Firefox:
Настройки → Сеть → Настроить соединение

Прокси-сервер:

HTTP Proxy: 127.0.0.1

Port: 8080

Также использовать для HTTPS

В Chrome (через системные настройки):
Linux/macOS:

google-chrome --proxy-server="http=127.0.0.1:8080;https=127.0.0.1:8080"
🔧 Шаг 3: Установи сертификат mitmproxy, если не установил
Перейди в браузере на:

http://mitm.it
Выбери Linux/macOS → скачай и установи сертификат (инструкции выше).

🔎 Шаг 4: Зайди вручную на https://www.ozon.ru
Открой в браузере Ozon, полазай по сайту (главная, авторизация, каталог и т.д.)

Все запросы будут автоматически видны в mitmproxy / mitmweb.

👁 Что смотреть в mitmweb
Перейди на http://127.0.0.1:8081:

Нажми на нужный запрос → справа:

Request → заголовки (User-Agent, Cookie, и т.д.)

Response → тело ответа

Также можно экспортировать запрос или сохранить лог.

🧠 Что это даёт
Теперь ты точно видишь, что получает сайт:

Твои cookies и ID сессии

Что отправляется при входе, поиске, фильтрации и т.д.

Можно сохранить User-Agent, Cookie и использовать их в автоматических скриптах позже
________________________________________________________________________________




func (sui *natCmpRulesetStyleTestSuite) Test_TCPDnatAndRedirect() {
	testCases := []struct {
		name     string
		setup    func(ctx *ctx) []irNode
		expected string
	}{
		{
			name: "tcp dport 8080 dnat to 192.168.0.1:8080",
			setup: func(ctx *ctx) []irNode {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "8080"})
				ctx.reg.Set(3, regVal{HumanExpr: "192.168.0.1"})
				ctx.reg.Set(4, regVal{HumanExpr: "8080"})

				protoCmp := &expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}}
				dportCmp := &expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x1f, 0x90}}
				nat := &expr.NAT{
					Type:        expr.NATTypeDestNAT,
					Family:      unix.NFPROTO_IPV4,
					RegAddrMin:  3,
					RegProtoMin: 4,
				}

				protoIR, _ := (&cmpEncoder{cmp: protoCmp}).EncodeIR(ctx)
				dportIR, _ := (&cmpEncoder{cmp: dportCmp}).EncodeIR(ctx)
				natIR, _ := (&natEncoder{nat: nat}).EncodeIR(ctx)

				return []irNode{protoIR, dportIR, natIR}
			},
			expected: "tcp dport 8080 dnat ip to 192.168.0.1:8080",
		},
		{
			name: "tcp dport 443 redirect to :443",
			setup: func(ctx *ctx) []irNode {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "443"})
				ctx.reg.Set(3, regVal{HumanExpr: "443"})

				protoCmp := &expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}}
				dportCmp := &expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x01, 0xbb}}
				nat := &expr.NAT{
					Type:        NATTypeRedir,
					Family:      unix.NFPROTO_IPV4,
					RegProtoMin: 3,
				}

				protoIR, _ := (&cmpEncoder{cmp: protoCmp}).EncodeIR(ctx)
				dportIR, _ := (&cmpEncoder{cmp: dportCmp}).EncodeIR(ctx)
				natIR, _ := (&natEncoder{nat: nat}).EncodeIR(ctx)

				return []irNode{protoIR, dportIR, natIR}
			},
			expected: "tcp dport 443 redirect ip to :443",
		},
		{
			name: "masquerade to :1000-2000 random persistent",
			setup: func(ctx *ctx) []irNode {
				ctx.reg.Set(1, regVal{HumanExpr: "1000"})
				ctx.reg.Set(2, regVal{HumanExpr: "2000"})

				nat := &expr.NAT{
					Type:        NATTypeMASQ,
					Family:      unix.NFPROTO_IPV4,
					RegProtoMin: 1,
					RegProtoMax: 2,
					Random:      true,
					Persistent:  true,
				}
				natIR, _ := (&natEncoder{nat: nat}).EncodeIR(ctx)
				return []irNode{natIR}
			},
			expected: "masquerade to :1000-2000 random persistent",
		},
		{
			name: "tcp dport 5000 snat to 10.0.0.1-10.0.0.10:1000-2000",
			setup: func(ctx *ctx) []irNode {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "5000"})
				ctx.reg.Set(3, regVal{HumanExpr: "10.0.0.1"})
				ctx.reg.Set(4, regVal{HumanExpr: "10.0.0.10"})
				ctx.reg.Set(5, regVal{HumanExpr: "1000"})
				ctx.reg.Set(6, regVal{HumanExpr: "2000"})

				protoCmp := &expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}}
				dportCmp := &expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x13, 0x88}} // 5000

				nat := &expr.NAT{
					Type:        expr.NATTypeSourceNAT,
					Family:      unix.NFPROTO_IPV4,
					RegAddrMin:  3,
					RegAddrMax:  4,
					RegProtoMin: 5,
					RegProtoMax: 6,
				}

				protoIR, _ := (&cmpEncoder{cmp: protoCmp}).EncodeIR(ctx)
				dportIR, _ := (&cmpEncoder{cmp: dportCmp}).EncodeIR(ctx)
				natIR, _ := (&natEncoder{nat: nat}).EncodeIR(ctx)
				return []irNode{protoIR, dportIR, natIR}
			},
			expected: "tcp dport 5000 snat ip to 10.0.0.1-10.0.0.10:1000-2000",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			ctx := &ctx{}
			irs := tc.setup(ctx)

			result := ""
			for i, ir := range irs {
				if ir == nil {
					continue
				}
				if i > 0 {
					result += " "
				}
				result += ir.Format()
			}

			sui.Equal(tc.expected, result)
		})
	}
}


sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority filter; policy accept; }'
sudo nft add chain ip test postrouting '{ type nat hook postrouting priority 100; policy accept; }'


_____________
sudo nft add rule ip test postrouting tcp dport 5000 snat to 10.0.0.1-10.0.0.10:1000-2000

az358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft list ruleset
table ip test {
        chain prerouting {
                type nat hook prerouting priority filter; policy accept;
        }

        chain postrouting {
                type nat hook postrouting priority srcnat; policy accept;
                tcp dport 5000 snat to 10.0.0.1-10.0.0.10:1000-2000
        }
}
_________________________________________

{
	name: "ip protocol tcp masquerade to :1000 random",
	setup: func(ctx *ctx) []irNode {
		// Установка значений регистров
		ctx.reg.Set(1, regVal{HumanExpr: "tcp"})    // ip protocol match
		ctx.reg.Set(2, regVal{HumanExpr: "1000"})   // masquerade порт

		// protocol match (обязателен для to :port)
		protoCmp := &expr.Cmp{
			Register: 1,
			Op:       expr.CmpOpEq,
			Data:     []byte{unix.IPPROTO_TCP}, // IPPROTO_TCP = 6
		}

		// NAT выражение
		nat := &expr.NAT{
			Type:        NATTypeMASQ,
			Family:      unix.NFPROTO_IPV4,
			RegProtoMin: 2,
			Random:      true,
		}

		// Генерация IR
		protoIR, _ := (&cmpEncoder{cmp: protoCmp}).EncodeIR(ctx)
		natIR, _ := (&natEncoder{nat: nat}).EncodeIR(ctx)

		return []irNode{protoIR, natIR}
	},
	expected: "ip protocol tcp masquerade to :1000 random",
}




sudo nft add rule ip test postrouting ip protocol tcp masquerade to :1000 random




_______________________________________________________________________________





sudo nft list ruleset





















