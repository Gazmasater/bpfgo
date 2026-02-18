
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





package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"strconv"
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

/*
Assumes generated by bpf2go:
  - type bpfObjects struct { TraceEvents *ebpf.Map; TraceSendtoEnter *ebpf.Program; ... }
  - func loadBpfObjects(*bpfObjects, *ebpf.CollectionOptions) error
  - type bpfTraceInfo struct { ... } // from struct trace_info in C (NOW includes fd+ret)
*/

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10

	// IP protocols
	IPPROTO_ICMP   = 1
	IPPROTO_TCP    = 6
	IPPROTO_UDP    = 17
	IPPROTO_ICMPV6 = 58

	// event codes (from BPF)
	EV_SENDTO   = 1
	EV_RECVFROM = 2
	EV_CONNECT  = 3
	EV_ACCEPT   = 4
	EV_BINDOK   = 20
	EV_SENDMSG  = 11
	EV_RECVMSG  = 12
)

/* ===================== flags (NO env) ===================== */

var (
	flgTracePort     = flag.Uint("tracePort", 0, "UDP port to resolve dst/src owner via /proc snapshot (0=off)")
	flgSnapshotEvery = flag.Duration("snapshotEvery", 0, "refresh UDP owners snapshot every duration (0=only once)")
	flgTTL           = flag.Duration("ttl", 3*time.Second, "TTL for in-memory caches (udp/tcp)")
	flgDebug         = flag.Bool("debug", false, "debug logs")
	flgPerfMB        = flag.Int("perfMB", 4, "perf ring buffer size in MB")
	flgPrint         = flag.Bool("print", true, "print events")
	flgSample        = flag.Uint64("sample", 1, "print every Nth matched event (>=1)")
	flgMatchOnly     = flag.Bool("matchOnly", false, "if tracePort>0, only count/print events where sport/dport==tracePort")

	flgStats     = flag.Duration("stats", 0, "print stats every interval (0=off)")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")

	flgRouteSrc = flag.Bool("routeSrc", true, "when srcIP is wildcard (*), try to infer real outgoing srcIP by route to dstIP (cached)")
)

func dbg(format string, args ...any) {
	if *flgDebug {
		log.Printf("[DBG] "+format, args...)
	}
}

/* ===================== types ===================== */

type Proc struct {
	Pid  uint32
	Comm string
	Seen time.Time
}

func (p Proc) String() string {
	if p.Pid == 0 {
		return "?"
	}
	if p.Comm == "" {
		return fmt.Sprintf("%d(?)", p.Pid)
	}
	return fmt.Sprintf("%d(%s)", p.Pid, p.Comm)
}

type EndpKey struct {
	Family uint16
	Port   uint16
	IP     [16]byte
}

type ConnKey struct {
	Family     uint16
	ClientIP   [16]byte
	ClientPort uint16
	ServerIP   [16]byte
	ServerPort uint16
}

type PendingConnect struct {
	Client   Proc
	ClientEp string
	ServerEp string
	Seen     time.Time

	Fd  uint32
	Ret int64
}

/* ===================== caches ===================== */

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	// who binds/listens on local port (best-effort)
	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	// tcp client/server by 4-tuple
	tcpMu        sync.Mutex
	tcpClientBy  = make(map[ConnKey]Proc, 16384)
	tcpServerBy  = make(map[ConnKey]Proc, 16384)
	tcpPendingMu sync.Mutex
	tcpPendBy    = make(map[ConnKey]PendingConnect, 16384)

	// udp dynamic owner by local port
	udpMu     sync.Mutex
	udpByPort = make(map[uint16]Proc, 65536)

	// snapshot (/proc) udp owners by local port
	udpOwnerAny atomic.Value // map[uint16]Proc

	// route-based src-ip cache (dstIP -> chosen local IP)
	routeMu    sync.Mutex
	routeCache = make(map[[18]byte]routeVal, 4096) // key: family(2)+dstIP(16)
)

type routeVal struct {
	ipStr  string
	ipKey  [16]byte
	expire time.Time
}

