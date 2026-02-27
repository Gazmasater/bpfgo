
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

bpf2go -output-dir . -tags linux \
  -type trace_info -type ls_chunk_t \
  -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



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


1) Подключение и апдейты
ssh root@45.145.5.125
apt update && apt upgrade -y
apt install -y curl git tmux

2) Установка Go (актуальная, системная)

Вариант A (быстро, репозиторий Ubuntu) — для тестов обычно хватает:

apt install -y golang
go version


Вариант B (официальный tar.gz, если нужна свежая 1.23.x)

cd /tmp
curl -OL https://go.dev/dl/go1.23.0.linux-amd64.tar.gz
rm -rf /usr/local/go
tar -C /usr/local -xzf go1.23.0.linux-amd64.tar.gz
echo 'export PATH=$PATH:/usr/local/go/bin' >/etc/profile.d/go.sh
source /etc/profile.d/go.sh
go version

3) Каталог проекта
mkdir -p /opt/arb && cd /opt/arb

Залить код

— Если у тебя есть репозиторий:

git clone https://<твой-репозиторий>.git .


— Если код на локальном ПК:

# выполнить на своём ПК, в папке проекта
scp -r . root@45.145.5.125:/opt/arb

4) Файл .env с ключами
cat > .env << 'EOF'
MEXC_API_KEY=__ВСТАВЬ__
MEXC_SECRET_KEY=__ВСТАВЬ__
# если понадобится OKX:
# OKX_API_KEY=__ВСТАВЬ__
# OKX_SECRET_KEY=__ВСТАВЬ__
# OKX_PASSPHRASE=__ВСТАВЬ__
PORT=8081
READ_TIMEOUT=10
WRITE_TIMEOUT=10
LOG_LEVEL=debug
EOF

5) Запуск в tmux (чтобы не падало при разрыве SSH)
tmux new -s arb
go run .            # или go run ./cmd/cryptarb — как в твоём проекте


— Отсоединиться: Ctrl+B, затем D
— Вернуться: tmux attach -t arb

6) Логи “в файл + на экран” (по желанию)
tmux new -s arb
go run . 2>&1 | tee -a /opt/arb/run.log

7) Доступ к pprof (у тебя включён на 6060)

На своём ПК сделай туннель:

ssh -N -L 6060:127.0.0.1:6060 root@45.145.5.125


Открой в браузере: http://127.0.0.1:6060/debug/pprof/

8) При блокировках (опционально: прокси)
export HTTP_PROXY=http://user:pass@host:port
export HTTPS_PROXY=http://user:pass@host:port
go run .


Готово. Если хочешь — дам минимальный systemd‑юнит, но для тестов tmux + go run максимально быстрый вариант.



