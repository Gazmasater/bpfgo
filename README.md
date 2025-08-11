
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



MAP=$(for f in *.proto; do printf -- "--go_opt=M%s=%s " "$f" "crypt_proto/pb"; done)

protoc -I=. \
  --go_out=. --go_opt=paths=source_relative \
  $MAP \
  *.proto



package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/gorilla/websocket"
	"google.golang.org/protobuf/proto"

	pb "crypt_proto/pb" // твой пакет со сгенерёнными *.pb.go
)

// ===== listenKey helpers (только API-KEY в заголовке, без подписи) =====

func createListenKey(apiKey string) (string, error) {
	req, _ := http.NewRequest("POST", "https://api.mexc.com/api/v3/userDataStream", nil)
	req.Header.Set("X-MEXC-APIKEY", apiKey)

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return "", err
	}
	defer resp.Body.Close()

	var out struct{ ListenKey string `json:"listenKey"` }
	if err := json.NewDecoder(resp.Body).Decode(&out); err != nil {
		return "", err
	}
	if out.ListenKey == "" {
		return "", fmt.Errorf("empty listenKey")
	}
	return out.ListenKey, nil
}

func keepAliveListenKey(apiKey, listenKey string, stop <-chan struct{}) {
	t := time.NewTicker(30 * time.Minute) // продлеваем раньше, чем 60 минут TTL
	defer t.Stop()
	for {
		select {
		case <-t.C:
			req, _ := http.NewRequest("PUT", "https://api.mexc.com/api/v3/userDataStream", nil)
			req.Header.Set("X-MEXC-APIKEY", apiKey)
			q := req.URL.Query()
			q.Set("listenKey", listenKey)
			req.URL.RawQuery = q.Encode()
			_, _ = http.DefaultClient.Do(req)
		case <-stop:
			return
		}
	}
}

func closeListenKey(apiKey, listenKey string) {
	req, _ := http.NewRequest("DELETE", "https://api.mexc.com/api/v3/userDataStream", nil)
	req.Header.Set("X-MEXC-APIKEY", apiKey)
	q := req.URL.Query()
	q.Set("listenKey", listenKey)
	req.URL.RawQuery = q.Encode()
	_, _ = http.DefaultClient.Do(req)
}

// =====================================================================

func main() {
	const baseWS = "wss://wbs-api.mexc.com/ws"

	apiKey := os.Getenv("MEXC_API_KEY")
	if apiKey == "" {
		log.Fatal("MEXC_API_KEY is empty. Export it first.")
	}
	// secret пока не нужен для listenKey/WS
	_ = os.Getenv("MEXC_SECRET_KEY")

	listenKey, err := createListenKey(apiKey)
	if err != nil {
		log.Fatal("listenKey:", err)
	}
	defer closeListenKey(apiKey, listenKey)

	stopKA := make(chan struct{})
	go keepAliveListenKey(apiKey, listenKey, stopKA)
	defer close(stopKA)

	// подключаемся к приватному ws-линку; подписываемся на те же public bookTicker
	wsURL := baseWS + "?listenKey=" + listenKey
	c, _, err := websocket.DefaultDialer.Dial(wsURL, nil)
	if err != nil {
		log.Fatal("dial:", err)
	}
	defer c.Close()

	sub := map[string]any{
		"method": "SUBSCRIPTION",
		"params": []string{
			"spot@public.aggre.bookTicker.v3.api.pb@100ms@BTCUSDT",
			"spot@public.aggre.bookTicker.v3.api.pb@100ms@ETHUSDT",
			"spot@public.aggre.bookTicker.v3.api.pb@100ms@ETHBTC",
		},
	}
	if err := c.WriteJSON(sub); err != nil {
		log.Fatal("send sub:", err)
	}

	// поддерживаем линк
	go func() {
		t := time.NewTicker(45 * time.Second)
		defer t.Stop()
		for range t.C {
			_ = c.WriteMessage(websocket.PingMessage, []byte("hb"))
		}
	}()

	for {
		mt, raw, err := c.ReadMessage()
		if err != nil {
			log.Fatal("read:", err)
		}

		// ACK/ошибки — текст/JSON
		if mt == websocket.TextMessage {
			var v any
			if json.Unmarshal(raw, &v) == nil {
				b, _ := json.MarshalIndent(v, "", "  ")
				fmt.Printf("ACK:\n%s\n", b)
			} else {
				fmt.Printf("TEXT:\n%s\n", string(raw))
			}
			continue
		}
		if mt != websocket.BinaryMessage {
			continue
		}

		// 1) Декодируем ОБЁРТКУ
		var w pb.PushDataV3ApiWrapper
		if err := proto.Unmarshal(raw, &w); err != nil {
			// бинарь не нашей схемы — пропускаем
			continue
		}

		// 2) Вытаскиваем symbol/ts из обёртки
		symbol := w.GetSymbol()
		if symbol == "" {
			// если symbol пуст — берём из channel (последний сегмент после '@')
			ch := w.GetChannel()
			if ch != "" {
				parts := strings.Split(ch, "@")
				symbol = parts[len(parts)-1]
			}
		}
		ts := time.Now()
		if t := w.GetSendTime(); t > 0 {
			ts = time.UnixMilli(t)
		}

		// 3) oneof Body → PublicAggreBookTicker (ВЫВОД НЕ МЕНЯЕМ)
		switch body := w.GetBody().(type) {
		case *pb.PushDataV3ApiWrapper_PublicAggreBookTicker:
			bt := body.PublicAggreBookTicker

			bid, _ := strconv.ParseFloat(bt.GetBidPrice(), 64)
			ask, _ := strconv.ParseFloat(bt.GetAskPrice(), 64)
			bq, _ := strconv.ParseFloat(bt.GetBidQuantity(), 64)
			aq, _ := strconv.ParseFloat(bt.GetAskQuantity(), 64)

			// === точь-в-точь ваш формат ===
			fmt.Printf("%s  bid=%.8f (%.6f)  ask=%.8f (%.6f)  ts=%s\n",
				symbol, bid, bq, ask, aq, ts.Format(time.RFC3339Nano))

		default:
			// игнорим прочее
		}
	}
}




export MEXC_API_KEY='mx0vglWtzbBOGF34or'   # можно без export, но так надёжнее
curl -i -X POST 'https://api.mexc.com/api/v3/userDataStream' \
  -H "X-MEXC-APIKEY: $MEXC_API_KEY"


