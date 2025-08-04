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



func (m *MexcExchange) FetchAvailableSymbols() (
    map[string]bool, map[string]float64, map[string]float64,
) {
    // 1) HTTP-клиент без gzip и без HTTP/2
    transport := &http.Transport{
        DisableCompression: true,
        TLSNextProto:       make(map[string]func(string, *tls.Conn) http.RoundTripper),
    }
    client := &http.Client{
        Timeout:   10 * time.Second,
        Transport: transport,
    }

    resp, err := client.Get("https://api.mexc.com/api/v3/exchangeInfo")
    if err != nil {
        log.Printf("❌ Ошибка запроса exchangeInfo: %v", err)
        return nil, nil, nil
    }
    defer resp.Body.Close()

    dec := json.NewDecoder(resp.Body)

    // Проматываем до ключа "symbols"
    for {
        tok, err := dec.Token()
        if err != nil {
            log.Printf("❌ Не нашли поле symbols: %v", err)
            return nil, nil, nil
        }
        if key, ok := tok.(string); ok && key == "symbols" {
            break
        }
    }
    // Ожидаем '['
    if _, err := dec.Token(); err != nil {
        log.Printf("❌ Ожидаем '[': %v", err)
        return nil, nil, nil
    }

    // Вспомогательная структура и слайс валидных
    type symInfo struct {
        Symbol               string `json:"symbol"`
        Status               string `json:"status"`
        IsSpotTradingAllowed bool   `json:"isSpotTradingAllowed"`
        BaseSizePrecision    string `json:"baseSizePrecision"`
    }
    type valid struct {
        sym  string
        step float64
    }
    valids := make([]valid, 0, 64) // пусть начальный cap будет небольшой

    // 2) Стримим по одному, фильтруем и собираем валидные
    for dec.More() {
        var s symInfo
        if err := dec.Decode(&s); err != nil {
            log.Printf("❌ Ошибка Decode: %v", err)
            return nil, nil, nil
        }
        if s.Status != "1" || !s.IsSpotTradingAllowed {
            continue
        }
        step, err := strconv.ParseFloat(s.BaseSizePrecision, 64)
        if err != nil || step <= 0 {
            continue
        }
        valids = append(valids, valid{s.Symbol, step})
    }
    // Закрываем массив ']'
    dec.Token()

    // 3) Теперь точно знаем n
    n := len(valids)
    available := make(map[string]bool, n)
    stepSizes := make(map[string]float64, n)
    minQtys   := make(map[string]float64, n)

    // Заполняем
    var logLines []string
    logLines = make([]string, 0, n)
    for _, v := range valids {
        available[v.sym] = true
        stepSizes[v.sym] = v.step
        minQtys[v.sym]   = v.step
        logLines = append(logLines, fmt.Sprintf("%s\tstep=%g", v.sym, v.step))
    }

    _ = os.WriteFile("available_all_symbols.log", []byte(strings.Join(logLines, "\n")), 0644)
    log.Printf("✅ Подходящих пар: %d", n)

    return available, stepSizes, minQtys
}


gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof http://localhost:6060/debug/pprof/heap
Fetching profile over HTTP from http://localhost:6060/debug/pprof/heap
Saved profile in /home/gaz358/pprof/pprof.cryptarb.alloc_objects.alloc_space.inuse_objects.inuse_space.008.pb.gz
File: cryptarb
Build ID: 0b9736d25f08520070715f602ec1ccfea5320a11
Type: inuse_space
Time: 2025-08-05 02:54:56 MSK
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 1545.79kB, 100% of 1545.79kB total
Showing top 10 nodes out of 23
      flat  flat%   sum%        cum   cum%
  520.04kB 33.64% 33.64%   520.04kB 33.64%  cryptarb/internal/repository/filesystem.LoadTrianglesFromSymbols
     513kB 33.19% 66.83%      513kB 33.19%  runtime.allocm
  512.75kB 33.17%   100%   512.75kB 33.17%  crypto/x509.parseCertificate
         0     0%   100%   520.04kB 33.64%  cryptarb/internal/app.New
         0     0%   100%   512.75kB 33.17%  crypto/tls.(*Conn).HandshakeContext
         0     0%   100%   512.75kB 33.17%  crypto/tls.(*Conn).clientHandshake
         0     0%   100%   512.75kB 33.17%  crypto/tls.(*Conn).handshakeContext
         0     0%   100%   512.75kB 33.17%  crypto/tls.(*Conn).verifyServerCertificate
         0     0%   100%   512.75kB 33.17%  crypto/tls.(*certCache).newCert
         0     0%   100%   512.75kB 33.17%  crypto/tls.(*clientHandshakeStateTLS13).handshake
