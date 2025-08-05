
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


gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof --text --focus="cryptarb" --ignore="runtime\..*" cpu.prof
File: cryptarb
Build ID: 7af130719f312e9f1eafb7d1c085f23719e8020a
Type: cpu
Time: 2025-08-05 23:01:15 MSK
Duration: 15s, Total samples = 120ms (  0.8%)
Active filters:
   focus=cryptarb
   ignore=runtime\..*
Showing nodes accounting for 20ms, 16.67% of 120ms total
      flat  flat%   sum%        cum   cum%
      10ms  8.33%  8.33%       20ms 16.67%  bytes.(*Buffer).ReadFrom
      10ms  8.33% 16.67%       10ms  8.33%  internal/runtime/syscall.Syscall6
         0     0% 16.67%       20ms 16.67%  bufio.(*Reader).Peek
         0     0% 16.67%       20ms 16.67%  bufio.(*Reader).fill
         0     0% 16.67%       20ms 16.67%  cryptarb/internal/app.New.func1
         0     0% 16.67%       20ms 16.67%  cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals
         0     0% 16.67%       20ms 16.67%  crypto/tls.(*Conn).Read
         0     0% 16.67%       20ms 16.67%  crypto/tls.(*Conn).readFromUntil
         0     0% 16.67%       20ms 16.67%  crypto/tls.(*Conn).readRecord (inline)
         0     0% 16.67%       20ms 16.67%  crypto/tls.(*Conn).readRecordOrCCS
         0     0% 16.67%       10ms  8.33%  crypto/tls.(*atLeastReader).Read
         0     0% 16.67%       20ms 16.67%  github.com/gorilla/websocket.(*Conn).NextReader
         0     0% 16.67%       20ms 16.67%  github.com/gorilla/websocket.(*Conn).ReadMessage
         0     0% 16.67%       20ms 16.67%  github.com/gorilla/websocket.(*Conn).advanceFrame
         0     0% 16.67%       20ms 16.67%  github.com/gorilla/websocket.(*Conn).read
         0     0% 16.67%       10ms  8.33%  internal/poll.(*FD).Read
         0     0% 16.67%       10ms  8.33%  internal/poll.ignoringEINTRIO (inline)
         0     0% 16.67%       10ms  8.33%  net.(*conn).Read
         0     0% 16.67%       10ms  8.33%  net.(*netFD).Read
         0     0% 16.67%       10ms  8.33%  syscall.RawSyscall6
         0     0% 16.67%       10ms  8.33%  syscall.Read (inline)
         0     0% 16.67%       10ms  8.33%  syscall.Syscall
         0     0% 16.67%       10ms  8.33%  syscall.read
gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ 



__________________________________________________________________________________


heap profile: 6: 8000 [34: 552784] @ heap/1048576
0: 0 [1: 128] @ 0x730bb0 0x740cfa 0x745945 0x745629 0x5afcb3 0x5afc46 0x5abfa5 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x730baf	crypto/internal/fips140/hmac.New[...]+0x12f						/usr/local/go/src/crypto/internal/fips140/hmac/hmac.go:149
#	0x740cf9	crypto/internal/fips140/hkdf.Expand[...]+0x99						/usr/local/go/src/crypto/internal/fips140/hkdf/hkdf.go:28
#	0x745944	crypto/internal/fips140/tls13.ExpandLabel[...]+0x264					/usr/local/go/src/crypto/internal/fips140/tls13/tls13.go:39
#	0x745628	crypto/internal/fips140/tls13.deriveSecret[...]+0x128					/usr/local/go/src/crypto/internal/fips140/tls13/tls13.go:53
#	0x5afcb2	crypto/internal/fips140/tls13.(*MasterSecret).ClientApplicationTrafficSecret+0x272	/usr/local/go/src/crypto/internal/fips140/tls13/tls13.go:131
#	0x5afc45	crypto/tls.(*clientHandshakeStateTLS13).readServerFinished+0x205			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:732
#	0x5abfa4	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x764					/usr/local/go/src/crypto/tls/handshake_client_tls13.go:139
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f						/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399						/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d						/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68						/usr/local/go/src/net/http/transport.go:1703

