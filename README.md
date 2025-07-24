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



Перенёс type Triangle из filesystem в пакет domain/triangle, чтобы он был единым для всей системы, и убрал аналогичную структуру из filesystem/loader.go.

В filesystem/loader.go теперь возвращается []triangle.Triangle вместо своего локального типа.

В app.Arbitrager поле triangles я экспортировал под именем Triangles и поменял конструктор на приём []triangle.Triangle.

До

бавил у Arbitrager метод Channels() []string, чтобы в main не лезть в неэкспортированные поля.


package main

import (
	"cryptarb/internal/app"
	"cryptarb/internal/repository/mexc"
	"log"
)

func main() {
	arb, err := app.New("triangles.json")
	if err != nil {
		log.Fatal(err)
	}

	// 1) Запускаем WS-подписку
	go func() {
		if err := mexc.ListenWS(arb.Channels(), arb.HandleRaw); err != nil {
			log.Fatal(err)
		}
	}()

	// 2) Цикл поиска профитов
	arb.CheckLoop()
}

package app

import (
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
	"cryptarb/internal/repository/mexc"
	"encoding/json"
	"log"
	"strconv"
	"sync"
	"time"
)

type Arbitrager struct {
	Triangles []triangle.Triangle // ЭКСПОРТИРОВАНО
	latest    map[string]float64
	mu        sync.Mutex
}

func New(dataPath string) (*Arbitrager, error) {
	// 1) Загружаем все треугольники
	ts, err := filesystem.LoadTriangles(dataPath)
	if err != nil {
		return nil, err
	}
	// 2) Фильтруем по доступным парам
	avail := mexc.FetchAvailableSymbols()
	ts = triangle.Filter(ts, avail)

	return &Arbitrager{
		Triangles: ts,
		latest:    make(map[string]float64),
	}, nil
}

// Channels отдаёт WS-каналы на основе отфильтрованных треугольников
func (a *Arbitrager) Channels() []string {
	return triangle.BuildChannels(a.Triangles)
}

func (a *Arbitrager) HandleRaw(raw []byte) {
	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if json.Unmarshal(raw, &msg) != nil || msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		return
	}
	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		return
	}

	a.mu.Lock()
	a.latest[msg.Symbol] = price
	rev := msg.Symbol[len(msg.Symbol)/2:] + msg.Symbol[:len(msg.Symbol)/2]
	a.latest[rev] = 1 / price
	a.mu.Unlock()
}

func (a *Arbitrager) CheckLoop() {
	var sum float64 // ← обнуляется один раз при запуске
	t := time.NewTicker(5 * time.Second)
	defer t.Stop()

	for range t.C {
		a.mu.Lock()
		for _, tri := range a.Triangles {
			ab := tri.A + tri.B
			bc := tri.B + tri.C
			ac := tri.A + tri.C

			p1, ok1 := a.latest[ab]
			p2, ok2 := a.latest[bc]
			p3, ok3 := a.latest[ac]

			if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
				continue
			}

			const commission = 0.001
			const minProfitPercent = 0.0

			nf := (1 - commission) * (1 - commission) * (1 - commission)
			profit := (p1*p2/p3*nf - 1) * 100

			if profit > minProfitPercent {
				sum += profit // ← суммируется каждый положительный profit
				log.Printf("🔺 %s/%s/%s profit %.3f%% sum=%.3f", tri.A, tri.B, tri.C, profit, sum)
			}
		}
		a.mu.Unlock()
	}
}


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


package filesystem

import (
	"cryptarb/internal/domain/triangle"
	"encoding/json"
	"fmt"
	"io"
	"os"
)

func LoadTriangles(path string) ([]triangle.Triangle, error) {
	// дефолтные треугольники
	t := []triangle.Triangle{
		{A: "XRP", B: "BTC", C: "USDT"},
		{A: "ETH", B: "BTC", C: "USDT"},
		{A: "TRX", B: "BTC", C: "USDT"},
		{A: "ADA", B: "USDT", C: "BTC"},
		{A: "BTC", B: "SOL", C: "USDT"},
		{A: "XRP", B: "USDT", C: "ETH"},
		{A: "XRP", B: "BTC", C: "ETH"},
		{A: "LTC", B: "BTC", C: "USDT"},
		{A: "DOGE", B: "BTC", C: "USDT"},
		{A: "MATIC", B: "USDT", C: "BTC"},
		{A: "DOT", B: "BTC", C: "USDT"},
		{A: "AVAX", B: "BTC", C: "USDT"},
		{A: "BCH", B: "BTC", C: "USDT"},
		{A: "LINK", B: "BTC", C: "USDT"},
		{A: "ETC", B: "BTC", C: "USDT"},
	}

	// сериализуем и записываем в файл, если он не существует
	if _, err := os.Stat(path); os.IsNotExist(err) {
		b, _ := json.MarshalIndent(t, "", "  ")
		_ = os.WriteFile(path, b, 0644)
	}

	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	b, err := io.ReadAll(f)
	if err != nil {
		return nil, err
	}

	var ts []triangle.Triangle
	if err := json.Unmarshal(b, &ts); err != nil {
		return nil, fmt.Errorf("unmarshal %s: %w", path, err)
	}
	return ts, nil
}


package mexc

import (
	"encoding/json"
	"io"
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

	b, err := io.ReadAll(resp.Body)
	if err != nil {
		return out
	}

	var info ExchangeInfo
	if err := json.Unmarshal(b, &info); err != nil {
		return out
	}

	for _, s := range info.Symbols {
		out[s.Symbol] = true
	}
	return out
}

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

	var mu sync.Mutex
	var lastPing time.Time
	conn.SetPongHandler(func(string) error {
		log.Printf("📶 Pong after %v\n", time.Since(lastPing))
		return nil
	})
	go func() {
		t := time.NewTicker(15 * time.Second)
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


			









