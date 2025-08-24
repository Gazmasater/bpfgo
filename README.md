
mx0vgldDsxPkp5g9tc
82889cdd48fe4437b474558b91066c9d

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


package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"net/url"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/gorilla/websocket"
	"github.com/joho/godotenv"
	"google.golang.org/protobuf/proto"

	pb "crypt_proto/pb" // твои *.pb.go из схем MEXC v3
)

func main() {
	_ = godotenv.Load(".env")

	apiKey := os.Getenv("MEXC_API_KEY")
	secret := os.Getenv("MEXC_SECRET_KEY")
	if apiKey == "" || secret == "" {
		log.Fatal("MEXC_API_KEY / MEXC_SECRET_KEY пусты в .env")
	}

	// --- параметры ---
	symbol := os.Getenv("SYMBOL")
	if symbol == "" {
		symbol = "BTCUSDT"
	}
	interval := os.Getenv("BOOK_INTERVAL")
	if interval == "" {
		interval = "100ms" // варианты обычно: 100ms / 300ms / 500ms / 1s
	}

	// --- вычислим дрейф времени, чтобы не ловить 700003 ---
	drift, err := mexcDriftMs()
	if err != nil {
		log.Printf("⚠️ Не удалось получить server time, продолжаю без дрейфа: %v", err)
		drift = 0
	}

	// --- создаём listenKey (SIGNED с учетом дрейфа и широким recvWindow) ---
	lk, err := createListenKey(apiKey, secret, drift)
	if err != nil {
		log.Fatal(err)
	}
	defer closeListenKey(apiKey, secret, lk, drift)

	// --- коннект к приватному WS ---
	wsURL := "wss://wbs.mexc.com/ws?listenKey=" + lk
	conn, _, err := websocket.DefaultDialer.Dial(wsURL, nil)
	if err != nil {
		log.Fatal("dial:", err)
	}
	defer conn.Close()

	// --- подписка на protobuf bookTicker ---
	channel := "spot@public.bookTicker.v3.api.pb@" + interval + "@" + symbol
	sub := map[string]any{
		"method": "SUBSCRIPTION",
		"params": []string{channel},
		"id":     time.Now().Unix(),
	}
	if err := conn.WriteJSON(sub); err != nil {
		log.Fatal("send sub:", err)
	}
	log.Printf("✅ SUB → %s", channel)

	// --- heartbeat ---
	go func() {
		t := time.NewTicker(45 * time.Second)
		defer t.Stop()
		for range t.C {
			_ = conn.WriteMessage(websocket.PingMessage, []byte("hb"))
		}
	}()

	// --- чтение ---
	for {
		mt, raw, err := conn.ReadMessage()
		if err != nil {
			log.Fatal("read:", err)
		}

		// ACK / ошибки — текст
		if mt == websocket.TextMessage {
			// красиво распечатаем
			var v any
			if json.Unmarshal(raw, &v) == nil {
				b, _ := json.MarshalIndent(v, "", "  ")
				fmt.Printf("ACK:\n%s\n", b)
			} else {
				fmt.Println("TEXT:", string(raw))
			}
			continue
		}
		if mt != websocket.BinaryMessage {
			continue
		}

		// бинарь → protobuf PushDataV3ApiWrapper
		if out, ok := parsePBWrapperToSP(raw); ok {
			// {"s":"SYMBOL","p":"PRICE"} — mid=(bid+ask)/2
			fmt.Println(string(out))
		}
	}
}

/* -------------------------- REST utils -------------------------- */

// смещение серверных часов MEXC относительно локальных (ms)
func mexcDriftMs() (int64, error) {
	resp, err := (&http.Client{Timeout: 5 * time.Second}).Get("https://api.mexc.com/api/v3/time")
	if err != nil {
		return 0, err
	}
	defer resp.Body.Close()
	var v struct{ ServerTime int64 `json:"serverTime"` }
	if err := json.NewDecoder(resp.Body).Decode(&v); err != nil {
		return 0, err
	}
	local := time.Now().UnixMilli()
	return v.ServerTime - local, nil
}

func hmacSHA256Hex(secret, data string) string {
	h := hmac.New(sha256.New, []byte(secret))
	h.Write([]byte(data))
	return hex.EncodeToString(h.Sum(nil))
}

// добавляет timestamp(+drift), recvWindow=60000 и signature в query
func signParams(secret string, v url.Values, driftMs int64) url.Values {
	if v == nil {
		v = url.Values{}
	}
	v.Set("timestamp", strconv.FormatInt(time.Now().UnixMilli()+driftMs, 10))
	if v.Get("recvWindow") == "" {
		v.Set("recvWindow", "60000")
	}
	v.Set("signature", hmacSHA256Hex(secret, v.Encode()))
	return v
}

