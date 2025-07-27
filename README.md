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


func New(path string, ex exchange.Exchange) (*Arbitrager, error) {
    ts, err := filesystem.LoadTriangles(path)
    if err != nil {
        return nil, err
    }
    avail := ex.FetchAvailableSymbols()
    ts = triangle.Filter(ts, avail)

    // собираем пары и индексы
    trianglesByPair := make(map[string][]int)
    var subPairs []string
    for i, tri := range ts {
        ab := tri.A + tri.B
        bc := tri.B + tri.C
        ca := tri.C + tri.A
        trianglesByPair[ab] = append(trianglesByPair[ab], i)
        trianglesByPair[bc] = append(trianglesByPair[bc], i)
        trianglesByPair[ca] = append(trianglesByPair[ca], i)
        subPairs = append(subPairs, ab, bc, ca)
    }

    // фильтруем только по реально существующим символам
    uniq := make(map[string]struct{}, len(subPairs))
    for _, p := range subPairs {
        if avail[p] {
            uniq[p] = struct{}{}
        }
    }
    subPairs = subPairs[:0]
    for p := range uniq {
        subPairs = append(subPairs, p)
    }
    log.Printf("[INIT] triangles=%d, subscribing on %d valid pairs: %v",
        len(ts), len(subPairs), subPairs,
    )

    arb := &Arbitrager{
        Triangles:       ts,
        latest:          make(map[string]float64),
        trianglesByPair: trianglesByPair,
    }

    // подписываемся чанками по 25 пар, чтобы не перегрузить WS
    const maxPerConn = 25
    for i := 0; i < len(subPairs); i += maxPerConn {
        end := i + maxPerConn
        if end > len(subPairs) {
            end = len(subPairs)
        }
        chunk := subPairs[i:end]
        go func(pairs []string) {
            for {
                if err := ex.SubscribeDeals(pairs, arb.HandleRaw); err != nil {
                    log.Printf("[WS][%s] subscribe chunk error: %v; retrying…", ex.Name(), err)
                    time.Sleep(time.Second)
                    continue
                }
                break
            }
        }(chunk)
    }

    return arb, nil
}


