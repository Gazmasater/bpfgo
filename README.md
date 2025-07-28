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


🔧 1. Расширь структуру Arbitrager:

type Arbitrager struct {
	Triangles       []triangle.Triangle
	latest          map[string]float64
	trianglesByPair map[string][]int
	sumProfit       float64
	realSymbols     map[string]bool // 🔥 добавь
	mu              sync.Mutex
}
🔧 2. В New(...) сохрани avail:

arb := &Arbitrager{
	Triangles:       ts,
	latest:          make(map[string]float64),
	trianglesByPair: trianglesByPair,
	realSymbols:     avail, // 🔥 сохрани реальные пары
}
🔧 3. Добавь метод normalizeSymbolDir(...):

func (a *Arbitrager) normalizeSymbolDir(base, quote string) (symbol string, ok bool, reversed bool) {
	if a.realSymbols[base+quote] {
		return base + quote, true, false // прямое направление
	}
	if a.realSymbols[quote+base] {
		return quote + base, true, true // обратное направление
	}
	return "", false, false
}
🔧 4. Обнови Check(symbol):
Заменить вот это:

go
Копировать код
ab := tri.A + tri.B
bc := tri.B + tri.C
ca := tri.C + tri.A

p1, ok1 := a.latest[ab]
p2, ok2 := a.latest[bc]
p3, ok3 := a.latest[ca]
На это:

go
Копировать код
ab, okAB, revAB := a.normalizeSymbolDir(tri.A, tri.B)
bc, okBC, revBC := a.normalizeSymbolDir(tri.B, tri.C)
ca, okCA, revCA := a.normalizeSymbolDir(tri.C, tri.A)

if !okAB || !okBC || !okCA {
	log.Printf("[SKIP] triangle %s/%s/%s has no real pairs", tri.A, tri.B, tri.C)
	return
}

p1, ok1 := a.latest[ab]
p2, ok2 := a.latest[bc]
p3, ok3 := a.latest[ca]

if !ok1 || !ok2 || !ok3 || p1 == 0 || p2 == 0 || p3 == 0 {
	log.Printf("ab bc ca ok1 ok2 ok3 %s %s %s %v %v %v", ab, bc, ca, ok1, ok2, ok3)
	return
}

// 🔁 инвертируем, если направление "обратное"
if revAB {
	p1 = 1 / p1
}
if revBC {
	p2 = 1 / p2
}
if revCA {
	p3 = 1 / p3
}
Остальное — оставить как есть:

go
Копировать код
const commission = 0.0005
nf := (1 - commission) * (1 - commission) * (1 - commission)

profitFactor := p1 * p2 * p3 * nf
profit := (profitFactor - 1) * 100

a.sumProfit += profit
log.Printf("🔺 ARB %s/%s/%s profit=%.4f%% total=%.4f%%",
	tri.A, tri.B, tri.C, profit, a.sumProfit)


