
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

	EV_SENDMSG = 11
	EV_RECVMSG = 12
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

/* ===================== keys ===================== */

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

// UDP flow key in "delivery direction": src -> dst
type UdpKey struct {
	Family uint16
	Proto  uint8
	SrcIP  [16]byte
	SrcP   uint16
	DstIP  [16]byte
	DstP   uint16
}

/* ===================== caches ===================== */

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	// Кто "владеет" портом (bind) + кто владеет эпемерным портом (видели в send/recv)
	ownerMu sync.Mutex
	ownerBy = make(map[EndpKey]Proc, 65536)

	// TCP: pending CONNECT ждёт ACCEPT
	tcpPendMu sync.Mutex
	tcpPend   = make(map[ConnKey]tcpPending, 16384)

	// UDP: pending SEND ждёт RECV (чтобы вывести парой)
	udpPendMu sync.Mutex
	udpPend   = make(map[UdpKey]udpPending, 65536)
)

type tcpPending struct {
	Client   Proc
	ClientEp string
	ServerEp string
	Key      ConnKey
	Seen     time.Time
}

type udpPending struct {
	Kind   string // SENDTO/SENDMSG
	Pid    Proc   // syscall caller (sender)
	SrcEp  string
	DstEp  string
	SrcIP  [16]byte
	DstIP  [16]byte
	SrcP   uint16
	DstP   uint16
	Family uint16
	Proto  uint8
	Seen   time.Time
}

/* ===================== comm/proto ===================== */

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

/* ===================== IP decode ===================== */

// FIX: 1.0.0.127 -> 127.0.0.1
func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	binary.LittleEndian.PutUint32(b[:], x)
	return
}

func endpFromEvIPv4(ipU32 uint32, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	b := ip4BytesFromU32Net(ipU32)
	copy(ipKey[:4], b[:])
	return fmt.Sprintf("%d.%d.%d.%d", b[0], b[1], b[2], b[3]), ipKey, port
}

