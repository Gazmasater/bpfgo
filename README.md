
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





package main

import (
	"bytes"
	"context"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
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

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info -type tls_chunk_t bpf trace.c -- -I.

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

	EV_SKB_OUT   = 40
	EV_TLS_CHUNK = 50
)

var (
	flgPerfMB    = flag.Int("perfMB", 8, "perf buffer total budget in MB (per-cpu split)")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")

	flgTTL   = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep = flag.Duration("print", 1*time.Second, "sweep interval + perf-loss rate logging")

	flgOnlyPID  = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring")

	flgRW   = flag.Bool("rw", true, "trace read/write on socket fds")
	flgMmsg = flag.Bool("mmsg", true, "trace sendmmsg/recvmmsg")

	// L3 hint (skb)
	flgL3TTL       = flag.Duration("l3ttl", 10*time.Second, "TTL for skb-derived L3 hints (cookie->src/dst)")
	flgOpenDelay   = flag.Duration("openDelay", 200*time.Millisecond, "delay OPEN print to wait for skb hint")
	flgL3SweepEach = flag.Int("l3SweepEach", 500, "l3 hint sweep per tick")

	// TLS/SNI
	flgSniTTL     = flag.Duration("sniTTL", 30*time.Second, "TTL for SNI cache per cookie")
	flgTlsAccTTL  = flag.Duration("tlsAccTTL", 5*time.Second, "TTL for TLS accumulator per cookie")
	flgTlsAccMax  = flag.Int("tlsAccMax", 4096, "max bytes kept per cookie for TLS ClientHello parse")
	flgTlsPerfMB  = flag.Int("tlsPerfMB", 2, "perf buffer MB for tls_events")
	flgTlsPorts   = flag.String("tlsPorts", "443,853", "ports considered TLS (comma-separated)")
	flgPrintClose = flag.Bool("printClose", true, "print CLOSE lines")
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

// kernel u32 is network-order; in your pipeline you used LittleEndian to show as dotted.
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

/* ===== perf reader (total budget -> per-cpu split) ===== */

func openPerfReaderTotalBudget(events *ebpf.Map, totalMB int) (*perf.Reader, int, error) {
	totalBytes := totalMB * 1024 * 1024
	if totalBytes < 256*1024 {
		totalBytes = 256 * 1024
	}
	nCPU := runtime.NumCPU()
	perCPU := totalBytes / nCPU
	if perCPU < 256*1024 {
		perCPU = 256 * 1024
	}
	page := os.Getpagesize()
	pages := perCPU / page
	if pages < 8 {
		pages = 8
	}
	p2 := 1 << (bits.Len(uint(pages)) - 1)
	try := p2 * page

	var rd *perf.Reader
	var err error
	for try >= 256*1024 {
		rd, err = perf.NewReader(events, try)
		if err == nil {
			return rd, try, nil
		}
		if strings.Contains(err.Error(), "cannot allocate memory") || strings.Contains(err.Error(), "can't mmap") {
			try /= 2
			continue
		}
		break
	}
	return nil, 0, err
}

/* ===== TLS SNI ===== */

type sniEntry struct {
	Name string
	Seen time.Time
}

type tlsAcc struct {
	Buf  []byte
	Seen time.Time
}

func appendClamped(dst []byte, src []byte, max int) []byte {
	if len(src) == 0 || max <= 0 {
		return dst
	}
	need := len(dst) + len(src)
	if need <= max {
		return append(dst, src...)
	}
	over := need - max
	if over >= len(dst) {
		// всё старое выкинули, оставим хвост src
		dst = dst[:0]
		if over >= len(src) {
			return dst
		}
		return append(dst, src[over:]...)
	}
	copy(dst, dst[over:])
	dst = dst[:len(dst)-over]
	return append(dst, src...)
}

// parseSNIFromTLS expects a FULL TLS record containing ClientHello.
// If buffer doesn't contain full record/handshake — returns false (need more data).
func parseSNIFromTLS(b []byte) (string, bool) {
	if len(b) < 5 {
		return "", false
	}
	if b[0] != 0x16 || b[1] != 0x03 { // handshake record, TLS major=3
		return "", false
	}
	recLen := int(b[3])<<8 | int(b[4])
	if recLen <= 0 || 5+recLen > len(b) {
		return "", false
	}
	p := b[5 : 5+recLen]
	if len(p) < 4 || p[0] != 0x01 { // client_hello
		return "", false
	}
	hsLen := int(p[1])<<16 | int(p[2])<<8 | int(p[3])
	if hsLen <= 0 || 4+hsLen > len(p) {
		return "", false
	}
	ch := p[4 : 4+hsLen]

	// version(2)+random(32)+sidLen(1)
	if len(ch) < 2+32+1 {
		return "", false
	}
	off := 0
	off += 2
	off += 32
	sidLen := int(ch[off])
	off++
	if off+sidLen > len(ch) {
		return "", false
	}
	off += sidLen

	if off+2 > len(ch) {
		return "", false
	}
	csLen := int(ch[off])<<8 | int(ch[off+1])
	off += 2
	if off+csLen > len(ch) {
		return "", false
	}
	off += csLen

	if off+1 > len(ch) {
		return "", false
	}
	compLen := int(ch[off])
	off++
	if off+compLen > len(ch) {
		return "", false
	}
	off += compLen

	if off+2 > len(ch) {
		return "", false
	}
	extLen := int(ch[off])<<8 | int(ch[off+1])
	off += 2
	if off+extLen > len(ch) {
		return "", false
	}

	exts := ch[off : off+extLen]
	i := 0
	for i+4 <= len(exts) {
		et := int(exts[i])<<8 | int(exts[i+1])
		el := int(exts[i+2])<<8 | int(exts[i+3])
		i += 4
		if i+el > len(exts) {
			return "", false
		}
		ed := exts[i : i+el]
		i += el

		if et != 0x0000 { // server_name
			continue
		}
		if len(ed) < 2 {
			return "", false
		}
		listLen := int(ed[0])<<8 | int(ed[1])
		ed = ed[2:]
		if listLen > len(ed) {
			return "", false
		}
		j := 0
		for j+3 <= listLen {
			nameType := ed[j]
			nameLen := int(ed[j+1])<<8 | int(ed[j+2])
			j += 3
			if j+nameLen > listLen {
				return "", false
			}
			if nameType == 0 && nameLen > 0 {
				return string(ed[j : j+nameLen]), true
			}
			j += nameLen
		}
		return "", false
	}
	return "", false
}

/* ===== L3 hint cache (cookie -> real src/dst) ===== */

type l3Info struct {
	Family uint16
	Proto  uint8

	Src   [16]byte
	Sport uint16
	SrcSc uint32

	Dst   [16]byte
	Dport uint16
	DstSc uint32

	Seen time.Time
}

func (l l3Info) expired(now time.Time, ttl time.Duration) bool {
	return now.Sub(l.Seen) > ttl
}

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

	TLSSNI string
}

