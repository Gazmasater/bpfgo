
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




strace -f -e sendto,sendmsg ping -c1 8.8.8.8


// main.go — bpfgo userspace
//
// ✅ Протокол берём из ev.Proto (TCP/UDP/ICMP/ICMPv6/…)
// ✅ Для ICMP/ICMPv6 печатаем ТОЛЬКО: srcIP -> dstIP (без портов, без *:0, без *:58)
// ✅ Для UDP пытаемся резолвить dst pid (для локальных/loopback) через:
//    1) динамику по портам (learned from SEND*/RECV*),
//    2) snapshot /proc/net/udp* + /proc/<pid>/fd (чтобы работало даже если сервер запущен ДО bpfgo),
//    3) bind-cache (BINDOK)
// ✅ TCP connect/accept печатаем отдельно (как раньше)

package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
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
  - type bpfTraceInfo struct { ... } // from struct trace_info in C
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

	// event codes (from your BPF side)
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
	flgTracePort     = flag.Uint("tracePort", 9999, "UDP port to resolve dst/src owner via /proc snapshot (0=off)")
	flgSnapshotEvery = flag.Duration("snapshotEvery", 0, "refresh UDP owners snapshot every duration (0=only once)")
	flgTTL           = flag.Duration("ttl", 2*time.Second, "TTL for in-memory caches (udp/tcp)")
	flgDebug         = flag.Bool("debug", false, "debug logs")
	flgPerfMB        = flag.Int("perfMB", 4, "perf ring buffer size in MB")
	flgPrint         = flag.Bool("print", true, "print events")
	flgSample        = flag.Uint64("sample", 1, "print every Nth matched event (>=1)")
	flgMatchOnly     = flag.Bool("matchOnly", false, "if tracePort>0, only count/print events where sport/dport==tracePort")

	flgStats     = flag.Duration("stats", 0, "print stats every interval (0=off)")
	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")
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
}

/* ===================== caches ===================== */

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384)

	pendMu sync.Mutex
	pendBy = make(map[ConnKey]PendingConnect, 16384)

	udpMu     sync.Mutex
	udpByPort = make(map[uint16]Proc, 65536) // dynamic: port->proc learned from SEND*/RECV*

	udpOwnerAny atomic.Value // map[uint16]Proc from /proc snapshot (owners by local port)
)

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

/* ===================== endpoint formatting helpers ===================== */

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
	// (без сжатия, как у тебя было)
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

