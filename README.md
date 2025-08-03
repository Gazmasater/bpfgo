mx0vglWtzbBOGF34or  апи кей
77658a3144bd469fa8050b9c91b9cd4e секр кей

swag init -g cmd/server/main.go -o cmd/server/docs



./chromedriver-linux64/chromedriver --browser-binary=./chrome-linux64/chrome



package main

import (
Gazmasater
bpfgo

Code
Issues
Pull requests 2
Actions
Projects
Wiki

    bpfgo

/
in
main
	"context"
	"fmt"
	"log"
	"time"

	"github.com/chromedp/chromedp"
)

func main() {
	ctx, cancel := chromedp.NewExecAllocator(context.Background(),
		chromedp.Flag("headless", false), // запускаем с GUI
		chromedp.Flag("disable-blink-features", "AutomationControlled"),
	)
	defer cancel()

	ctx, cancel = chromedp.NewContext(ctx)
	defer cancel()

	var html string
	err := chromedp.Run(ctx,
		chromedp.Navigate("https://ozon.ru"),
		chromedp.Sleep(3*time.Second),
		chromedp.OuterHTML("html", &html),
	)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("HTML длина:", len(html))
}


 www.tractorhouse.com.














https://github.com/Morwran/nft-go/tree/main/internal/expr-encoders



https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!

__________________________________________________

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




____________________________________________________________________________________________


    go func() {
        log.Println("pprof listening on :6060")
        http.ListenAndServe("localhost:6060", nil)
    }()

http://localhost:6060/debug/pprof/

go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30

go tool pprof http://localhost:6060/debug/pprof/heap

go tool pprof http://localhost:6060/debug/pprof/goroutine


go test -run TestInMemoryRepo_Concurrency -cpuprofile=cpu.out -memprofile=mem.out
go tool pprof cpu.out


_____________________________________________________________________


go test -cover ./...

go test -coverprofile=coverage.out ./...



dockerfile

Отлично, образ собран на Ubuntu-only. Чтобы проверить, что всё работает:

Запустите контейнер:


docker run -d --name workmate_app -p 8080:8080 workmate:latest
Убедитесь, что он поднялся:


docker ps

docker logs workmate_app

http://localhost:8080/docs/index.html

curl http://localhost:8080/health


sudo docker exec -it workmate_app sh -c "ls -R /app/cmd/server/docs && head -n 20 /app/cmd/server/docs/index.html"



sudo docker rm workmate_app



sudo docker run -d \
  --name workmate_app \
  -p 8080:8080 \
  workmate:latest
Убедитесь, что он запущен:

bash
Копировать код
docker ps
Вы должны увидеть в списке workmate_app со статусом Up и пробросом портов 0.0.0.0:8080->8080/tcp.

Проверьте наличие строк в docs.go:


docker exec workmate_app grep -n 'LeftDelim\|RightDelim' /app/cmd/server/docs/docs.go || echo "Строк не найдены"
— ожидание: Строк не найдены.

Проверьте содержимое папки и index.html:


docker exec workmate_app sh -c "ls -R /app/cmd/server/docs && head -n 20 /app/cmd/server/docs/index.html"
Проверка отдачи статики изнутри контейнера:


docker exec workmate_app curl -I http://localhost:8080/docs/index.html
— должен прийти HTTP 200 и заголовки.

Внешняя проверка в браузере
Откройте

bash
Копировать код
http://localhost:8080/docs/index.html
или

arduino
Копировать код
http://127.0.0.1:8080/docs/



gaz358@gaz358-BOD-WXX9:~/myprog/workmate$ sudo docker ps
CONTAINER ID   IMAGE             COMMAND                  CREATED         STATUS         PORTS                                       NAMES
6c57122b3cff   workmate:latest   "/app/workmate --swa…"   4 seconds ago   Up 3 seconds   0.0.0.0:8080->8080/tcp, :::8080->8080/tcp   workmate_app

gaz358@gaz358-BOD-WXX9:~/myprog/workmate$ sudo docker exec workmate_app curl -I http://lo
calhost:8080/docs/index.html
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
  0    19    0     0    0     0      0      0 --:--:-- --:--:-- --:--:--     0