0: 0 [1: 221184] @ 0x4d28d9 0x4d2789 0x5764d6 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x4d28d8	os.readFileContents+0x58						/usr/local/go/src/os/file.go:826
#	0x4d2788	os.ReadFile+0xa8							/usr/local/go/src/os/file.go:805
#	0x5764d5	crypto/x509.loadSystemRoots+0x135					/usr/local/go/src/crypto/x509/root_unix.go:42
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

0: 0 [1: 512] @ 0x4ab092 0x6e7114 0x6da0de 0x47d4e1
#	0x4ab091	io.ReadAll+0x31								/usr/local/go/src/io/io.go:710
#	0x6e7113	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x3d3	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:134
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

0: 0 [4: 7168] @ 0x473e33 0x40613d 0x40613e 0x40608f 0x404719 0x473f29 0x6db0f0 0x6dad85 0x6e70ee 0x6da0de 0x47d4e1
#	0x6db0ef	cryptarb/internal/app.(*Arbitrager).Check+0x14f				/home/gaz358/myprog/crypt/internal/app/arbitrage.go:227
#	0x6dad84	cryptarb/internal/app.(*Arbitrager).HandleRaw+0x604			/home/gaz358/myprog/crypt/internal/app/arbitrage.go:212
#	0x6e70ed	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x3ad	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:141
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

0: 0 [1: 32] @ 0x406032 0x404719 0x473f29 0x6db0f0 0x6dad85 0x6e70ee 0x6da0de 0x47d4e1
#	0x6db0ef	cryptarb/internal/app.(*Arbitrager).Check+0x14f				/home/gaz358/myprog/crypt/internal/app/arbitrage.go:227
#	0x6dad84	cryptarb/internal/app.(*Arbitrager).HandleRaw+0x604			/home/gaz358/myprog/crypt/internal/app/arbitrage.go:212
#	0x6e70ed	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x3ad	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:141
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

0: 0 [1: 32] @ 0x63a0b5 0x63a08a 0x6dd5fe 0x6e6db0 0x6e6d88 0x6da0de 0x47d4e1
#	0x63a0b4	net/textproto.(*Reader).ReadLine+0xb4					/usr/local/go/src/net/textproto/reader.go:45
#	0x63a089	net/http.ReadResponse+0x89						/usr/local/go/src/net/http/response.go:161
#	0x6dd5fd	github.com/gorilla/websocket.(*Dialer).DialContext+0x197d		/home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:372
#	0x6e6daf	github.com/gorilla/websocket.(*Dialer).Dial+0x6f			/home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
#	0x6e6d87	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x47	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:110
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

0: 0 [1: 48] @ 0x4efa9f 0x57b87d 0x57b863 0x57bd0d 0x5a1415 0x6e3c45 0x6dd1e5 0x6e6db0 0x6e6d88 0x6da0de 0x47d4e1
#	0x4efa9e	strings.genSplit+0x7e							/usr/local/go/src/strings/strings.go:305
#	0x57b87c	strings.Split+0x23c							/usr/local/go/src/strings/strings.go:361
#	0x57b862	crypto/x509.matchHostnames+0x222					/usr/local/go/src/crypto/x509/verify.go:1088
#	0x57bd0c	crypto/x509.(*Certificate).VerifyHostname+0x30c				/usr/local/go/src/crypto/x509/verify.go:1175
#	0x5a1414	crypto/tls.(*Conn).VerifyHostname+0xd4					/usr/local/go/src/crypto/tls/conn.go:1684
#	0x6e3c44	github.com/gorilla/websocket.doHandshake+0x64				/home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/tls_handshake.go:16
#	0x6dd1e4	github.com/gorilla/websocket.(*Dialer).DialContext+0x1564		/home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:350
#	0x6e6daf	github.com/gorilla/websocket.(*Dialer).Dial+0x6f			/home/gaz358/go/pkg/mod/github.com/gorilla/websocket@v1.5.3/client.go:117
#	0x6e6d87	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x47	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:110
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

