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


(pprof) top
Showing nodes accounting for 130ms, 100% of 130ms total
Showing top 10 nodes out of 62
      flat  flat%   sum%        cum   cum%
      40ms 30.77% 30.77%       40ms 30.77%  internal/runtime/syscall.Syscall6
      10ms  7.69% 38.46%       10ms  7.69%  encoding/json.stateBeginValue
      10ms  7.69% 46.15%       10ms  7.69%  encoding/json.unquoteBytes
      10ms  7.69% 53.85%       10ms  7.69%  memeqbody
      10ms  7.69% 61.54%       10ms  7.69%  runtime.(*spanSet).push
      10ms  7.69% 69.23%       10ms  7.69%  runtime.ifaceeq
      10ms  7.69% 76.92%       10ms  7.69%  runtime.memclrNoHeapPointers
      10ms  7.69% 84.62%       10ms  7.69%  runtime.nextFreeFast
      10ms  7.69% 92.31%       10ms  7.69%  runtime.typePointers.next
      10ms  7.69%   100%       10ms  7.69%  strconv.ParseFloat
(pprof) 



Build ID: 18dfa87cfc416462058286251d48f916dc0060b6
Type: cpu
Time: 2025-08-04 18:11:58 MSK
Duration: 30s, Total samples = 220ms ( 0.73%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 170ms, 77.27% of 220ms total
Showing top 10 nodes out of 91
      flat  flat%   sum%        cum   cum%
      50ms 22.73% 22.73%       50ms 22.73%  internal/runtime/syscall.Syscall6
      30ms 13.64% 36.36%       30ms 13.64%  runtime.nextFreeFast (inline)
      20ms  9.09% 45.45%       20ms  9.09%  runtime.mapaccess1_faststr
      10ms  4.55% 50.00%       20ms  9.09%  encoding/json.(*decodeState).array
      10ms  4.55% 54.55%       30ms 13.64%  encoding/json.(*scanner).pushParseState
      10ms  4.55% 59.09%       60ms 27.27%  encoding/json.checkValid
      10ms  4.55% 63.64%       10ms  4.55%  encoding/json.state1
      10ms  4.55% 68.18%       10ms  4.55%  encoding/json.stateInString
      10ms  4.55% 72.73%       10ms  4.55%  internal/poll.(*fdMutex).rwunlock
      10ms  4.55% 77.27%       20ms  9.09%  runtime.(*mcache).prepareForSweep
(pprof) 



howing nodes accounting for 120ms, 100% of 120ms total
Showing top 10 nodes out of 67
      flat  flat%   sum%        cum   cum%
      20ms 16.67% 16.67%       20ms 16.67%  internal/runtime/syscall.Syscall6
      20ms 16.67% 33.33%       20ms 16.67%  runtime.futex
      10ms  8.33% 41.67%       40ms 33.33%  bytes.(*Buffer).ReadFrom
      10ms  8.33% 50.00%       10ms  8.33%  crypto/tls.(*halfConn).decrypt
      10ms  8.33% 58.33%       10ms  8.33%  github.com/json-iterator/go.(*Iterator).readPositiveFloat64
      10ms  8.33% 66.67%       10ms  8.33%  github.com/json-iterator/go.locatePath
      10ms  8.33% 75.00%       20ms 16.67%  runtime.entersyscall
      10ms  8.33% 83.33%       10ms  8.33%  runtime.heapSetTypeNoHeader
      10ms  8.33% 91.67%       10ms  8.33%  runtime.memclrNoHeapPointers
      10ms  8.33%   100%       30ms 25.00%  runtime.schedule
(pprof) 


az358@gaz358-BOD-WXX9:~/myprog/crypt$ go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
Fetching profile over HTTP from http://localhost:6060/debug/pprof/profile?seconds=30
Saved profile in /home/gaz358/pprof/pprof.cryptarb.samples.cpu.012.pb.gz
File: cryptarb
Build ID: 4b4ef03c1c40450744136a477782f1c9c14a6cdb
Type: cpu
Time: 2025-08-04 23:33:46 MSK
Duration: 30s, Total samples = 110ms ( 0.37%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 110ms, 100% of 110ms total
Showing top 10 nodes out of 47
      flat  flat%   sum%        cum   cum%
      40ms 36.36% 36.36%       40ms 36.36%  internal/runtime/syscall.Syscall6
      10ms  9.09% 45.45%       10ms  9.09%  internal/runtime/maps.ctrlGroup.matchH2
      10ms  9.09% 54.55%       10ms  9.09%  runtime.(*mcache).prepareForSweep
      10ms  9.09% 63.64%       10ms  9.09%  runtime.(*timeHistogram).record
      10ms  9.09% 72.73%       10ms  9.09%  runtime.concatstrings
      10ms  9.09% 81.82%       10ms  9.09%  runtime.nanotime1
      10ms  9.09% 90.91%       10ms  9.09%  runtime.pollInfo.closing (inline)
      10ms  9.09%   100%       10ms  9.09%  runtime.runqget
         0     0%   100%       20ms 18.18%  bufio.(*Reader).Peek
         0     0%   100%       20ms 18.18%  bufio.(*Reader).fill
(pprof) 






package app

import (
	"bytes"
	"log"
	"strconv"
	"sync"
)

// PrecomputedLeg хранит заранее вычисленные данные по каждому leg.
type PrecomputedLeg struct {
	Symbol string
	Invert bool
}

// OptimizedTriangle хранит заранее вычисленные legs.
type OptimizedTriangle struct {
	AB PrecomputedLeg
	BC PrecomputedLeg
	CA PrecomputedLeg
	A, B, C string
}

// Arbitrager оптимизированный.
type Arbitrager struct {
	Triangles       []OptimizedTriangle
	latest          map[string]float64
	trianglesByPair map[string][]int
	realSymbols     map[string]bool
	mu              sync.Mutex
	StartAmount     float64
}

// PrecomputeTriangles оптимизирует и заранее вычисляет символы и инверсии.
func PrecomputeTriangles(tris []triangle.Triangle, avail map[string]bool) []OptimizedTriangle {
	out := make([]OptimizedTriangle, 0, len(tris))

	for _, tri := range tris {
		ab, okAB, invAB := normalizeSymbolDirStatic(tri.A, tri.B, avail)
		bc, okBC, invBC := normalizeSymbolDirStatic(tri.B, tri.C, avail)
		ca, okCA, invCA := normalizeSymbolDirStatic(tri.C, tri.A, avail)

		if !okAB || !okBC || !okCA {
			continue
		}

		out = append(out, OptimizedTriangle{
			AB: PrecomputedLeg{Symbol: ab, Invert: invAB},
			BC: PrecomputedLeg{Symbol: bc, Invert: invBC},
			CA: PrecomputedLeg{Symbol: ca, Invert: invCA},
			A:  tri.A, B: tri.B, C: tri.C,
		})
	}

	return out
}

func normalizeSymbolDirStatic(base, quote string, avail map[string]bool) (symbol string, ok bool, invert bool) {
	if avail[base+quote] {
		return base + quote, true, false
	}
	if avail[quote+base] {
		return quote + base, true, true
	}
	return "", false, false
}

func (a *Arbitrager) HandleRaw(_exchange string, raw []byte) {
	// парсим symbol
	sym := parseSymbol(raw)
	if sym == "" {
		return
	}

	a.mu.Lock()
	price, ok := parsePrice(raw)
	if !ok {
		a.mu.Unlock()
		return
	}

	a.latest[sym] = price
	a.mu.Unlock()

	a.Check(sym)
}

func parseSymbol(raw []byte) string {
	key := []byte(`"s":"`)
	i := bytes.Index(raw, key)
	if i < 0 {
		return ""
	}
	i += len(key)
	j := bytes.IndexByte(raw[i:], '"')
	if j < 0 {
		return ""
	}
	return string(raw[i : i+j])
}

func parsePrice(raw []byte) (float64, bool) {
	key := []byte(`"p":"`)
	i := bytes.Index(raw, key)
	if i < 0 {
		return 0, false
	}
	i += len(key)
	j := bytes.IndexByte(raw[i:], '"')
	if j < 0 {
		return 0, false
	}
	price, err := strconv.ParseFloat(string(raw[i:i+j]), 64)
	if err != nil {
		return 0, false
	}
	return price, true
}

func (a *Arbitrager) Check(symbol string) {
	a.mu.Lock()
	defer a.mu.Unlock()

	indices := a.trianglesByPair[symbol]
	nf := 0.9965 * 0.9965 * 0.9965

	for _, idx := range indices {
		tri := a.Triangles[idx]

		p1, ex1 := a.latest[tri.AB.Symbol]
		p2, ex2 := a.latest[tri.BC.Symbol]
		p3, ex3 := a.latest[tri.CA.Symbol]

		if !ex1 || !ex2 || !ex3 || p1 == 0 || p2 == 0 || p3 == 0 {
			continue
		}

		if tri.AB.Invert {
			p1 = 1 / p1
		}
		if tri.BC.Invert {
			p2 = 1 / p2
		}
		if tri.CA.Invert {
			p3 = 1 / p3
		}

		profit := (p1*p2*p3*nf - 1) * 100
		if profit > 0 && tri.A == "USDT" {
			log.Printf("🔺 ARB %s/%s/%s profit=%.4f%%", tri.A, tri.B, tri.C, profit)
		}
	}
}

