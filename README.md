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

package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type rejectEncoderViaRuleTestSuite struct {
	suite.Suite
}

func (s *rejectEncoderViaRuleTestSuite) Test_RejectEncodeViaRuleEncoder() {
	testCases := []struct {
		name     string
		reject   *expr.Reject
		expected string
	}{
		{
			name: "tcp reset",
			reject: &expr.Reject{
				Type: unix.NFT_REJECT_TCP_RST,
				Code: 0,
			},
			expected: "reject with tcp reset 0",
		},
		{
			name: "icmpv4 (NFPROTO_IPV4)",
			reject: &expr.Reject{
				Type: unix.NFT_REJECT_ICMP_UNREACH,
				Code: unix.NFPROTO_IPV4,
			},
			expected: "reject with icmp 2",
		},
		{
			name: "icmpv6 (NFPROTO_IPV6)",
			reject: &expr.Reject{
				Type: unix.NFT_REJECT_ICMP_UNREACH,
				Code: unix.NFPROTO_IPV6,
			},
			expected: "reject with icmpv6 10",
		},
		{
			name: "icmpx non-port-unreach",
			reject: &expr.Reject{
				Type: unix.NFT_REJECT_ICMPX_UNREACH,
				Code: 5,
			},
			expected: "reject with icmpx 5",
		},
		{
			name: "icmpx with port unreachable — silent",
			reject: &expr.Reject{
				Type: unix.NFT_REJECT_ICMPX_UNREACH,
				Code: unix.NFT_REJECT_ICMPX_PORT_UNREACH,
			},
			expected: "reject",
		},
		{
			name: "empty reject",
			reject: &expr.Reject{
				Type: 0,
				Code: 0,
			},
			expected: "reject",
		},
	}

	for _, tc := range testCases {
		s.Run(tc.name, func() {
			rule := &nftables.Rule{
				Exprs: []expr.Any{
					tc.reject,
				},
			}
			str, err := NewRuleExprEncoder(rule).Format()
			s.Require().NoError(err)
			s.Equal(tc.expected, str)
		})
	}
}

func Test_RejectEncoderViaRule(t *testing.T) {
	suite.Run(t, new(rejectEncoderViaRuleTestSuite))
}



sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip test prerouting reject with tcp reset
sudo nft add rule ip test prerouting reject with icmp type 2
sudo nft add rule ip test prerouting reject with icmpv6 type 10
sudo nft add rule ip test prerouting reject with icmpx type 5
sudo nft add rule ip test prerouting reject