func setUDPOwners(m map[uint16]Proc) { udpOwnerAny.Store(m) }
func getUDPOwnerSnapshot(port uint16) (Proc, bool) {
	m, _ := udpOwnerAny.Load().(map[uint16]Proc)
	if m == nil {
		return Proc{}, false
	}
	p, ok := m[port]
	return p, ok
}

/* ===================== comm/proto/event names ===================== */

func commString(c [32]int8) string {
	commMu.RLock()
	if s, ok := commCache[c]; ok {
		commMu.RUnlock()
		return s
	}
	commMu.RUnlock()

	var b [32]byte
	for i := 0; i < 32; i++ {
		b[i] = byte(c[i])
	}
	n := bytes.IndexByte(b[:], 0)
	if n < 0 {
		n = len(b)
	}
	s := string(b[:n])

	commMu.Lock()
	commCache[c] = s
	commMu.Unlock()
	return s
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

func evName(code uint8) string {
	switch code {
	case EV_SENDTO:
		return "SENDTO"
	case EV_RECVFROM:
		return "RECVFROM"
	case EV_SENDMSG:
		return "SENDMSG"
	case EV_RECVMSG:
		return "RECVMSG"
	case EV_CONNECT:
		return "CONNECT"
	case EV_ACCEPT:
		return "ACCEPT"
	case EV_BINDOK:
		return "BIND"
	default:
		return fmt.Sprintf("EV%d", code)
	}
}

/* ===================== endpoint helpers ===================== */

// in_addr.s_addr is network-order, but comes as uint32 on little-endian => restore bytes
func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	binary.LittleEndian.PutUint32(b[:], x)
	return
}

func isAllZero16(b [16]byte) bool {
	for i := 0; i < 16; i++ {
		if b[i] != 0 {
			return false
		}
	}
	return true
}

func isIPv4Loop(ip [16]byte) bool { return ip[0] == 127 }
func isIPv6Loop(ip [16]byte) bool {
	for i := 0; i < 15; i++ {
		if ip[i] != 0 {
			return false
		}
	}
	return ip[15] == 1
}

func isLocalish(family uint16, ip [16]byte) bool {
	if isAllZero16(ip) { // wildcard like 0.0.0.0 / ::
		return true
	}
	if family == AF_INET && isIPv4Loop(ip) {
		return true
	}
	if family == AF_INET6 && isIPv6Loop(ip) {
		return true
	}
	return false
}

func endpFromEvIPv4(ipU32 uint32, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	b := ip4BytesFromU32Net(ipU32)
	copy(ipKey[:4], b[:])
	return fmt.Sprintf("%d.%d.%d.%d", b[0], b[1], b[2], b[3]), ipKey, port
}

func endpFromEvIPv6(ipArr [16]uint8, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	copy(ipKey[:], ipArr[:])
	// без сжатия, как у тебя
	return fmt.Sprintf("%x:%x:%x:%x:%x:%x:%x:%x",
		uint16(ipArr[0])<<8|uint16(ipArr[1]),
		uint16(ipArr[2])<<8|uint16(ipArr[3]),
		uint16(ipArr[4])<<8|uint16(ipArr[5]),
		uint16(ipArr[6])<<8|uint16(ipArr[7]),
		uint16(ipArr[8])<<8|uint16(ipArr[9]),
		uint16(ipArr[10])<<8|uint16(ipArr[11]),
		uint16(ipArr[12])<<8|uint16(ipArr[13]),
		uint16(ipArr[14])<<8|uint16(ipArr[15]),
	), ipKey, port
}

// for ICMP/ICMPv6 print only IP
func formatIPOnly(ipStr string, ipKey [16]byte) string {
	if isAllZero16(ipKey) {
		return "*"
	}
	return ipStr
}