func runPublicBookTicker(ctx context.Context, wg *sync.WaitGroup, symbol, interval string, out chan<- string) {
	defer wg.Done()

	const (
		baseRetry = 2 * time.Second
		maxRetry  = 30 * time.Second
	)

	urlWS := "wss://wbs-api.mexc.com/ws" // актуальный публичный WS
	topic := "spot@public.aggre.bookTicker.v3.api.pb@" + interval + "@" + symbol

	retry := baseRetry

	for {
		select {
		case <-ctx.Done():
			return
		default:
		}

		conn, _, err := websocket.DefaultDialer.Dial(urlWS, nil)
		if err != nil {
			log.Printf("[PUB] dial err: %v (retry in %v)", err, retry)
			time.Sleep(retry)
			if retry < maxRetry {
				retry *= 2
				if retry > maxRetry {
					retry = maxRetry
				}
			}
			continue
		}
		log.Printf("[PUB] connected to %s", urlWS)
		retry = baseRetry // сбрасываем бэкофф после успешного коннекта

		// дедлайн для чтения + обработчик PONG с печатью RTT
		_ = conn.SetReadDeadline(time.Now().Add(90 * time.Second))

		var lastPing time.Time
		conn.SetPongHandler(func(appData string) error {
			rtt := time.Since(lastPing)
			log.Printf("[PING] Pong от %s через %v", urlWS, rtt)
			return conn.SetReadDeadline(time.Now().Add(90 * time.Second))
		})

		// keepalive (PING с логом времени отправки)
		stopPing := make(chan struct{})
		go func() {
			t := time.NewTicker(45 * time.Second)
			defer t.Stop()
			for {
				select {
				case <-t.C:
					lastPing = time.Now()
					if err := conn.WriteControl(websocket.PingMessage, []byte("hb"), time.Now().Add(5*time.Second)); err != nil {
						log.Printf("⚠️ [PING] send error: %v", err)
						return
					}
					log.Printf("[PING] Sent at %s", lastPing.Format("15:04:05.000"))
				case <-stopPing:
					return
				}
			}
		}()

		// подписка
		sub := map[string]any{
			"method": "SUBSCRIPTION",
			"params": []string{topic},
			"id":     time.Now().Unix(),
		}
		if err := conn.WriteJSON(sub); err != nil {
			log.Printf("[PUB] subscribe send err: %v", err)
			close(stopPing)
			_ = conn.Close()
			time.Sleep(retry)
			continue
		}
		log.Printf("[PUB] SUB → %s", topic)

		// цикл чтения
		for {
			mt, raw, err := conn.ReadMessage()
			if err != nil {
				log.Printf("[PUB] read err: %v (reconnect)", err)
				break
			}
			switch mt {
			case websocket.TextMessage:
				var v any
				if json.Unmarshal(raw, &v) == nil {
					b, _ := json.MarshalIndent(v, "", "  ")
					log.Printf("[PUB ACK]\n%s", b)
				} else {
					log.Printf("[PUB TEXT] %s", string(raw))
				}
			case websocket.BinaryMessage:
				if sym, mid, ok := parsePBWrapperMid(raw); ok {
					out <- fmt.Sprintf(`{"type":"bookTicker","s":"%s","mid":%.10f}`, sym, mid)
				}
			default:
				// игнорируем другие типы
			}
		}

		// cleanup и реконнект
		close(stopPing)
		_ = conn.Close()
		time.Sleep(retry)
		if retry < maxRetry {
			retry *= 2
			if retry > maxRetry {
				retry = maxRetry
			}
		}
	}
}


Услуга: Lite - Linux SSD
IP: 38.180.106.46
Имя пользователя: root
Пароль: tkP6FoppKo (настоятельно рекомендуем сменить в целях безопасности)


ssh root@38.180.106.46




sed -i 's/bpf_core_read_kernel/bpf_probe_read_kernel/g' /home/lev/bpfgo/trace.c



git fetch origin
git pull --rebase origin ProcNet_monitor
git push origin ProcNet_monitor

3) Как компилировать (gcc)
gcc -O2 -Wall -Wextra -o udp_server udp_server.c
gcc -O2 -Wall -Wextra -o udp_client udp_client.c


dig -x 13.107.253.44 +short
dig -x 151.101.193.91 +short
dig -x 142.251.1.119 +short
sudo ./bpfgo 2>&1 | grep -F -C2 '*(any)'

ncat -u -l 127.0.0.1 9999 >/dev/null
echo -n "ping" | ncat -u -w1 127.0.0.1 9999
curl -I https://github.com --max-time 5

python3 - <<'PY'
import socket
s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("127.0.0.1",9999))
while True:
    data,addr=s.recvfrom(65535)
    s.sendto(data,addr)
PY

echo -n "ping" | nc -u -w1 127.0.0.1 9999

sudo ./bpfgo -resolve=false | stdbuf -oL egrep --line-buffered 'python3|nc'

sudo ./bpfgo -resolve=false | stdbuf -oL egrep --line-buffered 'python3|curl|nc'
python3 -m http.server 18080 --bind 127.0.0.1
curl -s http://127.0.0.1:18080/ >/dev/null



gcc -O2 -Wall -o udp_mmsg_server udp_mmsg_server.c
./udp_mmsg_server

