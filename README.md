
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



http://localhost:6060/debug/pprof/

go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30

go tool pprof http://localhost:6060/debug/pprof/heap
(pprof) top


(pprof) list FetchAvailableSymbols

go tool pprof http://localhost:6060/debug/pprof/heap?debug=1
(pprof) top --alloc_space
(pprof) top --inuse_objects


go tool pprof http://localhost:6060/debug/pprof/goroutine
(pprof) top


import _ "net/http/pprof"


go func() {
        log.Println("🚀 pprof доступен на http://localhost:6060/debug/pprof/")
        log.Println(http.ListenAndServe("localhost:6060", nil))
    }()



func init() {
    // Собирать каждый блокирующий вызов
    runtime.SetBlockProfileRate(1)
}

go tool pprof --text --focus="cryptarb" --ignore="runtime\..*" cpu.prof





__________________________________________________________________________________


https://github.com/mexcdevelop/websocket-proto


sudo apt install -y protobuf-compiler
go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

protoc --go_out=. --go_opt=paths=source_relative PrivateDealsV3Api.proto

syntax = "proto3";
option go_package = "crypt_proto/pb";



package mexc

import (
	"bytes"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"strconv"
	"strings"
	"sync/atomic"
	"time"

	"github.com/gorilla/websocket"
)

// ---- Тип и конструктор ----

type MEXCExchange struct{}

func NewMEXCExchange() *MEXCExchange { return &MEXCExchange{} }
func (m *MEXCExchange) Name() string { return "MEXC" }

// ---- Справочник доступных спот-символов ----

