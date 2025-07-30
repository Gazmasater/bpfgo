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



func (a *Arbitrager) ExecuteTriangle(tri triangle.Triangle, amountUSDT float64) error {
	if tri.A != "USDT" {
		return fmt.Errorf("треугольник должен начинаться с USDT")
	}

	log.Printf("🔺 Запуск арбитража по треугольнику: %s → %s → %s → %s", tri.A, tri.B, tri.C, tri.A)
	log.Printf("💰 Стартовая сумма: %.4f %s", amountUSDT, tri.A)

	// STEP 1: USDT → B
	log.Println("📌 Step 1: Покупаем", tri.B, "за USDT")

	symbol1, ok1, rev1 := a.normalizeSymbolDir(tri.B, tri.A) // всегда USDT → B
	log.Printf("🔎 Step 1 symbol: %s (rev=%v)", symbol1, rev1)

	if !ok1 {
		return fmt.Errorf("❌ нет пары %s/%s", tri.B, tri.A)
	}

	ask1, err := a.exchange.GetBestAsk(symbol1)
	if err != nil {
		return fmt.Errorf("❌ Step 1 ask error (%s): %v", symbol1, err)
	}
	log.Printf("📊 Step 1 ask: %.6f", ask1)

	if rev1 {
		ask1 = 1 / ask1
		log.Printf("🔁 Step 1 ask перевёрнут: %.6f", ask1)
	}

	ask1Adj := ask1 * 1.0003
	amountB := amountUSDT / ask1Adj
	log.Printf("💱 Step 1: BUY %s за %.4f USDT @ %.6f (adj %.6f) ≈ %.6f",
		tri.B, amountUSDT, ask1, ask1Adj, amountB)

	order1, err := a.exchange.PlaceMarketOrder(symbol1, "BUY", amountUSDT)
	if err != nil {
		return fmt.Errorf("❌ Step 1 order failed: %v", err)
	}
	log.Printf("✅ Step 1: OrderID %s", order1)

	// STEP 2: B → C
	log.Println("📌 Step 2: Обмениваем", tri.B, "→", tri.C)

	symbol2, ok2, rev2 := a.normalizeSymbolDir(tri.B, tri.C)
	log.Printf("🔎 Step 2 symbol: %s (rev=%v)", symbol2, rev2)

	if !ok2 {
		return fmt.Errorf("❌ нет пары %s/%s", tri.B, tri.C)
	}

	ask2, err := a.exchange.GetBestAsk(symbol2)
	if err != nil {
		return fmt.Errorf("❌ Step 2 ask error (%s): %v", symbol2, err)
	}
	log.Printf("📊 Step 2 ask: %.6f", ask2)

	if rev2 {
		ask2 = 1 / ask2
		log.Printf("🔁 Step 2 ask перевёрнут: %.6f", ask2)
	}

	ask2Adj := ask2 * 1.0003
	amountC := amountB / ask2Adj

	side2 := "BUY"
	qty2 := amountB
	if rev2 {
		side2 = "SELL"
		qty2 = amountB
	}

	log.Printf("💱 Step 2: %s %s → %s: %.6f (adj %.6f) qty=%.6f",
		side2, tri.B, tri.C, ask2, ask2Adj, qty2)

	order2, err := a.exchange.PlaceMarketOrder(symbol2, side2, qty2)
	if err != nil {
		return fmt.Errorf("❌ Step 2 order failed: %v", err)
	}
	log.Printf("✅ Step 2: OrderID %s", order2)

	// STEP 3: C → USDT
	log.Println("📌 Step 3: Обмениваем", tri.C, "→ USDT")

	symbol3, ok3, rev3 := a.normalizeSymbolDir(tri.C, tri.A)
	log.Printf("🔎 Step 3 symbol: %s (rev=%v)", symbol3, rev3)

	if !ok3 {
		return fmt.Errorf("❌ нет пары %s/%s", tri.C, tri.A)
	}

	bid3, err := a.exchange.GetBestBid(symbol3)
	if err != nil {
		return fmt.Errorf("❌ Step 3 bid error (%s): %v", symbol3, err)
	}
	log.Printf("📊 Step 3 bid: %.6f", bid3)

	if rev3 {
		bid3 = 1 / bid3
		log.Printf("🔁 Step 3 bid перевёрнут: %.6f", bid3)
	}

	bid3Adj := bid3 * 0.9997
	finalUSDT := amountC * bid3Adj

	side3 := "SELL"
	qty3 := amountC
	if rev3 {
		side3 = "BUY"
		qty3 = finalUSDT
	}

	log.Printf("💱 Step 3: %s %s → USDT @ %.6f (adj %.6f) qty=%.6f ≈ %.4f USDT",
		side3, tri.C, bid3, bid3Adj, qty3, finalUSDT)

	order3, err := a.exchange.PlaceMarketOrder(symbol3, side3, qty3)
	if err != nil {
		return fmt.Errorf("❌ Step 3 order failed: %v", err)
	}
	log.Printf("✅ Step 3: OrderID %s", order3)

	log.Printf("🎯 Арбитраж завершён: с %.4f USDT получили ≈ %.4f USDT", amountUSDT, finalUSDT)
	return nil
}

