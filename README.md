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


Я разбил бота по принципам чистой архитектуры на три слоя:

go
Копировать код
cmd/cryptarb/main.go
internal/
  app/
    arbitrage.go
  domain/
    triangle/
      triangle.go
  repository/
    filesystem/
      loader.go
    mexc/
      client.go
      ws.go
Вот полный код:

internal/repository/filesystem/loader.go
go
Копировать код
package filesystem

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
)

type Triangle struct{ A, B, C string }

func LoadTriangles(path string) ([]Triangle, error) {
	// если файла нет — запишем дефолт
	t := []Triangle{
		{"XRP", "BTC", "USDT"}, {"ETH", "BTC", "USDT"},
		{"TRX", "BTC", "USDT"}, {"ADA", "USDT", "BTC"},
		{"BTC", "SOL", "USDT"}, {"XRP", "USDT", "ETH"},
		{"XRP", "BTC", "ETH"}, {"LTC", "BTC", "USDT"},
		{"DOGE", "BTC", "USDT"}, {"MATIC", "USDT", "BTC"},
		{"DOT", "BTC", "USDT"}, {"AVAX", "BTC", "USDT"},
		{"BCH", "BTC", "USDT"}, {"LINK", "BTC", "USDT"},
		{"ETC", "BTC", "USDT"},
	}
	b, _ := json.MarshalIndent(t, "", "  ")
	_ = ioutil.WriteFile(path, b, 0644)

	b, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	var ts []Triangle
	if err := json.Unmarshal(b, &ts); err != nil {
		return nil, fmt.Errorf("unmarshal %s: %w", path, err)
	}
	return ts, nil
}
internal/repository/mexc/client.go
go
Копировать код
package mexc

import (
	"encoding/json"
	"io/ioutil"
	"net/http"
)

type SymbolInfo struct{ Symbol string }
type ExchangeInfo struct{ Symbols []SymbolInfo }

// FetchAvailableSymbols возвращает карту доступных торговых пар
func FetchAvailableSymbols() map[string]bool {
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
internal/repository/mexc/ws.go
go
Копировать код
package mexc

import (
	"log"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

// ListenWS запускает подключение к WS и передаёт каждый raw-сообщение в handler
func ListenWS(channels []string, handler func(raw []byte)) error {
	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	// подписка
	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": channels,
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return err
	}

	// пинг
	var mu sync.Mutex
	var lastPing time.Time
	conn.SetPongHandler(func(string) error {
		log.Printf("📶 Pong after %v\n", time.Since(lastPing))
		return nil
	})
	go func() {
		t := time.NewTicker(60 * time.Second)
		defer t.Stop()
		for range t.C {
			mu.Lock()
			lastPing = time.Now()
			_ = conn.WriteMessage(websocket.PingMessage, []byte("hb"))
			mu.Unlock()
		}
	}()

	// чтение
	for {
		_, raw, err := conn.ReadMessage()
		if err != nil {
			return err
		}
		handler(raw)
	}
}
internal/domain/triangle/triangle.go
go
Копировать код
package triangle

import (
	"fmt"
	"sort"
)

type Triangle struct{ A, B, C string }

// Filter оставляет только те треугольники, пары в которых есть в available
func Filter(ts []Triangle, available map[string]bool) []Triangle {
	out := make([]Triangle, 0, len(ts))
	for _, t := range ts {
		ok := func(a, b string) bool {
			return available[a+b] || available[b+a]
		}
		if ok(t.A, t.B) && ok(t.B, t.C) && ok(t.A, t.C) {
			out = append(out, t)
		}
	}
	return out
}

// BuildChannels возвращает WS-каналы для сделок
func BuildChannels(ts []Triangle) []string {
	set := map[string]struct{}{}
	for _, t := range ts {
		for _, pr := range [][2]string{{t.A, t.B}, {t.B, t.C}, {t.A, t.C}} {
			set[pr[0]+pr[1]] = struct{}{}
		}
	}
	list := make([]string, 0, len(set))
	for k := range set {
		list = append(list, fmt.Sprintf("spot@public.deals.v3.api@%s", k))
	}
	sort.Strings(list)
	return list
}
internal/app/arbitrage.go
go
Копировать код
package app

import (
	"encoding/json"
	"log"
	"strconv"
	"sync"
	"time"

	"github.com/Gazmasater/cryp_arbtryang/internal/domain/triangle"
	"github.com/Gazmasater/cryp_arbtryang/internal/repository/filesystem"
	"github.com/Gazmasater/cryp_arbtryang/internal/repository/mexc"
)

type Arbitrager struct {
	triangles []triangle.Triangle
	latest    map[string]float64
	mu        sync.Mutex
}

func New(dataPath string) (*Arbitrager, error) {
	// 1) Загружаем все треугольники из FS
	ts, err := filesystem.LoadTriangles(dataPath)
	if err != nil {
		return nil, err
	}
	// 2) Фильтруем по доступным парам с API
	avail := mexc.FetchAvailableSymbols()
	ts = triangle.Filter(ts, avail)

	return &Arbitrager{
		triangles: ts,
		latest:    make(map[string]float64, len(avail)),
	}, nil
}

func (a *Arbitrager) HandleRaw(raw []byte) {
	// парсим, кладём в a.latest
	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		return
	}
	if msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		return
	}
	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		return
	}
	a.mu.Lock()
	a.latest[msg.Symbol] = price
	a.latest[msg.Symbol[len(msg.Symbol)/2:]+msg.Symbol[:len(msg.Symbol)/2]] = 1 / price
	a.mu.Unlock()
}