gcc -O2 -Wall -o udp_mmsg_client udp_mmsg_client.c
./udp_mmsg_client

sudo strace -f -e trace=sendmsg,write -p <PID>


sudo ss -tnp | grep "pid=3215"

sudo strace -ff -p 3215 -e trace=write,writev,sendmsg,sendmmsg -s 0 2>&1 \
 | egrep ' (write|writev|sendmsg|sendmmsg)\((22|108|127|109|129|124|107|112),'

 sudo bpftool map show | grep -n "tls_peek"
 sudo bpftool map dump id <ID>



while true; do
  openssl s_client -connect example.com:443 -servername example.com </dev/null >/dev/null 2>&1
done
sudo strace -f -e trace=sendmsg -s 0 openssl s_client -connect example.com:443 -servername example.com </dev/null >/dev/null


sudo bpftool map show | grep -n "tls_peek_map"

sudo bpftool map dump id 188




// === ДОБАВЬ SNI (TLS ClientHello) В ТВОЙ main.go ===
// Ниже — минимальные вставки/правки “по месту”, чтобы из EV_WRITE/EV_SENDMSG/EV_SENDTO/…
// (где ты уже тащишь user payload в ev.Data/ev.Len) вытащить SNI и привязать к flow.
// ВАЖНО: я исхожу из того, что в bpfTraceInfo есть поля Len и Data[] (как в твоём map dump: len=103, data=[...]).
// Если у тебя поле называется иначе (например DataLen / Buf / Payload), просто переименуй в 2 местах:
//   - payloadFromEvent()
//   - appendTLSChunk()

package main

import (
	"bufio"
	"bytes"
	"context"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"io"
	"log"
	"math/bits"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"sync/atomic"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info bpf trace.c -- -I.

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10

	IPPROTO_ICMP   = 1
	IPPROTO_TCP    = 6
	IPPROTO_UDP    = 17
	IPPROTO_ICMPV6 = 58

	EV_SENDTO   = 1
	EV_RECVFROM = 2
	EV_CONNECT  = 3
	EV_ACCEPT   = 4
	EV_BINDOK   = 20
	EV_SENDMSG  = 11
	EV_RECVMSG  = 12
	EV_SENDMMSG = 13
	EV_RECVMMSG = 14
	EV_READ     = 21
	EV_WRITE    = 22
	EV_CLOSE    = 30

	// new: skb-derived L3 hint (real selected src IP)
	EV_SKB_OUT = 40
)

var (
	// perfMB = общий бюджет, делим на CPU + fallback.
	flgPerfMB    = flag.Int("perfMB", 8, "perf buffer total budget in MB (divided per-CPU with fallback)")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")

	flgTTL   = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep = flag.Duration("print", 1*time.Second, "TTL sweep interval + perf-loss rate logging interval")

	flgOnlyPID  = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring (empty=all)")

	flgRW   = flag.Bool("rw", true, "trace read/write on socket fds")
	flgMmsg = flag.Bool("mmsg", true, "trace sendmmsg/recvmmsg")

	// PTR resolve
	flgResolve        = flag.Bool("resolve", true, "reverse DNS resolve IPs (PTR) async")
	flgResolveTTL     = flag.Duration("resolveTTL", 30*time.Minute, "PTR cache TTL (positive)")
	flgResolveNegTTL  = flag.Duration("resolveNegTTL", 5*time.Minute, "PTR negative TTL (NXDOMAIN only)")
	flgResolveWorkers = flag.Int("resolveWorkers", 4, "PTR workers")
	flgResolveTimeout = flag.Duration("resolveTimeout", 2*time.Second, "PTR timeout")
	flgResolveQ       = flag.Int("resolveQ", 4096, "PTR queue size")
	flgHostState      = flag.Bool("hostState", true, "show alias state: pending/miss/no-ptr/skip")

	// known names from /etc/hosts
	flgHostsPrefill = flag.Bool("hostsPrefill", true, "prefill names from /etc/hosts")
	flgHostsFile    = flag.String("hostsFile", "/etc/hosts", "hosts file")
	flgHostsTTL     = flag.Duration("hostsTTL", 24*time.Hour, "hosts prefill TTL")

	// dns cache sweep
	flgResolveSweepEach = flag.Int("resolveSweepEach", 500, "dns cache sweep per tick")
	flgResolvePokeEach  = flag.Int("resolvePokeEach", 256, "poke flows per tick to resolve alias")

	// L3-hint cache ttl + OPEN delay to wait for skb-hint (to avoid *(any))
	flgL3TTL       = flag.Duration("l3ttl", 10*time.Second, "TTL for skb-derived L3 hints (cookie->src/dst)")
	flgOpenDelay   = flag.Duration("openDelay", 200*time.Millisecond, "delay OPEN print (max) to wait for skb hint to fill src ip")
	flgL3SweepEach = flag.Int("l3SweepEach", 500, "l3 hint sweep per tick")

	// === SNI flags (добавлено) ===
	flgSNI         = flag.Bool("sni", true, "parse TLS ClientHello SNI from outgoing payload (best-effort)")
	flgSNITTL      = flag.Duration("sniTTL", 3*time.Second, "TTL for TLS chunk accumulator per cookie")
	flgSNIMaxBytes = flag.Int("sniMax", 8192, "max accumulated TLS bytes per cookie")
)

