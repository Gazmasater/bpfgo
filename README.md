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


gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/07/27 18:44:39 🔺 ARB ADA/USDT/BTC profit=1401420371070.588% total=1401420371070.588%
2025/07/27 18:44:40 🔺 ARB ADA/USDT/BTC profit=1401420371070.588% total=2802840742141.175%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401420489388.956% total=4204261231530.131%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401420489388.956% total=5605681720919.087%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401420489388.956% total=7007102210308.043%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401420371070.588% total=8408522581378.631%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401420371070.588% total=9809942952449.219%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401420371070.588% total=11211363323519.807%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401249341003.323% total=12612612664523.129%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401591401137.853% total=14014204065660.982%
2025/07/27 18:44:41 🔺 ARB ADA/USDT/BTC profit=1401249341003.323% total=15415453406664.305%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249341003.323% total=16816702747667.627%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249222699.395% total=18217951970367.023%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249222699.395% total=19619201193066.418%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249222699.395% total=21020450415765.812%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249222699.395% total=22421699638465.207%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249222699.395% total=23822948861164.602%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401249222699.395% total=25224198083863.996%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400641569090.201% total=26624839652954.195%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844061727.065% total=28025683714681.262%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844061727.065% total=29426527776408.328%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844061727.065% total=30827371838135.395%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844061727.065% total=32228215899862.461%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844179996.787% total=33629060079859.250%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844179996.787% total=35029904259856.039%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844179996.787% total=36430748439852.828%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1400844179996.787% total=37831592619849.617%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=39232778761076.750%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401015160611.958% total=40633793921688.711%
2025/07/27 18:44:42 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=42034980062915.844%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=43436166204142.977%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=44837352345370.109%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=46238538486597.242%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=47639724627824.375%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401186141227.129% total=49040910769051.508%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1400781115604.546% total=50441691884656.055%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1400983599142.520% total=51842675483798.578%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401591401137.853% total=53244266884936.430%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401591401137.853% total=54645858286074.281%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1401794119014.804% total=56047652405089.086%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1400983599142.520% total=57448636004231.609%
2025/07/27 18:44:43 🔺 ARB ADA/USDT/BTC profit=1400641687342.827% total=58849277691574.438%