func LoadTriangles(path string) ([]triangle.Triangle, error) {
	// дефолтные треугольники
	t := []triangle.Triangle{
		{A: "SOL", B: "USDT", C: "USDC"},
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
		// Новые 10
		{A: "SOL", B: "USDT", C: "ADA"},
		{A: "SOL", B: "BTC", C: "ETH"},
		{A: "ETH", B: "USDT", C: "DOT"},
		{A: "ADA", B: "BTC", C: "LTC"},
		{A: "DOGE", B: "USDT", C: "MATIC"},
		{A: "LINK", B: "ETH", C: "USDT"},
		{A: "AVAX", B: "USDT", C: "LINK"},
		{A: "TRX", B: "USDT", C: "ADA"},
		{A: "BCH", B: "USDT", C: "SOL"},
		{A: "DOT", B: "USDT", C: "DOGE"},
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



____________________________________________________________________________


// internal/repository/filesystem/loader.go
package filesystem

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"

	"cryptarb/internal/domain/triangle"
)

// LoadTriangles возвращает суровый список треугольников,
// затем вы можете дополнительно фильтровать его по доступности пар.
func LoadTriangles(path string) ([]triangle.Triangle, error) {
	// жёстко заданный набор (можно читать из файла path, но здесь для примера — константа)
	t := []triangle.Triangle{
		{A: "SOL", B: "USDT", C: "USDC"},
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

	// Если вы хотите динамически подтягивать из API:
	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		return t, nil // fallback на константу
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return t, nil
	}

	type symbolInfo struct {
		Base  string `json:"baseAsset"`
		Quote string `json:"quoteAsset"`
		Maker string `json:"makerCommission"`
		Taker string `json:"takerCommission"`
	}
	var payload struct {
		Symbols []symbolInfo `json:"symbols"`
	}
	if err := json.Unmarshal(body, &payload); err != nil {
		return t, nil
	}

	// Построим все треугольники нулевой комиссии (пример см. ранее)
	edges := make(map[string]map[string]bool)
	assets := make(map[string]bool)
	for _, s := range payload.Symbols {
		if s.Maker == "0" && s.Taker == "0" {
			if edges[s.Base] == nil {
				edges[s.Base] = make(map[string]bool)
			}
			edges[s.Base][s.Quote] = true
			assets[s.Base] = true
			assets[s.Quote] = true
		}
	}
	// генерация циклов длины 3 (треугольников)
	var toks []string
	for a := range assets {
		toks = append(toks, a)
	}
	for i := 0; i < len(toks); i++ {
		for j := i + 1; j < len(toks); j++ {
			for k := j + 1; k < len(toks); k++ {
				A, B, C := toks[i], toks[j], toks[k]
				perms := [][3]string{
					{A, B, C}, {A, C, B},
					{B, A, C}, {B, C, A},
					{C, A, B}, {C, B, A},
				}
				for _, p := range perms {
					if edges[p[0]][p[1]] && edges[p[1]][p[2]] && edges[p[2]][p[0]] {
						t = append(t, triangle.Triangle{A: p[0], B: p[1], C: p[2]})
						break
					}
				}
			}
		}
	}

	return t, nil
}



// internal/app/arbitrage.go
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
	// 1. Загружаем треугольники и фильтруем по доступности пар
	ts, err := filesystem.LoadTriangles(path)
	if err != nil {
		return nil, err
	}
	avail := ex.FetchAvailableSymbols()
	ts = triangle.Filter(ts, avail)

	// 2. Собираем мапу индексов и список всех потенциальных подписок
	trianglesByPair := make(map[string][]int)
	var subPairs []string
	for i, tri := range ts {
		ab := tri.A + tri.B // A→B
		bc := tri.B + tri.C // B→C
		ca := tri.C + tri.A // C→A
		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)
		subPairs = append(subPairs, ab, bc, ca)
	}

	// 3. Оставляем только реально существующие пары
	uniq := make(map[string]struct{}, len(subPairs))
	for _, p := range subPairs {
		if avail[p] {
			uniq[p] = struct{}{}
		}
	}
	subPairs = subPairs[:0]
	for p := range uniq {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] triangles=%d, subscribing on %d valid pairs: %v",
		len(ts), len(subPairs), subPairs,
	)

	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
	}

	// 4. Подписываемся чанками по maxPerConn пар
	const maxPerConn = 25
	for i := 0; i < len(subPairs); i += maxPerConn {
		end := i + maxPerConn
		if end > len(subPairs) {
			end = len(subPairs)
		}
		chunk := subPairs[i:end]
		go func(pairs []string) {
			for {
				if err := ex.SubscribeDeals(pairs, arb.HandleRaw); err != nil {
					log.Printf("[WS][%s] subscribe chunk error: %v; retrying…", ex.Name(), err)
					time.Sleep(time.Second)
					continue
				}
				return
			}
		}(chunk)
	}

	return arb, nil
}

func (a *Arbitrager) HandleRaw(exchangeName string, raw []byte) {
	// 1) Парсим WS-сообщение
	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct{ Price string `json:"p"` } `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("[ERROR][%s] unmarshal raw: %v", exchangeName, err)
		return
	}
	if msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		return
	}

	// 2) Конвертируем цену
	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		log.Printf("[ERROR][%s] parse price %q: %v", exchangeName, msg.Data.Deals[0].Price, err)
		return
	}

	// 3) Сохраняем цену и логируем тик
	a.mu.Lock()
	a.latest[msg.Symbol] = price
	log.Printf("[TICK][%s] %s=%.8f", exchangeName, msg.Symbol, price)
	a.mu.Unlock()

	// 4) Проверяем все треугольники, где участвует этот symbol
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
		ab := tri.A + tri.B // A→B
		bc := tri.B + tri.C // B→C
		ca := tri.C + tri.A // C→A

		p1, ok1 := a.latest[ab] // price of B per A
		p2, ok2 := a.latest[bc] // price of C per B
		p3, ok3 := a.latest[ca] // price of A per C

		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		// Profit factor = p1 * p2 * p3 * net_fees
		profitFactor := p1 * p2 * p3 * nf
		profit := (profitFactor - 1) * 100

		if profit > 0 {
			a.sumProfit += profit
			log.Printf("🔺 ARB %s/%s/%s profit=%.4f%% total=%.4f%%",
				tri.A, tri.B, tri.C, profit, a.sumProfit)
		}
	}
}














