
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



gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof http://localhost:6060/debug/pprof/heap?debug=1
Fetching profile over HTTP from http://localhost:6060/debug/pprof/heap?debug=1
http post http://localhost:6060/debug/pprof/symbol: server response: 405 Method Not Allowed


gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/08/05 20:31:03 📈 Profiler доступен на http://localhost:6060/debug/pprof/
2025/08/05 20:31:04 ✅ Подходящих пар: 558
2025/08/05 20:31:04 📊 Доступные пары (с инверсиями): 1116
2025/08/05 20:31:04 [TRIANGLE] Found 234 triangles
2025/08/05 20:31:04 [INIT] Треугольников найдено: 234
2025/08/05 20:31:04 [INIT] Составили индекс по парам: 144 ключей
2025/08/05 20:31:04 [INIT] Подписка на пар: 144 шт.
fatal error: concurrent map read and map write

goroutine 49 [running]:
internal/runtime/maps.fatal({0x800b46?, 0xc0003486e0?})
        /usr/local/go/src/runtime/panic.go:1053 +0x18
cryptarb/internal/app.(*Arbitrager).HandleRaw(0xc0004de0c0, {0xc000221e10?, 0x6e2e9c?}, {0xc000549600, 0xaa, 0x200})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:180 +0x358
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc0000004b8, 0x19, 0x0?}, 0xc000014020)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:144 +0x3a6
cryptarb/internal/app.New.func1({0xc0000004b8, 0x19, 0x45})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 1 [select (no cases)]:
main.main()
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:45 +0x17f

