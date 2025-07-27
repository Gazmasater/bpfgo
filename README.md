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
	"fmt"
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

	// Запускаем WS с авто-переподключением
	go func() {
		pairs := triangle.SymbolPairs(ts)
		for {
			err := ex.SubscribeDeals(pairs, arb.HandleRaw)
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
	log.Printf("[RAW][%s] %s", exchangeName, string(raw))

	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("[ERROR][%s] unmarshal raw: %v", exchangeName, err)
		return
	}
	if msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		log.Printf("[SKIP][%s] empty symbol or no deals", exchangeName)
		return
	}

	priceStr := msg.Data.Deals[0].Price
	price, err := strconv.ParseFloat(priceStr, 64)
	if err != nil {
		log.Printf("[ERROR][%s] parse price %q: %v", exchangeName, priceStr, err)
		return
	}

	a.mu.Lock()
	a.latest[msg.Symbol] = price
	rev := reverseSymbol(msg.Symbol)
	a.latest[rev] = 1 / price
	log.Printf("[TICK][%s] %s=%.8f %s=%.8f", exchangeName, msg.Symbol, price, rev, 1/price)
	a.mu.Unlock()

	a.Check(msg.Symbol)
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	log.Printf("[CHECK] symbol=%s indices=%v", symbol, indices)
	if len(indices) == 0 {
		return
	}

	const commission = 0.0005
	nf := (1 - commission) * (1 - commission) * (1 - commission)

	for _, i := range indices {
		tri := a.Triangles[i]
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ac := tri.A + tri.C

		p1, ok1 := a.latest[ab]
		p2, ok2 := a.latest[bc]
		p3, ok3 := a.latest[ac]

		log.Printf("[DATA] tri=%s/%s/%s AB=%v(ok=%v) BC=%v(ok=%v) AC=%v(ok=%v)",
			tri.A, tri.B, tri.C,
			p1, ok1, p2, ok2, p3, ok3,
		)

		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			log.Printf("[SKIP] incomplete for %v", tri)
			continue
		}

		profit := (p1*p2/p3*nf - 1) * 100
		a.sumProfit += profit
		log.Printf("🔺 ARB %s/%s/%s profit=%.3f%% total=%.3f%%", tri.A, tri.B, tri.C, profit, a.sumProfit)
	}
}

func reverseSymbol(sym string) string {
	if len(sym)%2 != 0 {
		return sym
	}
	half := len(sym) / 2
	return sym[half:] + sym[:half]
}



gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/07/27 17:18:47 [RAW][MEXC] {"id":1753625926,"code":0,"msg":"Subscribed successful! [spot@public.deals.v3.api@SOLUSDT,spot@public.deals.v3.api@SOLUSDC]. Not Subscribed successfully! [spot@public.deals.v3.api@USDTUSDC].  Reason： Blocked! "}
2025/07/27 17:18:47 [SKIP][MEXC] empty symbol or no deals
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDC","d":{"deals":[{"p":"186.2964","v":"0.02","S":1,"t":1753625927133}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDC","t":1753625927137}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDC=186.29640000 SOLUSDC=0.00536779
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDC indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0(ok=false) BC=0(ok=false) AC=0.005367790252522324(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"186.37","v":"13.180","S":2,"t":1753625927152}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753625927155}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDT=186.37000000 SOLUSDT=0.00536567
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDT indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367790252522324(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"186.37","v":"0.180","S":2,"t":1753625927152}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753625927155}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDT=186.37000000 SOLUSDT=0.00536567
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDT indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367790252522324(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"186.37","v":"7.020","S":2,"t":1753625927152}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753625927155}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDT=186.37000000 SOLUSDT=0.00536567
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDT indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367790252522324(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDC","d":{"deals":[{"p":"186.2972","v":"0.05","S":1,"t":1753625927634}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDC","t":1753625927637}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDC=186.29720000 SOLUSDC=0.00536777
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDC indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367767202083552(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDC","d":{"deals":[{"p":"186.2972","v":"0.19","S":1,"t":1753625927634}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDC","t":1753625927638}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDC=186.29720000 SOLUSDC=0.00536777
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDC indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367767202083552(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDC","d":{"deals":[{"p":"186.2972","v":"0.06","S":2,"t":1753625927634}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDC","t":1753625927638}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDC=186.29720000 SOLUSDC=0.00536777
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDC indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367767202083552(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDC","d":{"deals":[{"p":"186.2971","v":"0.19","S":2,"t":1753625927635}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDC","t":1753625927638}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDC=186.29710000 SOLUSDC=0.00536777
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDC indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367770083377573(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDC","d":{"deals":[{"p":"186.2971","v":"0.12","S":2,"t":1753625927636}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDC","t":1753625927640}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDC=186.29710000 SOLUSDC=0.00536777
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDC indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365670440521543(ok=true) BC=0(ok=false) AC=0.005367770083377573(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"186.39","v":"1.200","S":2,"t":1753625927651}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753625927655}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDT=186.39000000 SOLUSDT=0.00536509
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDT indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365094693921348(ok=true) BC=0(ok=false) AC=0.005367770083377573(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
2025/07/27 17:18:47 [RAW][MEXC] {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"186.39","v":"0.930","S":2,"t":1753625927652}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1753625927655}
2025/07/27 17:18:47 [TICK][MEXC] SOLUSDT=186.39000000 SOLUSDT=0.00536509
2025/07/27 17:18:47 [CHECK] symbol=SOLUSDT indices=[0]
2025/07/27 17:18:47 [DATA] tri=SOL/USDT/USDC AB=0.005365094693921348(ok=true) BC=0(ok=false) AC=0.005367770083377573(ok=true)
2025/07/27 17:18:47 [SKIP] incomplete for {SOL USDT USDC}
^Csignal: interrupt







