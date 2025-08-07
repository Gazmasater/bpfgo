
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


grep -o '^[A-Z0-9]\+' available_all_symbols.log | sort > all.txt
sort blocked_pairs.log | uniq > blocked.txt
comm -23 all.txt blocked.txt > allowed_ws_symbols.log


gaz358@gaz358-BOD-WXX9:~/myprog/crypt/cmd/cryptarb$ go run .
2025/08/07 22:18:10 📈 Profiler доступен на http://localhost:6060/debug/pprof/
2025/08/07 22:18:10 ✅ OKX: Подходящих пар: 755
2025/08/07 22:18:10 📊 Доступные пары (с инверсиями): 1216
2025/08/07 22:18:10 [TRIANGLE] Found 1356 triangles
2025/08/07 22:18:10 [INIT] Треугольников найдено: 1356
2025/08/07 22:18:10 [INIT] Составили индекс по парам: 542 ключей
2025/08/07 22:18:10 [INIT] Подписка на пар: 542 шт.
2025/08/07 22:18:11 📩 OKX подписка отправлена: [ETHWUSDC USDTFXS BETHUSDT PYTHUSDT NEARUSDC WOOEUR TRUMPUSDT EURMANA EURHBAR USDCBTC USDCFIL TONUSDC USDCLTC EURIMX YGGUSDC EURLDO USDCPEPE USDTSOL ETHUSDC EURLTC PIEUR USDT1INCH MOVEUSDT USDCCOMP FILUSDT]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [ETHWUSDC USDTFXS BETHUSDT PYTHUSDT NEARUSDC WOOEUR TRUMPUSDT EURMANA EURHBAR USDCBTC USDCFIL TONUSDC USDCLTC EURIMX YGGUSDC EURLDO USDCPEPE USDTSOL ETHUSDC EURLTC PIEUR USDT1INCH MOVEUSDT USDCCOMP FILUSDT]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [USDTDOGS EURBAL SOLBTC USDCOKT OKTUSDC EUR1INCH OKBUSDT COMPUSDC BCHUSDC PEPEUSDC USDTFIL WIFUSDT EURATOM USDTZENT EURTRX USDCLINK USDCAAVE USDTETC EURINJ SOLBRL USDCTURBO USDCTRX BRLUSDT USDCDOGS BTCLTC]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [HBAREUR 1INCHUSDT USDTPYTH EURUNI USDTALGO USDTEGLD USDTJUP PEPEBRL LUNAUSDC NOTUSDC USDCSOL USDCWIF USDCENS 1INCHUSDC CHZUSDT NEARUSDT EURSOL ENSUSDT NEIROUSDC ASTREUR USDTIMX EURWIF STXUSDC FLOWUSDT DOGEUSDC]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [USDTSUSHI USDTENS APEEUR ONDOUSDT USDCONDO CRVUSDC BATUSDT STRKUSDT EURBTC LDOEUR USDTBAL USDTXTZ MKRUSDC WIFUSDC DAIUSDT WBTCUSDT ATOMUSDT LTCBTC ICPEUR USDTEIGEN TONUSDT USDTRDNT USDTPEPE USDTZETA ADAEUR]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [XLMUSDC ASTRUSDT GRTUSDT USDTOKT ATOMEUR EURSAND FXSEUR USDCXLM MKREUR ETHUSDT LTCEUR USDCUSDT USDTLUNC ZENTUSDT USDTTURBO USDTBAT USDTJTO ZEUSUSDT AVAXUSDC SHIBUSDC CHZEUR EURAPT TURBOUSDC ADAUSDC USDCPYTH]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [USDTDOGS EURBAL SOLBTC USDCOKT OKTUSDC EUR1INCH OKBUSDT COMPUSDC BCHUSDC PEPEUSDC USDTFIL WIFUSDT EURATOM USDTZENT EURTRX USDCLINK USDCAAVE USDTETC EURINJ SOLBRL USDCTURBO USDCTRX BRLUSDT USDCDOGS BTCLTC]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [USDTSUSHI USDTENS APEEUR ONDOUSDT USDCONDO CRVUSDC BATUSDT STRKUSDT EURBTC LDOEUR USDTBAL USDTXTZ MKRUSDC WIFUSDC DAIUSDT WBTCUSDT ATOMUSDT LTCBTC ICPEUR USDTEIGEN TONUSDT USDTRDNT USDTPEPE USDTZETA ADAEUR]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [ARBUSDC USDTDOGE XRPEUR BALUSDT SHIBUSDT SANDUSDC USDCBLUR GRTEUR MANAUSDT FETEUR FXSUSDT USDTNOT ADAUSDT ETHFIUSDT USDCCATI EURPI BRLXRP LTCUSDC ONDOUSDC OKBBTC USDTSUI DOGEEUR USDTMOODENG USDCSUI USDTZEUS]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [HBAREUR 1INCHUSDT USDTPYTH EURUNI USDTALGO USDTEGLD USDTJUP PEPEBRL LUNAUSDC NOTUSDC USDCSOL USDCWIF USDCENS 1INCHUSDC CHZUSDT NEARUSDT EURSOL ENSUSDT NEIROUSDC ASTREUR USDTIMX EURWIF STXUSDC FLOWUSDT DOGEUSDC]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [XLMUSDC ASTRUSDT GRTUSDT USDTOKT ATOMEUR EURSAND FXSEUR USDCXLM MKREUR ETHUSDT LTCEUR USDCUSDT USDTLUNC ZENTUSDT USDTTURBO USDTBAT USDTJTO ZEUSUSDT AVAXUSDC SHIBUSDC CHZEUR EURAPT TURBOUSDC ADAUSDC USDCPYTH]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [ARBUSDC USDTDOGE XRPEUR BALUSDT SHIBUSDT SANDUSDC USDCBLUR GRTEUR MANAUSDT FETEUR FXSUSDT USDTNOT ADAUSDT ETHFIUSDT USDCCATI EURPI BRLXRP LTCUSDC ONDOUSDC OKBBTC USDTSUI DOGEEUR USDTMOODENG USDCSUI USDTZEUS]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [BTCETH APTEUR ENSUSDC XRPBRL EURFLR LTCUSDT MERLUSDT USDCUNI BONKUSDC EURAAVE USDCICP USDTSNX USDTMKR EURFLOW USDTYGG TIAUSDC USDCADA IMXEUR EURCHZ ETHSTETH USDCRDNT MKRUSDT USDTMOVE USDTPI USDCTON]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [BTCETH APTEUR ENSUSDC XRPBRL EURFLR LTCUSDT MERLUSDT USDCUNI BONKUSDC EURAAVE USDCICP USDTSNX USDTMKR EURFLOW USDTYGG TIAUSDC USDCADA IMXEUR EURCHZ ETHSTETH USDCRDNT MKRUSDT USDTMOVE USDTPI USDCTON]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [USDCATOM SNXEUR JUPUSDT USDCDAI AXSUSDT MORPHOUSDT EURDYDX LDOUSDC USDTAPT TRXUSDC EURSTX DOTEUR USDTDYDX MANAEUR USDTAEVO USDCBRL USDCGALA]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [USDCATOM SNXEUR JUPUSDT USDCDAI AXSUSDT MORPHOUSDT EURDYDX LDOUSDC USDTAPT TRXUSDC EURSTX DOTEUR USDTDYDX MANAEUR USDTAEVO USDCBRL USDCGALA]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [MORPHOUSDC JTOEUR LINKEUR USDCALGO USDCWBTC AGLDUSDC XTZEUR USDCZETA BTCBRL MINAUSDT FLREUR INJEUR STXEUR SANDUSDT EURBAT USDTDOT USDTFLR USDCZEUS USDTLINK EURUSDC WIFEUR MERLUSDC USDCETHW SNXUSDT CROUSDT]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [MORPHOUSDC JTOEUR LINKEUR USDCALGO USDCWBTC AGLDUSDC XTZEUR USDCZETA BTCBRL MINAUSDT FLREUR INJEUR STXEUR SANDUSDT EURBAT USDTDOT USDTFLR USDCZEUS USDTLINK EURUSDC WIFEUR MERLUSDC USDCETHW SNXUSDT CROUSDT]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [USDTUNI SUSHIEUR BRLETH USDCSAND USDTAPE USDTMINA DYDXEUR USDTSTRK ATHUSDT BRLSOL EURXLM RDNTUSDC ETCUSDT USDTARKM BTCWBTC XTZUSDT AAVEUSDC YGGEUR USDCNEAR XLMUSDT CRVUSDT MOVEUSDC USDCWLD EURDOGE AGLDUSDT]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [USDTUNI SUSHIEUR BRLETH USDCSAND USDTAPE USDTMINA DYDXEUR USDTSTRK ATHUSDT BRLSOL EURXLM RDNTUSDC ETCUSDT USDTARKM BTCWBTC XTZUSDT AAVEUSDC YGGEUR USDCNEAR XLMUSDT CRVUSDT MOVEUSDC USDCWLD EURDOGE AGLDUSDT]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [EURWOO ETHBTC USDCGRT XRPUSDT USDTOP EURAVAX EURXRP AXSEUR EURFXS ETHBRL ETHFIUSDC LUNAUSDT EURICP USDCCRO YGGUSDT USDTADA USDTETH USDTXRP USDCSHIB DOTUSDT ZEUSUSDC WOOUSDT PIUSDT EURTRUMP USDCYGG]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [EURWOO ETHBTC USDCGRT XRPUSDT USDTOP EURAVAX EURXRP AXSEUR EURFXS ETHBRL ETHFIUSDC LUNAUSDT EURICP USDCCRO YGGUSDT USDTADA USDTETH USDTXRP USDCSHIB DOTUSDT ZEUSUSDC WOOUSDT PIUSDT EURTRUMP USDCYGG]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [EURCRO USDCLUNA EURYGG EURSUI STXBTC USDCETH USDCAEVO USDTNEAR EURDOT USDCMORPHO USDCAGLD USDTEUR PYTHUSDC BCHBTC UNIEUR USDTTIA ICPUSDT USDCAPE LINKUSDC USDCDOT FLRUSDT EUROP USDTTRX USDTWLD USDTZK]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [EURCRO USDCLUNA EURYGG EURSUI STXBTC USDCETH USDCAEVO USDTNEAR EURDOT USDCMORPHO USDCAGLD USDTEUR PYTHUSDC BCHBTC UNIEUR USDTTIA ICPUSDT USDCAPE LINKUSDC USDCDOT FLRUSDT EUROP USDTTRX USDTWLD USDTZK]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [WBTCUSDC ALGOUSDT USDTATH HBARUSDC USDCLDO BLURUSDT DOGSUSDT LDOUSDT ARKMUSDC EURJTO USDTICP UNIUSDC TURBOUSDT DOGEUSDT CROEUR DOTUSDC USDTCATI USDCAPT EURMINA TRXEUR BTCSTX USDTUSDC USDTBONK USDTSHIB BRLTRUMP]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [WBTCUSDC ALGOUSDT USDTATH HBARUSDC USDCLDO BLURUSDT DOGSUSDT LDOUSDT ARKMUSDC EURJTO USDTICP UNIUSDC TURBOUSDT DOGEUSDT CROEUR DOTUSDC USDTCATI USDCAPT EURMINA TRXEUR BTCSTX USDTUSDC USDTBONK USDTSHIB BRLTRUMP]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [UNIUSDT ETHWUSDT USDTCRO EURSNX ICPUSDC USDTDAI ALGOEUR PIBRL USDTSTX USDTWBTC GALAUSDC BTCBCH TRUMPEUR SUIUSDT LUNCEUR SHIBEUR LINKUSDT USDCETHFI USDCNOT USDTFLOW CATIUSDC CRVEUR FLRUSDC RDNTUSDT EURALGO]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [UNIUSDT ETHWUSDT USDTCRO EURSNX ICPUSDC USDTDAI ALGOEUR PIBRL USDTSTX USDTWBTC GALAUSDC BTCBCH TRUMPEUR SUIUSDT LUNCEUR SHIBEUR LINKUSDT USDCETHFI USDCNOT USDTFLOW CATIUSDC CRVEUR FLRUSDC RDNTUSDT EURALGO]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [EURLINK ARBEUR USDCJTO USDCBCH USDCETC USDTLRC ARBUSDT EGLDEUR USDTXLM EURFET STRKUSDC USDTETHW GALAUSDT APTUSDT USDCFLR SUIEUR WLDUSDT EURAXS TRXUSDT USDCMERL ETHEUR USDCXRP ETHSOL WBTCBTC ZENTUSDC]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [EURLINK ARBEUR USDCJTO USDCBCH USDCETC USDTLRC ARBUSDT EGLDEUR USDTXLM EURFET STRKUSDC USDTETHW GALAUSDT APTUSDT USDCFLR SUIEUR WLDUSDT EURAXS TRXUSDT USDCMERL ETHEUR USDCXRP ETHSOL WBTCBTC ZENTUSDC]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [USDTWOO BETHETH USDTCOMP USDCDOGE USDTMANA USDTLDO USDTAAVE SANDEUR BTCUSDT USDTBTC BTCUSDC AAVEUSDT BLURUSDC ZROUSDC SUIUSDC EURGRT BONKUSDT PNUTUSDC USDTARB CHZUSDC EURUSDT USDTSAND USDTASTR CROUSDC ATHUSDC]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [USDTWOO BETHETH USDTCOMP USDCDOGE USDTMANA USDTLDO USDTAAVE SANDEUR BTCUSDT USDTBTC BTCUSDC AAVEUSDT BLURUSDC ZROUSDC SUIUSDC EURGRT BONKUSDT PNUTUSDC USDTARB CHZUSDC EURUSDT USDTSAND USDTASTR CROUSDC ATHUSDC]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [ZETAUSDC APEUSDT USDCOP GRTUSDC STETHETH OPUSDT EURLUNC USDTGRT 1INCHEUR BRLUSDC PNUTUSDT USDCAVAX ETHBETH SOLEUR PEPEUSDT USDTAGLD JTOUSDT USDTWIF BTCSOL USDCZK EURSHIB ARKMUSDT USDTBETH DYDXUSDC USDTLTC]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [ZETAUSDC APEUSDT USDCOP GRTUSDC STETHETH OPUSDT EURLUNC USDTGRT 1INCHEUR BRLUSDC PNUTUSDT USDCAVAX ETHBETH SOLEUR PEPEUSDT USDTAGLD JTOUSDT USDTWIF BTCSOL USDCZK EURSHIB ARKMUSDT USDTBETH DYDXUSDC USDTLTC]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [EURTON USDTPNUT USDTMORPHO OPUSDC MOODENGUSDC ZKUSDC EURCOMP USDTAXS ZETAUSDT DYDXUSDT ZKUSDT BTCEUR BRLBTC LUNCUSDT SOLUSDT OKBUSDC LRCUSDT FETUSDT USDCSTX STETHUSDT EGLDUSDT USDTOKB SOLETH NOTUSDT USDCMKR]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [EURTON USDTPNUT USDTMORPHO OPUSDC MOODENGUSDC ZKUSDC EURCOMP USDTAXS ZETAUSDT DYDXUSDT ZKUSDT BTCEUR BRLBTC LUNCUSDT SOLUSDT OKBUSDC LRCUSDT FETUSDT USDCSTX STETHUSDT EGLDUSDT USDTOKB SOLETH NOTUSDT USDCMKR]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [ATOMUSDC FLOWEUR JTOUSDC USDCTIA ETCUSDC USDCLRC USDTBRL JUPUSDC NEIROUSDT EURMKR USDCZENT EURSUSHI USDCEUR USDC1INCH USDTSTETH DAIUSDC USDTFET EUREGLD USDCMOODENG USDCATH USDTETHFI USDTMERL BCHUSDT APEUSDC BALEUR]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [ATOMUSDC FLOWEUR JTOUSDC USDCTIA ETCUSDC USDCLRC USDTBRL JUPUSDC NEIROUSDT EURMKR USDCZENT EURSUSHI USDCEUR USDC1INCH USDTSTETH DAIUSDC USDTFET EUREGLD USDCMOODENG USDCATH USDTETHFI USDTMERL BCHUSDT APEUSDC BALEUR]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [EIGENUSDT SOLUSDC EURETH USDCNEIRO USDTAVAX USDCCHZ USDTATOM ALGOUSDC DOGSUSDC TONEUR USDCEIGEN USDCSTRK USDTINJ USDCARB USDCBONK USDTBLUR EIGENUSDC OPEUR BATEUR USDCLUNC AVAXUSDT EURADA USDCCRV COMPUSDT OKTUSDT]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [EIGENUSDT SOLUSDC EURETH USDCNEIRO USDTAVAX USDCCHZ USDTATOM ALGOUSDC DOGSUSDC TONEUR USDCEIGEN USDCSTRK USDTINJ USDCARB USDCBONK USDTBLUR EIGENUSDC OPEUR BATEUR USDCLUNC AVAXUSDT EURADA USDCCRV COMPUSDT OKTUSDT]
2025/08/07 22:18:11 📩 OKX подписка отправлена: [AEVOUSDT ZROUSDT EURARB USDCPNUT SUSHIUSDT USDCARKM USDCMOVE CATIUSDT BTCOKB USDCHBAR EURAPE COMPEUR USDCOKB USDTNEIRO USDCJUP USDTCRV AAVEEUR USDTTRUMP EURXTZ USDTONDO USDTLUNA XRPUSDC USDTBCH EURCRV BRLPEPE]
2025/08/07 22:18:11 [WS][OKX] subscribed to channels: [AEVOUSDT ZROUSDT EURARB USDCPNUT SUSHIUSDT USDCARKM USDCMOVE CATIUSDT BTCOKB USDCHBAR EURAPE COMPEUR USDCOKB USDTNEIRO USDCJUP USDTCRV AAVEEUR USDTTRUMP EURXTZ USDTONDO USDTLUNA XRPUSDC USDTBCH EURCRV BRLPEPE]
2025/08/07 22:18:12 📩 OKX подписка отправлена: [USDCZRO LRCUSDC USDTCHZ XLMEUR USDTTON LUNCUSDC USDCDYDX USDTHBAR WLDUSDC AVAXEUR MINAEUR MOODENGUSDT TIAUSDT USDTZRO EURASTR APTUSDC FILUSDC HBARUSDT BRLPI IMXUSDT AEVOUSDC TRUMPBRL INJUSDT USDTGALA STXUSDT]
2025/08/07 22:18:12 [WS][OKX] subscribed to channels: [USDCZRO LRCUSDC USDTCHZ XLMEUR USDTTON LUNCUSDC USDCDYDX USDTHBAR WLDUSDC AVAXEUR MINAEUR MOODENGUSDT TIAUSDT USDTZRO EURASTR APTUSDC FILUSDC HBARUSDT BRLPI IMXUSDT AEVOUSDC TRUMPBRL INJUSDT USDTGALA STXUSDT]
^Csignal: interrupt
