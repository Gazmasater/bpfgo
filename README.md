8ae390ad89f04bec97cb7b81413de813


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
	"math"
	"strconv"
	"sync"
	"time"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

type Arbitrager struct {
	Triangles       []triangle.Triangle
	latest          map[string]float64
	trianglesByPair map[string][]int

	// realSymbols теперь map[string]bool
	realSymbols map[string]bool

	stepSizes map[string]float64
	minQtys   map[string]float64
	mu        sync.Mutex

	StartAmount float64
	exchange    exchange.Exchange
}

func New(ex exchange.Exchange) (*Arbitrager, error) {
	// 1. Загружаем доступные пары и фильтры
	rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()

	// 2. Добавляем инверсии символов
	avail := filesystem.ExpandAvailableSymbols(rawSymbols)
	log.Printf("📊 Всего доступных пар (с инверсиями): %d", len(avail))

	// 3. Строим треугольники
	ts, err := filesystem.LoadTrianglesFromSymbols(avail)
	if err != nil {
		return nil, fmt.Errorf("LoadTrianglesFromSymbols: %w", err)
	}
	log.Printf("[INIT] Загружено треугольников: %d", len(ts))

	// 4. Создание карты индексов треугольников и подписываемых пар
	trianglesByPair := make(map[string][]int)
	var subPairsRaw []string
	for i, tri := range ts {
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ca := tri.C + tri.A

		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)

		subPairsRaw = append(subPairsRaw, ab, bc, ca)
	}

	// 5. Фильтрация уникальных доступных пар
	uniq := make(map[string]struct{})
	for _, p := range subPairsRaw {
		if avail[p] {
			uniq[p] = struct{}{}
		}
	}
	var subPairs []string
	for p := range uniq {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] Итог: подписываемся на %d уникальных пар", len(subPairs))

	// 6. Создаём арбитражер
	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
		realSymbols:     avail, // теперь map[string]bool
		stepSizes:       stepSizes,
		minQtys:         minQtys,
		StartAmount:     0.5,
		exchange:        ex,
	}

	// 7. Подписка чанками
	const maxPerConn = 20
	for i := 0; i < len(subPairs); i += maxPerConn {
		end := i + maxPerConn
		if end > len(subPairs) {
			end = len(subPairs)
		}
		chunk := subPairs[i:end]
		go func(pairs []string) {
			for {
				if err := ex.SubscribeDeals(pairs, arb.HandleRaw); err != nil {
					log.Printf("[WS][%s] subscribe error: %v; retrying...", ex.Name(), err)
					time.Sleep(time.Second)
					continue
				}
				return
			}
		}(chunk)
	}

	return arb, nil
}

func (a *Arbitrager) normalizeSymbolDir(base, quote string) (string, bool, bool) {
	// теперь realSymbols содержит только bool
	if a.realSymbols[base+quote] {
		return base + quote, true, false
	}
	if a.realSymbols[quote+base] {
		return quote + base, true, true
	}
	return "", false, false
}

func (a *Arbitrager) HandleRaw(exchangeName string, raw []byte) {
	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil || msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		return
	}
	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		return
	}
	a.mu.Lock()
	a.latest[msg.Symbol] = price
	a.mu.Unlock()
	a.Check(msg.Symbol)
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		return
	}

	nf := 0.9965 * 0.9965 * 0.9965

	for _, i := range indices {
		tri := a.Triangles[i]
		ab, okAB, revAB := a.normalizeSymbolDir(tri.A, tri.B)
		bc, okBC, revBC := a.normalizeSymbolDir(tri.B, tri.C)
		ca, okCA, revCA := a.normalizeSymbolDir(tri.C, tri.A)
		if !okAB || !okBC || !okCA {
			continue
		}
		p1, ok1 := a.latest[ab]
		p2, ok2 := a.latest[bc]
		p3, ok3 := a.latest[ca]
		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}
		if revAB {
			p1 = 1 / p1
		}
		if revBC {
			p2 = 1 / p2
		}
		if revCA {
			p3 = 1 / p3
		}
		profitFactor := p1 * p2 * p3 * nf
		profit := (profitFactor - 1) * 100
		if profit > 0.3 && tri.A == "USDT" {
			log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
		}
	}
}