func makeKey(ev bpfTraceInfo) FlowKey {
	k := FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}
	evt := uint8(ev.Event)

	// keep your “udp/icmphard peer key” logic
	if k.Proto == IPPROTO_UDP || k.Proto == IPPROTO_ICMP || k.Proto == IPPROTO_ICMPV6 {
		var remote [16]byte
		var rport uint16
		var rscope uint32
		if isRecv(evt) {
			remote = srcKeyFromEvent(ev)
			rport = uint16(ev.Sport)
			rscope = srcScopeFromEvent(ev)
		} else {
			remote = dstKeyFromEvent(ev)
			rport = uint16(ev.Dport)
			rscope = dstScopeFromEvent(ev)
		}
		if !isAllZero16(remote) {
			k.PeerMode = 1
			k.Remote = remote
			if k.Proto == IPPROTO_UDP && rport != 0 {
				k.Rport = rport
			}
			if k.Family == AF_INET6 && needsScope6(remote) && rscope != 0 {
				k.Rscope = rscope
			}
		}
	}
	return k
}

func applyEndpoints(f *Flow, ev bpfTraceInfo) {
	evt := uint8(ev.Event)

	var localIP, remoteIP [16]byte
	var lport, rport uint16
	var localScope, remoteScope uint32

	switch {
	case isSend(evt) || evt == EV_CONNECT:
		localIP = srcKeyFromEvent(ev)
		remoteIP = dstKeyFromEvent(ev)
		lport = uint16(ev.Sport)
		rport = uint16(ev.Dport)
		localScope = srcScopeFromEvent(ev)
		remoteScope = dstScopeFromEvent(ev)

	case isRecv(evt):
		localIP = dstKeyFromEvent(ev)
		remoteIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)
		localScope = dstScopeFromEvent(ev)
		remoteScope = srcScopeFromEvent(ev)

	case evt == EV_BINDOK:
		localIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Sport)
		localScope = srcScopeFromEvent(ev)

	case evt == EV_ACCEPT:
		localIP = dstKeyFromEvent(ev)
		remoteIP = srcKeyFromEvent(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)
		localScope = dstScopeFromEvent(ev)
		remoteScope = srcScopeFromEvent(ev)
	}

	if f.Lport == 0 && lport != 0 {
		f.Lport = lport
	}
	if isAllZero16(f.Local) && !isAllZero16(localIP) {
		f.Local = localIP
	}
	if f.LocalScope == 0 && localScope != 0 && needsScope6(localIP) {
		f.LocalScope = localScope
	}

	if f.Rport == 0 && rport != 0 {
		f.Rport = rport
	}
	if isAllZero16(f.Remote) && !isAllZero16(remoteIP) {
		f.Remote = remoteIP
	}
	if f.RemoteScope == 0 && remoteScope != 0 && needsScope6(remoteIP) {
		f.RemoteScope = remoteScope
	}
}

