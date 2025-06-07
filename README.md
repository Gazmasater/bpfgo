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
		expected string
	}{
		{
			name: "add to IPv4 set",
			dynset: &expr.Dynset{
				Operation: uint32(DynSetOPAdd),
				SetName:   "testset",
				SrcKeyExpr: &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       12,
					Len:          4,
					DestRegister: 1,
				},
			},
			expected: "add @testset { ip saddr }",
		},
		{
			name: "add to set with timeout",
			dynset: &expr.Dynset{
				Operation: uint32(DynSetOPAdd),
				SetName:   "timeoutset",
				Timeout:   10 * time.Second,
				SrcKeyExpr: &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       12,
					Len:          4,
					DestRegister: 2,
				},
			},
			expected: "add @timeoutset { ip saddr timeout 10s }",
		},
		{
			name: "update set with counter",
			dynset: &expr.Dynset{
				Operation: uint32(DynSetOPUpdate),
				SetName:   "updset",
				Exprs: []expr.Any{
					&expr.Counter{},
				},
				SrcKeyExpr: &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       12,
					Len:          4,
					DestRegister: 3,
				},
			},
			expected: "update @updset { ip saddr counter packets 0 bytes 0 }",
		},
	}

	for _, tc := range testData {
		sui.Run(tc.name, func() {
			ctx := &ctx{
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
		exprs    []expr.Any
		expected string
	}{
		{
			name: "tcp dport 8080 dnat to 192.168.0.1:8080",
			exprs: []expr.Any{
				&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
				&expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x1f, 0x90}},
				&expr.NAT{
					Type:        expr.NATTypeDestNAT,
					Family:      unix.NFPROTO_IPV4,
					RegAddrMin:  3,
					RegProtoMin: 4,
				},
			},
			expected: "tcp dport 8080 dnat ip to 192.168.0.1:8080",
		},
		{
			name: "tcp dport 443 redirect to :443",
			exprs: []expr.Any{
				&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
				&expr.Cmp{Register: 2, Op: expr.CmpOpEq, Data: []byte{0x01, 0xbb}},
				&expr.NAT{
					Type:        NATTypeRedir,
					Family:      unix.NFPROTO_IPV4,
					RegProtoMin: 3,
				},
			},
			expected: "tcp dport 443 redirect ip to :443",
		},
		{
			name: "ip protocol tcp masquerade to :1000 random",
			exprs: []expr.Any{
				&expr.Cmp{Register: 1, Op: expr.CmpOpEq, Data: []byte{unix.IPPROTO_TCP}},
				&expr.NAT{
					Type:        NATTypeMASQ,
					Family:      unix.NFPROTO_IPV4,
					RegProtoMin: 2,
					Random:      true,
				},
			},
			expected: "ip protocol tcp masquerade to :1000 random",
		},
		{
			name: "tcp dport 5000 snat to 10.0.0.1-10.0.0.10:1000-2000",
			exprs: []expr.Any{
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
			},
			expected: "tcp dport 5000 snat ip to 10.0.0.1-10.0.0.10:1000-2000",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			ctx := &ctx{}
			result := ""
			for i, ex := range tc.exprs {
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
			sui.Require().Equal(tc.expected, result)
		})
	}
}

func Test_NatCmpRulesetStyle(t *testing.T) {
	suite.Run(t, new(natCmpRulesetStyleTestSuite))
}



package encoders

import (
	"testing"

	"github.com/Morwran/nft-go/pkg/protocols"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type cmpEncoderAdvancedTestSuite struct {
	suite.Suite
}

func (sui *cmpEncoderAdvancedTestSuite) Test_CmpEncodeIR() {
	testCases := []struct {
		name     string
		cmp      *expr.Cmp
		expected string
	}{
		{
			name: "ct state != established",
			cmp: &expr.Cmp{
				Op:       expr.CmpOpNeq,
				Register: 1,
				Data:     []byte{byte(CtStateBitESTABLISHED), 0, 0, 0, 0, 0, 0, 0},
			},
			expected: "ct state != established",
		},
		{
			name: "payload ip version != 5",
			cmp: &expr.Cmp{
				Op:       expr.CmpOpNeq,
				Register: 2,
				Data:     []byte{0x50},
			},
			expected: "ip version != 5",
		},
		{
			name: "ip version == 4",
			cmp: &expr.Cmp{
				Op:       expr.CmpOpEq,
				Register: 2,
				Data:     []byte{0x40},
			},
			expected: "ip version 4",
		},
		{
			name: "ip version == 6",
			cmp: &expr.Cmp{
				Op:       expr.CmpOpEq,
				Register: 2,
				Data:     []byte{0x60},
			},
			expected: "ip version 6",
		},
		{
			name: "meta cpu == 3",
			cmp: &expr.Cmp{
				Op:       expr.CmpOpEq,
				Register: 1,
				Data:     []byte{3},
			},
			expected: "meta cpu 3",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			ctx := &ctx{
				hdr: new(protocols.ProtoDescPtr),
			}
			enc := &cmpEncoder{cmp: tc.cmp}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.Format())
		})
	}
}

func Test_CmpEncoderAdvanced(t *testing.T) {
	suite.Run(t, new(cmpEncoderAdvancedTestSuite))
}


package encoders

import (
	"testing"

	"github.com/Morwran/nft-go/pkg/protocols"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"golang.org/x/sys/unix"
)

type cmpEncoderAdvancedTestSuite struct {
	suite.Suite
}

