
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
)

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

/* ===== keys/caches ===== */

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

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384)

	pendMu sync.Mutex
	pendBy = make(map[ConnKey]PendingConnect, 16384)

	// UDP: port -> owner Proc (для dst/src в loopback-сценариях)
	udpMu     sync.RWMutex
	udpByPort = make(map[uint16]Proc, 8192)
)

/* ===== comm/proto ===== */

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
	default:
		return fmt.Sprintf("P%d", p)
	}
}

/* ===== IPv4 byte-order FIX (1.0.0.127 -> 127.0.0.1) ===== */

func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	// in_addr.s_addr хранится в network order, но как uint32 на little-endian выглядит "перевёрнутым".
	binary.LittleEndian.PutUint32(b[:], x)
	return
}

func isZero4(b [4]byte) bool { return b[0] == 0 && b[1] == 0 && b[2] == 0 && b[3] == 0 }
func isLoop4(b [4]byte) bool { return b[0] == 127 }

func endpFromEvIPv4(ipU32 uint32, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16, ip4 [4]byte) {
	b := ip4BytesFromU32Net(ipU32)
	ip4 = b
	copy(ipKey[:4], b[:])
	if isZero4(b) {
		return "*", ipKey, port, ip4
	}
	return fmt.Sprintf("%d.%d.%d.%d", b[0], b[1], b[2], b[3]), ipKey, port, ip4
}

func isZero16(a [16]uint8) bool {
	for i := 0; i < 16; i++ {
		if a[i] != 0 {
			return false
		}
	}
	return true
}
func isLoop6(a [16]uint8) bool {
	// ::1
	for i := 0; i < 15; i++ {
		if a[i] != 0 {
			return false
		}
	}
	return a[15] == 1
}

func endpFromEvIPv6(ipArr [16]uint8, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16, ip6 [16]uint8) {
	ip6 = ipArr
	copy(ipKey[:], ipArr[:])
	if isZero16(ipArr) {
		return "*", ipKey, port, ip6
	}
	// оставляю как у тебя (без сжатия)
	return fmt.Sprintf("%x:%x:%x:%x:%x:%x:%x:%x",
		uint16(ipArr[0])<<8|uint16(ipArr[1]),
		uint16(ipArr[2])<<8|uint16(ipArr[3]),
		uint16(ipArr[4])<<8|uint16(ipArr[5]),
		uint16(ipArr[6])<<8|uint16(ipArr[7]),
		uint16(ipArr[8])<<8|uint16(ipArr[9]),
		uint16(ipArr[10])<<8|uint16(ipArr[11]),
		uint16(ipArr[12])<<8|uint16(ipArr[13]),
		uint16(ipArr[14])<<8|uint16(ipArr[15]),
	), ipKey, port, ip6
}

