
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
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10

	IPPROTO_TCP = 6
	IPPROTO_UDP = 17

	EV_SENDTO   = 1
	EV_RECVFROM = 2
	EV_CONNECT  = 3
	EV_ACCEPT   = 4
	EV_BINDOK   = 20
	EV_SENDMSG  = 11
	EV_RECVMSG  = 12
)

/* =========================
 * Debug config
 * ========================= */

var (
	debug    = os.Getenv("BPFGO_DEBUG") != ""
	tracePort uint16
)

func initDebug() {
	if s := os.Getenv("BPFGO_TRACE_PORT"); s != "" {
		if v, err := strconv.Atoi(s); err == nil && v > 0 && v < 65536 {
			tracePort = uint16(v)
		}
	}
}

func dbg(format string, args ...any) {
	if debug {
		log.Printf("[DBG] "+format, args...)
	}
}

func famStr(f uint16) string {
	switch f {
	case AF_INET:
		return "AF_INET"
	case AF_INET6:
		return "AF_INET6"
	default:
		return fmt.Sprintf("AF_%d", f)
	}
}

/* =========================
 * Proc
 * ========================= */

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

/* =========================
 * Keys
 * ========================= */

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

type PortKey struct {
	Family uint16
	Port   uint16
}

type PendingUDPSend struct {
	Label string // SENDTO / SENDMSG
	Pid   Proc
	SrcEp string
	DstEp string
	Seen  time.Time
}

type UDPDedupKey struct {
	Dir    uint8 // 1=SEND,2=RECV
	Family uint16
	Pid    uint32
	Sport  uint16
	Dport  uint16
	SrcIP  [16]byte
	DstIP  [16]byte
}

/* =========================
 * Caches
 * ========================= */

var (
	// comm cache
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string, 4096)

	// TCP bind/listen
	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	// TCP connects
	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384)

	// pending connects until accept
	pendMu sync.Mutex
	pendBy = make(map[ConnKey]PendingConnect, 16384)

	// UDP owners (family+port) + (port-only for loopback/dualstack)
	udpPortMu    sync.Mutex
	udpPortOwner = make(map[PortKey]Proc, 65536)
	udpPortAny   = make(map[uint16]Proc, 65536)

	// pending UDP sends for loopback by port only
	udpPendMu     sync.Mutex
	udpPendByPort = make(map[uint16][]PendingUDPSend, 4096)

	// dedup recvfrom/recvmsg, sendto/sendmsg
	udpDedupMu sync.Mutex
	udpDedup   = make(map[UDPDedupKey]time.Time, 65536)
)

/* =========================
 * comm helpers
 * ========================= */

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

/* =========================
 * IP helpers
 * ========================= */

func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	// ядро пишет __be32, а Go читает uint32 на little-endian → восстанавливаем исходные байты
	binary.LittleEndian.PutUint32(b[:], x)
	return
}

func isZero16(a [16]byte) bool {
	for _, v := range a {
		if v != 0 {
			return false
		}
	}
	return true
}

func isV4Mapped(ip [16]byte) (v4 [4]byte, ok bool) {
	// ::ffff:a.b.c.d  =>  00..00 ff ff a b c d
	for i := 0; i < 10; i++ {
		if ip[i] != 0 {
			return v4, false
		}
	}
	if ip[10] != 0xff || ip[11] != 0xff {
		return v4, false
	}
	copy(v4[:], ip[12:16])
	return v4, true
}

func isLoopback4(ip [16]byte) bool { return ip[0] == 127 }

func isLoopback6(ip [16]byte) bool {
	// pure ::1
	pure := true
	for i := 0; i < 15; i++ {
		if ip[i] != 0 {
			pure = false
			break
		}
	}
	if pure && ip[15] == 1 {
		return true
	}
	// or ::ffff:127.x.x.x
	v4, ok := isV4Mapped(ip)
	return ok && v4[0] == 127
}