func rest(method, endpoint, apiKey string, v url.Values) (int, []byte, error) {
	if len(v) > 0 {
		endpoint += "?" + v.Encode()
	}
	req, _ := http.NewRequest(method, endpoint, nil)
	req.Header.Set("X-MEXC-APIKEY", apiKey)
	resp, err := (&http.Client{Timeout: 10 * time.Second}).Do(req)
	if err != nil {
		return 0, nil, err
	}
	defer resp.Body.Close()
	b, _ := io.ReadAll(resp.Body)
	return resp.StatusCode, b, nil
}

func createListenKey(apiKey, secret string, driftMs int64) (string, error) {
	st, body, err := rest("POST", "https://api.mexc.com/api/v3/userDataStream", apiKey, signParams(secret, nil, driftMs))
	if err != nil {
		return "", fmt.Errorf("listenKey req: %w", err)
	}
	if st != http.StatusOK {
		return "", fmt.Errorf("listenKey %d: %s", st, strings.TrimSpace(string(body)))
	}
	var out struct{ ListenKey string `json:"listenKey"` }
	if err := json.Unmarshal(body, &out); err != nil || out.ListenKey == "" {
		return "", fmt.Errorf("listenKey decode: %v; body=%s", err, body)
	}
	return out.ListenKey, nil
}

func closeListenKey(apiKey, secret, listenKey string, driftMs int64) {
	v := url.Values{}
	v.Set("listenKey", listenKey)
	_, _, _ = rest("DELETE", "https://api.mexc.com/api/v3/userDataStream", apiKey, signParams(secret, v, driftMs))
}

/* ---------------------- Protobuf parsing ------------------------ */

// Поддержим два типа: PublicBookTicker и PublicAggreBookTicker.
// На выход даём {"s","p"} где p = mid(bid, ask).
func parsePBWrapperToSP(raw []byte) ([]byte, bool) {
	var w pb.PushDataV3ApiWrapper
	if err := proto.Unmarshal(raw, &w); err != nil {
		return nil, false
	}

	sym := w.GetSymbol()
	if sym == "" {
		ch := w.GetChannel()
		if i := strings.LastIndex(ch, "@"); i >= 0 && i+1 < len(ch) {
			sym = ch[i+1:]
		}
	}
	if sym == "" {
		return nil, false
	}

	// 1) Обычный bookTicker
	if b1, ok := w.GetBody().(*pb.PushDataV3ApiWrapper_PublicBookTicker); ok && b1.PublicBookTicker != nil {
		bp := b1.PublicBookTicker.GetBidPrice()
		ap := b1.PublicBookTicker.GetAskPrice()
		if bp == "" || ap == "" {
			return nil, false
		}
		bid, err1 := strconv.ParseFloat(bp, 64)
		ask, err2 := strconv.ParseFloat(ap, 64)
		if err1 != nil || err2 != nil || bid <= 0 || ask <= 0 {
			return nil, false
		}
		mid := (bid + ask) / 2
		out := fmt.Sprintf(`{"s":"%s","p":"%.10f"}`, sym, mid)
		return []byte(out), true
	}

	// 2) AggreBookTicker (если вдруг подпишешься на aggre)
	if b2, ok := w.GetBody().(*pb.PushDataV3ApiWrapper_PublicAggreBookTicker); ok && b2.PublicAggreBookTicker != nil {
		bp := b2.PublicAggreBookTicker.GetBidPrice()
		ap := b2.PublicAggreBookTicker.GetAskPrice()
		if bp == "" || ap == "" {
			return nil, false
		}
		bid, err1 := strconv.ParseFloat(bp, 64)
		ask, err2 := strconv.ParseFloat(ap, 64)
		if err1 != nil || err2 != nil || bid <= 0 || ask <= 0 {
			return nil, false
		}
		mid := (bid + ask) / 2
		out := fmt.Sprintf(`{"s":"%s","p":"%.10f"}`, sym, mid)
		return []byte(out), true
	}

	return nil, false
}



curl https://api.ipify.org


gaz358@gaz358-BOD-WXX9:~/myprog/crypt_proto$ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute 
       valid_lft forever preferred_lft forever
2: wlp0s20f3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether e4:fd:45:a8:ec:0b brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.71/24 brd 192.168.1.255 scope global dynamic noprefixroute wlp0s20f3
       valid_lft 86258sec preferred_lft 86258sec
    inet6 fe80::d6b2:9200:15bb:a0e8/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
3: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:05:e2:0c:b6 brd ff:ff:ff:ff:ff:ff
    inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
       valid_lft forever preferred_lft forever
gaz358@gaz358-BOD-WXX9:~/myprog/crypt_proto$ curl https://api.ipify.org
2.94.139.198gaz358@gaz358-BOD-WXX9:~/myprog/crypt_proto$ 

