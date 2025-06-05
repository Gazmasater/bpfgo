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
	"time"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type dynsetIRTestSuite struct {
	suite.Suite
}

func (sui *dynsetIRTestSuite) Test_DynsetEncodeIR() {
	testData := []struct {
		name     string
		dynset   *expr.Dynset
		srcKey   string
		srcData  string
		expected string
	}{
		{
			name: "add to IPv4 set",
			dynset: &expr.Dynset{
				Operation:  uint32(DynSetOPAdd),
				SetName:    "testset",
				SrcRegKey:  1,
			},
			srcKey:   "ip saddr",
			expected: "add @testset { ip saddr }",
		},
		{
			name: "add to set with timeout",
			dynset: &expr.Dynset{
				Operation:  uint32(DynSetOPAdd),
				SetName:    "timeoutset",
				SrcRegKey:  2,
				Timeout:    10 * time.Second,
			},
			srcKey:   "ip saddr",
			expected: "add @timeoutset { ip saddr timeout 10s }",
		},
		{
			name: "update set with counter",
			dynset: &expr.Dynset{
				Operation:  uint32(DynSetOPUpdate),
				SetName:    "updset",
				SrcRegKey:  3,
				Exprs: []expr.Any{
					&expr.Counter{},
				},
			},
			srcKey:   "ip saddr",
			expected: "update @updset { ip saddr counter packets 0 bytes 0 }",
		},
		{
			name: "delete from map with data and counter",
			dynset: &expr.Dynset{
				Operation:  uint32(DynSetOPDelete),
				SetName:    "delset",
				SrcRegKey:  4,
				SrcRegData: 5,
				Exprs: []expr.Any{
					&expr.Counter{},
				},
			},
			srcKey:   "ip saddr",
			srcData:  "lo",
			expected: "delete @delset { ip saddr counter packets 0 bytes 0 : lo }",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			reg := regHolder{}
			reg.Set(regID(tc.dynset.SrcRegKey), regVal{HumanExpr: tc.srcKey})
			if tc.dynset.SrcRegData != 0 {
				reg.Set(regID(tc.dynset.SrcRegData), regVal{HumanExpr: tc.srcData})
			}

			ctx := &ctx{
				reg:  reg,
				rule: &nftables.Rule{},
			}
			enc := &dynsetEncoder{dynset: tc.dynset}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.Format())
		})
	}
}

func Test_DynsetEncodeIR(t *testing.T) {
	suite.Run(t, new(dynsetIRTestSuite))
}



# Инициализация
sudo nft flush ruleset
sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type filter hook prerouting priority 0; }'

sudo nft add set ip test testset { type ipv4_addr\; flags dynamic\; }
sudo nft add rule ip test prerouting add @testset { ip saddr }

sudo nft add set ip test timeoutset { type ipv4_addr\; timeout 10s\; flags dynamic\; }
sudo nft add rule ip test prerouting add @timeoutset { ip saddr timeout 10s }

sudo nft add set ip test updset { type ipv4_addr\; flags dynamic\; }
sudo nft add rule ip test prerouting update @updset { ip saddr counter }

sudo nft add set ip test delset { type ipv4_addr : ifname\; flags dynamic\; }
sudo nft add rule ip test prerouting delete @delset { ip saddr counter : "lo" }

sudo nft list table ip test



func (b *dynsetEncoder) EncodeIR(ctx *ctx) (irNode, error) {
	dyn := b.dynset
	if ctx.rule == nil {
		return nil, errors.New("ctx has no rule")
	}

	srcRegKey, ok := ctx.reg.Get(regID(dyn.SrcRegKey))
	if !ok {
		return nil, errors.Errorf("%T statement has no key expression", dyn)
	}
	exp := srcRegKey.HumanExpr

	// Формируем выражение из Exprs (например, counter/log)
	tmpRule := nftables.Rule{
		Table: ctx.rule.Table,
		Exprs: dyn.Exprs,
	}
	exprsStr, err := NewRuleExprEncoder(&tmpRule).Format()
	if err != nil {
		return nil, err
	}

	// Добавляем timeout, если есть
	if dyn.Timeout != 0 {
		exp = fmt.Sprintf("%s timeout %s", exp, dyn.Timeout)
	}

	setName := fmt.Sprintf("@%s", dyn.SetName)
	sb := strings.Builder{}

	sb.WriteString(fmt.Sprintf("%s %s { %s", DynSetOP(dyn.Operation), setName, exp))

	if exprsStr != "" {
		sb.WriteString(" ")
		sb.WriteString(exprsStr)
	}

	// Добавляем ": value", если есть data
	if srcRegData, ok := ctx.reg.Get(regID(dyn.SrcRegData)); ok {
		if exprData := srcRegData.HumanExpr; exprData != "" {
			sb.WriteString(fmt.Sprintf(" : %s", exprData))
		}
	}

	sb.WriteString(" }")

	return simpleIR(sb.String()), nil
}


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add set ip test delset { type ipv4_addr : ifname\; flags dynamic\; }
Error: syntax error, unexpected colon, expecting newline or semicolon or .
add set ip test delset { type ipv4_addr : ifname; flags dynamic; }
                                        ^
Error: set definition does not specify key
add set ip test delset { type ipv4_addr : ifname; flags dynamic; }