func isLoopback(family uint16, ip [16]byte) bool {
	if family == AF_INET {
		return isLoopback4(ip)
	}
	if family == AF_INET6 {
		return isLoopback6(ip)
	}
	return false
}

func loopbackKey(family uint16) (ipStr string, ipKey [16]byte) {
	if family == AF_INET {
		ipKey[0] = 127
		ipKey[3] = 1
		return "127.0.0.1", ipKey
	}
	ipKey[15] = 1
	return "::1", ipKey
}

func endpFromEvIPv4(ipU32 uint32, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	b := ip4BytesFromU32Net(ipU32)
	copy(ipKey[:4], b[:])
	return fmt.Sprintf("%d.%d.%d.%d", b[0], b[1], b[2], b[3]), ipKey, port
}

func endpFromEvIPv6(ipArr [16]uint8, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	copy(ipKey[:], ipArr[:])

	// если это ::ffff:127.0.0.1 — печатаем как IPv4, чтобы было читаемо
	if v4, ok := isV4Mapped(ipKey); ok {
		return fmt.Sprintf("%d.%d.%d.%d", v4[0], v4[1], v4[2], v4[3]), ipKey, port
	}

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
	if isZero16(ipKey) {
		return fmt.Sprintf("*:%d", port)
	}
	// если это v4-mapped — печатаем как IPv4
	if family == AF_INET6 {
		if v4, ok := isV4Mapped(ipKey); ok {
			return fmt.Sprintf("%d.%d.%d.%d:%d", v4[0], v4[1], v4[2], v4[3], port)
		}
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

/* =========================
 * TCP listen cache
 * ========================= */

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
	// wildcard (0.0.0.0 / ::)
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	listenMu.Lock()
	p2, ok2 := listenBy[k2]
	listenMu.Unlock()
	return p2, ok2
}

/* =========================
 * TCP conn cache
 * ========================= */

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

func savePendingConn(key ConnKey, pc PendingConnect) {
	pc.Seen = time.Now()
	pendMu.Lock()
	pendBy[key] = pc
	pendMu.Unlock()
}

func takePendingConn(key ConnKey) (PendingConnect, bool) {
	pendMu.Lock()
	v, ok := pendBy[key]
	if ok {
		delete(pendBy, key)
	}
	pendMu.Unlock()
	return v, ok
}

/* =========================
 * UDP owner + pending
 * ========================= */

func savePortOwnerSrc(family uint16, port uint16, p Proc, src string) {
	p.Seen = time.Now()

	udpPortMu.Lock()
	udpPortOwner[PortKey{family, port}] = p
	udpPortAny[port] = p // склейка IPv4/IPv6 для loopback
	udpPortMu.Unlock()

	if debug && (tracePort == 0 || tracePort == port) {
		dbg("UDP owner learned (%s): port=%d family=%s proc=%s", src, port, famStr(family), p.String())
	}

	flushPendingUDPSend(port, p)
}

func savePortOwner(family uint16, port uint16, p Proc) { savePortOwnerSrc(family, port, p, "event") }

func lookupPortOwnerAny(port uint16) (Proc, bool) {
	udpPortMu.Lock()
	p, ok := udpPortAny[port]
	udpPortMu.Unlock()
	return p, ok
}

func savePendingUDPSend(port uint16, it PendingUDPSend) {
	it.Seen = time.Now()
	udpPendMu.Lock()
	udpPendByPort[port] = append(udpPendByPort[port], it)
	udpPendMu.Unlock()

	if debug && (tracePort == 0 || tracePort == port) {
		dbg("UDP pending add: port=%d label=%s pid=%s %s -> %s", port, it.Label, it.Pid.String(), it.SrcEp, it.DstEp)
	}
}

func flushPendingUDPSend(port uint16, dstProc Proc) {
	udpPendMu.Lock()
	list, ok := udpPendByPort[port]
	if ok {
		delete(udpPendByPort, port)
	}
	udpPendMu.Unlock()

	if !ok || len(list) == 0 {
		return
	}

	if debug && (tracePort == 0 || tracePort == port) {
		dbg("UDP pending flush: port=%d n=%d dst=%s", port, len(list), dstProc.String())
	}

	for _, it := range list {
		printUDP(it.Label, it.Pid, it.Pid, dstProc, it.SrcEp, it.DstEp)
	}
}

/* =========================
 * UDP dedup
 * ========================= */

func udpSeenRecently(k UDPDedupKey, window time.Duration) bool {
	now := time.Now()
	udpDedupMu.Lock()
	defer udpDedupMu.Unlock()

	if ts, ok := udpDedup[k]; ok {
		if now.Sub(ts) <= window {
			return true
		}
	}
	udpDedup[k] = now
	return false
}

/* =========================
 * Printing
 * ========================= */

func printTCPConnect(client Proc, clientEp, serverEp string, server Proc) {
	fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=%s\n",
		client.String(), clientEp, serverEp, server.String())
}

