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


(pprof) top
Showing nodes accounting for 130ms, 100% of 130ms total
Showing top 10 nodes out of 62
      flat  flat%   sum%        cum   cum%
      40ms 30.77% 30.77%       40ms 30.77%  internal/runtime/syscall.Syscall6
      10ms  7.69% 38.46%       10ms  7.69%  encoding/json.stateBeginValue
      10ms  7.69% 46.15%       10ms  7.69%  encoding/json.unquoteBytes
      10ms  7.69% 53.85%       10ms  7.69%  memeqbody
      10ms  7.69% 61.54%       10ms  7.69%  runtime.(*spanSet).push
      10ms  7.69% 69.23%       10ms  7.69%  runtime.ifaceeq
      10ms  7.69% 76.92%       10ms  7.69%  runtime.memclrNoHeapPointers
      10ms  7.69% 84.62%       10ms  7.69%  runtime.nextFreeFast
      10ms  7.69% 92.31%       10ms  7.69%  runtime.typePointers.next
      10ms  7.69%   100%       10ms  7.69%  strconv.ParseFloat
(pprof) 


package app

import (
    "log"
    "math"
    "strconv"
    "strings"
    "sync"
    "time"

    "github.com/valyala/fastjson"

    "cryptarb/internal/domain/exchange"
    "cryptarb/internal/domain/triangle"
    "cryptarb/internal/repository/filesystem"
)

// Arbitrager handles real-time price updates
// and checks for triangular arbitrage opportunities.
type Arbitrager struct {
    Triangles       []triangle.Triangle
    latest          sync.Map            // map[string]float64
    trianglesByPair map[string][]int
    realSymbols     map[string]bool
    realSymbolsMu   sync.Mutex
    stepSizes       map[string]float64
    minQtys         map[string]float64
    StartAmount     float64
    exchange        exchange.Exchange
    checkJobs       chan string
    workerWG        sync.WaitGroup
}

// New creates and initializes an Arbitrager, subscribes to price feeds.
func New(ex exchange.Exchange) (*Arbitrager, error) {
    rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()
    avail := filesystem.ExpandAvailableSymbols(rawSymbols)
    log.Printf("📊 Всего доступных пар (с инверсиями): %d", len(avail))

    ts, err := filesystem.LoadTrianglesFromSymbols(avail)
    if err != nil {
        return nil, err
    }
    log.Printf("[INIT] Загружено треугольников: %d", len(ts))

    // Build reverse index and subscription list
    trianglesByPair := make(map[string][]int)
    subPairsRaw := make([]string, 0, len(ts)*3)
    for i, tri := range ts {
        ab := tri.A + tri.B
        bc := tri.B + tri.C
        ca := tri.C + tri.A
        trianglesByPair[ab] = append(trianglesByPair[ab], i)
        trianglesByPair[bc] = append(trianglesByPair[bc], i)
        trianglesByPair[ca] = append(trianglesByPair[ca], i)
        subPairsRaw = append(subPairsRaw, ab, bc, ca)
    }
    uniq := make(map[string]struct{})
    for _, p := range subPairsRaw {
        if avail[p] {
            uniq[p] = struct{}{}
        }
    }
    subPairs := make([]string, 0, len(uniq))
    for p := range uniq {
        subPairs = append(subPairs, p)
    }
    log.Printf("[INIT] Итог: подписываемся на %d уникальных пар", len(subPairs))

    arb := &Arbitrager{
        Triangles:       ts,
        trianglesByPair: trianglesByPair,
        realSymbols:     avail,
        stepSizes:       stepSizes,
        minQtys:         minQtys,
        StartAmount:     0.5,
        exchange:        ex,
        checkJobs:       make(chan string, 1000),
    }

    // Start worker pool
    const workers = 4
    for i := 0; i < workers; i++ {
        arb.workerWG.Add(1)
        go arb.worker()
    }

    // Subscribe in chunks
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
                    log.Printf("[WS][%s] subscribe error: %v; retrying", ex.Name(), err)
                    time.Sleep(time.Second)
                    continue
                }
                return
            }
        }(chunk)
    }

    return arb, nil
}