0: 0 [1: 16] @ 0x45faf7 0x460435 0x460249 0x427391 0x427366 0x4277b4 0x47d4e1
#	0x45faf6	runtime.(*timers).addHeap+0x96		/usr/local/go/src/runtime/time.go:437
#	0x460434	runtime.(*timer).maybeAdd+0x114		/usr/local/go/src/runtime/time.go:692
#	0x460248	runtime.(*timer).modify+0x268		/usr/local/go/src/runtime/time.go:620
#	0x427390	runtime.(*timer).reset+0xd0		/usr/local/go/src/runtime/time.go:706
#	0x427365	runtime.(*scavengerState).sleep+0xa5	/usr/local/go/src/runtime/mgcscavenge.go:500
#	0x4277b3	runtime.bgscavenge+0x73			/usr/local/go/src/runtime/mgcscavenge.go:662

0: 0 [1: 8192] @ 0x6d8b8b 0x6d929c 0x6e9cef 0x44134b 0x47d4e1
#	0x6d8b8a	cryptarb/internal/repository/filesystem.LoadTrianglesFromSymbols+0x9aa	/home/gaz358/myprog/crypt/internal/repository/filesystem/loader.go:33
#	0x6d929b	cryptarb/internal/app.New+0x11b						/home/gaz358/myprog/crypt/internal/app/arbitrage.go:40
#	0x6e9cee	main.main+0x12e								/home/gaz358/myprog/crypt/cmd/cryptarb/main.go:46
#	0x44134a	runtime.main+0x28a							/usr/local/go/src/runtime/proc.go:283

0: 0 [1: 4096] @ 0x473e33 0x40613d 0x40613e 0x40608f 0x4077f7 0x407565 0x409ec6 0x6d8aef 0x6d929c 0x6e9cef 0x44134b 0x47d4e1
#	0x6d8aee	cryptarb/internal/repository/filesystem.LoadTrianglesFromSymbols+0x90e	/home/gaz358/myprog/crypt/internal/repository/filesystem/loader.go:32
#	0x6d929b	cryptarb/internal/app.New+0x11b						/home/gaz358/myprog/crypt/internal/app/arbitrage.go:40
#	0x6e9cee	main.main+0x12e								/home/gaz358/myprog/crypt/cmd/cryptarb/main.go:46
#	0x44134a	runtime.main+0x28a							/usr/local/go/src/runtime/proc.go:283

0: 0 [1: 27264] @ 0x473e33 0x40613d 0x40613e 0x40608f 0x404719 0x473f29 0x6e7ab8 0x6d91b6 0x6e9cef 0x44134b 0x47d4e1
#	0x6e7ab7	cryptarb/internal/repository/mexc.(*MexcExchange).FetchAvailableSymbols+0x7f7	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:254
#	0x6d91b5	cryptarb/internal/app.New+0x35							/home/gaz358/myprog/crypt/internal/app/arbitrage.go:35
#	0x6e9cee	main.main+0x12e									/home/gaz358/myprog/crypt/cmd/cryptarb/main.go:46
#	0x44134a	runtime.main+0x28a								/usr/local/go/src/runtime/proc.go:283

0: 0 [1: 64] @ 0x6e7854 0x6d91b6 0x6e9cef 0x44134b 0x47d4e1
#	0x6e7853	cryptarb/internal/repository/mexc.(*MexcExchange).FetchAvailableSymbols+0x593	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:235
#	0x6d91b5	cryptarb/internal/app.New+0x35							/home/gaz358/myprog/crypt/internal/app/arbitrage.go:35
#	0x6e9cee	main.main+0x12e									/home/gaz358/myprog/crypt/cmd/cryptarb/main.go:46
#	0x44134a	runtime.main+0x28a								/usr/local/go/src/runtime/proc.go:283

0: 0 [3: 10368] @ 0x473e33 0x40613d 0x40613e 0x40608f 0x404719 0x473f29 0x6db0f0 0x6dad85 0x6e70ee 0x6da0de 0x47d4e1
#	0x6db0ef	cryptarb/internal/app.(*Arbitrager).Check+0x14f				/home/gaz358/myprog/crypt/internal/app/arbitrage.go:227
#	0x6dad84	cryptarb/internal/app.(*Arbitrager).HandleRaw+0x604			/home/gaz358/myprog/crypt/internal/app/arbitrage.go:212
#	0x6e70ed	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x3ad	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:141
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

