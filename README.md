
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


2025/08/07 09:51:40 📨 [MEXC] Ответ: {"id":1754549499,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@OPUSDT,spot@public.deals.v3.api@BTCETH,spot@public.deals.v3.api@USDCBTC,spot@public.deals.v3.api@SOLUSDT,spot@public.deals.v3.api@ENSUSDC,spot@public.deals.v3.api@USDCXRP,spot@public.deals.v3.api@USDCWBTC,spot@public.deals.v3.api@USDTUNI,spot@public.deals.v3.api@USDCCAW,spot@public.deals.v3.api@USDCTRX,spot@public.deals.v3.api@USDCAVAX,spot@public.deals.v3.api@JASMYUSDT,spot@public.deals.v3.api@USDTMX,spot@public.deals.v3.api@TRXUSDT,spot@public.deals.v3.api@ATOMUSDT,spot@public.deals.v3.api@MINAUSDT,spot@public.deals.v3.api@USDTFIL,spot@public.deals.v3.api@FTTUSDC,spot@public.deals.v3.api@ETHUNI,spot@public.deals.v3.api@DOGEUSDT].  Reason： Blocked! "}
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: OPUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: BTCETH
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCBTC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: SOLUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: ENSUSDC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCXRP
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCWBTC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTUNI
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCCAW
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCTRX
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCAVAX
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: JASMYUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTMX
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: TRXUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: ATOMUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: MINAUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTFIL
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: FTTUSDC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: ETHUNI
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: DOGEUSDT
2025/08/07 09:51:40 📨 [MEXC] Ответ: {"id":1754549500,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@APEUSDT,spot@public.deals.v3.api@MXUSDT,spot@public.deals.v3.api@USDTNEAR,spot@public.deals.v3.api@XRPUSDC,spot@public.deals.v3.api@RAYUSDT,spot@public.deals.v3.api@ADAUSDT,spot@public.deals.v3.api@USDCUNI,spot@public.deals.v3.api@USDTLUNC,spot@public.deals.v3.api@MXUSDC,spot@public.deals.v3.api@BTCSOL,spot@public.deals.v3.api@CAWUSDT,spot@public.deals.v3.api@BTCATOM,spot@public.deals.v3.api@BNBUSDT,spot@public.deals.v3.api@USDTXRP,spot@public.deals.v3.api@AVAXUSDT,spot@public.deals.v3.api@USDTAPE,spot@public.deals.v3.api@RAYUSDC,spot@public.deals.v3.api@ADAUSDC,spot@public.deals.v3.api@USDCSHIB,spot@public.deals.v3.api@USDCWAVES].  Reason： Blocked! "}
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: APEUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: MXUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTNEAR
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: XRPUSDC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: RAYUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: ADAUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCUNI
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTLUNC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: MXUSDC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: BTCSOL
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: CAWUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: BTCATOM
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: BNBUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTXRP
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: AVAXUSDT
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDTAPE
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: RAYUSDC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: ADAUSDC
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCSHIB
2025/08/07 09:51:40 🚫 [MEXC] Символ отключён сервером: USDCWAVES
2025/08/07 09:51:40 🌐 [MEXC] Подключаемся к wss://wbs.mexc.com/ws
2025/08/07 09:51:40 ✅ [MEXC] Соединение установлено
2025/08/07 09:51:40 📩 [MEXC] Подписка отправлена: [USDCATOM USDTCAW BCHUSDC BTCXRP ALGOUSDC SOLUSDC USDCAZERO USDCBCH USDTXEN CAWUSDC LTCBTC USDCMX USDTMINA BTCTRX ATOMBTC USDCMINA LTCUSDC USDTJASMY OPUSDC USDCFTT]
2025/08/07 09:51:41 🌐 [MEXC] Подключаемся к wss://wbs.mexc.com/ws
2025/08/07 09:51:41 📨 [MEXC] Ответ: {"id":1754549500,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@CAWUSDC,spot@public.deals.v3.api@USDCFTT,spot@public.deals.v3.api@USDCBCH,spot@public.deals.v3.api@USDTXEN,spot@public.deals.v3.api@USDCAZERO,spot@public.deals.v3.api@USDTCAW,spot@public.deals.v3.api@BCHUSDC,spot@public.deals.v3.api@USDTJASMY,spot@public.deals.v3.api@USDCMX,spot@public.deals.v3.api@BTCTRX,spot@public.deals.v3.api@ALGOUSDC,spot@public.deals.v3.api@ATOMBTC,spot@public.deals.v3.api@USDCATOM,spot@public.deals.v3.api@LTCBTC,spot@public.deals.v3.api@BTCXRP,spot@public.deals.v3.api@LTCUSDC,spot@public.deals.v3.api@USDCMINA,spot@public.deals.v3.api@USDTMINA,spot@public.deals.v3.api@SOLUSDC,spot@public.deals.v3.api@OPUSDC].  Reason： Blocked! "}
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: CAWUSDC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDCFTT
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDCBCH
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDTXEN
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDCAZERO
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDTCAW
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: BCHUSDC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDTJASMY
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDCMX
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: BTCTRX
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: ALGOUSDC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: ATOMBTC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDCATOM
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: LTCBTC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: BTCXRP
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: LTCUSDC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDCMINA
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: USDTMINA
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: SOLUSDC
2025/08/07 09:51:41 🚫 [MEXC] Символ отключён сервером: OPUSDC
2025/08/07 09:51:41 🌐 [MEXC] Подключаемся к wss://wbs.mexc.com/ws
2025/08/07 09:51:41 ✅ [MEXC] Соединение установлено
2025/08/07 09:51:41 📩 [MEXC] Подписка отправлена: [USDCSOL WBTCUSDC ENSUSDT USDTWAVES USDTDOGE SOLBTC USDCAAVE FILUSDC USDTUSDC USDCALGO LTCUSDT USDTATOM NEARUSDC TRXBTC USDTAAVE USDCFIL XENUSDT UNIUSDT USDTAZERO ADABTC]
2025/08/07 09:51:42 ✅ [MEXC] Соединение установлено
2025/08/07 09:51:42 📩 [MEXC] Подписка отправлена: [BTCBCH WBTCUSDT LUNCUSDT AZEROUSDC AAVEUSDT USDCETH UNIUSDC BCHUSDT USDTSHIB WAVESUSDC AAVEUSDC AVAXUSDC USDTWBTC ETHBTC USDTBNB DOGEUSDC ATOMUSDC BTCLTC AZEROUSDT WAVESUSDT]
2025/08/07 09:51:42 📨 [MEXC] Ответ: {"id":1754549501,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@XENUSDT,spot@public.deals.v3.api@USDCFIL,spot@public.deals.v3.api@USDCSOL,spot@public.deals.v3.api@USDTUSDC,spot@public.deals.v3.api@NEARUSDC,spot@public.deals.v3.api@USDTWAVES,spot@public.deals.v3.api@USDTAAVE,spot@public.deals.v3.api@USDTAZERO,spot@public.deals.v3.api@SOLBTC,spot@public.deals.v3.api@USDCAAVE,spot@public.deals.v3.api@UNIUSDT,spot@public.deals.v3.api@WBTCUSDC,spot@public.deals.v3.api@FILUSDC,spot@public.deals.v3.api@LTCUSDT,spot@public.deals.v3.api@USDTDOGE,spot@public.deals.v3.api@ADABTC,spot@public.deals.v3.api@USDCALGO,spot@public.deals.v3.api@USDTATOM,spot@public.deals.v3.api@ENSUSDT,spot@public.deals.v3.api@TRXBTC].  Reason： Blocked! "}
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: XENUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDCFIL
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDCSOL
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: NEARUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTWAVES
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTAAVE
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTAZERO
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: SOLBTC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDCAAVE
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: UNIUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: WBTCUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: FILUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: LTCUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTDOGE
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: ADABTC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDCALGO
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTATOM
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: ENSUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: TRXBTC
2025/08/07 09:51:42 📨 [MEXC] Ответ: {"id":1754549502,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@UNIUSDC,spot@public.deals.v3.api@BTCBCH,spot@public.deals.v3.api@AVAXUSDC,spot@public.deals.v3.api@ATOMUSDC,spot@public.deals.v3.api@WBTCUSDT,spot@public.deals.v3.api@WAVESUSDC,spot@public.deals.v3.api@BTCLTC,spot@public.deals.v3.api@AAVEUSDC,spot@public.deals.v3.api@AZEROUSDC,spot@public.deals.v3.api@BCHUSDT,spot@public.deals.v3.api@USDTWBTC,spot@public.deals.v3.api@DOGEUSDC,spot@public.deals.v3.api@USDCETH,spot@public.deals.v3.api@WAVESUSDT,spot@public.deals.v3.api@LUNCUSDT,spot@public.deals.v3.api@USDTBNB,spot@public.deals.v3.api@USDTSHIB,spot@public.deals.v3.api@AZEROUSDT,spot@public.deals.v3.api@AAVEUSDT,spot@public.deals.v3.api@ETHBTC].  Reason： Blocked! "}
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: UNIUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: BTCBCH
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: AVAXUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: ATOMUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: WBTCUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: WAVESUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: BTCLTC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: AAVEUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: AZEROUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: BCHUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTWBTC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: DOGEUSDC
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDCETH
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: WAVESUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: LUNCUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTBNB
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: USDTSHIB
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: AZEROUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: AAVEUSDT
2025/08/07 09:51:42 🚫 [MEXC] Символ отключён сервером: ETHBTC
2025/08/07 09:51:42 🌐 [MEXC] Подключаемся к wss://wbs.mexc.com/ws
2025/08/07 09:51:42 🌐 [MEXC] Подключаемся к wss://wbs.mexc.com/ws
2025/08/07 09:51:42 ✅ [MEXC] Соединение установлено
2025/08/07 09:51:42 📩 [MEXC] Подписка отправлена: [FTTUSDT ALGOUSDT USDCRAY USDTENS]
2025/08/07 09:51:42 ✅ [MEXC] Соединение установлено
2025/08/07 09:51:42 📩 [MEXC] Подписка отправлена: [USDCBNB USDTOP UNIETH FILUSDT SHIBUSDT JASMYUSDC USDTLTC BTCUSDC USDCNEAR USDCENS USDCXEN USDCAPE USDCDOGE USDTRAY USDCLTC USDTALGO USDTAVAX USDTTRX LUNCUSDC MINAUSDC]
2025/08/07 09:51:43 📨 [MEXC] Ответ: {"id":1754549502,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@USDCRAY,spot@public.deals.v3.api@FTTUSDT,spot@public.deals.v3.api@USDTENS,spot@public.deals.v3.api@ALGOUSDT].  Reason： Blocked! "}
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCRAY
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: FTTUSDT
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTENS
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: ALGOUSDT
2025/08/07 09:51:43 📨 [MEXC] Ответ: {"id":1754549502,"code":0,"msg":"Not Subscribed successfully! [spot@public.deals.v3.api@USDTOP,spot@public.deals.v3.api@USDCNEAR,spot@public.deals.v3.api@JASMYUSDC,spot@public.deals.v3.api@USDCENS,spot@public.deals.v3.api@FILUSDT,spot@public.deals.v3.api@SHIBUSDT,spot@public.deals.v3.api@LUNCUSDC,spot@public.deals.v3.api@UNIETH,spot@public.deals.v3.api@USDCDOGE,spot@public.deals.v3.api@MINAUSDC,spot@public.deals.v3.api@USDTTRX,spot@public.deals.v3.api@USDTLTC,spot@public.deals.v3.api@USDCBNB,spot@public.deals.v3.api@USDCAPE,spot@public.deals.v3.api@USDCLTC,spot@public.deals.v3.api@USDTAVAX,spot@public.deals.v3.api@USDCXEN,spot@public.deals.v3.api@USDTRAY,spot@public.deals.v3.api@BTCUSDC,spot@public.deals.v3.api@USDTALGO].  Reason： Blocked! "}
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTOP
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCNEAR
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: JASMYUSDC
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCENS
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: FILUSDT
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: SHIBUSDT
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: LUNCUSDC
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: UNIETH
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCDOGE
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: MINAUSDC
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTTRX
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTLTC
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCBNB
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCAPE
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCLTC
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTAVAX
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDCXEN
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTRAY
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: BTCUSDC
2025/08/07 09:51:43 🚫 [MEXC] Символ отключён сервером: USDTALGO
2025/08/07 09:51:46 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent
2025/08/07 09:51:47 ❌ [MEXC] PING ошибка: websocket: close sent