func formatEndp(family uint16, ipStr string, port uint16) string {
	if family == AF_INET6 && ipStr != "*" {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

/* ===== UDP owner helpers ===== */

func udpSetOwner(port uint16, p Proc) {
	if port == 0 || p.Pid == 0 {
		return
	}
	p.Seen = time.Now()
	udpMu.Lock()
	udpByPort[port] = p
	udpMu.Unlock()
}

func udpGetOwner(port uint16) (Proc, bool) {
	if port == 0 {
		return Proc{}, false
	}
	udpMu.RLock()
	p, ok := udpByPort[port]
	udpMu.RUnlock()
	return p, ok
}

/* ===== snapshot UDP owners from /proc ===== */

type inodePort struct {
	Inode uint64
	Port  uint16
}

// /proc/net/udp* columns: ... local_address rem_address st ... inode ...
// inode index стабильно 9 в обычном формате /proc/net/udp
func parseProcNetInodesUDP(path string) []inodePort {
	f, err := os.Open(path)
	if err != nil {
		return nil
	}
	defer f.Close()

	sc := bufio.NewScanner(f)
	if !sc.Scan() { // header
		return nil
	}

	out := make([]inodePort, 0, 256)
	for sc.Scan() {
		line := strings.TrimSpace(sc.Text())
		if line == "" {
			continue
		}
		fields := strings.Fields(line)
		if len(fields) < 10 {
			continue
		}

		local := fields[1] // "IP:PORT" both hex
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
	return out
}

func commFromProc(pid uint32) string {
	b, err := os.ReadFile(fmt.Sprintf("/proc/%d/comm", pid))
	if err != nil {
		return ""
	}
	return strings.TrimSpace(string(b))
}

// inode -> Proc by scanning /proc/<pid>/fd -> socket:[inode]
func buildInodeToProc(skipPID uint32) map[uint64]Proc {
	out := make(map[uint64]Proc, 4096)

	ents, err := os.ReadDir("/proc")
	if err != nil {
		return out
	}

	for _, e := range ents {
		if !e.IsDir() {
			continue
		}
		pid64, err := strconv.ParseUint(e.Name(), 10, 32)
		if err != nil {
			continue
		}
		pid := uint32(pid64)
		if pid == 0 || pid == skipPID {
			continue
		}

		comm := commFromProc(pid)
		fdDir := fmt.Sprintf("/proc/%d/fd", pid)

		fds, err := os.ReadDir(fdDir)
		if err != nil {
			continue
		}
		for _, fd := range fds {
			target, err := os.Readlink(fdDir + "/" + fd.Name())
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
	return out
}

func snapshotUDPOwners() {
	selfPID := uint32(os.Getpid())
	inode2proc := buildInodeToProc(selfPID)

	udp4 := parseProcNetInodesUDP("/proc/net/udp")
	udp6 := parseProcNetInodesUDP("/proc/net/udp6")

	udpMu.Lock()
	// не чистим полностью, а обновляем — так “самообучение” по событиям тоже сохраняется.
	for _, it := range udp4 {
		if p, ok := inode2proc[it.Inode]; ok {
			p.Seen = time.Now()
			udpByPort[it.Port] = p
		}
	}
	for _, it := range udp6 {
		if p, ok := inode2proc[it.Inode]; ok {
			// если уже есть запись — не трогаем (порт общий для v4/v6 нам ок)
			if _, exists := udpByPort[it.Port]; !exists {
				p.Seen = time.Now()
				udpByPort[it.Port] = p
			}
		}
	}
	udpMu.Unlock()
}

/* ===== caches (TCP) ===== */

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

		// если CONNECT так и не получил ACCEPT (удалённый сервер) — печатаем CONNECT с server=?
		pendMu.Lock()
		for k, v := range pendBy {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=?\n",
					v.Client.String(), v.ClientEp, v.ServerEp)
				delete(pendBy, k)
			}
		}
		pendMu.Unlock()

		// UDP map можно чистить очень мягко (чтобы не разрастался на краткоживущих портах)
		udpMu.Lock()
		for port, p := range udpByPort {
			if !p.Seen.IsZero() && p.Seen.Before(cut) {
				delete(udpByPort, port)
			}
		}
		udpMu.Unlock()
	}
}

/* ===== prints ===== */

func printTCPConnect(client Proc, clientEp, serverEp string, server Proc) {
	fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=%s\n",
		client.String(), clientEp, serverEp, server.String())
}

func printTCPAccept(server Proc, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  server=%s  %s -> %s  client=%s\n",
		server.String(), serverEp, clientEp, client.String())
}

func printUDPSend(kind string, p Proc, srcEp, dstEp string, dstOwner Proc, haveDst bool) {
	dst := "?"
	if haveDst {
		dst = dstOwner.String()
	}
	fmt.Printf("UDP %-7s pid=%s  src=%s  %s -> %s  dst=%s\n", kind, p.String(), p.String(), srcEp, dstEp, dst)
}

func printUDPRecv(kind string, p Proc, srcOwner Proc, haveSrc bool, srcEp, dstEp string) {
	src := "?"
	if haveSrc {
		src = srcOwner.String()
	}
	fmt.Printf("UDP %-7s pid=%s  src=%s  %s -> %s  dst=%s\n", kind, p.String(), src, srcEp, dstEp, p.String())
}

/* ===== init/load ===== */

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

