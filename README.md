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



package encoders

import (
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type natCmpRulesetStyleTestSuite struct {
	suite.Suite
}

func (sui *natCmpRulesetStyleTestSuite) Test_TCPNATRules() {
	testCases := []struct {
		name     string
		setup    func(ctx *ctx) []expr.Any
		expected string
	}{
		{
			name: "tcp dport 8080 dnat to 192.168.0.1:8080",
			setup: func(ctx *ctx) []expr.Any {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "8080"})
				ctx.reg.Set(3, regVal{HumanExpr: "192.168.0.1"})
				ctx.reg.Set(4, regVal{HumanExpr: "8080"})

				return []expr.Any{
					&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
					&expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x1f, 0x90}},
					&expr.NAT{
						Type:        expr.NATTypeDestNAT,
						Family:      unix.NFPROTO_IPV4,
						RegAddrMin:  3,
						RegProtoMin: 4,
					},
				}
			},
			expected: "tcp dport 8080 dnat ip to 192.168.0.1:8080",
		},
		{
			name: "tcp dport 443 redirect to :443",
			setup: func(ctx *ctx) []expr.Any {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "443"})
				ctx.reg.Set(3, regVal{HumanExpr: "443"})

				return []expr.Any{
					&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
					&expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x01, 0xbb}},
					&expr.NAT{
						Type:        NATTypeRedir,
						Family:      unix.NFPROTO_IPV4,
						RegProtoMin: 3,
					},
				}
			},
			expected: "tcp dport 443 redirect ip to :443",
		},
		{
			name: "ip protocol tcp masquerade to :1000 random",
			setup: func(ctx *ctx) []expr.Any {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "1000"})

				return []expr.Any{
					&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
					&expr.NAT{
						Type:        NATTypeMASQ,
						Family:      unix.NFPROTO_IPV4,
						RegProtoMin: 2,
						Random:      true,
					},
				}
			},
			expected: "ip protocol tcp masquerade to :1000 random",
		},
		{
			name: "tcp dport 5000 snat to 10.0.0.1-10.0.0.10:1000-2000",
			setup: func(ctx *ctx) []expr.Any {
				ctx.reg.Set(1, regVal{HumanExpr: "tcp"})
				ctx.reg.Set(2, regVal{HumanExpr: "5000"})
				ctx.reg.Set(3, regVal{HumanExpr: "10.0.0.1"})
				ctx.reg.Set(4, regVal{HumanExpr: "10.0.0.10"})
				ctx.reg.Set(5, regVal{HumanExpr: "1000"})
				ctx.reg.Set(6, regVal{HumanExpr: "2000"})

				return []expr.Any{
					&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
					&expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x13, 0x88}},
					&expr.NAT{
						Type:        expr.NATTypeSourceNAT,
						Family:      unix.NFPROTO_IPV4,
						RegAddrMin:  3,
						RegAddrMax:  4,
						RegProtoMin: 5,
						RegProtoMax: 6,
					},
				}
			},
			expected: "tcp dport 5000 snat ip to 10.0.0.1-10.0.0.10:1000-2000",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			ctx := &ctx{}
			exprs := tc.setup(ctx)
			result := ""
			for i, ex := range exprs {
				var ir irNode
				var err error
				switch e := ex.(type) {
				case *expr.Cmp:
					ir, err = (&cmpEncoder{cmp: e}).EncodeIR(ctx)
				case *expr.NAT:
					ir, err = (&natEncoder{nat: e}).EncodeIR(ctx)
				}
				sui.Require().NoError(err)
				if i > 0 {
					result += " "
				}
				result += ir.Format()
			}
			sui.Equal(tc.expected, result)
		})
	}
}

func Test_NatCmpRulesetStyle(t *testing.T) {
	suite.Run(t, new(natCmpRulesetStyleTestSuite))
}


___________________________________

gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test -v
=== RUN   Test_DynsetEncodeIR
=== RUN   Test_DynsetEncodeIR/Test_DynsetEncodeIR
=== RUN   Test_DynsetEncodeIR/Test_DynsetEncodeIR/add_to_IPv4_set
=== RUN   Test_DynsetEncodeIR/Test_DynsetEncodeIR/add_to_set_with_timeout
=== RUN   Test_DynsetEncodeIR/Test_DynsetEncodeIR/update_set_with_counter
--- PASS: Test_DynsetEncodeIR (0.00s)
    --- PASS: Test_DynsetEncodeIR/Test_DynsetEncodeIR (0.00s)
        --- PASS: Test_DynsetEncodeIR/Test_DynsetEncodeIR/add_to_IPv4_set (0.00s)
        --- PASS: Test_DynsetEncodeIR/Test_DynsetEncodeIR/add_to_set_with_timeout (0.00s)
        --- PASS: Test_DynsetEncodeIR/Test_DynsetEncodeIR/update_set_with_counter (0.00s)
