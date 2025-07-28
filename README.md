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


package filesystem

import (
    "cryptarb/internal/domain/triangle"
    "encoding/json"
    "fmt"
    "io"
    "log"
    "net/http"
    "sort"
)

// LoadTriangles загружает все возможные треугольники с биржи MEXC
func LoadTriangles(_ string) ([]triangle.Triangle, error) {
    // 1) Получаем exchangeInfo
    resp, err := http.Get("https://api.mexc.com/api/v3/exchangeInfo")
    if err != nil {
        return nil, fmt.Errorf("fetch exchangeInfo: %w", err)
    }
    defer resp.Body.Close()

    // 2) Читаем тело
    body, err := io.ReadAll(resp.Body)
    if err != nil {
        return nil, fmt.Errorf("read exchangeInfo: %w", err)
    }

    // 3) Разбираем JSON: сразу top-level "symbols"
    type symbolInfo struct {
        Base  string `json:"baseAsset"`
        Quote string `json:"quoteAsset"`
    }
    var payload struct {
        Symbols []symbolInfo `json:"symbols"`
    }
    if err := json.Unmarshal(body, &payload); err != nil {
        return nil, fmt.Errorf("unmarshal exchangeInfo: %w", err)
    }

    symbols := payload.Symbols
    log.Printf("[DEBUG] fetched %d symbols", len(symbols))

    // 4) Строим двунаправленный граф смежностей
    //    edges[A][B] = true и edges[B][A] = true
    edges := make(map[string]map[string]bool, len(symbols)*2)
    assets := make(map[string]struct{}, len(symbols)*2)
    for _, s := range symbols {
        if s.Base == "" || s.Quote == "" {
            continue
        }
        // инициализируем map-у по необходимости
        if edges[s.Base] == nil {
            edges[s.Base] = make(map[string]bool)
        }
        if edges[s.Quote] == nil {
            edges[s.Quote] = make(map[string]bool)
        }
        // прямая и обратная «рёбра»
        edges[s.Base][s.Quote] = true
        edges[s.Quote][s.Base] = true

        assets[s.Base] = struct{}{}
        assets[s.Quote] = struct{}{}
    }

    // 5) Собираем список всех уникальных активов
    var toks []string
    for a := range assets {
        toks = append(toks, a)
    }
    log.Printf("[INFO] Total unique assets: %d", len(toks))

    // 6) Ищем все 3-циклы A → B → C → A
    var tris []triangle.Triangle
    seen := make(map[[3]string]struct{})

    for a := range edges {
        for b := range edges[a] {
            for c := range edges[b] {
                // если есть назад в A
                if edges[c][a] {
                    // дедупликация: сортируем имена
                    keyArr := []string{a, b, c}
                    sort.Strings(keyArr)
                    key := [3]string{keyArr[0], keyArr[1], keyArr[2]}
                    if _, ok := seen[key]; !ok {
                        tris = append(tris, triangle.Triangle{A: a, B: b, C: c})
                        seen[key] = struct{}{}
                    }
                }
            }
        }
    }

    log.Printf("[INFO] Loaded %d triangles", len(tris))
    return tris, nil
}


