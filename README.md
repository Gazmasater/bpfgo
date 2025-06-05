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
	"golang.org/x/sys/unix"
)

func TestNatEncodeIR(t *testing.T) {
	tests := []struct {
		name    string
		nat     *expr.NAT
		regVals map[regID]regVal
		want    string
	}{
		{
			name: "simple dnat",
			nat: &expr.NAT{
				Type:        expr.NATTypeDestNAT,
				Family:      unix.NFPROTO_IPV4,
				RegAddrMin:  1,
				RegProtoMin: 2,
			},
			regVals: map[regID]regVal{
				1: {HumanExpr: "192.168.0.1"},
				2: {HumanExpr: "8080"},
			},
			want: "dnat ip to 192.168.0.1:8080",
		},
		{
			name: "masquerade with port range",
			nat: &expr.NAT{
				Type:        NATTypeMASQ,
				Family:      unix.NFPROTO_IPV4,
				RegProtoMin: 3,
				RegProtoMax: 4,
			},
			regVals: map[regID]regVal{
				3: {HumanExpr: "1000"},
				4: {HumanExpr: "2000"},
			},
			want: "masquerade to :1000-2000",
		},
		{
			name: "snat with addr range",
			nat: &expr.NAT{
				Type:       expr.NATTypeSourceNAT,
				Family:     unix.NFPROTO_IPV4,
				RegAddrMin: 1,
				RegAddrMax: 2,
			},
			regVals: map[regID]regVal{
				1: {HumanExpr: "10.0.0.1"},
				2: {HumanExpr: "10.0.0.5"},
			},
			want: "snat ip to 10.0.0.1-10.0.0.5",
		},
		{
			name: "redirect ipv6 single port",
			nat: &expr.NAT{
				Type:        NATTypeRedir,
				Family:      unix.NFPROTO_IPV6,
				RegProtoMin: 5,
			},
			regVals: map[regID]regVal{
				5: {HumanExpr: "443"},
			},
			want: "redirect ip6 to :443",
		},
		{
			name: "dnat with flags",
			nat: &expr.NAT{
				Type:        expr.NATTypeDestNAT,
				Family:      unix.NFPROTO_IPV4,
				RegAddrMin:  1,
				RegProtoMin: 2,
				Random:      true,
				Persistent:  true,
			},
			regVals: map[regID]regVal{
				1: {HumanExpr: "10.1.1.1"},
				2: {HumanExpr: "8080"},
			},
			want: "dnat ip to 10.1.1.1:8080 random persistent",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			ctx := &ctx{reg: regHolder{cache: tt.regVals}}
			enc := &natEncoder{nat: tt.nat}
			ir, err := enc.EncodeIR(ctx)
			if err != nil {
				t.Fatalf("unexpected error: %v", err)
			}
			got := ir.Format()
			if got != tt.want {
				t.Errorf("expected: %q, got: %q", tt.want, got)
			}
		})
	}
}






🧩 Шаг 1: Создать таблицу
bash
Копировать
Редактировать
sudo nft add table ip test
🧩 Шаг 2: Создать цепочки NAT
prerouting (для dnat, redirect)
bash
Копировать
Редактировать
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
postrouting (для snat, masquerade)
bash
Копировать
Редактировать
sudo nft add chain ip test postrouting '{ type nat hook postrouting priority 100; }'
🧩 Шаг 3: Добавить правила
1. DNAT к IP и порту
bash
Копировать
Редактировать
sudo nft add rule ip test prerouting dnat to 192.168.0.1:8080
2. MASQUERADE с диапазоном портов
bash
Копировать
Редактировать
sudo nft add rule ip test postrouting masquerade to :1000-2000
3. SNAT с диапазоном IP-адресов
bash
Копировать
Редактировать
sudo nft add rule ip test postrouting snat to 10.0.0.1-10.0.0.5
4. REDIRECT на порт (IPv6)
bash
Копировать
Редактировать
sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting redirect to :443
5. DNAT с флагами random, persistent
bash
Копировать
Редактировать
sudo nft add rule ip test prerouting dnat to 10.1.1.1:8080 random persistent

















