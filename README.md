
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




package app

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"sync"
	"time"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

// Arbitrager ищет треугольные арбитражные возможности на бирже.
type Arbitrager struct {
	Triangles       []triangle.Triangle
	trianglesByPair map[string][]int   // Индексы треугольников по паре
	realSymbols     map[string]bool    // Доступные пары (с инверсиями)
	stepSizes       map[string]float64 // Шаг лота
	minQtys         map[string]float64 // Мин. объём
	latest          map[string]Quote   // было map[string]float64

	mu          sync.Mutex
	StartAmount float64
	exchange    exchange.Exchange
}

type Quote struct {
	Bid float64
	Ask float64
	Ts  time.Time
}

// New создаёт и инициализирует арбитражер.
func New(ex exchange.Exchange) (*Arbitrager, error) {
	// 1) Реальные пары и лоты с бирж (без инверсий)
	rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()
	if len(rawSymbols) == 0 {
		return nil, fmt.Errorf("no spot symbols from %s", ex.Name())
	}

	// 2) Расширяем инверсиями и строим треугольники
	avail := filesystem.ExpandAvailableSymbols(rawSymbols) // AB и BA = true
	log.Printf("📊 Доступные пары (реальные+инверсии): %d", len(avail))

	ts, err := filesystem.LoadTrianglesFromSymbols(avail)
	if err != nil {
		return nil, fmt.Errorf("LoadTrianglesFromSymbols: %w", err)
	}
	log.Printf("[INIT] Треугольников найдено: %d", len(ts))

	// Для отладки — дамп треугольников
	if data, err := json.MarshalIndent(ts, "", "  "); err == nil {
		_ = os.WriteFile("triangles_dump.json", data, 0644)
	}

	// 3) Индексация: на каждый треугольник вешаем оба направления ребра (AB и BA)
	trianglesByPair := make(map[string][]int, len(ts)*6)
	// И список реальных пар для подписки: подписываемся только на то, что реально торгуется (rawSymbols)
	realToSubscribe := make(map[string]struct{}, len(ts)*6)

	for i, tri := range ts {
		ab, ba := tri.A+tri.B, tri.B+tri.A
		bc, cb := tri.B+tri.C, tri.C+tri.B
		ca, ac := tri.C+tri.A, tri.A+tri.C

		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[ba] = append(trianglesByPair[ba], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[cb] = append(trianglesByPair[cb], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)
		trianglesByPair[ac] = append(trianglesByPair[ac], i)

		// Подписываемся только на реально существующие направления
		if rawSymbols[ab] {
			realToSubscribe[ab] = struct{}{}
		}
		if rawSymbols[ba] {
			realToSubscribe[ba] = struct{}{}
		}
		if rawSymbols[bc] {
			realToSubscribe[bc] = struct{}{}
		}
		if rawSymbols[cb] {
			realToSubscribe[cb] = struct{}{}
		}
		if rawSymbols[ca] {
			realToSubscribe[ca] = struct{}{}
		}
		if rawSymbols[ac] {
			realToSubscribe[ac] = struct{}{}
		}
	}

	subPairs := make([]string, 0, len(realToSubscribe))
	for p := range realToSubscribe {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] Индекс по парам: %d ключей", len(trianglesByPair))
	log.Printf("[INIT] Подписка на реальных пар: %d шт.", len(subPairs))

	// 4) Инициализация арбитражёра
	arb := &Arbitrager{
		Triangles: ts,
		latest:    make(map[string]Quote, len(subPairs)),

		trianglesByPair: trianglesByPair,
		realSymbols:     avail,     // содержит и AB, и BA -> нужно для normalize/invert
		stepSizes:       stepSizes, // как пришло от биржи
		minQtys:         minQtys,   // как пришло от биржи
		StartAmount:     0.5,
		exchange:        ex,
	}

	// 5) WS-подписки чанками; каждый чанк — отдельное соединение у адаптера биржи
	const maxPerConn = 25
	for i := 0; i < len(subPairs); i += maxPerConn {
		end := i + maxPerConn
		if end > len(subPairs) {
			end = len(subPairs)
		}
		chunk := append([]string(nil), subPairs[i:end]...) // защитим от гонок

		go func(pairs []string) {
			for {
				if err := ex.SubscribeTickers(pairs, arb.OnTick); err != nil {
					log.Printf("[WS][%s] subscribe error: %v, retrying...", ex.Name(), err)
					time.Sleep(time.Second)
					continue
				}
				log.Printf("[WS][%s] subscribed to %d pairs", ex.Name(), len(pairs))
				return
			}
		}(chunk)
	}

	return arb, nil
}

// OnTick кладёт последнюю цену и триггерит пересчёт
func (a *Arbitrager) OnTick(symbol string, price float64) {
	if price <= 0 {
		return
	}

	a.mu.Lock()
	_, has := a.trianglesByPair[symbol]
	if !has {
		a.mu.Unlock()
		return
	}
	a.latest[symbol] = price
	a.mu.Unlock()

	a.Check(symbol)
}

// Check пересчитывает профит для всех треугольников, связанных с символом.
// Берёт снапшот цен, чтобы не держать мьютекс на вычислениях.
func (a *Arbitrager) Check(symbol string) {
	const (
		feeFactor  = 0.9965 * 0.9965 * 0.9965 // комиссия 3 ног
		staleLimit = 300 * time.Millisecond   // макс. «возраст» котировки
	)

	// 1) Забираем индексы и снапшот цен под мьютексом
	a.mu.Lock()
	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		a.mu.Unlock()
		return
	}
	now := time.Now()
	snap := make(map[string]Quote, len(a.latest))
	for k, v := range a.latest {
		// можно отфильтровать сразу «протухшие», но оставим на getLeg
		snap[k] = v
	}
	a.mu.Unlock()

	// 2) Направленная цена ребра A→B:
	//    - если есть котировка по AB: продаём A за B → используем bid_AB
	//    - иначе, если есть по BA: покупаем A за B через BA → 1 / ask_BA
	getLeg := func(a, b string) (float64, bool) {
		if q, ok := snap[a+b]; ok && q.Bid > 0 && now.Sub(q.Ts) <= staleLimit {
			return q.Bid, true
		}
		if q, ok := snap[b+a]; ok && q.Ask > 0 && now.Sub(q.Ts) <= staleLimit {
			return 1 / q.Ask, true
		}
		return 0, false
	}

	// 3) Считаем треугольники
	for _, idx := range indices {
		tri := a.Triangles[idx]

		p1, ok1 := getLeg(tri.A, tri.B)
		p2, ok2 := getLeg(tri.B, tri.C)
		p3, ok3 := getLeg(tri.C, tri.A)
		if !ok1 || !ok2 || !ok3 {
			continue
		}

		gross := p1 * p2 * p3
		net := gross * feeFactor
		profitPct := (net - 1) * 100

		// Хочешь — активируй порог:
		// if profitPct < 0.05 { continue } // пример: не ниже 5 б.п.

		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%% (gross=%.6f net=%.6f)",
			tri.A, tri.B, tri.C, profitPct, gross, net)
	}
}

func (a *Arbitrager) OnQuote(symbol string, bid, ask float64, ts time.Time) {
	if bid <= 0 || ask <= 0 || ask < bid {
		return
	}

	a.mu.Lock()
	_, has := a.trianglesByPair[symbol]
	if !has {
		a.mu.Unlock()
		return
	}
	a.latest[symbol] = Quote{Bid: bid, Ask: ask, Ts: ts}
	a.mu.Unlock()

	a.Check(symbol)
}


[{
	"resource": "/home/gaz358/myprog/crypt/internal/app/arbitrage.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "IncompatibleAssign",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "IncompatibleAssign"
		}
	},
	"severity": 8,
	"message": "cannot use price (variable of type float64) as Quote value in assignment",
	"source": "compiler",
	"startLineNumber": 154,
	"startColumn": 21,
	"endLineNumber": 154,
	"endColumn": 26,
	"origin": "extHost1"
}]





