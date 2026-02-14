
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

	IPPROTO_TCP = 6

	EV_CONNECT = 3
	EV_ACCEPT  = 4
	EV_BINDOK  = 20
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

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096) // (local ip,port)->server proc

	connMu  sync.Mutex
	connBy  = make(map[ConnKey]Proc, 16384) // (client<->server tuple)->client proc
	sepLine = "------------------------------------------------------------"
)

/* ===== helpers: COMM ===== */

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

/* ===== helpers: IP decode (FIX for 1.0.0.127) =====
Kernel stores __be32 bytes in memory as BE (7f 00 00 01),
but when you unsafe-cast into Go uint32 on little-endian,
you get 0x0100007f. Therefore: use LittleEndian.PutUint32
to reconstruct original bytes.
*/

func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	binary.LittleEndian.PutUint32(b[:], x) // <-- IMPORTANT
	return
}

func ip4FromU32Net(x uint32) net.IP {
	b := ip4BytesFromU32Net(x)
	return net.IP(b[:])
}

func ip6FromArr(a [16]uint8) net.IP {
	ip := make(net.IP, net.IPv6len)
	copy(ip, a[:])
	return ip
}

func endpFromEvIPv4(ipU32 uint32, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	b := ip4BytesFromU32Net(ipU32)
	copy(ipKey[:4], b[:])
	return net.IP(b[:]).String(), ipKey, port
}

func endpFromEvIPv6(ipArr [16]uint8, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	copy(ipKey[:], ipArr[:])
	return net.IP(ipArr[:]).String(), ipKey, port
}

func formatEndp(family uint16, ipStr string, port uint16) string {
	if family == AF_INET6 {
		return fmt.Sprintf("[%s]:%d", ipStr, port)
	}
	return fmt.Sprintf("%s:%d", ipStr, port)
}

/* ===== caches ===== */

func saveListen(ep EndpKey, p Proc) {
	p.Seen = time.Now()
	listenMu.Lock()
	listenBy[ep] = p
	listenMu.Unlock()
}

func lookupListen(family uint16, ip [16]byte, port uint16) (Proc, bool) {
	// exact
	k := EndpKey{Family: family, Port: port, IP: ip}
	listenMu.Lock()
	p, ok := listenBy[k]
	listenMu.Unlock()
	if ok {
		return p, true
	}

	// wildcard fallback: 0.0.0.0 / ::
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	listenMu.Lock()
	p2, ok2 := listenBy[k2]
	listenMu.Unlock()
	return p2, ok2
}

func saveConnectTuple(key ConnKey, client Proc) {
	client.Seen = time.Now()
	connMu.Lock()
	connBy[key] = client
	connMu.Unlock()
}

func lookupConnectTuple(key ConnKey) (Proc, bool) {
	connMu.Lock()
	p, ok := connBy[key]
	connMu.Unlock()
	return p, ok
}

func cleanupTTL(ttl time.Duration) {
	t := time.NewTicker(2 * time.Second)
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
	}
}

/* ===== pretty print ===== */

