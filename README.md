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



package app

import (
	"encoding/json"
	"log"
	"strconv"
	"sync"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

// Arbitrager выполняет арбитражные проверки по треугольным парам
type Arbitrager struct {
	Triangles       []triangle.Triangle
	latest          map[string]float64
	trianglesByPair map[string][]int

	// origSymbols — реальные торговые пары из API биржи
	origSymbols map[string]bool
	// availSymbols — расширенный граф (оригинальные + инверсии)
	availSymbols map[string]bool

	stepSizes map[string]float64
	minQtys   map[string]float64
	mu        sync.Mutex

	StartAmount float64
	exchange    exchange.Exchange
}

// New создаёт новый экземпляр Arbitrager с корректными символами и треугольниками
func New(ex exchange.Exchange) (*Arbitrager, error) {
	// 1. Получаем оригинальные символы и параметры из API биржи
	rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()

	// 2. Расширяем граф для подписки инверсиями пар
	avail := filesystem.ExpandAvailableSymbols(rawSymbols)
	log.Printf("📊 Всего подписываемых пар (с инверсиями): %d", len(avail))

	// 3. Строим все возможные треугольники по расширенному графу
	ts := buildTriangles(avail)

	// 4. Фильтруем треугольники: оставляем только те, у которых все стороны существуют в rawSymbols
	ts = filterTriangles(ts, rawSymbols)

	// 5. Группируем треугольники по символам, используя origSymbols для определения направления
	trianglesByPair := groupByPair(ts, rawSymbols)

	// 6. Инициализируем Arbitrager
	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
		origSymbols:     rawSymbols,
		availSymbols:    avail,
		stepSizes:       stepSizes,
		minQtys:         minQtys,
		StartAmount:     0.5,
		exchange:        ex,
	}
	return arb, nil
}

// filterTriangles оставляет только те треугольники, где все три стороны существуют в origSymbols
func filterTriangles(ts []triangle.Triangle, orig map[string]bool) []triangle.Triangle {
	var result []triangle.Triangle
	for _, tri := range ts {
		if (orig[tri.A+tri.B] || orig[tri.B+tri.A]) &&
			(orig[tri.B+tri.C] || orig[tri.C+tri.B]) &&
			(orig[tri.C+tri.A] || orig[tri.A+tri.C]) {
			result = append(result, tri)
		}
	}
	return result
}

// groupByPair группирует индексы треугольников по каждому ребру с учётом направления из origSymbols
func groupByPair(ts []triangle.Triangle, orig map[string]bool) map[string][]int {
	m := make(map[string][]int)
	for i, tri := range ts {
		edges := [][2]string{{tri.A, tri.B}, {tri.B, tri.C}, {tri.C, tri.A}}
		for _, e := range edges {
			base, quote := e[0], e[1]
			if orig[base+quote] {
				m[base+quote] = append(m[base+quote], i)
			} else {
				m[quote+base] = append(m[quote+base], i)
			}
		}
	}
	return m
}

// normalizeSymbolDir возвращает символ для запроса цены и флаг реверса на основе origSymbols
func (a *Arbitrager) normalizeSymbolDir(base, quote string) (symbol string, rev bool, ok bool) {
	if a.origSymbols[base+quote] {
		return base + quote, false, true
	}
	if a.origSymbols[quote+base] {
		return quote + base, true, true
	}
	return "", false, false
}

// buildTriangles загружает все возможные треугольники по расширенному графу символов
func buildTriangles(avail map[string]bool) []triangle.Triangle {
	ts, err := filesystem.LoadTrianglesFromSymbols(avail)
	if err != nil {
		log.Printf("⚠️ buildTriangles: ошибка LoadTrianglesFromSymbols: %v", err)
		return nil
	}
	return ts
}

// HandleRaw обрабатывает необработанное сообщение из биржи и обновляет цену
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

// Check выполняет проверку по символу на наличие арбитражных возможностей
func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	log.Printf("🛠️ Check called for symbol=%s, triangle indices=%v", symbol, indices)
	if len(indices) == 0 {
		return
	}

	nf := 0.9965 * 0.9965 * 0.9965

	for _, i := range indices {
		tri := a.Triangles[i]
		log.Printf("🔍 Triangle %d: %s->%s->%s", i, tri.A, tri.B, tri.C)
		ab, revAB, okAB := a.normalizeSymbolDir(tri.A, tri.B)
		bc, revBC, okBC := a.normalizeSymbolDir(tri.B, tri.C)
		ca, revCA, okCA := a.normalizeSymbolDir(tri.C, tri.A)
		log.Printf(
			"   normalize: AB=%s (ok=%v, rev=%v), BC=%s (ok=%v, rev=%v), CA=%s (ok=%v, rev=%v)",
			ab, okAB, revAB, bc, okBC, revBC, ca, okCA, revCA,
		)
		if !okAB || !okBC || !okCA {
			continue
		}

		p1, ok1 := a.latest[ab]
		p2, ok2 := a.latest[bc]
		p3, ok3 := a.latest[ca]
		if !ok1 || !ok2 || !ok3 {
			log.Printf("   missing prices: %s ok1=%v, %s ok2=%v, %s ok3=%v", ab, ok1, bc, ok2, ca, ok3)
			continue
		}
		if p1 == 0 || p2 == 0 || p3 == 0 {
			log.Printf("   zero prices: %s=%f, %s=%f, %s=%f", ab, p1, bc, p2, ca, p3)
			continue
		}
		log.Printf("   prices before rev: %s=%f, %s=%f, %s=%f", ab, p1, bc, p2, ca, p3)
		if revAB {
			p1 = 1 / p1
			log.Printf("   reversed AB price: %f", p1)
		}
		if revBC {
			p2 = 1 / p2
			log.Printf("   reversed BC price: %f", p2)
		}
		if revCA {
			p3 = 1 / p3
			log.Printf("   reversed CA price: %f", p3)
		}
		profitFactor := p1 * p2 * p3 * nf
		profit := (profitFactor - 1) * 100
		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
	}
}