func formatEndp(family uint16, ipStr string, ipKey [16]byte, port uint16) string {
	// keep old style: wildcard => *:PORT (but we will try to replace * via routeSrc before printing)
	if isAllZero16(ipKey) {
		return fmt.Sprintf("*:%d", port)
	}
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

/* ===================== route-based srcIP resolve ===================== */

func routeKey(family uint16, dst [16]byte) (k [18]byte) {
	k[0] = byte(family >> 8)
	k[1] = byte(family)
	copy(k[2:], dst[:])
	return
}

func tryRouteLocalIP(family uint16, dstIPKey [16]byte, dport uint16) (string, [16]byte, bool) {
	var outKey [16]byte

	// need a real dst, otherwise routing is meaningless
	if isAllZero16(dstIPKey) {
		return "", outKey, false
	}

	// cache (30s)
	now := time.Now()
	k := routeKey(family, dstIPKey)

	routeMu.Lock()
	if v, ok := routeCache[k]; ok && now.Before(v.expire) {
		routeMu.Unlock()
		return v.ipStr, v.ipKey, true
	}
	routeMu.Unlock()

	port := int(dport)
	if port == 0 {
		port = 443
	}

	var (
		lip net.IP
		err error
	)

	if family == AF_INET {
		dst := net.IPv4(dstIPKey[0], dstIPKey[1], dstIPKey[2], dstIPKey[3])
		c, e := net.DialUDP("udp4", nil, &net.UDPAddr{IP: dst, Port: port})
		if e != nil {
			err = e
		} else {
			lip = c.LocalAddr().(*net.UDPAddr).IP
			_ = c.Close()
		}
	} else if family == AF_INET6 {
		dst := net.IP(dstIPKey[:])
		c, e := net.DialUDP("udp6", nil, &net.UDPAddr{IP: dst, Port: port})
		if e != nil {
			err = e
		} else {
			lip = c.LocalAddr().(*net.UDPAddr).IP
			_ = c.Close()
		}
	} else {
		return "", outKey, false
	}

	if err != nil || lip == nil {
		return "", outKey, false
	}

	if family == AF_INET {
		l4 := lip.To4()
		if l4 == nil {
			return "", outKey, false
		}
		copy(outKey[:4], l4)
		ipStr := fmt.Sprintf("%d.%d.%d.%d", l4[0], l4[1], l4[2], l4[3])

		routeMu.Lock()
		routeCache[k] = routeVal{ipStr: ipStr, ipKey: outKey, expire: now.Add(30 * time.Second)}
		routeMu.Unlock()

		return ipStr, outKey, true
	}

	// v6
	l16 := lip.To16()
	if l16 == nil {
		return "", outKey, false
	}
	copy(outKey[:], l16)
	ipStr := net.IP(l16).String() // compressed is fine for route-only
	routeMu.Lock()
	routeCache[k] = routeVal{ipStr: ipStr, ipKey: outKey, expire: now.Add(30 * time.Second)}
	routeMu.Unlock()
	return ipStr, outKey, true
}

/* ===================== caches ops ===================== */

func saveListen(ep EndpKey, p Proc) {
	p.Seen = time.Now()
	listenMu.Lock()
	listenBy[ep] = p
	listenMu.Unlock()
}

func lookupListen(family uint16, ip [16]byte, port uint16) (Proc, bool) {
	k := EndpKey{Family: family, Port: port, IP: ip}
	listenMu.Lock()
	p, ok := listenBy[k]
	listenMu.Unlock()
	if ok {
		return p, true
	}
	// wildcard fallback (0.0.0.0 / ::)
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	listenMu.Lock()
	p2, ok2 := listenBy[k2]
	listenMu.Unlock()
	return p2, ok2
}

func udpSetOwner(port uint16, p Proc) {
	p.Seen = time.Now()
	udpMu.Lock()
	udpByPort[port] = p
	udpMu.Unlock()
}

func udpGetOwner(port uint16) (Proc, bool) {
	udpMu.Lock()
	p, ok := udpByPort[port]
	udpMu.Unlock()
	return p, ok
}

func tcpSaveClient(key ConnKey, p Proc) {
	p.Seen = time.Now()
	tcpMu.Lock()
	tcpClientBy[key] = p
	tcpMu.Unlock()
}

func tcpSaveServer(key ConnKey, p Proc) {
	p.Seen = time.Now()
	tcpMu.Lock()
	tcpServerBy[key] = p
	tcpMu.Unlock()
}

func tcpGetClient(key ConnKey) (Proc, bool) {
	tcpMu.Lock()
	p, ok := tcpClientBy[key]
	tcpMu.Unlock()
	return p, ok
}

func tcpGetServer(key ConnKey) (Proc, bool) {
	tcpMu.Lock()
	p, ok := tcpServerBy[key]
	tcpMu.Unlock()
	return p, ok
}

func tcpSavePending(key ConnKey, pc PendingConnect) {
	pc.Seen = time.Now()
	tcpPendingMu.Lock()
	tcpPendBy[key] = pc
	tcpPendingMu.Unlock()
}

func tcpTakePending(key ConnKey) (PendingConnect, bool) {
	tcpPendingMu.Lock()
	v, ok := tcpPendBy[key]
	if ok {
		delete(tcpPendBy, key)
	}
	tcpPendingMu.Unlock()
	return v, ok
}

/* ===================== printing ===================== */

func printConnect(p Proc, fd uint32, ret int64, clientEp, serverEp string, server Proc) {
	srv := server.String()
	fmt.Printf("TCP CONNECT client=%s fd=%d ret=%d  %s -> %s  server=%s\n",
		p.String(), fd, ret, clientEp, serverEp, srv)
}

func printAccept(server Proc, fd uint32, ret int64, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  server=%s fd=%d ret=%d  %s -> %s  client=%s\n",
		server.String(), fd, ret, serverEp, clientEp, client.String())
}