func applyL3HintToFlow(f *Flow, h l3Info) {
	if h.Family != f.Key.Family || h.Proto != f.Key.Proto {
		return
	}

	// try match by remote when known
	if !isAllZero16(f.Remote) {
		// send: remote==dst
		if f.Rport != 0 && f.Rport == h.Dport && bytes.Equal(f.Remote[:], h.Dst[:]) {
			if isAllZero16(f.Local) && !isAllZero16(h.Src) {
				f.Local = h.Src
				f.LocalScope = h.SrcSc
			}
			if f.Lport == 0 && h.Sport != 0 {
				f.Lport = h.Sport
			}
			return
		}
		// recv: remote==src
		if f.Rport != 0 && f.Rport == h.Sport && bytes.Equal(f.Remote[:], h.Src[:]) {
			if isAllZero16(f.Local) && !isAllZero16(h.Dst) {
				f.Local = h.Dst
				f.LocalScope = h.DstSc
			}
			if f.Lport == 0 && h.Dport != 0 {
				f.Lport = h.Dport
			}
			return
		}
	}

	// best-effort fill local from src
	if isAllZero16(f.Local) && !isAllZero16(h.Src) {
		f.Local = h.Src
		f.LocalScope = h.SrcSc
		if f.Lport == 0 && h.Sport != 0 {
			f.Lport = h.Sport
		}
	}
}

func flowReadyToOpenBase(f *Flow) bool {
	if isAllZero16(f.Remote) {
		return false
	}
	switch f.Key.Proto {
	case IPPROTO_TCP, IPPROTO_UDP:
		return f.Lport != 0 && f.Rport != 0
	default:
		return true
	}
}

func flowReadyToPrintOpen(f *Flow) bool {
	if !flowReadyToOpenBase(f) {
		return false
	}
	if isAllZero16(f.Local) && time.Since(f.FirstSeen) < *flgOpenDelay {
		return false
	}
	return true
}

func addrStr(family uint16, ip [16]byte, scope uint32) string {
	if isAllZero16(ip) {
		return "*"
	}
	if family == AF_INET6 {
		s := fmtIPv6Full(ip)
		if needsScope6(ip) && scope != 0 {
			s += "%" + ifr.name(scope)
		}
		return s
	}
	return fmtIPv4FromKey(ip)
}

// Здесь можешь воткнуть твой aliasForIP/PTR cache.
// Сейчас: простая заглушка.
func aliasForIP(_ uint16, _ [16]byte) string { return "?" }

func fmtEndpointAll(family uint16, ip [16]byte, port uint16, scope uint32, proto uint8, alias string) string {
	isICMP := proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6
	a := addrStr(family, ip, scope)
	if alias == "" {
		alias = "?"
	}
	if isICMP {
		return fmt.Sprintf("%s(%s)", a, alias)
	}
	if family == AF_INET6 && !isAllZero16(ip) {
		return fmt.Sprintf("[%s](%s):%d", a, alias, port)
	}
	return fmt.Sprintf("%s(%s):%d", a, alias, port)
}

