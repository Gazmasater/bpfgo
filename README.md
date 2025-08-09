
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



package main

import (
	"log"
	"strings"
	"sync"
	"time"

	"cryptarb/internal/repository/mexc"
)

type qv struct{ bid, ask float64 }

func main() {
	ex := mexc.NewMEXCExchange()

	avail, _, _ := ex.FetchAvailableSymbols()
	if len(avail) == 0 {
		log.Fatal("MEXC avail empty")
	}

	tri := pickTriangle(avail) // BTCUSDT + ALTUSDT + ALTBTC
	log.Printf("🔺 TRI: %v", tri)

	var (
		mu   sync.Mutex
		book = map[string]qv{}
	)

	const (
		fee       = 0.0010 // 0.10% такер (подставь свою)
		threshold = 0.02   // 0.02% порог (для теста можно 0.005)
	)

	// Дополнительно подпишемся на deals (last), чтобы видеть сам поток
	go ex.SubscribeTickers(tri, func(sym string, last float64) {
		log.Printf("[MEXC] TICK  %-10s last=%f", strings.ToUpper(sym), last)
	})

	// Основная подписка: bid/ask для арбитража
	if err := ex.SubscribeQuotes(tri, func(sym string, bid, ask float64, ts time.Time) {
		sym = strings.ToUpper(sym)

		mu.Lock()
		book[sym] = qv{bid: bid, ask: ask}
		// снимем снапшот, чтобы не держать лок в расчёте
		snap := make(map[string]qv, len(book))
		for k, v := range book {
			snap[k] = v
		}
		mu.Unlock()

		log.Printf("[MEXC] QUOTE %-10s bid=%f ask=%f", sym, bid, ask)
		tryProfit(snap, fee, threshold)
	}); err != nil {
		log.Fatalf("SubscribeQuotes error: %v", err)
	}

	// держим процесс (SubscribeQuotes внутри запускает горутины)
	select {}
}

// Возвращает корректный треугольник: BTCUSDT + ALTUSDT + ALTBTC.
func pickTriangle(avail map[string]bool) []string {
	if !avail["BTCUSDT"] {
		// редкий фолбэк на ETH-хаб
		if avail["ETHUSDT"] {
			for s := range avail {
				if strings.HasSuffix(s, "USDT") && len(s) > 4 {
					alt := s[:len(s)-4]
					if avail[alt+"ETH"] {
						return []string{"ETHUSDT", alt + "USDT", alt + "ETH"}
					}
				}
			}
		}
		return []string{"BTCUSDT", "ETHUSDT", "ETHBTC"}
	}

	hot := []string{"ETH", "BNB", "XRP", "SOL", "ADA", "DOGE", "TRX", "TON", "LINK", "LTC"}
	for _, alt := range hot {
		if avail[alt+"USDT"] && avail[alt+"BTC"] {
			return []string{"BTCUSDT", alt + "USDT", alt + "BTC"}
		}
	}
	for s := range avail {
		if strings.HasSuffix(s, "USDT") && len(s) > 4 {
			alt := s[:len(s)-4]
			if avail[alt+"BTC"] {
				return []string{"BTCUSDT", alt + "USDT", alt + "BTC"}
			}
		}
	}
	return []string{"BTCUSDT", "ETHUSDT", "ETHBTC"}
}

// Печатает профит, если выше threshold (%). Возвращает true/false.
func tryProfit(book map[string]qv, fee, threshold float64) bool {
	a, ok1 := book["BTCUSDT"]
	b, ok2 := book["ETHUSDT"]
	c, ok3 := book["ETHBTC"]
	if !(ok1 && ok2 && ok3) {
		log.Printf("⛔ нет котировок: BTCUSDT=%v ETHUSDT=%v ETHBTC=%v", ok1, ok2, ok3)
		return false
	}
	if a.ask <= 0 || b.ask <= 0 || c.ask <= 0 || a.bid <= 0 || b.bid <= 0 || c.bid <= 0 ||
		a.ask < a.bid || b.ask < b.bid || c.ask < c.bid {
		log.Printf("⛔ некорректные цены: BTCUSDT %.6f/%.6f ETHUSDT %.6f/%.6f ETHBTC %.6f/%.6f",
			a.bid, a.ask, b.bid, b.ask, c.bid, c.ask)
		return false
	}

	start := 1.0 // 1 USDT

	// Путь 1: USDT→BTC (ask), BTC→ETH (ask), ETH→USDT (bid)
	btc := start / a.ask * (1 - fee)
	eth := btc / c.ask * (1 - fee)
	usdtBack := eth * b.bid * (1 - fee)
	p1 := (usdtBack - start) * 100

	// Путь 2: USDT→ETH (ask), ETH→BTC (bid), BTC→USDT (bid)
	eth2 := start / b.ask * (1 - fee)
	btc2 := eth2 * c.bid * (1 - fee)
	usdtBack2 := btc2 * a.bid * (1 - fee)
	p2 := (usdtBack2 - start) * 100

	if p1 > threshold || p2 > threshold {
		log.Printf("💹 PROFIT p1=%.3f%% p2=%.3f%% | BTCUSDT %.4f/%.4f ETHUSDT %.4f/%.4f ETHBTC %.6f/%.6f",
			p1, p2, a.bid, a.ask, b.bid, b.ask, c.bid, c.ask)
		return true
	}
	return false
}