func (a *Arbitrager) CheckLoop() {
	t := time.NewTicker(5 * time.Second)
	defer t.Stop()
	for range t.C {
		a.mu.Lock()
		for _, tri := range a.triangles {
			ab, bc, ac := tri.A+tri.B, tri.B+tri.C, tri.A+tri.C
			p1, ok1 := a.latest[ab]; p2, ok2 := a.latest[bc]; p3, ok3 := a.latest[ac]
			if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
				continue
			}
			commission := 0.001
			nf := (1-commission)*(1-commission)*(1-commission)
			profit := (p1*p2/p3*nf - 1) * 100
			if profit > 0.2 {
				log.Printf("🔺 %s/%s/%s profit %.3f%%", tri.A, tri.B, tri.C, profit)
			}
		}
		a.mu.Unlock()
	}
}
cmd/cryptarb/main.go
go
Копировать код
package main

import (
	"log"
	"os"

	"github.com/Gazmasater/cryp_arbtryang/internal/app"
	"github.com/Gazmasater/cryp_arbtryang/internal/domain/triangle"
	"github.com/Gazmasater/cryp_arbtryang/internal/repository/mexc"
)

func main() {
	arb, err := app.New("triangles.json")
	if err != nil {
		log.Fatal(err)
	}

	channels := triangle.BuildChannels(arb.triangles)
	// WS.Listen — передаём raw-сообщения в HandleRaw
	go func() {
		if err := mexc.ListenWS(channels, arb.HandleRaw); err != nil {
			log.Fatal(err)
		}
	}()

	// Запускаем цикл проверки арбитража
	go arb.CheckLoop()

	// Блокируем main (например, ждем SIGINT)
	select {}
}
Что мы получили:

Полное разделение:

repository — работа с файлы/HTTP/WS.

domain/triangle — чистая бизнес-логика про треугольники.

app — оркестратор: загружает, передаёт сообщения, считает профит.

Никаких глобальных latest, loadTriangles и т.п. в main — всё импортируется.

cmd/cryptarb — точка входа, где мы только “собираем” все зависимости.

С этим layout’ом IDE перестанет ругаться на undefined, и код будет легко поддерживать и тестировать.






Источники

Спросить ChatGPT