func printOpen(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := aliasForIP(f.Key.Family, f.Remote)

	sni := ""
	if f.TLSSNI != "" {
		sni = " sni=" + f.TLSSNI
	}

	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		sni,
	)
}

func printClose(f *Flow, reason string) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := aliasForIP(f.Key.Family, f.Remote)

	sni := ""
	if f.TLSSNI != "" {
		sni = " sni=" + f.TLSSNI
	}

	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		sni,
	)
}

/* ===== Engine ===== */

type Config struct {
	SelfName string
	TLSPorts map[uint16]struct{}
}

type Engine struct {
	cfg Config

	flows      map[FlowKey]*Flow
	l3ByCookie map[uint64]l3Info

	sniByCookie    map[uint64]sniEntry
	tlsAccByCookie map[uint64]*tlsAcc

	lostTotal uint64
	lostGen   uint64

	lastLost uint64
	lastTick time.Time
}

func parseTLSPorts(s string) map[uint16]struct{} {
	m := make(map[uint16]struct{}, 8)
	for _, part := range strings.Split(s, ",") {
		part = strings.TrimSpace(part)
		if part == "" {
			continue
		}
		var p int
		_, _ = fmt.Sscanf(part, "%d", &p)
		if p > 0 && p < 65536 {
			m[uint16(p)] = struct{}{}
		}
	}
	// дефолт
	if len(m) == 0 {
		m[443] = struct{}{}
		m[853] = struct{}{}
	}
	return m
}

func NewEngine(self string) *Engine {
	return &Engine{
		cfg: Config{
			SelfName: self,
			TLSPorts: parseTLSPorts(*flgTlsPorts),
		},
		flows:          make(map[FlowKey]*Flow, 8192),
		l3ByCookie:     make(map[uint64]l3Info, 8192),
		sniByCookie:    make(map[uint64]sniEntry, 8192),
		tlsAccByCookie: make(map[uint64]*tlsAcc, 8192),
		lastTick:       time.Now(),
	}
}

