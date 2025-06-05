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

func (sui *encodersTestSuite) Test_MultipleExprToString() {
	const tableName = "test"

	testData := []struct {
		name     string
		exprs    nftables.Rule
		preRun   func()
		expected string
	}{
		// ... (твои предыдущие тесты)

		{
			name: "dup to address",
			exprs: nftables.Rule{
				Table: &nftables.Table{Name: tableName},
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte("10.1.2.3")},
					&expr.Dup{RegAddr: 1},
				},
			},
			expected: "dup to 10.1.2.3",
		},
		{
			name: "dup to address and device",
			exprs: nftables.Rule{
				Table: &nftables.Table{Name: tableName},
				Exprs: []expr.Any{
					&expr.Immediate{Register: 1, Data: []byte("192.168.1.10")},
					&expr.Immediate{Register: 2, Data: []byte("lo")},
					&expr.Dup{RegAddr: 1, RegDev: 2},
				},
			},
			expected: "dup to 192.168.1.10 device lo",
		},
		{
			name: "dup only device",
			exprs: nftables.Rule{
				Table: &nftables.Table{Name: tableName},
				Exprs: []expr.Any{
					&expr.Immediate{Register: 2, Data: []byte("br-lan")},
					&expr.Dup{RegDev: 2},
				},
			},
			expected: "dup",
		},
		{
			name: "dup no params",
			exprs: nftables.Rule{
				Table: &nftables.Table{Name: tableName},
				Exprs: []expr.Any{
					&expr.Dup{},
				},
			},
			expected: "dup",
		},
	}

	for _, t := range testData {
		sui.Run(t.name, func() {
			if t.preRun != nil {
				t.preRun()
			}
			str, err := NewRuleExprEncoder(&t.exprs).Format()
			sui.Require().NoError(err)
			sui.Require().Equal(t.expected, str)
		})
	}
}



go test -run Test_DupExprToString

____________________________________________________________________

package encoders

import (
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type exthdrEncoderTestSuite struct {
	suite.Suite
}

func (sui *exthdrEncoderTestSuite) Test_ExthdrEncodeIR_ValidOnly() {
	testCases := []struct {
		name     string
		exthdr   *expr.Exthdr
		regSetup func(ctx *ctx)
		expected string
	}{
		{
			name: "tcp option present → store to register",
			exthdr: &expr.Exthdr{
				Op:           expr.ExthdrOpTcpopt,
				Type:         2,
				Flags:        unix.NFT_EXTHDR_F_PRESENT,
				DestRegister: 1,
			},
			expected: "", // EncodeIR вернёт nil, ErrNoIR
		},
		{
			name: "ipv6 option present → store to register",
			exthdr: &expr.Exthdr{
				Op:           expr.ExthdrOpIpv6,
				Type:         1,
				Flags:        unix.NFT_EXTHDR_F_PRESENT,
				DestRegister: 2,
			},
			expected: "", // тоже вернёт nil, ErrNoIR
		},
		{
			name: "exthdr read and compare (source + rhs)",
			exthdr: &expr.Exthdr{
				Type:           4,
				Offset:         8,
				Len:            2,
				SourceRegister: 3,
			},
			regSetup: func(ctx *ctx) {
				ctx.reg.Set(3, regVal{HumanExpr: "0x1234"})
			},
			expected: "exthdr @4,8,2 set 0x1234",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			ctx := &ctx{}
			if tc.regSetup != nil {
				tc.regSetup(ctx)
			}
			enc := &exthdrEncoder{extdhdr: tc.exthdr}
			ir, err := enc.EncodeIR(ctx)

			if tc.expected == "" {
				sui.Require().ErrorIs(err, ErrNoIR)
				sui.Require().Nil(ir)
			} else {
				sui.Require().NoError(err)
				sui.Require().Equal(tc.expected, ir.Format())
			}
		})
	}
}

func Test_ExthdrEncoder(t *testing.T) {
	suite.Run(t, new(exthdrEncoderTestSuite))
}



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'

1. Чтение расширенного заголовка exthdr @0,0,1 => reg 1
(читаем 1 байт из IPv6 exthdr с type=0, offset=0)


sudo nft add rule ip6 test prerouting exthdr load 1b @0,0 => reg 1
2. Сравнение значения exthdr (например, 0x01)

sudo nft add rule ip6 test prerouting exthdr load 1b @0,0 == 0x01
3. Сравнение длины заголовка (если поддерживается)

sudo nft add rule ip6 test prerouting exthdr hdrlength 0 == 8




