HTTP/1.1 404 Not Found
Content-Type: text/plain; charset=utf-8
X-Content-Type-Options: nosniff
Date: Mon, 07 Jul 2025 07:45:48 GMT
Content-Length: 19


sudo docker exec workmate_app ls -l /app/cmd/server/docs



# syntax=docker/dockerfile:1.4

FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive \
    GOPATH=/go \
    PATH="/usr/local/go/bin:${GOPATH}/bin:${PATH}" \
    GOLANG_VERSION=1.21.7

# 1) Ставим curl, git, make, ca-certificates, скачиваем Go
RUN apt-get update && \
    apt-get install -y --no-install-recommends curl git make ca-certificates && \
    curl -fsSL https://go.dev/dl/go${GOLANG_VERSION}.linux-amd64.tar.gz \
      | tar -C /usr/local -xz && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# 2) Go-модули
COPY go.mod go.sum ./
RUN go mod download

# 3) Исходники
COPY . .

# 4) Генерим спецификации и фронтенд Swagger UI
WORKDIR /app/cmd/server
RUN go install github.com/swaggo/swag/cmd/swag@latest && \
    swag init -g main.go -o docs && \
    sed -i '/LeftDelim:/d; /RightDelim:/d' docs/docs.go && \
    mkdir -p docs/swagger-ui && \
    # Скачиваем статические файлы Swagger UI
    curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui.css   -o docs/swagger-ui/swagger-ui.css && \
    curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui-bundle.js -o docs/swagger-ui/swagger-ui-bundle.js && \
    curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui-standalone-preset.js -o docs/swagger-ui/swagger-ui-standalone-preset.js && \
    # Генерируем index.html, который подключает swagger.json и UI-ассеты
    printf '<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Swagger UI</title><link rel="stylesheet" href="swagger-ui/swagger-ui.css"></head><body><div id="swagger-ui"></div><script src="swagger-ui/swagger-ui-bundle.js"></script><script src="swagger-ui/swagger-ui-standalone-preset.js"></script><script>window.onload=function(){SwaggerUIBundle({url:"swagger.json",dom_id:"#swagger-ui",presets:[SwaggerUIBundle.presets.apis,SwaggerUIStandalonePreset]});};</script></body></html>' > docs/index.html

# 5) Собираем бинарник
WORKDIR /app
RUN CGO_ENABLED=0 GOOS=linux go build -ldflags="-s -w" -o workmate cmd/server/main.go

# 6) Пробрасываем порт и точка входа
EXPOSE 8080
ENTRYPOINT ["/app/workmate", "--swagger-dir", "/app/cmd/server/docs"]


sudo docker build -t workmate:latest .
sudo docker rm -f workmate_app 
sudo docker run -d --name workmate_app -p 8080:8080 workmate:latest

http://localhost:8080/docs/index.html

___________________________________________________
sudo docker build -t workmate:latest .

sudo systemctl start docker

sudo systemctl status docker

sudo docker run -d \
  --name workmate_app \
  -p 8080:8080 \
  workmate:latest

sudo docker ps

sudo docker logs workmate_app --tail 50

http://localhost:8080/swagger/index.html


sudo docker exec -it workmate_app ls -lR /app/cmd/server

sudo docker build -t workmate:latest .
sudo docker rm -f workmate_app || true

sudo docker run -d \
  --name workmate_app \
  -p 8080:8080 \
  workmate:latest

  ___________________________________________________________________________________________

docker build -t cryptarb .
docker run -v $(pwd)/data:/app/data cryptarb

sudo apt update
sudo apt install docker-compose-plugin -y
docker compose version

sudo docker compose build --no-cache
sudo docker compose up -d


docker compose ps
sudo docker compose logs -f
docker compose down



✅ 2. Docker Compose (рекомендуется)
Создай файл docker-compose.yml:


version: "3.9"

services:
  cryptarb:
    build: .          # ← добавляем это
    image: cryptarb
    restart: always
    volumes:
      - ./data:/app/data