2025/07/28 13:52:25 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=1.1042%
2025/07/28 13:52:25 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=1.5973%
2025/07/28 13:52:25 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=2.0904%
2025/07/28 13:52:25 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=2.5643%
2025/07/28 13:52:25 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=3.0574%
2025/07/28 13:52:25 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=3.5312%
2025/07/28 13:52:25 🔺 ARB USDC/USDT/ULTIMA profit=0.5031% total=4.0344%
2025/07/28 13:52:25 🔺 ARB USDT/ULTIMA/USDC profit=0.5031% total=4.5375%
2025/07/28 13:52:25 🔺 ARB ULTIMA/USDC/USDT profit=0.5031% total=5.0407%
2025/07/28 13:52:26 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=5.5338%
2025/07/28 13:52:26 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=6.0269%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=6.5200%
2025/07/28 13:52:26 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=7.0131%
2025/07/28 13:52:26 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=7.5062%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=7.9993%
2025/07/28 13:52:26 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=8.4924%
2025/07/28 13:52:26 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=8.9855%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=9.4785%
2025/07/28 13:52:26 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=9.9173%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=10.3560%
2025/07/28 13:52:26 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=10.7947%
2025/07/28 13:52:26 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=11.2568%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=11.7190%
2025/07/28 13:52:26 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=12.1811%
2025/07/28 13:52:26 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=12.6198%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=13.0586%
2025/07/28 13:52:26 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=13.4973%
2025/07/28 13:52:26 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=13.9360%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=14.3747%
2025/07/28 13:52:26 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=14.8134%
2025/07/28 13:52:26 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=15.2756%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=15.7377%
2025/07/28 13:52:26 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=16.1998%
2025/07/28 13:52:26 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=16.6929%
2025/07/28 13:52:26 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=17.1860%
2025/07/28 13:52:26 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=17.6791%
2025/07/28 13:52:28 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=18.1722%
2025/07/28 13:52:28 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=18.6653%
2025/07/28 13:52:28 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=19.1584%
2025/07/28 13:52:28 🔺 ARB USDC/USDT/ULTIMA profit=0.5031% total=19.6616%
2025/07/28 13:52:28 🔺 ARB USDT/ULTIMA/USDC profit=0.5031% total=20.1647%
2025/07/28 13:52:28 🔺 ARB ULTIMA/USDC/USDT profit=0.5031% total=20.6679%
2025/07/28 13:52:29 🔺 ARB USDC/EUR/ULTIMA profit=0.4856% total=21.1534%
2025/07/28 13:52:29 🔺 ARB ULTIMA/USDC/EUR profit=0.4856% total=21.6390%
2025/07/28 13:52:29 🔺 ARB EUR/ULTIMA/USDC profit=0.4856% total=22.1246%
2025/07/28 13:52:29 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=22.5633%
2025/07/28 13:52:29 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=23.0020%
2025/07/28 13:52:29 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=23.4407%
2025/07/28 13:52:29 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=23.9146%
2025/07/28 13:52:29 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=24.3884%
2025/07/28 13:52:29 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=24.8623%
2025/07/28 13:52:29 🔺 ARB USDC/USDT/ULTIMA profit=0.5031% total=25.3654%
2025/07/28 13:52:29 🔺 ARB USDT/ULTIMA/USDC profit=0.5031% total=25.8686%
2025/07/28 13:52:29 🔺 ARB ULTIMA/USDC/USDT profit=0.5031% total=26.3717%
2025/07/28 13:52:29 📶 [MEXC] Pong after 215.703058ms
2025/07/28 13:52:29 📶 [MEXC] Pong after 215.996215ms
2025/07/28 13:52:29 📶 [MEXC] Pong after 229.541452ms
2025/07/28 13:52:29 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=26.8104%
2025/07/28 13:52:29 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=27.2491%
2025/07/28 13:52:29 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=27.6879%
2025/07/28 13:52:29 📶 [MEXC] Pong after 432.114378ms
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=28.1810%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=28.6741%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=29.1672%
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=29.6603%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=30.1534%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=30.6464%
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=31.1395%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=31.6326%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=32.1257%
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=32.6188%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=33.1119%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=33.6050%
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=34.0981%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=34.5912%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=35.0843%
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=35.5774%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=36.0705%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=36.5636%
2025/07/28 13:52:30 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=37.0567%
2025/07/28 13:52:30 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=37.5498%
2025/07/28 13:52:30 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=38.0429%
2025/07/28 13:52:31 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=38.5051%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=38.9672%
2025/07/28 13:52:31 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=39.4293%
2025/07/28 13:52:31 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=39.9224%
2025/07/28 13:52:31 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=40.4155%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=40.9086%
2025/07/28 13:52:31 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=41.4017%
2025/07/28 13:52:31 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=41.8948%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=42.3879%
2025/07/28 13:52:31 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=42.8810%
2025/07/28 13:52:31 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=43.3741%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=43.8672%
2025/07/28 13:52:31 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=44.3603%
2025/07/28 13:52:31 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=44.8534%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=45.3465%
2025/07/28 13:52:31 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=45.8396%
2025/07/28 13:52:31 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=46.3327%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=46.8258%
2025/07/28 13:52:31 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=47.3189%
2025/07/28 13:52:31 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=47.8120%
2025/07/28 13:52:31 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=48.3051%
2025/07/28 13:52:32 🔺 ARB USDC/EUR/ULTIMA profit=0.4856% total=48.7907%
2025/07/28 13:52:32 🔺 ARB ULTIMA/USDC/EUR profit=0.4856% total=49.2762%
2025/07/28 13:52:32 🔺 ARB EUR/ULTIMA/USDC profit=0.4856% total=49.7618%
2025/07/28 13:52:32 🔺 ARB USDC/USDT/ULTIMA profit=0.5031% total=50.2649%
2025/07/28 13:52:32 🔺 ARB USDT/ULTIMA/USDC profit=0.5031% total=50.7681%
2025/07/28 13:52:32 🔺 ARB ULTIMA/USDC/USDT profit=0.5031% total=51.2712%
2025/07/28 13:52:33 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=51.7643%
2025/07/28 13:52:33 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=52.2574%
2025/07/28 13:52:33 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=52.7505%
2025/07/28 13:52:33 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=53.2436%
2025/07/28 13:52:33 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=53.7367%
2025/07/28 13:52:33 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=54.2298%
2025/07/28 13:52:34 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=54.6685%
2025/07/28 13:52:34 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=55.1072%
2025/07/28 13:52:34 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=55.5460%
2025/07/28 13:52:34 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=56.0391%
2025/07/28 13:52:34 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=56.5322%
2025/07/28 13:52:34 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=57.0253%
2025/07/28 13:52:34 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=57.5184%
2025/07/28 13:52:34 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=58.0115%
2025/07/28 13:52:34 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=58.5046%
2025/07/28 13:52:35 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=58.9976%
2025/07/28 13:52:35 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=59.4907%
2025/07/28 13:52:35 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=59.9838%
2025/07/28 13:52:35 🔺 ARB USDC/USDT/ULTIMA profit=0.4931% total=60.4769%
2025/07/28 13:52:35 🔺 ARB USDT/ULTIMA/USDC profit=0.4931% total=60.9700%
2025/07/28 13:52:35 🔺 ARB ULTIMA/USDC/USDT profit=0.4931% total=61.4631%
2025/07/28 13:52:35 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=61.9370%
2025/07/28 13:52:35 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=62.4108%
2025/07/28 13:52:35 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=62.8847%
2025/07/28 13:52:36 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=63.4028%
2025/07/28 13:52:36 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=63.9209%
2025/07/28 13:52:36 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=64.4390%
2025/07/28 13:52:36 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=64.9571%
2025/07/28 13:52:36 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=65.4752%
2025/07/28 13:52:36 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=65.9933%
2025/07/28 13:52:37 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=66.4555%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=66.9176%
2025/07/28 13:52:37 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=67.3798%
2025/07/28 13:52:37 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=67.8185%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=68.2572%
2025/07/28 13:52:37 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=68.6959%
2025/07/28 13:52:37 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=69.1346%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=69.5733%
2025/07/28 13:52:37 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=70.0121%
2025/07/28 13:52:37 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=70.4508%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=70.8895%
2025/07/28 13:52:37 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=71.3282%
2025/07/28 13:52:37 🔺 ARB USDC/EUR/ULTIMA profit=0.4856% total=71.8138%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/EUR profit=0.4856% total=72.2993%
2025/07/28 13:52:37 🔺 ARB EUR/ULTIMA/USDC profit=0.4856% total=72.7849%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=73.3030%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=73.8211%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=74.3392%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=74.8573%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=75.3754%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=75.8935%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=76.4116%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=76.9298%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=77.4479%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=77.9660%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=78.4841%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=79.0022%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=79.5203%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=80.0384%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=80.5565%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=81.0746%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=81.5927%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=82.1108%
2025/07/28 13:52:37 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=82.6289%
2025/07/28 13:52:37 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=83.1470%
2025/07/28 13:52:37 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=83.6652%
2025/07/28 13:52:38 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=84.1390%
2025/07/28 13:52:38 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=84.6129%
2025/07/28 13:52:38 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=85.0867%
2025/07/28 13:52:38 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=85.6048%
2025/07/28 13:52:38 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=86.1229%
2025/07/28 13:52:38 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=86.6410%
2025/07/28 13:52:39 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=87.1591%
2025/07/28 13:52:39 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=87.6772%
2025/07/28 13:52:39 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=88.1954%
2025/07/28 13:52:39 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=88.7135%
2025/07/28 13:52:39 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=89.2316%
2025/07/28 13:52:39 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=89.7497%
2025/07/28 13:52:39 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=90.2678%
2025/07/28 13:52:39 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=90.7859%
2025/07/28 13:52:39 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=91.3040%
2025/07/28 13:52:39 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=91.8221%
2025/07/28 13:52:39 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=92.3402%
2025/07/28 13:52:39 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=92.8583%
2025/07/28 13:52:39 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=93.3764%
2025/07/28 13:52:39 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=93.8945%
2025/07/28 13:52:39 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=94.4126%
2025/07/28 13:52:39 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=94.9308%
2025/07/28 13:52:39 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=95.4489%
2025/07/28 13:52:39 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=95.9670%
2025/07/28 13:52:40 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=96.4851%
2025/07/28 13:52:40 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=97.0032%
2025/07/28 13:52:40 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=97.5213%
2025/07/28 13:52:40 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=98.0394%
2025/07/28 13:52:40 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=98.5575%
2025/07/28 13:52:40 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=99.0756%
2025/07/28 13:52:40 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=99.5937%
2025/07/28 13:52:40 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=100.1118%
2025/07/28 13:52:40 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=100.6299%
2025/07/28 13:52:41 🔺 ARB USDC/EUR/ULTIMA profit=0.4504% total=101.0804%
2025/07/28 13:52:41 🔺 ARB ULTIMA/USDC/EUR profit=0.4504% total=101.5308%
2025/07/28 13:52:41 🔺 ARB EUR/ULTIMA/USDC profit=0.4504% total=101.9812%
2025/07/28 13:52:41 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=102.5094%
2025/07/28 13:52:41 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=103.0375%
2025/07/28 13:52:41 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=103.5657%
2025/07/28 13:52:41 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=104.0939%
2025/07/28 13:52:41 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=104.6220%
2025/07/28 13:52:41 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=105.1502%
2025/07/28 13:52:41 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=105.6783%
2025/07/28 13:52:41 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=106.2065%
2025/07/28 13:52:41 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=106.7347%
2025/07/28 13:52:41 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=107.2628%
2025/07/28 13:52:41 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=107.7910%
2025/07/28 13:52:41 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=108.3192%
2025/07/28 13:52:42 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=108.7930%
2025/07/28 13:52:42 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=109.2669%
2025/07/28 13:52:42 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=109.7407%
2025/07/28 13:52:42 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=110.2146%
2025/07/28 13:52:42 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=110.6884%
2025/07/28 13:52:42 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=111.1623%
2025/07/28 13:52:42 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=111.6361%
2025/07/28 13:52:42 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=112.1100%
2025/07/28 13:52:42 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=112.5838%
2025/07/28 13:52:42 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=113.0460%
2025/07/28 13:52:42 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=113.5081%
2025/07/28 13:52:42 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=113.9702%
2025/07/28 13:52:42 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=114.4441%
2025/07/28 13:52:42 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=114.9179%
2025/07/28 13:52:42 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=115.3918%
2025/07/28 13:52:42 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=115.8656%
2025/07/28 13:52:42 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=116.3395%
2025/07/28 13:52:42 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=116.8133%
2025/07/28 13:52:43 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=117.3315%
2025/07/28 13:52:43 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=117.8496%
2025/07/28 13:52:43 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=118.3677%
2025/07/28 13:52:43 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=118.8958%
2025/07/28 13:52:43 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=119.4240%
2025/07/28 13:52:43 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=119.9522%
2025/07/28 13:52:43 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=120.4803%
2025/07/28 13:52:43 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=121.0085%
2025/07/28 13:52:43 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=121.5366%
2025/07/28 13:52:43 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=122.0648%
2025/07/28 13:52:43 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=122.5930%
2025/07/28 13:52:43 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=123.1211%
2025/07/28 13:52:44 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=123.5598%
2025/07/28 13:52:44 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=123.9986%
2025/07/28 13:52:44 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=124.4373%
2025/07/28 13:52:44 📶 [MEXC] Pong after 206.844763ms
2025/07/28 13:52:44 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=124.9554%
2025/07/28 13:52:44 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=125.4735%
2025/07/28 13:52:44 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=125.9916%
2025/07/28 13:52:44 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=126.5097%
2025/07/28 13:52:44 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=127.0278%
2025/07/28 13:52:44 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=127.5459%
2025/07/28 13:52:44 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=128.0640%
2025/07/28 13:52:44 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=128.5821%
2025/07/28 13:52:44 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=129.1002%
2025/07/28 13:52:44 📶 [MEXC] Pong after 209.863939ms
2025/07/28 13:52:44 📶 [MEXC] Pong after 212.891493ms
2025/07/28 13:52:44 📶 [MEXC] Pong after 214.578038ms
2025/07/28 13:52:44 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=129.6183%
2025/07/28 13:52:44 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=130.1365%
2025/07/28 13:52:44 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=130.6546%
2025/07/28 13:52:44 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=131.1727%
2025/07/28 13:52:44 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=131.6908%
2025/07/28 13:52:44 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=132.2089%
2025/07/28 13:52:45 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=132.7370%
2025/07/28 13:52:45 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=133.2652%
2025/07/28 13:52:45 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=133.7934%
2025/07/28 13:52:45 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=134.3215%
2025/07/28 13:52:45 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=134.8497%
2025/07/28 13:52:45 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=135.3779%
2025/07/28 13:52:45 🔺 ARB USDC/EUR/ULTIMA profit=0.4856% total=135.8634%
2025/07/28 13:52:45 🔺 ARB ULTIMA/USDC/EUR profit=0.4856% total=136.3490%
2025/07/28 13:52:45 🔺 ARB EUR/ULTIMA/USDC profit=0.4856% total=136.8345%
2025/07/28 13:52:45 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=137.3527%
2025/07/28 13:52:45 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=137.8708%
2025/07/28 13:52:45 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=138.3889%
2025/07/28 13:52:46 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=138.9170%
2025/07/28 13:52:46 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=139.4452%
2025/07/28 13:52:46 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=139.9734%
2025/07/28 13:52:46 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=140.4915%
2025/07/28 13:52:46 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=141.0096%
2025/07/28 13:52:46 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=141.5277%
2025/07/28 13:52:46 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=142.0458%
2025/07/28 13:52:46 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=142.5639%
2025/07/28 13:52:46 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=143.0820%
2025/07/28 13:52:46 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=143.6001%
2025/07/28 13:52:46 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=144.1182%
2025/07/28 13:52:46 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=144.6363%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4504% total=145.0867%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4504% total=145.5372%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4504% total=145.9876%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4270% total=146.4146%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4270% total=146.8416%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4270% total=147.2686%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4504% total=147.7190%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4504% total=148.1695%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4504% total=148.6199%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4504% total=149.0703%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4504% total=149.5208%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4504% total=149.9712%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4270% total=150.3982%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4270% total=150.8252%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4270% total=151.2522%
2025/07/28 13:52:47 🔺 ARB USDC/USDT/ULTIMA profit=0.5282% total=151.7804%
2025/07/28 13:52:47 🔺 ARB USDT/ULTIMA/USDC profit=0.5282% total=152.3085%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/USDT profit=0.5282% total=152.8367%
2025/07/28 13:52:47 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=153.3548%
2025/07/28 13:52:47 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=153.8729%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=154.3910%
2025/07/28 13:52:47 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=154.9091%
2025/07/28 13:52:47 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=155.4272%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=155.9453%
2025/07/28 13:52:47 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=156.4634%
2025/07/28 13:52:47 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=156.9815%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=157.4997%
2025/07/28 13:52:47 🔺 ARB USDC/USDT/ULTIMA profit=0.5181% total=158.0178%
2025/07/28 13:52:47 🔺 ARB USDT/ULTIMA/USDC profit=0.5181% total=158.5359%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/USDT profit=0.5181% total=159.0540%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=159.5278%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=160.0017%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=160.4755%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4856% total=160.9611%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4856% total=161.4467%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4856% total=161.9322%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4856% total=162.4178%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4856% total=162.9033%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4856% total=163.3889%
2025/07/28 13:52:47 🔺 ARB USDC/EUR/ULTIMA profit=0.4387% total=163.8276%
2025/07/28 13:52:47 🔺 ARB ULTIMA/USDC/EUR profit=0.4387% total=164.2663%
2025/07/28 13:52:47 🔺 ARB EUR/ULTIMA/USDC profit=0.4387% total=164.7051%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=165.2197%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=165.7344%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=166.2490%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=166.7637%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=167.2784%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=167.7930%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=168.3077%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=168.8223%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=169.3370%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=169.8517%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=170.3663%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=170.8810%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=171.3957%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=171.9103%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=172.4250%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=172.9396%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=173.4543%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=173.9690%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5147% total=174.4836%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5147% total=174.9983%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5147% total=175.5129%
2025/07/28 13:52:48 🔺 ARB USDC/EUR/ULTIMA profit=0.4739% total=175.9868%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/EUR profit=0.4739% total=176.4606%
2025/07/28 13:52:48 🔺 ARB EUR/ULTIMA/USDC profit=0.4739% total=176.9345%
2025/07/28 13:52:48 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=177.3966%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=177.8588%
2025/07/28 13:52:48 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=178.3209%
2025/07/28 13:52:48 🔺 ARB USDC/EUR/ULTIMA profit=0.4504% total=178.7713%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/EUR profit=0.4504% total=179.2218%
2025/07/28 13:52:48 🔺 ARB EUR/ULTIMA/USDC profit=0.4504% total=179.6722%
2025/07/28 13:52:48 🔺 ARB USDC/EUR/ULTIMA profit=0.4621% total=180.1343%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/EUR profit=0.4621% total=180.5965%
2025/07/28 13:52:48 🔺 ARB EUR/ULTIMA/USDC profit=0.4621% total=181.0586%
2025/07/28 13:52:48 🔺 ARB USDC/EUR/ULTIMA profit=0.4504% total=181.5091%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/EUR profit=0.4504% total=181.9595%
2025/07/28 13:52:48 🔺 ARB EUR/ULTIMA/USDC profit=0.4504% total=182.4099%
2025/07/28 13:52:48 🔺 ARB USDC/USDT/ULTIMA profit=0.5046% total=182.9145%
2025/07/28 13:52:48 🔺 ARB USDT/ULTIMA/USDC profit=0.5046% total=183.4191%
2025/07/28 13:52:48 🔺 ARB ULTIMA/USDC/USDT profit=0.5046% total=183.9237%
2025/07/28 13:52:55 🔺 ARB USDC/USDT/ULTIMA profit=0.5046% total=184.4283%
2025/07/28 13:52:55 🔺 ARB USDT/ULTIMA/USDC profit=0.5046% total=184.9329%
2025/07/28 13:52:55 🔺 ARB ULTIMA/USDC/USDT profit=0.5046% total=185.4376%
2025/07/28 13:52:55 🔺 ARB USDC/USDT/ULTIMA profit=0.5046% total=185.9422%
2025/07/28 13:52:55 🔺 ARB USDT/ULTIMA/USDC profit=0.5046% total=186.4468%
2025/07/28 13:52:55 🔺 ARB ULTIMA/USDC/USDT profit=0.5046% total=186.9514%
2025/07/28 13:52:56 🔺 ARB USDC/EUR/ULTIMA profit=0.4718% total=187.4232%
2025/07/28 13:52:56 🔺 ARB ULTIMA/USDC/EUR profit=0.4718% total=187.8949%
2025/07/28 13:52:56 🔺 ARB EUR/ULTIMA/USDC profit=0.4718% total=188.3667%
2025/07/28 13:52:59 📶 [MEXC] Pong after 210.30273ms
2025/07/28 13:52:59 📶 [MEXC] Pong after 211.627927ms
2025/07/28 13:52:59 📶 [MEXC] Pong after 214.433405ms
2025/07/28 13:52:59 📶 [MEXC] Pong after 224.190719ms
2025/07/28 13:53:06 🔺 ARB USDC/USDT/ULTIMA profit=0.4386% total=188.8053%
2025/07/28 13:53:06 🔺 ARB USDT/ULTIMA/USDC profit=0.4386% total=189.2438%
2025/07/28 13:53:06 🔺 ARB ULTIMA/USDC/USDT profit=0.4386% total=189.6824%
2025/07/28 13:53:07 🔺 ARB USDC/EUR/ULTIMA profit=0.4256% total=190.1080%
2025/07/28 13:53:07 🔺 ARB ULTIMA/USDC/EUR profit=0.4256% total=190.5336%
2025/07/28 13:53:07 🔺 ARB EUR/ULTIMA/USDC profit=0.4256% total=190.9592%
2025/07/28 13:53:08 🔺 ARB USDC/USDT/ULTIMA profit=0.4486% total=191.4078%
2025/07/28 13:53:08 🔺 ARB USDT/ULTIMA/USDC profit=0.4486% total=191.8564%
2025/07/28 13:53:08 🔺 ARB ULTIMA/USDC/USDT profit=0.4486% total=192.3050%
2025/07/28 13:53:08 🔺 ARB USDC/EUR/ULTIMA profit=0.4490% total=192.7541%
2025/07/28 13:53:08 🔺 ARB ULTIMA/USDC/EUR profit=0.4490% total=193.2031%
2025/07/28 13:53:08 🔺 ARB EUR/ULTIMA/USDC profit=0.4490% total=193.6522%
2025/07/28 13:53:14 📶 [MEXC] Pong after 205.060668ms