2025/07/28 01:13:04 [INIT] total unique pairs after filtering: 270
2025/07/28 01:13:04 [INIT] subscribing on: [LINGOUSDT MXBRL KILOUSDT PEPEEUR DASHUSDT DOTBTC WAVESUSDT BTCUSDT ARUSDT RIOUSDC DSYNCUSDC USDCEUR BTCUSDC USD1USDT BUTTHOLEUSDT NMRUSDT WBTCUSDT FHEUSDC MELANIAUSD1 RWAUSDC ELDEUSDC BROCCOLIF3BUSDC ANKRUSDT ENAUSDE SUSDC PIEUR SPKUSDT BLUMUSDT LUNCUSDT DOGEEUR PLUMEUSDC ABTC ETHEUR WIFEUR MELANIAUSDC ICPUSDC UNIUSDC AVAXUSDT XRPUSDC MXBTC ONTBTC XMRUSDC PRAIUSDC ALCHUSDC SOLBTC SUIUSDT BANKUSDC KAIAUSDT QUBICUSDT USDCBRL LOTUSDT RFCUSDT ZBCNUSDC STOUSDT TAOUSDT USELESSUSDC ALGOUSDT XLMUSDT KEKIUSUSDC PNUTUSDT SUIUSDC FETUSDC HUSDT AERGOUSDT SOLUSDT ATOMBTC BDXUSDT NEOBTC FLYUSDC TONUSDC OMIUSDC BKNUSDT AVAXUSDC BUSDT TONEUR HBARUSDT NEWTUSDT SKATEUSDT ERAUSDC ZORAUSDT BTCUSD1 PUMPUSDC JASMYUSDT LAUSDT EURUSDT SHIBUSDT ADAUSDC REDUSDT RDOUSDC USDCUSDT TURBOUSDC KERNELUSDC BERAUSDT PUMPBTCUSDT BOOMUSDC BCHUSDT XRPEUR FARTCOINUSDC ARBUSDT RBNTUSDC ETHUSDC ULTIMAEUR LAUNCHCOINUSDC ETHUSD1 ENAUSDC TRUMPUSDT XCNUSDT LIBERTYUSD1 BTCEUR BNBUSDC VINEUSDT WCTUSDC AAVEUSDC CGPTUSDC MXEUR ULTIMAUSDC HYPEUSDT APEUSDT SOONUSDT WIFUSDC ACHUSDT XDCUSDC SNSUSDC EGL1USDT SAHARAUSDT CRVETH MUBARAKUSDC ETHBRL TRUMPEUR MELANIAEUR BABYUSDT ULTIMAUSDT FTTUSDT GHIBLIUSDC ETHBTC MNTUSDC INJUSDT LINKUSDT BTCBRL ETHUSDT SUIEUR KASUSDC FILUSDC NXPCUSDT DOODUSDC BTCUSDE RSRUSDT SUPRAUSDC BANKUSD1 B2USDT GRIFFAINUSDT XRPBTC BDXNUSDC 1DOLLARUSDT TAGUSD1 XRPUSD1 KASUSD1 TRXBTC SOSOUSDT EPTUSDT XRPUSDE MXETH BMTUSDT GUNUSDC TRUMPUSDC LTCUSDC MILKUSDT NPCUSDC MOREUSDT CUSDT TRUMPUSD1 ONDOUSDT LINKUSDC NODEUSDC LTCBTC XLMUSDC NAKAUSDC FLOCKUSDC RENDERUSDT PARTIUSDC XENUSDT SENUSDT DOLOUSDT RIOEUR SPXUSDC VIRTUALUSDT ENSUSDT ADAUSDT DOTUSDT RAIUSDT KAITOUSDT TIAUSDT SHIBUSDC TAOEUR MINAUSDT CRVUSDT MYXUSDT OXTETH APTUSDC BRLUSDT CAWUSDC LINKETH LTCEUR OPUSDT WBTCUSDC SERAPHUSDC MXUSDC HUMAUSDC DEXEUSDT USDRUSDT RAYUSDT OBTUSDC AIXBTUSDC AUSDC POPCATUSDC NILUSDT PEAQUSDT AZEROUSDT SXTUSDC PIUSD1 QTUMBTC SOLEUR ADABTC TRXUSDT HOMEUSDT VELVETUSDT RAREUSDT MXUSDT PEPEUSDC BEEUSDT BOMBUSDT KASUSDE WHITEUSDT SOLUSDC OMGUSDT NEARUSDT PIUSDT PENGUUSDT PROMPTUSDT HYPERUSDC SIGNUSDC TAOUSDC ATOMUSDT OBOLUSDT AI16ZUSDT SHMUSDT ASRRUSDT VIDTUSDC INITUSDC USDEUSDT WALUSDT UNIUSDT CORNUSDC KASEUR MOONPIGUSDT SNXUSDT ESUSDT BCHBTC DOGEUSDT ADAEUR AIOTUSDT BABYDOGEUSDT SOPHUSDT XRPETH IDOLUSDT SGCUSDC PEPEUSDT XRPUSDT ETCBTC AGTUSDT]
2025/07/28 01:13:04 [WS] subscribing chunk 0:25: [LINGOUSDT MXBRL KILOUSDT PEPEEUR DASHUSDT DOTBTC WAVESUSDT BTCUSDT ARUSDT RIOUSDC DSYNCUSDC USDCEUR BTCUSDC USD1USDT BUTTHOLEUSDT NMRUSDT WBTCUSDT FHEUSDC MELANIAUSD1 RWAUSDC ELDEUSDC BROCCOLIF3BUSDC ANKRUSDT ENAUSDE SUSDC]
2025/07/28 01:13:04 [WS] subscribing chunk 25:50: [PIEUR SPKUSDT BLUMUSDT LUNCUSDT DOGEEUR PLUMEUSDC ABTC ETHEUR WIFEUR MELANIAUSDC ICPUSDC UNIUSDC AVAXUSDT XRPUSDC MXBTC ONTBTC XMRUSDC PRAIUSDC ALCHUSDC SOLBTC SUIUSDT BANKUSDC KAIAUSDT QUBICUSDT USDCBRL]
2025/07/28 01:13:04 [WS] subscribing chunk 50:75: [LOTUSDT RFCUSDT ZBCNUSDC STOUSDT TAOUSDT USELESSUSDC ALGOUSDT XLMUSDT KEKIUSUSDC PNUTUSDT SUIUSDC FETUSDC HUSDT AERGOUSDT SOLUSDT ATOMBTC BDXUSDT NEOBTC FLYUSDC TONUSDC OMIUSDC BKNUSDT AVAXUSDC BUSDT TONEUR]
2025/07/28 01:13:04 [WS] subscribing chunk 75:100: [HBARUSDT NEWTUSDT SKATEUSDT ERAUSDC ZORAUSDT BTCUSD1 PUMPUSDC JASMYUSDT LAUSDT EURUSDT SHIBUSDT ADAUSDC REDUSDT RDOUSDC USDCUSDT TURBOUSDC KERNELUSDC BERAUSDT PUMPBTCUSDT BOOMUSDC BCHUSDT XRPEUR FARTCOINUSDC ARBUSDT RBNTUSDC]
2025/07/28 01:13:04 [WS] subscribing chunk 100:125: [ETHUSDC ULTIMAEUR LAUNCHCOINUSDC ETHUSD1 ENAUSDC TRUMPUSDT XCNUSDT LIBERTYUSD1 BTCEUR BNBUSDC VINEUSDT WCTUSDC AAVEUSDC CGPTUSDC MXEUR ULTIMAUSDC HYPEUSDT APEUSDT SOONUSDT WIFUSDC ACHUSDT XDCUSDC SNSUSDC EGL1USDT SAHARAUSDT]
2025/07/28 01:13:04 [WS] subscribing chunk 125:150: [CRVETH MUBARAKUSDC ETHBRL TRUMPEUR MELANIAEUR BABYUSDT ULTIMAUSDT FTTUSDT GHIBLIUSDC ETHBTC MNTUSDC INJUSDT LINKUSDT BTCBRL ETHUSDT SUIEUR KASUSDC FILUSDC NXPCUSDT DOODUSDC BTCUSDE RSRUSDT SUPRAUSDC BANKUSD1 B2USDT]
2025/07/28 01:13:04 [WS] subscribing chunk 150:175: [GRIFFAINUSDT XRPBTC BDXNUSDC 1DOLLARUSDT TAGUSD1 XRPUSD1 KASUSD1 TRXBTC SOSOUSDT EPTUSDT XRPUSDE MXETH BMTUSDT GUNUSDC TRUMPUSDC LTCUSDC MILKUSDT NPCUSDC MOREUSDT CUSDT TRUMPUSD1 ONDOUSDT LINKUSDC NODEUSDC LTCBTC]
2025/07/28 01:13:04 [WS] subscribing chunk 175:200: [XLMUSDC NAKAUSDC FLOCKUSDC RENDERUSDT PARTIUSDC XENUSDT SENUSDT DOLOUSDT RIOEUR SPXUSDC VIRTUALUSDT ENSUSDT ADAUSDT DOTUSDT RAIUSDT KAITOUSDT TIAUSDT SHIBUSDC TAOEUR MINAUSDT CRVUSDT MYXUSDT OXTETH APTUSDC BRLUSDT]
2025/07/28 01:13:04 [WS] subscribing chunk 200:225: [CAWUSDC LINKETH LTCEUR OPUSDT WBTCUSDC SERAPHUSDC MXUSDC HUMAUSDC DEXEUSDT USDRUSDT RAYUSDT OBTUSDC AIXBTUSDC AUSDC POPCATUSDC NILUSDT PEAQUSDT AZEROUSDT SXTUSDC PIUSD1 QTUMBTC SOLEUR ADABTC TRXUSDT HOMEUSDT]
2025/07/28 01:13:04 [WS] subscribing chunk 225:250: [VELVETUSDT RAREUSDT MXUSDT PEPEUSDC BEEUSDT BOMBUSDT KASUSDE WHITEUSDT SOLUSDC OMGUSDT NEARUSDT PIUSDT PENGUUSDT PROMPTUSDT HYPERUSDC SIGNUSDC TAOUSDC ATOMUSDT OBOLUSDT AI16ZUSDT SHMUSDT ASRRUSDT VIDTUSDC INITUSDC USDEUSDT]
2025/07/28 01:13:04 [WS] subscribing chunk 250:270: [WALUSDT UNIUSDT CORNUSDC KASEUR MOONPIGUSDT SNXUSDT ESUSDT BCHBTC DOGEUSDT ADAEUR AIOTUSDT BABYDOGEUSDT SOPHUSDT XRPETH IDOLUSDT SGCUSDC PEPEUSDT XRPUSDT ETCBTC AGTUSDT]

