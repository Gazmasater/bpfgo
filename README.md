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
	"fmt"
	"math/big"
	"regexp"

	rb "github.com/Morwran/nft-go/internal/bytes"
	"github.com/google/nftables/expr"
	"github.com/pkg/errors"
	"golang.org/x/sys/unix"
)

func init() {
	register(&expr.Bitwise{}, func(e expr.Any) encoder {
		return &bitwiseEncoder{bitwise: e.(*expr.Bitwise)}
	})
}

type (
	bitwiseEncoder struct {
		bitwise *expr.Bitwise
	}
)

func (b *bitwiseEncoder) EncodeIR(ctx *ctx) (irNode, error) {
	bw := b.bitwise
	if bw.DestRegister == unix.NFT_REG_VERDICT {
		return nil, fmt.Errorf("bitwise: invalid dest register %d", bw.DestRegister)
	}

	src := b.Source(ctx)
	if src.Expr == nil {
		return nil, fmt.Errorf("bitwise: source reg %d empty", bw.SourceRegister)
	}

	mask, xor, or := evalBitwise(bw.Mask, bw.Xor, int(bw.Len))

	var human string
	switch t := src.Expr.(type) {
	case *expr.Ct:
		ctBuilder := &ctEncoder{t}
		human = ctBuilder.buildCtWithMask(src.HumanExpr, bw.Mask)
	case *expr.Payload:
		plBuilder := &payloadEncoder{t}
		human = plBuilder.buildPlWithMask(ctx, bw.Mask)
	default:
		human = buildBitwiseExpr(src.HumanExpr, mask, xor, or)
	}

	ctx.reg.Set(regID(bw.DestRegister), regVal{
		HumanExpr: human,
		Len:       src.Len,
		Expr:      bw,
	})
	return nil, ErrNoIR
}

func (b *bitwiseEncoder) EncodeJSON(ctx *ctx) ([]byte, error) {
	type exprCmp struct {
		Op    string `json:"op"`
		Left  any    `json:"left"`
		Right any    `json:"right"`
	}
	bw := b.bitwise
	srcReg := b.Source(ctx)
	if srcReg.Expr == nil {
		return nil, errors.Errorf("%T expression has no left side", bw)
	}
	if bw.DestRegister == unix.NFT_REG_VERDICT {
		return nil, errors.Errorf("%T expression has invalid destination register %d", bw, bw.DestRegister)
	}

	mask, xor, or := evalBitwise(bw.Mask, bw.Xor, int(bw.Len))
	exp := srcReg.Data

	if !(srcReg.Len > 0 && scan0(mask, 0) >= srcReg.Len) {
		exp = exprCmp{Op: LogicAND.String(), Left: exp, Right: mask.Uint64()}
	}
	if xor.Uint64() != 0 {
		exp = exprCmp{Op: LogicXOR.String(), Left: exp, Right: xor.Uint64()}
	}
	if or.Uint64() != 0 {
		exp = exprCmp{Op: LogicOR.String(), Left: exp, Right: or.Uint64()}
	}

	ctx.reg.Set(regID(bw.DestRegister), regVal{
		Data: exp,
		Len:  srcReg.Len,
	})
	return nil, ErrNoJSON
}

func (b *bitwiseEncoder) buildFromCmpData(ctx *ctx, cmp *expr.Cmp) (res string) {
	src := b.Source(ctx)

	// 🎯 Обработка специального случая "ip version"
	if payload, ok := src.Expr.(*expr.Payload); ok &&
		payload.Offset == 0 && payload.Len == 1 &&
		len(b.bitwise.Mask) == 1 && b.bitwise.Mask[0] == 0xF0 {
		val := rb.RawBytes(cmp.Data).Uint64() >> 4
		return fmt.Sprintf("%d", val)
	}

	// 📌 Общий случай с контекстом заголовка
	if *ctx.hdr != nil {
		if desc, ok := (*ctx.hdr).Offsets[(*ctx.hdr).CurrentOffset]; ok {
			return desc.Desc(cmp.Data)
		}
	}

	// fallback: hex
	if rb.RawBytes(cmp.Data).Uint64() != 0 {
		return fmt.Sprintf("0x%s", rb.RawBytes(cmp.Data).Text(rb.BaseHex))
	}
	return res
}

func (b *bitwiseEncoder) Source(ctx *ctx) regVal {
	src, _ := ctx.reg.Get(regID(b.bitwise.SourceRegister))
	return src
}