goroutine 7 [IO wait]:
internal/poll.runtime_pollWait(0x75e1cd0bd5d8, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0001ae080?, 0x380016?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Accept(0xc0001ae080)
        /usr/local/go/src/internal/poll/fd_unix.go:620 +0x295
net.(*netFD).accept(0xc0001ae080)
        /usr/local/go/src/net/fd_unix.go:172 +0x29
net.(*TCPListener).accept(0xc0001a8040)
        /usr/local/go/src/net/tcpsock_posix.go:159 +0x1b
net.(*TCPListener).Accept(0xc0001a8040)
        /usr/local/go/src/net/tcpsock.go:380 +0x30
net/http.(*Server).Serve(0xc0001a0000, {0x881a20, 0xc0001a8040})
        /usr/local/go/src/net/http/server.go:3424 +0x30c
net/http.(*Server).ListenAndServe(0xc0001a0000)
        /usr/local/go/src/net/http/server.go:3350 +0x71
net/http.ListenAndServe(...)
        /usr/local/go/src/net/http/server.go:3659
main.main.func1()
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:19 +0x9b
created by main.main in goroutine 1
        /home/gaz358/myprog/crypt/cmd/cryptarb/main.go:17 +0x1e

goroutine 47 [IO wait]:
internal/poll.runtime_pollWait(0x75e1cd0bd290, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc000130080?, 0xc000518000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc000130080, {0xc000518000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc000130080, {0xc000518000?, 0xc00051811f?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000062110, {0xc000518000?, 0x75e1cc5a10e8?, 0x75e213d513e8?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc0001ea258, {0xc000518000?, 0xedc?, 0xc00026b3a8?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc000242638, {0x87f300, 0xc0001ea258})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000242388, {0x87f5c0, 0xc000062110}, 0xee1?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000242388, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000242388, {0xc000519000, 0x1000, 0xb9?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000215b60)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).ReadSlice(0xc000215b60, 0xa)
        /usr/local/go/src/bufio/bufio.go:380 +0x29
bufio.(*Reader).ReadLine(0xc000215b60)
        /usr/local/go/src/bufio/bufio.go:409 +0x25
net/textproto.(*Reader).readLineSlice(0xc00026b920, 0xffffffffffffffff)
        /usr/local/go/src/net/textproto/reader.go:64 +0xa5
net/textproto.(*Reader).ReadLine(...)
        /usr/local/go/src/net/textproto/reader.go:44
net/http.ReadResponse(0xc000215b60, 0xc000204140)
        /usr/local/go/src/net/http/response.go:161 +0x9e
github.com/gorilla/websocket.(*Dialer).DialContext(0xaf1200?, {0x881cc8, 0xb19aa0}, {0x7f9920, 0x15}, 0x0)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372 +0x197e
github.com/gorilla/websocket.(*Dialer).Dial(...)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc000000198, 0x19, 0x0?}, 0xc0002f6000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:106 +0x70
cryptarb/internal/app.New.func1({0xc000000198, 0x19, 0x77})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 46 [IO wait]:
internal/poll.runtime_pollWait(0x75e1cd0bd4c0, 0x72)
        /usr/local/go/src/runtime/netpoll.go:351 +0x85
internal/poll.(*pollDesc).wait(0xc0001ae200?, 0xc000480000?, 0x0)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:84 +0x27
internal/poll.(*pollDesc).waitRead(...)
        /usr/local/go/src/internal/poll/fd_poll_runtime.go:89
internal/poll.(*FD).Read(0xc0001ae200, {0xc000480000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:165 +0x27a
net.(*netFD).Read(0xc0001ae200, {0xc000480000?, 0x0?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000062108, {0xc000480000?, 0x75e1cd071728?, 0x75e213d518a0?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc000128348, {0xc000480000?, 0xfffffffffffffffc?, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0002422b8, {0x87f300, 0xc000128348})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc000242008, {0x87f5c0, 0xc000062108}, 0x43d194?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc000242008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc000242008, {0xc000481000, 0x1000, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc0002b9c20)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc0002b9c20, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc00048c420, 0x7?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc00048c420)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc00048c420)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x7ef5a1?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc000000008, 0x19, 0x0?}, 0xc00012c010)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:140 +0x3b0
cryptarb/internal/app.New.func1({0xc000000008, 0x19, 0x90})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 48 [runnable]:
strings.genSplit({0xc000602200, 0x1f7}, {0x7ef202, 0x1}, 0x0, 0xa?)
        /usr/local/go/src/strings/strings.go:305 +0x7f
strings.Split(...)
        /usr/local/go/src/strings/strings.go:361
cryptarb/internal/app.(*Arbitrager).HandleRaw(0xc0004de0c0, {0xc0001fbe10?, 0x6e2e9c?}, {0xc000606000, 0x3b6, 0x580})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:154 +0x21f
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x0?, {0xc000000328, 0x19, 0x0?}, 0xc000180000)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:144 +0x3a6
cryptarb/internal/app.New.func1({0xc000000328, 0x19, 0x5e})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 50 [runnable]:
bytes.Index({0xc00038b1e3, 0x252, 0x41d}, {0xc00030fd26, 0xa, 0xa})
        /usr/local/go/src/bytes/bytes.go:1305 +0x705
cryptarb/internal/app.(*Arbitrager).HandleRaw(0xc0004de0c0, {0xc00030fe10?, 0x6e2e9c?}, {0xc00038b080, 0x3b5, 0x580})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:151 +0x1bb
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0x10000c00005a7a0?, {0xc000000648, 0x19, 0xc00005a730?}, 0xc000180200)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:144 +0x3a6
cryptarb/internal/app.New.func1({0xc000000648, 0x19, 0x2c})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed

goroutine 51 [runnable]:
syscall.Syscall(0x0, 0x7, 0xc000276000, 0x1000)
        /usr/local/go/src/syscall/syscall_linux.go:73 +0x25
syscall.read(0xc000298100?, {0xc000276000?, 0x1000?, 0xc000352700?})
        /usr/local/go/src/syscall/zsyscall_linux_amd64.go:736 +0x38
syscall.Read(...)
        /usr/local/go/src/syscall/syscall_unix.go:183
internal/poll.ignoringEINTRIO(...)
        /usr/local/go/src/internal/poll/fd_unix.go:745
internal/poll.(*FD).Read(0xc000298100, {0xc000276000, 0x1000, 0x1000})
        /usr/local/go/src/internal/poll/fd_unix.go:161 +0x2ae
net.(*netFD).Read(0xc000298100, {0xc000276000?, 0xc000276000?, 0x5?})
        /usr/local/go/src/net/fd_posix.go:55 +0x25
net.(*conn).Read(0xc000136008, {0xc000276000?, 0x75e213d59170?, 0x75e213d50f30?})
        /usr/local/go/src/net/net.go:194 +0x45
crypto/tls.(*atLeastReader).Read(0xc0000106f0, {0xc000276000?, 0xffb?, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:809 +0x3b
bytes.(*Buffer).ReadFrom(0xc0002bc2b8, {0x87f300, 0xc0000106f0})
        /usr/local/go/src/bytes/buffer.go:211 +0x98
crypto/tls.(*Conn).readFromUntil(0xc0002bc008, {0x87f5c0, 0xc000136008}, 0x43d194?)
        /usr/local/go/src/crypto/tls/conn.go:831 +0xde
crypto/tls.(*Conn).readRecordOrCCS(0xc0002bc008, 0x0)
        /usr/local/go/src/crypto/tls/conn.go:629 +0x3cf
crypto/tls.(*Conn).readRecord(...)
        /usr/local/go/src/crypto/tls/conn.go:591
crypto/tls.(*Conn).Read(0xc0002bc008, {0xc000277000, 0x1000, 0x0?})
        /usr/local/go/src/crypto/tls/conn.go:1385 +0x145
bufio.(*Reader).fill(0xc000278600)
        /usr/local/go/src/bufio/bufio.go:113 +0x103
bufio.(*Reader).Peek(0xc000278600, 0x2)
        /usr/local/go/src/bufio/bufio.go:152 +0x53
github.com/gorilla/websocket.(*Conn).read(0xc000228580, 0x7?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:371 +0x25
github.com/gorilla/websocket.(*Conn).advanceFrame(0xc000228580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:809 +0x6d
github.com/gorilla/websocket.(*Conn).NextReader(0xc000228580)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1009 +0xad
github.com/gorilla/websocket.(*Conn).ReadMessage(0x7ef5a1?)
        /home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/conn.go:1093 +0x13
cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals(0xc0001a6000?, {0xc0000007d8, 0x13, 0x0?}, 0xc0003a4010)
        /home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:140 +0x3b0
cryptarb/internal/app.New.func1({0xc0000007d8, 0x13, 0x13})
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:104 +0x13e
created by cryptarb/internal/app.New in goroutine 1
        /home/gaz358/myprog/crypt/internal/app/arbitrage.go:102 +0xbed
exit status 2
gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ 









