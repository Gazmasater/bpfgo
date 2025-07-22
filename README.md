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


// Go-бот: 7 треугольников + deals + ping-pong + детальное логирование + тайминг сборки треугольника
package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"sort"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

type Triangle struct{ A, B, C string }

type SymbolInfo struct{ Symbol string }

type ExchangeInfo struct{ Symbols []SymbolInfo }

type DealsMsg struct {
	Symbol string `json:"symbol"`
	Data   []struct {
		Price     string `json:"p"`
		Quantity  string `json:"v"`
		Timestamp int64  `json:"T"`
		Side      string `json:"S"`
	} `json:"data"`
}

var (
	priceLock  sync.Mutex
	lastUpdate = map[string]time.Time{} // время последнего тика для каждой пары
)

func ensureTrianglesFile() error {
	triangles := []Triangle{
		{"XRP", "BTC", "USDT"},
		{"ETH", "BTC", "USDT"},
		{"TRX", "BTC", "USDT"},
		{"ADA", "USDT", "BTC"},
		{"BTC", "SOL", "USDT"},
		{"XRP", "USDT", "ETH"},
		{"XRP", "BTC", "ETH"},
	}
	data, _ := json.MarshalIndent(triangles, "", "  ")
	return ioutil.WriteFile("triangles.json", data, 0644)
}

func loadTriangles() ([]Triangle, error) {
	if err := ensureTrianglesFile(); err != nil {
		return nil, fmt.Errorf("не удалось создать triangles.json: %v", err)
	}
	b, err := ioutil.ReadFile("triangles.json")
	if err != nil {
		return nil, err
	}
	var ts []Triangle
	if err := json.Unmarshal(b, &ts); err != nil {
		return nil, err
	}
	return ts, nil
}

func fetchAvailableSymbols() map[string]bool {
	out := make(map[string]bool, 0)
	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		log.Println("⚠️ Ошибка fetching symbols:", err)
		return out
	}
	defer resp.Body.Close()
	b, _ := ioutil.ReadAll(resp.Body)
	var info ExchangeInfo
	if err := json.Unmarshal(b, &info); err != nil {
		log.Println("⚠️ Ошибка парсинга exchangeInfo:", err)
		return out
	}
	for _, s := range info.Symbols {
		out[s.Symbol] = true
	}
	return out
}

func buildChannels(triangles []Triangle, valid map[string]bool) []string {
	set := map[string]bool{}
	for _, t := range triangles {
		pairs := [][2]string{{t.A, t.B}, {t.B, t.C}, {t.A, t.C}}
		for _, pr := range pairs {
			a, b := pr[0], pr[1]
			p1, p2 := a+b, b+a
			switch {
			case valid[p1]:
				set[p1] = true
				log.Printf("✅ Пара %s найдена", p1)
			case valid[p2]:
				set[p2] = true
				log.Printf("🔁 Переворачиваем %s → %s", p1, p2)
			default:
				log.Printf("❌ Пара %s/%s не найдена на бирже", a, b)
			}
		}
	}
	list := make([]string, 0, len(set))
	for s := range set {
		list = append(list, s)
	}
	sort.Strings(list)
	channels := make([]string, len(list))
	for i, s := range list {
		channels[i] = fmt.Sprintf("spot@public.deals.v3.api@%s", s)
	}
	return channels
}

func reversePair(s string) string {
	n := len(s) / 2
	return s[n:] + s[:n]
}

func logPriceUpdate(symbol, side, price string) {
	log.Printf("📊 %s | side=%s | price=%s", symbol, side, price)
}

func findActualPairKey(a, b string) string {
	key := a + b
	if _, ok := lastUpdate[key]; ok {
		return key
	}
	return b + a
}

