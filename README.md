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
list LoadTrianglesFromSymbols


File: cryptarb
Build ID: 89faeb18e3964e33a0ca535fd97aefb0dae94650
Type: inuse_space
Time: 2025-08-05 15:49:05 MSK
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 1538.28kB, 100% of 1538.28kB total
Showing top 10 nodes out of 26
      flat  flat%   sum%        cum   cum%
    1026kB 66.70% 66.70%     1026kB 66.70%  runtime.allocm
  512.28kB 33.30%   100%   512.28kB 33.30%  encoding/pem.Decode
         0     0%   100%   512.28kB 33.30%  crypto/tls.(*Conn).HandshakeContext
         0     0%   100%   512.28kB 33.30%  crypto/tls.(*Conn).clientHandshake
         0     0%   100%   512.28kB 33.30%  crypto/tls.(*Conn).handshakeContext
         0     0%   100%   512.28kB 33.30%  crypto/tls.(*Conn).verifyServerCertificate
         0     0%   100%   512.28kB 33.30%  crypto/tls.(*clientHandshakeStateTLS13).handshake
         0     0%   100%   512.28kB 33.30%  crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate
         0     0%   100%   512.28kB 33.30%  crypto/x509.(*CertPool).AppendCertsFromPEM
         0     0%   100%   512.28kB 33.30%  crypto/x509.(*Certificate).Verify
(pprof) 




SubscribeDeals(ctx context.Context, pairs []string, handler func(exchange string, raw []byte)) error





Showing nodes accounting for 3100.92kB, 100% of 3100.92kB total
Showing top 10 nodes out of 38
      flat  flat%   sum%        cum   cum%
 1050.86kB 33.89% 33.89%  1050.86kB 33.89%  cryptarb/internal/repository/filesystem.ExpandAvailableSymbols
    1026kB 33.09% 66.98%     1026kB 33.09%  runtime.allocm
  512.05kB 16.51% 83.49%   512.05kB 16.51%  runtime.acquireSudog
  512.02kB 16.51%   100%   512.02kB 16.51%  syscall.anyToSockaddr
         0     0%   100%   512.02kB 16.51%  cryptarb/internal/app.(*Arbitrager).subscriptionLoop
         0     0%   100%  1050.86kB 33.89%  cryptarb/internal/app.New
         0     0%   100%   512.02kB 16.51%  cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals
         0     0%   100%   512.02kB 16.51%  github.com/gorilla/websocket.(*Dialer).Dial (inline)
         0     0%   100%   512.02kB 16.51%  github.com/gorilla/websocket.(*Dialer).DialContext
         0     0%   100%   512.02kB 16.51%  github.com/gorilla/websocket.(*Dialer).DialContext.func4
(pprof) 




package app

import (
	"bytes"
	"fmt"
	"log"
	"strconv"
	"strings"
	"sync"
	"time"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

// flip возвращает инвертированный символ (например, "BTCUSDT" → "USDTBTC").
// В реальном коде может потребоваться более точное разделение на актив- и ценовую части.
func flip(sym string) string {
	n := len(sym)
	mid := n / 2
	return sym[mid:] + sym[:mid]
}

// Arbitrager ищет треугольные арбитражные возможности на бирже.
type Arbitrager struct {
	Triangles       []triangle.Triangle // Список всех треугольников
	latest          map[string]float64  // Последние цены по символам
	trianglesByPair map[string][]int    // Индексы треугольников по паре
	realSymbols     map[string]bool     // Карта реально доступных символов
	stepSizes       map[string]float64  // Шаги лотов
	minQtys         map[string]float64  // Мин. объёмы

	msgCh chan []byte    // Канал для сырых WS-сообщений
	wg    sync.WaitGroup // Для ожидания завершения воркеров

	mu          sync.Mutex
	exchange    exchange.Exchange
	StartAmount float64
}

// New создаёт и инициализирует арбитражер.
func New(ex exchange.Exchange) (*Arbitrager, error) {
	// 1) Получаем все доступные символы и параметры лотов
	rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()
	log.Printf("📊 Получено сырьёвых символов: %d", len(rawSymbols))

	// 2) Загружаем треугольники
	ts, err := filesystem.LoadTrianglesFromSymbols(rawSymbols)
	if err != nil {
		return nil, fmt.Errorf("LoadTriangles: %w", err)
	}
	log.Printf("[INIT] Треугольников найдено: %d", len(ts))

	// 3) Индексируем треугольники по каждой паре (AB, BC, CA)
	trianglesByPair := make(map[string][]int, len(ts)*3)
	var subRaw []string
	subRaw = make([]string, 0, len(ts)*3)
	for i, tri := range ts {
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ca := tri.C + tri.A
		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)
		subRaw = append(subRaw, ab, bc, ca)
	}
	log.Printf("[INIT] Индекс по парам: %d ключей", len(trianglesByPair))

	// 4) Фильтруем только реально доступные пары и убираем дубликаты
	uniq := make(map[string]struct{}, len(subRaw))
	for _, p := range subRaw {
		if rawSymbols[p] || rawSymbols[flip(p)] {
			uniq[p] = struct{}{}
		}
	}
	subPairs := make([]string, 0, len(uniq))
	for p := range uniq {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] Реальных пар для подписки: %d", len(subPairs))

	// 5) Инициализируем Arbitrager
	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64, len(subPairs)),
		trianglesByPair: trianglesByPair,
		realSymbols:     make(map[string]bool, len(subPairs)),
		stepSizes:       stepSizes,
		minQtys:         minQtys,
		msgCh:           make(chan []byte, 100),
		exchange:        ex,
		StartAmount:     0.5,
	}
	for _, p := range subPairs {
		arb.realSymbols[p] = true
	}

	// 6) Запускаем пул воркеров
	const workerCount = 4
	arb.wg.Add(workerCount)
	for i := 0; i < workerCount; i++ {
		go func() {
			defer arb.wg.Done()
			for raw := range arb.msgCh {
				arb.HandleRaw(ex.Name(), raw)
			}
		}()
	}

	// 7) Единая WS-подписка
	go func() {
		for {
			err := ex.SubscribeDeals(subPairs, func(_ string, raw []byte) {
				arb.msgCh <- raw
			})
			if err != nil {
				log.Printf("[WS][%s] subscribe error: %v, retrying...", ex.Name(), err)
				time.Sleep(time.Second)
				continue
			}
			log.Printf("[WS][%s] subscribed to %d channels", ex.Name(), len(subPairs))
			return
		}
	}()

	return arb, nil
}

