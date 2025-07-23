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


Ниже пример, как тот же бот можно переехать на «чистую» (hexagonal/абстракционно-слойную) архитектуру. Структура проекта:

go
Копировать код
├── cmd
│   └── mexc-arb
│       └── main.go
├── internal
│   ├── app
│   │   └── arbitrage.go
│   ├── domain
│   │   ├── model.go
│   │   └── service.go
│   └── infra
│       ├── mxclient.go
│       └── wsrepo.go
├── triangles.json
└── go.mod
domain/model.go
go
Копировать код
package domain

// Triangle — три валюты для арбитража.
type Triangle struct{ A, B, C string }

// PriceRepo описывает источник цен.
type PriceRepo interface {
    Subscribe(channels []string) (<-chan PriceUpdate, error)
    PingLoop()
}

// HTTPClient описывает минимальный HTTP-интерфейс.
type HTTPClient interface {
    Get(url string) ([]byte, error)
}

// PriceUpdate — обновление цены для пары.
type PriceUpdate struct {
    Symbol string
    Price  float64
}
domain/service.go
go
Копировать код
package domain

import "sync"

// Arbitrager считает арбитраж.
type Arbitrager struct {
    repo       PriceRepo
    triangles  []Triangle
    prices     map[string]float64
    lock       sync.RWMutex
    commission float64
    minProfit  float64
}

// NewArbitrager создаёт новый экземпляр.
func NewArbitrager(repo PriceRepo, triangles []Triangle, commission, minProfit float64) *Arbitrager {
    return &Arbitrager{
        repo:       repo,
        triangles:  triangles,
        prices:     make(map[string]float64),
        commission: commission,
        minProfit:  minProfit,
    }
}

// Start запускает подписку и вычисления.
func (a *Arbitrager) Start() error {
    channels := buildChannels(a.triangles)
    updates, err := a.repo.Subscribe(channels)
    if err != nil {
        return err
    }
    go a.repo.PingLoop()
    go a.processUpdates(updates)
    return nil
}

func (a *Arbitrager) processUpdates(upd <-chan PriceUpdate) {
    netF := (1 - a.commission)
    for pu := range upd {
        a.lock.Lock()
        a.prices[pu.Symbol] = pu.Price
        a.lock.Unlock()
        a.checkAll(netF)
    }
}

func (a *Arbitrager) checkAll(netF float64) {
    a.lock.RLock()
    defer a.lock.RUnlock()
    for _, t := range a.triangles {
        p1, ok1 := a.prices[t.A+t.B]
        p2, ok2 := a.prices[t.B+t.C]
        p3, ok3 := a.prices[t.A+t.C]
        if !ok1 || !ok2 || !ok3 { continue }
        // тройное списание комиссии netF^3
        profit := (p1*p2/p3*netF*netF*netF - 1) * 100
        if profit > a.minProfit {
            log.Printf("🔺 %s/%s/%s | profit: %.3f%%", t.A, t.B, t.C, profit)
        }
    }
}
infra/mxclient.go
go
Копировать код
package infra

import (
    "encoding/json"
    "net/http"
    "time"
)

// ExchangeInfo импортируемая модель.
type ExchangeInfo struct{ Symbols []struct{ Symbol string } }

// FetchSymbols загружает список торговых символов.
func FetchSymbols(client HTTPClient, url string) (map[string]bool, error) {
    data, err := client.Get(url)
    if err != nil {
        return nil, err
    }
    var info ExchangeInfo
    if err := json.Unmarshal(data, &info); err != nil {
        return nil, err
    }
    out := make(map[string]bool, len(info.Symbols))
    for _, s := range info.Symbols {
        out[s.Symbol] = true
    }
    return out, nil
}

// RealHTTPClient — реальный HTTP-клиент.
type RealHTTPClient struct{}

func (c *RealHTTPClient) Get(url string) ([]byte, error) {
    resp, err := http.Get(url)
    if err != nil {
        return nil, err
    }
    defer resp.Body.Close()
    return io.ReadAll(resp.Body)
}
infra/wsrepo.go
go
Копировать код
package infra

import (
    "github.com/gorilla/websocket"
    "time"
    "internal/domain"
)

// WSRepo реализует domain.PriceRepo поверх websocket.
type WSRepo struct {
    url       string
    conn      *websocket.Conn
    lastPong  time.Time
}