func printBlock(kind string, client Proc, server Proc, clientEp, serverEp string) {
	fmt.Println(sepLine)
	fmt.Printf("TCP %-7s client=%s  %s -> %s  server=%s\n", kind, client.String(), clientEp, serverEp, server.String())
	fmt.Printf("TCP %-7s server=%s  %s -> %s  client=%s\n", kind, server.String(), serverEp, clientEp, client.String())
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

/* ===== main ===== */

func main() {
	go func() {
		log.Println("pprof on :6060")
		_ = http.ListenAndServe(":6060", nil)
	}()

	defer objs.Close()
	go cleanupTTL(5 * time.Second)

	selfName := filepath.Base(os.Args[0])

	links := make([]link.Link, 0, 16)
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()

	// attach only what we need for TCP roles
	mustAttach := func(group, name string, prog interface{}) {
		p := prog.(*ebpf.Program) // not accessible here in snippet
		_ = p
	}

	// bind
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_bind", objs.TraceBindEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_bind: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_bind", objs.TraceBindExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_bind: %v", err)
		}
		links = append(links, l)
	}

	// connect
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_connect", objs.TraceConnectEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_connect: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_connect", objs.TraceConnectExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_connect: %v", err)
		}
		links = append(links, l)
	}

	// accept/accept4
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_accept4: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_accept4: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_accept", objs.TraceAcceptEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_accept: %v", err)
		}
		links = append(links, l)
	}
	{
		l, err := link.Tracepoint("syscalls", "sys_exit_accept", objs.TraceAcceptExit, nil)
		if err != nil {
			log.Fatalf("attach sys_exit_accept: %v", err)
		}
		links = append(links, l)
	}

	// close (чтобы ядро чистило fd_state_map — у тебя BPF уже делает)
	{
		l, err := link.Tracepoint("syscalls", "sys_enter_close", objs.TraceCloseEnter, nil)
		if err != nil {
			log.Fatalf("attach sys_enter_close: %v", err)
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

			// TCP only
			if uint8(ev.Proto) != IPPROTO_TCP {
				continue
			}

			comm := cachedComm(ev.Comm)
			if comm == selfName {
				continue
			}

			switch ev.Sysexit {
			case EV_BINDOK:
				// bind event: pid/comm = server
				server := Proc{Pid: ev.Pid, Comm: comm}

				if ev.Family == AF_INET {
					ipStr, ipKey, port := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)
					ep := EndpKey{Family: uint16(ev.Family), Port: port, IP: ipKey}
					saveListen(ep, server)
					// также сохраним wildcard на этот порт (на случай bind 0.0.0.0)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: port, IP: zero}, server)

					_ = ipStr // можно печатать bind, если хочешь
				} else if ev.Family == AF_INET6 {
					ipStr, ipKey, port := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)
					ep := EndpKey{Family: uint16(ev.Family), Port: port, IP: ipKey}
					saveListen(ep, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: port, IP: zero}, server)
					_ = ipStr
				}

			case EV_CONNECT:
				// connect event runs in CLIENT pid
				client := Proc{Pid: ev.Pid, Comm: comm}

				if ev.Family == AF_INET {
					cIPStr, cIPKey, cPort := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)

					clientEp := formatEndp(AF_INET, cIPStr, cPort)
					serverEp := formatEndp(AF_INET, sIPStr, sPort)

					key := ConnKey{
						Family:     uint16(ev.Family),
						ClientIP:   cIPKey, ClientPort: cPort,
						ServerIP:   sIPKey, ServerPort: sPort,
					}
					saveConnectTuple(key, client)

					server, ok := lookupListen(uint16(ev.Family), sIPKey, sPort)
					if !ok {
						server = Proc{} // "?"
					}

					printBlock("CONNECT", client, server, clientEp, serverEp)

				} else if ev.Family == AF_INET6 {
					cIPStr, cIPKey, cPort := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)

					clientEp := formatEndp(AF_INET6, cIPStr, cPort)
					serverEp := formatEndp(AF_INET6, sIPStr, sPort)

					key := ConnKey{
						Family:     uint16(ev.Family),
						ClientIP:   cIPKey, ClientPort: cPort,
						ServerIP:   sIPKey, ServerPort: sPort,
					}
					saveConnectTuple(key, client)

					server, ok := lookupListen(uint16(ev.Family), sIPKey, sPort)
					if !ok {
						server = Proc{}
					}

					printBlock("CONNECT", client, server, clientEp, serverEp)
				}

			case EV_ACCEPT:
				// accept event runs in SERVER pid
				server := Proc{Pid: ev.Pid, Comm: comm}

				// src=client, dst=server (как у тебя в BPF accept_exit_common)
				if ev.Family == AF_INET {
					cIPStr, cIPKey, cPort := endpFromEvIPv4(ev.SrcIP.S_addr, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv4(ev.DstIP.S_addr, ev.Dport)

					clientEp := formatEndp(AF_INET, cIPStr, cPort)
					serverEp := formatEndp(AF_INET, sIPStr, sPort)

					// accept помогает “узнать” server pid для порта (на случай если bind не поймали)
					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: sIPKey}, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: zero}, server)

					key := ConnKey{
						Family:     uint16(ev.Family),
						ClientIP:   cIPKey, ClientPort: cPort,
						ServerIP:   sIPKey, ServerPort: sPort,
					}
					client, ok := lookupConnectTuple(key)
					if !ok {
						client = Proc{}
					}

					printBlock("ACCEPT", client, server, clientEp, serverEp)

				} else if ev.Family == AF_INET6 {
					cIPStr, cIPKey, cPort := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, ev.Sport)
					sIPStr, sIPKey, sPort := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, ev.Dport)

					clientEp := formatEndp(AF_INET6, cIPStr, cPort)
					serverEp := formatEndp(AF_INET6, sIPStr, sPort)

					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: sIPKey}, server)
					var zero [16]byte
					saveListen(EndpKey{Family: uint16(ev.Family), Port: sPort, IP: zero}, server)

					key := ConnKey{
						Family:     uint16(ev.Family),
						ClientIP:   cIPKey, ClientPort: cPort,
						ServerIP:   sIPKey, ServerPort: sPort,
					}
					client, ok := lookupConnectTuple(key)
					if !ok {
						client = Proc{}
					}

					printBlock("ACCEPT", client, server, clientEp, serverEp)
				}
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}