func (m *MEXCExchange) FetchAvailableSymbols() (map[string]bool, map[string]float64, map[string]float64) {
	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		log.Printf("❌ MEXC exchangeInfo: %v", err)
		return nil, nil, nil
	}
	defer resp.Body.Close()

	var data struct {
		Symbols []struct {
			Symbol               string   `json:"symbol"`
			Status               string   `json:"status"`
			IsSpotTradingAllowed bool     `json:"isSpotTradingAllowed"`
			OrderTypes           []string `json:"orderTypes"`
			BaseSizePrecision    string   `json:"baseSizePrecision"`
			Filters              []struct {
				FilterType string `json:"filterType"`
				MinQty     string `json:"minQty"`
				StepSize   string `json:"stepSize"`
			} `json:"filters"`
		} `json:"symbols"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		log.Printf("❌ MEXC decode exchangeInfo: %v", err)
		return nil, nil, nil
	}

	hasMarket := func(ot []string) bool {
		for _, t := range ot {
			if strings.EqualFold(t, "MARKET") {
				return true
			}
		}
		return false
	}

	avail := make(map[string]bool, len(data.Symbols))
	step := make(map[string]float64, len(data.Symbols))
	min := make(map[string]float64, len(data.Symbols))

	for _, s := range data.Symbols {
		statusOK := s.Status == "1" || strings.EqualFold(s.Status, "ENABLED") || strings.EqualFold(s.Status, "TRADING")
		if !statusOK || !s.IsSpotTradingAllowed || !hasMarket(s.OrderTypes) {
			continue
		}
		var stepSz, minQty float64
		for _, f := range s.Filters {
			if strings.EqualFold(f.FilterType, "LOT_SIZE") {
				if v, err := strconv.ParseFloat(f.StepSize, 64); err == nil && v > 0 {
					stepSz = v
				}
				if v, err := strconv.ParseFloat(f.MinQty, 64); err == nil && v > 0 {
					minQty = v
				}
			}
		}
		if stepSz == 0 && s.BaseSizePrecision != "" {
			if v, err := strconv.ParseFloat(s.BaseSizePrecision, 64); err == nil && v > 0 {
				stepSz = v
			}
		}
		if minQty == 0 && stepSz > 0 {
			minQty = stepSz
		}
		if stepSz == 0 {
			stepSz = 0.0001
		}
		if minQty == 0 {
			minQty = stepSz
		}
		avail[s.Symbol] = true
		step[s.Symbol] = stepSz
		min[s.Symbol] = minQty
	}

	log.Printf("✅ MEXC: %d spot symbols", len(avail))
	return avail, step, min
}

// ---- WS (JSON) ----

const (
	wsURL          = "wss://wbs-api.mexc.com/ws"
	mexcMaxPerConn = 30
)

// Управление логами
var (
	debugRawFirstN = 10   // сколько «сырых» кадров показать на старте (0 — выкл)
	debugVerbose   = true // печатать ветки парсинга
)

// уникальные id для SUBSCRIPTION
var subID int64

func nextID() int64 { return atomic.AddInt64(&subID, 1) }

// триммер для логов
func trimBytes(b []byte, n int) string {
	if n <= 0 || len(b) <= n {
		return string(b)
	}
	return string(b[:n]) + "... (" + strconv.Itoa(len(b)) + " bytes)"
}

func chunkStrings(in []string, n int) [][]string {
	if n <= 0 || len(in) == 0 {
		return nil
	}
	var out [][]string
	for i := 0; i < len(in); i += n {
		end := i + n
		if end > len(in) {
			end = len(in)
		}
		out = append(out, append([]string(nil), in[i:end]...))
	}
	return out
}

// JSON-каналы
func dealsChannel(sym string) string      { return "spot@public.deals.v3.api@" + strings.ToUpper(sym) }
func bookTickerChannel(sym string) string { return "spot@public.bookTicker.v3.api@" + strings.ToUpper(sym) }

func symbolFromChannel(c string) string {
	i := strings.LastIndexByte(c, '@')
	if i < 0 || i+1 >= len(c) {
		return ""
	}
	return strings.ToUpper(c[i+1:])
}

// отправка подписки чанками (чтобы не получить "msg length invalid")
func sendSubs(conn *websocket.Conn, params []string) error {
	const maxTopics = 20 // безопасная порция на один SUBSCRIPTION
	for i := 0; i < len(params); i += maxTopics {
		end := i + maxTopics
		if end > len(params) {
			end = len(params)
		}
		sub := map[string]any{
			"method": "SUBSCRIPTION",
			"params": params[i:end],
			"id":     nextID(),
		}
		if err := conn.WriteJSON(sub); err != nil {
			return err
		}
		time.Sleep(50 * time.Millisecond) // лёгкий троттлинг
	}
	return nil
}

// ---- SubscribeTickers: deals (last) ----

func (m *MEXCExchange) SubscribeTickers(pairs []string, handler func(symbol string, price float64)) error {
	// по одному каналу на символ → можно держать 20 пар на коннект (с запасом)
	perConn := 20
	if perConn <= 0 {
		perConn = 10
	}
	chunks := chunkStrings(pairs, perConn)

	for _, ch := range chunks {
		ps := append([]string(nil), ch...)
		go func(pairs []string) {
			for {
				log.Printf("🌐 [MEXC] dial %s (tickers json, pairs=%d)", wsURL, len(pairs))
				conn, _, err := websocket.DefaultDialer.Dial(wsURL, nil)
				if err != nil {
					log.Printf("❌ [MEXC] dial: %v", err)
					time.Sleep(3 * time.Second)
					continue
				}
				log.Printf("✅ [MEXC] connected")

				params := make([]string, 0, len(pairs))
				for _, p := range pairs {
					params = append(params, dealsChannel(p))
				}
				if err := sendSubs(conn, params); err != nil {
					log.Printf("❌ [MEXC] send sub (tickers): %v", err)
					_ = conn.Close()
					time.Sleep(2 * time.Second)
					continue
				}
				log.Printf("📩 [MEXC] subscribed (tickers-json): %d (chunked)", len(params))

				donePing := make(chan struct{})
				go func(c *websocket.Conn) {
					t := time.NewTicker(20 * time.Second)
					defer t.Stop()
					for {
						select {
						case <-donePing:
							return
						case <-t.C:
							_ = c.WriteMessage(websocket.TextMessage, []byte(`{"method":"PING"}`))
						}
					}
				}(conn)

				for {
					mt, msg, err := conn.ReadMessage()
					if err != nil {
						log.Printf("⚠️  [MEXC] read: %v", err)
						close(donePing)
						_ = conn.Close()
						time.Sleep(2 * time.Second)
						break
					}

					// контрольные кадры / отказы сервера
					if mt == websocket.TextMessage {
						if bytes.Contains(msg, []byte(`"PONG"`)) || bytes.Contains(msg, []byte(`"success"`)) {
							if debugVerbose {
								log.Printf("↔️  [MEXC] control: %s", trimBytes(msg, 200))
							}
							continue
						}
						if bytes.Contains(msg, []byte("Not Subscribed successfully")) {
							log.Printf("🚫 [MEXC] refused: %s", trimBytes(msg, 500))
							continue
						}
					}

					if debugRawFirstN > 0 {
						log.Printf("[MEXC][RAW TICK] %s", trimBytes(msg, 600))
					}

					// deals: {"s":"BTCUSDT","d":[{"p":"123.45"}]}
					var f1 struct {
						S string `json:"s"`
						D []struct{ P string `json:"p"` } `json:"d"`
					}
					if json.Unmarshal(msg, &f1) == nil && f1.S != "" && len(f1.D) > 0 && f1.D[0].P != "" {
						if last, err := strconv.ParseFloat(f1.D[0].P, 64); err == nil && last > 0 {
							if debugVerbose {
								log.Printf("✅ [MEXC][DEALS s] %s last=%f", strings.ToUpper(f1.S), last)
							}
							handler(strings.ToUpper(f1.S), last)
							if debugRawFirstN > 0 {
								debugRawFirstN--
							}
							continue
						}
					}

					// deals: {"c":"spot@public.deals.v3.api@BTCUSDT","d":[{"p":"123.45"}]}
					var f2 struct {
						C string `json:"c"`
						D []struct{ P string `json:"p"` } `json:"d"`
					}
					if json.Unmarshal(msg, &f2) == nil && f2.C != "" && len(f2.D) > 0 && f2.D[0].P != "" {
						if last, err := strconv.ParseFloat(f2.D[0].P, 64); err == nil && last > 0 {
							if sym := symbolFromChannel(f2.C); sym != "" {
								if debugVerbose {
									log.Printf("✅ [MEXC][DEALS c] %s last=%f", sym, last)
								}
								handler(sym, last)
								if debugRawFirstN > 0 {
									debugRawFirstN--
								}
								continue
							}
						}
					}

					// неизвестный формат
					if debugRawFirstN > 0 {
						debugRawFirstN--
						log.Printf("🤷 [MEXC][TICK] unparsed frame (left=%d)", debugRawFirstN)
					}
				}
			}
		}(ps)
	}
	return nil
}

// ---- SubscribeQuotes: bookTicker (bid/ask) ----

func (m *MEXCExchange) SubscribeQuotes(pairs []string, handler func(symbol string, bid, ask float64, ts time.Time)) error {
	// по одному каналу на символ → можно держать 20 пар на коннект (с запасом)
	perConn := 20
	if perConn <= 0 {
		perConn = 10
	}
	chunks := chunkStrings(pairs, perConn)

	for _, ch := range chunks {
		ps := append([]string(nil), ch...)
		go func(pairs []string) {
			for {
				log.Printf("🌐 [MEXC] dial %s (quotes json, pairs=%d)", wsURL, len(pairs))
				conn, _, err := websocket.DefaultDialer.Dial(wsURL, nil)
				if err != nil {
					log.Printf("❌ [MEXC] dial: %v", err)
					time.Sleep(3 * time.Second)
					continue
				}

				// ТОЛЬКО bookTicker (без batch)
				params := make([]string, 0, len(pairs))
				for _, p := range pairs {
					params = append(params, bookTickerChannel(p))
				}
				if err := sendSubs(conn, params); err != nil {
					log.Printf("❌ [MEXC] send sub (quotes): %v", err)
					_ = conn.Close()
					time.Sleep(2 * time.Second)
					continue
				}
				log.Printf("📩 [MEXC] subscribed (quotes-json): %d (chunked)", len(params))

				donePing := make(chan struct{})
				go func(c *websocket.Conn) {
					t := time.NewTicker(20 * time.Second)
					defer t.Stop()
					for {
						select {
						case <-donePing:
							return
						case <-t.C:
							_ = c.WriteMessage(websocket.TextMessage, []byte(`{"method":"PING"}`))
						}
					}
				}(conn)

				for {
					mt, msg, err := conn.ReadMessage()
					if err != nil {
						close(donePing)
						_ = conn.Close()
						time.Sleep(2 * time.Second)
						break
					}

					// контроль/отказы
					if mt == websocket.TextMessage {
						if bytes.Contains(msg, []byte(`"PONG"`)) || bytes.Contains(msg, []byte(`"success"`)) {
							if debugVerbose {
								log.Printf("↔️  [MEXC] control: %s", trimBytes(msg, 200))
							}
							continue
						}
						if bytes.Contains(msg, []byte("Not Subscribed successfully")) {
							log.Printf("🚫 [MEXC] refused: %s", trimBytes(msg, 500))
							continue
						}
					}

					if debugRawFirstN > 0 {
						log.Printf("[MEXC][RAW QUOTE] %s", trimBytes(msg, 600))
					}

					// {"s":"BTCUSDT","d":{"bp":"...","ap":"...","t":...}}
					var a struct {
						S string `json:"s"`
						D struct {
							Bp string `json:"bp"`
							Ap string `json:"ap"`
							T  int64  `json:"t"`
						} `json:"d"`
					}
					if json.Unmarshal(msg, &a) == nil && a.S != "" && a.D.Bp != "" && a.D.Ap != "" {
						bid, _ := strconv.ParseFloat(a.D.Bp, 64)
						ask, _ := strconv.ParseFloat(a.D.Ap, 64)
						if bid > 0 && ask > 0 && ask >= bid {
							ts := time.Now()
							if a.D.T > 0 {
								ts = time.UnixMilli(a.D.T)
							}
							if debugVerbose {
								log.Printf("✅ [MEXC][QUOTE s] %s bid=%f ask=%f", strings.ToUpper(a.S), bid, ask)
							}
							handler(strings.ToUpper(a.S), bid, ask, ts)
							if debugRawFirstN > 0 {
								debugRawFirstN--
							}
							continue
						}
					}

					// {"c":"spot@public.bookTicker.v3.api@BTCUSDT","d":{"bp":"...","ap":"...","t":...}}
					var b struct {
						C string `json:"c"`
						D struct {
							Bp string `json:"bp"`
							Ap string `json:"ap"`
							T  int64  `json:"t"`
						} `json:"d"`
					}
					if json.Unmarshal(msg, &b) == nil && b.C != "" && b.D.Bp != "" && b.D.Ap != "" {
						bid, _ := strconv.ParseFloat(b.D.Bp, 64)
						ask, _ := strconv.ParseFloat(b.D.Ap, 64)
						if bid > 0 && ask > 0 && ask >= bid {
							ts := time.Now()
							if b.D.T > 0 {
								ts = time.UnixMilli(b.D.T)
							}
							if sym := symbolFromChannel(b.C); sym != "" {
								if debugVerbose {
									log.Printf("✅ [MEXC][QUOTE c] %s bid=%f ask=%f", sym, bid, ask)
								}
								handler(sym, bid, ask, ts)
								if debugRawFirstN > 0 {
									debugRawFirstN--
								}
								continue
							}
						}
					}

					// неизвестный формат
					if debugRawFirstN > 0 {
						debugRawFirstN--
						log.Printf("🤷 [MEXC][QUOTE] unparsed frame (left=%d)", debugRawFirstN)
					}
				}
			}
		}(ps)
	}
	return nil
}

// ---- REST-заглушки ----

func (m *MEXCExchange) PlaceMarketOrder(symbol, side string, quantity float64) (string, error) {
	return "", fmt.Errorf("MEXC PlaceMarketOrder not implemented")
}

func (m *MEXCExchange) GetBestAsk(symbol string) (float64, error) {
	resp, err := http.Get("https://api.mexc.com/api/v3/depth?symbol=" + symbol + "&limit=1")
	if err != nil {
		return 0, fmt.Errorf("get depth failed: %v", err)
	}
	defer resp.Body.Close()
	var data struct{ Asks [][]string `json:"asks"` }
	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		return 0, fmt.Errorf("decode depth failed: %v", err)
	}
	if len(data.Asks) == 0 {
		return 0, fmt.Errorf("no ask in depth for %s", symbol)
	}
	return strconv.ParseFloat(data.Asks[0][0], 64)
}

func (m *MEXCExchange) GetBestBid(symbol string) (float64, error) {
	resp, err := http.Get("https://api.mexc.com/api/v3/depth?symbol=" + symbol + "&limit=1")
	if err != nil {
		return 0, fmt.Errorf("get depth failed: %v", err)
	}
	defer resp.Body.Close()
	var data struct{ Bids [][]string `json:"bids"` }
	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		return 0, fmt.Errorf("decode depth failed: %v", err)
	}
	if len(data.Bids) == 0 {
		return 0, fmt.Errorf("no bid in depth for %s", symbol)
	}
	return strconv.ParseFloat(data.Bids[0][0], 64)
}



gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ cd cmd/cryptarb
gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/08/09 08:53:51 📈 Profiler: http://localhost:6060/debug/pprof/
2025/08/09 08:53:52 ✅ MEXC: 1828 spot symbols
2025/08/09 08:53:52 📊 Доступные пары (реальные+инверсии): 3656
2025/08/09 08:53:52 [TRIANGLE] Found 282 triangles
2025/08/09 08:53:52 [INIT] Треугольников найдено: 282
2025/08/09 08:53:52 [INIT] Индекс по парам: 184 ключей
2025/08/09 08:53:52 [INIT] Подписка на реальных пар: 92 шт.
2025/08/09 08:53:52 [WS][MEXC] subscribed (quotes) to 17 pairs
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=17)
2025/08/09 08:53:52 [WS][MEXC] subscribed (quotes) to 25 pairs
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=5)
2025/08/09 08:53:52 [WS][MEXC] subscribed (quotes) to 25 pairs
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=20)
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=5)
2025/08/09 08:53:52 [WS][MEXC] subscribed (quotes) to 25 pairs
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=20)
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=5)
2025/08/09 08:53:52 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=20)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 5 (chunked)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 17 (chunked)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 5 (chunked)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 20 (chunked)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 20 (chunked)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 5 (chunked)
2025/08/09 08:53:53 📩 [MEXC] subscribed (quotes-json): 20 (chunked)
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":1,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@XLMUSDC,spot@public.bookTicker.v3.api@LINGOUSDT,spot@public.bookTicker.v3.api@SUPRAUSDC,spot@public.bookTicker.v3.api@PENGUUSDC,spot@public.bookTicker.v3.api@FETUSDC].  Reason： Blocked! "}
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":2,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@USDRUSDT,spot@public.bookTicker.v3.api@AI16ZUSDT,spot@public.bookTicker.v3.api@VIRTUALUSDC,spot@public.bookTicker.v3.api@BABYDOGEUSDC,spot@public.bookTicker.v3.api@USDCUSDT,spot@public.bookTicker.v3.api@SUIUSDT,spot@public.bookTicker.v3.api@PEPEEUR,spot@public.bookTicker.v3.api@BUTTHOLEUSDC,spot@public.bookTicker.v3.api@BABYDOGEUSDT,spot@public.bookTicker.v3.api@KAITOUSDT,spot@public.bookTicker.v3.api@VIRTUALUSDT... (757 bytes)
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":3,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@USDRUSDC,spot@public.bookTicker.v3.api@ULTIMAUSDT,spot@public.bookTicker.v3.api@FARTCOINUSDT,spot@public.bookTicker.v3.api@KASEUR,spot@public.bookTicker.v3.api@PEPEUSDT].  Reason： Blocked! "}
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":4,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@ICPUSDT,spot@public.bookTicker.v3.api@MELANIAUSDT,spot@public.bookTicker.v3.api@FARTCOINUSDC,spot@public.bookTicker.v3.api@QUBICUSDT,spot@public.bookTicker.v3.api@XMRUSDT,spot@public.bookTicker.v3.api@KASUSDC,spot@public.bookTicker.v3.api@SOSOUSDC,spot@public.bookTicker.v3.api@LINKUSDT,spot@public.bookTicker.v3.api@KEKIUSUSDC,spot@public.bookTicker.v3.api@RAIUSDT,spot@public.bookTicker.v3.api@AIXBTUSDC,spot@publi... (859 bytes)
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":6,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@NPCUSDC,spot@public.bookTicker.v3.api@ULTIMAUSDC,spot@public.bookTicker.v3.api@SUPRAUSDT,spot@public.bookTicker.v3.api@SENUSDT,spot@public.bookTicker.v3.api@RIOUSDT,spot@public.bookTicker.v3.api@RBNTUSDC,spot@public.bookTicker.v3.api@INJUSDC,spot@public.bookTicker.v3.api@TAOUSDC,spot@public.bookTicker.v3.api@NAKAUSDC,spot@public.bookTicker.v3.api@ENAUSDC,spot@public.bookTicker.v3.api@HBARUSDC,spot@public.bookTick... (856 bytes)
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":5,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@MELANIAUSDC,spot@public.bookTicker.v3.api@USDCEUR,spot@public.bookTicker.v3.api@RIOUSDC,spot@public.bookTicker.v3.api@SENUSDC,spot@public.bookTicker.v3.api@BUTTHOLEUSDT].  Reason： Blocked! "}
2025/08/09 08:53:53 🚫 [MEXC] refused: {"id":7,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@KAITOUSDC,spot@public.bookTicker.v3.api@PIUSDC,spot@public.bookTicker.v3.api@TURBOUSDT,spot@public.bookTicker.v3.api@INJUSDT,spot@public.bookTicker.v3.api@PEAQUSDT,spot@public.bookTicker.v3.api@DSYNCUSDC,spot@public.bookTicker.v3.api@ONDOUSDC,spot@public.bookTicker.v3.api@PEPEUSDC,spot@public.bookTicker.v3.api@CGPTUSDT,spot@public.bookTicker.v3.api@TONUSDC,spot@public.bookTicker.v3.api@SUSDC,spot@public.bookTicke... (852 bytes)
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:13 ↔️  [MEXC] control: {"id":0,"code":0,"msg":"PONG"}
2025/08/09 08:54:26 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=20)
2025/08/09 08:54:27 📩 [MEXC] subscribed (quotes-json): 20 (chunked)
2025/08/09 08:54:27 🚫 [MEXC] refused: {"id":8,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@NPCUSDC,spot@public.bookTicker.v3.api@ULTIMAUSDC,spot@public.bookTicker.v3.api@SUPRAUSDT,spot@public.bookTicker.v3.api@SENUSDT,spot@public.bookTicker.v3.api@RIOUSDT,spot@public.bookTicker.v3.api@RBNTUSDC,spot@public.bookTicker.v3.api@INJUSDC,spot@public.bookTicker.v3.api@TAOUSDC,spot@public.bookTicker.v3.api@NAKAUSDC,spot@public.bookTicker.v3.api@ENAUSDC,spot@public.bookTicker.v3.api@HBARUSDC,spot@public.bookTick... (856 bytes)
2025/08/09 08:54:27 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=5)
2025/08/09 08:54:27 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=20)
2025/08/09 08:54:28 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=20)
2025/08/09 08:54:28 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=5)
2025/08/09 08:54:28 📩 [MEXC] subscribed (quotes-json): 5 (chunked)
2025/08/09 08:54:28 📩 [MEXC] subscribed (quotes-json): 20 (chunked)
2025/08/09 08:54:28 🚫 [MEXC] refused: {"id":9,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@MELANIAUSDC,spot@public.bookTicker.v3.api@USDCEUR,spot@public.bookTicker.v3.api@RIOUSDC,spot@public.bookTicker.v3.api@SENUSDC,spot@public.bookTicker.v3.api@BUTTHOLEUSDT].  Reason： Blocked! "}
2025/08/09 08:54:28 🚫 [MEXC] refused: {"id":10,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@KAITOUSDC,spot@public.bookTicker.v3.api@PIUSDC,spot@public.bookTicker.v3.api@TURBOUSDT,spot@public.bookTicker.v3.api@INJUSDT,spot@public.bookTicker.v3.api@PEAQUSDT,spot@public.bookTicker.v3.api@DSYNCUSDC,spot@public.bookTicker.v3.api@ONDOUSDC,spot@public.bookTicker.v3.api@PEPEUSDC,spot@public.bookTicker.v3.api@CGPTUSDT,spot@public.bookTicker.v3.api@TONUSDC,spot@public.bookTicker.v3.api@SUSDC,spot@public.bookTick... (853 bytes)
2025/08/09 08:54:29 📩 [MEXC] subscribed (quotes-json): 5 (chunked)
2025/08/09 08:54:29 📩 [MEXC] subscribed (quotes-json): 20 (chunked)
2025/08/09 08:54:29 🚫 [MEXC] refused: {"id":11,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@USDRUSDC,spot@public.bookTicker.v3.api@ULTIMAUSDT,spot@public.bookTicker.v3.api@FARTCOINUSDT,spot@public.bookTicker.v3.api@KASEUR,spot@public.bookTicker.v3.api@PEPEUSDT].  Reason： Blocked! "}
2025/08/09 08:54:29 🚫 [MEXC] refused: {"id":12,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@ICPUSDT,spot@public.bookTicker.v3.api@MELANIAUSDT,spot@public.bookTicker.v3.api@FARTCOINUSDC,spot@public.bookTicker.v3.api@QUBICUSDT,spot@public.bookTicker.v3.api@XMRUSDT,spot@public.bookTicker.v3.api@KASUSDC,spot@public.bookTicker.v3.api@SOSOUSDC,spot@public.bookTicker.v3.api@LINKUSDT,spot@public.bookTicker.v3.api@KEKIUSUSDC,spot@public.bookTicker.v3.api@RAIUSDT,spot@public.bookTicker.v3.api@AIXBTUSDC,spot@publ... (860 bytes)
2025/08/09 08:54:29 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=5)
2025/08/09 08:54:30 📩 [MEXC] subscribed (quotes-json): 5 (chunked)
2025/08/09 08:54:30 🌐 [MEXC] dial wss://wbs-api.mexc.com/ws (quotes json, pairs=17)
2025/08/09 08:54:30 🚫 [MEXC] refused: {"id":13,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@XLMUSDC,spot@public.bookTicker.v3.api@LINGOUSDT,spot@public.bookTicker.v3.api@SUPRAUSDC,spot@public.bookTicker.v3.api@PENGUUSDC,spot@public.bookTicker.v3.api@FETUSDC].  Reason： Blocked! "}
2025/08/09 08:54:31 📩 [MEXC] subscribed (quotes-json): 17 (chunked)
2025/08/09 08:54:32 🚫 [MEXC] refused: {"id":14,"code":0,"msg":"Not Subscribed successfully! [spot@public.bookTicker.v3.api@USDRUSDT,spot@public.bookTicker.v3.api@AI16ZUSDT,spot@public.bookTicker.v3.api@VIRTUALUSDC,spot@public.bookTicker.v3.api@BABYDOGEUSDC,spot@public.bookTicker.v3.api@USDCUSDT,spot@public.bookTicker.v3.api@SUIUSDT,spot@public.bookTicker.v3.api@PEPEEUR,spot@public.bookTicker.v3.api@BUTTHOLEUSDC,spot@public.bookTicker.v3.api@BABYDOGEUSDT,spot@public.bookTicker.v3.api@KAITOUSDT,spot@public.bookTicker.v3.api@VIRTUALUSD... (758 bytes)
^Csignal: interrupt







