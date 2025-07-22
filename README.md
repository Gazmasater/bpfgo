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

// Go-бот: подписка на 1 треугольник с ping/pong и автопереподключением
package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strings"
	"time"

	"github.com/gorilla/websocket"
)

type Triangle struct {
	A string `json:"a"`
	B string `json:"b"`
	C string `json:"c"`
}

type SymbolInfo struct {
	Symbol string `json:"symbol"`
}

type ExchangeInfo struct {
	Symbols []SymbolInfo `json:"symbols"`
}

type TickerMsg struct {
	Symbol string `json:"symbol"`
	Data   struct {
		Bid string `json:"b"`
		Ask string `json:"a"`
	} `json:"data"`
}

func ensureTrianglesFile() error {
	triangles := []Triangle{
		{A: "ETH", B: "BTC", C: "USDT"},
	}
	data, _ := json.MarshalIndent(triangles, "", "  ")
	return ioutil.WriteFile("triangles.json", data, 0644)
}

func loadTriangles() ([]Triangle, error) {
	if err := ensureTrianglesFile(); err != nil {
		return nil, fmt.Errorf("не удалось создать triangles.json: %v", err)
	}
	data, err := ioutil.ReadFile("triangles.json")
	if err != nil {
		return nil, err
	}
	var triangles []Triangle
	err = json.Unmarshal(data, &triangles)
	return triangles, err
}

func fetchAvailableSymbols() map[string]bool {
	symbolSet := map[string]bool{}
	resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
	if err != nil {
		log.Println("⚠️ Не удалось загрузить список символов с биржи:", err)
		return symbolSet
	}
	defer resp.Body.Close()

	body, _ := ioutil.ReadAll(resp.Body)
	var info ExchangeInfo
	if err := json.Unmarshal(body, &info); err != nil {
		log.Println("⚠️ Ошибка парсинга exchangeInfo:", err)
		return symbolSet
	}

	for _, s := range info.Symbols {
		symbolSet[s.Symbol] = true
	}
	return symbolSet
}

func buildValidSymbols(triangles []Triangle, valid map[string]bool) []string {
	pairs := map[string]bool{}
	for _, t := range triangles {
		pairs[t.A+t.B] = true
		pairs[t.B+t.C] = true
		pairs[t.C+t.A] = true
	}
	result := []string{}
	for p := range pairs {
		if valid[p] {
			result = append(result, p)
		} else {
			log.Printf("❌ Пара %s не найдена на бирже, пропускаем", p)
		}
	}
	return result
}

func runBot(logFile *os.File) error {
	triangles, err := loadTriangles()
	if err != nil {
		return fmt.Errorf("не удалось загрузить triangles.json: %v", err)
	}

	validSymbols := fetchAvailableSymbols()
	symbols := buildValidSymbols(triangles, validSymbols)
	channels := []string{}
	for _, s := range symbols {
		ch := fmt.Sprintf("spot@public.ticker.v3.api@%s", s)
		channels = append(channels, ch)
		log.Println("📡 Подписка на:", ch)
	}

	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", nil)
	if err != nil {
		return fmt.Errorf("ошибка подключения к WebSocket: %v", err)
	}
	defer conn.Close()

	conn.SetPongHandler(func(appData string) error {
		log.Println("📶 Получен pong от сервера")
		return nil
	})

	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": channels,
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		return fmt.Errorf("ошибка подписки: %v", err)
	}
	log.Println("✅ Подписка на пары отправлена")

	encoder := json.NewEncoder(logFile)
	pingTicker := time.NewTicker(15 * time.Second)
	defer pingTicker.Stop()

	go func() {
		for range pingTicker.C {
			if err := conn.WriteMessage(websocket.PingMessage, []byte("keepalive")); err != nil {
				log.Println("⚠️ Ошибка отправки ping:", err)
			}
		}
	}()

	for {
		_, msg, err := conn.ReadMessage()
		if err != nil {
			log.Println("⚠️ Ошибка чтения WebSocket:", err)
			if websocket.IsUnexpectedCloseError(err) {
				break
			}
			time.Sleep(time.Second)
			continue
		}

		var tick TickerMsg
		if err := json.Unmarshal(msg, &tick); err == nil && tick.Symbol != "" {
			log.Printf("📈 %s → Bid: %s | Ask: %s", tick.Symbol, tick.Data.Bid, tick.Data.Ask)
			entry := map[string]string{
				"symbol": tick.Symbol,
				"bid":    tick.Data.Bid,
				"ask":    tick.Data.Ask,
				"time":   time.Now().Format(time.RFC3339Nano),
			}
			encoder.Encode(entry)
		}
	}
	return nil
}