func evalBitwise(maskB, xorB []byte, length int) (mask, xor, or *big.Int) {
	mask = new(big.Int).SetBytes(maskB)
	xor = new(big.Int).SetBytes(xorB)
	or = big.NewInt(0)

	if scan0(mask, 0) != length || xor.Uint64() != 0 {
		or = new(big.Int).And(mask, xor)
		or = new(big.Int).Xor(or, xor)
		xor = new(big.Int).And(xor, mask)
		mask = new(big.Int).Or(mask, or)
	}
	return
}

func buildBitwiseExpr(base string, mask, xor, or *big.Int) string {
	needPar := regexp.MustCompile(`[()&|^<> ]`).MatchString
	cur := base

	if !(scan0(mask, 0) >= len(base)) {
		if needPar(cur) {
			cur = fmt.Sprintf("(%s)", cur)
		}
		cur = fmt.Sprintf("%s & 0x%x", cur, mask)
	}
	if xor.Uint64() != 0 {
		if needPar(cur) {
			cur = fmt.Sprintf("(%s)", cur)
		}
		cur = fmt.Sprintf("%s ^ 0x%x", cur, xor)
	}
	if or.Uint64() != 0 {
		if needPar(cur) {
			cur = fmt.Sprintf("(%s)", cur)
		}
		cur = fmt.Sprintf("%s | 0x%x", cur, or)
	}
	return cur
}

const (
	LogicAND LogicOp = iota
	LogicOR
	LogicXOR
	LogicLShift
	LogicRShift
)

type (
	LogicOp    uint32
	BitwiseOps uint32
)

func (l LogicOp) String() string {
	switch l {
	case LogicAND:
		return "&"
	case LogicOR:
		return "|"
	case LogicXOR:
		return "^"
	case LogicLShift:
		return "<<"
	case LogicRShift:
		return ">>"
	}
	return ""
}

func scan0(x *big.Int, start int) int {
	for i := start; i < x.BitLen(); i++ {
		if x.Bit(i) == 0 {
			return i
		}
	}
	return -1
}




package encoders

import (
	"testing"

	"github.com/google/nftables/expr"
	"github.com/stretchr/testify/suite"
	"github.com/Morwran/nft-go/pkg/protocols"
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
				// Эмулируем ip version: offset 0, len 1, mask 0xF0, value = 0x50 (5 << 4)
				payload := &expr.Payload{
					Base:         expr.PayloadBaseNetworkHeader,
					Offset:       0,
					Len:          1,
					DestRegister: 1,
				}
				bitwise := &expr.Bitwise{
					SourceRegister: 1,
					DestRegister:   1,
					Len:            1,
					Mask:           []byte{0xF0},
					Xor:            []byte{0x00},
				}
				cmp := &expr.Cmp{
					Op:       expr.CmpOpNeq,
					Register: 1,
					Data:     []byte{0x50}, // version 5 << 4
				}

				// Записываем payload как источник
				ctx.reg.Set(1, regVal{
					HumanExpr: "ip version",
					Expr:      payload,
				})

				// Записываем bitwise как результат обработки payload
				ctx.reg.Set(1, regVal{
					HumanExpr: "ip version",
					Expr:      bitwise,
				})

				return cmp
			},
			expected: "ip version != 5",
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
				hdr: new(protocols.ProtoDescPtr), // обязательно для payload/bitwise
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



gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ go test
--- FAIL: Test_CmpEncoderAdvanced (0.00s)
    --- FAIL: Test_CmpEncoderAdvanced/Test_CmpEncodeIR (0.00s)
        --- FAIL: Test_CmpEncoderAdvanced/Test_CmpEncodeIR/payload_ip_version_!=_5 (0.00s)
            encodersCmp_test.go:103: 
                        Error Trace:    /home/gaz358/myprog/nft-go/internal/expr-encoders/encodersCmp_test.go:103
                                                                /home/gaz358/go/pkg/mod/github.com/stretchr/testify@v1.10.0/suite/suite.go:115
                        Error:          Not equal: 
                                        expected: "ip version != 5"
                                        actual  : "ip version != 0x50"
                                    
                                        Diff:
                                        --- Expected
                                        +++ Actual
                                        @@ -1 +1 @@
                                        -ip version != 5
                                        +ip version != 0x50
                        Test:           Test_CmpEncoderAdvanced/Test_CmpEncodeIR/payload_ip_version_!=_5
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
FAIL    github.com/Morwran/nft-go/internal/expr-encoders        0.012s