=== RUN   Test_CmpEncoderAdvanced
=== RUN   Test_CmpEncoderAdvanced/Test_CmpEncodeIR
=== RUN   Test_CmpEncoderAdvanced/Test_CmpEncodeIR/ct_state_!=_established
=== RUN   Test_CmpEncoderAdvanced/Test_CmpEncodeIR/payload_ip_version_!=_5
    panic.go:262: test panicked: runtime error: invalid memory address or nil pointer dereference
        goroutine 15 [running]:
        runtime/debug.Stack()
                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/debug/stack.go:26 +0x5e
        github.com/stretchr/testify/suite.failOnPanic(0xc0002768c0, {0x8b5b00, 0xd2f540})
                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:89 +0x37
        github.com/stretchr/testify/suite.recoverAndFailOnPanic(0xc0002768c0)
                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:83 +0x35
        panic({0x8b5b00?, 0xd2f540?})
                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/runtime/panic.go:792 +0x132
        github.com/Morwran/nft-go/internal/expr-encoders.includeHeaderIfKnown(...)
                /home/gaz358/myprog/nft-go/internal/expr-encoders/payload.go:213
        github.com/Morwran/nft-go/internal/expr-encoders.(*payloadEncoder).buildLRFromCmpData(0x75b87ca578d8?, 0xc0001ff520, 0xc0001ff540)
                /home/gaz358/myprog/nft-go/internal/expr-encoders/payload.go:158 +0x30
        github.com/Morwran/nft-go/internal/expr-encoders.(*cmpEncoder).formatCmpLR(0x75b87cbc0108?, 0x20?, {{0x93bfbe, 0xa}, 0x0, {0x9d79c0, 0xc00002c900}, {0x0, 0x0}, {0x0, ...}})
                /home/gaz358/myprog/nft-go/internal/expr-encoders/cmp.go:109 +0x134
        github.com/Morwran/nft-go/internal/expr-encoders.(*cmpEncoder).EncodeIR(0xc00006eea8, 0xc0001ff520)
                /home/gaz358/myprog/nft-go/internal/expr-encoders/cmp.go:36 +0x174
        github.com/Morwran/nft-go/internal/expr-encoders.(*cmpEncoderAdvancedTestSuite).Test_CmpEncodeIR.func4()
                /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCmp_test.go:80 +0x67
        github.com/stretchr/testify/suite.(*Suite).Run.func1(0xc0002768c0)
                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115 +0x169
        testing.tRunner(0xc0002768c0, 0xc0001ff500)
                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1792 +0xf4
        created by testing.(*T).Run in goroutine 13
                /home/gaz358/go/pkg/mod/golang.org/toolchain@v0.0.1-go1.24.2.linux-amd64/src/testing/testing.go:1851 +0x413
=== RUN   Test_CmpEncoderAdvanced/Test_CmpEncodeIR/meta_cpu_==_3
--- FAIL: Test_CmpEncoderAdvanced (0.00s)
    --- FAIL: Test_CmpEncoderAdvanced/Test_CmpEncodeIR (0.00s)
        --- PASS: Test_CmpEncoderAdvanced/Test_CmpEncodeIR/ct_state_!=_established (0.00s)
        --- FAIL: Test_CmpEncoderAdvanced/Test_CmpEncodeIR/payload_ip_version_!=_5 (0.00s)
        --- PASS: Test_CmpEncoderAdvanced/Test_CmpEncodeIR/meta_cpu_==_3 (0.00s)
=== RUN   Test_ConnlimitEncoder
=== RUN   Test_ConnlimitEncoder/Test_ConnlimitEncodeIR
=== RUN   Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/basic_count
=== RUN   Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/with_inv_flag
=== RUN   Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/zero_count
=== RUN   Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/max_uint32_count
=== RUN   Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/unknown_flags_fallback_to_over
--- PASS: Test_ConnlimitEncoder (0.00s)
    --- PASS: Test_ConnlimitEncoder/Test_ConnlimitEncodeIR (0.00s)
        --- PASS: Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/basic_count (0.00s)
        --- PASS: Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/with_inv_flag (0.00s)
        --- PASS: Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/zero_count (0.00s)
        --- PASS: Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/max_uint32_count (0.00s)
        --- PASS: Test_ConnlimitEncoder/Test_ConnlimitEncodeIR/unknown_flags_fallback_to_over (0.00s)
=== RUN   Test_CtEncoderAdvanced
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_state_new,established
    encodersCt_test.go:127: 
                Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCt_test.go:127
                                                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                Error:          Not equal: 
                                expected: "ct state new,established"
                                actual  : "ct state established,new"
                            
                                Diff:
                                --- Expected
                                +++ Actual
                                @@ -1 +1 @@
                                -ct state new,established
                                +ct state established,new
                Test:           Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_state_new,established
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_direction_original
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_expiration_5s
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_protocol_tcp
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_mark_set_42
    encodersCt_test.go:127: 
                Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCt_test.go:127
                                                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                Error:          Not equal: 
                                expected: "ct mark set 42"
                                actual  : "ct mark set *"
                            
                                Diff:
                                --- Expected
                                +++ Actual
                                @@ -1 +1 @@
                                -ct mark set 42
                                +ct mark set *
                Test:           Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_mark_set_42