(pprof) 

(pprof) list FetchAvailableSymbols
no matches found for regexp: FetchAvailableSymbols


gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/08/05 02:54:26 📈 Profiler доступен на http://localhost:6060/debug/pprof/
2025/08/05 02:54:26 ✅ Подходящих пар: 557
2025/08/05 02:54:26 📊 Доступные пары (с инверсиями): 1114
2025/08/05 02:54:26 [TRIANGLE] Found 234 triangles
2025/08/05 02:54:26 [INIT] Треугольников найдено: 234
2025/08/05 02:54:26 [INIT] Составили индекс по парам: 144 ключей
2025/08/05 02:54:26 [INIT] Подписка на пар: 144 шт.
fatal error: concurrent map read and map write

goroutine 52 [running]:
internal/runtime/maps.fatal({0x800b66?, 0xc0003640e0?})
        /usr/local/go/src/runtime/panic.go:1053 +0x18
cryptarb/internal/app.(*Arbitrager).HandleRaw(0xc0001c5b00, {0xc000035e10?, 0x6e2e9c?}, {0xc000629c00, 0xab, 0x200})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:180 +0x358
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc00033b0c8, 0x14, 0x0?}, 0xc000392000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:144 +0x3a6
cryptarb/internal/app.New.func1({0xc00033b0c8, 0x14, 0x2c})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 1 [select (no cases)]:
main.main()
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:45 +0x17f

goroutine 7 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd90415d8, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0001b0080?, 0x380016?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Accept(0xc0001b0080)
        /usr/local/go/src/internal/poll/fd_unix.go:620 +0x295
net.(*netFD).accept(0xc0001b0080)
        /usr/local/go/src/net/fd_unix.go:172 +0x29
net.(*TCPListener).accept(0xc0001aa040)
        /usr/local/go/src/net/tcpsock_posix.go:159 +0x1b
net.(*TCPListener).Accept(0xc0001aa040)
        /usr/local/go/src/net/tcpsock.go:380 +0x30
net/http.(*Server).Serve(0xc0001a2000, {0x881a40, 0xc0001aa040})
        /usr/local/go/src/net/http/server.go:3424 +0x30c
net/http.(*Server).ListenAndServe(0xc0001a2000)
        /usr/local/go/src/net/http/server.go:3350 +0x71
net/http.ListenAndServe(...)
        /usr/local/go/src/net/http/server.go:3659
main.main.func1()
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:19 +0x9b
created by main.main in goroutine 1
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:17 +0x1e