func roundToStep(symbol string, value float64, steps map[string]float64) float64 {
	step := steps[symbol]
	if step == 0 {
		return value
	}
	return math.Floor(value/step) * step
}

package exchange

// Exchange определяет интерфейс криптобиржи для подключения к арбитражному движку
// и абстрагирует WebSocket/REST взаимодействие.
type Exchange interface {
	// Name возвращает уникальное имя биржи (например: "MEXC")
	Name() string

	// FetchAvailableSymbols возвращает список доступных торговых пар ("BTCUSDT" и т.д.)
	FetchAvailableSymbols() (map[string]bool, map[string]float64, map[string]float64)
	// SubscribeDeals подписывается на обновления по указанным парам
	// и вызывает handler при каждом событии сделки
	SubscribeDeals(pairs []string, handler func(exchange string, raw []byte)) error

	PlaceMarketOrder(symbol, side string, quantity float64) (orderID string, err error)

	GetBestAsk(symbol string) (float64, error)
	GetBestBid(symbol string) (float64, error)
}

package triangle

import "log"

type Triangle struct{ A, B, C string }

// Filter оставляет только те треугольники, пары в которых есть в available
func Filter(ts []Triangle, available map[string]bool) []Triangle {
	out := make([]Triangle, 0, len(ts))
	var skipped int

	for _, t := range ts {
		ok := func(a, b string) bool {
			return available[a+b] || available[b+a]
		}

		okAB := ok(t.A, t.B)
		okBC := ok(t.B, t.C)
		okCA := ok(t.C, t.A)

		if okAB && okBC && okCA {
			out = append(out, t)
		} else {
			skipped++
			log.Printf("[❌ FILTER] %s-%s-%s excluded: AB=%v BC=%v CA=%v",
				t.A, t.B, t.C, okAB, okBC, okCA)
		}
	}

	log.Printf("[FILTER ✅] Осталось треугольников после фильтрации: %d (пропущено: %d)", len(out), skipped)

	return out
}

func SymbolPairs(ts []Triangle) []string {
	set := make(map[string]struct{})
	for _, t := range ts {
		for _, p := range [][2]string{{t.A, t.B}, {t.B, t.C}, {t.A, t.C}} {
			set[p[0]+p[1]] = struct{}{}
		}
	}
	out := make([]string, 0, len(set))
	for k := range set {
		out = append(out, k)
	}
	return out
}


package filesystem

import (
	"cryptarb/internal/domain/triangle"
	"log"
)

// LoadTrianglesFromSymbols строит граф и находит треугольники на основе доступных символов
func LoadTrianglesFromSymbols(baseQuote map[string][2]string) ([]triangle.Triangle, error) {
	graph := make(map[string][]string)

	for _, parts := range baseQuote {
		base := parts[0]
		quote := parts[1]
		graph[quote] = append(graph[quote], base) // Покупка base за quote
		graph[base] = append(graph[base], quote)  // Продажа base за quote
	}

	var tris []triangle.Triangle
	seen := make(map[[3]string]struct{})

	for a, bList := range graph {
		for _, b := range bList {
			for _, c := range graph[b] {
				for _, back := range graph[c] {
					if back == a {
						key := [3]string{a, b, c}
						if _, ok := seen[key]; !ok {
							seen[key] = struct{}{}
							tris = append(tris, triangle.Triangle{A: a, B: b, C: c})
						}
					}
				}
			}
		}
	}

	log.Printf("[TRIANGLE] Found %d triangles", len(tris))
	return tris, nil
}

// ExpandAvailableSymbols добавляет инверсии символов и возвращает map: symbol → [base, quote]
func ExpandAvailableSymbols(raw map[string]bool) map[string][2]string {
	pairs := make(map[string][2]string)
	for symbol := range raw {
		base, quote := unpackPair(symbol)
		if base != "" && quote != "" {
			pairs[symbol] = [2]string{base, quote}
			inv := quote + base
			pairs[inv] = [2]string{quote, base} // обратная пара
		}
	}
	return pairs
}

