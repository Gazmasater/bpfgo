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
	// Debug: выводим сырое сообщение
	log.Printf("HandleRaw raw: %s", raw)

	// Структура согласно MEXC протобуферному каналу publicdeals
	var msg struct {
		Channel     string `json:"channel"`
		Symbol      string `json:"symbol"`
		PublicDeals struct {
			DealsList []struct {
				Price string `json:"price"`
			} `json:"dealsList"`
		} `json:"publicdeals"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("unmarshal WS error: %v, raw=%s", err, raw)
		return
	}
	// Игнорируем системные/пустые сообщения
	if msg.Symbol == "" || len(msg.PublicDeals.DealsList) == 0 {
		log.Printf("HandleRaw skipped: no symbol or dealsList empty")
		return
	}

	// Парсим цену первой сделки
	price, err := strconv.ParseFloat(msg.PublicDeals.DealsList[0].Price, 64)
	if err != nil {
		log.Printf("parse price error: %v, priceStr=%v", err, msg.PublicDeals.DealsList[0].Price)
		return
	}
	log.Printf("HandleRaw parsed: symbol=%s price=%.8f", msg.Symbol, price)

	// Сохраняем цену и запускаем проверку
	a.mu.Lock()
	a.latest[msg.Symbol] = price
	a.mu.Unlock()
	// Проверяем треугольники для этого символа
	a.Check(msg.Symbol)
}

// Check проверяет все треугольники, связанные с символом.
func (a *Arbitrager) Check(symbol string) {
	// Считываем под замком
	a.mu.Lock()
	indices := a.trianglesByPair[symbol]
	priceMap := a.latest
	a.mu.Unlock()
	log.Printf("Check start: symbol=%s indices=%v", symbol, indices)

	if len(indices) == 0 {
		return
	}

	nf := 0.9965 * 0.9965 * 0.9965
	for _, idx := range indices {
		tri := a.Triangles[idx]

		// символы и флаги инверсии
		ab, ok1, rev1 := a.normalizeSymbolDir(tri.A, tri.B)
		bc, ok2, rev2 := a.normalizeSymbolDir(tri.B, tri.C)
		ca, ok3, rev3 := a.normalizeSymbolDir(tri.C, tri.A)
		log.Printf("Triangle check: %s/%s/%s -> %s(rev=%v), %s(rev=%v), %s(rev=%v)",
			tri.A, tri.B, tri.C, ab, rev1, bc, rev2, ca, rev3)
		if !ok1 || !ok2 || !ok3 {
			continue
		}

		// получаем цены
		p1, ex1 := priceMap[ab]
		p2, ex2 := priceMap[bc]
		p3, ex3 := priceMap[ca]
		log.Printf("Prices raw: %s=%.8f(ok=%v), %s=%.8f(ok=%v), %s=%.8f(ok=%v)",
			ab, p1, ex1, bc, p2, ex2, ca, p3, ex3)
		if !ex1 || !ex2 || !ex3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		// применяем инверсию
		if rev1 { p1 = 1 / p1 }
		if rev2 { p2 = 1 / p2 }
		if rev3 { p3 = 1 / p3 }
		log.Printf("Prices inv: p1=%.8f, p2=%.8f, p3=%.8f", p1, p2, p3)

		// рассчитываем и выводим прибыль
		profit := (p1 * p2 * p3 * nf - 1) * 100
		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
	}
}


gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/08/03 21:20:14 ✅ Подходящих пар: 557
2025/08/03 21:20:14 📊 Доступные пары (с инверсиями): 1114
2025/08/03 21:20:14 [TRIANGLE] Found 234 triangles
2025/08/03 21:20:14 [INIT] Треугольников найдено: 234
2025/08/03 21:20:14 [INIT] Подписка на пар: 144 шт.
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@APEUSDT,spot@public.deals.v3.api@XRPUSDT,spot@public.deals.v3.api@FTTUSDC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDCMX].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@SOLBTC,spot@public.deals.v3.api@XENUSDT,spot@public.deals.v3.api@CAWUSDT,spot@public.deals.v3.api@WBTCUSDT,spot@public.deals.v3.api@SHIBUSDC,spot@public.deals.v3.api@MINAUSDT]. Not Subscribed successfully! [spot@public.deals.v3.api@USDTADA,spot@public.deals.v3.api@USDTNEAR,spot@public.deals.v3.api@BTCBCH,spot@public.deals.v3.api@USDTCAW,spot@public.deals.v3.api@USDTFTT,spot@public.deals.v3.api@USDCSOL,spot@public.deals.v3.api@USDTTRX,spot@public.deals.v3.api@USDCBTC,spot@public.deals.v3.api@USDTWAVES,spot@public.deals.v3.api@USDCWBTC,spot@public.deals.v3.api@USDTWBTC,spot@public.deals.v3.api@BTCXRP,spot@public.deals.v3.api@USDTRAY,spot@public.deals.v3.api@USDTALGO].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@JASMYUSDC,spot@public.deals.v3.api@UNIETH,spot@public.deals.v3.api@RAYUSDT,spot@public.deals.v3.api@TRXUSDC,spot@public.deals.v3.api@MINAUSDC,spot@public.deals.v3.api@BCHUSDT,spot@public.deals.v3.api@AVAXUSDT,spot@public.deals.v3.api@TRXUSDT,spot@public.deals.v3.api@ATOMUSDT,spot@public.deals.v3.api@NEARUSDT,spot@public.deals.v3.api@ADAUSDC,spot@public.deals.v3.api@AZEROUSDT,spot@public.deals.v3.api@ALGOUSDT,spot@public.deals.v3.api@ENSUSDT]. Not Subscribed successfully! [spot@public.deals.v3.api@USDCNEAR,spot@public.deals.v3.api@USDCRAY,spot@public.deals.v3.api@USDCADA,spot@public.deals.v3.api@USDTDOGE,spot@public.deals.v3.api@USDTLTC,spot@public.deals.v3.api@USDCWAVES].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@MXUSDC,spot@public.deals.v3.api@UNIUSDC,spot@public.deals.v3.api@UNIUSDT,spot@public.deals.v3.api@LUNCUSDT,spot@public.deals.v3.api@LTCUSDT,spot@public.deals.v3.api@ETHBTC,spot@public.deals.v3.api@AAVEUSDT,spot@public.deals.v3.api@TRXBTC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDCBNB,spot@public.deals.v3.api@USDTAZERO,spot@public.deals.v3.api@USDTSOL,spot@public.deals.v3.api@USDTXEN,spot@public.deals.v3.api@USDCFIL,spot@public.deals.v3.api@USDCENS,spot@public.deals.v3.api@USDTAPE,spot@public.deals.v3.api@USDTMX,spot@public.deals.v3.api@USDTFIL,spot@public.deals.v3.api@USDTMINA,spot@public.deals.v3.api@USDCSHIB,spot@public.deals.v3.api@USDTSHIB].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@MXUSDT,spot@public.deals.v3.api@USDCUSDT,spot@public.deals.v3.api@FILUSDT,spot@public.deals.v3.api@AVAXUSDC,spot@public.deals.v3.api@LUNCUSDC,spot@public.deals.v3.api@ADAUSDT,spot@public.deals.v3.api@ALGOUSDC,spot@public.deals.v3.api@APEUSDC,spot@public.deals.v3.api@LTCBTC,spot@public.deals.v3.api@BNBUSDT,spot@public.deals.v3.api@BTCUSDC,spot@public.deals.v3.api@OPUSDC,spot@public.deals.v3.api@BCHBTC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDTAAVE,spot@public.deals.v3.api@BTCATOM,spot@public.deals.v3.api@USDCDOGE,spot@public.deals.v3.api@USDCLUNC,spot@public.deals.v3.api@USDCMINA,spot@public.deals.v3.api@BTCETH,spot@public.deals.v3.api@ETHUNI].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw raw: {"c":"spot@public.deals.v3.api@USDCUSDT","d":{"deals":[{"p":"1.0000","v":"23.83","S":1,"t":1754245215800}],"e":"spot@public.deals.v3.api"},"s":"USDCUSDT","t":1754245215802}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@XRPUSDC,spot@public.deals.v3.api@JASMYUSDT,spot@public.deals.v3.api@WBTCUSDC,spot@public.deals.v3.api@LTCUSDC,spot@public.deals.v3.api@RAYUSDC,spot@public.deals.v3.api@FTTUSDT,spot@public.deals.v3.api@NEARUSDC,spot@public.deals.v3.api@XRPBTC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDCAPE,spot@public.deals.v3.api@USDTOP,spot@public.deals.v3.api@USDCAZERO,spot@public.deals.v3.api@USDCETH,spot@public.deals.v3.api@USDCAVAX,spot@public.deals.v3.api@USDTAVAX,spot@public.deals.v3.api@USDTBCH,spot@public.deals.v3.api@USDTBNB,spot@public.deals.v3.api@USDCUNI,spot@public.deals.v3.api@USDTUSDC,spot@public.deals.v3.api@BTCADA,spot@public.deals.v3.api@USDCALGO].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@CAWUSDC,spot@public.deals.v3.api@XENUSDC,spot@public.deals.v3.api@BCHUSDC,spot@public.deals.v3.api@SOLUSDT,spot@public.deals.v3.api@AAVEUSDC,spot@public.deals.v3.api@ENSUSDC,spot@public.deals.v3.api@DOGEUSDT,spot@public.deals.v3.api@AZEROUSDC,spot@public.deals.v3.api@ATOMBTC,spot@public.deals.v3.api@ETHUSDC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDCATOM,spot@public.deals.v3.api@USDCBCH,spot@public.deals.v3.api@BTCSOL,spot@public.deals.v3.api@USDCAAVE,spot@public.deals.v3.api@USDCJASMY,spot@public.deals.v3.api@USDTXRP,spot@public.deals.v3.api@BTCLTC,spot@public.deals.v3.api@BTCTRX,spot@public.deals.v3.api@USDTENS,spot@public.deals.v3.api@USDTLUNC].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:15 HandleRaw raw: {"id":1754245215,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@DOGEUSDC,spot@public.deals.v3.api@OPUSDT,spot@public.deals.v3.api@SHIBUSDT,spot@public.deals.v3.api@ATOMUSDC,spot@public.deals.v3.api@FILUSDC,spot@public.deals.v3.api@WAVESUSDT,spot@public.deals.v3.api@WAVESUSDC,spot@public.deals.v3.api@BNBUSDC,spot@public.deals.v3.api@ADABTC,spot@public.deals.v3.api@SOLUSDC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDCFTT,spot@public.deals.v3.api@USDCXRP,spot@public.deals.v3.api@USDTUNI,spot@public.deals.v3.api@USDCCAW,spot@public.deals.v3.api@USDCTRX,spot@public.deals.v3.api@USDCLTC,spot@public.deals.v3.api@USDTJASMY,spot@public.deals.v3.api@USDCXEN,spot@public.deals.v3.api@USDCOP,spot@public.deals.v3.api@USDTATOM].  Reason： Blocked! "}
2025/08/03 21:20:15 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@LTCUSDT","d":{"deals":[{"p":"108.71","v":"0.2650","S":2,"t":1754245215942}],"e":"spot@public.deals.v3.api"},"s":"LTCUSDT","t":1754245215945}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@LTCUSDT","d":{"deals":[{"p":"108.71","v":"0.2337","S":2,"t":1754245215942}],"e":"spot@public.deals.v3.api"},"s":"LTCUSDT","t":1754245215945}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@LTCUSDT","d":{"deals":[{"p":"108.71","v":"0.0945","S":2,"t":1754245215943}],"e":"spot@public.deals.v3.api"},"s":"LTCUSDT","t":1754245215945}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"161.57","v":"0.119","S":1,"t":1754245216096}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1754245216098}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@RAYUSDT","d":{"deals":[{"p":"2.557","v":"3.79","S":2,"t":1754245216141}],"e":"spot@public.deals.v3.api"},"s":"RAYUSDT","t":1754245216143}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@RAYUSDT","d":{"deals":[{"p":"2.557","v":"4.62","S":2,"t":1754245216141}],"e":"spot@public.deals.v3.api"},"s":"RAYUSDT","t":1754245216143}
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@USDCUSDT","d":{"deals":[{"p":"0.9999","v":"9.17","S":2,"t":1754245216114}],"e":"spot@public.deals.v3.api"},"s":"USDCUSDT","t":1754245216127}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@RAYUSDT","d":{"deals":[{"p":"2.557","v":"5.20","S":2,"t":1754245216142}],"e":"spot@public.deals.v3.api"},"s":"RAYUSDT","t":1754245216144}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@LTCBTC","d":{"deals":[{"p":"0.0009526","v":"0.012","S":1,"t":1754245216253}],"e":"spot@public.deals.v3.api"},"s":"LTCBTC","t":1754245216257}
2025/08/03 21:20:16 HandleRaw skipped: no symbol or dealsList empty
2025/08/03 21:20:16 HandleRaw raw: {"c":"spot@public.deals.v3.api@LTCBTC","d":{"deals":[{"p":"0.0009526","v":"0.025","S":2,"t":1754245216254}],"e":"spot@public.deals.v3.api"},"s":"LTCBTC","t":1754245216257}
