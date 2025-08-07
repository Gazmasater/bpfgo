
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


BNTUSDT	step=0.1
NTXUSDT	step=0.001
ARUSDT	step=0.01
SNTUSDT	step=1
BAKEUSDT	step=0.001
IXSUSDT	step=0.01
HANDYUSDT	step=0.001
DFUSDT	step=1
BOSONUSDT	step=0.001
NFTUSDT	step=100
IQUSDT	step=0.001
RSRUSDT	step=5
QTUMBTC	step=0.01
SOLUSDT	step=0.01
XENUSDT	step=0.0001
GELUSDT	step=0.0001
JUSTICEUSDT	step=0.0001
FIOUSDT	step=0.001
TGT1USDT	step=0.0001
BONDUSDT	step=0.001
TRXUSDC	step=0.001
VELOUSDT	step=0.0001
GARIUSDT	step=0.01
LPTUSDT	step=0.001
ROGUSDT	step=0.01
DOTBTC	step=0.0001
LTCUSDC	step=0.0001
PKRUSDT	step=0.01
SYSUSDT	step=0.01
AVAXUSDT	step=0.0001
CAWUSDC	step=100000
CELRUSDT	step=0.001
NEXOUSDT	step=0.01
TAOUSDT	step=0.01
OMGUSDT	step=0.0001
MAVUSDT	step=0.01
FILUSDC	step=0.0001
WBTCUSDT	step=1e-05
AURAUSDT	step=0.01
BXXUSDT	step=0.0001
MANAUSDT	step=0.001
PSPUSDT	step=0.0001
CHRPUSDT	step=0.01
OOEUSDT	step=0.0001
ALGOUSDT	step=0.001
STRMUSDT	step=0.01
ONTUSDT	step=0.1
CROUSDT	step=1
WINGUSDT	step=0.0001
BLZUSDT	step=1
KARATUSDT	step=0.001
RVNUSDT	step=0.001
TRXUSDT	step=1
CHZUSDT	step=0.001
MAGICUSDT	step=0.1
ATOMUSDC	step=0.01
NULSUSDT	step=1
PROSUSDT	step=0.01
MLNUSDT	step=0.01
SINUSDT	step=0.0001
VRAUSDT	step=1
HAIUSDT	step=0.0001
TRAVAUSDT	step=0.001
ISPUSDT	step=0.0001
BNCUSDT	step=0.0001
ROCKIUSDT	step=0.01
WNCGUSDT	step=0.0001
SANTOSUSDT	step=0.01
CAWUSDT	step=100000
XECUSDT	step=0.001
XYOUSDT	step=0.001
SLPUSDT	step=0.0001
ETHWUSDT	step=0.0001
YAYUSDT	step=0.001
DOGEUSDT	step=1
KLVUSDT	step=0.0001
KP3RUSDT	step=1e-05
TLMUSDT	step=0.0001
FITFIUSDT	step=0.0001
TRU1USDT	step=0.0001
DYPUSDT	step=0.01
MYSTUSDT	step=0.01
ARPAUSDT	step=0.001
RPLUSDT	step=0.01
BLOKUSDT	step=0.001
ASTRUSDT	step=0.01
GOMININGUSDT	step=0.01
XRPUSDT	step=0.1
IDEXUSDT	step=1
LUNCUSDT	step=0.001
FLMUSDT	step=0.0001
ALEPHUSDT	step=5
HOSKYUSDT	step=1000
ATLASUSDT	step=0.0001
SANDUSDT	step=0.0001
XPRUSDT	step=0.001
PSGUSDT	step=0.0001
SUSHIUSDT	step=0.1
SXPUSDT	step=1
SPAUSDT	step=1
HNSUSDT	step=0.0001
AERGOUSDT	step=0.001
OGUSDT	step=0.0001
ZENUSDT	step=0.001
JASMYUSDC	step=0.0001
PCXUSDT	step=0.001
SKLUSDT	step=0.01
GSTUSDT	step=0.1
XCADUSDT	step=0.0001
CHESSUSDT	step=0.0001
CEREUSDT	step=0.001
SUP1USDT	step=0.0001
EVMOSUSDT	step=0.1
ELONUSDT	step=0.001
JAMUSDT	step=0.001
BUNNYUSDT	step=0.001
BONEUSDT	step=0.01
ALPINEUSDT	step=0.01
RUNEUSDT	step=1
SPELLUSDT	step=0.001
NKNUSDT	step=0.001
GENEUSDT	step=0.0001
XCNUSDT	step=0.001
BZZUSDT	step=0.0001
POLISUSDT	step=0.0001
ZRXUSDT	step=0.001
FUSEUSDT	step=0.01
CLVUSDT	step=0.0001
GMXUSDT	step=1e-05
IOSTUSDT	step=1
RADUSDT	step=0.001
HUAHUAUSDT	step=0.001
ARCUSDT	step=0.001
BATUSDT	step=0.001
MKRUSDT	step=0.0001
ENSUSDT	step=0.0001
BSTUSDT	step=0.001
INTERUSDT	step=0.01
TSUKAUSDT	step=0.001
GMEEUSDT	step=0.1
ATOMUSDT	step=0.001
TONUSDT	step=0.001
CBKUSDT	step=0.0001
ZCXUSDT	step=0.0001
REEFUSDT	step=0.001
ETCUSDT	step=0.1
DEPUSDT	step=0.0001
WEMIXUSDT	step=0.0001
NWCUSDT	step=0.0001
OXTETH	step=0.1
ILVUSDT	step=0.0001
STOSUSDT	step=0.1
FTTUSDT	step=0.0001
XLMUSDT	step=0.1
ORCAUSDT	step=0.0001
AAVEUSDT	step=0.0001
FXSUSDT	step=0.001
OPUSDT	step=0.01
POLSUSDT	step=0.01
HEARTUSDT	step=0.01
MLTUSDT	step=0.01
SOLOUSDT	step=0.01
CTCUSDT	step=0.1
CRVETH	step=0.001
STNDUSDT	step=0.0001
ALICEUSDT	step=0.0001
CVXUSDT	step=0.1
BARUSDT	step=0.01
TRVLUSDT	step=0.001
CAKEUSDT	step=0.001
CASUSDT	step=0.0001
LAZIOUSDT	step=0.01
NEARUSDT	step=0.0001
GRTUSDT	step=0.001
SAUCEUSDT	step=0.001
CHOUSDT	step=0.1
RBTUSDT	step=0.001
XENUSDC	step=0.001
OSMOUSDT	step=0.1
AZITUSDT	step=0.01
HOPRUSDT	step=0.0001
BEAMUSDT	step=0.001
KAVAUSDT	step=0.0001
STARUSDT	step=0.001
UMAUSDT	step=0.1
CENNZUSDT	step=5
REIUSDT	step=0.01
QTUMUSDT	step=0.1
1INCHUSDT	step=0.0002
WAVESUSDT	step=1e-05
NEOBTC	step=0.1
ADAUSDC	step=0.001
RSRETH	step=5
AURUSDT	step=0.0001
LEVERUSDT	step=0.0001
DOGEUSDC	step=0.001
STEEMUSDT	step=5
XVGUSDT	step=0.001
ALTUSDT	step=0.01
USDCUSDT	step=1
SOLBTC	step=1
JUNOUSDT	step=0.1
ONTBTC	step=0.01
CREOUSDT	step=0.001
DKSUSDT	step=0.001
NUMUSDT	step=0.01
DAOUSDT	step=0.1
FIDAUSDT	step=0.01
LIKEUSDT	step=0.0001
XCHUSDT	step=0.0001
ROUTEUSDT	step=0.1
SDNUSDT	step=0.0001
VICUSDT	step=0.0001
XAUTUSDT	step=0.001
QNTUSDT	step=0.01
COWUSDT	step=0.0001
CITYUSDT	step=0.01
AFCUSDT	step=0.01
KPCUSDT	step=0.01
MIRUSDT	step=0.01
PENDLEUSDT	step=0.0001
CELUSDT	step=0.01
LQTYUSDT	step=0.0001
FARMUSDT	step=0.001
SNFTUSDT	step=0.01
CRETAUSDT	step=0.001
BCHBTC	step=0.001
GTCUSDT	step=0.0001
MBLUSDT	step=0.001
PROPSUSDT	step=0.001
METAVUSDT	step=0.01
DIONEUSDT	step=0.01
BOBAUSDT	step=0.0001
XRDUSDT	step=0.0001
SFPUSDT	step=0.001
LOOKSUSDT	step=0.0001
FTTUSDC	step=0.0001
SCUSDT	step=0.0001
VINUUSDT	step=100000
SUKUUSDT	step=0.01
IXTUSDT	step=0.01
FOXUSDT	step=0.0001
ROSEUSDT	step=0.001
BURGERUSDT	step=0.002
SFINUSDT	step=1e-06
JOEUSDT	step=0.1
OOBITUSDT	step=0.01
LUNCUSDC	step=0.001
RSS3USDT	step=0.001
ABBCUSDT	step=0.001
OGNUSDT	step=0.01
EKTAUSDT	step=0.1
ZIGUSDT	step=0.001
VTHOUSDT	step=5
JSTUSDT	step=1
VOLTUSDT	step=10000
SHIBAUSDT	step=100000
QKCUSDT	step=0.001
OVRUSDT	step=0.01
RAYUSDC	step=0.0001
ELFUSDT	step=1
APXUSDT	step=0.001
BVGUSDT	step=0.01
LDOUSDT	step=0.0001
HYDRAUSDT	step=0.0001
NAKAUSDT	step=0.01
WOOUSDT	step=0.001
FLOKIUSDT	step=1
MXUSDT	step=0.01
CFXUSDT	step=0.0001
BTTUSDT	step=0.001
MBXUSDT	step=0.1
SUNUSDT	step=0.001
SNXETH	step=0.1
BOOUSDT	step=0.0001
BNBUSDC	step=0.0001
SAFEUSDT	step=0.0001
DASHUSDT	step=0.001
SEROUSDT	step=0.1
ORTUSDT	step=0.001
METALUSDT	step=0.1
RARIUSDT	step=0.0001
NMRETH	step=0.01
NOSUSDT	step=0.01
LTCBTC	step=0.01
HBARUSDT	step=0.01
LOOMUSDT	step=0.001
TLOSUSDT	step=0.0001
RITEUSDT	step=0.001
RBCUSDT	step=0.001
XMRUSDT	step=0.001
BALUSDT	step=0.0001
RAYUSDT	step=0.0001
ASSUSDT	step=0.0001
CNSUSDT	step=0.01
SWAPUSDT	step=1
WBTCUSDC	step=1e-05
FEARUSDT	step=0.0001
BDXUSDT	step=0.0001
CRVUSDT	step=0.01
BICOUSDT	step=0.1
ACSUSDT	step=0.001
FORTUSDT	step=0.01
SCRTUSDT	step=0.0001
DCUSDT	step=0.001
XXUSDT	step=0.01
APEUSDT	step=0.01
NYMUSDT	step=1e-06
CEEKUSDT	step=0.0001
ADAUSDT	step=0.1
CANTOUSDT	step=0.01
ACXUSDT	step=0.01
HIVEUSDT	step=0.001
ACMUSDT	step=0.1
EULUSDT	step=0.0001
DEXEUSDT	step=0.001
METISUSDT	step=1e-05
LINKUSDT	step=0.01
CULTDAOUSDT	step=0.0001
BIT1USDT	step=0.0001
BAXUSDT	step=1000
KASTAUSDT	step=0.001
ETHBTC	step=0.001
ADSUSDT	step=0.0001
MARSHUSDT	step=0.0001
SWEATUSDT	step=0.01
POLYDOGEUSDT	step=100000
SHIBUSDC	step=1e-06
TRXBTC	step=0.1
IOTXUSDT	step=0.001
TRADEUSDT	step=0.01
TOKENUSDT	step=0.0001
HFTUSDT	step=0.01
DCRUSDT	step=0.001
STEPNUSDT	step=0.001
MINAUSDT	step=0.0001
SYNUSDT	step=0.001
POLCUSDT	step=0.001
WAVESUSDC	step=1e-06
USTCUSDT	step=0.001
TKOUSDT	step=0.01
BSWUSDT	step=0.0001
PIGUSDT	step=1000
SGTUSDT	step=0.1
INVUSDT	step=0.001
ICPUSDT	step=0.0001
ETHUSDC	step=1e-06
GLQUSDT	step=0.0001
UNIETH	step=0.001
SRMUSDT	step=0.0001
NCTUSDT	step=0.001
WNKUSDT	step=0.001
FIROUSDT	step=0.001
YGGUSDT	step=0.0001
ZILUSDT	step=1
AGLDUSDT	step=0.0001
AVAXUSDC	step=0.0001
SMARSUSDT	step=1e+09
BCHUSDT	step=0.001
YFIUSDT	step=0.0001
SENSOUSDT	step=0.001
WNXMUSDT	step=0.1
PEOPLEUSDT	step=0.0001
AAVEUSDC	step=1e-06
BIOFIUSDT	step=0.0001
PORUSDT	step=0.1
SGBUSDT	step=0.0001
XRPAYNETUSDT	step=0.001
DOTUSDT	step=0.001
XYMUSDT	step=0.001
UMBUSDT	step=0.001
KSMUSDT	step=0.001
MASKUSDT	step=0.001
BCHUSDC	step=0.0001
RAREUSDT	step=0.0001
KOMUSDT	step=0.001
ACHUSDT	step=0.001
CCDUSDT	step=0.0001
APTUSDT	step=0.0001
DODOUSDT	step=0.001
FORTHUSDT	step=0.0001
COREUSDT	step=0.01
CWEBUSDT	step=0.0001
PNGUSDT	step=0.01
SOLSUSDT	step=0.001
AMEUSDT	step=0.0001
XTZUSDT	step=0.1
DYDXUSDT	step=0.0001
GODSUSDT	step=0.0001
BELUSDT	step=0.0001
ONEUSDT	step=0.001
PHBUSDT	step=0.01
ANKRBTC	step=0.01
DEXEBTC	step=0.1
HOTUSDT	step=0.0001
CPHUSDT	step=0.001
ISKUSDT	step=0.001
GHSTUSDT	step=0.001
CSPRUSDT	step=0.0001
UNFIUSDT	step=0.001
XCVUSDT	step=0.1
LATUSDT	step=0.001
STETHUSDT	step=1e-05
ARGUSDT	step=0.1
XVSUSDT	step=0.001
PHAUSDT	step=0.01
TRIBEUSDT	step=0.001
QUICKUSDT	step=0.0001
AVAUSDT	step=0.1
TWTUSDT	step=1
ATOMBTC	step=0.001
FURYUSDT	step=0.001
GLMRUSDT	step=0.0001
ADABTC	step=0.001
XLMBTC	step=0.1
BNBUSDT	step=0.001
RIFUSDT	step=0.001
ULTIUSDT	step=1000
C98USDT	step=0.0001
WILDUSDT	step=1e-05
TUSDT	step=0.0001
YFIIUSDT	step=0.0001
ANKRUSDT	step=0.001
CLYUSDT	step=0.01
MEMEUSDT	step=0.0001
LBCUSDT	step=0.001
STORJUSDT	step=0.5
MMUIUSDT	step=0.01
SFUNDUSDT	step=0.1
OPULUSDT	step=0.01
CTKUSDT	step=0.0001
KISHUUSDT	step=100
PERPUSDT	step=0.01
ENSUSDC	step=0.0001
AIUSDT	step=0.001
VETUSDT	step=10
DASHBTC	step=0.01
OCTUSDT	step=0.0001
COMBOUSDT	step=0.001
MXUSDC	step=1e-05
PSTAKEUSDT	step=0.0001
EGCUSDT	step=0.02
USDPUSDT	step=0.0001
HIUSDT	step=0.01
AZEROUSDT	step=0.01
GALAUSDT	step=0.001
RACAUSDT	step=10
SSVUSDT	step=0.0001
WAXLUSDT	step=0.01
BENQIUSDT	step=0.0001
RENUSDT	step=1
BFTUSDT	step=0.01
SOLXUSDT	step=0.01
KASUSDT	step=0.001
PONDUSDT	step=0.001
METAHEROUSDT	step=0.01
JASMYUSDT	step=0.0001
SOLUSDC	step=1e-06
AKITAUSDT	step=1000
STARLUSDT	step=0.001
OKTUSDT	step=0.0001
NEARUSDC	step=1e-06
FRTSUSDT	step=0.01
DEGOUSDT	step=0.01
PORTOUSDT	step=0.01
MTRGUSDT	step=0.1
OMGBTC	step=1e-06
JEWELUSDT	step=0.1
FLOWUSDT	step=0.0001
MINAUSDC	step=0.001
BSVUSDT	step=0.001
AZEROUSDC	step=0.01
PITUSDT	step=10
PIXELUSDT	step=0.001
OPUSDC	step=0.001
ARKUSDT	step=0.01
BABYDOGEUSDT	step=10000
OXTUSDT	step=0.0001
NMRUSDT	step=0.001
ETHFUSDT	step=0.0001
ALGOUSDC	step=0.0001
PUSHUSDT	step=0.0001
SLIMUSDT	step=0.001
CLSUSDT	step=0.1
PRTGUSDT	step=0.1
SNXUSDT	step=0.01
COTIUSDT	step=0.01
MOONUSDT	step=0.001
XRPBTC	step=10
OMNOMUSDT	step=1000
SHIBUSDT	step=10000
XRPUSDC	step=0.0001
COMPUSDT	step=0.001
ONGUSDT	step=0.001
TORUMUSDT	step=0.1
STGUSDT	step=0.1
SAFFRONFIUSDT	step=0.001
API3USDT	step=0.001
OKBUSDT	step=0.0001
DEBUSDT	step=0.01
POKTUSDT	step=0.1
AXSUSDT	step=0.0001
HAPIUSDT	step=0.0001
XEPUSDT	step=0.0001
UNIUSDC	step=0.001
TYTUSDT	step=100
TARAUSDT	step=0.001
FETUSDT	step=0.001
BTCUSDC	step=1e-06
UFOUSDT	step=1
DAIUSDT	step=0.5
HAMUSDT	step=1e+06
MOVRUSDT	step=0.0001
BADGERUSDT	step=0.0001
WITUSDT	step=0.001
IMXUSDT	step=0.0001
KCSUSDT	step=0.0001
KARUSDT	step=0.0001
HOOKUSDT	step=0.01
ACAUSDT	step=0.0001
ELUSDT	step=0.001
WAXPUSDT	step=0.001
KOKOUSDT	step=1e-05
LOOPUSDT	step=0.001
TRUUSDT	step=0.0001
PIBUSDT	step=0.0001
FILUSDT	step=0.0001
HIGHUSDT	step=0.001
CKBUSDT	step=0.001
UNIUSDT	step=0.001
ZECUSDT	step=0.001
AUCTIONUSDT	step=0.001
WBTUSDT	step=0.01
XAVAUSDT	step=0.0001
TAIUSDT	step=0.001
SAMOUSDT	step=0.0001
CTXCUSDT	step=0.001
TPTUSDT	step=0.001
TUSDUSDT	step=0.0001
GATUSDT	step=1e-05
PAXGUSDT	step=1e-06
MATHUSDT	step=0.0001
APEUSDC	step=1e-06
KAIAUSDC	step=1e-05
KNCUSDT	step=0.001
NEOUSDT	step=0.1
EGLDUSDT	step=0.0001
WINUSDT	step=0.001
LTCUSDT	step=0.001
XIUSDT	step=0.01
ARETH	step=0.1
LRCUSDT	step=1
MBOXUSDT	step=0.0001
BLUEMOVEUSDT	step=0.0001
TORNUSDT	step=0.001
ENJUSDT	step=5
AURORAUSDT	step=0.001
LUNAUSDT	step=0.01 



