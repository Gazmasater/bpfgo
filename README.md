
mx0vgldDsxPkp5g9tc
82889cdd48fe4437b474558b91066c9d

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


package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"crypto/tls"
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
	"time"

	"github.com/gorilla/websocket"
	"github.com/joho/godotenv"
)

// ===================== КЛИЕНТ MEXC (одним пакетом) =====================

type MexcExchange struct {
	apiKey    string
	apiSecret string
}

func NewMexcExchange(apiKey, apiSecret string) *MexcExchange {
	return &MexcExchange{apiKey: apiKey, apiSecret: apiSecret}
}

func (m *MexcExchange) Name() string { return "MEXC" }

// --------------------- ВСПОМОГАТЕЛЬНОЕ ---------------------

func createSignature(secret, query string) string {
	h := hmac.New(sha256.New, []byte(secret))
	h.Write([]byte(query))
	return hex.EncodeToString(h.Sum(nil))
}

func httpClient() *http.Client { return &http.Client{Timeout: 10 * time.Second} }

// Добавляем timestamp/recvWindow и signature в query
func signParams(secret string, params url.Values) url.Values {
	if params == nil {
		params = url.Values{}
	}
	if params.Get("timestamp") == "" {
		params.Set("timestamp", strconv.FormatInt(time.Now().UnixMilli(), 10))
	}
	if params.Get("recvWindow") == "" {
		params.Set("recvWindow", "5000")
	}
	sig := createSignature(secret, params.Encode())
	params.Set("signature", sig)
	return params
}

// Выполнить запрос с APIKEY и, при необходимости, подписанными params в query
func doReq(method, endpoint, apiKey string, params url.Values) (int, []byte, error) {
	if len(params) > 0 {
		endpoint += "?" + params.Encode()
	}
	req, _ := http.NewRequest(method, endpoint, nil)
	req.Header.Set("X-MEXC-APIKEY", apiKey)
	resp, err := httpClient().Do(req)
	if err != nil {
		return 0, nil, err
	}
	defer resp.Body.Close()
	body, _ := io.ReadAll(resp.Body)
	return resp.StatusCode, body, nil
}

// --------------------- LISTEN KEY (SIGNED) ---------------------

func (m *MexcExchange) createListenKey() (string, error) {
	params := signParams(m.apiSecret, nil)
	status, body, err := doReq("POST", "https://api.mexc.com/api/v3/userDataStream", m.apiKey, params)
	if err != nil {
		return "", fmt.Errorf("create listenKey: %w", err)
	}
	if status != http.StatusOK {
		return "", fmt.Errorf("listenKey http %d: %s", status, strings.TrimSpace(string(body)))
	}
	var out struct{ ListenKey string `json:"listenKey"` }
	if err := json.Unmarshal(body, &out); err != nil || out.ListenKey == "" {
		return "", fmt.Errorf("decode listenKey: %v; body=%s", err, body)
	}
	return out.ListenKey, nil
}

func (m *MexcExchange) keepAliveListenKey(listenKey string, stop <-chan struct{}) {
	t := time.NewTicker(30 * time.Minute) // ключ живёт ~60м — продлеваем заранее
	defer t.Stop()
	for {
		select {
		case <-t.C:
			params := url.Values{}
			params.Set("listenKey", listenKey)
			params = signParams(m.apiSecret, params)
			status, body, err := doReq("PUT", "https://api.mexc.com/api/v3/userDataStream", m.apiKey, params)
			if err != nil || status != http.StatusOK {
				log.Printf("[MEXC] keepAlive error: status=%d err=%v body=%s", status, err, strings.TrimSpace(string(body)))
			}
		case <-stop:
			return
		}
	}
}

func (m *MexcExchange) closeListenKey(listenKey string) {
	params := url.Values{}
	params.Set("listenKey", listenKey)
	params = signParams(m.apiSecret, params)
	status, body, err := doReq("DELETE", "https://api.mexc.com/api/v3/userDataStream", m.apiKey, params)
	if err != nil || status != http.StatusOK {
		log.Printf("[MEXC] close listenKey error: status=%d err=%v body=%s", status, err, strings.TrimSpace(string(body)))
	}
}

// --------------------- PUBLIC: пары/фильтры ---------------------