func printTCPAccept(server Proc, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  server=%s  %s -> %s  client=%s\n",
		server.String(), serverEp, clientEp, client.String())
}

func printUDP(label string, pid Proc, srcProc Proc, dstProc Proc, srcEp, dstEp string) {
	fmt.Printf("UDP %-7s pid=%s  src=%s  %s -> %s  dst=%s\n",
		label, pid.String(), srcProc.String(), srcEp, dstEp, dstProc.String())
}

/* =========================
 * TTL cleanup
 * ========================= */

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

		udpPortMu.Lock()
		for k, v := range udpPortOwner {
			if v.Seen.Before(cut) {
				delete(udpPortOwner, k)
			}
		}
		for port, v := range udpPortAny {
			if v.Seen.Before(cut) {
				delete(udpPortAny, port)
			}
		}
		udpPortMu.Unlock()

		pendMu.Lock()
		for k, v := range pendBy {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=?\n",
					v.Client.String(), v.ClientEp, v.ServerEp)
				delete(pendBy, k)
			}
		}
		pendMu.Unlock()

		// UDP pending by port
		udpPendMu.Lock()
		for port, list := range udpPendByPort {
			j := 0
			for _, it := range list {
				if it.Seen.Before(cut) {
					// timeout: всё-таки печатаем dst=?
					printUDP(it.Label, it.Pid, it.Pid, Proc{}, it.SrcEp, it.DstEp)
					continue
				}
				list[j] = it
				j++
			}
			if j == 0 {
				delete(udpPendByPort, port)
			} else {
				udpPendByPort[port] = list[:j]
			}
		}
		udpPendMu.Unlock()
	}
}

/* =========================
 * B) /proc snapshot
 * ========================= */

type inodePort struct {
	Inode uint64
	Port  uint16
}

func readFirstLine(path string) string {
	f, err := os.Open(path)
	if err != nil {
		return ""
	}
	defer f.Close()
	sc := bufio.NewScanner(f)
	if sc.Scan() {
		return strings.TrimSpace(sc.Text())
	}
	return ""
}

func buildInodeToProc(selfPID uint32, selfName string) map[uint64]Proc {
	out := make(map[uint64]Proc, 200000)

	ents, err := os.ReadDir("/proc")
	if err != nil {
		dbg("cannot read /proc: %v", err)
		return out
	}

	scanned := 0
	skipped := 0

	for _, e := range ents {
		if !e.IsDir() {
			continue
		}
		pid, err := strconv.Atoi(e.Name())
		if err != nil || pid <= 0 {
			continue
		}
		up := uint32(pid)
		if up == selfPID {
			continue
		}

		comm := readFirstLine(filepath.Join("/proc", e.Name(), "comm"))
		if comm == "" {
			comm = "?"
		}
		if comm == selfName {
			continue
		}

		fdDir := filepath.Join("/proc", e.Name(), "fd")
		fds, err := os.ReadDir(fdDir)
		if err != nil {
			skipped++
			continue
		}

		scanned++
		p := Proc{Pid: up, Comm: comm}
		for _, fd := range fds {
			target, err := os.Readlink(filepath.Join(fdDir, fd.Name()))
			if err != nil {
				continue
			}
			if !strings.HasPrefix(target, "socket:[") || !strings.HasSuffix(target, "]") {
				continue
			}
			num := strings.TrimSuffix(strings.TrimPrefix(target, "socket:["), "]")
			inode, err := strconv.ParseUint(num, 10, 64)
			if err != nil || inode == 0 {
				continue
			}
			if _, ok := out[inode]; !ok {
				out[inode] = p
			}
		}
	}

	dbg("inode snapshot: procs_scanned=%d procs_skipped=%d unique_inodes=%d", scanned, skipped, len(out))
	return out
}