func NewWSRepo(url string) *WSRepo { return &WSRepo{url: url} }

func (r *WSRepo) Subscribe(channels []string) (<-chan domain.PriceUpdate, error) {
    conn, _, err := websocket.DefaultDialer.Dial(r.url, nil)
    if err != nil {
        return nil, err
    }
    r.conn = conn
    // подписка
    req := map[string]interface{}{"method":"SUBSCRIPTION","params":channels,"id":time.Now().Unix()}
    if err := conn.WriteJSON(req); err != nil {
        return nil, err
    }
    out := make(chan domain.PriceUpdate)
    go func() {
        defer close(out)
        for {
            _, msg, err := conn.ReadMessage()
            if err != nil {
                return
            }
            var u struct {
                Symbol string `json:"s"`
                Deals  []struct{ Price string `json:"p"` } `json:"d.deals"`
            }
            if err := json.Unmarshal(msg, &u); err != nil {
                continue
            }
            if len(u.Deals)==0 { continue }
            price, _ := strconv.ParseFloat(u.Deals[0].Price,64)
            out <- domain.PriceUpdate{Symbol:u.Symbol,Price:price}
        }
    }()
    return out, nil
}

func (r *WSRepo) PingLoop() {
    t := time.NewTicker(60*time.Second); defer t.Stop()
    for range t.C {
        r.conn.WriteMessage(websocket.PingMessage, []byte("hb"))
    }
}
app/arbitrage.go
go
Копировать код
package app

import (
    "internal/domain"
)

// Run запускает всю систему.
func Run() error {
    // загрузить triangles.json
    triangles, _ := loadTriangles("triangles.json")
    // HTTP → список символов
    httpClient := &infra.RealHTTPClient{}
    valid, _ := infra.FetchSymbols(httpClient, "https://api.mexc.com/api/v3/exchangeInfo")
    // отфильтровать треугольники под доступные пары
    triangles = filterTriangles(triangles, valid)

    // WebSocket-репозиторий
    wsRepo := infra.NewWSRepo("wss://wbs.mexc.com/ws")
    // сервис арбитража с комиссией 0.0005 (0.05%) и порогом 0.3%
    arb := domain.NewArbitrager(wsRepo, triangles, 0.0005, 0.3)
    if err := arb.Start(); err != nil {
        return err
    }
    select{} // работа бесконечно
}
cmd/mexc-arb/main.go
go
Копировать код
package main

import (
    "log"
    "internal/app"
)

func main() {
    log.SetFlags(log.LstdFlags|log.Lmicroseconds)
    if err := app.Run(); err != nil {
        log.Fatal(err)
    }
}
Весь «мотор» пересобран в три слоя:

domain — сущности, интерфейсы (PriceRepo, Triangle) и чистая бизнес-логика (Arbitrager).

infra — реализация PriceRepo через WebSocket, HTTP-клиент и всё, что общается с внешними API.

app + cmd — оркестратор: подгрузка конфигов, Run(), сборка зависимостей и запуск.

Теперь задержки в WebSocket, фильтрация HTTP-символов, сам расчёт арбитража и порогов находятся в чётко разделённых модулях — легко тестировать, профилировать и менять по отдельности.






Источники

Спросить ChatGPT


https://github.com/Gazmasater/cryp_arbtryang

gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ git init
hint: Using 'master' as the name for the initial branch. This default branch name
hint: is subject to change. To configure the initial branch name to use in all
hint: of your new repositories, which will suppress this warning, call:
hint: 
hint:   git config --global init.defaultBranch <name>
hint: 
hint: Names commonly chosen instead of 'master' are 'main', 'trunk' and
hint: 'development'. The just-created branch can be renamed via this command:
hint: 
hint:   git branch -m <name>
Initialized empty Git repository in /home/gaz358/myprog/crypt/.git/
gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ 

git config --global --add safe.directory /home/gaz358/myprog/crypt
git remote add origin https://github.com/Gazmasater/cryp_arbtryang.git

gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ git remote add origin https://github.com/Gazmasater/cryp_arbtryang.git
fatal: detected dubious ownership in repository at '/home/gaz358/myprog/crypt'
To add an exception for this directory, call:

        git config --global --add safe.directory /home/gaz358/myprog/crypt
gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ 

git add .
git commit -m "Initial import of triangular-arb bot"

git push -u origin master

git branch -m main        # переименовать локальную master → main
git push -u origin main