goroutine 14 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd90413a8, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0001b0100?, 0xc000344000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc0001b0100, {0xc000344000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc0001b0100, {0xc000344000?, 0xc00034411f?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000616018, {0xc000344000?, 0x7f6b92480648?, 0x7f6bd91b4108?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000186180, {0xc000344000?, 0xedc?, 0xc0001773a8?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0000d49b8, {0x87f320, 0xc000186180})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc0000d4708, {0x87f5e0, 0xc000616018}, 0xee1?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc0000d4708, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc0000d4708, {0xc000689000, 0x1000, 0xc0001777c8?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc0005b5b60)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).ReadSlice(0xc0005b5b60, 0xa)
        /usr/local/go/src/bufio/bufio.go:380 +0x29
bufio.(*Reader).ReadLine(0xc0005b5b60)
        /usr/local/go/src/bufio/bufio.go:409 +0x25
net/textproto.(*Reader).readLineSlice(0xc000177920, 0xffffffffffffffff)
        /usr/local/go/src/net/textproto/reader.go:64 +0xa5
net/textproto.(*Reader).ReadLine(...)
        /usr/local/go/src/net/textproto/reader.go:44
net/http.ReadResponse(0xc0005b5b60, 0xc0001bb2c0)
        /usr/local/go/src/net/http/response.go:161 +0x9e
github.com/gorilla/websocket.(*Dialer).DialContext(0xaf1200?, {0x881ce8, 0xb19aa0}, {0x7f9940, 0x15}, 0x0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372 +0x197e
github.com/gorilla/websocket.(*Dialer).Dial(...)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x10000c0000617a0?, {0xc00033aa88, 0x14, 0xc000061730?}, 0xc00033f420)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:106 +0x70
cryptarb/internal/app.New.func1({0xc00033aa88, 0x14, 0x90})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 16 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd9041290, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc00011c000?, 0xc0003d2000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc00011c000, {0xc0003d2000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc00011c000, {0xc0003d2000?, 0xc0003d211f?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000616020, {0xc0003d2000?, 0x7f6b92483c08?, 0x7f6bd91b45c0?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc0003ee120, {0xc0003d2000?, 0xedc?, 0xc0002353a8?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0000d4d38, {0x87f320, 0xc0003ee120})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc0000d4a88, {0x87f5e0, 0xc000616020}, 0xee1?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc0000d4a88, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc0000d4a88, {0xc0002eb000, 0x1000, 0x425877?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc0007023c0)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).ReadSlice(0xc0007023c0, 0xa)
        /usr/local/go/src/bufio/bufio.go:380 +0x29
bufio.(*Reader).ReadLine(0xc0007023c0)
        /usr/local/go/src/bufio/bufio.go:409 +0x25
net/textproto.(*Reader).readLineSlice(0xc000235920, 0xffffffffffffffff)
        /usr/local/go/src/net/textproto/reader.go:64 +0xa5
net/textproto.(*Reader).ReadLine(...)
        /usr/local/go/src/net/textproto/reader.go:44
net/http.ReadResponse(0xc0007023c0, 0xc00050c3c0)
        /usr/local/go/src/net/http/response.go:161 +0x9e
github.com/gorilla/websocket.(*Dialer).DialContext(0xaf1200?, {0x881ce8, 0xb19aa0}, {0x7f9940, 0x15}, 0x0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372 +0x197e
github.com/gorilla/websocket.(*Dialer).Dial(...)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x1?, {0xc00033ad08, 0x14, 0x1?}, 0xc0005040a0)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:106 +0x70
cryptarb/internal/app.New.func1({0xc00033ad08, 0x14, 0x68})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 15 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd90414c0, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000130000?, 0xc00038f000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000130000, {0xc00038f000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000130000, {0xc00038f000?, 0xc00038f000?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc0001ac008, {0xc00038f000?, 0x7f6b92480648?, 0x7f6bd91b4108?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000186420, {0xc00038f000?, 0xffb?, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0001b82b8, {0x87f320, 0xc000186420})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc0001b8008, {0x87f5e0, 0xc0001ac008}, 0x43d194?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc0001b8008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc0001b8008, {0xc0003d3000, 0x1000, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000181080)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000181080, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc0003d86e0, 0x7?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc0003d86e0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc0003d86e0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x7ef5c1?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc00033abc8, 0x14, 0x0?}, 0xc000400000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:140 +0x3b0
cryptarb/internal/app.New.func1({0xc00033abc8, 0x14, 0x7c})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 50 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd9041178, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0000e6000?, 0xc0002ea000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc0000e6000, {0xc0002ea000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc0000e6000, {0xc0002ea000?, 0xc0002ea000?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000062100, {0xc0002ea000?, 0x7f6b92480648?, 0x7f6bd91b4108?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000186150, {0xc0002ea000?, 0xffb?, 0xc00024d3a8?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0002862b8, {0x87f320, 0xc000186150})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000286008, {0x87f5e0, 0xc000062100}, 0x1000?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000286008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000286008, {0xc000524000, 0x1000, 0xc00024d7c8?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000591440)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).ReadSlice(0xc000591440, 0xa)
        /usr/local/go/src/bufio/bufio.go:380 +0x29
bufio.(*Reader).ReadLine(0xc000591440)
        /usr/local/go/src/bufio/bufio.go:409 +0x25
net/textproto.(*Reader).readLineSlice(0xc00024d920, 0xffffffffffffffff)
        /usr/local/go/src/net/textproto/reader.go:64 +0xa5
net/textproto.(*Reader).ReadLine(...)
        /usr/local/go/src/net/textproto/reader.go:44
net/http.ReadResponse(0xc000591440, 0xc00050c140)
        /usr/local/go/src/net/http/response.go:161 +0x9e
github.com/gorilla/websocket.(*Dialer).DialContext(0xaf1200?, {0x881ce8, 0xb19aa0}, {0x7f9940, 0x15}, 0x0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372 +0x197e
github.com/gorilla/websocket.(*Dialer).Dial(...)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x60?, {0xc00033ae48, 0x14, 0x4?}, 0xc000504000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:106 +0x70
cryptarb/internal/app.New.func1({0xc00033ae48, 0x14, 0x54})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 51 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd9041060, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000612280?, 0xc0001f8000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000612280, {0xc0001f8000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000612280, {0xc0001f8000?, 0xc0001f8000?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc0001ac010, {0xc0001f8000?, 0x7f6b92483c08?, 0x7f6bd91b45c0?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc0003ee1f8, {0xc0001f8000?, 0xffb?, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0001b8638, {0x87f320, 0xc0003ee1f8})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc0001b8388, {0x87f5e0, 0xc0001ac010}, 0x43d194?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc0001b8388, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc0001b8388, {0xc0005c3000, 0x1000, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc00048fb00)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc00048fb00, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc0000dcb00, 0x9?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc0000dcb00)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc0000dcb00)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x7ef5c1?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc00033af88, 0x14, 0x0?}, 0xc000504120)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:140 +0x3b0
cryptarb/internal/app.New.func1({0xc00033af88, 0x14, 0x40})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 53 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd9040f48, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0001b0200?, 0xc00014c000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc0001b0200, {0xc00014c000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc0001b0200, {0xc00014c000?, 0xc00014c11f?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc00023e000, {0xc00014c000?, 0x7f6b92480648?, 0x7f6bd91b4108?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000186048, {0xc00014c000?, 0xedc?, 0xc00025b3a8?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0002422b8, {0x87f320, 0xc000186048})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000242008, {0x87f5e0, 0xc00023e000}, 0xee1?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000242008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000242008, {0xc00014d000, 0x1000, 0xc00025b7c8?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc0005b5620)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).ReadSlice(0xc0005b5620, 0xa)
        /usr/local/go/src/bufio/bufio.go:380 +0x29
bufio.(*Reader).ReadLine(0xc0005b5620)
        /usr/local/go/src/bufio/bufio.go:409 +0x25
net/textproto.(*Reader).readLineSlice(0xc00025b920, 0xffffffffffffffff)
        /usr/local/go/src/net/textproto/reader.go:64 +0xa5
net/textproto.(*Reader).ReadLine(...)
        /usr/local/go/src/net/textproto/reader.go:44
net/http.ReadResponse(0xc0005b5620, 0xc0004643c0)
        /usr/local/go/src/net/http/response.go:161 +0x9e
github.com/gorilla/websocket.(*Dialer).DialContext(0xc000442e80?, {0x881ce8, 0xb19aa0}, {0x7f9940, 0x15}, 0x0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372 +0x197e
github.com/gorilla/websocket.(*Dialer).Dial(...)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc00033b208, 0x14, 0x0?}, 0xc00048c000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:106 +0x70
cryptarb/internal/app.New.func1({0xc00033b208, 0x14, 0x18})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 54 [IO wait]:
internal/poll.runtime_pollWait(0x7f6bd9040e30, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000130080?, 0xc000406000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000130080, {0xc000406000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000130080, {0xc000406000?, 0xc000406000?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000300000, {0xc000406000?, 0x7f6b92483c08?, 0x7f6bd91b45c0?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc0003ee0a8, {0xc000406000?, 0xffb?, 0x4265c5?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0003042b8, {0x87f320, 0xc0003ee0a8})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000304008, {0x87f5e0, 0xc000300000}, 0x1000?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000304008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000304008, {0xc000345000, 0x1000, 0xc0002197c8?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc00048f680)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).ReadSlice(0xc00048f680, 0xa)
        /usr/local/go/src/bufio/bufio.go:380 +0x29
bufio.(*Reader).ReadLine(0xc00048f680)
        /usr/local/go/src/bufio/bufio.go:409 +0x25
net/textproto.(*Reader).readLineSlice(0xc000219920, 0xffffffffffffffff)
        /usr/local/go/src/net/textproto/reader.go:64 +0xa5
net/textproto.(*Reader).ReadLine(...)
        /usr/local/go/src/net/textproto/reader.go:44
net/http.ReadResponse(0xc00048f680, 0xc0001bb040)
        /usr/local/go/src/net/http/response.go:161 +0x9e
github.com/gorilla/websocket.(*Dialer).DialContext(0xaf1200?, {0x881ce8, 0xb19aa0}, {0x7f9940, 0x15}, 0x0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372 +0x197e
github.com/gorilla/websocket.(*Dialer).Dial(...)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc00033b348, 0x4, 0x0?}, 0xc00033f380)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:106 +0x70
cryptarb/internal/app.New.func1({0xc00033b348, 0x4, 0x4})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed
exit status 2