func checkTriangleTimings(triangles []Triangle) {
	priceLock.Lock()
	defer priceLock.Unlock()

	now := time.Now()

	for _, t := range triangles {
		ps := []string{
			findActualPairKey(t.A, t.B),
			findActualPairKey(t.B, t.C),
			findActualPairKey(t.A, t.C),
		}

		var times []time.Time
		missing := false

		for _, p := range ps {
			ts, ok := lastUpdate[p]
			if !ok {
				log.Printf("⚠️ Пара %s ещё не получила ни одного тика", p)
				missing = true
				break
			}
			age := now.Sub(ts)
			log.Printf("⏱ Пара %s обновлена %v назад (в %s)",
				p,
				age.Truncate(time.Millisecond),
				ts.Format("15:04:05.000"),
			)
			times = append(times, ts)
		}

		if missing {
			continue
		}

		sort.Slice(times, func(i, j int) bool { return times[i].Before(times[j]) })
		delta := times[len(times)-1].Sub(times[0])
		log.Printf("✅ Треугольник %s/%s/%s собран за %v",
			t.A, t.B, t.C, delta.Truncate(time.Millisecond),
		)
	}
}

func runBot(logFile *os.File) error {
	triangles, err := loadTriangles()
	if err != nil {
		return err
	}
	valid := fetchAvailableSymbols()
	channels := buildChannels(triangles, valid)
	for _, ch := range channels {
		log.Println("📡 Subscribe:", ch)
	}

	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	// pong handler
	var lastPing time.Time
	conn.SetPongHandler(func(string) error {
		d := time.Since(lastPing)
		log.Printf("📶 Pong received after %v", d)
		return nil
	})

	// subscribe
	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": channels,
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return err
	}
	log.Println("✅ Subscription sent")

	// JSONL writer
	enc := json.NewEncoder(logFile)

	// ping ticker
	pingT := time.NewTicker(15 * time.Second)
	defer pingT.Stop()
	go func() {
		for range pingT.C {
			lastPing = time.Now()
			if err := conn.WriteMessage(websocket.PingMessage, []byte("hb")); err != nil {
				log.Println("⚠️ Ping error:", err)
			}
		}
	}()

	// check timings ticker
	checkT := time.NewTicker(5 * time.Second)
	defer checkT.Stop()
	go func() {
		for range checkT.C {
			checkTriangleTimings(triangles)
		}
	}()

	for {
		_, raw, err := conn.ReadMessage()
		if err != nil {
			log.Println("⚠️ Read error:", err)
			return err
		}

		// 1) Детальное логирование «сырых» месседжей
		log.Printf("🔴 RAW MSG: %s\n", string(raw))

		// 2) Парсим во «внешнюю» обёртку
		var wrapper struct {
			Method string     `json:"method"`
			Params []DealsMsg `json:"params"`
		}
		if err := json.Unmarshal(raw, &wrapper); err != nil {
			log.Println("⚠️ JSON wrapper unmarshal failed:", err)
			continue
		}
		if wrapper.Method != "push.deals.v3" || len(wrapper.Params) == 0 {
			log.Printf("⚠️ skipping, method=%s\n", wrapper.Method)
			continue
		}

		// 3) Вытаскиваем реальный payload
		m := wrapper.Params[0]
		if m.Symbol == "" || len(m.Data) == 0 {
			log.Println("⚠️ empty Params[0]:", m)
			continue
		}

		// 4) Обрабатываем тики
		now := time.Now()
		side, price := m.Data[0].Side, m.Data[0].Price

		priceLock.Lock()
		lastUpdate[m.Symbol] = now
		lastUpdate[reversePair(m.Symbol)] = now
		priceLock.Unlock()

		logPriceUpdate(m.Symbol, side, price)

		// 5) Dump JSONL
		enc.Encode(map[string]interface{}{
			"symbol": m.Symbol,
			"side":   side,
			"price":  price,
			"time":   now.Format(time.RFC3339Nano),
		})
	}
}