// Вернём: available(symbol->true), stepSizes(symbol->step), minQtys(symbol->minQty)
func (m *MexcExchange) FetchAvailableSymbols() (map[string]bool, map[string]float64, map[string]float64) {
	transport := &http.Transport{
		DisableCompression: true,
		TLSNextProto:       make(map[string]func(string, *tls.Conn) http.RoundTripper),
	}
	client := &http.Client{Timeout: 10 * time.Second, Transport: transport}

	resp, err := client.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		log.Printf("❌ Ошибка запроса exchangeInfo: %v", err)
		return nil, nil, nil
	}
	defer resp.Body.Close()

	dec := json.NewDecoder(resp.Body)

	// Проматываем до "symbols"
	for {
		tok, err := dec.Token()
		if err != nil {
			log.Printf("❌ Не нашли поле symbols: %v", err)
			return nil, nil, nil
		}
		if key, ok := tok.(string); ok && key == "symbols" {
			break
		}
	}
	// Ожидаем '['
	if _, err := dec.Token(); err != nil {
		log.Printf("❌ Ожидаем '[': %v", err)
		return nil, nil, nil
	}

	type symInfo struct {
		Symbol               string `json:"symbol"`
		Status               string `json:"status"`
		IsSpotTradingAllowed bool   `json:"isSpotTradingAllowed"`
		BaseSizePrecision    string `json:"baseSizePrecision"`
	}
	type valid struct {
		sym  string
		step float64
	}
	valids := make([]valid, 0, 64)

	for dec.More() {
		var s symInfo
		if err := dec.Decode(&s); err != nil {
			log.Printf("❌ Ошибка Decode: %v", err)
			return nil, nil, nil
		}
		if s.Status != "1" || !s.IsSpotTradingAllowed {
			continue
		}
		step, err := strconv.ParseFloat(s.BaseSizePrecision, 64)
		if err != nil || step <= 0 {
			continue
		}
		valids = append(valids, valid{s.Symbol, step})
	}
	_, _ = dec.Token() // закрываем ']'

	n := len(valids)
	available := make(map[string]bool, n)
	stepSizes := make(map[string]float64, n)
	minQtys := make(map[string]float64, n)

	var logLines []string
	for _, v := range valids {
		available[v.sym] = true
		stepSizes[v.sym] = v.step
		minQtys[v.sym] = v.step
		logLines = append(logLines, fmt.Sprintf("%s\tstep=%g", v.sym, v.step))
	}
	_ = os.WriteFile("available_all_symbols.log", []byte(strings.Join(logLines, "\n")), 0644)
	log.Printf("✅ Подходящих пар: %d", n)

	return available, stepSizes, minQtys
}

// --------------------- WS подписки (deals JSON) ---------------------

func (m *MexcExchange) SubscribeDeals(pairs []string, handler func(exchange string, raw []byte)) error {
	// 1) создаём listenKey
	lk, err := m.createListenKey()
	if err != nil {
		return fmt.Errorf("get listenKey: %w", err)
	}
	defer m.closeListenKey(lk)

	// 2) продлеваем в фоне
	stopKA := make(chan struct{})
	go m.keepAliveListenKey(lk, stopKA)
	defer close(stopKA)

	// 3) подключаемся к приватному ws
	wsURL := "wss://wbs.mexc.com/ws?listenKey=" + lk
	conn, _, err := websocket.DefaultDialer.Dial(wsURL, nil)
	if err != nil {
		return fmt.Errorf("ws dial: %w", err)
	}
	defer conn.Close()

	// 4) подписка на сделки JSON
	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": buildChannels(pairs),
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return fmt.Errorf("ws write sub: %w", err)
	}

	// 5) heartbeat
	go func() {
		t := time.NewTicker(45 * time.Second)
		defer t.Stop()
		for range t.C {
			_ = conn.WriteMessage(websocket.PingMessage, []byte("hb"))
		}
	}()

	// 6) чтение
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

// --------------------- PUBLIC best bid/ask ---------------------

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

// ===================== MAIN =====================

func main() {
	_ = godotenv.Load(".env")

	apiKey := os.Getenv("MEXC_API_KEY")
	secret := os.Getenv("MEXC_SECRET_KEY")
	if apiKey == "" || secret == "" {
		log.Fatal("❌ MEXC_API_KEY / MEXC_SECRET_KEY пусты — проверь .env")
	}

	ex := NewMexcExchange(apiKey, secret)

	// Пример: подписываемся на 3 пары
	pairs := []string{"BTCUSDT", "ETHUSDT", "ETHBTC"}

	// Хендлер просто печатает сырой JSON (как у тебя парсит app.HandleRaw "s"/"p")
	handler := func(exchange string, raw []byte) {
		// Быстрая фильтрация по "s" и "p" не делаем — просто покажем строку.
		fmt.Println(string(raw))
	}

	log.Println("🔌 Подключаюсь к MEXC WS (private) и подписываюсь на deals...")
	if err := ex.SubscribeDeals(pairs, handler); err != nil {
		log.Fatal("WS error:", err)
	}
}