// Robust parser: uses header columns (local_address, st, inode)
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
	hdr := strings.Fields(strings.TrimSpace(sc.Text()))

	col := func(name string) int {
		for i, v := range hdr {
			if v == name {
				return i
			}
		}
		return -1
	}

	iLocal := col("local_address")
	iState := col("st")
	iInode := col("inode")
	if iLocal < 0 || iInode < 0 {
		dbg("%s: header missing columns (local_address/inode)", path)
		return nil
	}

	maxNeed := iLocal
	if iState > maxNeed {
		maxNeed = iState
	}
	if iInode > maxNeed {
		maxNeed = iInode
	}

	var out []inodePort
	for sc.Scan() {
		fields := strings.Fields(strings.TrimSpace(sc.Text()))
		if len(fields) <= maxNeed {
			continue
		}

		local := fields[iLocal]
		state := ""
		if iState >= 0 {
			state = fields[iState]
		}
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

		inodeStr := fields[iInode]
		inode, err := strconv.ParseUint(inodeStr, 10, 64)
		if err != nil || inode == 0 {
			continue
		}

		out = append(out, inodePort{Inode: inode, Port: uint16(port64)})
	}

	return out
}

func snapshotPorts(selfName string) {
	selfPID := uint32(os.Getpid())
	dbg("snapshotPorts: starting (self=%d %s)", selfPID, selfName)

	inode2proc := buildInodeToProc(selfPID, selfName)
	if len(inode2proc) == 0 {
		dbg("snapshotPorts: inode2proc empty")
		return
	}

	// UDP owners
	nUDP4, nUDP6 := 0, 0
	for _, it := range parseProcNetInodes("/proc/net/udp", "") {
		if p, ok := inode2proc[it.Inode]; ok {
			savePortOwnerSrc(AF_INET, it.Port, p, "proc/udp")
			nUDP4++
		}
	}
	for _, it := range parseProcNetInodes("/proc/net/udp6", "") {
		if p, ok := inode2proc[it.Inode]; ok {
			savePortOwnerSrc(AF_INET6, it.Port, p, "proc/udp6")
			nUDP6++
		}
	}
	dbg("snapshotPorts: udp owners loaded: udp=%d udp6=%d", nUDP4, nUDP6)

	// TCP listeners (state 0A = LISTEN)
	var zero [16]byte
	nTCP4, nTCP6 := 0, 0
	for _, it := range parseProcNetInodes("/proc/net/tcp", "0A") {
		if p, ok := inode2proc[it.Inode]; ok {
			saveListen(EndpKey{Family: AF_INET, Port: it.Port, IP: zero}, p)
			nTCP4++
		}
	}
	for _, it := range parseProcNetInodes("/proc/net/tcp6", "0A") {
		if p, ok := inode2proc[it.Inode]; ok {
			saveListen(EndpKey{Family: AF_INET6, Port: it.Port, IP: zero}, p)
			nTCP6++
		}
	}
	dbg("snapshotPorts: tcp listeners loaded: tcp=%d tcp6=%d", nTCP4, nTCP6)

	if tracePort != 0 {
		if p, ok := lookupPortOwnerAny(tracePort); ok {
			dbg("snapshotPorts: TRACE_PORT=%d ownerAny=%s", tracePort, p.String())
		} else {
			dbg("snapshotPorts: TRACE_PORT=%d ownerAny NOT FOUND", tracePort)
		}
	}
}