0: 0 [1: 40960] @ 0x4e8834 0x4e82dd 0x59c190 0x59c157 0x599c7e 0x59fae5 0x59fae6 0x65ce47 0x5d03f7 0x5f900b 0x651b7b 0x651a5f 0x660ee2 0x60130a 0x6caba8 0x6ca7a5 0x6ca555 0x6e7873 0x6d91b6 0x6e9cef 0x44134b 0x47d4e1
#	0x4e8833	bytes.growSlice+0x93								/usr/local/go/src/bytes/buffer.go:249
#	0x4e82dc	bytes.(*Buffer).grow+0x13c							/usr/local/go/src/bytes/buffer.go:151
#	0x59c18f	bytes.(*Buffer).Grow+0x6f							/usr/local/go/src/bytes/buffer.go:168
#	0x59c156	crypto/tls.(*Conn).readFromUntil+0x36						/usr/local/go/src/crypto/tls/conn.go:830
#	0x599c7d	crypto/tls.(*Conn).readRecordOrCCS+0xd3d					/usr/local/go/src/crypto/tls/conn.go:680
#	0x59fae4	crypto/tls.(*Conn).readRecord+0x144						/usr/local/go/src/crypto/tls/conn.go:591
#	0x59fae5	crypto/tls.(*Conn).Read+0x145							/usr/local/go/src/crypto/tls/conn.go:1385
#	0x65ce46	net/http.(*persistConn).Read+0x46						/usr/local/go/src/net/http/transport.go:2122
#	0x5d03f6	bufio.(*Reader).Read+0x196							/usr/local/go/src/bufio/bufio.go:245
#	0x5f900a	net/http/internal.(*chunkedReader).Read+0x16a					/usr/local/go/src/net/http/internal/chunked.go:136
#	0x651b7a	net/http.(*body).readLocked+0x3a						/usr/local/go/src/net/http/transfer.go:845
#	0x651a5e	net/http.(*body).Read+0xfe							/usr/local/go/src/net/http/transfer.go:837
#	0x660ee1	net/http.(*bodyEOFSignal).Read+0x141						/usr/local/go/src/net/http/transport.go:2984
#	0x601309	net/http.(*cancelTimerBody).Read+0x29						/usr/local/go/src/net/http/client.go:971
#	0x6caba7	encoding/json.(*Decoder).refill+0x187						/usr/local/go/src/encoding/json/stream.go:165
#	0x6ca7a4	encoding/json.(*Decoder).readValue+0x84						/usr/local/go/src/encoding/json/stream.go:140
#	0x6ca554	encoding/json.(*Decoder).Decode+0x74						/usr/local/go/src/encoding/json/stream.go:63
#	0x6e7872	cryptarb/internal/repository/mexc.(*MexcExchange).FetchAvailableSymbols+0x5b2	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:236
#	0x6d91b5	cryptarb/internal/app.New+0x35							/home/gaz358/myprog/crypt/internal/app/arbitrage.go:35
#	0x6e9cee	main.main+0x12e									/home/gaz358/myprog/crypt/cmd/cryptarb/main.go:46
#	0x44134a	runtime.main+0x28a								/usr/local/go/src/runtime/proc.go:283

0: 0 [1: 112] @ 0x673f2e 0x673e91 0x673a9b 0x676ddd 0x6848f0 0x6848eb 0x68544c 0x685440 0x6e8359 0x44ff58 0x441405 0x4412ee 0x47d4e1
#	0x673f2d	regexp/syntax.(*parser).newRegexp+0xed	/usr/local/go/src/regexp/syntax/parse.go:146
#	0x673e90	regexp/syntax.(*parser).collapse+0x50	/usr/local/go/src/regexp/syntax/parse.go:552
#	0x673a9a	regexp/syntax.(*parser).concat+0x13a	/usr/local/go/src/regexp/syntax/parse.go:492
#	0x676ddc	regexp/syntax.parse+0x103c		/usr/local/go/src/regexp/syntax/parse.go:1084
#	0x6848ef	regexp/syntax.Parse+0x2f		/usr/local/go/src/regexp/syntax/parse.go:887
#	0x6848ea	regexp.compile+0x2a			/usr/local/go/src/regexp/regexp.go:168
#	0x68544b	regexp.Compile+0x2b			/usr/local/go/src/regexp/regexp.go:131
#	0x68543f	regexp.MustCompile+0x1f			/usr/local/go/src/regexp/regexp.go:311
#	0x6e8358	github.com/joho/godotenv.init+0x58	/home/gaz358/go/pkg/mod/github.com/joho/godotenv@v1.5.1/parser.go:253
#	0x44ff57	runtime.doInit1+0xd7			/usr/local/go/src/runtime/proc.go:7350
#	0x441404	runtime.doInit+0x344			/usr/local/go/src/runtime/proc.go:7317
#	0x4412ed	runtime.main+0x22d			/usr/local/go/src/runtime/proc.go:254