docker compose up -d
Контейнер будет автоматически перезапускаться при сбое или после перезагрузки машины.



sudo gpg --no-default-keyring \
  --keyring /etc/apt/keyrings/docker.gpg \
  --keyserver keyserver.ubuntu.com \
  --recv-keys 7EA0A9C3F273FCD8


curl -fsSL https://download.docker.com/linux/ubuntu/gpg \
  | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg


sudo apt update

sudo apt install docker-compose-plugin -y

_______________________________________________________________________________



package app

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

// Arbitrager ищет треугольные арбитражные возможности на бирже.
type Arbitrager struct {
	Triangles       []triangle.Triangle // Все допустимые треугольники
	latest          map[string]float64  // Последние цены по парам
	trianglesByPair map[string][]int    // Индексы треугольников по паре
	realSymbols     map[string]bool     // Доступные пары (с инверсиями)
	stepSizes       map[string]float64  // Шаг лота
	minQtys         map[string]float64  // Мин. объём
	mu              sync.Mutex
	StartAmount     float64
	exchange        exchange.Exchange
}

// New создаёт и инициализирует арбитражер.
func New(ex exchange.Exchange) (*Arbitrager, error) {
	rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()
	avail := filesystem.ExpandAvailableSymbols(rawSymbols)
	log.Printf("📊 Доступные пары (с инверсиями): %d", len(avail))

	ts, err := filesystem.LoadTrianglesFromSymbols(avail)
	if err != nil {
		return nil, fmt.Errorf("LoadTriangles: %w", err)
	}
	log.Printf("[INIT] Треугольников найдено: %d", len(ts))

	if data, err := json.MarshalIndent(ts, "", "  "); err == nil {
		_ = os.WriteFile("triangles_dump.json", data, 0644)
	}

	// Индексация по паре
	trianglesByPair := make(map[string][]int)
	var subRaw []string
	for i, tri := range ts {
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ca := tri.C + tri.A
		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)
		subRaw = append(subRaw, ab, bc, ca)
	}

	// Уникальные пары для подписки
	uniq := make(map[string]struct{})
	for _, p := range subRaw {
		if avail[p] {
			uniq[p] = struct{}{}
		}
	}
	var subPairs []string
	for p := range uniq {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] Подписка на пар: %d шт.", len(subPairs))

	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
		realSymbols:     avail,
		stepSizes:       stepSizes,
		minQtys:         minQtys,
		StartAmount:     0.5,
		exchange:        ex,
	}

	// WS подписки чанками по 20
	const maxPerConn = 20
	for i := 0; i < len(subPairs); i += maxPerConn {
		end := i + maxPerConn
		if end > len(subPairs) {
			end = len(subPairs)
		}
		chunk := subPairs[i:end]
		go func(pairs []string) {
			for {
				err := ex.SubscribeDeals(pairs, arb.HandleRaw)
				if err != nil {
					log.Printf("[WS][%s] subscribe error: %v, retrying...", ex.Name(), err)
					time.Sleep(time.Second)
					continue
				}
				log.Printf("[WS][%s] subscribed to channels: %v", ex.Name(), pairs)
				return
			}
		}(chunk)
	}

	return arb, nil
}

// normalizeSymbolDir собирает правильный символ и указывает, нужно ли инвертировать цену.
func (a *Arbitrager) normalizeSymbolDir(base, quote string) (symbol string, ok bool, invert bool) {
	if a.realSymbols[base+quote] {
		return base + quote, true, false
	}
	if a.realSymbols[quote+base] {
		return quote + base, true, true
	}
	return "", false, false
}