___________________________________________________________________________________________


func LoadTriangles(_ string) ([]triangle.Triangle, error) {
	// Все 270 пар
	subPairs := []string{
		"LINGOUSDT", "MXBRL", "KILOUSDT", "PEPEEUR", "DASHUSDT", "DOTBTC", "WAVESUSDT", "BTCUSDT", "ARUSDT", "RIOUSDC", "DSYNCUSDC", "USDCEUR", "BTCUSDC", "USD1USDT", "BUTTHOLEUSDT", "NMRUSDT", "WBTCUSDT", "FHEUSDC", "MELANIAUSD1", "RWAUSDC", "ELDEUSDC", "BROCCOLIF3BUSDC", "ANKRUSDT", "ENAUSDE", "SUSDC", "PIEUR", "SPKUSDT", "BLUMUSDT", "LUNCUSDT", "DOGEEUR", "PLUMEUSDC", "ABTC", "ETHEUR", "WIFEUR", "MELANIAUSDC", "ICPUSDC", "UNIUSDC", "AVAXUSDT", "XRPUSDC", "MXBTC", "ONTBTC", "XMRUSDC", "PRAIUSDC", "ALCHUSDC", "SOLBTC", "SUIUSDT", "BANKUSDC", "KAIAUSDT", "QUBICUSDT", "USDCBRL", "LOTUSDT", "RFCUSDT", "ZBCNUSDC", "STOUSDT", "TAOUSDT", "USELESSUSDC", "ALGOUSDT", "XLMUSDT", "KEKIUSUSDC", "PNUTUSDT", "SUIUSDC", "FETUSDC", "HUSDT", "AERGOUSDT", "SOLUSDT", "ATOMBTC", "BDXUSDT", "NEOBTC", "FLYUSDC", "TONUSDC", "OMIUSDC", "BKNUSDT", "AVAXUSDC", "BUSDT", "TONEUR", "HBARUSDT", "NEWTUSDT", "SKATEUSDT", "ERAUSDC", "ZORAUSDT", "BTCUSD1", "PUMPUSDC", "JASMYUSDT", "LAUSDT", "EURUSDT", "SHIBUSDT", "ADAUSDC", "REDUSDT", "RDOUSDC", "USDCUSDT", "TURBOUSDC", "KERNELUSDC", "BERAUSDT", "PUMPBTCUSDT", "BOOMUSDC", "BCHUSDT", "XRPEUR", "FARTCOINUSDC", "ARBUSDT", "RBNTUSDC", "ETHUSDC", "ULTIMAEUR", "LAUNCHCOINUSDC", "ETHUSD1", "ENAUSDC", "TRUMPUSDT", "XCNUSDT", "LIBERTYUSD1", "BTCEUR", "BNBUSDC", "VINEUSDT", "WCTUSDC", "AAVEUSDC", "CGPTUSDC", "MXEUR", "ULTIMAUSDC", "HYPEUSDT", "APEUSDT", "SOONUSDT", "WIFUSDC", "ACHUSDT", "XDCUSDC", "SNSUSDC", "EGL1USDT", "SAHARAUSDT", "CRVETH", "MUBARAKUSDC", "ETHBRL", "TRUMPEUR", "MELANIAEUR", "BABYUSDT", "ULTIMAUSDT", "FTTUSDT", "GHIBLIUSDC", "ETHBTC", "MNTUSDC", "INJUSDT", "LINKUSDT", "BTCBRL", "ETHUSDT", "SUIEUR", "KASUSDC", "FILUSDC", "NXPCUSDT", "DOODUSDC", "BTCUSDE", "RSRUSDT", "SUPRAUSDC", "BANKUSD1", "B2USDT", "GRIFFAINUSDT", "XRPBTC", "BDXNUSDC", "1DOLLARUSDT", "TAGUSD1", "XRPUSD1", "KASUSD1", "TRXBTC", "SOSOUSDT", "EPTUSDT", "XRPUSDE", "MXETH", "BMTUSDT", "GUNUSDC", "TRUMPUSDC", "LTCUSDC", "MILKUSDT", "NPCUSDC", "MOREUSDT", "CUSDT", "TRUMPUSD1", "ONDOUSDT", "LINKUSDC", "NODEUSDC", "LTCBTC", "XLMUSDC", "NAKAUSDC", "FLOCKUSDC", "RENDERUSDT", "PARTIUSDC", "XENUSDT", "SENUSDT", "DOLOUSDT", "RIOEUR", "SPXUSDC", "VIRTUALUSDT", "ENSUSDT", "ADAUSDT", "DOTUSDT", "RAIUSDT", "KAITOUSDT", "TIAUSDT", "SHIBUSDC", "TAOEUR", "MINAUSDT", "CRVUSDT", "MYXUSDT", "OXTETH", "APTUSDC", "BRLUSDT", "CAWUSDC", "LINKETH", "LTCEUR", "OPUSDT", "WBTCUSDC", "SERAPHUSDC", "MXUSDC", "HUMAUSDC", "DEXEUSDT", "USDRUSDT", "RAYUSDT", "OBTUSDC", "AIXBTUSDC", "AUSDC", "POPCATUSDC", "NILUSDT", "PEAQUSDT", "AZEROUSDT", "SXTUSDC", "PIUSD1", "QTUMBTC", "SOLEUR", "ADABTC", "TRXUSDT", "HOMEUSDT", "VELVETUSDT", "RAREUSDT", "MXUSDT", "PEPEUSDC", "BEEUSDT", "BOMBUSDT", "KASUSDE", "WHITEUSDT", "SOLUSDC", "OMGUSDT", "NEARUSDT", "PIUSDT", "PENGUUSDT", "PROMPTUSDT", "HYPERUSDC", "SIGNUSDC", "TAOUSDC", "ATOMUSDT", "OBOLUSDT", "AI16ZUSDT", "SHMUSDT", "ASRRUSDT", "VIDTUSDC", "INITUSDC", "USDEUSDT", "WALUSDT", "UNIUSDT", "CORNUSDC", "KASEUR", "MOONPIGUSDT", "SNXUSDT", "ESUSDT", "BCHBTC", "DOGEUSDT", "ADAEUR", "AIOTUSDT", "BABYDOGEUSDT", "SOPHUSDT", "XRPETH", "IDOLUSDT", "SGCUSDC", "PEPEUSDT", "XRPUSDT", "ETCBTC", "AGTUSDT",
	}

	// Построим направленный граф
	edges := map[string]map[string]bool{}
	for _, pair := range subPairs {
		base, quote := unpackPair(pair)
		if base == "" || quote == "" {
			log.Printf("[SKIP] cannot unpack pair: %s", pair)
			continue
		}
		if edges[base] == nil {
			edges[base] = map[string]bool{}
		}
		edges[base][quote] = true
	}

	// Собираем уникальные активы
	nodes := []string{}
	seen := map[string]bool{}
	for a := range edges {
		if !seen[a] {
			nodes = append(nodes, a)
			seen[a] = true
		}
		for b := range edges[a] {
			if !seen[b] {
				nodes = append(nodes, b)
				seen[b] = true
			}
		}
	}

	// Поиск треугольников
	var tris []triangle.Triangle
	uniq := map[[3]string]struct{}{}
	for _, a := range nodes {
		for b := range edges[a] {
			for c := range edges[b] {
				if edges[c][a] {
					arr := []string{a, b, c}
					sort.Strings(arr)
					key := [3]string{arr[0], arr[1], arr[2]}
					if _, ok := uniq[key]; !ok {
						uniq[key] = struct{}{}
						tris = append(tris, triangle.Triangle{A: a, B: b, C: c})
					}
				}
			}
		}
	}

	log.Printf("[INFO] Found %d triangles from %d pairs", len(tris), len(subPairs))
	return tris, nil
}