/* ===== basic helpers ===== */

func commString(c [32]int8) string {
	var b [32]byte
	for i := 0; i < 32; i++ {
		b[i] = byte(c[i])
	}
	n := bytes.IndexByte(b[:], 0)
	if n < 0 {
		n = len(b)
	}
	return string(b[:n])
}

func protoAllowed(p uint8) bool {
	switch p {
	case IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP, IPPROTO_ICMPV6:
		return true
	default:
		return false
	}
}

func protoStr(p uint8) string {
	switch p {
	case IPPROTO_TCP:
		return "TCP"
	case IPPROTO_UDP:
		return "UDP"
	case IPPROTO_ICMP:
		return "ICMP"
	case IPPROTO_ICMPV6:
		return "ICMPv6"
	default:
		return fmt.Sprintf("P%d", p)
	}
}

func isSend(ev uint8) bool {
	return ev == EV_SENDTO || ev == EV_SENDMSG || ev == EV_SENDMMSG || ev == EV_WRITE
}
func isRecv(ev uint8) bool {
	return ev == EV_RECVFROM || ev == EV_RECVMSG || ev == EV_RECVMMSG || ev == EV_READ
}

func isAllZero16(b [16]byte) bool {
	for i := 0; i < 16; i++ {
		if b[i] != 0 {
			return false
		}
	}
	return true
}

// IPv4 u32 from kernel is network-order but looks swapped on little-endian.
func ip4KeyFromU32Net(x uint32) (key [16]byte) {
	var b4 [4]byte
	binary.LittleEndian.PutUint32(b4[:], x)
	copy(key[:4], b4[:])
	return
}

func fmtIPv4FromKey(k [16]byte) string {
	return fmt.Sprintf("%d.%d.%d.%d", k[0], k[1], k[2], k[3])
}

func fmtIPv6Full(b [16]byte) string {
	return fmt.Sprintf("%x:%x:%x:%x:%x:%x:%x:%x",
		uint16(b[0])<<8|uint16(b[1]),
		uint16(b[2])<<8|uint16(b[3]),
		uint16(b[4])<<8|uint16(b[5]),
		uint16(b[6])<<8|uint16(b[7]),
		uint16(b[8])<<8|uint16(b[9]),
		uint16(b[10])<<8|uint16(b[11]),
		uint16(b[12])<<8|uint16(b[13]),
		uint16(b[14])<<8|uint16(b[15]),
	)
}

func isIPv6LinkLocalUnicast(ip [16]byte) bool { return ip[0] == 0xfe && (ip[1]&0xc0) == 0x80 }
func isIPv6LinkLocalMulticast(ip [16]byte) bool {
	return ip[0] == 0xff && (ip[1]&0x0f) == 0x02
}
func needsScope6(ip [16]byte) bool { return isIPv6LinkLocalUnicast(ip) || isIPv6LinkLocalMulticast(ip) }