func main() {
	log.SetOutput(os.Stdout)

	logFile, err := os.OpenFile("prices_log.jsonl", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0644)
	if err != nil {
		log.Fatal("Не удалось создать файл лога:", err)
	}
	defer logFile.Close()

	for {
		err := runBot(logFile)
		log.Printf("🔄 Переподключение через 5 сек... (%v)", err)
		time.Sleep(5 * time.Second)
	}
}


gaz358@gaz358-BOD-WXX9:~/myprog/crypt$ go run .
2025/07/22 16:14:42 ❌ Пара USDTETH не найдена на бирже, пропускаем
2025/07/22 16:14:42 📡 Подписка на: spot@public.ticker.v3.api@BTCUSDT
2025/07/22 16:14:42 📡 Подписка на: spot@public.ticker.v3.api@ETHBTC
2025/07/22 16:14:43 ✅ Подписка на пары отправлена
2025/07/22 16:14:58 📶 Получен pong от сервера
2025/07/22 16:15:13 📶 Получен pong от сервера
2025/07/22 16:15:15 ⚠️ Ошибка чтения WebSocket: websocket: close 1005 (no status)
2025/07/22 16:15:15 🔄 Переподключение через 5 сек... (<nil>)
2025/07/22 16:15:20 ❌ Пара USDTETH не найдена на бирже, пропускаем
2025/07/22 16:15:20 📡 Подписка на: spot@public.ticker.v3.api@ETHBTC
2025/07/22 16:15:20 📡 Подписка на: spot@public.ticker.v3.api@BTCUSDT
2025/07/22 16:15:21 ✅ Подписка на пары отправлена
2025/07/22 16:15:36 📶 Получен pong от сервера
2025/07/22 16:15:51 📶 Получен pong от сервера
2025/07/22 16:15:56 ⚠️ Ошибка чтения WebSocket: websocket: close 1005 (no status)
2025/07/22 16:15:56 🔄 Переподключение через 5 сек... (<nil>)
2025/07/22 16:16:01 ❌ Пара USDTETH не найдена на бирже, пропускаем
2025/07/22 16:16:01 📡 Подписка на: spot@public.ticker.v3.api@ETHBTC
2025/07/22 16:16:01 📡 Подписка на: spot@public.ticker.v3.api@BTCUSDT
2025/07/22 16:16:01 ✅ Подписка на пары отправлена
2025/07/22 16:16:17 📶 Получен pong от сервера
2025/07/22 16:16:32 📶 Получен pong от сервера
2025/07/22 16:16:36 ⚠️ Ошибка чтения WebSocket: websocket: close 1005 (no status)
2025/07/22 16:16:36 🔄 Переподключение через 5 сек... (<nil>)
2025/07/22 16:16:42 ❌ Пара USDTETH не найдена на бирже, пропускаем
2025/07/22 16:16:42 📡 Подписка на: spot@public.ticker.v3.api@ETHBTC
2025/07/22 16:16:42 📡 Подписка на: spot@public.ticker.v3.api@BTCUSDT
2025/07/22 16:16:42 ✅ Подписка на пары отправлена
2025/07/22 16:16:57 📶 Получен pong от сервера
2025/07/22 16:17:12 📶 Получен pong от сервера
2025/07/22 16:17:16 ⚠️ Ошибка чтения WebSocket: websocket: close 1005 (no status)
2025/07/22 16:17:16 🔄 Переподключение через 5 сек... (<nil>)
2025/07/22 16:17:21 ❌ Пара USDTETH не найдена на бирже, пропускаем
2025/07/22 16:17:21 📡 Подписка на: spot@public.ticker.v3.api@BTCUSDT
2025/07/22 16:17:21 📡 Подписка на: spot@public.ticker.v3.api@ETHBTC
2025/07/22 16:17:21 ✅ Подписка на пары отправлена
2025/07/22 16:17:37 📶 Получен pong от сервера
2025/07/22 16:17:52 📶 Получен pong от сервера
2025/07/22 16:17:55 ⚠️ Ошибка чтения WebSocket: websocket: close 1005 (no status)
2025/07/22 16:17:55 🔄 Переподключение через 5 сек... (<nil>)
2025/07/22 16:18:00 ❌ Пара USDTETH не найдена на бирже, пропускаем
2025/07/22 16:18:00 📡 Подписка на: spot@public.ticker.v3.api@ETHBTC
2025/07/22 16:18:00 📡 Подписка на: spot@public.ticker.v3.api@BTCUSDT
2025/07/22 16:18:00 ✅ Подписка на пары отправлена







