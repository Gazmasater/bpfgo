
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

sudo apt install ntpdate
sudo ntpdate time.cloudflare.com
curl -s https://api.mexc.com/api/v3/time
date +%s%3N




syntax = "proto3";
option go_package = "crypt_proto/pb";


package app

import (
	"bytes"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
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
	realSymbols     map[string]bool
	stepSizes       map[string]float64
	minQtys         map[string]float64
	mu              sync.Mutex
	StartAmount     float64
	exchange        exchange.Exchange
}

func New(ex exchange.Exchange) (*Arbitrager, error) {
	rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()
	avail := filesystem.ExpandAvailableSymbols(rawSymbols)
	log.Printf("📊 Доступные пары (с инверсиями): %d", len(avail))

	ts, err := filesystem.LoadTrianglesFromSymbols(avail)
	if err != nil {
		return nil, fmt.Errorf("LoadTriangles: %w", err)
	}
	log.Printf("[INIT] Треугольников найдено: %d", len(ts))

	if data, err := json.MarshalIndent(ts, "", "  "); err == nil {
		_ = os.WriteFile("triangles_dump.json", data, 0644)
	}

	trianglesByPair := make(map[string][]int, len(ts)*3)
	subRaw := make([]string, 0, len(ts)*3)

	for i, tri := range ts {
		ab := tri.A + tri.B
		bc := tri.B + tri.C
		ca := tri.C + tri.A

		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)

		subRaw = append(subRaw, ab, bc, ca)
	}
	log.Printf("[INIT] Составили индекс по парам: %d ключей", len(trianglesByPair))

	uniq := make(map[string]struct{}, len(subRaw))
	invalid := make([]string, 0)

	for _, p := range subRaw {
		if avail[p] {
			uniq[p] = struct{}{}
		} else {
			invalid = append(invalid, p)
		}
	}

	subPairs := make([]string, 0, len(uniq))
	for p := range uniq {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] Пары для подписки: %d шт.", len(subPairs))

	if len(invalid) > 0 {
		_ = os.WriteFile("excluded_pairs.log", []byte(strings.Join(invalid, "\n")), 0644)
		log.Printf("⚠️ Исключено %d неподходящих пар (см. excluded_pairs.log)", len(invalid))
	}

	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64, len(subPairs)),
		trianglesByPair: trianglesByPair,
		realSymbols:     avail,
		stepSizes:       stepSizes,
		minQtys:         minQtys,
		StartAmount:     0.5,
		exchange:        ex,
	}

	const maxPerConn = 20
	for i := 0; i < len(subPairs); i += maxPerConn {
		end := i + maxPerConn
		if end > len(subPairs) {
			end = len(subPairs)
		}
		chunk := subPairs[i:end]
		go func(idx int, pairs []string) {
			for {
				err := ex.SubscribeDeals(pairs, arb.HandleRaw)
				if err != nil {
					log.Printf("[WS][%s] ❌ Подписка #%d: %v, повтор через 1с...", ex.Name(), idx, err)
					time.Sleep(time.Second)
					continue
				}
				log.Printf("[WS][%s] ✅ Подписка #%d активна: %v", ex.Name(), idx, pairs)
				return
			}
		}(i/maxPerConn+1, chunk)
	}

	return arb, nil
}

func (a *Arbitrager) normalizeSymbolDir(base, quote string) (symbol string, ok bool, invert bool) {
	if a.realSymbols[base+quote] {
		return base + quote, true, false
	}
	if a.realSymbols[quote+base] {
		return quote + base, true, true
	}
	return "", false, false
}

var (
	idKey      = []byte(`"id":`)
	code0Key   = []byte(`"code":0`)
	sKey       = []byte(`"s":"`)
	pKey       = []byte(`"p":"`)
	prefixFail = "Not Subscribed successfully! ["
)

func (a *Arbitrager) HandleRaw(_exchange string, raw []byte) {
	if bytes.Contains(raw, idKey) &&
		bytes.Contains(raw, code0Key) &&
		!bytes.Contains(raw, sKey) {

		start := bytes.Index(raw, []byte(prefixFail))
		if start >= 0 {
			start += len(prefixFail)
			end := bytes.Index(raw[start:], []byte("].  Reason"))
			if end > 0 {
				blockedList := raw[start : start+end]
				for _, ch := range strings.Split(string(blockedList), ",") {
					if idx := strings.LastIndex(ch, "@"); idx != -1 {
						sym := ch[idx+1:]

						a.mu.Lock()
						a.realSymbols[sym] = false
						a.mu.Unlock()

						log.Printf("🚫 [MEXC] Символ отключён сервером: %s", sym)

						f, err := os.OpenFile("blocked_pairs.log", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
						if err == nil {
							_, _ = f.WriteString(sym + "\n")
							_ = f.Close()
						}
					}
				}
			} else {
				log.Printf("⚠️ [RAW] Подписка отклонена без Reason: %s", raw)
			}
		} else {
			log.Printf("⚠️ [RAW] Не удалось распарсить ошибку подписки: %s", raw)
		}
		return
	}

	i := bytes.Index(raw, sKey)
	if i < 0 {
		log.Printf("⚠️ [RAW] Нет поля 's': %s", raw)
		return
	}
	i += len(sKey)
	j := bytes.IndexByte(raw[i:], '"')
	if j < 0 {
		log.Printf("⚠️ [RAW] Ошибка разбора символа: %s", raw)
		return
	}
	sym := string(raw[i : i+j])

	if ok, ex := a.realSymbols[sym]; !ex || !ok {
		log.Printf("⛔ [RAW] Неизвестный или отключённый символ: %s", sym)
		return
	}
	if _, ex := a.trianglesByPair[sym]; !ex {
		log.Printf("🔕 [RAW] Символ не используется в треугольниках: %s", sym)
		return
	}

	i = bytes.Index(raw, pKey)
	if i < 0 {
		log.Printf("⚠️ [RAW] Нет поля 'p': %s", raw)
		return
	}
	i += len(pKey)
	j = bytes.IndexByte(raw[i:], '"')
	if j < 0 {
		log.Printf("⚠️ [RAW] Ошибка разбора цены: %s", raw)
		return
	}
	priceBytes := raw[i : i+j]
	price, err := strconv.ParseFloat(string(priceBytes), 64)
	if err != nil {
		log.Printf("⚠️ [RAW] Не удалось распарсить цену: %s", raw)
		return
	}

	a.mu.Lock()
	a.latest[sym] = price
	a.mu.Unlock()

	a.Check(sym)
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		return
	}

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
		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
	}
}