// HandleRaw обрабатывает каждое WS-сообщение.
func (a *Arbitrager) HandleRaw(_exchange string, raw []byte) {
	// Обработка ACK сообщений подписки
	var ack struct {
		ID   int64  `json:"id"`
		Code int    `json:"code"`
		Msg  string `json:"msg"`
	}
	if err := json.Unmarshal(raw, &ack); err == nil && ack.Code == 0 {
		const prefixFail = "Not Subscribed successfully! ["
		if parts := strings.Split(ack.Msg, prefixFail); len(parts) == 2 {
			blocked := strings.Split(strings.TrimSuffix(parts[1], "].  Reason： Blocked! \""), ",")
			for _, ch := range blocked {
				if idx := strings.LastIndex(ch, "@"); idx != -1 {
					sym := ch[idx+1:]
					a.mu.Lock()
					a.realSymbols[sym] = false
					a.mu.Unlock()
				}
			}
			return
		}
	}

	// Debug: выводим сырое сообщение
	log.Printf("HandleRaw raw: %s", raw)

	// Структура для разбора формата MEXC public deals
	var msg struct {
		Channel string `json:"c"`
		Symbol  string `json:"s"`
		Data    struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("unmarshal WS error: %v, raw=%s", err, raw)
		return
	}
	if msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		log.Printf("HandleRaw skipped: no symbol or deals empty (symbol=%q, deals=%d)", msg.Symbol, len(msg.Data.Deals))
		return
	}

	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		log.Printf("parse price error: %v, priceStr=%v", err, msg.Data.Deals[0].Price)
		return
	}
	log.Printf("HandleRaw parsed: symbol=%s price=%.8f", msg.Symbol, price)

	a.mu.Lock()
	a.latest[msg.Symbol] = price
	a.mu.Unlock()

	a.Check(msg.Symbol)
}

// Check проверяет все треугольники, связанные с символом.
func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	indices := a.trianglesByPair[symbol]
	priceMap := a.latest
	a.mu.Unlock()

	if len(indices) == 0 {
		return
	}

	// С учётом торговых комиссий
	nf := 0.9965 * 0.9965 * 0.9965

	for _, idx := range indices {
		tri := a.Triangles[idx]

		ab, ok1, rev1 := a.normalizeSymbolDir(tri.A, tri.B)
		bc, ok2, rev2 := a.normalizeSymbolDir(tri.B, tri.C)
		ca, ok3, rev3 := a.normalizeSymbolDir(tri.C, tri.A)
		if !ok1 || !ok2 || !ok3 {
			continue
		}

		p1, ex1 := priceMap[ab]
		p2, ex2 := priceMap[bc]
		p3, ex3 := priceMap[ca]
		if !ex1 || !ex2 || !ex3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		if rev1 {
			p1 = 1 / p1
		}
		if rev2 {
			p2 = 1 / p2
		}
		if rev3 {
			p3 = 1 / p3
		}

		profit := (p1 * p2 * p3 * nf - 1) * 100
		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
	}
}