func isIPv6Loopback(ip [16]byte) bool {
	for i := 0; i < 15; i++ {
		if ip[i] != 0 {
			return false
		}
	}
	return ip[15] == 1 // ::1
}

type ifResolver struct {
	mu sync.Mutex
	m  map[uint32]string
}

func (r *ifResolver) name(ifidx uint32) string {
	if ifidx == 0 {
		return ""
	}
	r.mu.Lock()
	defer r.mu.Unlock()
	if r.m == nil {
		r.m = make(map[uint32]string, 32)
	}
	if s, ok := r.m[ifidx]; ok {
		return s
	}
	ifi, err := net.InterfaceByIndex(int(ifidx))
	if err != nil || ifi == nil || ifi.Name == "" {
		s := fmt.Sprintf("if%d", ifidx)
		r.m[ifidx] = s
		return s
	}
	r.m[ifidx] = ifi.Name
	return ifi.Name
}

var ifr ifResolver

func srcKeyFromEvent(ev bpfTraceInfo) (k [16]byte) {
	if uint16(ev.Family) == AF_INET {
		return ip4KeyFromU32Net(ev.SrcIp4)
	}
	copy(k[:], ev.SrcIp6[:])
	return
}
func dstKeyFromEvent(ev bpfTraceInfo) (k [16]byte) {
	if uint16(ev.Family) == AF_INET {
		return ip4KeyFromU32Net(ev.DstIp4)
	}
	copy(k[:], ev.DstIp6[:])
	return
}

func srcScopeFromEvent(ev bpfTraceInfo) uint32 {
	if uint16(ev.Family) == AF_INET6 {
		return uint32(ev.SrcScope)
	}
	return 0
}
func dstScopeFromEvent(ev bpfTraceInfo) uint32 {
	if uint16(ev.Family) == AF_INET6 {
		return uint32(ev.DstScope)
	}
	return 0
}

/* ===== SNI parser (добавлено) ===== */

// tlsAcc — накопитель кусочков TLS для одного cookie.
type tlsAcc struct {
	buf  []byte
	last time.Time
}

// payloadFromEvent вытаскивает байты из bpfTraceInfo.
// ПЕРЕИМЕНУЙ поля, если у тебя не Len/Data.
func payloadFromEvent(ev bpfTraceInfo) []byte {
	// ожидается: ev.Len и ev.Data
	n := int(ev.Len)
	if n <= 0 {
		return nil
	}
	// Data обычно фиксированный массив
	max := len(ev.Data)
	if n > max {
		n = max
	}
	out := make([]byte, n)
	copy(out, ev.Data[:n])
	return out
}

func isLikelyTLSClientHelloPrefix(b []byte) bool {
	// TLS record header: ContentType(1)=22, Version(2)=0x0301..0x0304, Length(2)
	if len(b) < 5 {
		return false
	}
	if b[0] != 0x16 { // handshake
		return false
	}
	// version check: 0x03 0x01..0x04 (TLS1.0..1.3 record layer)
	if b[1] != 0x03 {
		return false
	}
	if b[2] < 0x01 || b[2] > 0x04 {
		return false
	}
	return true
}

