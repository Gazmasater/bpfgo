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


// Go-бот: 7 треугольников + deals + ping-pong + профит треугольников
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

var (
	priceLock  sync.Mutex
	latest     = map[string]float64{}
	lastUpdate = map[string]time.Time{}
)

func ensureTrianglesFile() error {
	t := []Triangle{
		{"XRP", "BTC", "USDT"},
		{"ETH", "BTC", "USDT"},
		{"TRX", "BTC", "USDT"},
		{"ADA", "USDT", "BTC"},
		{"BTC", "SOL", "USDT"},
		{"XRP", "USDT", "ETH"},
		{"XRP", "BTC", "ETH"},
	}
	b, _ := json.MarshalIndent(t, "", "  ")
	return ioutil.WriteFile("triangles.json", b, 0644)
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
	out := make(map[string]bool)
	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		return out
	}
	defer resp.Body.Close()
	b, _ := ioutil.ReadAll(resp.Body)
	var info ExchangeInfo
	if err := json.Unmarshal(b, &info); err != nil {
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
			case valid[p2]:
				set[p2] = true
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

func findActualPairKey(a, b string) string {
	k := a + b
	if _, ok := latest[k]; ok {
		return k
	}
	return b + a
}

func checkTriangleProfit(triangles []Triangle) {
	priceLock.Lock()
	defer priceLock.Unlock()

	for _, t := range triangles {
		ab := findActualPairKey(t.A, t.B)
		bc := findActualPairKey(t.B, t.C)
		ac := findActualPairKey(t.A, t.C)
		
		p1, ok1 := latest[ab]
		p2, ok2 := latest[bc]
		p3, ok3 := latest[ac]
		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}
		
		profit := (p1 * p2 / p3 - 1) * 100
		log.Printf("🔺 %s/%s/%s | profit: %.3f%%", t.A, t.B, t.C, profit)
	}
}

func runBot(logFile *os.File) error {
	triangles, err := loadTriangles()
	if err != nil {
		return err
	}
	valid := fetchAvailableSymbols()
	channels := buildChannels(triangles, valid)

	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	var lastPing time.Time
	conn.SetPongHandler(func(string) error {
		d := time.Since(lastPing)
		log.Printf("📶 Pong after %v", d)
		return nil
	})

	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": channels,
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return err
	}

	enc := json.NewEncoder(logFile)
	pingT := time.NewTicker(15 * time.Second)
	defer pingT.Stop()
	go func() {
		for range pingT.C {
			lastPing = time.Now()
			conn.WriteMessage(websocket.PingMessage, []byte("hb"))
		}
	}()

	checkT := time.NewTicker(5 * time.Second)
	defer checkT.Stop()
	go func() {
		for range checkT.C {
			checkTriangleProfit(triangles)
		}
	}()

	for {
		_, raw, err := conn.ReadMessage()
		if err != nil {
			return err
		}
		var outer struct {
			Channel string `json:"c"`
			Data    struct {
				Deals []struct {
					Price     string `json:"p"`
					Side      int    `json:"S"`
				} `json:"deals"`
			} `json:"d"`
			Symbol string `json:"s"`
		}
		if err := json.Unmarshal(raw, &outer); err != nil {
			continue
		}
		if outer.Symbol == "" || len(outer.Data.Deals) == 0 {
			continue
		}

		price, err := strconv.ParseFloat(outer.Data.Deals[0].Price, 64)
		if err != nil {
			continue
		}

		priceLock.Lock()
		latest[outer.Symbol] = price
		latest[reversePair(outer.Symbol)] = 1 / price
		priceLock.Unlock()

		enc.Encode(map[string]interface{}{
			"symbol": outer.Symbol,
			"price":  price,
			"time":   time.Now().Format(time.RFC3339Nano),
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


[{
	"resource": "/home/gaz358/myprog/crypt/main.go",
	"owner": "go-staticcheck",
	"severity": 4,
	"message": "var lastUpdate is unused (U1000)",
	"source": "go-staticcheck",
	"startLineNumber": 27,
	"startColumn": 2,
	"endLineNumber": 27,
	"endColumn": 37,
	"origin": "extHost1"
}]