2025/08/03 22:05:17 HandleRaw parsed: symbol=MXUSDT price=2.21530000
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHUSDC price=542.14000000
2025/08/03 22:05:17 🔺 ARB BCH/USDC/USDT profit=-0.8844%
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BCHBTC","d":{"deals":[{"p":"0.004730","v":"0.002","S":2,"t":1754247917258}],"e":"spot@public.deals.v3.api"},"s":"BCHBTC","t":1754247917262}
2025/08/03 22:05:17 🔺 ARB USDT/BCH/USDC profit=-0.8844%
2025/08/03 22:05:17 🔺 ARB USDC/USDT/BCH profit=-0.8844%
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHBTC price=0.00473000
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BCHUSDC","d":{"deals":[{"p":"542.09","v":"0.01","S":2,"t":1754247917225}],"e":"spot@public.deals.v3.api"},"s":"BCHUSDC","t":1754247917229}
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BCHBTC","d":{"deals":[{"p":"0.004729","v":"0.005","S":2,"t":1754247917258}],"e":"spot@public.deals.v3.api"},"s":"BCHBTC","t":1754247917262}
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHUSDC price=542.09000000
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHBTC price=0.00472900
2025/08/03 22:05:17 🔺 ARB BCH/USDC/USDT profit=-0.8935%
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BCHBTC","d":{"deals":[{"p":"0.004730","v":"0.004","S":1,"t":1754247917259}],"e":"spot@public.deals.v3.api"},"s":"BCHBTC","t":1754247917262}
2025/08/03 22:05:17 🔺 ARB USDT/BCH/USDC profit=-0.8935%
2025/08/03 22:05:17 🔺 ARB USDC/USDT/BCH profit=-0.8935%
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHBTC price=0.00473000
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BTCUSDC","d":{"deals":[{"p":"114372.72","v":"0.000050","S":2,"t":1754247917291}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDC","t":1754247917295}
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BCHBTC","d":{"deals":[{"p":"0.004730","v":"0.002","S":1,"t":1754247917259}],"e":"spot@public.deals.v3.api"},"s":"BCHBTC","t":1754247917263}
2025/08/03 22:05:17 HandleRaw parsed: symbol=BTCUSDC price=114372.72000000
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHBTC price=0.00473000
fatal error: 2025/08/03 22:05:17 🔺 ARB BCH/BTC/USDC profit=-1.2484%
concurrent map read and map write
2025/08/03 22:05:17 🔺 ARB USDC/BCH/BTC profit=-1.2484%
2025/08/03 22:05:17 🔺 ARB BTC/USDC/BCH profit=-1.2484%
2025/08/03 22:05:17 HandleRaw raw: {"c":"spot@public.deals.v3.api@BCHBTC","d":{"deals":[{"p":"0.004730","v":"0.002","S":2,"t":1754247917259}],"e":"spot@public.deals.v3.api"},"s":"BCHBTC","t":1754247917263}
2025/08/03 22:05:17 HandleRaw parsed: symbol=BCHBTC price=0.00473000
2025/08/03 22:05:17 🔺 ARB BCH/BTC/USDC profit=-1.2484%
2025/08/03 22:05:17 🔺 ARB USDC/BCH/BTC profit=-1.2484%
2025/08/03 22:05:17 🔺 ARB BTC/USDC/BCH profit=-1.2484%

goroutine 34 [running]:
internal/runtime/maps.fatal({0x773b0f?, 0xc0007abac3?})
        /usr/local/go/src/runtime/panic.go:1053 +0x18
cryptarb/internal/app.(*Arbitrager).Check(0xc0001aa1e0, {0xc000d942f8?, 0xc000d942f8?})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:204 +0x669
cryptarb/internal/app.(*Arbitrager).HandleRaw(0xc0001aa1e0, {0xc0007abe00?, 0x65853c?}, {0xc00018a800, 0xb0, 0x200})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:177 +0x6f2
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002dbb88, 0x14, 0x0?}, 0xc000782090)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:141 +0x4c3
cryptarb/internal/app.New.func1({0xc0002dbb88, 0x14, 0x107})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 1 [select (no cases)]:
main.main()
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:37 +0x175