func (e *Engine) shouldKeep(pid uint32, comm string) bool {
	if comm == "" || comm == e.cfg.SelfName {
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

func (e *Engine) onTLSEvent(ev bpfTlsChunkT, now time.Time) {
	if ev.Cookie == 0 || ev.Len == 0 {
		return
	}

	ln := int(ev.Len)
	if ln <= 0 {
		return
	}
	if ln > len(ev.Data) {
		ln = len(ev.Data)
	}
	chunk := ev.Data[:ln]

	// accumulator by cookie
	acc := e.tlsAccByCookie[ev.Cookie]
	if acc == nil {
		acc = &tlsAcc{Buf: make([]byte, 0, 512)}
		e.tlsAccByCookie[ev.Cookie] = acc
	}
	acc.Seen = now
	acc.Buf = appendClamped(acc.Buf, chunk, *flgTlsAccMax)

	if host, ok := parseSNIFromTLS(acc.Buf); ok && host != "" {
		e.sniByCookie[ev.Cookie] = sniEntry{Name: host, Seen: now}
		delete(e.tlsAccByCookie, ev.Cookie)

		// apply to existing flows
		for _, f := range e.flows {
			if f.Key.Cookie == ev.Cookie && f.Key.Proto == IPPROTO_TCP {
				f.TLSSNI = host
			}
		}
	}
}

func (e *Engine) closeByCookie(tgid uint32, cookie uint64, reason string) {
	for k, f := range e.flows {
		if k.TGID == tgid && k.Cookie == cookie {
			// try l3 fill before close
			if h, ok := e.l3ByCookie[cookie]; ok {
				applyL3HintToFlow(f, h)
			}
			if f.OpenedPrinted && *flgPrintClose {
				printClose(f, reason)
			}
			delete(e.flows, k)
		}
	}
}

func (e *Engine) onTraceEvent(ev bpfTraceInfo, now time.Time) {
	evt := uint8(ev.Event)
	proto := uint8(ev.Proto)
	family := uint16(ev.Family)

	// skb-out hint
	if evt == EV_SKB_OUT {
		if !protoAllowed(proto) || (family != AF_INET && family != AF_INET6) || ev.Cookie == 0 {
			return
		}
		h := l3Info{
			Family: family, Proto: proto,
			Src:   srcKeyFromEvent(ev), Sport: uint16(ev.Sport), SrcSc: srcScopeFromEvent(ev),
			Dst:   dstKeyFromEvent(ev), Dport: uint16(ev.Dport), DstSc: dstScopeFromEvent(ev),
			Seen: now,
		}
		e.l3ByCookie[ev.Cookie] = h
		for _, f := range e.flows {
			if f.Key.Cookie == ev.Cookie {
				applyL3HintToFlow(f, h)
			}
		}
		return
	}

	comm := commString(ev.Comm)
	if !e.shouldKeep(ev.Tgid, comm) {
		return
	}

	if !protoAllowed(proto) {
		return
	}
	if family != AF_INET && family != AF_INET6 {
		return
	}

	if evt == EV_CLOSE {
		e.closeByCookie(ev.Tgid, ev.Cookie, "close()")
		return
	}

	key := makeKey(ev)
	f := e.flows[key]
	if f == nil {
		f = &Flow{
			Key:       key,
			Comm:      comm,
			FirstSeen: now,
			LastSeen:  now,
			GenStart:  atomic.LoadUint64(&e.lostGen),
		}
		// apply cached SNI early if already known
		if se, ok := e.sniByCookie[f.Key.Cookie]; ok {
			f.TLSSNI = se.Name
		}
		e.flows[key] = f
	}

	f.LastSeen = now
	applyEndpoints(f, ev)

	// apply l3 hints if exist
	if h, ok := e.l3ByCookie[f.Key.Cookie]; ok {
		applyL3HintToFlow(f, h)
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

func (e *Engine) onTick(now time.Time) {
	// perf lost rate
	total := atomic.LoadUint64(&e.lostTotal)
	delta := total - e.lastLost
	dt := now.Sub(e.lastTick)
	if delta > 0 {
		log.Printf("PERF_LOST_RATE lost=%d in=%s total=%d gen=%d flows=%d",
			delta, dt.Truncate(time.Millisecond),
			total, atomic.LoadUint64(&e.lostGen),
			len(e.flows),
		)
	}
	e.lastLost = total
	e.lastTick = now

	// sweep L3 hints
	if *flgL3SweepEach > 0 {
		n := 0
		for c, h := range e.l3ByCookie {
			if h.expired(now, *flgL3TTL) {
				delete(e.l3ByCookie, c)
				n++
				if n >= *flgL3SweepEach {
					break
				}
			}
		}
	}

	// sweep TLS accumulators + SNI
	n := 0
	for c, a := range e.tlsAccByCookie {
		if now.Sub(a.Seen) > *flgTlsAccTTL {
			delete(e.tlsAccByCookie, c)
			n++
			if n >= 2000 {
				break
			}
		}
	}
	n = 0
	for c, s := range e.sniByCookie {
		if now.Sub(s.Seen) > *flgSniTTL {
			delete(e.sniByCookie, c)
			n++
			if n >= 2000 {
				break
			}
		}
	}

	// TTL sweep flows
	for k, f := range e.flows {
		if now.Sub(f.LastSeen) > *flgTTL {
			if h, ok := e.l3ByCookie[f.Key.Cookie]; ok {
				applyL3HintToFlow(f, h)
			}
			if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
				printOpen(f)
				f.OpenedPrinted = true
			}
			if f.OpenedPrinted && *flgPrintClose {
				printClose(f, "idle")
			}
			delete(e.flows, k)
		}
	}
}

/* ===== attach helpers ===== */

func attachTracepoint(links *[]link.Link, cat, name string, prog *ebpf.Program) {
	l, err := link.Tracepoint(cat, name, prog, nil)
	if err != nil {
		log.Fatalf("attach %s/%s: %v", cat, name, err)
	}
	*links = append(*links, l)
}

/* ===== main ===== */

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
	defer objs.Close()

	if *flgPprof {
		go func() {
			log.Printf("pprof on %s", *flgPprofAddr)
			_ = http.ListenAndServe(*flgPprofAddr, nil)
		}()
	}

	selfName := filepath.Base(os.Args[0])
	engine := NewEngine(selfName)

	var links []link.Link
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()

	// core syscalls
	attachTracepoint(&links, "syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_bind", objs.TraceBindExit)

	attachTracepoint(&links, "syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attachTracepoint(&links, "syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attachTracepoint(&links, "syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attachTracepoint(&links, "syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attachTracepoint(&links, "syscalls", "sys_enter_close", objs.TraceCloseEnter)

	attachTracepoint(&links, "syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attachTracepoint(&links, "syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

	attachTracepoint(&links, "syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit)
	attachTracepoint(&links, "syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter)
	attachTracepoint(&links, "syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit)

	if *flgMmsg {
		attachTracepoint(&links, "syscalls", "sys_enter_sendmmsg", objs.TraceSendmmsgEnter)
		attachTracepoint(&links, "syscalls", "sys_exit_sendmmsg", objs.TraceSendmmsgExit)
		attachTracepoint(&links, "syscalls", "sys_enter_recvmmsg", objs.TraceRecvmmsgEnter)
		attachTracepoint(&links, "syscalls", "sys_exit_recvmmsg", objs.TraceRecvmmsgExit)
	}
	if *flgRW {
		attachTracepoint(&links, "syscalls", "sys_enter_write", objs.TraceWriteEnter)
		attachTracepoint(&links, "syscalls", "sys_exit_write", objs.TraceWriteExit)
		attachTracepoint(&links, "syscalls", "sys_enter_read", objs.TraceReadEnter)
		attachTracepoint(&links, "syscalls", "sys_exit_read", objs.TraceReadExit)
	}

	// skb hint
	attachTracepoint(&links, "net", "net_dev_queue", objs.TraceNetDevQueue)

	rd, perCPUBytes, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("trace perf.NewReader: %v", err)
	}
	defer rd.Close()

	tlsRd, _, err := openPerfReaderTotalBudget(objs.TlsEvents, *flgTlsPerfMB)
	if err != nil {
		log.Fatalf("tls perf.NewReader: %v", err)
	}
	defer tlsRd.Close()

	log.Printf("perf trace per-cpu=%dKB total~=%dMB cpus=%d", perCPUBytes/1024, (perCPUBytes*runtime.NumCPU())/(1024*1024), runtime.NumCPU())
	log.Printf("started. Ctrl+C to exit")

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	type evWrap struct {
		ev  bpfTraceInfo
		now time.Time
	}
	type tlsWrap struct {
		ev  bpfTlsChunkT
		now time.Time
	}

	evCh := make(chan evWrap, 16384)
	tlsCh := make(chan tlsWrap, 4096)

	// trace reader
	go func() {
		defer close(evCh)
		for {
			rec, e := rd.Read()
			if e != nil {
				if errors.Is(e, perf.ErrClosed) {
					return
				}
				continue
			}
			if rec.LostSamples != 0 {
				atomic.AddUint64(&engine.lostTotal, rec.LostSamples)
				atomic.AddUint64(&engine.lostGen, 1)
				continue
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}
			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			evCh <- evWrap{ev: ev, now: time.Now()}
		}
	}()

	// tls reader
	go func() {
		defer close(tlsCh)
		for {
			rec, e := tlsRd.Read()
			if e != nil {
				if errors.Is(e, perf.ErrClosed) {
					return
				}
				continue
			}
			if rec.LostSamples != 0 {
				continue
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTlsChunkT{})) {
				continue
			}
			ev := *(*bpfTlsChunkT)(unsafe.Pointer(&rec.RawSample[0]))
			tlsCh <- tlsWrap{ev: ev, now: time.Now()}
		}
	}()

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	for {
		select {
		case <-stop:
			_ = rd.Close()
			_ = tlsRd.Close()
			// close remaining
			now := time.Now()
			for _, f := range engine.flows {
				if f.OpenedPrinted && *flgPrintClose {
					printClose(f, "signal")
				}
			}
			log.Printf("PERF_LOST_TOTAL total=%d gen=%d", atomic.LoadUint64(&engine.lostTotal), atomic.LoadUint64(&engine.lostGen))
			log.Println("Exiting...")
			return

		case <-ticker.C:
			engine.onTick(time.Now())

		case tw, ok := <-tlsCh:
			if !ok {
				return
			}
			engine.onTLSEvent(tw.ev, tw.now)

		case w, ok := <-evCh:
			if !ok {
				return
			}
			engine.onTraceEvent(w.ev, w.now)
		}
	}
}




sudo strace -f -e trace=sendmsg,write -p <PID>