0: 0 [1: 64] @ 0x56c685 0x576745 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x56c684	crypto/x509.(*CertPool).AppendCertsFromPEM+0x1c4			/usr/local/go/src/crypto/x509/cert_pool.go:239
#	0x576744	crypto/x509.loadSystemRoots+0x3a4					/usr/local/go/src/crypto/x509/root_unix.go:72
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

0: 0 [1: 80] @ 0x51c4c5 0x56f6a5 0x5750d9 0x57607d 0x56c5de 0x576745 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x51c4c4	vendor/golang.org/x/crypto/cryptobyte.(*String).ReadASN1ObjectIdentifier+0x64	/usr/local/go/src/vendor/golang.org/x/crypto/cryptobyte/asn1.go:461
#	0x56f6a4	crypto/x509.parseAI+0x84							/usr/local/go/src/crypto/x509/parser.go:154
#	0x5750d8	crypto/x509.parseCertificate+0x3f8						/usr/local/go/src/crypto/x509/parser.go:949
#	0x57607c	crypto/x509.ParseCertificate+0x1c						/usr/local/go/src/crypto/x509/parser.go:1074
#	0x56c5dd	crypto/x509.(*CertPool).AppendCertsFromPEM+0x11d				/usr/local/go/src/crypto/x509/cert_pool.go:231
#	0x576744	crypto/x509.loadSystemRoots+0x3a4						/usr/local/go/src/crypto/x509/root_unix.go:72
#	0x576295	crypto/x509.initSystemRoots+0x55						/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa							/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44								/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27						/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c						/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c				/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e		/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b				/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f					/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399					/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d					/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68					/usr/local/go/src/net/http/transport.go:1703

0: 0 [2: 3072] @ 0x574d25 0x57607d 0x56c5de 0x576745 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x574d24	crypto/x509.parseCertificate+0x44					/usr/local/go/src/crypto/x509/parser.go:884
#	0x57607c	crypto/x509.ParseCertificate+0x1c					/usr/local/go/src/crypto/x509/parser.go:1074
#	0x56c5dd	crypto/x509.(*CertPool).AppendCertsFromPEM+0x11d			/usr/local/go/src/crypto/x509/cert_pool.go:231
#	0x576744	crypto/x509.loadSystemRoots+0x3a4					/usr/local/go/src/crypto/x509/root_unix.go:72
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

0: 0 [1: 221184] @ 0x4d28d9 0x4d2789 0x57672a 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x4d28d8	os.readFileContents+0x58						/usr/local/go/src/os/file.go:826
#	0x4d2788	os.ReadFile+0xa8							/usr/local/go/src/os/file.go:805
#	0x576729	crypto/x509.loadSystemRoots+0x389					/usr/local/go/src/crypto/x509/root_unix.go:70
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

0: 0 [1: 112] @ 0x65d89a 0x47d4e1
#	0x65d899	net/http.(*persistConn).readLoop+0xb9	/usr/local/go/src/net/http/transport.go:2264

0: 0 [1: 48] @ 0x56f47f 0x57536a 0x57607d 0x56c5de 0x57654d 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x56f47e	crypto/x509.parseName+0x37e						/usr/local/go/src/crypto/x509/parser.go:143
#	0x575369	crypto/x509.parseCertificate+0x689					/usr/local/go/src/crypto/x509/parser.go:980
#	0x57607c	crypto/x509.ParseCertificate+0x1c					/usr/local/go/src/crypto/x509/parser.go:1074
#	0x56c5dd	crypto/x509.(*CertPool).AppendCertsFromPEM+0x11d			/usr/local/go/src/crypto/x509/cert_pool.go:231
#	0x57654c	crypto/x509.loadSystemRoots+0x1ac					/usr/local/go/src/crypto/x509/root_unix.go:44
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

