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






Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 100ms, 100% of 100ms total
Showing top 10 nodes out of 44
      flat  flat%   sum%        cum   cum%
      30ms 30.00% 30.00%       30ms 30.00%  internal/runtime/syscall.Syscall6
      20ms 20.00% 50.00%       20ms 20.00%  runtime.futex
      10ms 10.00% 60.00%       10ms 10.00%  aeshashbody
      10ms 10.00% 70.00%       10ms 10.00%  bytes.(*Reader).Len (inline)
      10ms 10.00% 80.00%       10ms 10.00%  indexbody
      10ms 10.00% 90.00%       10ms 10.00%  runtime.nanotime
      10ms 10.00%   100%       10ms 10.00%  runtime.releasep
         0     0%   100%       60ms 60.00%  bufio.(*Reader).Peek
         0     0%   100%       60ms 60.00%  bufio.(*Reader).fill
         0     0%   100%       50ms 50.00%  bytes.(*Buffer).ReadFrom
(pprof) 


az358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
Fetching profile over HTTP from http://localhost:6060/debug/pprof/profile?seconds=30
Saved profile in /home/gaz358/pprof/pprof.cryptarb.samples.cpu.017.pb.gz
File: cryptarb
Build ID: d58f3c0cd337874a0c993c0cb2d2bb8860416ca1
Type: cpu
Time: 2025-08-05 01:36:56 MSK
Duration: 30s, Total samples = 100ms ( 0.33%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 100ms, 100% of 100ms total
Showing top 10 nodes out of 59
      flat  flat%   sum%        cum   cum%
      30ms 30.00% 30.00%       30ms 30.00%  internal/runtime/syscall.Syscall6
      20ms 20.00% 50.00%       20ms 20.00%  runtime.futex
      10ms 10.00% 60.00%       10ms 10.00%  crypto/internal/fips140/aes/gcm.gcmAesDec
      10ms 10.00% 70.00%       10ms 10.00%  crypto/tls.(*halfConn).incSeq (inline)
      10ms 10.00% 80.00%       10ms 10.00%  internal/poll.(*fdMutex).rwlock
      10ms 10.00% 90.00%       10ms 10.00%  runtime.memmove
      10ms 10.00%   100%       20ms 20.00%  runtime.netpoll
         0     0%   100%       50ms 50.00%  bufio.(*Reader).Peek
         0     0%   100%       50ms 50.00%  bufio.(*Reader).fill
         0     0%   100%       30ms 30.00%  bytes.(*Buffer).ReadFrom
(pprof) 


az358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
Fetching profile over HTTP from http://localhost:6060/debug/pprof/profile?seconds=30
Saved profile in /home/gaz358/pprof/pprof.cryptarb.samples.cpu.017.pb.gz
File: cryptarb
Build ID: d58f3c0cd337874a0c993c0cb2d2bb8860416ca1
Type: cpu
Time: 2025-08-05 01:36:56 MSK
Duration: 30s, Total samples = 100ms ( 0.33%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 100ms, 100% of 100ms total
Showing top 10 nodes out of 59
      flat  flat%   sum%        cum   cum%
      30ms 30.00% 30.00%       30ms 30.00%  internal/runtime/syscall.Syscall6
      20ms 20.00% 50.00%       20ms 20.00%  runtime.futex
      10ms 10.00% 60.00%       10ms 10.00%  crypto/internal/fips140/aes/gcm.gcmAesDec
      10ms 10.00% 70.00%       10ms 10.00%  crypto/tls.(*halfConn).incSeq (inline)
      10ms 10.00% 80.00%       10ms 10.00%  internal/poll.(*fdMutex).rwlock
      10ms 10.00% 90.00%       10ms 10.00%  runtime.memmove
      10ms 10.00%   100%       20ms 20.00%  runtime.netpoll
         0     0%   100%       50ms 50.00%  bufio.(*Reader).Peek
         0     0%   100%       50ms 50.00%  bufio.(*Reader).fill
         0     0%   100%       30ms 30.00%  bytes.(*Buffer).ReadFrom
(pprof) 






// PairInfo содержит заранее вычисленные конкатенации
type PairInfo struct {
	Base         string
	Quote        string
	Symbol       string // base+quote
	Reverse      string // quote+base
	WSChannel    string // spot@public.deals.v3.api@symbol
}

// Arbitrager структура
type Arbitrager struct {
	Triangles        []triangle.Triangle
	latest           map[string]float64
	trianglesByPair  map[string][]int
	realSymbols      map[string]bool
	stepSizes        map[string]float64
	minQtys          map[string]float64
	mu               sync.Mutex
	StartAmount      float64
	exchange         exchange.Exchange
	pairsInfo        map[string]PairInfo // Новое поле с заранее вычисленными данными
}




func New(ex exchange.Exchange) (*Arbitrager, error) {
    // Получаем список символов и параметры лотов
    rawSymbols, stepSizes, minQtys := ex.FetchAvailableSymbols()
    avail := filesystem.ExpandAvailableSymbols(rawSymbols)
    log.Printf("📊 Доступные пары (с инверсиями): %d", len(avail))

    // Строим все возможные треугольники
    ts, err := filesystem.LoadTrianglesFromSymbols(avail)
    if err != nil {
        return nil, fmt.Errorf("LoadTriangles: %w", err)
    }
    log.Printf("[INIT] Треугольников найдено: %d", len(ts))

    // Для отладки сохраняем дамп
    if data, err := json.MarshalIndent(ts, "", "  "); err == nil {
        _ = os.WriteFile("triangles_dump.json", data, 0644)
    }

    // Индексация треугольников по паре
    trianglesByPair := make(map[string][]int, len(ts)*3)
    // У каждого треугольника ровно 3 пары, резервируем buffer сразу
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

    // Формируем список уникальных реальных пар для подписки
    uniq := make(map[string]struct{}, len(subRaw))
    for _, p := range subRaw {
        if avail[p] {
            uniq[p] = struct{}{}
        }
    }
    subPairs := make([]string, 0, len(uniq))
    for p := range uniq {
        subPairs = append(subPairs, p)
    }
    log.Printf("[INIT] Подписка на пар: %d шт.", len(subPairs))

    // Инициализируем арбитражёра
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

    // WS подписки чанками по 20
    const maxPerConn = 20
    for i := 0; i < len(subPairs); i += maxPerConn {
        end := i + maxPerConn
        if end > len(subPairs) {
            end = len(subPairs)
        }
        chunk := subPairs[i:end]
        go func(pairs []string) {
            for {
                err := ex.SubscribeDeals(pairs, arb.HandleRaw)
                if err != nil {
                    log.Printf("[WS][%s] subscribe error: %v, retrying...", ex.Name(), err)
                    time.Sleep(time.Second)
                    continue
                }
                log.Printf("[WS][%s] subscribed to channels: %v", ex.Name(), pairs)
                return
            }
        }(chunk)
    }

    return arb, nil
}


az358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof http://localhost:6060/debug/pprof/heap
Fetching profile over HTTP from http://localhost:6060/debug/pprof/heap
Saved profile in /home/gaz358/pprof/pprof.cryptarb.alloc_objects.alloc_space.inuse_objects.inuse_space.001.pb.gz
File: cryptarb
Build ID: d58f3c0cd337874a0c993c0cb2d2bb8860416ca1
Type: inuse_space
Time: 2025-08-05 01:49:26 MSK
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 10485.18kB, 100% of 10485.18kB total
Showing top 10 nodes out of 45
      flat  flat%   sum%        cum   cum%
 3075.38kB 29.33% 29.33%  3075.38kB 29.33%  reflect.mapassign_faststr0
 2560.04kB 24.42% 53.75%  2560.04kB 24.42%  encoding/json.(*decodeState).literalStore
 1774.65kB 16.93% 70.67%  1774.65kB 16.93%  io.ReadAll
    1539kB 14.68% 85.35%     1539kB 14.68%  runtime.allocm
  512.05kB  4.88% 90.23%   512.05kB  4.88%  runtime.(*scavengerState).init
  512.05kB  4.88% 95.12%   512.05kB  4.88%  runtime.acquireSudog
  512.02kB  4.88%   100%   512.02kB  4.88%  syscall.anyToSockaddr
         0     0%   100%  7410.07kB 70.67%  cryptarb/internal/app.New
         0     0%   100%  7410.07kB 70.67%  cryptarb/internal/repository/mexc.(*MexcExchange).FetchAvailableSymbols
         0     0%   100%  5635.42kB 53.75%  encoding/json.(*decodeState).array
(pprof) 




