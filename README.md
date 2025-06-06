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

# 1. ct state new,established
sudo nft add rule inet test prerouting ct state new,established

# 2. ct direction original
sudo nft add rule inet test prerouting ct direction original

# 3. ct expiration 5s
sudo nft add rule inet test prerouting ct expiration 5s

# 4. ct protocol tcp
sudo nft add rule inet test prerouting ct protocol tcp

# 5. ct mark set 42
sudo nft add rule inet test prerouting ct mark set 42

# 6. ct status assured,confirmed,snat,dnat
sudo nft add rule inet test prerouting ct status assured,confirmed,snat,dnat


package utils

import (
	"sort"
	"strings"
)

// NormalizeCtExpr приводит ct-выражения к каноническому виду с отсортированными значениями.
func NormalizeCtExpr(input string) string {
	prefixes := []string{
		"ct state ",
		"ct status ",
		"ct event ",
		"ct state !",
		"ct status !",
		"ct event !",
	}

	for _, prefix := range prefixes {
		if strings.HasPrefix(input, prefix) {
			return normalizeWithPrefix(input, prefix)
		}
		// поддержка выражений типа: ct state != ...
		if strings.HasPrefix(input, prefix[:len(prefix)-1]+"!=") {
			return normalizeWithPrefix(input, prefix[:len(prefix)-1]+"!= ")
		}
	}
	return input
}

func normalizeWithPrefix(input, prefix string) string {
	rest := strings.TrimPrefix(input, prefix)
	rest = strings.TrimSpace(rest)
	values := strings.Split(rest, ",")
	for i := range values {
		values[i] = strings.TrimSpace(values[i])
	}
	sort.Strings(values)
	return prefix + strings.Join(values, ",")
}



package encoders_test

import (
	"testing"

	"github.com/google/nftables"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"your_project_path/internal/utils" // замените на актуальный импорт
)

type ctEncoderAdvancedTestSuite struct {
	suite.Suite
}

func (sui *ctEncoderAdvancedTestSuite) Test_CtEncodeIR_Complex() {
	testData := []struct {
		name     string
		exprs    []expr.Any
		expected string
	}{
		{
			name: "ct state new,established",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeySTATE, Register: 1},
				&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{byte(1 | 2), 0, 0, 0, 0, 0, 0, 0}},
			},
			expected: "ct state new,established",
		},
		{
			name: "ct direction original",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeyDIRECTION, Register: 1},
				&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{0}},
			},
			expected: "ct direction original",
		},
		{
			name: "ct expiration 5s",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeyEXPIRATION, Register: 1},
				&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{0x88, 0x13, 0x00, 0x00}},
			},
			expected: "ct expiration 5s",
		},
		{
			name: "ct protocol tcp",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeyPROTOCOL, Register: 1},
				&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{6}},
			},
			expected: "ct protocol tcp",
		},
		{
			name: "ct mark set 42",
			exprs: []expr.Any{
				&expr.Immediate{Register: 1, Data: []byte{42, 0, 0, 0}},
				&expr.Ct{Key: expr.CtKeyMARK, Register: 1, SourceRegister: true},
			},
			expected: "ct mark set 42",
		},
		{
			name: "ct status snat,dnat,confirmed,assured",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeySTATUS, Register: 1},
				&expr.Cmp{Op: expr.CmpOpEq, Register: 1, Data: []byte{0x3C, 0x00, 0x00, 0x00, 0, 0, 0, 0}},
			},
			expected: "ct status snat,dnat,confirmed,assured",
		},
		{
			name: "ct state != established,invalid",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeySTATE, Register: 1},
				&expr.Cmp{Op: expr.CmpOpNeq, Register: 1, Data: []byte{byte(2 | 4), 0, 0, 0, 0, 0, 0, 0}},
			},
			expected: "ct state != established,invalid",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			rule := nftables.Rule{Exprs: tc.exprs}
			str, err := NewRuleExprEncoder(&rule).Format()
			sui.Require().NoError(err)

			expectedNorm := utils.NormalizeCtExpr(tc.expected)
			actualNorm := utils.NormalizeCtExpr(str)
			sui.Require().Equal(expectedNorm, actualNorm)
		})
	}
}

func Test_CtEncoderAdvanced(t *testing.T) {
	suite.Run(t, new(ctEncoderAdvancedTestSuite))
}









