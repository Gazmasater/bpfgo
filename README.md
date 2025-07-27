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
	"time"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

type Arbitrager struct {
	Triangles       []triangle.Triangle
	latest          map[string]float64
	trianglesByPair map[string][]int
	sumProfit       float64
	mu              sync.Mutex
}

func New(path string, ex exchange.Exchange) (*Arbitrager, error) {
	ts, err := filesystem.LoadTriangles(path)
	if err != nil {
		return nil, err
	}

	avail := ex.FetchAvailableSymbols()
	ts = triangle.Filter(ts, avail)

	// Построим индексы треугольников по парам
	trianglesByPair := make(map[string][]int)
	// Подготовим список подписки
	subPairs := make([]string, 0, len(ts)*3)
	for i, tri := range ts {
		// Определяем пары подписки и индексы для Check
		ab := tri.A + tri.B    // A→B, base A, quote B
		bc := tri.C + tri.B    // B→C, подписываем C+B, так как на MEXC base=C, quote=B для B->C
		ca := tri.A + tri.C    // C→A, подписываем A+C, base A, quote C for C->A

		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)

		subPairs = append(subPairs, ab, bc, ca)
	}
	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
	}

	// Запускаем WS с авто-переподключением на нужные пары
	go func() {
		for {
			err := ex.SubscribeDeals(subPairs, arb.HandleRaw)
			if err != nil {
				log.Printf("[WS][%s] subscribe error: %v; reconnecting in 1s...", ex.Name(), err)
				time.Sleep(time.Second)
				continue
			}
			break
		}
	}()

	return arb, nil
}

func (a *Arbitrager) HandleRaw(exchangeName string, raw []byte) {
	// Парсим WS-сообщение
	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct { Price string `json:"p"`} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("[ERROR][%s] unmarshal raw: %v", exchangeName, err)
		return
	}
	if msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		return
	}

	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		log.Printf("[ERROR][%s] parse price %q: %v", exchangeName, msg.Data.Deals[0].Price, err)
		return
	}

	a.mu.Lock()
	a.latest[msg.Symbol] = price
	log.Printf("[TICK][%s] %s=%.8f", exchangeName, msg.Symbol, price)
	a.mu.Unlock()

	// Проверяем треугольники по этому символу
	a.Check(msg.Symbol)
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		return
	}

	const commission = 0.0005
	nf := (1 - commission) * (1 - commission) * (1 - commission)

	for _, i := range indices {
		tri := a.Triangles[i]
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ca := tri.C + tri.A

		p1, ok1 := a.latest[ab]
		p2, ok2 := a.latest[bc]
		p3, ok3 := a.latest[ca]

		log.Printf("[DATA] tri=%s/%s/%s AB=%v(ok=%v) BC=%v(ok=%v) CA=%v(ok=%v)",
			tri.A, tri.B, tri.C,
			p1, ok1, p2, ok2, p3, ok3)

		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		profit := (p1 * p2 / p3 * nf - 1) * 100
		a.sumProfit += profit
		log.Printf("🔺 ARB %s/%s/%s profit=%.3f%% total=%.3f%%", tri.A, tri.B, tri.C, profit, a.sumProfit)
	}
}



gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83730000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83800000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83820000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83930000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83760000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83740000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83840000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83880000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83910000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:30 [TICK][MEXC] SOLUSDC=185.83900000
2025/07/27 17:58:30 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:32 [TICK][MEXC] USDCUSDT=0.99950000
2025/07/27 17:58:32 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:32 [TICK][MEXC] USDCUSDT=0.99950000
2025/07/27 17:58:32 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:32 [TICK][MEXC] USDCUSDT=0.99950000
2025/07/27 17:58:32 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99950000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99950000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] SOLUSDT=185.79000000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] SOLUSDT=185.79000000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] SOLUSDT=185.79000000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99960000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99960000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99960000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99960000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:33 [TICK][MEXC] USDCUSDT=0.99960000
2025/07/27 17:58:33 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:34 [TICK][MEXC] USDCUSDT=0.99960000
2025/07/27 17:58:34 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:34 [TICK][MEXC] SOLUSDT=185.80000000
2025/07/27 17:58:34 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:34 [TICK][MEXC] SOLUSDT=185.80000000
2025/07/27 17:58:34 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:34 [TICK][MEXC] SOLUSDT=185.80000000
2025/07/27 17:58:34 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83850000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83810000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83970000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83920000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83950000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83950000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83950000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83820000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83950000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83950000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.79000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.79000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.79000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83990000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83920000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.84000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.84050000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83930000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83870000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.84030000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDC=185.83850000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.79(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.77000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.77(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.77000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.77(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.77000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.77(ok=true) BC=0(ok=false) CA=0(ok=false)
2025/07/27 17:58:35 [TICK][MEXC] SOLUSDT=185.80000000
2025/07/27 17:58:35 [DATA] tri=SOL/USDT/USDC AB=185.8(ok=true) BC=0(ok=false) CA=0(ok=false)