____________________________________________________________________________________

import (
	"fmt"
	"log"
	"math"
)

// вспомогательная функция для округления вниз по stepSize
func roundDown(val float64, decimals int) float64 {
	factor := math.Pow(10, float64(decimals))
	return math.Floor(val*factor) / factor
}

func (a *Arbitrager) ExecuteTriangle(tri triangle.Triangle, amountUSDT float64) error {
	if tri.A != "USDT" {
		return fmt.Errorf("треугольник должен начинаться с USDT")
	}

	log.Printf("🔺 Запуск арбитража по треугольнику: %s → %s → %s → %s", tri.A, tri.B, tri.C, tri.A)
	log.Printf("💰 Стартовая сумма: %.4f %s", amountUSDT, tri.A)

	// STEP 1: USDT → B
	log.Println("📌 Step 1: Покупаем", tri.B, "за USDT")

	symbol1, ok1, rev1 := a.normalizeSymbolDir(tri.B, tri.A)
	log.Printf("🔎 Step 1 symbol: %s (rev=%v)", symbol1, rev1)
	if !ok1 {
		return fmt.Errorf("❌ нет пары %s/%s", tri.B, tri.A)
	}

	ask1, err := a.exchange.GetBestAsk(symbol1)
	if err != nil {
		return fmt.Errorf("❌ Step 1 ask error (%s): %v", symbol1, err)
	}
	log.Printf("📊 Step 1 ask: %.6f", ask1)

	if rev1 {
		ask1 = 1 / ask1
		log.Printf("🔁 Step 1 ask перевёрнут: %.6f", ask1)
	}

	ask1Adj := ask1 * 1.0003
	amountB := amountUSDT / ask1Adj
	amountB = roundDown(amountB, 6)

	log.Printf("💱 Step 1: BUY %s за %.4f USDT @ %.6f (adj %.6f) ≈ %.6f",
		tri.B, amountUSDT, ask1, ask1Adj, amountB)

	order1, err := a.exchange.PlaceMarketOrder(symbol1, "BUY", amountUSDT)
	if err != nil {
		return fmt.Errorf("❌ Step 1 order failed: %v", err)
	}
	log.Printf("✅ Step 1: OrderID %s", order1)

	// STEP 2: B → C
	log.Println("📌 Step 2: Обмениваем", tri.B, "→", tri.C)

	symbol2, ok2, rev2 := a.normalizeSymbolDir(tri.B, tri.C)
	log.Printf("🔎 Step 2 symbol: %s (rev=%v)", symbol2, rev2)
	if !ok2 {
		return fmt.Errorf("❌ нет пары %s/%s", tri.B, tri.C)
	}

	ask2, err := a.exchange.GetBestAsk(symbol2)
	if err != nil {
		return fmt.Errorf("❌ Step 2 ask error (%s): %v", symbol2, err)
	}
	log.Printf("📊 Step 2 ask: %.6f", ask2)

	if rev2 {
		ask2 = 1 / ask2
		log.Printf("🔁 Step 2 ask перевёрнут: %.6f", ask2)
	}

	ask2Adj := ask2 * 1.0003
	amountC := amountB / ask2Adj
	amountC = roundDown(amountC, 6)

	var side2 string
	var qty2 float64
	if rev2 {
		side2 = "BUY"
		qty2 = roundDown(amountB, 6)
	} else {
		side2 = "SELL"
		qty2 = roundDown(amountB, 6)
	}

	log.Printf("💱 Step 2: %s %s → %s: %.6f (adj %.6f) qty=%.6f",
		side2, tri.B, tri.C, ask2, ask2Adj, qty2)

	order2, err := a.exchange.PlaceMarketOrder(symbol2, side2, qty2)
	if err != nil {
		return fmt.Errorf("❌ Step 2 order failed: %v", err)
	}
	log.Printf("✅ Step 2: OrderID %s", order2)

	// STEP 3: C → USDT
	log.Println("📌 Step 3: Обмениваем", tri.C, "→ USDT")

	symbol3, ok3, rev3 := a.normalizeSymbolDir(tri.C, tri.A)
	log.Printf("🔎 Step 3 symbol: %s (rev=%v)", symbol3, rev3)
	if !ok3 {
		return fmt.Errorf("❌ нет пары %s/%s", tri.C, tri.A)
	}

	bid3, err := a.exchange.GetBestBid(symbol3)
	if err != nil {
		return fmt.Errorf("❌ Step 3 bid error (%s): %v", symbol3, err)
	}
	log.Printf("📊 Step 3 bid: %.6f", bid3)

	if rev3 {
		bid3 = 1 / bid3
		log.Printf("🔁 Step 3 bid перевёрнут: %.6f", bid3)
	}

	bid3Adj := bid3 * 0.9997
	finalUSDT := amountC * bid3Adj
	finalUSDT = roundDown(finalUSDT, 6)

	var side3 string
	var qty3 float64
	if rev3 {
		side3 = "BUY"
		qty3 = roundDown(finalUSDT, 6)
	} else {
		side3 = "SELL"
		qty3 = roundDown(amountC, 6)
	}

	log.Printf("💱 Step 3: %s %s → USDT @ %.6f (adj %.6f) qty=%.6f ≈ %.4f USDT",
		side3, tri.C, bid3, bid3Adj, qty3, finalUSDT)

	order3, err := a.exchange.PlaceMarketOrder(symbol3, side3, qty3)
	if err != nil {
		return fmt.Errorf("❌ Step 3 order failed: %v", err)
	}
	log.Printf("✅ Step 3: OrderID %s", order3)

	log.Printf("🎯 Арбитраж завершён: с %.4f USDT получили ≈ %.4f USDT", amountUSDT, finalUSDT)
	return nil
}