// Примитивный парсер базового и котируемого актива
func unpackPair(pair string) (string, string) {
	quotes := []string{"USDT", "USDC", "USDE", "BTC", "ETH", "EUR", "BRL", "USD1"}
	for _, q := range quotes {
		if len(pair) > len(q) && pair[len(pair)-len(q):] == q {
			return pair[:len(pair)-len(q)], q
		}
	}
	return "", ""
}

_________________________________________________________________________________________


package app

import (
	"encoding/json"
	"log"
	"strconv"
	"sync"
	"time"

	"cryptarb/internal/domain/exchange"
	"cryptarb/internal/domain/triangle"
	"cryptarb/internal/repository/filesystem"
)

type Arbitrager struct {
	Triangles       []triangle.Triangle
	latest          map[string]float64
	trianglesByPair map[string][]int
	sumProfit       float64
	mu              sync.Mutex
}

func New(path string, ex exchange.Exchange) (*Arbitrager, error) {
	// 1. Загружаем треугольники и фильтруем по доступности пар
	ts, err := filesystem.LoadTriangles(path)
	if err != nil {
		return nil, err
	}
	avail := ex.FetchAvailableSymbols()
	ts = triangle.Filter(ts, avail)

	log.Printf("[INIT] Loaded %d triangles after filtering", len(ts))

	// 2. Собираем мапу индексов и список всех потенциальных подписок
	trianglesByPair := make(map[string][]int)
	var subPairsRaw []string
	for i, tri := range ts {
		ab := tri.A + tri.B // A→B
		bc := tri.B + tri.C // B→C
		ca := tri.C + tri.A // C→A

		log.Printf("[TRI %2d] %s → %s → %s → %s (AB=%s BC=%s CA=%s)",
			i, tri.A, tri.B, tri.C, tri.A, ab, bc, ca)

		trianglesByPair[ab] = append(trianglesByPair[ab], i)
		trianglesByPair[bc] = append(trianglesByPair[bc], i)
		trianglesByPair[ca] = append(trianglesByPair[ca], i)

		subPairsRaw = append(subPairsRaw, ab, bc, ca)
	}
	log.Printf("[INIT] total raw pairs before filtering: %d", len(subPairsRaw))

	// 3. Фильтрация по доступным символам
	uniq := make(map[string]struct{})
	for _, p := range subPairsRaw {
		if avail[p] {
			uniq[p] = struct{}{}
		} else {
			log.Printf("[SKIP] %s not available on exchange", p)
		}
	}
	var subPairs []string
	for p := range uniq {
		subPairs = append(subPairs, p)
	}
	log.Printf("[INIT] total unique pairs after filtering: %d", len(subPairs))
	log.Printf("[INIT] subscribing on: %v", subPairs)

	arb := &Arbitrager{
		Triangles:       ts,
		latest:          make(map[string]float64),
		trianglesByPair: trianglesByPair,
	}

	// 4. Подписываемся чанками по maxPerConn
	const maxPerConn = 25
	for i := 0; i < len(subPairs); i += maxPerConn {
		end := i + maxPerConn
		if end > len(subPairs) {
			end = len(subPairs)
		}
		chunk := subPairs[i:end]
		log.Printf("[WS] subscribing chunk %d:%d: %v", i, end, chunk)

		go func(pairs []string) {
			for {
				if err := ex.SubscribeDeals(pairs, arb.HandleRaw); err != nil {
					log.Printf("[WS][%s] subscribe chunk error: %v; retrying…", ex.Name(), err)
					time.Sleep(time.Second)
					continue
				}
				return
			}
		}(chunk)
	}

	return arb, nil
}