// worker processes symbols for arbitrage checks.
func (a *Arbitrager) worker() {
    defer a.workerWG.Done()
    for sym := range a.checkJobs {
        a.Check(sym)
    }
}

// HandleRaw is called on each raw WebSocket message.
func (a *Arbitrager) HandleRaw(exchangeName string, raw []byte) {
    var p fastjson.Parser
    v, err := p.ParseBytes(raw)
    if err != nil {
        log.Printf("[WS][%s] JSON parse error: %v", exchangeName, err)
        return
    }

    // Process subscription ACK failures
    if code := v.GetInt("code"); code == 0 {
        msg := string(v.GetStringBytes("msg"))
        if strings.Contains(msg, "Not Subscribed successfully!") {
            parts := strings.Split(msg, "Not Subscribed successfully! [")
            if len(parts) == 2 {
                blocked := strings.Split(
                    strings.TrimSuffix(parts[1], "].  Reason： Blocked! \""), ",")
                for _, ch := range blocked {
                    if idx := strings.LastIndex(ch, "@"); idx != -1 {
                        sym := ch[idx+1:]
                        a.realSymbolsMu.Lock()
                        a.realSymbols[sym] = false
                        a.realSymbolsMu.Unlock()
                    }
                }
                return
            }
        }
    }

    // Extract symbol and price
    symBytes := v.GetStringBytes("s")
    deals := v.GetArray("d", "deals")
    if len(symBytes) == 0 || len(deals) == 0 {
        return
    }
    sym := string(symBytes)
    priceStr := deals[0].GetStringBytes("p")
    price, err := strconv.ParseFloat(string(priceStr), 64)
    if err != nil {
        log.Printf("[WS][%s] parse price error: %v", exchangeName, err)
        return
    }

    // Store and enqueue for check
    a.latest.Store(sym, price)
    select {
    case a.checkJobs <- sym:
    default:
    }
}

// Check calculates arbitrage for a given symbol.
func (a *Arbitrager) Check(symbol string) {
    const feeFactor = 0.9965 * 0.9965 * 0.9965
    indices, ok := a.trianglesByPair[symbol]
    if !ok {
        return
    }
    for _, i := range indices {
        tri := a.Triangles[i]

        // Keys for each leg
        keyAB := tri.A + tri.B
        keyBC := tri.B + tri.C
        keyCA := tri.C + tri.A

        // Retrieve prices
        p1v, ok1 := a.latest.Load(keyAB)
        p2v, ok2 := a.latest.Load(keyBC)
        p3v, ok3 := a.latest.Load(keyCA)
        if !ok1 || !ok2 || !ok3 {
            continue
        }
        p1 := p1v.(float64)
        p2 := p2v.(float64)
        p3 := p3v.(float64)
        if p1 == 0 || p2 == 0 || p3 == 0 {
            continue
        }

        // Start with base amount
        amount := a.StartAmount

        // Adjust amount by step size and min quantity for A->B
        if min, has := a.minQtys[keyAB]; has && amount < min {
            continue
        }
        if step, has := a.stepSizes[keyAB]; has {
            amount = math.Floor(amount/step) * step
        }
        if amount == 0 {
            continue
        }

        // Leg 1: A -> B
        amountB := amount / p1 * feeFactor

        // Adjust B->C
        if min, has := a.minQtys[keyBC]; has && amountB < min {
            continue
        }
        if step, has := a.stepSizes[keyBC]; has {
            amountB = math.Floor(amountB/step) * step
        }
        if amountB == 0 {
            continue
        }

        // Leg 2: B -> C
        amountC := amountB / p2 * feeFactor

        // Adjust C->A
        if min, has := a.minQtys[keyCA]; has && amountC < min {
            continue
        }
        if step, has := a.stepSizes[keyCA]; has {
            amountC = math.Floor(amountC/step) * step
        }
        if amountC == 0 {
            continue
        }

        // Leg 3: C -> A
        finalAmount := amountC * p3 * feeFactor
        profit := finalAmount - amount
        profitPerc := profit / amount * 100

        if profit > 0 {
            log.Printf("🔺 ARB %s/%s/%s | start=%.6f end=%.6f profit=%.6f %.4f%%",
                tri.A, tri.B, tri.C,
                amount, finalAmount, profit, profitPerc)
        }
    }
}