func (sui *cmpEncoderAdvancedTestSuite) Test_CmpEncodeIR() {
	testCases := []struct {
		name     string
		exprs    []expr.Any
		expected string
	}{
		{
			name: "ct state != established",
			exprs: []expr.Any{
				&expr.Ct{Key: expr.CtKeySTATE, Register: 1},
				&expr.Cmp{
					Op:       expr.CmpOpNeq,
					Register: 1,
					Data:     []byte{byte(CtStateBitESTABLISHED), 0, 0, 0, 0, 0, 0, 0},
				},
			},
			expected: "ct state != established",
		},
		{
			name: "ip version == 4",
			exprs: []expr.Any{
				&expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
					DestRegister: 1,
				},
				&expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   2,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				},
				&expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 2,
					Data:     []byte{0x40},
				},
			},
			expected: "ip version 4",
		},
		{
			name: "ip version == 6",
			exprs: []expr.Any{
				&expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
					DestRegister: 1,
				},
				&expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   2,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				},
				&expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 2,
					Data:     []byte{0x60},
				},
			},
			expected: "ip version 6",
		},
		{
			name: "meta cpu == 3",
			exprs: []expr.Any{
				&expr.Meta{Key: expr.MetaKeyCPU, Register: 1},
				&expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 1,
					Data:     []byte{3},
				},
			},
			expected: "meta cpu 3",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			rule := &exprListToRule(tc.exprs)
			str, err := NewRuleExprEncoder(rule).Format()
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, str)
		})
	}
}

func exprListToRule(exprs []expr.Any) *Rule {
	return &Rule{
		Exprs: exprs,
	}
}

func Test_CmpEncoderAdvanced(t *testing.T) {
	suite.Run(t, new(cmpEncoderAdvancedTestSuite))
}


_______________________________________________________________________________________

package encoders

import (
	"testing"

	"github.com/Morwran/nft-go/pkg/protocols"
	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
)

type cmpEncoderAdvancedTestSuite struct {
	suite.Suite
}

func (sui *cmpEncoderAdvancedTestSuite) Test_CmpEncodeIR() {
	testCases := []struct {
		name     string
		setup    func(ctx *ctx) *expr.Cmp
		expected string
	}{
		{
			name: "ct state != established",
			setup: func(ctx *ctx) *expr.Cmp {
				ct := &expr.Ct{Key: expr.CtKeySTATE, Register: 1}
				ctx.reg.Set(1, regVal{
					HumanExpr: "ct state",
					Expr:      ct,
				})
				return &expr.Cmp{
					Op:       expr.CmpOpNeq,
					Register: 1,
					Data:     []byte{byte(CtStateBitESTABLISHED), 0, 0, 0, 0, 0, 0, 0},
				}
			},
			expected: "ct state != established",
		},
		{
			name: "payload ip version != 5",
			setup: func(ctx *ctx) *expr.Cmp {
				pl := &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
					DestRegister: 1,
				}
				bw := &expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   2,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				}
				cmp := &expr.Cmp{
					Op:       expr.CmpOpNeq,
					Register: 2,
					Data:     []byte{0x50},
				}

				ctx.reg.Set(1, regVal{
					HumanExpr: "ip version",
					Expr:      pl,
				})
				ctx.reg.Set(2, regVal{
					HumanExpr: "ip version",
					Expr:      bw,
				})

				return cmp
			},
			expected: "ip version != 5",
		},

		{
			name: "ip version == 4",
			setup: func(ctx *ctx) *expr.Cmp {
				pl := &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
					DestRegister: 1,
				}
				bw := &expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   2,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				}
				cmp := &expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 2,
					Data:     []byte{0x40},
				}
				ctx.reg.Set(1, regVal{
					HumanExpr: "ip version",
					Expr:      pl,
				})
				ctx.reg.Set(2, regVal{
					HumanExpr: "ip version",
					Expr:      bw,
				})
				return cmp
			},
			expected: "ip version 4",
		},
		{
			name: "ip version == 6",
			setup: func(ctx *ctx) *expr.Cmp {
				pl := &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
					DestRegister: 1,
				}
				bw := &expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   2,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				}
				cmp := &expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 2,
					Data:     []byte{0x60},
				}
				ctx.reg.Set(1, regVal{
					HumanExpr: "ip version",
					Expr:      pl,
				})
				ctx.reg.Set(2, regVal{
					HumanExpr: "ip version",
					Expr:      bw,
				})
				return cmp
			},
			expected: "ip version 6",
		},
		{
			name: "meta cpu == 3",
			setup: func(ctx *ctx) *expr.Cmp {
				meta := &expr.Meta{Key: expr.MetaKeyCPU, Register: 1}
				ctx.reg.Set(1, regVal{
					HumanExpr: "meta cpu",
					Expr:      meta,
				})
				return &expr.Cmp{
					Op:       expr.CmpOpEq,
					Register: 1,
					Data:     []byte{3},
				}
			},
			expected: "meta cpu 3",
		},
	}

	for _, tc := range testCases {
		sui.Run(tc.name, func() {
			ctx := &ctx{
				hdr: new(protocols.ProtoDescPtr),
			}
			cmp := tc.setup(ctx)
			enc := &cmpEncoder{cmp: cmp}
			ir, err := enc.EncodeIR(ctx)
			sui.Require().NoError(err)
			sui.Require().Equal(tc.expected, ir.Format())
		})
	}
}

func Test_CmpEncoderAdvanced(t *testing.T) {
	suite.Run(t, new(cmpEncoderAdvancedTestSuite))
}