// tryParseSNI из буфера (который может содержать несколько records) пытается достать SNI.
func tryParseSNI(buf []byte) (string, bool) {
	// Сканируем TLS records.
	i := 0
	for {
		if len(buf)-i < 5 {
			return "", false
		}
		ct := buf[i]
		verMaj := buf[i+1]
		verMin := buf[i+2]
		if ct != 0x16 || verMaj != 0x03 || verMin < 0x01 || verMin > 0x04 {
			// не похоже на TLS handshake record — пробуем сдвиг на 1 (best-effort)
			i++
			continue
		}
		recLen := int(binary.BigEndian.Uint16(buf[i+3 : i+5]))
		if recLen <= 0 || recLen > 1<<14+2048 { // разумный верх
			i++
			continue
		}
		if len(buf)-i < 5+recLen {
			return "", false // ждём ещё
		}
		rec := buf[i+5 : i+5+recLen]
		// Внутри может быть несколько handshake сообщений.
		j := 0
		for {
			if len(rec)-j < 4 {
				break
			}
			hsType := rec[j]
			hsLen := int(rec[j+1])<<16 | int(rec[j+2])<<8 | int(rec[j+3])
			if hsLen < 0 || hsLen > len(rec)-(j+4) {
				break
			}
			hs := rec[j+4 : j+4+hsLen]
			if hsType == 0x01 { // ClientHello
				if s, ok := parseClientHelloSNI(hs); ok {
					return s, true
				}
			}
			j += 4 + hsLen
		}
		i += 5 + recLen
		if i >= len(buf) {
			return "", false
		}
	}
}

// parseClientHelloSNI парсит тело ClientHello (без handshake header).
func parseClientHelloSNI(ch []byte) (string, bool) {
	// struct:
	// client_version(2), random(32),
	// session_id_len(1), session_id,
	// cipher_suites_len(2), cipher_suites,
	// comp_methods_len(1), comp_methods,
	// extensions_len(2), extensions...
	if len(ch) < 2+32+1 {
		return "", false
	}
	p := 0
	p += 2 + 32
	sidLen := int(ch[p])
	p++
	if len(ch)-p < sidLen+2 {
		return "", false
	}
	p += sidLen
	if len(ch)-p < 2 {
		return "", false
	}
	csLen := int(binary.BigEndian.Uint16(ch[p : p+2]))
	p += 2
	if csLen < 2 || len(ch)-p < csLen+1 {
		return "", false
	}
	p += csLen
	cmLen := int(ch[p])
	p++
	if len(ch)-p < cmLen+2 {
		return "", false
	}
	p += cmLen
	extLen := int(binary.BigEndian.Uint16(ch[p : p+2]))
	p += 2
	if extLen <= 0 || len(ch)-p < extLen {
		return "", false
	}
	exts := ch[p : p+extLen]

	// iterate extensions: type(2), len(2), data...
	q := 0
	for {
		if len(exts)-q < 4 {
			return "", false
		}
		typ := binary.BigEndian.Uint16(exts[q : q+2])
		l := int(binary.BigEndian.Uint16(exts[q+2 : q+4]))
		q += 4
		if l < 0 || len(exts)-q < l {
			return "", false
		}
		data := exts[q : q+l]
		q += l

		if typ != 0x0000 { // server_name
			continue
		}
		// server_name extension:
		// list_len(2), [ name_type(1), name_len(2), name... ]...
		if len(data) < 2 {
			return "", false
		}
		listLen := int(binary.BigEndian.Uint16(data[0:2]))
		if listLen <= 0 || len(data)-2 < listLen {
			return "", false
		}
		snl := data[2 : 2+listLen]
		r := 0
		for {
			if len(snl)-r < 3 {
				break
			}
			nameType := snl[r]
			nameLen := int(binary.BigEndian.Uint16(snl[r+1 : r+3]))
			r += 3
			if nameLen <= 0 || len(snl)-r < nameLen {
				break
			}
			name := snl[r : r+nameLen]
			r += nameLen
			if nameType == 0 { // host_name
				// минимальная валидация
				s := strings.TrimSpace(string(name))
				if s != "" && len(s) <= 253 && strings.IndexByte(s, 0) < 0 {
					return s, true
				}
			}
		}
		return "", false
	}
}

// appendTLSChunk добавляет кусок и пытается распарсить SNI.
// Возвращает sni и ok=true, если получилось.
func appendTLSChunk(acc *tlsAcc, chunk []byte, maxBytes int) (string, bool) {
	if len(chunk) == 0 {
		return "", false
	}
	// Быстрый фильтр: если буфер пустой, а начало не TLS — не накапливаем (чтобы не засорять)
	if len(acc.buf) == 0 && !isLikelyTLSClientHelloPrefix(chunk) {
		return "", false
	}
	acc.buf = append(acc.buf, chunk...)
	if maxBytes > 0 && len(acc.buf) > maxBytes {
		// оставим хвост, но обычно ClientHello в начале — поэтому лучше просто сбросить
		acc.buf = nil
		return "", false
	}
	return tryParseSNI(acc.buf)
}

