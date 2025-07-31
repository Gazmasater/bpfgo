8ae390ad89f04bec97cb7b81413de813


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
2025/07/31 19:23:15 [GRAPH] ONT -> [BTC]
2025/07/31 19:23:15 [GRAPH] HYPE -> [USDT]
2025/07/31 19:23:15 [GRAPH] AIXBT -> [USDC]
2025/07/31 19:23:15 [GRAPH] ZBCN -> [USDC]
2025/07/31 19:23:15 [GRAPH] H -> [USDT]
2025/07/31 19:23:15 [GRAPH] CORN -> [USDC]
2025/07/31 19:23:15 [GRAPH] ANKR -> [USDT]
2025/07/31 19:23:15 [GRAPH] ARB -> [USDT]
2025/07/31 19:23:15 [GRAPH] GHIBLI -> [USDC]
2025/07/31 19:23:15 [GRAPH] MINA -> [USDT]
2025/07/31 19:23:15 [GRAPH] OP -> [USDT]
2025/07/31 19:23:15 [GRAPH] NIL -> [USDT]
2025/07/31 19:23:15 [GRAPH] SIGN -> [USDC]
2025/07/31 19:23:15 [GRAPH] ES -> [USDT]
2025/07/31 19:23:15 [GRAPH] TON -> [USDC EUR]
2025/07/31 19:23:15 [GRAPH] RBNT -> [USDC]
2025/07/31 19:23:15 [GRAPH] AZERO -> [USDT]
2025/07/31 19:23:15 [GRAPH] WHITE -> [USDT]
2025/07/31 19:23:15 [GRAPH] ETC -> [BTC]
2025/07/31 19:23:15 [GRAPH] DSYNC -> [USDC]
2025/07/31 19:23:15 [GRAPH] TRUMP -> [USDT EUR USDC USD1]
2025/07/31 19:23:15 [GRAPH] TRX -> [BTC USDT]
2025/07/31 19:23:15 [GRAPH] SXT -> [USDC]
2025/07/31 19:23:15 [GRAPH] AERGO -> [USDT]
2025/07/31 19:23:15 [GRAPH] CGPT -> [USDC]
2025/07/31 19:23:15 [GRAPH] MNT -> [USDC]
2025/07/31 19:23:15 [GRAPH] GRIFFAIN -> [USDT]
2025/07/31 19:23:15 [GRAPH] GUN -> [USDC]
2025/07/31 19:23:15 [GRAPH] NAKA -> [USDC]
2025/07/31 19:23:15 [GRAPH] ENS -> [USDT]
2025/07/31 19:23:15 [GRAPH] OBOL -> [USDT]
2025/07/31 19:23:15 [GRAPH] MUBARAK -> [USDC]
2025/07/31 19:23:15 [GRAPH] DASH -> [USDT]
2025/07/31 19:23:15 [GRAPH] DOT -> [BTC USDT]
2025/07/31 19:23:15 [GRAPH] USDE -> [ENA BTC XRP KAS USDT]
2025/07/31 19:23:15 [GRAPH] BKN -> [USDT]
2025/07/31 19:23:15 [GRAPH] PUMP -> [USDC]
2025/07/31 19:23:15 [GRAPH] ACH -> [USDT]
2025/07/31 19:23:15 [GRAPH] DOOD -> [USDC]
2025/07/31 19:23:15 [GRAPH] WBTC -> [USDT USDC]
2025/07/31 19:23:15 [GRAPH] KEKIUS -> [USDC]
2025/07/31 19:23:15 [GRAPH] SUPRA -> [USDC]
2025/07/31 19:23:15 [GRAPH] B2 -> [USDT]
2025/07/31 19:23:15 [GRAPH] SOSO -> [USDT]
2025/07/31 19:23:15 [GRAPH] MORE -> [USDT]
2025/07/31 19:23:15 [GRAPH] ONDO -> [USDT]
2025/07/31 19:23:15 [GRAPH] DOLO -> [USDT]
2025/07/31 19:23:15 [GRAPH] USELESS -> [USDC]
2025/07/31 19:23:15 [GRAPH] SERAPH -> [USDC]
2025/07/31 19:23:15 [GRAPH] INIT -> [USDC]
2025/07/31 19:23:15 [GRAPH] SNX -> [USDT]
2025/07/31 19:23:15 [GRAPH] USD1 -> [USDT MELANIA BTC ETH LIBERTY BANK TAG XRP KAS TRUMP PI]
2025/07/31 19:23:15 [GRAPH] PLUME -> [USDC]
2025/07/31 19:23:15 [GRAPH] ALCH -> [USDC]
2025/07/31 19:23:15 [GRAPH] ERA -> [USDC]
2025/07/31 19:23:15 [GRAPH] DEXE -> [USDT]
2025/07/31 19:23:15 [GRAPH] POPCAT -> [USDC]
2025/07/31 19:23:15 [GRAPH] AR -> [USDT]
2025/07/31 19:23:15 [GRAPH] FET -> [USDC]
2025/07/31 19:23:15 [GRAPH] BOOM -> [USDC]
2025/07/31 19:23:15 [GRAPH] BABY -> [USDT]
2025/07/31 19:23:15 [GRAPH] 1DOLLAR -> [USDT]
2025/07/31 19:23:15 [GRAPH] NPC -> [USDC]
2025/07/31 19:23:15 [GRAPH] AGT -> [USDT]
2025/07/31 19:23:15 [GRAPH] KILO -> [USDT]
2025/07/31 19:23:15 [GRAPH] ATOM -> [BTC USDT]
2025/07/31 19:23:15 [GRAPH] ZORA -> [USDT]
2025/07/31 19:23:15 [GRAPH] APE -> [USDT]
2025/07/31 19:23:15 [GRAPH] RAY -> [USDT]
2025/07/31 19:23:15 [GRAPH] OBT -> [USDC]
2025/07/31 19:23:15 [GRAPH] PEPE -> [EUR USDC USDT]
2025/07/31 19:23:15 [GRAPH] NMR -> [USDT]
2025/07/31 19:23:15 [GRAPH] MELANIA -> [USD1 USDC EUR]
2025/07/31 19:23:15 [GRAPH] SNS -> [USDC]
2025/07/31 19:23:15 [GRAPH] EGL1 -> [USDT]
2025/07/31 19:23:15 [GRAPH] NODE -> [USDC]
2025/07/31 19:23:15 [GRAPH] WAVES -> [USDT]
2025/07/31 19:23:15 [GRAPH] ETH -> [EUR USDC USD1 CRV BRL BTC USDT MX OXT LINK XRP]
2025/07/31 19:23:15 [GRAPH] NEAR -> [USDT]
2025/07/31 19:23:15 [GRAPH] PENGU -> [USDT]
2025/07/31 19:23:15 [GRAPH] MOONPIG -> [USDT]
2025/07/31 19:23:15 [GRAPH] SOPH -> [USDT]
2025/07/31 19:23:15 [GRAPH] XRP -> [USDC EUR BTC USD1 USDE ETH USDT]
2025/07/31 19:23:15 [GRAPH] PRAI -> [USDC]
2025/07/31 19:23:15 [GRAPH] KERNEL -> [USDC]
2025/07/31 19:23:15 [GRAPH] LINK -> [USDT USDC ETH]
2025/07/31 19:23:15 [GRAPH] NXPC -> [USDT]
2025/07/31 19:23:15 [GRAPH] SOL -> [BTC USDT EUR USDC]
2025/07/31 19:23:15 [GRAPH] BANK -> [USDC USD1]
2025/07/31 19:23:15 [GRAPH] PUMPBTC -> [USDT]
2025/07/31 19:23:15 [GRAPH] FARTCOIN -> [USDC]
2025/07/31 19:23:15 [GRAPH] TIA -> [USDT]
2025/07/31 19:23:15 [GRAPH] APT -> [USDC]
2025/07/31 19:23:15 [GRAPH] USDR -> [USDT]
2025/07/31 19:23:15 [GRAPH] RARE -> [USDT]
2025/07/31 19:23:15 [GRAPH] LOT -> [USDT]
2025/07/31 19:23:15 [GRAPH] BCH -> [USDT BTC]
2025/07/31 19:23:15 [GRAPH] MILK -> [USDT]
2025/07/31 19:23:15 [GRAPH] RAI -> [USDT]
2025/07/31 19:23:15 [GRAPH] PEAQ -> [USDT]
2025/07/31 19:23:15 [GRAPH] AI16Z -> [USDT]
2025/07/31 19:23:15 [GRAPH] SHM -> [USDT]
2025/07/31 19:23:15 [GRAPH] IDOL -> [USDT]
2025/07/31 19:23:15 [GRAPH] SUI -> [USDT USDC EUR]
2025/07/31 19:23:15 [GRAPH] NEO -> [BTC]
2025/07/31 19:23:15 [GRAPH] B -> [USDT]
2025/07/31 19:23:15 [GRAPH] SHIB -> [USDT USDC]
2025/07/31 19:23:15 [GRAPH] KAS -> [USDC USD1 USDE EUR]
2025/07/31 19:23:15 [GRAPH] TAG -> [USD1]
2025/07/31 19:23:15 [GRAPH] BABYDOGE -> [USDT]
2025/07/31 19:23:15 [GRAPH] SPK -> [USDT]
2025/07/31 19:23:15 [GRAPH] RFC -> [USDT]
2025/07/31 19:23:15 [GRAPH] SKATE -> [USDT]
2025/07/31 19:23:15 [GRAPH] ULTIMA -> [EUR USDC USDT]
2025/07/31 19:23:15 [GRAPH] KAITO -> [USDT]
2025/07/31 19:23:15 [GRAPH] HOME -> [USDT]
2025/07/31 19:23:15 [GRAPH] EUR -> [PEPE USDC PI DOGE ETH WIF TON USDT XRP ULTIMA BTC MX TRUMP MELANIA SUI RIO TAO LTC SOL KAS ADA]
2025/07/31 19:23:15 [GRAPH] KAIA -> [USDT]
2025/07/31 19:23:15 [GRAPH] PNUT -> [USDT]
2025/07/31 19:23:15 [GRAPH] SAHARA -> [USDT]
2025/07/31 19:23:15 [GRAPH] RSR -> [USDT]
2025/07/31 19:23:15 [GRAPH] VIRTUAL -> [USDT]
2025/07/31 19:23:15 [GRAPH] ELDE -> [USDC]
2025/07/31 19:23:15 [GRAPH] BROCCOLIF3B -> [USDC]
2025/07/31 19:23:15 [GRAPH] RED -> [USDT]
2025/07/31 19:23:15 [GRAPH] EPT -> [USDT]
2025/07/31 19:23:15 [GRAPH] OXT -> [ETH]
2025/07/31 19:23:15 [GRAPH] QTUM -> [BTC]
2025/07/31 19:23:15 [GRAPH] PROMPT -> [USDT]
2025/07/31 19:23:15 [GRAPH] BLUM -> [USDT]
2025/07/31 19:23:15 [GRAPH] A -> [BTC USDC]
2025/07/31 19:23:15 [GRAPH] WIF -> [EUR USDC]
2025/07/31 19:23:15 [GRAPH] AVAX -> [USDT USDC]
2025/07/31 19:23:15 [GRAPH] LAUNCHCOIN -> [USDC]
2025/07/31 19:23:15 [GRAPH] RENDER -> [USDT]
2025/07/31 19:23:15 [GRAPH] SPX -> [USDC]
2025/07/31 19:23:15 [GRAPH] FHE -> [USDC]
2025/07/31 19:23:15 [GRAPH] QUBIC -> [USDT]
2025/07/31 19:23:15 [GRAPH] HBAR -> [USDT]
2025/07/31 19:23:15 [GRAPH] LA -> [USDT]
2025/07/31 19:23:15 [GRAPH] XCN -> [USDT]
2025/07/31 19:23:15 [GRAPH] HUMA -> [USDC]
2025/07/31 19:23:15 [GRAPH] SGC -> [USDC]
2025/07/31 19:23:15 [GRAPH] USDT -> [LINGO KILO DASH WAVES BTC AR USD1 BUTTHOLE NMR WBTC ANKR SPK BLUM LUNC AVAX SUI KAIA QUBIC LOT RFC STO TAO ALGO XLM PNUT H AERGO SOL BDX BKN B HBAR NEWT SKATE ZORA JASMY LA EUR SHIB RED USDC BERA PUMPBTC BCH ARB TRUMP XCN VINE HYPE APE SOON ACH EGL1 SAHARA BABY ULTIMA FTT INJ LINK ETH NXPC RSR B2 GRIFFAIN 1DOLLAR SOSO EPT BMT MILK MORE C ONDO RENDER XEN SEN DOLO VIRTUAL ENS ADA DOT RAI KAITO TIA MINA CRV MYX BRL OP DEXE USDR RAY NIL PEAQ AZERO TRX HOME VELVET RARE MX BEE BOMB WHITE OMG NEAR PI PENGU PROMPT ATOM OBOL AI16Z SHM ASRR USDE WAL UNI MOONPIG SNX ES DOGE AIOT BABYDOGE SOPH IDOL PEPE XRP AGT]
2025/07/31 19:23:15 [GRAPH] USDC -> [RIO DSYNC EUR BTC FHE RWA ELDE BROCCOLIF3B S PLUME MELANIA ICP UNI XRP XMR PRAI ALCH BANK BRL ZBCN USELESS KEKIUS SUI FET FLY TON OMI AVAX ERA PUMP ADA RDO USDT TURBO KERNEL BOOM FARTCOIN RBNT ETH LAUNCHCOIN ENA BNB WCT AAVE CGPT ULTIMA WIF XDC SNS MUBARAK GHIBLI MNT KAS FIL DOOD SUPRA BDXN GUN TRUMP LTC NPC LINK NODE XLM NAKA FLOCK PARTI SPX SHIB APT CAW WBTC SERAPH MX HUMA OBT AIXBT A POPCAT SXT PEPE SOL HYPER SIGN TAO VIDT INIT CORN SGC]
2025/07/31 19:23:15 [GRAPH] ALGO -> [USDT]
2025/07/31 19:23:15 [GRAPH] ADA -> [USDC USDT BTC EUR]
2025/07/31 19:23:15 [GRAPH] RDO -> [USDC]
2025/07/31 19:23:15 [GRAPH] BMT -> [USDT]
2025/07/31 19:23:15 [GRAPH] SEN -> [USDT]
2025/07/31 19:23:15 [GRAPH] OMG -> [USDT]
2025/07/31 19:23:15 [GRAPH] ENA -> [USDE USDC]
2025/07/31 19:23:15 [GRAPH] STO -> [USDT]
2025/07/31 19:23:15 [GRAPH] TURBO -> [USDC]
2025/07/31 19:23:15 [GRAPH] BERA -> [USDT]
2025/07/31 19:23:15 [GRAPH] FIL -> [USDC]
2025/07/31 19:23:15 [GRAPH] C -> [USDT]
2025/07/31 19:23:15 [GRAPH] WAL -> [USDT]
2025/07/31 19:23:15 [GRAPH] BRL -> [MX USDC ETH BTC USDT]
2025/07/31 19:23:15 [GRAPH] BUTTHOLE -> [USDT]
2025/07/31 19:23:15 [GRAPH] DOGE -> [EUR USDT]
2025/07/31 19:23:15 [GRAPH] NEWT -> [USDT]
2025/07/31 19:23:15 [GRAPH] WCT -> [USDC]
2025/07/31 19:23:15 [GRAPH] VELVET -> [USDT]
2025/07/31 19:23:15 [GRAPH] RWA -> [USDC]
2025/07/31 19:23:15 [GRAPH] XMR -> [USDC]
2025/07/31 19:23:15 [GRAPH] VINE -> [USDT]
2025/07/31 19:23:15 [GRAPH] MYX -> [USDT]
2025/07/31 19:23:15 [GRAPH] HYPER -> [USDC]
2025/07/31 19:23:15 [GRAPH] PI -> [EUR USD1 USDT]
2025/07/31 19:23:15 [GRAPH] LIBERTY -> [USD1]
2025/07/31 19:23:15 [GRAPH] BNB -> [USDC]
2025/07/31 19:23:15 [GRAPH] FTT -> [USDT]
2025/07/31 19:23:15 [GRAPH] FLOCK -> [USDC]
2025/07/31 19:23:15 [GRAPH] RIO -> [USDC EUR]
2025/07/31 19:23:15 [GRAPH] ICP -> [USDC]
2025/07/31 19:23:15 [GRAPH] JASMY -> [USDT]
2025/07/31 19:23:15 [GRAPH] LTC -> [USDC BTC EUR]
2025/07/31 19:23:15 [GRAPH] ASRR -> [USDT]
2025/07/31 19:23:15 [GRAPH] AIOT -> [USDT]
2025/07/31 19:23:15 [GRAPH] MX -> [BRL BTC EUR ETH USDC USDT]
2025/07/31 19:23:15 [GRAPH] XLM -> [USDT USDC]
2025/07/31 19:23:15 [GRAPH] BDX -> [USDT]
2025/07/31 19:23:15 [GRAPH] INJ -> [USDT]
2025/07/31 19:23:15 [GRAPH] BDXN -> [USDC]
2025/07/31 19:23:15 [GRAPH] XEN -> [USDT]
2025/07/31 19:23:15 [GRAPH] CAW -> [USDC]
2025/07/31 19:23:15 [GRAPH] VIDT -> [USDC]
2025/07/31 19:23:15 [GRAPH] TAO -> [USDT EUR USDC]
2025/07/31 19:23:15 [GRAPH] UNI -> [USDC USDT]
2025/07/31 19:23:15 [GRAPH] OMI -> [USDC]
2025/07/31 19:23:15 [GRAPH] AAVE -> [USDC]
2025/07/31 19:23:15 [GRAPH] CRV -> [ETH USDT]
2025/07/31 19:23:15 [GRAPH] BOMB -> [USDT]
2025/07/31 19:23:15 [GRAPH] S -> [USDC]
2025/07/31 19:23:15 [GRAPH] FLY -> [USDC]
2025/07/31 19:23:15 [GRAPH] SOON -> [USDT]
2025/07/31 19:23:15 [GRAPH] XDC -> [USDC]
2025/07/31 19:23:15 [GRAPH] PARTI -> [USDC]
2025/07/31 19:23:15 [GRAPH] BEE -> [USDT]
2025/07/31 19:23:15 [GRAPH] LINGO -> [USDT]
2025/07/31 19:23:15 [GRAPH] BTC -> [DOT USDT USDC A MX ONT SOL ATOM NEO USD1 EUR ETH BRL USDE XRP TRX LTC QTUM ADA BCH ETC]
2025/07/31 19:23:15 [GRAPH] LUNC -> [USDT]
2025/07/31 19:23:15 [INFO] Found 612 directed triangles from 270 pairs
2025/07/31 19:23:15 ✅ Всего подходящих пар: 514
2025/07/31 19:23:15 📝 available_all_symbols.log и excluded_all_symbols.log сохранены
2025/07/31 19:23:15 !!!!!!!![DEBUG] Биржа вернула 514 доступных пар
2025/07/31 19:23:15 [❌ FILTER] RIO-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] RIO-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LTC-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LTC-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] LTC-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] LTC-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LTC-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LTC-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BRL-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BRL-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BRL-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BRL-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] MX-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] MX-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BTC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-BTC-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-EUR-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] MX-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-ETH-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-ETH-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-ETH-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-ETH-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-ETH-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] MX-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDC-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] MX-USDC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDT-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MX-USDT-BRL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XLM-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] XLM-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] TAO-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TAO-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] TAO-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TAO-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] TAO-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TAO-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] UNI-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] UNI-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] CRV-ETH-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] CRV-USDT-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-DOT-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-SOL excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-USDC excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-BCH excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-ADA excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-DOT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-TRX excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-MX excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-ATOM excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-USDE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDT-XRP excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-XRP excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-BRL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-ADA excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-USDT excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-LTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-MX excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-A excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDC-SOL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-A-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-MX-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-MX-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-MX-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-MX-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-MX-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-SOL-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-SOL-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-SOL-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-ATOM-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USD1-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USD1-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USD1-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-LTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-SOL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-EUR-ADA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ETH-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-BRL-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-BRL-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-BRL-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-BRL-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDE-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-USDE-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-XRP-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-XRP-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-XRP-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-XRP-USDE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-XRP-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-XRP-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-TRX-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-LTC-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-LTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ADA-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] BTC-ADA-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-ADA-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BTC-BCH-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] TON-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TON-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-USDT-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] TRUMP-USD1-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] TRX-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] TRX-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] DOT-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] DOT-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDE-BTC-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDE-BTC-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDE-XRP-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDE-XRP-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDE-USDT-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDE-USDT-XRP excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] WBTC-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] WBTC-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USD1-USDT-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-USDT-TRUMP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-USDT-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-USDT-PI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-USDT-XRP excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-BTC-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-BTC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-BTC-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-ETH-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-ETH-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-ETH-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-XRP-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-XRP-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-XRP-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-TRUMP-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USD1-PI-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ATOM-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] ATOM-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PEPE-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PEPE-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PEPE-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PEPE-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] PEPE-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PEPE-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] MELANIA-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] MELANIA-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-EUR-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-EUR-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-LINK excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDC-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USD1-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USD1-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USD1-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-CRV-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BRL-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BRL-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BRL-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BRL-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-BTC-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-LINK excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-CRV excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-MX excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-USDT-XRP excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-MX-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-MX-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-MX-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-MX-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-MX-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-LINK-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-LINK-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-XRP-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-XRP-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-XRP-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-XRP-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ETH-XRP-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] XRP-USDC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-EUR-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] XRP-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] XRP-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-BTC-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-BTC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-BTC-USDE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USD1-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] XRP-USD1-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USD1-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDE-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDE-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] XRP-ETH-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-ETH-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-ETH-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-ETH-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-ETH-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] XRP-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDT-USD1 excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDT-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] XRP-USDT-USDE excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LINK-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] LINK-USDT-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LINK-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] LINK-USDC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LINK-ETH-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] LINK-ETH-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] SOL-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] SOL-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] SOL-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] SOL-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] BCH-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BCH-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] SUI-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SUI-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] SUI-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SUI-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] SUI-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SUI-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] SHIB-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] SHIB-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] KAS-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] KAS-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ULTIMA-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ULTIMA-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ULTIMA-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ULTIMA-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ULTIMA-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ULTIMA-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-PEPE-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-PEPE-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-RIO excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-MELANIA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-SUI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-TON excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-ADA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-ULTIMA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-WIF excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-KAS excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-TRUMP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-LTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-PEPE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-SOL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDC-TAO excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-PI-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-DOGE-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ETH-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ETH-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ETH-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ETH-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ETH-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-WIF-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-TON-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-SUI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-TAO excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-SOL excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-TRUMP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-ULTIMA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-ADA excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-MX excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-PI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-DOGE excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-PEPE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-USDT-XRP excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-XRP-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-XRP-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-XRP-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-XRP-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ULTIMA-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ULTIMA-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-SOL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-LTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-BTC-ADA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-MX-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-MX-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-MX-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-MX-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-TRUMP-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-TRUMP-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-MELANIA-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-SUI-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-SUI-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-RIO-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-TAO-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-TAO-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-LTC-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-LTC-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-SOL-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-SOL-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-SOL-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-KAS-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ADA-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ADA-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] EUR-ADA-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] A-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] A-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] WIF-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] WIF-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] AVAX-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] AVAX-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-DOT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-USDC excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-MX excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-SOL excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-ATOM excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-USDE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-XRP excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-TRX excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-ADA excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BTC-BCH excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USD1-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USD1-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USD1-XRP excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USD1-TRUMP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USD1-PI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-WBTC-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-AVAX-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-SUI-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-SUI-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-TAO-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-TAO-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-XLM-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-SOL-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-SOL-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-SOL-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-PEPE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-PI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-DOGE excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-XRP excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-ULTIMA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-MX excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-TRUMP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-SUI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-TAO excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-SOL excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-EUR-ADA excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-SHIB-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-BTC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-UNI excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-XRP excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-BRL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-SUI excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-AVAX excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-ADA excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-ULTIMA excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-TRUMP excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-LINK excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-XLM excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-SHIB excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-WBTC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-MX excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-PEPE excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-SOL excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-USDC-TAO excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BCH-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-TRUMP-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-TRUMP-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-TRUMP-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ULTIMA-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ULTIMA-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-LINK-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-LINK-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-CRV excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-MX excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-LINK excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-ETH-XRP excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-ADA-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-ADA-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ADA-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-DOT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-CRV-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BRL-MX excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BRL-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-BRL-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-BRL-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-TRX-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-MX-BRL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-MX-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-MX-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-MX-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-MX-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-PI-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-PI-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-ATOM-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDE-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-USDE-XRP excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-UNI-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-DOGE-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-PEPE-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-PEPE-USDC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-XRP-USDC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDT-XRP-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-XRP-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-XRP-USD1 excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-XRP-USDE excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDT-XRP-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-RIO-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-PEPE excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-WIF excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-TON excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-ULTIMA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-TRUMP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-MELANIA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-SUI excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-RIO excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-TAO excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-LTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-SOL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-KAS excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-EUR-ADA excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-USDT excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-A excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-MX excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-SOL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-BRL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-XRP excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-LTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BTC-ADA excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-MELANIA-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-UNI-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-XRP-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-XRP-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-XRP-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-XRP-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-BRL-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BRL-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-BRL-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-BRL-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-SUI-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-SUI-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-TON-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-AVAX-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-ADA-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-ADA-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-ADA-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-BTC excluded: AB=true BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-WBTC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-AVAX excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-SUI excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-TAO excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-XLM excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-SOL excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-SHIB excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-TRUMP excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-ULTIMA excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-LINK excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-ETH excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-ADA excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-BRL excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-MX excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-UNI excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-PEPE excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-USDT-XRP excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-LINK excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ETH-XRP excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ULTIMA-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-ULTIMA-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-WIF-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-KAS-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-TRUMP-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-TRUMP-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-LTC-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-LTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-LINK-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-LINK-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-XLM-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-SHIB-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-WBTC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-MX-BRL excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-MX-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-MX-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-MX-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-MX-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-A-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-PEPE-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-PEPE-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-SOL-BTC excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-SOL-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-SOL-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] USDC-TAO-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] USDC-TAO-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-USDC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-USDC-USDT excluded: AB=false BC=true CA=true
2025/07/31 19:23:15 [❌ FILTER] ADA-USDT-BTC excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-USDT-USDC excluded: AB=true BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-BTC-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] ADA-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-BTC-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-EUR-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] ADA-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] ADA-EUR-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-MX-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-MX-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-MX-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-MX-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDC-BTC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDC-USDT excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDC-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-ETH-USDC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-ETH-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-ETH-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-ETH-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-BTC-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-BTC-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-BTC-MX excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-BTC-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDT-BTC excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDT-USDC excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDT-ETH excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] BRL-USDT-MX excluded: AB=false BC=true CA=false
2025/07/31 19:23:15 [❌ FILTER] DOGE-EUR-USDT excluded: AB=false BC=false CA=true
2025/07/31 19:23:15 [❌ FILTER] DOGE-USDT-EUR excluded: AB=true BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PI-EUR-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PI-USD1-USDT excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PI-USDT-USD1 excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [❌ FILTER] PI-USDT-EUR excluded: AB=false BC=false CA=false
2025/07/31 19:23:15 [FILTER ✅] Осталось треугольников после фильтрации: 0 (пропущено: 612)
2025/07/31 19:23:15 [INIT] Loaded 0 triangles after filtering
2025/07/31 19:23:15 [INIT] total unique pairs after filtering: 0
