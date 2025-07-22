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
2025/07/22 19:19:26 🔁 Переворачиваем USDTBTC → BTCUSDT
2025/07/22 19:19:26 🔁 Переворачиваем BTCSOL → SOLBTC
2025/07/22 19:19:26 🔁 Переворачиваем USDTETH → ETHUSDT
2025/07/22 19:19:26 🔁 Переворачиваем BTCETH → ETHBTC
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@ADAUSDT
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@SOLBTC
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@XRPBTC
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@BTCUSDT
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@XRPUSDT
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@ETHBTC
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@ETHUSDT
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@ADABTC
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@SOLUSDT
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@XRPETH
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@TRXBTC
2025/07/22 19:19:26 📡 Подписка на: spot@public.deals.v3.api@TRXUSDT
2025/07/22 19:19:26 ✅ Подписка на пары отправлена
2025/07/22 19:19:41 📶 Получен pong от сервера (через 254.222437ms)
2025/07/22 19:19:56 📶 Получен pong от сервера (через 223.677886ms)
2025/07/22 19:20:11 📶 Получен pong от сервера (через 266.766694ms)
2025/07/22 19:20:26 📶 Получен pong от сервера (через 321.171687ms)
2025/07/22 19:20:41 📶 Получен pong от сервера (через 223.724079ms)
2025/07/22 19:20:56 📶 Получен pong от сервера (через 225.429443ms)
2025/07/22 19:21:11 📶 Получен pong от сервера (через 276.359552ms)
2025/07/22 19:21:26 📶 Получен pong от сервера (через 227.997644ms)
2025/07/22 19:21:41 📶 Получен pong от сервера (через 278.052579ms)
2025/07/22 19:21:56 📶 Получен pong от сервера (через 232.082897ms)
2025/07/22 19:22:11 📶 Получен pong от сервера (через 224.656461ms)
2025/07/22 19:22:26 📶 Получен pong от сервера (через 239.647941ms)
2025/07/22 19:22:41 📶 Получен pong от сервера (через 295.708858ms)
2025/07/22 19:22:56 📶 Получен pong от сервера (через 247.25121ms)
2025/07/22 19:23:11 📶 Получен pong от сервера (через 260.725088ms)
2025/07/22 19:23:26 📶 Получен pong от сервера (через 252.809435ms)
2025/07/22 19:23:41 📶 Получен pong от сервера (через 307.208748ms)
2025/07/22 19:23:56 📶 Получен pong от сервера (через 227.306499ms)
2025/07/22 19:24:11 📶 Получен pong от сервера (через 224.807844ms)
2025/07/22 19:24:26 📶 Получен pong от сервера (через 228.252348ms)
2025/07/22 19:24:41 📶 Получен pong от сервера (через 228.19568ms)
2025/07/22 19:24:56 📶 Получен pong от сервера (через 222.263154ms)
2025/07/22 19:25:11 📶 Получен pong от сервера (через 317.355132ms)
2025/07/22 19:25:26 📶 Получен pong от сервера (через 370.426036ms)
2025/07/22 19:25:41 📶 Получен pong от сервера (через 322.228373ms)
2025/07/22 19:25:56 📶 Получен pong от сервера (через 271.179417ms)
2025/07/22 19:26:11 📶 Получен pong от сервера (через 324.990719ms)





