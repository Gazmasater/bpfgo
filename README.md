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

# Полный юнит-тест с кейсами для payload + bitwise + verdict
bitwise_payload_verdict_tests = '''package encoders

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type payloadBitwiseVerdictTestSuite struct {
	suite.Suite
}

func (sui *payloadBitwiseVerdictTestSuite) Test_PayloadWithBitwiseAndVerdict() {
	testData := []struct {
		name     string
		exprs    []expr.Any
		expected string
	}{
		{
			name: "match ip version 4 accept",
			exprs: []expr.Any{
				&expr.Payload{
					DestRegister: 1,
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
				},
				&expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   1,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				},
				&expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 1,
					Data:     []byte{0x40}, // version 4 << 4
				},
				&expr.Verdict{Kind: expr.VerdictAccept},
			},
			expected: "ip version 4 accept",
		},
		{
			name: "match ip version != 6 drop",
			exprs: []expr.Any{
				&expr.Payload{
					DestRegister: 1,
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
				},
				&expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   1,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				},
				&expr.Cmp{
					Op:       expr.CmpOpNeq,
					Register: 1,
					Data:     []byte{0x60}, // version 6 << 4
				},
				&expr.Verdict{Kind: expr.VerdictDrop},
			},
			expected: "ip version != 6 drop",
		},
		{
			name: "bitwise mask+xor+accept on dport",
			exprs: []expr.Any{
				&expr.Payload{
					DestRegister: 1,
					Base:         expr.PayloadBaseTransportHeader,
					Offset:       2,
					Len:          2,
				},
				&expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   2,
					Len:            2,
					Mask:           []byte{0xFF, 0xFF},
					Xor:            []byte{0x00, 0x10},
				},
				&expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 2,
					Data:     []byte{0x01, 0x50},
				},
				&expr.Verdict{Kind: expr.VerdictAccept},
			},
			expected: "(th dport & 0xffff) ^ 0x10 == 336", // manually calculated for explanation
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			rule := nftables.Rule{Exprs: tc.exprs}
			str, err := NewRuleExprEncoder(&rule).Format()
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, str)
		})
	}
}

func Test_PayloadBitwiseVerdict(t *testing.T) {
	suite.Run(t, new(payloadBitwiseVerdictTestSuite))
}
'''

# Сохраняем файл
file_path = "/mnt/data/payload_bitwise_verdict_test.go"
with open(file_path, "w") as f:
    f.write(bitwise_payload_verdict_tests)

file_path



gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_PayloadEncoderWithVerdict (0.00s)
    --- FAIL: Test_PayloadEncoderWithVerdict/Test_PayloadExprToString_WithVerdict (0.00s)
        --- FAIL: Test_PayloadEncoderWithVerdict/Test_PayloadExprToString_WithVerdict/bitwise_mask+xor+accept_on_dport (0.00s)
            encodersPayload_test.go:162: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersPayload_test.go:162
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "(th dport & 0xffff) ^ 0x10 == 336"
                                        actual  : "th dport 336 accept"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -(th dport & 0xffff) ^ 0x10 == 336
                                        +th dport 336 accept
                        Test:           Test_PayloadEncoderWithVerdict/Test_PayloadExprToString_WithVerdict/bitwise_mask+xor+accept_on_dport
[{"match":{"op":"==","left":{"meta":{"key":"l4proto"}},"right":"tcp"}},{"counter":{"bytes":0,"packets":0}},{"log":null},{"accept":null}]
[{"match":{"op":"!=","left":{"meta":{"key":"oifname"}},"right":"lo"}},{"mangle":{"key":{"meta":{"key":"nftrace"}},"value":1}},{"goto":{"target":"FW-OUT"}}]
meta l4proto tcp counter packets 0 bytes 0 log accept
ip version != 5
ip daddr @ipSet
ip daddr != 93.184.216.34 meta l4proto tcp dport {80,443} meta l4proto tcp
th dport != 80
meta l4proto tcp dport != 80
meta l4proto tcp sport >= 80 sport <= 100
meta nftrace set 1 ip daddr 10.0.0.0/8 meta l4proto udp
meta l4proto icmp type echo-reply
ct state established,related
ct expiration 1s
ct direction original
ct l3proto ipv4
ct protocol tcp
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.020s