// unpackPair разбивает символ на base и quote по известным суффиксам
func unpackPair(pair string) (string, string) {
	quotes := []string{"USDT", "USDC", "BTC", "ETH", "EUR", "BRL", "USD1", "USDE"}
	for _, q := range quotes {
		if len(pair) > len(q) && pair[len(pair)-len(q):] == q {
			return pair[:len(pair)-len(q)], q
		}
	}
	return "", ""
}


package mexc

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"net/url"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

type Mexc struct{}

type MexcExchange struct {
	apiKey    string
	apiSecret string
}

func NewMexcExchange(apiKey, apiSecret string) *MexcExchange {
	return &MexcExchange{
		apiKey:    apiKey,
		apiSecret: apiSecret,
	}
}

func (m *MexcExchange) Name() string {
	return "MEXC"
}

func (m *MexcExchange) PlaceMarketOrder(symbol, side string, quantity float64) (string, error) {
	endpoint := "https://api.mexc.com/api/v3/order"
	timestamp := strconv.FormatInt(time.Now().UnixMilli(), 10)

	// Запрос тела
	params := make(map[string]string)
	params["symbol"] = symbol
	params["side"] = strings.ToUpper(side) // "BUY" или "SELL"
	params["type"] = "MARKET"
	if side == "BUY" {
		params["quoteOrderQty"] = fmt.Sprintf("%.4f", quantity) // сумма в USDT
	} else {
		params["quantity"] = fmt.Sprintf("%.6f", quantity) // количество монет
	}

	// Формируем строку запроса
	query := url.Values{}
	for k, v := range params {
		query.Set(k, v)
	}
	query.Set("timestamp", timestamp)

	// Подпись
	signature := createSignature(m.apiSecret, query.Encode())
	query.Set("signature", signature)

	// Отправка
	req, _ := http.NewRequest("POST", endpoint+"?"+query.Encode(), nil)
	req.Header.Set("X-MEXC-APIKEY", m.apiKey)

	client := &http.Client{Timeout: 10 * time.Second}
	resp, err := client.Do(req)
	if err != nil {
		return "", fmt.Errorf("HTTP error: %v", err)
	}
	defer resp.Body.Close()

	body, _ := io.ReadAll(resp.Body)
	if resp.StatusCode != 200 {
		return "", fmt.Errorf("order failed: %s", string(body))
	}

	var result struct {
		OrderID string `json:"orderId"`
	}
	if err := json.Unmarshal(body, &result); err != nil {
		return "", fmt.Errorf("decode error: %v", err)
	}

	return result.OrderID, nil
}

func createSignature(secret, query string) string {
	h := hmac.New(sha256.New, []byte(secret))
	h.Write([]byte(query))
	return hex.EncodeToString(h.Sum(nil))
}

func (Mexc) Name() string {
	return "MEXC"
}

func (m *MexcExchange) SubscribeDeals(pairs []string, handler func(exchange string, raw []byte)) error {
	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": buildChannels(pairs),
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return err
	}

	var mu sync.Mutex
	var lastPing time.Time
	conn.SetPongHandler(func(string) error {
		log.Printf("📶 [MEXC] Pong after %v\n", time.Since(lastPing))
		return nil
	})

	go func() {
		t := time.NewTicker(45 * time.Second)
		defer t.Stop()
		for range t.C {
			mu.Lock()
			lastPing = time.Now()
			_ = conn.WriteMessage(websocket.PingMessage, []byte("hb"))
			mu.Unlock()
		}
	}()

	for {
		_, raw, err := conn.ReadMessage()
		if err != nil {
			return err
		}
		handler("MEXC", raw)
	}
}

func buildChannels(pairs []string) []string {
	out := make([]string, 0, len(pairs))
	for _, p := range pairs {
		out = append(out, "spot@public.deals.v3.api@"+p)
	}
	return out
}