func (a *Arbitrager) HandleRaw(exchangeName string, raw []byte) {
	// 1) Парсим WS-сообщение
	var msg struct {
		Symbol string `json:"s"`
		Data   struct {
			Deals []struct {
				Price string `json:"p"`
			} `json:"deals"`
		} `json:"d"`
	}
	if err := json.Unmarshal(raw, &msg); err != nil {
		log.Printf("[ERROR][%s] unmarshal raw: %v", exchangeName, err)
		return
	}
	if msg.Symbol == "" || len(msg.Data.Deals) == 0 {
		return
	}

	// 2) Конвертируем цену
	price, err := strconv.ParseFloat(msg.Data.Deals[0].Price, 64)
	if err != nil {
		log.Printf("[ERROR][%s] parse price %q: %v", exchangeName, msg.Data.Deals[0].Price, err)
		return
	}

	// 3) Сохраняем цену и логируем тик
	a.mu.Lock()
	a.latest[msg.Symbol] = price
	log.Printf("[TICK][%s] %s=%.8f", exchangeName, msg.Symbol, price)
	a.mu.Unlock()

	// 4) Проверяем все треугольники, где участвует этот symbol
	a.Check(msg.Symbol)
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	if len(indices) == 0 {
		return
	}

	const commission = 0.0005
	nf := (1 - commission) * (1 - commission) * (1 - commission)

	for _, i := range indices {
		tri := a.Triangles[i]
		ab := tri.A + tri.B // A→B
		bc := tri.B + tri.C // B→C
		ca := tri.C + tri.A // C→A

		p1, ok1 := a.latest[ab] // price of B per A
		p2, ok2 := a.latest[bc] // price of C per B
		p3, ok3 := a.latest[ca] // price of A per C

		if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		// Profit factor = p1 * p2 * p3 * net_fees
		profitFactor := p1 * p2 * p3 * nf
		profit := (profitFactor - 1) * 100

		//if profit > 0 {
		a.sumProfit += profit
		log.Printf("🔺 ARB %s/%s/%s profit=%.4f%% total=%.4f%%",
			tri.A, tri.B, tri.C, profit, a.sumProfit)
		//	}
	}
}