/* =========================
 * init/load
 * ========================= */

func init() {
	initDebug()

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

/* =========================
 * main
 * ========================= */

func main() {
	go func() {
		log.Println("pprof on :6060")
		_ = http.ListenAndServe(":6060", nil)
	}()

	defer objs.Close()
	go cleanupTTL(2 * time.Second)

	selfName := filepath.Base(os.Args[0])

	// ✅ snapshot ДО attach — покрывает "server запущен раньше bpfgo"
	snapshotPorts(selfName)

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

	// TCP
	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	// UDP
	attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

	attach("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter)
	attach("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit)
	attach("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter)
	attach("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit)

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 256 * 1024
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("perf.NewReader: %v", err)
		}
		defer rd.Close()

		for {
			rec, err := rd.Read()
			if err != nil {
				if errors.Is(err, perf.ErrClosed) {
					return
				}
				log.Printf("perf read error: %v", err)
				continue
			}
			if rec.LostSamples != 0 {
				log.Printf("LOST %d samples", rec.LostSamples)
				continue
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}

			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			comm := commString(ev.Comm)
			if comm == selfName {
				continue
			}

			p := Proc{Pid: ev.Pid, Comm: comm}
			fam := uint16(ev.Family)
			proto := uint8(ev.Proto)

			// endpoints
			var (
				srcEp, dstEp       string
				srcIPKey, dstIPKey [16]byte
				sport, dport       uint16
			)

			if ev.Family == AF_INET {
				sIP, sKey, sP := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
				srcEp = formatEndp(AF_INET, sIP, sKey, sP)
				dstEp = formatEndp(AF_INET, dIP, dKey, dP)
			} else if ev.Family == AF_INET6 {
				sIP, sKey, sP := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
				srcEp = formatEndp(AF_INET6, sIP, sKey, sP)
				dstEp = formatEndp(AF_INET6, dIP, dKey, dP)
			} else {
				continue
			}

			switch ev.Sysexit {

			case EV_BINDOK:
				// bind OK: сохраняем TCP listen (и дополнительно UDP owner по порту — полезно)
				saveListen(EndpKey{Family: fam, Port: dport, IP: dstIPKey}, p)
				var zero [16]byte
				saveListen(EndpKey{Family: fam, Port: dport, IP: zero}, p)

				if dport != 0 {
					savePortOwnerSrc(fam, dport, p, "bindok")
				}

			case EV_CONNECT:
				if proto != IPPROTO_TCP {
					continue
				}
				key := ConnKey{
					Family:     fam,
					ClientIP:   srcIPKey,
					ClientPort: sport,
					ServerIP:   dstIPKey,
					ServerPort: dport,
				}
				saveConn(key, p)

				if server, ok := lookupListen(fam, dstIPKey, dport); ok {
					printTCPConnect(p, srcEp, dstEp, server)
				} else {
					savePendingConn(key, PendingConnect{
						Client:   p,
						ClientEp: srcEp,
						ServerEp: dstEp,
					})
				}

			case EV_ACCEPT:
				if proto != IPPROTO_TCP {
					continue
				}
				server := p

				saveListen(EndpKey{Family: fam, Port: dport, IP: dstIPKey}, server)
				var zero [16]byte
				saveListen(EndpKey{Family: fam, Port: dport, IP: zero}, server)

				key := ConnKey{
					Family:     fam,
					ClientIP:   srcIPKey,
					ClientPort: sport,
					ServerIP:   dstIPKey,
					ServerPort: dport,
				}

				if pc, ok := takePendingConn(key); ok {
					printTCPConnect(pc.Client, pc.ClientEp, pc.ServerEp, server)
					printTCPAccept(server, pc.ServerEp, pc.ClientEp, pc.Client)
					continue
				}

				client, ok := lookupConn(key)
				if !ok {
					client = Proc{}
				}
				printTCPAccept(server, dstEp, srcEp, client)

			case EV_SENDTO, EV_SENDMSG:
				if proto != IPPROTO_UDP {
					continue
				}
				label := "SENDTO"
				if ev.Sysexit == EV_SENDMSG {
					label = "SENDMSG"
				}

				// cosmetic: dst loopback + src "*": подставим loopback
				if isLoopback(fam, dstIPKey) && isZero16(srcIPKey) {
					lbStr, lbKey := loopbackKey(fam)
					srcIPKey = lbKey
					srcEp = formatEndp(fam, lbStr, lbKey, sport)
				}

				// learn: исходящий ephemeral порт
				if sport != 0 {
					savePortOwnerSrc(fam, sport, p, "send-learn")
				}

				// dedup sendto/sendmsg
				dk := UDPDedupKey{
					Dir:    1,
					Family: fam,
					Pid:    p.Pid,
					Sport:  sport,
					Dport:  dport,
					SrcIP:  srcIPKey,
					DstIP:  dstIPKey,
				}
				if udpSeenRecently(dk, 2*time.Millisecond) {
					continue
				}

				isLB := isLoopback(fam, dstIPKey)

				// dst proc:
				dstProc, ok := lookupListen(fam, dstIPKey, dport)
				if !ok && isLB {
					dstProc, ok = lookupPortOwnerAny(dport)
				}

				if debug && (tracePort == 0 || tracePort == dport || tracePort == sport) {
					dbg("UDP %s: fam=%s loopback=%v pid=%s %s -> %s ownerKnown=%v",
						label, famStr(fam), isLB, p.String(), srcEp, dstEp, ok)
				}

				// loopback + dst неизвестен => pending (не печатаем dst=?)
				if !ok && isLB {
					savePendingUDPSend(dport, PendingUDPSend{
						Label: label,
						Pid:   p,
						SrcEp: srcEp,
						DstEp: dstEp,
					})
					continue
				}

				printUDP(label, p, p, dstProc, srcEp, dstEp)

			case EV_RECVFROM, EV_RECVMSG:
				if proto != IPPROTO_UDP {
					continue
				}
				label := "RECVFROM"
				if ev.Sysexit == EV_RECVMSG {
					label = "RECVMSG"
				}

				// ✅ ключ: всегда учим владельца локального порта
				if dport != 0 {
					savePortOwnerSrc(fam, dport, p, "recv-learn")
				}

				// если msg_name не заполнен — src неизвестен; owner уже сохранили
				if sport == 0 || isZero16(srcIPKey) {
					if debug && (tracePort == 0 || tracePort == dport) {
						dbg("UDP %s: src unknown (sport=%d, srcIP=zero), learned owner for dport=%d",
							label, sport, dport)
					}
					continue
				}

				// dedup recvfrom/recvmsg
				dk := UDPDedupKey{
					Dir:    2,
					Family: fam,
					Pid:    p.Pid,
					Sport:  sport,
					Dport:  dport,
					SrcIP:  srcIPKey,
					DstIP:  dstIPKey,
				}
				if udpSeenRecently(dk, 2*time.Millisecond) {
					continue
				}

				// src proc meaningful mostly for loopback
				srcProc := Proc{}
				if isLoopback(fam, srcIPKey) {
					srcProc, _ = lookupPortOwnerAny(sport)
				}

				printUDP(label, p, srcProc, p, srcEp, dstEp)
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

/* =========================
 * eBPF init/load
 * ========================= */

func init() {
	// NOTE: this init() is already defined above; keep only one in your file.
	// If your project has another init() (generated), merge these lines there.
}



BPFGO_DEBUG=1 BPFGO_TRACE_PORT=9999 sudo ./bpfgo