XENUSDT
USDCFIL
USDCSOL
USDTUSDC
NEARUSDC
USDTWAVES
USDTAAVE
USDTAZERO
SOLBTC
USDCAAVE
UNIUSDT
WBTCUSDC
FILUSDC
LTCUSDT
USDTDOGE
ADABTC
USDCALGO
USDTATOM
ENSUSDT
TRXBTC
UNIUSDC
USDTOP
BTCBCH
AVAXUSDC
USDCNEAR
ATOMUSDC
JASMYUSDC
WBTCUSDT
USDCENS
FILUSDT
SHIBUSDT
CAWUSDC
LUNCUSDC
USDCFTT
USDCRAY
UNIETH
USDCBCH
USDTXEN
USDCAZERO
USDTCAW
USDCDOGE
BCHUSDC
MINAUSDC
USDTJASMY
USDTTRX
USDCMX
USDTLTC
BTCTRX
USDCBNB
OPUSDT
ALGOUSDC
USDCAPE
BTCETH
WAVESUSDC
USDCLTC
USDCBTC
BTCLTC
USDTAVAX
SOLUSDT
AAVEUSDC
USDCXEN
USDTADA
AZEROUSDC
ATOMBTC
USDCUSDT
BCHUSDT
FTTUSDT
BNBUSDC
USDTWBTC
USDTENS
USDCATOM
DOGEUSDC
ALGOUSDT
LTCBTC
USDCETH
ENSUSDC
USDTRAY
WAVESUSDT
USDCXRP
LUNCUSDT
USDTBNB
BTCXRP
USDTFTT
USDTSHIB
LTCUSDC
AZEROUSDT
BTCUSDC
USDCMINA
AAVEUSDT
USDTALGO
USDCWBTC
USDTMINA
ETHBTC
SOLUSDC
USDTUNI
OPUSDC
USDCLUNC
USDCCAW
TRXUSDC
USDCTRX
XRPBTC
USDCAVAX
BTCADA
JASMYUSDT
APEUSDC
USDTMX
USDTSOL
TRXUSDT
XENUSDC
ATOMUSDT
XRPUSDT
MINAUSDT
USDCJASMY
USDTFIL
SHIBUSDC
FTTUSDC
USDTBCH
ETHUNI
USDCADA
DOGEUSDT
NEARUSDT
BCHBTC
USDCOP
ETHUSDC
APEUSDT
MXUSDT
USDTNEAR
XRPUSDC
RAYUSDT
ADAUSDT
USDCUNI
USDTLUNC
MXUSDC
BTCSOL
CAWUSDT
BTCATOM
BNBUSDT
USDTXRP
AVAXUSDT
USDTAPE
RAYUSDC
ADAUSDC
USDCSHIB
USDCWAVES
USDTADA
USDCUSDT
BNBUSDC
USDTFTT
USDCLUNC
TRXUSDC
XRPBTC
BTCADA
APEUSDC
USDTSOL
XENUSDC
XRPUSDT
USDCJASMY
SHIBUSDC
USDTBCH
USDCADA
NEARUSDT
BCHBTC
USDCOP
ETHUSDC
OPUSDT
BTCETH
USDCBTC
SOLUSDT
ENSUSDC
USDCXRP
USDCWBTC
USDTUNI
USDCCAW
USDCTRX
USDCAVAX
JASMYUSDT
USDTMX
TRXUSDT
ATOMUSDT
MINAUSDT
USDTFIL
FTTUSDC
ETHUNI
DOGEUSDT
APEUSDT
MXUSDT
USDTNEAR
XRPUSDC
RAYUSDT
ADAUSDT
USDCUNI
USDTLUNC
MXUSDC
BTCSOL
CAWUSDT
BTCATOM
BNBUSDT
USDTXRP
AVAXUSDT
USDTAPE
RAYUSDC
ADAUSDC
USDCSHIB
USDCWAVES
CAWUSDC
USDCFTT
USDCBCH
USDTXEN
USDCAZERO
USDTCAW
BCHUSDC
USDTJASMY
USDCMX
BTCTRX
ALGOUSDC
ATOMBTC
USDCATOM
LTCBTC
BTCXRP
LTCUSDC
USDCMINA
USDTMINA
SOLUSDC
OPUSDC
XENUSDT
USDCFIL
USDCSOL
USDTUSDC
NEARUSDC
USDTWAVES
USDTAAVE
USDTAZERO
SOLBTC
USDCAAVE
UNIUSDT
WBTCUSDC
FILUSDC
LTCUSDT
USDTDOGE
ADABTC
USDCALGO
USDTATOM
ENSUSDT
TRXBTC
UNIUSDC
BTCBCH
AVAXUSDC
ATOMUSDC
WBTCUSDT
WAVESUSDC
BTCLTC
AAVEUSDC
AZEROUSDC
BCHUSDT
USDTWBTC
DOGEUSDC
USDCETH
WAVESUSDT
LUNCUSDT
USDTBNB
USDTSHIB
AZEROUSDT
AAVEUSDT
ETHBTC
USDCRAY
FTTUSDT
USDTENS
ALGOUSDT
USDTOP
USDCNEAR
JASMYUSDC
USDCENS
FILUSDT
SHIBUSDT
LUNCUSDC
UNIETH
USDCDOGE
MINAUSDC
USDTTRX
USDTLTC
USDCBNB
USDCAPE
USDCLTC
USDTAVAX
USDCXEN
USDTRAY
BTCUSDC
USDTALGO
APEUSDT
MXUSDT
USDTNEAR
XRPUSDC
RAYUSDT
ADAUSDT
USDCUNI
USDTLUNC
MXUSDC
BTCSOL
CAWUSDT
BTCATOM
BNBUSDT
USDTXRP
AVAXUSDT
USDTAPE
RAYUSDC
ADAUSDC
USDCSHIB
USDCWAVES










