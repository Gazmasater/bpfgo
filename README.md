
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

message AggreDealPush {
  string s = 1; // symbol
  string p = 2; // price
  string v = 3; // volume
  int64 T = 4;  // timestamp (ms)
}



Создай файл:

bash
Копировать
Редактировать
nano AggreDealPush.proto
И вставь содержимое:


syntax = "proto3";
option go_package = "crypt_proto/pb";

message AggreDealPush {
  string s = 1; // symbol
  string p = 2; // price
  string v = 3; // volume
  int64 T = 4;  // timestamp (milliseconds)
}

protoc --go_out=. --go_opt=paths=source_relative AggreDealPush.proto

ь
crypt_proto/pb/AggreDealPush.pb.go


package main

import (
	"encoding/json"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/gorilla/websocket"
)

var pairsToTest = []string{
	"AGIXUSDT", "FETUSDT", "DODOUSDT", "MXUSDT", "TRXUSDT", "CAWUSDT",
	"VIDTUSDT", "COVERUSDT", "BTCUSDT", "ETHUSDT", "SOLUSDT", "DOGEUSDT",
	"DOTUSDT", "NEARUSDT", "SUIUSDT", "RNDRUSDT", "GALAUSDT", "LINAUSDT",
}

var outputFile = "protobuf_available.txt"

func main() {
	// создаём файл для записи
	f, err := os.Create(outputFile)
	if err != nil {
		log.Fatalf("❌ Не удалось создать файл: %v", err)
	}
	defer f.Close()

	for _, pair := range pairsToTest {
		ok := testProtobufSubscription(pair)
		if ok {
			f.WriteString(pair + "\n")
		}
		time.Sleep(300 * time.Millisecond) // пауза между запросами
	}

	log.Printf("✅ Проверка завершена. Результат сохранён в %s", outputFile)
}

func testProtobufSubscription(symbol string) bool {
	header := http.Header{}
	header.Set("Sec-WebSocket-Protocol", "protobuf")

	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", header)
	if err != nil {
		log.Printf("❌ [%s] Dial error: %v", symbol, err)
		return false
	}
	defer conn.Close()

	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": []string{"spot@public.deals.v3.api@" + symbol},
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		log.Printf("❌ [%s] WriteJSON error: %v", symbol, err)
		return false
	}

	// читаем ответы (макс 2 попытки)
	for i := 0; i < 2; i++ {
		mt, msg, err := conn.ReadMessage()
		if err != nil {
			log.Printf("❌ [%s] Read error: %v", symbol, err)
			return false
		}
		if mt != websocket.TextMessage {
			continue
		}

		var resp map[string]interface{}
		if err := json.Unmarshal(msg, &resp); err != nil {
			continue
		}

		if msgText, ok := resp["msg"].(string); ok {
			switch {
			case msgText == "success":
				log.Printf("✅ [%s] Подписка прошла успешно", symbol)
				return true
			case contains(msgText, "Blocked"):
				log.Printf("⛔ [%s] Заблокировано: %s", symbol, msgText)
				return false
			default:
				log.Printf("⚠️ [%s] Ответ: %s", symbol, msgText)
			}
		}
	}

	log.Printf("⚠️ [%s] Нет ответа на подписку", symbol)
	return false
}

func contains(s, substr string) bool {
	return len(s) >= len(substr) && (s == substr || len(s) > len(substr) && (s[0:len(substr)] == substr || s[len(s)-len(substr):] == substr))
}


gaz358@gaz358-BOD-WXX9:~/myprog/crypt_proto$ go run .
2025/08/06 21:45:39 ⚠️ [AGIXUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@AGIXUSDT].  Reason： Blocked! 
2025/08/06 21:46:09 ❌ [AGIXUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:46:10 ⚠️ [FETUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@FETUSDT].  Reason： Blocked! 
2025/08/06 21:46:44 ❌ [FETUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:46:45 ⚠️ [DODOUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@DODOUSDT].  Reason： Blocked! 
2025/08/06 21:47:19 ❌ [DODOUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:47:20 ⚠️ [MXUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@MXUSDT].  Reason： Blocked! 
2025/08/06 21:47:52 ❌ [MXUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:47:53 ⚠️ [TRXUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@TRXUSDT].  Reason： Blocked! 
2025/08/06 21:48:23 ❌ [TRXUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:48:24 ⚠️ [CAWUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@CAWUSDT].  Reason： Blocked! 
2025/08/06 21:48:58 ❌ [CAWUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:48:59 ⚠️ [VIDTUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@VIDTUSDT].  Reason： Blocked! 
2025/08/06 21:49:30 ❌ [VIDTUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:49:32 ⚠️ [COVERUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@COVERUSDT].  Reason： Blocked! 
2025/08/06 21:50:04 ❌ [COVERUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:50:05 ⚠️ [BTCUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@BTCUSDT].  Reason： Blocked! 
2025/08/06 21:50:38 ❌ [BTCUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:50:39 ⚠️ [ETHUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@ETHUSDT].  Reason： Blocked! 
2025/08/06 21:51:14 ❌ [ETHUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:51:15 ⚠️ [SOLUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@SOLUSDT].  Reason： Blocked! 
2025/08/06 21:51:50 ❌ [SOLUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:51:51 ⚠️ [DOGEUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@DOGEUSDT].  Reason： Blocked! 
2025/08/06 21:52:26 ❌ [DOGEUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:52:27 ⚠️ [DOTUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@DOTUSDT].  Reason： Blocked! 
2025/08/06 21:53:00 ❌ [DOTUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:53:02 ⚠️ [NEARUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@NEARUSDT].  Reason： Blocked! 
2025/08/06 21:53:35 ❌ [NEARUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:53:36 ⚠️ [SUIUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@SUIUSDT].  Reason： Blocked! 
2025/08/06 21:54:07 ❌ [SUIUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:54:08 ⚠️ [RNDRUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@RNDRUSDT].  Reason： Blocked! 
2025/08/06 21:54:40 ❌ [RNDRUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:54:41 ⚠️ [GALAUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@GALAUSDT].  Reason： Blocked! 
2025/08/06 21:55:15 ❌ [GALAUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:55:17 ⚠️ [LINAUSDT] Ответ: Not Subscribed successfully! [spot@public.deals.v3.api@LINAUSDT].  Reason： Blocked! 
2025/08/06 21:55:49 ❌ [LINAUSDT] Read error: websocket: close 1005 (no status)
2025/08/06 21:55:49 ✅ Проверка завершена. Результат сохранён в protobuf_available.txt
gaz358@gaz358-BOD-WXX9:~/myprog/crypt_proto$ 