// Stop корректно завершает работу: останавливает WS-подписку и ждёт воркеров.
func (a *Arbitrager) Stop() {
	close(a.msgCh)
	a.wg.Wait()
}

// normalizeSymbolDir собирает правильный символ и указывает, нужно ли инвертировать цену.
// Убираем здесь блокировки — метод зовётся внутри Check под мьютексом.
func (a *Arbitrager) normalizeSymbolDir(base, quote string) (symbol string, ok bool, invert bool) {
	if a.realSymbols[base+quote] {
		return base + quote, true, false
	}
	if a.realSymbols[quote+base] {
		return quote + base, true, true
	}
	return "", false, false
}

func (a *Arbitrager) HandleRaw(_exchange string, raw []byte) {
	const (
		idKey      = `"id":`
		code0Key   = `"code":0`
		sKey       = `"s":"`
		pKey       = `"p":"`
		prefixFail = "Not Subscribed successfully! ["
	)
	// 1) Обработка ACK-ошибки подписки
	if bytes.Contains(raw, []byte(idKey)) && bytes.Contains(raw, []byte(code0Key)) && !bytes.Contains(raw, []byte(sKey)) {
		if start := bytes.Index(raw, []byte(prefixFail)); start >= 0 {
			start += len(prefixFail)
			if end := bytes.Index(raw[start:], []byte("].  Reason")); end > 0 {
				list := raw[start : start+end]
				for _, ch := range strings.Split(string(list), ",") {
					if idx := strings.LastIndex(ch, "@"); idx != -1 {
						sym := ch[idx+1:]
						a.mu.Lock()
						a.realSymbols[sym] = false
						a.mu.Unlock()
					}
				}
			}
		}
		return
	}
	// 2) Парсим символ
	i := bytes.Index(raw, []byte(sKey))
	if i < 0 {
		return
	}
	i += len(sKey)
	j := bytes.IndexByte(raw[i:], '"')
	if j < 0 {
		return
	}
	sym := string(raw[i : i+j])
	// 3) Парсим цену
	i = bytes.Index(raw, []byte(pKey))
	if i < 0 {
		return
	}
	i += len(pKey)
	j = bytes.IndexByte(raw[i:], '"')
	if j < 0 {
		return
	}
	price, err := strconv.ParseFloat(string(raw[i:i+j]), 64)
	if err != nil {
		return
	}
	// 4) Проверка и обновление под мьютексом
	a.mu.Lock()
	alive, ok := a.realSymbols[sym]
	_, hasTri := a.trianglesByPair[sym]
	if !ok || !alive || !hasTri {
		a.mu.Unlock()
		return
	}
	a.latest[sym] = price
	a.mu.Unlock()
	// 5) Поиск арбитража
	a.Check(sym)
}

// Check проверяет все треугольники, связанные с символом.
// Весь метод выполняется под мьютексом, чтобы избежать concurrent map read/write.
func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		return
	}

	// С учётом торговых комиссий
	nf := 0.9965 * 0.9965 * 0.9965

	for _, idx := range indices {
		tri := a.Triangles[idx]

		ab, ok1, rev1 := a.normalizeSymbolDir(tri.A, tri.B)
		bc, ok2, rev2 := a.normalizeSymbolDir(tri.B, tri.C)
		ca, ok3, rev3 := a.normalizeSymbolDir(tri.C, tri.A)
		if !ok1 || !ok2 || !ok3 {
			continue
		}

		p1, ex1 := a.latest[ab]
		p2, ex2 := a.latest[bc]
		p3, ex3 := a.latest[ca]
		if !ex1 || !ex2 || !ex3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		if rev1 {
			p1 = 1 / p1
		}
		if rev2 {
			p2 = 1 / p2
		}
		if rev3 {
			p3 = 1 / p3
		}

		profit := (p1*p2*p3*nf - 1) * 100
		//	if profit > 0 && tri.A == "USDT" {
		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
		//	}
	}

}