0: 0 [1: 48] @ 0x56f21c 0x5751e5 0x57607d 0x56c5de 0x57654d 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x56f21b	crypto/x509.parseName+0x11b						/usr/local/go/src/crypto/x509/parser.go:146
#	0x5751e4	crypto/x509.parseCertificate+0x504					/usr/local/go/src/crypto/x509/parser.go:960
#	0x57607c	crypto/x509.ParseCertificate+0x1c					/usr/local/go/src/crypto/x509/parser.go:1074
#	0x56c5dd	crypto/x509.(*CertPool).AppendCertsFromPEM+0x11d			/usr/local/go/src/crypto/x509/cert_pool.go:231
#	0x57654c	crypto/x509.loadSystemRoots+0x1ac					/usr/local/go/src/crypto/x509/root_unix.go:44
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

1: 96 [1: 96] @ 0x441989 0x40efb9 0x40ed12 0x420d6f 0x47d4e1
#	0x441988	runtime.acquireSudog+0x228					/usr/local/go/src/runtime/proc.go:484
#	0x40efb8	runtime.chanrecv+0x278						/usr/local/go/src/runtime/chan.go:635
#	0x40ed11	runtime.chanrecv1+0x11						/usr/local/go/src/runtime/chan.go:506
#	0x420d6e	runtime.unique_runtime_registerUniqueMapCleanup.func2+0x2e	/usr/local/go/src/runtime/mgc.go:1796

1: 480 [1: 480] @ 0x6da985 0x6e70ee 0x6da0de 0x47d4e1
#	0x6da984	cryptarb/internal/app.(*Arbitrager).HandleRaw+0x204			/home/gaz358/myprog/crypt/internal/app/arbitrage.go:158
#	0x6e70ed	cryptarb/internal/repository/mexc.(*MexcExchange).SubscribeDeals+0x3ad	/home/gaz358/myprog/crypt/internal/repository/mexc/mex.go:141
#	0x6da0dd	cryptarb/internal/app.New.func1+0x13d					/home/gaz358/myprog/crypt/internal/app/arbitrage.go:104

1: 1536 [1: 1536] @ 0x524d93 0x56c51b 0x57654d 0x576296 0x48f60b 0x576145 0x576128 0x579bdd 0x5aa9fd 0x5af46f 0x5abf8c 0x5a5d70 0x5a04da 0x659cee 0x659ce9 0x47d4e1
#	0x524d92	encoding/pem.Decode+0x692						/usr/local/go/src/encoding/pem/pem.go:171
#	0x56c51a	crypto/x509.(*CertPool).AppendCertsFromPEM+0x5a				/usr/local/go/src/crypto/x509/cert_pool.go:222
#	0x57654c	crypto/x509.loadSystemRoots+0x1ac					/usr/local/go/src/crypto/x509/root_unix.go:44
#	0x576295	crypto/x509.initSystemRoots+0x55					/usr/local/go/src/crypto/x509/root.go:40
#	0x48f60a	sync.(*Once).doSlow+0xaa						/usr/local/go/src/sync/once.go:78
#	0x576144	sync.(*Once).Do+0x44							/usr/local/go/src/sync/once.go:69
#	0x576127	crypto/x509.systemRootsPool+0x27					/usr/local/go/src/crypto/x509/root.go:31
#	0x579bdc	crypto/x509.(*Certificate).Verify+0x15c					/usr/local/go/src/crypto/x509/verify.go:816
#	0x5aa9fc	crypto/tls.(*Conn).verifyServerCertificate+0xa9c			/usr/local/go/src/crypto/tls/handshake_client.go:1145
#	0x5af46e	crypto/tls.(*clientHandshakeStateTLS13).readServerCertificate+0x26e	/usr/local/go/src/crypto/tls/handshake_client_tls13.go:658
#	0x5abf8b	crypto/tls.(*clientHandshakeStateTLS13).handshake+0x74b			/usr/local/go/src/crypto/tls/handshake_client_tls13.go:136
#	0x5a5d6f	crypto/tls.(*Conn).clientHandshake+0x80f				/usr/local/go/src/crypto/tls/handshake_client.go:379
#	0x5a04d9	crypto/tls.(*Conn).handshakeContext+0x399				/usr/local/go/src/crypto/tls/conn.go:1568
#	0x659ced	crypto/tls.(*Conn).HandshakeContext+0x6d				/usr/local/go/src/crypto/tls/conn.go:1508
#	0x659ce8	net/http.(*persistConn).addTLS.func2+0x68				/usr/local/go/src/net/http/transport.go:1703