func printSendLine(proto, kind string, p Proc, fd uint32, ret int64, srcEp, dstEp string, dst Proc, ok bool) {
	d := "?"
	if ok {
		d = dst.String()
	}
	fmt.Printf("%s %-7s pid=%s fd=%d ret=%d  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), fd, ret, p.String(), srcEp, dstEp, d)
}

func printRecvLine(proto, kind string, p Proc, fd uint32, ret int64, src Proc, ok bool, srcEp, dstEp string) {
	s := "?"
	if ok {
		s = src.String()
	}
	fmt.Printf("%s %-7s pid=%s fd=%d ret=%d  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), fd, ret, s, srcEp, dstEp, p.String())
}

func printICMP(proto, kind string, p Proc, fd uint32, ret int64, srcIPOnly, dstIPOnly string) {
	fmt.Printf("%s %-7s pid=%s fd=%d ret=%d  %s -> %s\n",
		proto, kind, p.String(), fd, ret, srcIPOnly, dstIPOnly)
}

/* ===================== cleanup (TTL) ===================== */

func cleanupTTL(ttl time.Duration) {
	t := time.NewTicker(1 * time.Second)
	defer t.Stop()

	for range t.C {
		cut := time.Now().Add(-ttl)

		listenMu.Lock()
		for k, v := range listenBy {
			if v.Seen.Before(cut) {
				delete(listenBy, k)
			}
		}
		listenMu.Unlock()

		tcpMu.Lock()
		for k, v := range tcpClientBy {
			if v.Seen.Before(cut) {
				delete(tcpClientBy, k)
			}
		}
		for k, v := range tcpServerBy {
			if v.Seen.Before(cut) {
				delete(tcpServerBy, k)
			}
		}
		tcpMu.Unlock()

		udpMu.Lock()
		for port, v := range udpByPort {
			if v.Seen.Before(cut) {
				delete(udpByPort, port)
			}
		}
		udpMu.Unlock()

		// pending connect timeout => print server=?
		tcpPendingMu.Lock()
		for k, v := range tcpPendBy {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT client=%s fd=%d ret=%d  %s -> %s  server=?\n",
					v.Client.String(), v.Fd, v.Ret, v.ClientEp, v.ServerEp)
				delete(tcpPendBy, k)
			}
		}
		tcpPendingMu.Unlock()

		// route cache ttl cleanup (lazy)
		routeMu.Lock()
		now := time.Now()
		for k, v := range routeCache {
			if now.After(v.expire) {
				delete(routeCache, k)
			}
		}
		routeMu.Unlock()
	}
}

/* ===================== /proc UDP owners snapshot ===================== */

type inodePort struct {
	Inode uint64
	Port  uint16
}

func commFromProc(pid uint32) string {
	b, err := os.ReadFile(fmt.Sprintf("/proc/%d/comm", pid))
	if err != nil {
		return ""
	}
	return strings.TrimSpace(string(b))
}