/* ===== .env loader (no deps) ===== */

// ... (твой код без изменений)

/* ===== FLOW ===== */

type FlowKey struct {
	TGID   uint32
	Cookie uint64
	Proto  uint8
	Family uint16

	PeerMode uint8
	Rport    uint16
	Remote   [16]byte
	Rscope   uint32
}

type Flow struct {
	Key  FlowKey
	Comm string

	Local      [16]byte
	Lport      uint16
	LocalScope uint32

	Remote      [16]byte
	Rport       uint16
	RemoteScope uint32

	FirstSeen time.Time
	LastSeen  time.Time

	InBytes  uint64
	OutBytes uint64
	InPkts   uint64
	OutPkts  uint64

	OpenedPrinted bool
	GenStart      uint64

	RemoteHost string // per-flow cache for real resolved name (not miss/pending/no-ptr)

	// === SNI (добавлено) ===
	SNI string
}

// ... (твой код без изменений)

func printOpen(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	sni := ""
	if f.SNI != "" {
		sni = " sni=" + f.SNI
	}

	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		incompleteNote(f),
		maybeLostNote(f),
		sni,
	)
}

func printClose(f *Flow, reason string) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	sni := ""
	if f.SNI != "" {
		sni = " sni=" + f.SNI
	}

	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		incompleteNote(f),
		maybeLostNote(f),
		sni,
	)
}

/* ===== perf reader ... (твой код) ===== */