1: 1792 [1: 1792] @ 0x473e33 0x40613d 0x40613e 0x40608f 0x404719 0x473f29 0x5f4da5 0x44ff58 0x441405 0x4412ee 0x47d4e1
#	0x5f4da4	vendor/golang.org/x/net/http2/hpack.init+0x24	/usr/local/go/src/vendor/golang.org/x/net/http2/hpack/static_table.go:8
#	0x44ff57	runtime.doInit1+0xd7				/usr/local/go/src/runtime/proc.go:7350
#	0x441404	runtime.doInit+0x344				/usr/local/go/src/runtime/proc.go:7317
#	0x4412ed	runtime.main+0x22d				/usr/local/go/src/runtime/proc.go:254

1: 2048 [1: 2048] @ 0x445211 0x445d35 0x446419 0x4763ac 0x44899e 0x448e2f 0x449285 0x47b4ae
#	0x445210	runtime.allocm+0x90		/usr/local/go/src/runtime/proc.go:2233
#	0x445d34	runtime.newm+0x34		/usr/local/go/src/runtime/proc.go:2769
#	0x446418	runtime.startm+0x158		/usr/local/go/src/runtime/proc.go:2995
#	0x4763ab	runtime.wakep+0xeb		/usr/local/go/src/runtime/proc.go:3142
#	0x44899d	runtime.resetspinning+0x3d	/usr/local/go/src/runtime/proc.go:3882
#	0x448e2e	runtime.schedule+0x10e		/usr/local/go/src/runtime/proc.go:4035
#	0x449284	runtime.park_m+0x284		/usr/local/go/src/runtime/proc.go:4141
#	0x47b4ad	runtime.mcall+0x4d		/usr/local/go/src/runtime/asm_amd64.s:459

1: 2048 [1: 2048] @ 0x445211 0x445d35 0x446419 0x4763ac 0x44899e 0x448e2f 0x44492d 0x444835 0x47b425
#	0x445210	runtime.allocm+0x90		/usr/local/go/src/runtime/proc.go:2233
#	0x445d34	runtime.newm+0x34		/usr/local/go/src/runtime/proc.go:2769
#	0x446418	runtime.startm+0x158		/usr/local/go/src/runtime/proc.go:2995
#	0x4763ab	runtime.wakep+0xeb		/usr/local/go/src/runtime/proc.go:3142
#	0x44899d	runtime.resetspinning+0x3d	/usr/local/go/src/runtime/proc.go:3882
#	0x448e2e	runtime.schedule+0x10e		/usr/local/go/src/runtime/proc.go:4035
#	0x44492c	runtime.mstart1+0xcc		/usr/local/go/src/runtime/proc.go:1862
#	0x444834	runtime.mstart0+0x74		/usr/local/go/src/runtime/proc.go:1808
#	0x47b424	runtime.mstart+0x4		/usr/local/go/src/runtime/asm_amd64.s:395


# runtime.MemStats
# Alloc = 2888608
# TotalAlloc = 13476904
# Sys = 14767368
# Lookups = 0
# Mallocs = 134579
# Frees = 117944
# HeapAlloc = 2888608
# HeapSys = 7733248
# HeapIdle = 2973696
# HeapInuse = 4759552
# HeapReleased = 1859584
# HeapObjects = 16635
# Stack = 655360 / 655360
# MSpan = 142560 / 146880
# MCache = 9664 / 15704
# BuckHashSys = 1454632
# GCSys = 2943432
# OtherSys = 1818112
# NextGC = 4194304
# LastGC = 1754429958342637740
# PauseNs = [72196 79344 65677 46751 51336 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
# PauseEnd = [1754429901462727481 1754429901884110606 1754429902230362480 1754429935404175137 1754429958342637740 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
# NumGC = 5
# NumForcedGC = 0
# GCCPUFraction = 3.204728460320101e-05
# DebugGC = false
# MaxRSS = 26816512

