=== RUN   Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_status_snat,dnat,confirmed
    encodersCt_test.go:127: 
                Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCt_test.go:127
                                                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                Error:          Not equal: 
                                expected: "ct status snat,dnat,confirmed"
                                actual  : "ct status assured,confirmed,snat,dnat"
                            
                                Diff:
                                --- Expected
                                +++ Actual
                                @@ -1 +1 @@
                                -ct status snat,dnat,confirmed
                                +ct status assured,confirmed,snat,dnat
                Test:           Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_status_snat,dnat,confirmed
--- FAIL: Test_CtEncoderAdvanced (0.00s)
    --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex (0.00s)
        --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_state_new,established (0.00s)
        --- PASS: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_direction_original (0.00s)
        --- PASS: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_expiration_5s (0.00s)
        --- PASS: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_protocol_tcp (0.00s)
        --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_mark_set_42 (0.00s)
        --- FAIL: Test_CtEncoderAdvanced/Test_CtEncodeIR_Complex/ct_status_snat,dnat,confirmed (0.00s)
=== RUN   Test_ExthdrEncoder
=== RUN   Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly
=== RUN   Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/tcp_option_present_→_store_to_register
=== RUN   Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/ipv6_option_present_→_store_to_register
=== RUN   Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/exthdr_read_and_compare_(source_+_rhs)
    encodersExthdr_test.go:71: 
                Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersExthdr_test.go:71
                                                        /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                Error:          Not equal: 
                                expected: "exthdr @4,8,2 set 0x1234"
                                actual  : "ip option @4,8,2 set 0x1234"
                            
                                Diff:
                                --- Expected
                                +++ Actual
                                @@ -1 +1 @@
                                -exthdr @4,8,2 set 0x1234
                                +ip option @4,8,2 set 0x1234
                Test:           Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/exthdr_read_and_compare_(source_+_rhs)
--- FAIL: Test_ExthdrEncoder (0.00s)
    --- FAIL: Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly (0.00s)
        --- PASS: Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/tcp_option_present_→_store_to_register (0.00s)
        --- PASS: Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/ipv6_option_present_→_store_to_register (0.00s)
        --- FAIL: Test_ExthdrEncoder/Test_ExthdrEncodeIR_ValidOnly/exthdr_read_and_compare_(source_+_rhs) (0.00s)
=== RUN   Test_Encoders
=== RUN   Test_Encoders/Test_DupExprToString
=== RUN   Test_Encoders/Test_DupExprToString/dup_to_address
=== RUN   Test_Encoders/Test_DupExprToString/dup_to_address_and_device
=== RUN   Test_Encoders/Test_MultipleExprToJSON
=== RUN   Test_Encoders/Test_MultipleExprToJSON/Expression_1
[{"match":{"op":"==","left":{"meta":{"key":"l4proto"}},"right":"tcp"}},{"counter":{"bytes":0,"packets":0}},{"log":null},{"accept":null}]
=== RUN   Test_Encoders/Test_MultipleExprToJSON/Expression_2
[{"match":{"op":"!=","left":{"meta":{"key":"oifname"}},"right":"lo"}},{"mangle":{"key":{"meta":{"key":"nftrace"}},"value":1}},{"goto":{"target":"FW-OUT"}}]
=== RUN   Test_Encoders/Test_MultipleExprToString
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_1
meta l4proto tcp counter packets 0 bytes 0 log accept
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_3
ip version != 5
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_4
ip daddr @ipSet
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_5
ip daddr != 93.184.216.34 meta l4proto tcp dport {80,443} meta l4proto tcp
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_7
th dport != 80
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_8
meta l4proto tcp dport != 80
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_9
meta l4proto tcp sport >= 80 sport <= 100
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_10
meta nftrace set 1 ip daddr 10.0.0.0/8 meta l4proto udp
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_11
meta l4proto icmp type echo-reply
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_11#01
ct state established,related
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_12
ct expiration 1s
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_13
ct direction original
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_14
ct l3proto ipv4
=== RUN   Test_Encoders/Test_MultipleExprToString/Expression_15
ct protocol tcp
--- PASS: Test_Encoders (0.00s)
    --- PASS: Test_Encoders/Test_DupExprToString (0.00s)
        --- PASS: Test_Encoders/Test_DupExprToString/dup_to_address (0.00s)
        --- PASS: Test_Encoders/Test_DupExprToString/dup_to_address_and_device (0.00s)
    --- PASS: Test_Encoders/Test_MultipleExprToJSON (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToJSON/Expression_1 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToJSON/Expression_2 (0.00s)
    --- PASS: Test_Encoders/Test_MultipleExprToString (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_1 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_3 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_4 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_5 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_7 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_8 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_9 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_10 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_11 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_11#01 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_12 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_13 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_14 (0.00s)
        --- PASS: Test_Encoders/Test_MultipleExprToString/Expression_15 (0.00s)
FAIL
exit status 1
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.022s
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 