func endpFromEvIPv6(ipArr [16]uint8, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	copy(ipKey[:], ipArr[:])
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

func formatEndp(family uint16, ipStr string, port uint16) string {
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	// красиво показываем 0.0.0.0 как "*"
	if ipStr == "0.0.0.0" {
		return fmt.Sprintf("*:%d", port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

func isZeroIP(ip [16]byte) bool {
	for i := 0; i < 16; i++ {
		if ip[i] != 0 {
			return false
		}
	}
	return true
}

/* ===================== owner map ===================== */

func saveOwner(family uint16, ip [16]byte, port uint16, p Proc) {
	p.Seen = time.Now()
	k := EndpKey{Family: family, Port: port, IP: ip}
	ownerMu.Lock()
	ownerBy[k] = p
	ownerMu.Unlock()

	// wildcard (0.0.0.0/::) тоже полезен
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	ownerMu.Lock()
	if _, ok := ownerBy[k2]; !ok {
		ownerBy[k2] = p
	}
	ownerMu.Unlock()
}

func lookupOwner(family uint16, ip [16]byte, port uint16) (Proc, bool) {
	k := EndpKey{Family: family, Port: port, IP: ip}
	ownerMu.Lock()
	p, ok := ownerBy[k]
	ownerMu.Unlock()
	if ok {
		return p, true
	}
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	ownerMu.Lock()
	p2, ok2 := ownerBy[k2]
	ownerMu.Unlock()
	return p2, ok2
}

/* ===================== TCP pending ===================== */

func saveTcpPending(key ConnKey, pc tcpPending) {
	pc.Seen = time.Now()
	tcpPendMu.Lock()
	tcpPend[key] = pc
	tcpPendMu.Unlock()
}

func takeTcpPending(key ConnKey) (tcpPending, bool) {
	tcpPendMu.Lock()
	v, ok := tcpPend[key]
	if ok {
		delete(tcpPend, key)
	}
	tcpPendMu.Unlock()
	return v, ok
}

/* ===================== UDP pending ===================== */

func makeUdpKey(family uint16, proto uint8, srcIP [16]byte, srcP uint16, dstIP [16]byte, dstP uint16) UdpKey {
	return UdpKey{Family: family, Proto: proto, SrcIP: srcIP, SrcP: srcP, DstIP: dstIP, DstP: dstP}
}

// Для сопоставления RECV с SEND: пробуем точный и варианты с wildcard IP (0.0.0.0 / ::)
func recvMatchKeys(family uint16, proto uint8, srcIP [16]byte, srcP uint16, dstIP [16]byte, dstP uint16) []UdpKey {
	var zero [16]byte
	return []UdpKey{
		makeUdpKey(family, proto, srcIP, srcP, dstIP, dstP),
		makeUdpKey(family, proto, zero, srcP, dstIP, dstP),
		makeUdpKey(family, proto, srcIP, srcP, zero, dstP),
		makeUdpKey(family, proto, zero, srcP, zero, dstP),
	}
}

func saveUdpPending(k UdpKey, u udpPending) {
	u.Seen = time.Now()
	udpPendMu.Lock()
	udpPend[k] = u
	udpPendMu.Unlock()
}

func takeUdpPendingByRecv(family uint16, proto uint8, srcIP [16]byte, srcP uint16, dstIP [16]byte, dstP uint16) (udpPending, bool) {
	keys := recvMatchKeys(family, proto, srcIP, srcP, dstIP, dstP)
	udpPendMu.Lock()
	defer udpPendMu.Unlock()

	for _, k := range keys {
		if v, ok := udpPend[k]; ok {
			delete(udpPend, k)
			return v, true
		}
	}
	return udpPending{}, false
}

/* ===================== printing ===================== */

func printTCPConnectLine(client Proc, clientEp, serverEp string, server Proc) {
	fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=%s\n",
		client.String(), clientEp, serverEp, server.String())
}

func printTCPAcceptLine(server Proc, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  server=%s  %s -> %s  client=%s\n",
		server.String(), serverEp, clientEp, client.String())
}

func printUDPSend(kind string, pid Proc, srcProc Proc, dstProc Proc, srcEp, dstEp string) {
	// pid = кто сделал sendto/sendmsg
	fmt.Printf("UDP %-7s pid=%s  src=%s  %s -> %s  dst=%s\n",
		kind, pid.String(), srcProc.String(), srcEp, dstEp, dstProc.String())
}

func printUDPRecv(kind string, pid Proc, srcProc Proc, dstProc Proc, srcEp, dstEp string) {
	// pid = кто сделал recvfrom/recvmsg (это dstProc)
	fmt.Printf("UDP %-7s pid=%s  src=%s  %s -> %s  dst=%s\n",
		kind, pid.String(), srcProc.String(), srcEp, dstEp, dstProc.String())
}

/* ===================== cleanup ===================== */

func cleanupLoop(tcpTTL, udpTTL time.Duration) {
	t := time.NewTicker(100 * time.Millisecond)
	defer t.Stop()

	for range t.C {
		now := time.Now()

		// TCP: если ACCEPT не пришёл — печатаем CONNECT с server=? и отделяем пустой строкой
		tcpPendMu.Lock()
		for k, v := range tcpPend {
			if now.Sub(v.Seen) > tcpTTL {
				printTCPConnectLine(v.Client, v.ClientEp, v.ServerEp, Proc{})
				fmt.Println()
				delete(tcpPend, k)
			}
		}
		tcpPendMu.Unlock()

		// UDP: если RECV не пришёл — печатаем SEND как одиночный и отделяем пустой строкой
		udpPendMu.Lock()
		for k, v := range udpPend {
			if now.Sub(v.Seen) > udpTTL {
				// попытка дорезолвить dst по порту
				dstProc, ok := lookupOwner(v.Family, v.DstIP, v.DstP)
				if !ok {
					dstProc = Proc{}
				}
				// srcProc = сам pid
				srcProc := v.Pid
				printUDPSend(v.Kind, v.Pid, srcProc, dstProc, v.SrcEp, v.DstEp)
				fmt.Println()
				delete(udpPend, k)
			}
		}
		udpPendMu.Unlock()

		// ownerBy чистим грубо (не критично) — можно оставить как есть
		ownerMu.Lock()
		for kk, vv := range ownerBy {
			if now.Sub(vv.Seen) > 30*time.Second {
				delete(ownerBy, kk)
			}
		}
		ownerMu.Unlock()
	}
}

/* ===================== init/load ===================== */

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
}

/* ===================== main ===================== */

func main() {
	go func() {
		log.Println("pprof on :6060")
		_ = http.ListenAndServe(":6060", nil)
	}()

	defer objs.Close()

	// TCP connect ждём accept дольше, UDP ждём коротко
	go cleanupLoop(2*time.Second, 250*time.Millisecond)

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

	// roles (TCP/UDP owners)
	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	// TCP
	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)
	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	// UDP / generic datagram
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

			pidProc := Proc{Pid: ev.Pid, Comm: comm}

			// endpoints from event
			var srcEp, dstEp string
			var srcIPKey, dstIPKey [16]byte
			var sport, dport uint16
			fam := uint16(ev.Family)
			pr := uint8(ev.Proto)

			if ev.Family == AF_INET {
				sIP, sKey, sP := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)
				srcEp = formatEndp(AF_INET, sIP, sP)
				dstEp = formatEndp(AF_INET, dIP, dP)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
			} else if ev.Family == AF_INET6 {
				sIP, sKey, sP := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)
				srcEp = formatEndp(AF_INET6, sIP, sP)
				dstEp = formatEndp(AF_INET6, dIP, dP)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
			} else {
				continue
			}

			switch ev.Sysexit {

			case EV_BINDOK:
				// bind: фиксируем владельца порта (proto тут может быть 0 — не важно)
				saveOwner(fam, dstIPKey, dport, pidProc)

			/* ================= TCP ================= */

			case EV_CONNECT:
				if pr != IPPROTO_TCP {
					continue
				}
				// CONNECT: pidProc = client
				key := ConnKey{
					Family:     fam,
					ClientIP:   srcIPKey,
					ClientPort: sport,
					ServerIP:   dstIPKey,
					ServerPort: dport,
				}

				// ждём ACCEPT, чтобы точно знать server pid (и вывести блок компактно)
				saveTcpPending(key, tcpPending{
					Client:   pidProc,
					ClientEp: srcEp,
					ServerEp: dstEp,
					Key:      key,
				})

			case EV_ACCEPT:
				if pr != IPPROTO_TCP {
					continue
				}
				// ACCEPT: pidProc = server, tuple src=client -> dst=server
				server := pidProc
				// фиксируем owner порта сервера (полезно и для UDP, и для внешних разрешений)
				saveOwner(fam, dstIPKey, dport, server)

				key := ConnKey{
					Family:     fam,
					ClientIP:   srcIPKey,
					ClientPort: sport,
					ServerIP:   dstIPKey,
					ServerPort: dport,
				}

				if pc, ok := takeTcpPending(key); ok {
					// CONNECT + ACCEPT одним блоком, потом пустая строка
					printTCPConnectLine(pc.Client, pc.ClientEp, pc.ServerEp, server)
					printTCPAcceptLine(server, pc.ServerEp, pc.ClientEp, pc.Client)
					fmt.Println()
				} else {
					// иногда CONNECT мог не пойматься (редко) — тогда выводим только ACCEPT
					printTCPAcceptLine(server, dstEp, srcEp, Proc{})
					fmt.Println()
				}

			/* ================= UDP (syscall mode, paired) ================= */

			case EV_SENDTO, EV_SENDMSG:
				if pr != IPPROTO_UDP {
					continue
				}

				kind := "SENDTO"
				if ev.Sysexit == EV_SENDMSG {
					kind = "SENDMSG"
				}

				// владелец локального исходящего порта = этот pid
				saveOwner(fam, srcIPKey, sport, pidProc)

				// dstProc пытаемся определить по порту (если локально)
				dstProc, ok := lookupOwner(fam, dstIPKey, dport)
				if !ok {
					// если dstIP конкретный, но у нас owner записан только wildcard — lookupOwner уже это умеет
					dstProc = Proc{}
				}

				// буферим SEND, чтобы рядом напечатать с RECV
				k := makeUdpKey(fam, pr, srcIPKey, sport, dstIPKey, dport)
				saveUdpPending(k, udpPending{
					Kind:   kind,
					Pid:    pidProc,
					SrcEp:  srcEp,
					DstEp:  dstEp,
					SrcIP:  srcIPKey,
					DstIP:  dstIPKey,
					SrcP:   sport,
					DstP:   dport,
					Family: fam,
					Proto:  pr,
					Seen:   time.Now(),
				})

				// если dstProc уже известен и addr не wildcard — можно печатать без ожидания,
				// но тогда "пара" может разъехаться. Поэтому ждём 250ms (cleanup) или RECV.

				_ = dstProc // (оставлено на будущее — при желании можно печатать сразу)

			case EV_RECVFROM, EV_RECVMSG:
				if pr != IPPROTO_UDP {
					continue
				}

				kind := "RECVFROM"
				if ev.Sysexit == EV_RECVMSG {
					kind = "RECVMSG"
				}

				// pidProc = тот, кто сделал recv* => это dstProc
				dstProc := pidProc
				// владелец локального порта (на котором получили) = этот pid
				saveOwner(fam, dstIPKey, dport, dstProc)

				// srcProc пытаемся определить по исходному порту отправителя
				srcProc, ok := lookupOwner(fam, srcIPKey, sport)
				if !ok {
					// если srcIP конкретный, но owner записан wildcard — lookupOwner уже это умеет
					srcProc = Proc{}
				}

				// пробуем найти соответствующий SEND (чтобы печатать парой)
				if pend, ok := takeUdpPendingByRecv(fam, pr, srcIPKey, sport, dstIPKey, dport); ok {
					// Для SEND line: srcProc = pend.Pid (кто сделал send)
					// dstProc для SEND пытаемся определить сейчас по факту (если локально)
					sendDstProc, ok2 := lookupOwner(fam, pend.DstIP, pend.DstP)
					if !ok2 {
						sendDstProc = Proc{}
					}
					printUDPSend(pend.Kind, pend.Pid, pend.Pid, sendDstProc, pend.SrcEp, pend.DstEp)

					// Для RECV line: srcProc (кто реально отправил датаграмму) + dstProc (recv caller)
					printUDPRecv(kind, pidProc, srcProc, dstProc, srcEp, dstEp)

					// пустая строка после пары
					fmt.Println()
				} else {
					// RECV без найденного SEND
					printUDPRecv(kind, pidProc, srcProc, dstProc, srcEp, dstEp)
					fmt.Println()
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}



UDP RECVFROM pid=16364(udp_server)  src=16394(udp_client)  127.0.0.1:42459 -> *:9999  dst=16364(udp_server)

UDP RECVFROM pid=16394(udp_client)  src=16364(udp_server)  127.0.0.1:9999 -> *:42459  dst=16394(udp_client)

UDP SENDTO  pid=16364(udp_server)  src=16364(udp_server)  *:9999 -> 127.0.0.1:42459  dst=16394(udp_client)

UDP SENDTO  pid=16394(udp_client)  src=16394(udp_client)  *:42459 -> 127.0.0.1:9999  dst=16364(udp_server)

