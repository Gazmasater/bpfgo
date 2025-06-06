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
	"encoding/binary"
	"fmt"
	"sort"
	"strings"

	"github.com/google/nftables/expr"
)

func init() {
	register(&expr.Ct{}, func(e expr.Any) encoder {
		return &ctEncoder{ct: e.(*expr.Ct)}
	})
}

type ctEncoder struct {
	ct *expr.Ct
}

func (b *ctEncoder) EncodeIR(ctx *ctx) (irNode, error) {
	ct := b.ct
	if ct.SourceRegister {
		// handle set expression (e.g., ct mark set ...)
		src, ok := ctx.reg.Get(regID(ct.Register))
		if !ok {
			return newIRNodef("ct %s set *", ctKeyStr(ct.Key)), nil
		}
		if imm, ok := src.Expr.(*expr.Immediate); ok && len(imm.Data) >= 4 {
			val := binary.LittleEndian.Uint32(imm.Data)
			return newIRNodef("ct %s set %d", ctKeyStr(ct.Key), val), nil
		}
		return newIRNodef("ct %s set *", ctKeyStr(ct.Key)), nil
	}
	ctx.reg.Set(regID(ct.Register), regVal{
		HumanExpr: fmt.Sprintf("ct %s", ctKeyStr(ct.Key)),
		Expr:      ct,
		Len:       4,
	})
	return nil, ErrNoIR
}

func (b *ctEncoder) EncodeJSON(ctx *ctx) ([]byte, error) {
	return nil, ErrNoJSON
}

func ctKeyStr(k expr.CtKey) string {
	switch k {
	case expr.CtKeySTATE:
		return "state"
	case expr.CtKeyDIRECTION:
		return "direction"
	case expr.CtKeySTATUS:
		return "status"
	case expr.CtKeyEXPIRATION:
		return "expiration"
	case expr.CtKeyMARK:
		return "mark"
	case expr.CtKeyPROTOCOL:
		return "protocol"
	case expr.CtKeyL3PROTOCOL:
		return "l3proto"
	}
	return fmt.Sprintf("key(%d)", k)
}

func ctStatusFlags(data []byte) string {
	raw := binary.LittleEndian.Uint32(data)
	var flags []string
	if raw&nftCtStatusAssured != 0 {
		flags = append(flags, "assured")
	}
	if raw&nftCtStatusConfirmed != 0 {
		flags = append(flags, "confirmed")
	}
	if raw&nftCtStatusRelated != 0 {
		flags = append(flags, "related")
	}
	if raw&nftCtStatusSrcNAT != 0 {
		flags = append(flags, "snat")
	}
	if raw&nftCtStatusDstNAT != 0 {
		flags = append(flags, "dnat")
	}
	sort.Strings(flags)
	return strings.Join(flags, ",")
}

const (
	nftCtStatusExpected  = 1 << 0
	nftCtStatusSeenReply = 1 << 1
	nftCtStatusAssured   = 1 << 2
	nftCtStatusConfirmed = 1 << 3
	nftCtStatusRelated   = 1 << 4
	nftCtStatusSrcNAT    = 1 << 5
	nftCtStatusDstNAT    = 1 << 6
	nftCtStatusDYING     = 1 << 7
)