2025/07/28 12:00:41 [TICK][MEXC] BCHBTC=0.00485800
2025/07/28 12:00:41 [TICK][MEXC] BCHBTC=0.00485800
2025/07/28 12:00:41 [TICK][MEXC] BCHBTC=0.00485800
2025/07/28 12:00:41 [TICK][MEXC] BCHBTC=0.00485300
2025/07/28 12:00:41 [TICK][MEXC] BCHBTC=0.00485800
2025/07/28 12:00:41 [TICK][MEXC] BCHBTC=0.00485800
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.23000000
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63600000
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63600000
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63200000
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63600000
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63600000
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63600000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.23000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.23000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.42000000
2025/07/28 12:00:41 [TICK][MEXC] XLMUSDC=0.44290000
2025/07/28 12:00:41 [TICK][MEXC] USDCUSDT=0.99980000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.23000000
2025/07/28 12:00:41 [TICK][MEXC] PIUSD1=0.44735000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.12000000
2025/07/28 12:00:41 [TICK][MEXC] ULTIMAUSDT=5082.92000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119071.37000000
2025/07/28 12:00:41 [TICK][MEXC] ETHUSDT=3890.29000000
2025/07/28 12:00:41 [TICK][MEXC] MXUSDT=2.23470000
2025/07/28 12:00:41 [TICK][MEXC] MXUSDT=2.23470000
2025/07/28 12:00:41 [TICK][MEXC] MXUSDT=2.23470000
2025/07/28 12:00:41 [TICK][MEXC] XLMUSDT=0.44300000
2025/07/28 12:00:41 [TICK][MEXC] XLMUSDT=0.44300000
2025/07/28 12:00:41 [TICK][MEXC] XLMUSDT=0.44300000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.22000000
2025/07/28 12:00:41 [TICK][MEXC] DOGEUSDT=0.24179000
2025/07/28 12:00:41 [TICK][MEXC] DOGEUSDT=0.24179000
2025/07/28 12:00:41 [TICK][MEXC] DOGEUSDT=0.24179000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.22000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.11000000
2025/07/28 12:00:41 [TICK][MEXC] XRPUSDT=3.26250000
2025/07/28 12:00:41 [TICK][MEXC] XRPUSDT=3.26250000
2025/07/28 12:00:41 [TICK][MEXC] XRPUSDT=3.26250000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.11000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.22000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.43000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.42000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.43000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.43000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.42000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDT=119051.43000000
2025/07/28 12:00:41 [TICK][MEXC] MXUSDC=2.23730000
2025/07/28 12:00:41 [TICK][MEXC] MXETH=0.00057367
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.21000000
2025/07/28 12:00:41 [TICK][MEXC] MXBTC=0.00001875
2025/07/28 12:00:41 [TICK][MEXC] CRVUSDT=1.03080000
2025/07/28 12:00:41 [TICK][MEXC] CRVUSDT=1.03080000
2025/07/28 12:00:41 [TICK][MEXC] CRVUSDT=1.03080000
2025/07/28 12:00:41 [TICK][MEXC] CRVUSDT=1.03080000
2025/07/28 12:00:41 [TICK][MEXC] DOTUSDT=4.25300000
2025/07/28 12:00:41 [TICK][MEXC] DOGEEUR=0.20697000
2025/07/28 12:00:41 [TICK][MEXC] DOGEEUR=0.20699000
2025/07/28 12:00:41 [TICK][MEXC] DOGEEUR=0.20699000
2025/07/28 12:00:41 [TICK][MEXC] DOGEEUR=0.20699000
2025/07/28 12:00:41 [TICK][MEXC] DOGEEUR=0.20699000
2025/07/28 12:00:41 [TICK][MEXC] DOTUSDT=4.25300000
2025/07/28 12:00:41 [TICK][MEXC] DOGEEUR=0.20699000
2025/07/28 12:00:41 [TICK][MEXC] BRLUSDT=0.17900000
2025/07/28 12:00:41 [TICK][MEXC] XRPEUR=2.79330000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.21000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.10000000
2025/07/28 12:00:41 [TICK][MEXC] XRPUSDC=3.26440000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.10000000
2025/07/28 12:00:41 [TICK][MEXC] BTCUSDC=119073.21000000
2025/07/28 12:00:41 [TICK][MEXC] CRVETH=0.00026471
2025/07/28 12:00:41 [TICK][MEXC] CRVETH=0.00026466
2025/07/28 12:00:41 [TICK][MEXC] CRVETH=0.00026466
2025/07/28 12:00:41 [TICK][MEXC] CRVETH=0.00026466
2025/07/28 12:00:41 [TICK][MEXC] CRVETH=0.00026466
2025/07/28 12:00:41 [TICK][MEXC] CRVETH=0.00026466
2025/07/28 12:00:41 [TICK][MEXC] AVAXUSDC=26.63000000
2025/07/28 12:00:42 [TICK][MEXC] XLMUSDC=0.44300000
2025/07/28 12:00:42 [TICK][MEXC] XLMUSDC=0.44300000
2025/07/28 12:00:42 [TICK][MEXC] XLMUSDC=0.44300000