func buildInodeToProc(selfPID uint32) (map[uint64]Proc, int, int) {
	out := make(map[uint64]Proc, 4096)

	ents, err := os.ReadDir("/proc")
	if err != nil {
		dbg("readdir /proc: %v", err)
		return out, 0, 0
	}

	scanned, skipped := 0, 0

	for _, e := range ents {
		if !e.IsDir() {
			continue
		}
		pid64, err := strconv.ParseUint(e.Name(), 10, 32)
		if err != nil {
			continue
		}
		pid := uint32(pid64)

		if pid == selfPID {
			skipped++
			continue
		}

		scanned++
		comm := commFromProc(pid)

		fdDir := fmt.Sprintf("/proc/%d/fd", pid)
		fds, err := os.ReadDir(fdDir)
		if err != nil {
			continue
		}
		for _, fd := range fds {
			linkPath := fdDir + "/" + fd.Name()
			target, err := os.Readlink(linkPath)
			if err != nil {
				continue
			}
			if !strings.HasPrefix(target, "socket:[") {
				continue
			}
			trim := strings.TrimPrefix(target, "socket:[")
			trim = strings.TrimSuffix(trim, "]")
			inode, err := strconv.ParseUint(trim, 10, 64)
			if err != nil || inode == 0 {
				continue
			}
			out[inode] = Proc{Pid: pid, Comm: comm}
		}
	}

	return out, scanned, skipped
}

// /proc/net/udp*: wantState="" => all entries
func parseProcNetInodes(path string, wantState string) []inodePort {
	f, err := os.Open(path)
	if err != nil {
		dbg("open %s: %v", path, err)
		return nil
	}
	defer f.Close()

	sc := bufio.NewScanner(f)
	if !sc.Scan() {
		return nil
	}

	var out []inodePort
	lines := 0

	for sc.Scan() {
		lines++
		line := strings.TrimSpace(sc.Text())
		if line == "" {
			continue
		}
		fields := strings.Fields(line)
		if len(fields) < 10 {
			continue
		}

		local := fields[1]
		state := fields[3]
		if wantState != "" && state != wantState {
			continue
		}

		parts := strings.Split(local, ":")
		if len(parts) != 2 {
			continue
		}

		port64, err := strconv.ParseUint(parts[1], 16, 16)
		if err != nil || port64 == 0 {
			continue
		}
		inode, err := strconv.ParseUint(fields[9], 10, 64)
		if err != nil || inode == 0 {
			continue
		}

		out = append(out, inodePort{Inode: inode, Port: uint16(port64)})
	}

	dbg("%s: scanned_lines=%d parsed_entries=%d", path, lines, len(out))
	return out
}

func snapshotUDPOwners(tracePort uint16) {
	selfPID := uint32(os.Getpid())

	inode2proc, scanned, skipped := buildInodeToProc(selfPID)
	dbg("snapshotUDPOwners: inode2proc: procs_scanned=%d procs_skipped=%d unique_inodes=%d", scanned, skipped, len(inode2proc))

	udp4 := parseProcNetInodes("/proc/net/udp", "")
	udp6 := parseProcNetInodes("/proc/net/udp6", "")

	owners := make(map[uint16]Proc, 256)

	for _, it := range udp4 {
		if p, ok := inode2proc[it.Inode]; ok {
			owners[it.Port] = p
			if tracePort != 0 && it.Port == tracePort {
				dbg("UDP owner learned (udp4): port=%d proc=%s inode=%d", it.Port, p.String(), it.Inode)
			}
		}
	}
	for _, it := range udp6 {
		if p, ok := inode2proc[it.Inode]; ok {
			if _, exists := owners[it.Port]; !exists {
				owners[it.Port] = p
			}
			if tracePort != 0 && it.Port == tracePort {
				dbg("UDP owner learned (udp6): port=%d proc=%s inode=%d", it.Port, p.String(), it.Inode)
			}
		}
	}

	setUDPOwners(owners)
}

/* ===================== stats ===================== */

