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


(pprof) top
Showing nodes accounting for 130ms, 100% of 130ms total
Showing top 10 nodes out of 62
      flat  flat%   sum%        cum   cum%
      40ms 30.77% 30.77%       40ms 30.77%  internal/runtime/syscall.Syscall6
      10ms  7.69% 38.46%       10ms  7.69%  encoding/json.stateBeginValue
      10ms  7.69% 46.15%       10ms  7.69%  encoding/json.unquoteBytes
      10ms  7.69% 53.85%       10ms  7.69%  memeqbody
      10ms  7.69% 61.54%       10ms  7.69%  runtime.(*spanSet).push
      10ms  7.69% 69.23%       10ms  7.69%  runtime.ifaceeq
      10ms  7.69% 76.92%       10ms  7.69%  runtime.memclrNoHeapPointers
      10ms  7.69% 84.62%       10ms  7.69%  runtime.nextFreeFast
      10ms  7.69% 92.31%       10ms  7.69%  runtime.typePointers.next
      10ms  7.69%   100%       10ms  7.69%  strconv.ParseFloat
(pprof) 



Build ID: 18dfa87cfc416462058286251d48f916dc0060b6
Type: cpu
Time: 2025-08-04 18:11:58 MSK
Duration: 30s, Total samples = 220ms ( 0.73%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 170ms, 77.27% of 220ms total
Showing top 10 nodes out of 91
      flat  flat%   sum%        cum   cum%
      50ms 22.73% 22.73%       50ms 22.73%  internal/runtime/syscall.Syscall6
      30ms 13.64% 36.36%       30ms 13.64%  runtime.nextFreeFast (inline)
      20ms  9.09% 45.45%       20ms  9.09%  runtime.mapaccess1_faststr
      10ms  4.55% 50.00%       20ms  9.09%  encoding/json.(*decodeState).array
      10ms  4.55% 54.55%       30ms 13.64%  encoding/json.(*scanner).pushParseState
      10ms  4.55% 59.09%       60ms 27.27%  encoding/json.checkValid
      10ms  4.55% 63.64%       10ms  4.55%  encoding/json.state1
      10ms  4.55% 68.18%       10ms  4.55%  encoding/json.stateInString
      10ms  4.55% 72.73%       10ms  4.55%  internal/poll.(*fdMutex).rwunlock
      10ms  4.55% 77.27%       20ms  9.09%  runtime.(*mcache).prepareForSweep
(pprof) 



howing nodes accounting for 120ms, 100% of 120ms total
Showing top 10 nodes out of 67
      flat  flat%   sum%        cum   cum%
      20ms 16.67% 16.67%       20ms 16.67%  internal/runtime/syscall.Syscall6
      20ms 16.67% 33.33%       20ms 16.67%  runtime.futex
      10ms  8.33% 41.67%       40ms 33.33%  bytes.(*Buffer).ReadFrom
      10ms  8.33% 50.00%       10ms  8.33%  crypto/tls.(*halfConn).decrypt
      10ms  8.33% 58.33%       10ms  8.33%  github.com/json-iterator/go.(*Iterator).readPositiveFloat64
      10ms  8.33% 66.67%       10ms  8.33%  github.com/json-iterator/go.locatePath
      10ms  8.33% 75.00%       20ms 16.67%  runtime.entersyscall
      10ms  8.33% 83.33%       10ms  8.33%  runtime.heapSetTypeNoHeader
      10ms  8.33% 91.67%       10ms  8.33%  runtime.memclrNoHeapPointers
      10ms  8.33%   100%       30ms 25.00%  runtime.schedule
(pprof) 






func (a *Arbitrager) HandleRaw(_exchange string, raw []byte) {
    any := json.Get(raw)

    // DEBUG: смотрим каждый сырой фрейм
    log.Printf("[HandleRaw] raw: %s", raw)

    // 1) Обработка ACK: есть id, code, msg И нет поля "c"
    if any.Get("id").ValueType() != jsoniter.NilValue &&
       any.Get("code").ValueType() != jsoniter.NilValue &&
       any.Get("msg").ValueType() != jsoniter.NilValue &&
       any.Get("c").ValueType() == jsoniter.NilValue {

        log.Printf("[HandleRaw] ACK frame")
        if any.Get("code").ToInt() == 0 {
            const prefixFail = "Not Subscribed successfully! ["
            msg := any.Get("msg").ToString()
            if parts := strings.Split(msg, prefixFail); len(parts) == 2 {
                blocked := strings.Split(
                    strings.TrimSuffix(parts[1], "].  Reason： Blocked! \""), ",",
                )
                a.mu.Lock()
                for _, ch := range blocked {
                    if idx := strings.LastIndex(ch, "@"); idx != -1 {
                        sym := ch[idx+1:]
                        a.realSymbols[sym] = false
                        log.Printf("[HandleRaw] unsubscribed: %s", sym)
                    }
                }
                a.mu.Unlock()
            }
        }
        return
    }

    // 2) Это не ACK, пробуем взять symbol
    symAny := any.Get("s")
    if symAny.ValueType() == jsoniter.NilValue {
        log.Printf("[HandleRaw] no \"s\" field, skipping")
        return
    }
    sym := symAny.ToString()
    log.Printf("[HandleRaw] symbol: %s", sym)

    // 3) Проверяем realSymbols: именно существование ключа и что он true
    subscribed, exists := a.realSymbols[sym]
    if !exists {
        log.Printf("[HandleRaw] %s not in realSymbols map", sym)
        return
    }
    if !subscribed {
        log.Printf("[HandleRaw] %s is unsubscribed", sym)
        return
    }

    // 4) Есть ли у нас по этой паре треугольники?
    if _, ok := a.trianglesByPair[sym]; !ok {
        log.Printf("[HandleRaw] no triangles for %s", sym)
        return
    }

    // 5) Читаем массив deals
    deals := any.Get("d").Get("deals")
    if deals.ValueType() != jsoniter.ArrayValue {
        log.Printf("[HandleRaw] no deals array for %s", sym)
        return
    }
    if deals.Size() == 0 {
        log.Printf("[HandleRaw] empty deals for %s", sym)
        return
    }

    // 6) Первая цена
    priceStr := deals.Get(0).Get("p").ToString()
    if priceStr == "" {
        log.Printf("[HandleRaw] empty price for %s", sym)
        return
    }
    log.Printf("[HandleRaw] priceStr=%s", priceStr)

    // 7) В float
    price, err := strconv.ParseFloat(priceStr, 64)
    if err != nil {
        log.Printf("[HandleRaw] parse error %v", err)
        return
    }
    log.Printf("[HandleRaw] parsed price=%.8f", price)

    // 8) Сохраняем и проверяем
    a.mu.Lock()
    a.latest[sym] = price
    a.mu.Unlock()

    log.Printf("[HandleRaw] Calling Check(%s)", sym)
    a.Check(sym)
}


2025/08/04 20:07:37 [HandleRaw] symbol: LTCUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=119.24
2025/08/04 20:07:37 [HandleRaw] parsed price=119.24000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(LTCUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@LTCUSDT","d":{"deals":[{"p":"119.23","v":"0.0506","S":2,"t":1754327257691}],"e":"spot@public.deals.v3.api"},"s":"LTCUSDT","t":1754327257694}
2025/08/04 20:07:37 [HandleRaw] symbol: LTCUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=119.23
2025/08/04 20:07:37 [HandleRaw] parsed price=119.23000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(LTCUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDT","d":{"deals":[{"p":"22.76","v":"5.40","S":1,"t":1754327257686}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDT","t":1754327257688}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=22.76
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDT","d":{"deals":[{"p":"22.76","v":"6.60","S":2,"t":1754327257686}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDT","t":1754327257689}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=22.76
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDT","d":{"deals":[{"p":"22.76","v":"4.60","S":1,"t":1754327257686}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDT","t":1754327257689}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=22.76
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDT","d":{"deals":[{"p":"22.76","v":"6.12","S":1,"t":1754327257687}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDT","t":1754327257689}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=22.76
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.765","v":"0.28","S":2,"t":1754327257745}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.765
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76500000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.775","v":"0.30","S":2,"t":1754327257745}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.775
2025/08/04 20:07:37 [HandleRaw] parsed price=22.77500000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.774","v":"0.16","S":1,"t":1754327257745}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.774
2025/08/04 20:07:37 [HandleRaw] parsed price=22.77400000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.757","v":"0.26","S":2,"t":1754327257745}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.757
2025/08/04 20:07:37 [HandleRaw] parsed price=22.75700000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.771","v":"0.29","S":1,"t":1754327257746}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.771
2025/08/04 20:07:37 [HandleRaw] parsed price=22.77100000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.776","v":"0.25","S":2,"t":1754327257746}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.776
2025/08/04 20:07:37 [HandleRaw] parsed price=22.77600000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.762","v":"0.17","S":1,"t":1754327257746}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257749}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.762
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76200000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@AVAXUSDC","d":{"deals":[{"p":"22.769","v":"0.11","S":2,"t":1754327257748}],"e":"spot@public.deals.v3.api"},"s":"AVAXUSDC","t":1754327257752}
2025/08/04 20:07:37 [HandleRaw] symbol: AVAXUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=22.769
2025/08/04 20:07:37 [HandleRaw] parsed price=22.76900000
2025/08/04 20:07:37 [HandleRaw] Calling Check(AVAXUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@LTCUSDC","d":{"deals":[{"p":"119.225","v":"0.05","S":1,"t":1754327257776}],"e":"spot@public.deals.v3.api"},"s":"LTCUSDC","t":1754327257778}
2025/08/04 20:07:37 [HandleRaw] symbol: LTCUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=119.225
2025/08/04 20:07:37 [HandleRaw] parsed price=119.22500000
2025/08/04 20:07:37 [HandleRaw] Calling Check(LTCUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@ETHUSDC","d":{"deals":[{"p":"3710.92","v":"0.00028","S":2,"t":1754327257775}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDC","t":1754327257778}
2025/08/04 20:07:37 [HandleRaw] symbol: ETHUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=3710.92
2025/08/04 20:07:37 [HandleRaw] parsed price=3710.92000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(ETHUSDC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@LTCBTC","d":{"deals":[{"p":"0.0010317","v":"0.051","S":2,"t":1754327257756}],"e":"spot@public.deals.v3.api"},"s":"LTCBTC","t":1754327257760}
2025/08/04 20:07:37 [HandleRaw] symbol: LTCBTC
2025/08/04 20:07:37 [HandleRaw] priceStr=0.0010317
2025/08/04 20:07:37 [HandleRaw] parsed price=0.00103170
2025/08/04 20:07:37 [HandleRaw] Calling Check(LTCBTC)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"169.18","v":"0.459","S":2,"t":1754327257809}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1754327257819}
2025/08/04 20:07:37 [HandleRaw] symbol: SOLUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=169.18
2025/08/04 20:07:37 [HandleRaw] parsed price=169.18000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(SOLUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@SOLUSDT","d":{"deals":[{"p":"169.18","v":"0.610","S":2,"t":1754327257809}],"e":"spot@public.deals.v3.api"},"s":"SOLUSDT","t":1754327257819}
2025/08/04 20:07:37 [HandleRaw] symbol: SOLUSDT
2025/08/04 20:07:37 [HandleRaw] priceStr=169.18
2025/08/04 20:07:37 [HandleRaw] parsed price=169.18000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(SOLUSDT)
2025/08/04 20:07:37 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@ETHUSDC","d":{"deals":[{"p":"3710.92","v":"0.00028","S":2,"t":1754327257865}],"e":"spot@public.deals.v3.api"},"s":"ETHUSDC","t":1754327257868}
2025/08/04 20:07:37 [HandleRaw] symbol: ETHUSDC
2025/08/04 20:07:37 [HandleRaw] priceStr=3710.92
2025/08/04 20:07:37 [HandleRaw] parsed price=3710.92000000
2025/08/04 20:07:37 [HandleRaw] Calling Check(ETHUSDC)
2025/08/04 20:07:38 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@OPUSDT","d":{"deals":[{"p":"0.711","v":"21.44","S":1,"t":1754327258008}],"e":"spot@public.deals.v3.api"},"s":"OPUSDT","t":1754327258012}
2025/08/04 20:07:38 [HandleRaw] symbol: OPUSDT
2025/08/04 20:07:38 [HandleRaw] priceStr=0.711
2025/08/04 20:07:38 [HandleRaw] parsed price=0.71100000
2025/08/04 20:07:38 [HandleRaw] Calling Check(OPUSDT)
2025/08/04 20:07:38 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@DOGEUSDT","d":{"deals":[{"p":"0.20908","v":"33.53","S":1,"t":1754327258063}],"e":"spot@public.deals.v3.api"},"s":"DOGEUSDT","t":1754327258065}
2025/08/04 20:07:38 [HandleRaw] symbol: DOGEUSDT
2025/08/04 20:07:38 [HandleRaw] priceStr=0.20908
2025/08/04 20:07:38 [HandleRaw] parsed price=0.20908000
2025/08/04 20:07:38 [HandleRaw] Calling Check(DOGEUSDT)
2025/08/04 20:07:38 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@DOGEUSDT","d":{"deals":[{"p":"0.20908","v":"74.70","S":2,"t":1754327258064}],"e":"spot@public.deals.v3.api"},"s":"DOGEUSDT","t":1754327258068}
2025/08/04 20:07:38 [HandleRaw] symbol: DOGEUSDT
2025/08/04 20:07:38 [HandleRaw] priceStr=0.20908
2025/08/04 20:07:38 [HandleRaw] parsed price=0.20908000
2025/08/04 20:07:38 [HandleRaw] Calling Check(DOGEUSDT)
2025/08/04 20:07:38 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@DOGEUSDT","d":{"deals":[{"p":"0.20908","v":"41.77","S":2,"t":1754327258064}],"e":"spot@public.deals.v3.api"},"s":"DOGEUSDT","t":1754327258068}
2025/08/04 20:07:38 [HandleRaw] symbol: DOGEUSDT
2025/08/04 20:07:38 [HandleRaw] priceStr=0.20908
2025/08/04 20:07:38 [HandleRaw] parsed price=0.20908000
2025/08/04 20:07:38 [HandleRaw] Calling Check(DOGEUSDT)
2025/08/04 20:07:38 [HandleRaw] raw: {"c":"spot@public.deals.v3.api@USDCUSDT","d":{"deals":[{"p":"0.9999","v":"432.18","S":2,"t":1754327258113}],"e":"spot@public.deals.v3.api"},"s":"USDCUSDT","t":1754327258116}