func formatEndp(family uint16, ipStr string, ipKey [16]byte, port uint16) string {
	if isAllZero16(ipKey) {
		return fmt.Sprintf("*:%d", port)
	}
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

// ✅ для ICMP/ICMPv6 — только IP, без портов и без *:0 / *:58
func formatIPOnly(ipStr string, ipKey [16]byte) string {
	if isAllZero16(ipKey) {
		return "*"
	}
	return ipStr
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

func saveConn(key ConnKey, client Proc) {
	client.Seen = time.Now()
	connMu.Lock()
	connBy[key] = client
	connMu.Unlock()
}

func lookupConn(key ConnKey) (Proc, bool) {
	connMu.Lock()
	p, ok := connBy[key]
	connMu.Unlock()
	return p, ok
}

func savePending(key ConnKey, pc PendingConnect) {
	pc.Seen = time.Now()
	pendMu.Lock()
	pendBy[key] = pc
	pendMu.Unlock()
}

func takePending(key ConnKey) (PendingConnect, bool) {
	pendMu.Lock()
	v, ok := pendBy[key]
	if ok {
		delete(pendBy, key)
	}
	pendMu.Unlock()
	return v, ok
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

/* ===================== printing ===================== */

func printTCPConnect(client Proc, clientEp, serverEp string, server Proc) {
	fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=%s\n",
		client.String(), clientEp, serverEp, server.String())
}

func printTCPAccept(server Proc, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  server=%s  %s -> %s  client=%s\n",
		server.String(), serverEp, clientEp, client.String())
}

func printSendLine(proto, kind string, p Proc, srcEp, dstEp string, dst Proc, ok bool) {
	d := "?"
	if ok {
		d = dst.String()
	}
	// стиль как у тебя: "UDP SENDTO pid=... src=...  A -> B  dst=?"
	fmt.Printf("%s %-7s pid=%s  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), p.String(), srcEp, dstEp, d)
}

func printRecvLine(proto, kind string, p Proc, src Proc, ok bool, srcEp, dstEp string) {
	s := "?"
	if ok {
		s = src.String()
	}
	fmt.Printf("%s %-7s pid=%s  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), s, srcEp, dstEp, p.String())
}

func printICMP(proto, kind string, p Proc, srcIPOnly, dstIPOnly string) {
	fmt.Printf("%s %-7s pid=%s  %s -> %s\n",
		proto, kind, p.String(), srcIPOnly, dstIPOnly)
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

		connMu.Lock()
		for k, v := range connBy {
			if v.Seen.Before(cut) {
				delete(connBy, k)
			}
		}
		connMu.Unlock()

		udpMu.Lock()
		for port, v := range udpByPort {
			if v.Seen.Before(cut) {
				delete(udpByPort, port)
			}
		}
		udpMu.Unlock()

		// pending connect timeout => print server=?
		pendMu.Lock()
		for k, v := range pendBy {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=?\n",
					v.Client.String(), v.ClientEp, v.ServerEp)
				delete(pendBy, k)
			}
		}
		pendMu.Unlock()
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
// fields: sl local_address rem_address st tx_queue:rx_queue tr:tm->when retrnsmt uid timeout inode ...
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

	if tracePort != 0 {
		if p, ok := getUDPOwnerSnapshot(tracePort); ok {
			dbg("snapshotUDPOwners: TRACE_PORT=%d owner=%s", tracePort, p.String())
		} else {
			dbg("snapshotUDPOwners: TRACE_PORT=%d owner NOT FOUND", tracePort)
		}
	}
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

/* ===================== main ===================== */

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	tracePort := uint16(*flgTracePort)
	if *flgSample < 1 {
		*flgSample = 1
	}

	log.Printf("bpfgo start: debug=%v tracePort=%d perfMB=%d print=%v sample=%d matchOnly=%v snapshotEvery=%v ttl=%v stats=%v",
		*flgDebug, tracePort, *flgPerfMB, *flgPrint, *flgSample, *flgMatchOnly, *flgSnapshotEvery, *flgTTL, *flgStats)

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

		// optional match filter (userspace) — по портам
		if tracePort != 0 && *flgMatchOnly {
			if ev.Sport != tracePort && ev.Dport != tracePort {
				continue
			}
		}
		atomic.AddUint64(&cntMatch, 1)

		p := Proc{Pid: ev.Pid, Comm: comm}

		// endpoints + keys
		var (
			srcEp, dstEp       string
			srcIPKey, dstIPKey [16]byte
			sport, dport       uint16
			family             uint16

			srcIPStr, dstIPStr     string
			srcIPOnly, dstIPOnly   string
		)

		family = uint16(ev.Family)
		sport = uint16(ev.Sport)
		dport = uint16(ev.Dport)

		if ev.Family == AF_INET {
			sIP, sKey, sP := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
			dIP, dKey, dP := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP
			srcEp = formatEndp(AF_INET, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET, dIP, dKey, dP)
		} else if ev.Family == AF_INET6 {
			sIP, sKey, sP := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
			dIP, dKey, dP := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)

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

		proto := uint8(ev.Proto)
		pr := protoStr(proto)
		evn := evName(ev.Sysexit)

		// counters
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

		/* ===================== main logic ===================== */

		switch ev.Sysexit {

		case EV_BINDOK:
			// bind: remember "who listens this local port"
			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, p)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, p)

		case EV_CONNECT:
			// CONNECT (TCP): src=client, dst=server
			if proto != IPPROTO_TCP {
				break
			}
			key := ConnKey{
				Family:     family,
				ClientIP:   srcIPKey,
				ClientPort: sport,
				ServerIP:   dstIPKey,
				ServerPort: dport,
			}
			saveConn(key, p)

			if server, ok := lookupListen(family, dstIPKey, dport); ok {
				printTCPConnect(p, srcEp, dstEp, server)
			} else {
				savePending(key, PendingConnect{
					Client:   p,
					ClientEp: srcEp,
					ServerEp: dstEp,
					Seen:     time.Now(),
				})
			}

		case EV_ACCEPT:
			// ACCEPT (TCP): event in server pid, tuple src=client dst=server
			if proto != IPPROTO_TCP {
				break
			}
			server := p

			// if bind missed, still remember server for its local port
			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, server)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, server)

			key := ConnKey{
				Family:     family,
				ClientIP:   srcIPKey,
				ClientPort: sport,
				ServerIP:   dstIPKey,
				ServerPort: dport,
			}

			if pc, ok := takePending(key); ok {
				printTCPConnect(pc.Client, pc.ClientEp, pc.ServerEp, server)
				printTCPAccept(server, pc.ServerEp, pc.ClientEp, pc.Client)
				break
			}

			client, ok := lookupConn(key)
			if !ok {
				client = Proc{}
			}
			printTCPAccept(server, dstEp, srcEp, client)

		case EV_SENDTO, EV_SENDMSG:
			// ✅ ICMP/ICMPv6: печатаем только IP -> IP
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				if *flgPrint {
					n := atomic.AddUint64(&printed, 1)
					if n%*flgSample == 0 {
						_ = srcIPStr
						_ = dstIPStr
						printICMP(pr, evn, p, srcIPOnly, dstIPOnly)
					}
				}
				break
			}

			// learn UDP sender by its local port
			if proto == IPPROTO_UDP {
				udpSetOwner(sport, p)
			}

			// dst owner only makes sense for local-ish destinations (loopback/*)
			var dstOwner Proc
			var ok bool
			if proto == IPPROTO_UDP && isLocalish(family, dstIPKey) {
				// priority: dynamic mapping -> snapshot (/proc) -> bind cache
				if dstOwner, ok = udpGetOwner(dport); !ok {
					if dstOwner, ok = getUDPOwnerSnapshot(dport); !ok {
						dstOwner, ok = lookupListen(family, dstIPKey, dport)
					}
				}
			}

			if *flgPrint {
				n := atomic.AddUint64(&printed, 1)
				if n%*flgSample == 0 {
					printSendLine(pr, evn, p, srcEp, dstEp, dstOwner, ok)
				}
			}

		case EV_RECVFROM, EV_RECVMSG:
			// ✅ ICMP/ICMPv6: печатаем только IP -> IP
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				if *flgPrint {
					n := atomic.AddUint64(&printed, 1)
					if n%*flgSample == 0 {
						printICMP(pr, evn, p, srcIPOnly, dstIPOnly)
					}
				}
				break
			}

			// for UDP receive, dst is always this process
			if proto == IPPROTO_UDP {
				udpSetOwner(dport, p) // learn receiver by its local port (helps for server ports too)
			}

			// src owner if local-ish source (loopback/*)
			var srcOwner Proc
			var ok bool
			if proto == IPPROTO_UDP && isLocalish(family, srcIPKey) {
				// priority: dynamic mapping -> snapshot (/proc) -> bind cache
				if srcOwner, ok = udpGetOwner(sport); !ok {
					if srcOwner, ok = getUDPOwnerSnapshot(sport); !ok {
						srcOwner, ok = lookupListen(family, srcIPKey, sport)
					}
				}
			}

			if *flgPrint {
				n := atomic.AddUint64(&printed, 1)
				if n%*flgSample == 0 {
					printRecvLine(pr, evn, p, srcOwner, ok, srcEp, dstEp)
				}
			}

		default:
			// ignore others
		}
	}
}