var (
	cntAll   uint64
	cntMatch uint64
	cntLost  uint64

	cntUDPSend    uint64
	cntUDPRecv    uint64
	cntUDPSendMsg uint64
	cntUDPRecvMsg uint64
	cntTCPConn    uint64
	cntTCPAcc     uint64
)

/* ===================== TCP peer resolution for send/recv ===================== */

func tcpPeerForTuple(p Proc, family uint16, srcIP [16]byte, sport uint16, dstIP [16]byte, dport uint16) (peer Proc, ok bool) {
	// Try role=client (src=client dst=server)
	k1 := ConnKey{Family: family, ClientIP: srcIP, ClientPort: sport, ServerIP: dstIP, ServerPort: dport}
	if c, okc := tcpGetClient(k1); okc && c.Pid == p.Pid {
		if s, oks := tcpGetServer(k1); oks {
			return s, true
		}
		// local server maybe known by bind
		if s2, ok2 := lookupListen(family, dstIP, dport); ok2 {
			return s2, true
		}
		return Proc{}, false
	}

	// Try role=server (src=server dst=client) => reverse to canonical key
	k2 := ConnKey{Family: family, ClientIP: dstIP, ClientPort: dport, ServerIP: srcIP, ServerPort: sport}
	if s, oks := tcpGetServer(k2); oks && s.Pid == p.Pid {
		if c, okc := tcpGetClient(k2); okc {
			return c, true
		}
		return Proc{}, false
	}

	// fallback: if dst is a known listener port (server->server or client->server)
	if s, ok2 := lookupListen(family, dstIP, dport); ok2 {
		return s, true
	}

	return Proc{}, false
}

