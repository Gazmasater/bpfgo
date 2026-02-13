
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




package main

import (
	"bpfgo/pkg"
	"encoding/binary"
	"errors"
	"fmt"
	"log"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
	"sync"
	"syscall"
	"time"
	"unsafe"

	"github.com/cilium/ebpf/link"
	"github.com/cilium/ebpf/perf"
	"github.com/cilium/ebpf/rlimit"
)

var objs bpfObjects

const (
	AF_INET  = 2
	AF_INET6 = 10
)

type PeerKey struct {
	Family uint16
	Port   uint16
	IP     [16]byte // for IPv4: first 4 bytes used
}

type LookupInfo struct {
	Family uint16
	Proto  uint8

	PeerIP   [16]byte
	PeerPort uint16

	LocalIP   [16]byte
	LocalPort uint16

	Seen time.Time
}

var (
	lookupMu     sync.Mutex
	lookupByPeer = make(map[PeerKey]LookupInfo, 8192)
)

var (
	resolveCache = make(map[string]string)
	cacheMu      sync.RWMutex

	commCache = make(map[[32]int8]string)
	commMu    sync.RWMutex
)

func cachedComm(c [32]int8) string {
	commMu.RLock()
	if s, ok := commCache[c]; ok {
		commMu.RUnlock()
		return s
	}
	commMu.RUnlock()

	s := pkg.Int8ToString(c)

	commMu.Lock()
	commCache[c] = s
	commMu.Unlock()
	return s
}

