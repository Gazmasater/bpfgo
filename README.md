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


func (a *Arbitrager) HandleRaw(_exchange string, raw []byte) {
	// 1. Обработка ACK-сообщений подписки
	var ack struct {
		ID   int64  `json:"id"`
		Code int    `json:"code"`
		Msg  string `json:"msg"`
	}
	if err := json.Unmarshal(raw, &ack); err == nil && ack.Code == 0 {
		const prefixFail = "Not Subscribed successfully! ["
		if parts := strings.Split(ack.Msg, prefixFail); len(parts) == 2 {
			blocked := strings.Split(strings.TrimSuffix(parts[1], "].  Reason： Blocked! \""), ",")
			for _, ch := range blocked {
				if idx := strings.LastIndex(ch, "@"); idx != -1 {
					sym := ch[idx+1:]
					a.mu.Lock()
					a.realSymbols[sym] = false
					a.mu.Unlock()
					log.Printf("[WS][ACK] ⚠️ Заблокирована пара: %s", sym)
				}
			}
			return
		}
	}

	// 2. Парсинг JSON
	var msg struct {
		Channel string `json:"c"`
		Symbol  string `json:"s"`
		Data    struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("[WS][ERR] ❌ Ошибка парсинга JSON: %v\nRAW: %s", err, string(raw))
		return
	}

	// 3. Проверка полей
	if msg.Symbol == "" {
		log.Printf("[WS][SKIP] ❗ Пустой symbol, raw: %s", string(raw))
		return
	}
	if len(msg.Data.Deals) == 0 {
		log.Printf("[WS][SKIP] ❗ Пустой список deals по символу %s", msg.Symbol)
		return
	}

	// 4. Парсинг цены
	priceStr := msg.Data.Deals[0].Price
	price, err := strconv.ParseFloat(priceStr, 64)
	if err != nil {
		log.Printf("[WS][ERR] ❌ Ошибка преобразования цены: %v, str=%s", err, priceStr)
		return
	}

	// 5. Успешный тик
	log.Printf("[WS][TICK] ✅ %s = %.8f", msg.Symbol, price)

	// 6. Запись latest
	a.mu.Lock()
	a.latest[msg.Symbol] = price
	a.mu.Unlock()

	// 7. Проверка треугольников
	a.Check(msg.Symbol)
}



2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTBCH
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTAAVE
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCBCH
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCTRX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTJASMY
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCWBTC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTXEN
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTNEAR
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTWAVES].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCLUNC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCTRX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTFIL
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTSHIB
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCFIL
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTLTC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTAVAX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCFTT
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTFTT
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCXRP
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCLTC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCMX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCETH
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTAZERO
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCBCH
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCWAVES].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTUNI
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCAAVE
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCSOL
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCAPE
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCATOM
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCENS
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCSOL
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTMINA
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTCAW
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTRAY].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCAVAX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCBTC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTUSDC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTALGO
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCOP].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTSOL
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTWBTC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTADA
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCATOM
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCNEAR
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCETH
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCRAY
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCXRP
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCJASMY
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTMX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTTRX
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCXEN
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTLUNC].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTOP
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCLTC
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCADA
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCMINA
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTENS
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: BTCADA
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCALGO].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCSHIB
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: ETHUNI
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTATOM].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][TICK] ✅ BTCUSDC = 114751.72000000
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCBNB
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCAZERO
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCCAW
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCDOGE
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTXRP
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTDOGE
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTBNB
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDTAPE
2025/08/04 16:56:40 [WS][ACK] ⚠️ Заблокирована пара: USDCUNI].  Reason： Blocked! 
2025/08/04 16:56:40 [WS][TICK] ✅ ALGOUSDC = 0.24540000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDT = 118.25000000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDT = 118.26000000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDT = 118.25000000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDT = 118.25000000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDT = 118.25000000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDT = 118.25000000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDT = 0.74700000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDT = 0.74700000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDT = 0.74690000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDT = 0.74700000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDT = 0.74700000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDT = 0.74700000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDT = 761.19000000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDT = 761.19000000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDT = 761.19000000
2025/08/04 16:56:40 [WS][TICK] ✅ ADAUSDC = 0.74586400
2025/08/04 16:56:40 [WS][TICK] ✅ LTCBTC = 0.00103050
2025/08/04 16:56:40 [WS][TICK] ✅ LTCBTC = 0.00102950
2025/08/04 16:56:40 [WS][TICK] ✅ LTCBTC = 0.00103100
2025/08/04 16:56:40 [WS][TICK] ✅ LTCBTC = 0.00102960
2025/08/04 16:56:40 [WS][TICK] ✅ SOLUSDC = 165.11350000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDC = 118.24900000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDC = 118.25700000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDC = 118.24400000
2025/08/04 16:56:40 [WS][TICK] ✅ LTCUSDC = 118.24600000
2025/08/04 16:56:40 [WS][TICK] ✅ ETHUSDC = 3611.07000000
2025/08/04 16:56:40 [WS][TICK] ✅ BCHUSDT = 556.20000000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDC = 760.12510000
2025/08/04 16:56:40 [WS][TICK] ✅ BCHUSDT = 556.20000000
2025/08/04 16:56:40 [WS][TICK] ✅ USDCUSDT = 0.99990000
2025/08/04 16:56:40 [WS][TICK] ✅ BCHUSDT = 556.20000000
2025/08/04 16:56:40 [WS][TICK] ✅ USDCUSDT = 0.99980000
2025/08/04 16:56:40 [WS][TICK] ✅ USDCUSDT = 0.99980000
2025/08/04 16:56:40 [WS][TICK] ✅ ADABTC = 0.00000651
2025/08/04 16:56:40 [WS][TICK] ✅ ADABTC = 0.00000651
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDC = 760.12520000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDC = 760.12560000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDC = 760.12500000
2025/08/04 16:56:40 [WS][TICK] ✅ UNIETH = 0.00271900
2025/08/04 16:56:40 [WS][TICK] ✅ UNIETH = 0.00271900
2025/08/04 16:56:40 [WS][TICK] ✅ UNIETH = 0.00271900
2025/08/04 16:56:40 [WS][TICK] ✅ UNIETH = 0.00272100
2025/08/04 16:56:40 [WS][TICK] ✅ UNIETH = 0.00272100
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDT = 761.19000000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDT = 761.19000000
2025/08/04 16:56:40 [WS][TICK] ✅ BNBUSDT = 761.19000000
2025/08/04 16:56:40 [WS][TICK] ✅ AVAXUSDT = 22.15000000
2025/08/04 16:56:40 [WS][TICK] ✅ AVAXUSDT = 22.15000000
2025/08/04 16:56:40 [WS][TICK] ✅ AVAXUSDT = 22.15000000
2025/08/04 16:56:40 [WS][TICK] ✅ BCHUSDC = 555.13000000
2025/08/04 16:56:40 [WS][TICK] ✅ BCHUSDC = 554.95000000
2025/08/04 16:56:40 [WS][TICK] ✅ BCHUSDC = 555.00000000

