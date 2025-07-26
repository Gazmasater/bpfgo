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

// internal/repository/mexc/mexc.go
package mexc

import (
	"encoding/json"
	"log"
	"net/http"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

type Mexc struct{}

func (Mexc) Name() string {
	return "MEXC"
}

func (Mexc) FetchAvailableSymbols() map[string]bool {
	out := make(map[string]bool)
	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		return out
	}
	defer resp.Body.Close()
	var body struct {
		Symbols []struct {
			Symbol string `json:"symbol"`
		}
	}
	if err := json.NewDecoder(resp.Body).Decode(&body); err != nil {
		return out
	}
	for _, s := range body.Symbols {
		out[s.Symbol] = true
	}
	return out
}

func (Mexc) SubscribeDeals(pairs []string, handler func(exchange string, raw []byte)) error {
	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", nil)
	if err != nil {
		return err
	}
	defer conn.Close()

	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": buildChannels(pairs),
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return err
	}

	var mu sync.Mutex
	var lastPing time.Time
	conn.SetPongHandler(func(string) error {
		log.Printf("📶 [MEXC] Pong after %v\n", time.Since(lastPing))
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

	for {
		_, raw, err := conn.ReadMessage()
		if err != nil {
			return err
		}
		handler("MEXC", raw)
	}
}

func buildChannels(pairs []string) []string {
	out := make([]string, 0, len(pairs))
	for _, p := range pairs {
		out = append(out, "spot@public.deals.v3.api@"+p)
	}
	return out
}



// internal/app/arbitrage.go
package app

import (
	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
	"encoding/json"
	"log"
	"strconv"
	"strings"
	"sync"
)

type Arbitrager struct {
	Triangles       []triangle.Triangle
	latest          map[string]float64
	trianglesByPair map[string][]int
	sumProfit       float64
	mu              sync.Mutex
}

func New(dataPath string, ex exchange.Exchange) (*Arbitrager, error) {
	ts, err := filesystem.LoadTriangles(dataPath)
	if err != nil {
		return nil, err
	}

	avail := ex.FetchAvailableSymbols()
	ts = triangle.Filter(ts, avail)

	trianglesByPair := make(map[string][]int)
	for i, tri := range ts {
		pairs := []string{
			tri.A + tri.B,
			tri.B + tri.C,
			tri.A + tri.C,
			tri.B + tri.A,
			tri.C + tri.B,
			tri.C + tri.A,
		}
		for _, p := range pairs {
			trianglesByPair[p] = append(trianglesByPair[p], i)
		}
	}

	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
	}

	go func() {
		if err := ex.SubscribeDeals(triangle.SymbolPairs(ts), arb.HandleRaw); err != nil {
			log.Fatal(err)
		}
	}()

	return arb, nil
}

func (a *Arbitrager) HandleRaw(exchange string, raw []byte) {
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
	rev := reverseSymbol(msg.Symbol)
	a.latest[rev] = 1 / price
	a.mu.Unlock()

	a.Check(msg.Symbol)
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		return
	}

	const commission = 0.001
	nf := (1 - commission) * (1 - commission) * (1 - commission)

	for _, i := range indices {
		tri := a.Triangles[i]
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ac := tri.A + tri.C

		p1, ok1 := a.latest[ab]
		p2, ok2 := a.latest[bc]
		p3, ok3 := a.latest[ac]

		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		profit := (p1*p2/p3*nf - 1) * 100
		if profit > 0 {
			a.sumProfit += profit
			log.Printf("🔺 %s/%s/%s profit %.3f%% total=%.3f%%",
				tri.A, tri.B, tri.C, profit, a.sumProfit)
		}
	}
}

func reverseSymbol(sym string) string {
	if len(sym)%2 != 0 {
		return sym
	}
	half := len(sym) / 2
	return sym[half:] + sym[:half]
}



// cmd/cryptarb/main.go
package main

import (
	"cryptarb/internal/app"
	"cryptarb/internal/repository/mexc"
	"log"
)

func main() {
	ex := mexc.Mexc{}

	arb, err := app.New("triangles.json", ex)
	if err != nil {
		log.Fatal(err)
	}

	select {} // блокируем main, пока работает арбитраж
}


// internal/domain/exchange/exchange.go
package exchange

// Exchange определяет интерфейс криптобиржи для подключения к арбитражному движку
// и абстрагирует WebSocket/REST взаимодействие.
type Exchange interface {
	// Name возвращает уникальное имя биржи (например: "MEXC")
	Name() string

	// FetchAvailableSymbols возвращает список доступных торговых пар ("BTCUSDT" и т.д.)
	FetchAvailableSymbols() map[string]bool

	// SubscribeDeals подписывается на обновления по указанным парам
	// и вызывает handler при каждом событии сделки
	SubscribeDeals(pairs []string, handler func(exchange string, raw []byte)) error
}



// SymbolPairs возвращает все уникальные пары из треугольников
func SymbolPairs(ts []Triangle) []string {
	set := make(map[string]struct{})
	for _, t := range ts {
		for _, p := range [][2]string{{t.A, t.B}, {t.B, t.C}, {t.A, t.C}} {
			set[p[0]+p[1]] = struct{}{}
		}
	}
	out := make([]string, 0, len(set))
	for k := range set {
		out = append(out, k)
	}
	return out
}



sudo docker compose build --no-cache
sudo docker compose up -d


gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ sudo docker compose build --no-cache
[sudo] password for gaz358: 
WARN[0000] /home/gaz358/myprog/crypt/docker-compose.yml: the attribute `version` is obsolete, it will be ignored, please remove it to avoid potential confusion 
[+] Building 80.6s (14/14) FINISHED                                                              
 => [internal] load local bake definitions                                                  0.0s
 => => reading from stdin 375B                                                              0.0s
 => [internal] load build definition from Dockerfile                                        0.0s
 => => transferring dockerfile: 986B                                                        0.0s
 => resolve image config for docker-image://docker.io/docker/dockerfile:1.4                 1.9s
 => CACHED docker-image://docker.io/docker/dockerfile:1.4@sha256:9ba7531bd80fb0a858632727c  0.0s
 => [internal] load .dockerignore                                                           0.0s
 => => transferring context: 2B                                                             0.0s
 => [internal] load metadata for docker.io/library/ubuntu:24.04                             0.0s
 => CACHED [1/6] FROM docker.io/library/ubuntu:24.04                                        0.0s
 => [2/6] RUN apt-get update &&     apt-get install -y --no-install-recommends curl git m  30.2s
 => [3/6] WORKDIR /app                                                                      0.0s
 => [4/6] RUN git clone --branch cleanarh https://github.com/Gazmasater/cryp_arbtryang.git  1.9s
 => [5/6] RUN go mod download                                                              22.8s
 => [6/6] RUN CGO_ENABLED=0 GOOS=linux go build -ldflags="-s -w"     -o cryptarb ./cmd/cr  20.0s
 => exporting to image                                                                      3.4s
 => => exporting layers                                                                     3.4s
 => => writing image sha256:3029e906d78ff1a2d2f6a3bcdb1f82ed8657a94a01f7826e8640b274072d5d  0.0s
 => => naming to docker.io/library/cryptarb                                                 0.0s
 => resolving provenance for metadata file                                                  0.0s
[+] Building 1/1
 ✔ cryptarb  Built                                                                          0.0s 
gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ sudo docker compose up -d
WARN[0000] /home/gaz358/myprog/crypt/docker-compose.yml: the attribute `version` is obsolete, it will be ignored, please remove it to avoid potential confusion 
[+] Running 1/1
 ✔ Container crypt-cryptarb-1  Started    