func resolveHost(ip net.IP) string {
	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.IsLoopback() {
		host = "localhost"
	} else {
		if ip.To4() != nil {
			host = pkg.ResolveIP(ip)
		} else {
			h, err := pkg.ResolveIP_n(ip)
			if err != nil {
				host = "unknown"
			} else {
				host = h
			}
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()
	return host
}

func ip4FromBE(u uint32) net.IP {
	var b [4]byte
	binary.BigEndian.PutUint32(b[:], u)
	return net.IP(b[:])
}

func ip6FromArr(a [16]uint8) net.IP {
	ip := make(net.IP, net.IPv6len)
	copy(ip, a[:])
	return ip
}

func protoStr(p uint8) string {
	switch p {
	case 6:
		return "TCP"
	case 17:
		return "UDP"
	default:
		return fmt.Sprintf("P%d", p)
	}
}

func saveLookupBothDirections(family uint16, proto uint8, srcIP [16]byte, srcPort uint16, dstIP [16]byte, dstPort uint16) {
	now := time.Now()

	// интерпретация #1: peer = src, local = dst
	k1 := PeerKey{Family: family, Port: srcPort, IP: srcIP}
	v1 := LookupInfo{
		Family:    family,
		Proto:     proto,
		PeerIP:    srcIP,
		PeerPort:  srcPort,
		LocalIP:   dstIP,
		LocalPort: dstPort,
		Seen:      now,
	}

	// интерпретация #2: peer = dst, local = src (на случай, если local/remote перепутаны в BPF)
	k2 := PeerKey{Family: family, Port: dstPort, IP: dstIP}
	v2 := LookupInfo{
		Family:    family,
		Proto:     proto,
		PeerIP:    dstIP,
		PeerPort:  dstPort,
		LocalIP:   srcIP,
		LocalPort: srcPort,
		Seen:      now,
	}

	lookupMu.Lock()
	lookupByPeer[k1] = v1
	lookupByPeer[k2] = v2
	lookupMu.Unlock()
}

func takeLookup(family uint16, peerIP [16]byte, peerPort uint16) (LookupInfo, bool) {
	k := PeerKey{Family: family, Port: peerPort, IP: peerIP}
	lookupMu.Lock()
	v, ok := lookupByPeer[k]
	lookupMu.Unlock()
	return v, ok
}

func cleanupLookups(ttl time.Duration) {
	t := time.NewTicker(2 * time.Second)
	defer t.Stop()

	for range t.C {
		cut := time.Now().Add(-ttl)
		lookupMu.Lock()
		for k, v := range lookupByPeer {
			if v.Seen.Before(cut) {
				delete(lookupByPeer, k)
			}
		}
		lookupMu.Unlock()
	}
}

func init() {
	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memory limit: %v", err)
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

	go cleanupLookups(3 * time.Second)

	netns, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("open netns: %v", err)
	}
	defer netns.Close()

	fmt.Printf("netns fd=%d\n", netns.Fd())
	fmt.Printf("sizeof(traceInfo)=%d\n", unsafe.Sizeof(bpfTraceInfo{}))

	links := make([]link.Link, 0, 16)
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()

	// --- Attach tracepoints correctly (two-value returns) ---
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_sendmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_sendmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_sendto: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_sendto", objs.TraceSendtoExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_sendto: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_recvmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_recvmsg: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_recvfrom: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_recvfrom: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("sock", "inet_sock_set_state", objs.TraceTcpEst, nil)
		if err != nil {
			log.Fatalf("attach inet_sock_set_state: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.AttachNetNs(int(netns.Fd()), objs.LookUp)
		if err != nil {
			log.Fatalf("attach sk_lookup: %v", err)
		}
		links = append(links, l)
	}

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	go func() {
		const buffLen = 256 * 1024
		rd, err := perf.NewReader(objs.TraceEvents, buffLen)
		if err != nil {
			log.Fatalf("perf.NewReader: %v", err)
		}
		defer rd.Close()

		selfName := filepath.Base(os.Args[0])

		for {
			record, err := rd.Read()
			if err != nil {
				if errors.Is(err, perf.ErrClosed) {
					return
				}
				log.Printf("perf read error: %v", err)
				continue
			}

			if record.LostSamples != 0 {
				log.Printf("LOST %d samples", record.LostSamples)
				continue
			}

			if len(record.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				log.Printf("invalid event size: %d", len(record.RawSample))
				continue
			}

			ev := *(*bpfTraceInfo)(unsafe.Pointer(&record.RawSample[0]))
			comm := cachedComm(ev.Comm)
			if comm == selfName {
				continue
			}

			switch ev.Sysexit {

			// sendto_exit
			case 1:
				if ev.Family == AF_INET {
					dst := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("SENDTO  pid=%d comm=%s  %s dst=%s:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				} else if ev.Family == AF_INET6 {
					dst := ip6FromArr(ev.DstIP6.In6U.U6Addr8)
					fmt.Printf("SENDTO6 pid=%d comm=%s  %s dst=[%s]:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				}

			// recvfrom_exit
			case 2:
				handleRecv("RECVFROM", ev, comm)

			// recvmsg_exit
			case 12:
				handleRecv("RECVMSG", ev, comm)

			// sendmsg_exit
			case 11:
				if ev.Family == AF_INET {
					dst := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("SENDMSG  pid=%d comm=%s  %s dst=%s:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				} else if ev.Family == AF_INET6 {
					dst := ip6FromArr(ev.DstIP6.In6U.U6Addr8)
					fmt.Printf("SENDMSG6 pid=%d comm=%s  %s dst=[%s]:%d\n",
						ev.Pid, comm, protoStr(uint8(ev.Proto)),
						dst.String(), ev.Dport)
				}

			// sk_lookup
			case 3:
				if ev.Family == AF_INET {
					var srcIP [16]byte
					var dstIP [16]byte
					binary.BigEndian.PutUint32(srcIP[:4], ev.SrcIP.S_addr)
					binary.BigEndian.PutUint32(dstIP[:4], ev.DstIP.S_addr)

					saveLookupBothDirections(
						uint16(ev.Family),
						uint8(ev.Proto),
						srcIP, uint16(ev.Sport),
						dstIP, uint16(ev.Dport),
					)
				} else if ev.Family == AF_INET6 {
					var srcIP [16]byte
					var dstIP [16]byte
					copy(srcIP[:], ev.SrcIP6.In6U.U6Addr8[:])
					copy(dstIP[:], ev.DstIP6.In6U.U6Addr8[:])

					saveLookupBothDirections(
						uint16(ev.Family),
						uint8(ev.Proto),
						srcIP, uint16(ev.Sport),
						dstIP, uint16(ev.Dport),
					)
				}

			// inet_sock_set_state (опционально)
			case 6:
				if ev.Family == AF_INET && ev.State != 0 {
					src := ip4FromBE(ev.SrcIP.S_addr)
					dst := ip4FromBE(ev.DstIP.S_addr)
					fmt.Printf("TCPSTATE pid=%d comm=%s state=%d %s:%d -> %s:%d\n",
						ev.Pid, comm, ev.State, src.String(), ev.Sport, dst.String(), ev.Dport)
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}

func handleRecv(tag string, ev bpfTraceInfo, comm string) {
	if ev.Family == AF_INET {
		peerIPBE := ev.SrcIP.S_addr
		peer := ip4FromBE(peerIPBE)
		peerPort := uint16(ev.Sport)

		var peerKeyIP [16]byte
		binary.BigEndian.PutUint32(peerKeyIP[:4], peerIPBE)

		li, ok := takeLookup(uint16(AF_INET), peerKeyIP, peerPort)
		if ok && time.Since(li.Seen) <= 3*time.Second {
			local := net.IP(li.LocalIP[:4])
			p := protoStr(li.Proto)

			dstHost := resolveHost(local)
			srcHost := resolveHost(peer)

			fmt.Printf("%-7s pid=%d comm=%s  %s  %s[%s:%d] -> %s[%s:%d]\n",
				tag, ev.Pid, comm, p,
				srcHost, peer.String(), peerPort,
				dstHost, local.String(), li.LocalPort)
			return
		}

		fmt.Printf("%-7s pid=%d comm=%s  peer=%s:%d (no lookup)\n",
			tag, ev.Pid, comm, peer.String(), peerPort)
		return
	}

	if ev.Family == AF_INET6 {
		peer := ip6FromArr(ev.SrcIP6.In6U.U6Addr8)
		peerPort := uint16(ev.Sport)

		var peerKeyIP [16]byte
		copy(peerKeyIP[:], ev.SrcIP6.In6U.U6Addr8[:])

		li, ok := takeLookup(uint16(AF_INET6), peerKeyIP, peerPort)
		if ok && time.Since(li.Seen) <= 3*time.Second {
			local := net.IP(li.LocalIP[:16])
			p := protoStr(li.Proto)

			dstHost := resolveHost(local)
			srcHost := resolveHost(peer)

			fmt.Printf("%-7s pid=%d comm=%s  %s  %s[%s:%d] -> %s[%s:%d]\n",
				tag, ev.Pid, comm, p,
				srcHost, peer.String(), peerPort,
				dstHost, local.String(), li.LocalPort)
			return
		}

		fmt.Printf("%-7s pid=%d comm=%s  peer=[%s]:%d (no lookup)\n",
			tag, ev.Pid, comm, peer.String(), peerPort)
	}
}




mv recvmsg_test.c recvmsg_test.c.txt
mv send_udp.c send_udp.c.txt



lev@lev-VirtualBox:~/bpfgo$ sudo ./bpfgo
[sudo] password for lev: 
2026/02/13 21:33:36 failed to load bpf objects: field LookUp: program look_up: load program: permission denied: dereference of modified ctx ptr R2 off=60 disallowed (64 line(s) omitted)