025/07/31 02:57:35 📶 [MEXC] Pong after 221.42719ms
2025/07/31 02:57:51 🔺 ARB USDT/MX/USDC profit=0.0446%
2025/07/31 02:57:51 🔺 Запуск арбитража по треугольнику: USDT → MX → USDC → USDT
2025/07/31 02:57:51 💰 Стартовая сумма: 3.5000 USDT
2025/07/31 02:57:51 📌 Step 1: Покупаем MX за USDT
2025/07/31 02:57:51 🔎 Step 1 symbol: MXUSDT (rev=false)
2025/07/31 02:57:51 📊 Step 1 ask: 2.193200
2025/07/31 02:57:51 💱 Step 1: BUY MX за 3.5000 USDT @ 2.193200 (adj 2.193858) ≈ 1.595363
2025/07/31 02:57:51 ✅ Step 1: OrderID C02__579391239061786625022
2025/07/31 02:57:51 📌 Step 2: Обмениваем MX → USDC
2025/07/31 02:57:51 🔎 Step 2 symbol: MXUSDC (rev=false)
2025/07/31 02:57:52 📊 Step 2 ask: 2.191600
2025/07/31 02:57:52 💱 Step 2: SELL MX → USDC: 2.191600 (adj 2.192257) qty=1.595363
2025/07/31 02:57:52 ❌ Ошибка арбитража: ❌ Step 2 order failed: order failed: {"msg":" quantity scale is invalid","code":400}
^Csignal: interrupt