goroutine 99 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 34
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 15 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7ce30, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000b18880?, 0xc000e98000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000b18880, {0xc000e98000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000b18880, {0xc000e98000?, 0x300000002?, 0xc0000358b0?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000d86000, {0xc000e98000?, 0x7bc1934cf580?, 0x7bc1d9fc0108?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000b1e540, {0xc000e98000?, 0xc0000358f8?, 0x46ebf2?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000d8c2b8, {0x7e38a0, 0xc000b1e540})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000d8c008, {0x7e39c0, 0xc000d86000}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000d8c008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000d8c008, {0xc000cfd000, 0x1000, 0xc0007828f0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc0002b4a20)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc0002b4a20, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000796580, 0x5e43e5?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000796580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000796580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x767b34?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002db908, 0x14, 0x0?}, 0xc000c02000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:137 +0x4d0
cryptarb/internal/app.New.func1({0xc0002db908, 0x14, 0x12f})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 16 [runnable]:
syscall.Syscall(0x1, 0x2, 0xc00002e1a0, 0x3a)
        /usr/local/go/src/syscall/syscall_linux.go:73 +0x25
syscall.write(0xc00008a180?, {0xc00002e1a0?, 0x4?, 0x8?})
        /usr/local/go/src/syscall/zsyscall_linux_amd64.go:964 +0x3b
syscall.Write(...)
        /usr/local/go/src/syscall/syscall_unix.go:211
internal/poll.ignoringEINTRIO(...)
        /usr/local/go/src/internal/poll/fd_unix.go:745
internal/poll.(*FD).Write(0xc00008a180, {0xc00002e1a0, 0x3a, 0xd0})
        /usr/local/go/src/internal/poll/fd_unix.go:381 +0x394
os.(*File).write(...)
        /usr/local/go/src/os/file_posix.go:46
os.(*File).Write(0xc000062040, {0xc00002e1a0?, 0x3a, 0xc000b4f800?})
        /usr/local/go/src/os/file.go:195 +0x4e
log.(*Logger).output(0xc0000a21b0, 0x0, 0x2, 0xc000b4fbe0)
        /usr/local/go/src/log/log.go:245 +0x45a
log.Printf(...)
        /usr/local/go/src/log/log.go:397
cryptarb/internal/app.(*Arbitrager).Check(0xc0001aa1e0, {0xc000b02400?, 0xc000b02400?})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:222 +0x91a
cryptarb/internal/app.(*Arbitrager).HandleRaw(0xc0001aa1e0, {0xc000b4fe00?, 0x65853c?}, {0xc00012be00, 0xaa, 0x200})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:177 +0x6f2
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002dba48, 0x14, 0x0?}, 0xc000c00000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:141 +0x4c3
cryptarb/internal/app.New.func1({0xc0002dba48, 0x14, 0x11b})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 13 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7d5d8, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0000c6400?, 0xc0002bc000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc0000c6400, {0xc0002bc000, 0xa000, 0xa000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc0000c6400, {0xc0002bc000?, 0xc0002be9b3?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc00012e008, {0xc0002bc000?, 0x7bc1d9e3a138?, 0x7bc1d9fc05c0?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc0003360c0, {0xc0002bc000?, 0x7648?, 0xc0000f59b8?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0001362b8, {0x7e38a0, 0xc0003360c0})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000136008, {0x7e39c0, 0xc00012e008}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000136008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000136008, {0xc00016d000, 0x1000, 0xc0000e01c0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).Read(0xc0001aaf60, {0xc00019d380, 0x9, 0xa02450?})
        /usr/local/go/src/bufio/bufio.go:245 +0x197
io.ReadAtLeast({0x7e3140, 0xc0001aaf60}, {0xc00019d380, 0x9, 0x9}, 0x9)
        /usr/local/go/src/io/io.go:335 +0x91
io.ReadFull(...)
        /usr/local/go/src/io/io.go:354
net/http.http2readFrameHeader({0xc00019d380, 0x9, 0xc0002e6180?}, {0x7e3140?, 0xc0001aaf60?})
        /usr/local/go/src/net/http/h2_bundle.go:1805 +0x65
net/http.(*http2Framer).ReadFrame(0xc00019d340)
        /usr/local/go/src/net/http/h2_bundle.go:2072 +0x7d
net/http.(*http2clientConnReadLoop).run(0xc0000f5fa8)
        /usr/local/go/src/net/http/h2_bundle.go:9933 +0xda
net/http.(*http2ClientConn).readLoop(0xc000503a40)
        /usr/local/go/src/net/http/h2_bundle.go:9812 +0x79
created by net/http.(*http2Transport).newClientConn in goroutine 12
        /usr/local/go/src/net/http/h2_bundle.go:8334 +0xde5

goroutine 35 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7d3a8, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000b18380?, 0xc0000e8000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000b18380, {0xc0000e8000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000b18380, {0xc0000e8000?, 0x300000002?, 0xc0007a78b0?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000811518, {0xc0000e8000?, 0x7bc192ac3488?, 0x7bc1d9fc0f30?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000bb4480, {0xc0000e8000?, 0xc0007a7900?, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000137b38, {0x7e38a0, 0xc000bb4480})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000137888, {0x7e39c0, 0xc000811518}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000137888, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000137888, {0xc000cfc000, 0x1000, 0xc0000c41a0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000111da0)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000111da0, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000c926e0, 0x5e43e5?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000c926e0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000c926e0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x767b34?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002dbcc8, 0x14, 0x0?}, 0xc000782000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:137 +0x4d0
cryptarb/internal/app.New.func1({0xc0002dbcc8, 0x14, 0xf3})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 36 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7cf48, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000b18780?, 0xc000b56000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000b18780, {0xc000b56000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000b18780, {0xc000b56000?, 0x300000002?, 0xc0007b38b0?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000baa000, {0xc000b56000?, 0x7bc192ac3488?, 0x7bc1d9fc0f30?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000bb44b0, {0xc000b56000?, 0xc0007b3900?, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000bae2b8, {0x7e38a0, 0xc000bb44b0})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000bae008, {0x7e39c0, 0xc000baa000}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000bae008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000bae008, {0xc000b57000, 0x1000, 0xc0000c41a0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000b78420)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000b78420, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000b0c580, 0x5e43e5?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000b0c580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000b0c580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x767b34?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002dbe08, 0x14, 0x0?}, 0xc000c00090)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:137 +0x4d0
cryptarb/internal/app.New.func1({0xc0002dbe08, 0x14, 0xdf})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 37 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7d4c0, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000b18200?, 0xc00016c000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000b18200, {0xc00016c000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000b18200, {0xc00016c000?, 0x300000002?, 0xc000b3b8b0?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc0000635b8, {0xc00016c000?, 0x7bc192ac3488?, 0x7bc1d9fc0f30?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000bb4450, {0xc00016c000?, 0xc000b3b900?, 0x1?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000b2c2b8, {0x7e38a0, 0xc000bb4450})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000b2c008, {0x7e39c0, 0xc0000635b8}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000b2c008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000b2c008, {0xc000d14000, 0x1000, 0xc0000c4268?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000e865a0)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000e865a0, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000c92580, 0x5e43e5?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000c92580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000c92580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x767b34?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0xc0000617a0?, {0xc0002dbf48, 0x14, 0xc000061730?}, 0xc000b00000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:137 +0x4d0
cryptarb/internal/app.New.func1({0xc0002dbf48, 0x14, 0xcb})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 38 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7d060, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000b18680?, 0xc000bda000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000b18680, {0xc000bda000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000b18680, {0xc000bda000?, 0x300000002?, 0xc0007af8b0?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000c1e008, {0xc000bda000?, 0x7bc192ac3488?, 0x7bc1d9fc0f30?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000bb44e0, {0xc000bda000?, 0xc0007af900?, 0x1?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000c20638, {0x7e38a0, 0xc000bb44e0})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000c20388, {0x7e39c0, 0xc000c1e008}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000c20388, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000c20388, {0xc000bdb000, 0x1000, 0xc0000c4268?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000bfe0c0)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000bfe0c0, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000b94580, 0x5e43e5?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000b94580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000b94580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x767b34?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002dc088, 0x14, 0x0?}, 0xc000b80000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:137 +0x4d0
cryptarb/internal/app.New.func1({0xc0002dc088, 0x14, 0xb7})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 39 [IO wait]:
internal/poll.runtime_pollWait(0x7bc1d9e7d6f0, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0000c6680?, 0xc000e07000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc0000c6680, {0xc000e07000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc0000c6680, {0xc000e07000?, 0xc000e07000?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000c1e000, {0xc000e07000?, 0x7bc192ac3488?, 0x7bc1d9fc0f30?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000bb4408, {0xc000e07000?, 0xffb?, 0xc000065008?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000c202b8, {0x7e38a0, 0xc000bb4408})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000c20008, {0x7e39c0, 0xc000c1e000}, 0x43a2b4?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000c20008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000c20008, {0xc000144000, 0x1000, 0xc0008c4968?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000bfe660)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000bfe660, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000b94420, 0xc0001aa220?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000b94420)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000b94420)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x767b34?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0002dc1c8, 0x4, 0x0?}, 0xc0008ecc30)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:137 +0x4d0
cryptarb/internal/app.New.func1({0xc0002dc1c8, 0x4, 0xa3})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:94 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:92 +0xa9e

goroutine 130 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 39
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 53 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 16
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 83 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 35
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 54 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 36
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 84 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 15
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 131 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 37
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c

goroutine 55 [chan receive]:
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals.func2()
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:128 +0x95
created by cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals in goroutine 38
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:125 +0x46c
exit status 2
gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ 


