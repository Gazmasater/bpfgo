
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


// internal/domain/exchange/exchange.go
package exchange

import "time"

type Tick struct {
    Symbol string   // "BTCUSDT" без дефиса
    Price  float64
    Ts     time.Time
    Src    string   // "MEXC" или "OKX" — если захочешь логировать источник
}

type Exchange interface {
    Name() string
    FetchAvailableSymbols() (map[string]bool, map[string]float64, map[string]float64)
    SubscribeTicks(pairs []string, onTick func(Tick)) error
    PlaceMarketOrder(symbol, side string, quantity float64) (string, error)
    GetBestAsk(symbol string) (float64, error)
    GetBestBid(symbol string) (float64, error)
}



2) app больше не обрабатывает raw
go
Копировать
Редактировать
// internal/app/app.go (фрагменты)
type Arbitrager struct {
    Triangles       []triangle.Triangle
    latest          map[string]float64
    trianglesByPair map[string][]int
    realSymbols     map[string]bool
    mu              sync.Mutex
    StartAmount     float64
}

func New(ex exchange.Exchange) (*Arbitrager, error) {
    rawSymbols, _, _ := ex.FetchAvailableSymbols()
    avail := filesystem.ExpandAvailableSymbols(rawSymbols)

    a := &Arbitrager{
        latest:          make(map[string]float64, len(avail)),
        trianglesByPair: make(map[string][]int),
        realSymbols:     avail, // true для обеих направлений
        StartAmount:     100,
    }

    // строим треугольники сразу из avail (с учётом blocked_pairs.log, если нужно)
    tris, err := filesystem.LoadTrianglesFromSymbols(rawSymbols)
    if err != nil { return nil, err }
    a.Triangles = triangle.Filter(tris, avail)

    // Индексация треугольников по парам
    for i, t := range a.Triangles {
        for _, p := range [][2]string{{t.A,t.B},{t.B,t.C},{t.C,t.A}} {
            ab, ok, _ := a.normalizeSymbolDir(p[0], p[1])
            if ok {
                a.trianglesByPair[ab] = append(a.trianglesByPair[ab], i)
            }
        }
    }

    // Подписываемся только на нужные пары
    pairs := triangle.SymbolPairs(a.Triangles)
    go func() {
        _ = ex.SubscribeTicks(pairs, func(t exchange.Tick) { a.handleTick(t) })
    }()

    return a, nil
}

func (a *Arbitrager) handleTick(t exchange.Tick) {
    a.mu.Lock()
    a.latest[t.Symbol] = t.Price
    a.mu.Unlock()
    a.Check(t.Symbol)
}
Обрати внимание: testWsSubscription и HandleRaw больше не нужны — MEXC/OKX сами парсят свои сообщения и отдают Tick.

3) MEXC: парсим и вызываем onTick
go
Копировать
Редактировать
// internal/repository/mexc/mexc.go (фрагменты)
func (m *MexcExchange) SubscribeTicks(pairs []string, onTick func(exchange.Tick)) error {
    const wsURL = "wss://wbs.mexc.com/ws"
    // ... подключение, пинг как у тебя ...

    for {
        _, raw, err := conn.ReadMessage()
        if err != nil { /* реконнект */ }

        // Быстрый дешифр без аллокаций можно оставить,
        // но проще — минимальный unmarshal:
        var msg struct {
            S string `json:"s"` // symbol
            P string `json:"p"` // price (string)
        }
        if json.Unmarshal(raw, &msg) == nil && msg.S != "" && msg.P != "" {
            if px, err := strconv.ParseFloat(msg.P, 64); err == nil {
                onTick(exchange.Tick{Symbol: strings.ToUpper(msg.S), Price: px, Ts: time.Now(), Src: m.Name()})
            }
            continue
        }

        // обработка служебных ответов/ошибок — по желанию
    }
}
Твой старый SubscribeDeals почти готов — просто замени handler("MEXC", raw) на распарсить → onTick(...).

4) OKX: подписка и нормализация символов
В FetchAvailableSymbols уже норм — только разбери lotSz в float64, не ставь константу.

Для WS лучше канал trades или tickers. Для простоты — tickers:

go
Копировать
Редактировать
// internal/repository/okx/okx.go (фрагменты)
func okxToSym(instId string) string { // "BTC-USDT" -> "BTCUSDT"
    return strings.ReplaceAll(instId, "-", "")
}

func symToOKX(symbol string) string { // "BTCUSDT" -> "BTC-USDT" (через известные котировки)
    base, quote := filesystem.UnpackPair(symbol) // сделай Exported: UnpackPair
    if base == "" || quote == "" { return symbol }
    return base + "-" + quote
}

func (o *OKXExchange) SubscribeTicks(pairs []string, onTick func(exchange.Tick)) error {
    c, _, err := websocket.DefaultDialer.Dial("wss://ws.okx.com:8443/ws/v5/public", nil)
    if err != nil { return err }

    args := make([]map[string]string, 0, len(pairs))
    for _, p := range pairs {
        args = append(args, map[string]string{
            "channel": "tickers",
            "instId":  symToOKX(p),
        })
    }
    sub := map[string]any{"op": "subscribe", "args": args}
    if err := c.WriteJSON(sub); err != nil { return err }

    go func() {
        for {
            _, raw, err := c.ReadMessage()
            if err != nil { log.Printf("OKX read err: %v", err); time.Sleep(time.Second); continue }

            // Пример ответа: { "arg":{...}, "data":[{"instId":"BTC-USDT","last":"..."}] }
            var r struct {
                Arg  struct{ InstID string `json:"instId"` } `json:"arg"`
                Data []struct {
                    InstID string `json:"instId"`
                    Last   string `json:"last"`
                    Ts     string `json:"ts"`
                } `json:"data"`
            }
            if json.Unmarshal(raw, &r) == nil && len(r.Data) > 0 {
                d := r.Data[0]
                if px, err := strconv.ParseFloat(d.Last, 64); err == nil {
                    onTick(exchange.Tick{
                        Symbol: okxToSym(d.InstID),
                        Price:  px,
                        Ts:     time.Now(),
                        Src:    o.Name(),
                    })
                }
            }
        }
    }()
    return nil
}
И в FetchAvailableSymbols:

go
Копировать
Редактировать
for _, item := range data.Data {
    sym := okxToSym(item.InstID)
    if sym == "" { continue }
    avail[sym] = true
    if v, err := strconv.ParseFloat(item.LotSz, 64); err == nil && v > 0 {
        step[sym] = v
        minQty[sym] = v // минимумом можно считать lot size по умолчанию
    }
}
5) Выбор биржи в main
go
Копировать
Редактировать
exch := strings.ToUpper(os.Getenv("EXCHANGE")) // "MEXC" или "OKX"
var ex exchange.Exchange
switch exch {
case "MEXC":
    ex = mexc.NewMexcExchange(apiKey, secret)
case "OKX":
    ex = okx.NewOKXExchange()
default:
    log.Fatalf("Неизвестная биржа EXCHANGE=%q", exch)
}

_, err = app.New(ex)
if err != nil { log.Fatal(err) }
select {}