func main() {
	go func() {
		log.Println("pprof on :6060")
		_ = http.ListenAndServe(":6060", nil)
	}()

	defer objs.Close()

	// важное: если сервер был запущен ДО bpfgo — bind мы уже не увидим,
	// поэтому подгружаем владельцев UDP-портов из /proc прямо на старте.
	snapshotUDPOwners()

	go cleanupTTL(2 * time.Second)

	selfName := filepath.Base(os.Args[0])

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

	// TCP roles
	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	// UDP flow
	attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

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

			// endpoints
			var srcEp, dstEp string
			var srcIPKey, dstIPKey [16]byte
			var sport, dport uint16

			// для определения "локальность" на UDP
			var srcIsLoop, dstIsLoop bool

			if ev.Family == AF_INET {
				sIP, sKey, sP, s4 := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
				dIP, dKey, dP, d4 := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)

				srcEp = formatEndp(AF_INET, sIP, sP)
				dstEp = formatEndp(AF_INET, dIP, dP)

				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP

				// loopback если 127/8
				srcIsLoop = isLoop4(s4)
				dstIsLoop = isLoop4(d4)

			} else if ev.Family == AF_INET6 {
				sIP, sKey, sP, s6 := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
				dIP, dKey, dP, d6 := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)

				srcEp = formatEndp(AF_INET6, sIP, sP)
				dstEp = formatEndp(AF_INET6, dIP, dP)

				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP

				srcIsLoop = isLoop6(s6)
				dstIsLoop = isLoop6(d6)

			} else {
				continue
			}

			switch ev.Sysexit {

			case EV_BINDOK:
				// bind: сохраняем "кто слушает порт"
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: dstIPKey}, p)
				var zero [16]byte
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: zero}, p)

				// если это UDP сокет (мы не знаем proto в bind-event), всё равно полезно:
				udpSetOwner(dport, p)

			case EV_CONNECT:
				if uint8(ev.Proto) != IPPROTO_TCP {
					continue
				}
				key := ConnKey{
					Family:   uint16(ev.Family),
					ClientIP: srcIPKey, ClientPort: sport,
					ServerIP: dstIPKey, ServerPort: dport,
				}
				saveConn(key, p)

				if server, ok := lookupListen(uint16(ev.Family), dstIPKey, dport); ok {
					printTCPConnect(p, srcEp, dstEp, server)
				} else {
					savePending(key, PendingConnect{
						Client:   p,
						ClientEp: srcEp,
						ServerEp: dstEp,
					})
				}

			case EV_ACCEPT:
				if uint8(ev.Proto) != IPPROTO_TCP {
					continue
				}
				server := p

				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: dstIPKey}, server)
				var zero [16]byte
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: zero}, server)

				key := ConnKey{
					Family:   uint16(ev.Family),
					ClientIP: srcIPKey, ClientPort: sport,
					ServerIP: dstIPKey, ServerPort: dport,
				}

				if pc, ok := takePending(key); ok {
					printTCPConnect(pc.Client, pc.ClientEp, pc.ServerEp, server)
					printTCPAccept(server, pc.ServerEp, pc.ClientEp, pc.Client)
					continue
				}

				client, ok := lookupConn(key)
				if !ok {
					client = Proc{}
				}
				printTCPAccept(server, dstEp, srcEp, client)

			case EV_SENDTO:
				if uint8(ev.Proto) != IPPROTO_UDP {
					continue
				}
				// самообучение: этот процесс точно владелец своего local sport
				udpSetOwner(sport, p)

				// dst owner показываем ТОЛЬКО если dst loopback (127.0.0.1 / ::1 / 127.0.0.53 и т.п.)
				dstOwner, ok := Proc{}, false
				if dstIsLoop {
					dstOwner, ok = udpGetOwner(dport)
				}
				printUDPSend("SENDTO", p, srcEp, dstEp, dstOwner, ok)

			case EV_RECVFROM:
				if uint8(ev.Proto) != IPPROTO_UDP {
					continue
				}
				// самообучение: этот процесс точно владелец своего local dport
				udpSetOwner(dport, p)

				// src owner показываем только если src loopback
				srcOwner, ok := Proc{}, false
				if srcIsLoop {
					srcOwner, ok = udpGetOwner(sport)
				}
				printUDPRecv("RECVFROM", p, srcOwner, ok, srcEp, dstEp)
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}