func main() {
	// ... (твой код до создания maps/flows без изменений)

	flows := make(map[FlowKey]*Flow, 8192)
	l3ByCookie := make(map[uint64]l3Info, 8192)

	// === SNI acc map (добавлено) ===
	tlsByCookie := make(map[uint64]*tlsAcc, 8192)

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	log.Println("OPEN/CLOSE (TCP/UDP/ICMP) + PTR + skb-hint. Ctrl+C to exit")

	shouldKeep := func(pid uint32, comm string) bool {
		if comm == "" || comm == selfName {
			return false
		}
		if *flgOnlyPID != 0 && int(pid) != *flgOnlyPID {
			return false
		}
		if *flgOnlyComm != "" && !strings.Contains(comm, *flgOnlyComm) {
			return false
		}
		return true
	}

	// ... (твои функции upgradeKeyIfNeeded/closeByCookie без изменений)

	lastLost := uint64(0)
	lastTick := time.Now()

	for {
		select {
		case <-stop:
			_ = rd.Close()
			log.Printf("PERF_LOST_TOTAL total=%d gen=%d", atomic.LoadUint64(&lostTotal), atomic.LoadUint64(&lostGen))
			for _, f := range flows {
				if dropZeroFlow(f) {
					continue
				}
				if f.OpenedPrinted {
					printClose(f, "signal")
				}
			}
			log.Println("Exiting...")
			return

		case <-ticker.C:
			now := time.Now()

			// ... (твой ticker код без изменений: PERF_LOST_RATE, dns sweep, l3 sweep, poke flows, TTL sweep)

			// === sweep TLS accumulators (добавлено) ===
			if *flgSNI && *flgSNITTL > 0 {
				n := 0
				for c, a := range tlsByCookie {
					if now.Sub(a.last) > *flgSNITTL {
						delete(tlsByCookie, c)
						n++
						if n >= 500 {
							break
						}
					}
				}
			}

		case w, ok := <-evCh:
			if !ok {
				return
			}

			ev := w.ev
			evt := uint8(ev.Event)
			proto := uint8(ev.Proto)
			family := uint16(ev.Family)

			// handle skb-out hint first (no printing, no comm filtering)
			if evt == EV_SKB_OUT {
				if !protoAllowed(proto) || (family != AF_INET && family != AF_INET6) || ev.Cookie == 0 {
					continue
				}
				h := l3Info{
					Family: family,
					Proto:  proto,
					Src:    srcKeyFromEvent(ev),
					Sport:  uint16(ev.Sport),
					SrcSc:  srcScopeFromEvent(ev),
					Dst:    dstKeyFromEvent(ev),
					Dport:  uint16(ev.Dport),
					DstSc:  dstScopeFromEvent(ev),
					Seen:   w.now,
				}
				l3ByCookie[ev.Cookie] = h
				for _, f := range flows {
					if f.Key.Cookie == ev.Cookie {
						applyL3HintToFlow(f, h)
					}
				}
				continue
			}

			comm := commString(ev.Comm)
			if !shouldKeep(ev.Tgid, comm) {
				continue
			}

			if !protoAllowed(proto) {
				continue
			}
			if family != AF_INET && family != AF_INET6 {
				continue
			}

			if evt == EV_CLOSE {
				closeByCookie(ev.Tgid, ev.Cookie, "close()")
				// на всякий: почистим накопитель
				if *flgSNI && ev.Cookie != 0 {
					delete(tlsByCookie, ev.Cookie)
				}
				continue
			}

			key := makeKey(ev)
			key, upgraded := upgradeKeyIfNeeded(key)

			f := flows[key]
			if f == nil {
				if upgraded != nil {
					f = upgraded
				} else {
					f = &Flow{
						Key:       key,
						Comm:      comm,
						FirstSeen: w.now,
						LastSeen:  w.now,
						GenStart:  atomic.LoadUint64(&lostGen),
					}
					flows[key] = f
				}
			}

			f.LastSeen = w.now
			applyEndpoints(f, ev)

			// try fill local ip from skb hints
			if h, ok := l3ByCookie[f.Key.Cookie]; ok {
				applyL3HintToFlow(f, h)
			}

			// === SNI extraction (добавлено) ===
			// Пытаемся только на TCP и только для “выходящих” payload-событий,
			// где реально есть user bytes (обычно WRITE/SENDMSG/SENDTO/SENDMMSG).
			if *flgSNI && f.SNI == "" && f.Key.Proto == IPPROTO_TCP && ev.Cookie != 0 && isSend(evt) {
				// чтобы не парсить всё подряд, ограничимся 443 (или если порт пока не известен — попробуем по ev.Dport)
				dport := uint16(ev.Dport)
				// если flow уже знает remote port — используем его
				if f.Rport != 0 {
					dport = f.Rport
				}
				if dport == 443 {
					chunk := payloadFromEvent(ev)
					if len(chunk) != 0 {
						acc := tlsByCookie[ev.Cookie]
						if acc == nil {
							acc = &tlsAcc{}
							tlsByCookie[ev.Cookie] = acc
						}
						acc.last = w.now
						if sni, ok := appendTLSChunk(acc, chunk, *flgSNIMaxBytes); ok {
							f.SNI = sni
							delete(tlsByCookie, ev.Cookie) // больше не нужно
						}
					}
				}
			}

			// accounting
			switch evt {
			case EV_SENDMMSG:
				if ev.Ret > 0 {
					f.OutBytes += uint64(ev.Ret)
				}
				if ev.State > 0 {
					f.OutPkts += uint64(ev.State)
				} else {
					f.OutPkts++
				}
			case EV_RECVMMSG:
				if ev.Ret > 0 {
					f.InBytes += uint64(ev.Ret)
				}
				if ev.State > 0 {
					f.InPkts += uint64(ev.State)
				} else {
					f.InPkts++
				}
			default:
				if isSend(evt) && ev.Ret > 0 {
					f.OutBytes += uint64(ev.Ret)
					f.OutPkts++
				} else if isRecv(evt) && ev.Ret > 0 {
					f.InBytes += uint64(ev.Ret)
					f.InPkts++
				}
			}

			if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
				printOpen(f)
				f.OpenedPrinted = true
			}
		}
	}
}