func (m *MexcExchange) FetchAvailableSymbols() (map[string]bool, map[string]float64, map[string]float64) {
	availableSymbols := make(map[string]bool)
	stepSizes := make(map[string]float64)
	minQtys := make(map[string]float64) // будем использовать такую же точность, как и stepSize

	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		log.Printf("❌ Ошибка запроса exchangeInfo: %v", err)
		return availableSymbols, stepSizes, minQtys
	}
	defer resp.Body.Close()

	bodyBytes, err := io.ReadAll(resp.Body)
	if err != nil {
		log.Printf("❌ Ошибка чтения тела ответа: %v", err)
		return availableSymbols, stepSizes, minQtys
	}

	// Сохраняем весь JSON для анализа
	_ = os.WriteFile("all_symbols_full.json", bodyBytes, 0644)

	var raw struct {
		Symbols []map[string]interface{} `json:"symbols"`
	}
	if err := json.Unmarshal(bodyBytes, &raw); err != nil {
		log.Printf("❌ Ошибка разбора JSON: %v", err)
		return availableSymbols, stepSizes, minQtys
	}

	var availableLog []string
	var excludedLog []string

	for _, s := range raw.Symbols {
		symbol, _ := s["symbol"].(string)
		if symbol == "" {
			continue
		}

		var reasons []string

		// Проверки
		if s["status"] != "1" {
			reasons = append(reasons, "status != 1")
		}

		if allowed, ok := s["isSpotTradingAllowed"].(bool); !ok || !allowed {
			reasons = append(reasons, "spot trading not allowed")
		}

		orderTypes, _ := s["orderTypes"].([]interface{})
		hasMarket := false
		for _, o := range orderTypes {
			if str, ok := o.(string); ok && str == "MARKET" {
				hasMarket = true
				break
			}
		}
		if !hasMarket {
			reasons = append(reasons, "no MARKET order")
		}

		// baseSizePrecision = шаг
		stepStr, _ := s["baseSizePrecision"].(string)
		step, err := strconv.ParseFloat(stepStr, 64)
		if err != nil || step <= 0 {
			reasons = append(reasons, "invalid baseSizePrecision")
		}

		if len(reasons) == 0 {
			availableSymbols[symbol] = true
			stepSizes[symbol] = step
			minQtys[symbol] = step // допустим: минимум равен шагу
			availableLog = append(availableLog, fmt.Sprintf("%s\t✅ step=%s", symbol, strconv.FormatFloat(step, 'g', -1, 64)))
		} else {
			excludedLog = append(excludedLog, fmt.Sprintf("%s\t⛔ %s", symbol, strings.Join(reasons, ", ")))
		}
	}

	_ = os.WriteFile("available_all_symbols.log", []byte(strings.Join(availableLog, "\n")), 0644)
	_ = os.WriteFile("excluded_all_symbols.log", []byte(strings.Join(excludedLog, "\n")), 0644)

	log.Printf("✅ Подходящих пар: %d", len(availableSymbols))
	log.Printf("📂 Сохранено: all_symbols_full.json, available_all_symbols.log, excluded_all_symbols.log")

	return availableSymbols, stepSizes, minQtys
}

func (m *MexcExchange) GetBestAsk(symbol string) (float64, error) {
	resp, err := http.Get("https://api.mexc.com/api/v3/depth?symbol=" + symbol + "&limit=1")
	if err != nil {
		return 0, fmt.Errorf("get depth failed: %v", err)
	}
	defer resp.Body.Close()

	var data struct {
		Asks [][]string `json:"asks"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		return 0, fmt.Errorf("decode depth failed: %v", err)
	}
	if len(data.Asks) == 0 {
		return 0, fmt.Errorf("no ask in depth for %s", symbol)
	}
	return strconv.ParseFloat(data.Asks[0][0], 64)
}

func (m *MexcExchange) GetBestBid(symbol string) (float64, error) {
	resp, err := http.Get("https://api.mexc.com/api/v3/depth?symbol=" + symbol + "&limit=1")
	if err != nil {
		return 0, fmt.Errorf("get depth failed: %v", err)
	}
	defer resp.Body.Close()

	var data struct {
		Bids [][]string `json:"bids"`
	}
	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		return 0, fmt.Errorf("decode depth failed: %v", err)
	}
	if len(data.Bids) == 0 {
		return 0, fmt.Errorf("no bid in depth for %s", symbol)
	}
	return strconv.ParseFloat(data.Bids[0][0], 64)
}