func main() {
	log.SetOutput(os.Stdout)
	f, err := os.OpenFile("prices_log.jsonl", os.O_CREATE|os.O_APPEND|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	for {
		if err := runBot(f); err != nil {
			log.Printf("🔄 Reconnect in 5s: %v", err)
			time.Sleep(5 * time.Second)
		}
	}
}



2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 ⚠️ Пара BTCXRP ещё не получила ни одного тика
2025/07/22 23:02:53 ⚠️ Пара BTCETH ещё не получила ни одного тика
2025/07/22 23:02:53 ⚠️ Пара BTCTRX ещё не получила ни одного тика
2025/07/22 23:02:53 ⚠️ Пара USDTADA ещё не получила ни одного тика
2025/07/22 23:02:53 ⚠️ Пара SOLBTC ещё не получила ни одного тика
2025/07/22 23:02:53 ⚠️ Пара USDTXRP ещё не получила ни одного тика
2025/07/22 23:02:53 ⚠️ Пара BTCXRP ещё не получила ни одного тика
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.03620555","S":1,"t":1753214573415}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214573417}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.96","v":"0.18","S":1,"t":1753214573416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214573420}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.03430052","S":2,"t":1753214573417}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214573420}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.96","v":"0.65","S":1,"t":1753214573417}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214573420}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.96","v":"1.05","S":2,"t":1753214573417}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214573420}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.01588527","S":1,"t":1753214573418}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214573420}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.80","v":"2.35796","S":1,"t":1753214573455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214573458}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.80","v":"3.28673","S":2,"t":1753214573455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214573459}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030777","v":"0.013","S":2,"t":1753214573542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573544}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030784","v":"0.014","S":2,"t":1753214573542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573544}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030781","v":"0.002","S":2,"t":1753214573542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573544}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030773","v":"0.004","S":2,"t":1753214573542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573544}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030778","v":"0.009","S":1,"t":1753214573542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573544}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030775","v":"0.003","S":1,"t":1753214573542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573544}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030784","v":"0.008","S":1,"t":1753214573543}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573547}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030786","v":"0.014","S":2,"t":1753214573543}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214573548}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 📶 Pong received after 262.366173ms
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.80","v":"0.14611","S":2,"t":1753214573581}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214573584}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573749}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573752}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573749}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573752}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573751}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573751}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573751}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573752}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573752}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573753}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573753}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573755}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573754}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573759}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573756}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573759}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573756}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573759}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573759}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573763}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573760}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573763}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5347","v":"0.40","S":2,"t":1753214573765}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214573768}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:53 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009622","v":"0.60","S":2,"t":1753214573838}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214573840}
2025/07/22 23:02:53 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00009204","S":2,"t":1753214573916}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214573919}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00024807","S":2,"t":1753214573916}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214573919}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00011584","S":1,"t":1753214573918}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214573920}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.99","v":"1.60","S":1,"t":1753214573923}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214573926}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"199.01","v":"24.36","S":1,"t":1753214573923}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214573926}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5346","v":"1.30","S":2,"t":1753214574007}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214574009}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029623","v":"1.22","S":2,"t":1753214574029}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214574033}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030785","v":"0.001","S":1,"t":1753214574042}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574045}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030778","v":"0.001","S":1,"t":1753214574042}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574045}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.82","v":"0.17913","S":2,"t":1753214574453}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214574455}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.81","v":"1.34131","S":1,"t":1753214574454}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214574458}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.82","v":"2.23530","S":1,"t":1753214574455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214574458}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.82","v":"2.44390","S":2,"t":1753214574455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214574458}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.81","v":"0.26853","S":1,"t":1753214574456}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214574458}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.82","v":"0.47712","S":1,"t":1753214574456}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214574458}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030773","v":"0.009","S":2,"t":1753214574542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574544}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030774","v":"0.012","S":2,"t":1753214574543}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574547}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030775","v":"0.018","S":1,"t":1753214574544}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574547}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030787","v":"0.015","S":2,"t":1753214574544}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574547}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030779","v":"0.017","S":1,"t":1753214574545}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574547}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030780","v":"0.012","S":1,"t":1753214574556}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214574560}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:54 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADAUSDT","d":{"deals":[{"p":"0.8783","v":"8.70","S":1,"t":1753214574605}],"e":"spot@public.deals.v3.api"},"s":"ADAUSDT","t":1753214574608}
2025/07/22 23:02:54 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.00003604","S":1,"t":1753214574916}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214574918}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5340","v":"9.06","S":2,"t":1753214575012}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575014}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5340","v":"7.15","S":2,"t":1753214575012}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575014}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADAUSDT","d":{"deals":[{"p":"0.8784","v":"6.84","S":2,"t":1753214575104}],"e":"spot@public.deals.v3.api"},"s":"ADAUSDT","t":1753214575107}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADABTC","d":{"deals":[{"p":"0.000007364","v":"2.331","S":1,"t":1753214575159}],"e":"spot@public.deals.v3.api"},"s":"ADABTC","t":1753214575163}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5338","v":"0.37","S":2,"t":1753214575168}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575171}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.14","v":"0.00068","S":1,"t":1753214575334}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575336}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009623","v":"0.44","S":1,"t":1753214575338}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575340}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009621","v":"0.68","S":1,"t":1753214575338}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575341}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009619","v":"0.34","S":2,"t":1753214575338}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575341}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009617","v":"0.21","S":2,"t":1753214575338}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575341}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00008192","S":1,"t":1753214575415}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575417}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.91","v":"2.50","S":2,"t":1753214575415}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575418}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.91","v":"1.91","S":1,"t":1753214575415}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575418}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.95","v":"1.81","S":2,"t":1753214575416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575418}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00010403","S":2,"t":1753214575416}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575420}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00019801","S":2,"t":1753214575418}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575420}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.91","v":"2.87","S":1,"t":1753214575416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575419}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.91","v":"1.50","S":1,"t":1753214575416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575419}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.91","v":"2.12","S":1,"t":1753214575416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575419}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.13","v":"0.03112","S":2,"t":1753214575454}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575458}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.13","v":"0.01572","S":2,"t":1753214575454}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575459}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.13","v":"0.02962","S":1,"t":1753214575454}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575459}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.13","v":"0.01422","S":1,"t":1753214575455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575459}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.12","v":"0.02624","S":2,"t":1753214575456}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575460}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5331","v":"1.86","S":1,"t":1753214575511}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575513}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5331","v":"75.50","S":2,"t":1753214575511}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575513}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5331","v":"26.92","S":1,"t":1753214575511}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575513}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5331","v":"23.12","S":2,"t":1753214575512}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214575516}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029622","v":"1.44","S":1,"t":1753214575530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575534}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029618","v":"0.44","S":2,"t":1753214575530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575534}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029619","v":"0.78","S":2,"t":1753214575530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575534}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029618","v":"1.04","S":1,"t":1753214575530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575534}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029618","v":"0.83","S":1,"t":1753214575530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575535}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029619","v":"0.55","S":1,"t":1753214575530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575535}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029618","v":"0.93","S":2,"t":1753214575531}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575535}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029621","v":"0.95","S":1,"t":1753214575531}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575535}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029618","v":"0.65","S":1,"t":1753214575531}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575535}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029621","v":"1.70","S":2,"t":1753214575532}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214575535}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADAUSDT","d":{"deals":[{"p":"0.8782","v":"20.49","S":2,"t":1753214575603}],"e":"spot@public.deals.v3.api"},"s":"ADAUSDT","t":1753214575606}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADAUSDT","d":{"deals":[{"p":"0.8782","v":"7.30","S":2,"t":1753214575605}],"e":"spot@public.deals.v3.api"},"s":"ADAUSDT","t":1753214575609}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADAUSDT","d":{"deals":[{"p":"0.8782","v":"10.60","S":2,"t":1753214575607}],"e":"spot@public.deals.v3.api"},"s":"ADAUSDT","t":1753214575609}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:55 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ADABTC","d":{"deals":[{"p":"0.000007367","v":"5.759","S":2,"t":1753214575661}],"e":"spot@public.deals.v3.api"},"s":"ADABTC","t":1753214575665}
2025/07/22 23:02:55 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009622","v":"3.65","S":2,"t":1753214575838}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575841}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009624","v":"0.51","S":2,"t":1753214575838}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575841}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009611","v":"1.33","S":1,"t":1753214575839}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575842}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009613","v":"3.82","S":1,"t":1753214575839}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575842}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009625","v":"3.43","S":1,"t":1753214575839}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214575842}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.92","v":"0.08","S":1,"t":1753214575916}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214575918}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.00045871","S":2,"t":1753214575927}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575932}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.00275182","S":2,"t":1753214575928}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575932}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.00389836","S":1,"t":1753214575930}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575932}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.18","v":"0.00364912","S":1,"t":1753214575930}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214575935}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.19","v":"0.05865","S":2,"t":1753214575955}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575964}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.19","v":"0.78758","S":2,"t":1753214575955}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214575964}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5333","v":"3.00","S":2,"t":1753214576011}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214576014}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030773","v":"0.002","S":2,"t":1753214576042}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576044}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030778","v":"0.001","S":2,"t":1753214576042}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576044}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030787","v":"0.002","S":2,"t":1753214576042}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576044}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030771","v":"0.004","S":1,"t":1753214576042}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576045}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030776","v":"0.001","S":1,"t":1753214576043}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576048}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5337","v":"14.83","S":1,"t":1753214576086}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214576089}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009633","v":"0.81","S":2,"t":1753214576338}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214576340}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPETH","d":{"deals":[{"p":"0.0009617","v":"0.97","S":1,"t":1753214576339}],"e":"spot@public.deals.v3.api"},"s":"XRPETH","t":1753214576342}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.93","v":"1.86","S":1,"t":1753214576415}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214576419}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00006100","S":1,"t":1753214576415}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214576419}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00045153","S":1,"t":1753214576417}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214576420}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@BTCUSDT","d":{"deals":[{"p":"119309.19","v":"0.00006949","S":2,"t":1753214576417}],"e":"spot@public.deals.v3.api"},"s":"BTCUSDT","t":1753214576420}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.93","v":"1.92","S":1,"t":1753214576416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214576420}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"198.93","v":"1.88","S":1,"t":1753214576416}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753214576420}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.45","v":"0.21238","S":1,"t":1753214576455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214576457}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.45","v":"0.01664","S":2,"t":1753214576455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214576457}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHUSDT","d":{"deals":[{"p":"3670.45","v":"0.12539","S":1,"t":1753214576455}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDT","t":1753214576457}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5333","v":"225.72","S":2,"t":1753214576511}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214576513}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPUSDT","d":{"deals":[{"p":"3.5333","v":"191.22","S":1,"t":1753214576511}],"e":"spot@public.deals.v3.api"},"s":"XRPUSDT","t":1753214576513}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@XRPBTC","d":{"deals":[{"p":"0.000029618","v":"1.78","S":1,"t":1753214576530}],"e":"spot@public.deals.v3.api"},"s":"XRPBTC","t":1753214576534}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030780","v":"0.001","S":2,"t":1753214576542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576545}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030771","v":"0.001","S":2,"t":1753214576542}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576545}
2025/07/22 23:02:56 ⚠️ skipping, method=
2025/07/22 23:02:56 🔴 RAW MSG: {"c":"spot@public.deals.v3.api@ETHBTC","d":{"deals":[{"p":"0.030780","v":"0.001","S":2,"t":1753214576543}],"e":"spot@public.deals.v3.api"},"s":"ETHBTC","t":1753214576546}