/* ===================== main ===================== */

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	tracePort := uint16(*flgTracePort)
	if *flgSample < 1 {
		*flgSample = 1
	}

	log.Printf("bpfgo start: debug=%v tracePort=%d perfMB=%d print=%v sample=%d matchOnly=%v snapshotEvery=%v ttl=%v stats=%v routeSrc=%v",
		*flgDebug, tracePort, *flgPerfMB, *flgPrint, *flgSample, *flgMatchOnly, *flgSnapshotEvery, *flgTTL, *flgStats, *flgRouteSrc)

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

	// cleanup caches
	go cleanupTTL(*flgTTL)

	// snapshot UDP owners so dst resolves even if server started before bpfgo
	if tracePort != 0 {
		snapshotUDPOwners(tracePort)
		if *flgSnapshotEvery > 0 {
			go func() {
				t := time.NewTicker(*flgSnapshotEvery)
				defer t.Stop()
				for range t.C {
					snapshotUDPOwners(tracePort)
				}
			}()
		}
	}

	selfName := filepath.Base(os.Args[0])

	// attach tracepoints
	var links []link.Link
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()
	attach := func(cat, name string, prog *ebpf.Program) {
		l, err := link.Tracepoint(cat, name, prog, nil)
		if err != nil {
			log.Fatalf("attach %s/%s: %v", cat, name, err)
		}
		links = append(links, l)
	}

	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

	attach("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter)
	attach("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit)
	attach("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter)
	attach("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit)

	// perf reader
	perfBytes := *flgPerfMB * 1024 * 1024
	if perfBytes < 256*1024 {
		perfBytes = 256 * 1024
	}
	rd, err := perf.NewReader(objs.TraceEvents, perfBytes)
	if err != nil {
		log.Fatalf("perf.NewReader: %v", err)
	}
	defer rd.Close()

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	// optional stats printer
	if *flgStats > 0 {
		go func() {
			t := time.NewTicker(*flgStats)
			defer t.Stop()
			var lastAll, lastMatch, lastLost uint64
			for range t.C {
				all := atomic.LoadUint64(&cntAll)
				match := atomic.LoadUint64(&cntMatch)
				lost := atomic.LoadUint64(&cntLost)
				epsAll := all - lastAll
				epsMatch := match - lastMatch
				lostDelta := lost - lastLost
				lastAll, lastMatch, lastLost = all, match, lost

				fmt.Printf("[STAT] eps_all=%d eps_match=%d lost=%d (+%d) udp_send=%d udp_recv=%d udp_sendmsg=%d udp_recvmsg=%d tcp_conn=%d tcp_acc=%d tracePort=%d\n",
					epsAll, epsMatch, lost, lostDelta,
					atomic.LoadUint64(&cntUDPSend),
					atomic.LoadUint64(&cntUDPRecv),
					atomic.LoadUint64(&cntUDPSendMsg),
					atomic.LoadUint64(&cntUDPRecvMsg),
					atomic.LoadUint64(&cntTCPConn),
					atomic.LoadUint64(&cntTCPAcc),
					tracePort,
				)
			}
		}()
	}

	log.Println("Press Ctrl+C to exit")

	var printed uint64

	for {
		select {
		case <-stop:
			log.Println("Exiting...")
			return
		default:
		}

		rec, err := rd.Read()
		if err != nil {
			if errors.Is(err, perf.ErrClosed) {
				return
			}
			continue
		}

		if rec.LostSamples != 0 {
			atomic.AddUint64(&cntLost, uint64(rec.LostSamples))
			continue
		}
		if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			continue
		}

		atomic.AddUint64(&cntAll, 1)

		ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
		comm := commString(ev.Comm)
		if comm == selfName {
			continue
		}

		// optional port match filter
		if tracePort != 0 && *flgMatchOnly {
			if ev.Sport != tracePort && ev.Dport != tracePort {
				continue
			}
		}
		atomic.AddUint64(&cntMatch, 1)

		p := Proc{Pid: ev.Pid, Comm: comm}

		family := uint16(ev.Family)
		sport := uint16(ev.Sport)
		dport := uint16(ev.Dport)

		// NOTE: proto in C is __u32; use low byte
		proto := uint8(ev.Proto)
		pr := protoStr(proto)
		evn := evName(ev.Sysexit)

		fd := uint32(ev.Fd)
		ret := int64(ev.Ret)

		// endpoints + keys
		var (
			srcEp, dstEp       string
			srcIPKey, dstIPKey [16]byte

			srcIPStr, dstIPStr   string
			srcIPOnly, dstIPOnly string
		)

		if family == AF_INET {
			sIP, sKey, sP := endpFromEvIPv4(ev.SrcIP.S_addr, sport)
			dIP, dKey, dP := endpFromEvIPv4(ev.DstIP.S_addr, dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP
			srcEp = formatEndp(AF_INET, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET, dIP, dKey, dP)
		} else if family == AF_INET6 {
			sIP, sKey, sP := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, sport)
			dIP, dKey, dP := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP
			srcEp = formatEndp(AF_INET6, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET6, dIP, dKey, dP)
		} else {
			continue
		}

		// if src is wildcard (*), try infer real outgoing IP by routing to dst
		if *flgRouteSrc && isAllZero16(srcIPKey) && !isAllZero16(dstIPKey) {
			if rip, rkey, ok := tryRouteLocalIP(family, dstIPKey, dport); ok {
				srcIPStr = rip
				srcIPKey = rkey
				// rebuild srcEp (keep same sport)
				srcEp = formatEndp(family, srcIPStr, srcIPKey, sport)
				// also rebuild ipOnly
				srcIPOnly = formatIPOnly(srcIPStr, srcIPKey)
			}
		}

		// counters (rough)
		switch ev.Sysexit {
		case EV_SENDTO:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPSend, 1)
			}
		case EV_RECVFROM:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPRecv, 1)
			}
		case EV_SENDMSG:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPSendMsg, 1)
			}
		case EV_RECVMSG:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPRecvMsg, 1)
			}
		case EV_CONNECT:
			if proto == IPPROTO_TCP {
				atomic.AddUint64(&cntTCPConn, 1)
			}
		case EV_ACCEPT:
			if proto == IPPROTO_TCP {
				atomic.AddUint64(&cntTCPAcc, 1)
			}
		}

		// printing gate
		if !*flgPrint {
			continue
		}
		n := atomic.AddUint64(&printed, 1)
		if n%*flgSample != 0 {
			continue
		}

		/* ===================== main logic ===================== */

		switch ev.Sysexit {

		case EV_BINDOK:
			// remember listener by local port
			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, p)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, p)

		case EV_CONNECT:
			if proto != IPPROTO_TCP {
				break
			}
			// canonical: client=src server=dst
			key := ConnKey{Family: family, ClientIP: srcIPKey, ClientPort: sport, ServerIP: dstIPKey, ServerPort: dport}
			tcpSaveClient(key, p)

			if server, ok := lookupListen(family, dstIPKey, dport); ok {
				tcpSaveServer(key, server)
				printConnect(p, fd, ret, srcEp, dstEp, server)
			} else {
				tcpSavePending(key, PendingConnect{
					Client:   p,
					ClientEp: srcEp,
					ServerEp: dstEp,
					Seen:     time.Now(),
					Fd:       fd,
					Ret:      ret,
				})
				// можно не печатать сразу, иначе будет spam "server=?"
			}

		case EV_ACCEPT:
			if proto != IPPROTO_TCP {
				break
			}
			server := p
			// accept event: src=client dst=server (as you emit)
			key := ConnKey{Family: family, ClientIP: srcIPKey, ClientPort: sport, ServerIP: dstIPKey, ServerPort: dport}
			tcpSaveServer(key, server)

			// also remember server as listener (best effort)
			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, server)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, server)

			if pc, ok := tcpTakePending(key); ok {
				printConnect(pc.Client, pc.Fd, pc.Ret, pc.ClientEp, pc.ServerEp, server)
				printAccept(server, fd, ret, pc.ServerEp, pc.ClientEp, pc.Client)
				break
			}

			client, ok := tcpGetClient(key)
			if !ok {
				client = Proc{}
			}
			printAccept(server, fd, ret, dstEp, srcEp, client)

		case EV_SENDTO, EV_SENDMSG:
			// ICMP/ICMPv6: print only IP -> IP
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				printICMP(pr, evn, p, fd, ret, srcIPOnly, dstIPOnly)
				break
			}

			// learn UDP sender by local port
			if proto == IPPROTO_UDP {
				udpSetOwner(sport, p)
			}

			// dst pid resolve:
			// - UDP: only for localish destinations (loopback/*)
			// - TCP: only for loopback connections we can map via connect/accept/bind caches
			var dstOwner Proc
			var ok bool

			if proto == IPPROTO_UDP && isLocalish(family, dstIPKey) {
				if dstOwner, ok = udpGetOwner(dport); !ok {
					if dstOwner, ok = getUDPOwnerSnapshot(dport); !ok {
						dstOwner, ok = lookupListen(family, dstIPKey, dport)
					}
				}
			} else if proto == IPPROTO_TCP && isLocalish(family, dstIPKey) {
				// peer pid only makes sense if destination is local
				dstOwner, ok = tcpPeerForTuple(p, family, srcIPKey, sport, dstIPKey, dport)
			}

			printSendLine(pr, evn, p, fd, ret, srcEp, dstEp, dstOwner, ok)

		case EV_RECVFROM, EV_RECVMSG:
			// ICMP/ICMPv6: print only IP -> IP
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				printICMP(pr, evn, p, fd, ret, srcIPOnly, dstIPOnly)
				break
			}

			// learn UDP receiver by local port
			if proto == IPPROTO_UDP {
				udpSetOwner(dport, p)
			}

			// src owner resolve:
			var srcOwner Proc
			var ok bool

			if proto == IPPROTO_UDP && isLocalish(family, srcIPKey) {
				if srcOwner, ok = udpGetOwner(sport); !ok {
					if srcOwner, ok = getUDPOwnerSnapshot(sport); !ok {
						srcOwner, ok = lookupListen(family, srcIPKey, sport)
					}
				}
			} else if proto == IPPROTO_TCP && isLocalish(family, srcIPKey) {
				// if source is local, try map peer
				srcOwner, ok = tcpPeerForTuple(p, family, dstIPKey, dport, srcIPKey, sport) // note: reverse (local is dst for recv)
			}

			// For recv: packet direction printed as src -> dst (remote->local)
			printRecvLine(pr, evn, p, fd, ret, srcOwner, ok, fmt.Sprintf("%s:%d", srcIPStr, sport), fmt.Sprintf("%s:%d", dstIPStr, dport))

		default:
			// ignore
		}
	}
}



