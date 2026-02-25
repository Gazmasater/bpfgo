
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
  -type trace_info -type tls_sni_event \
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





//go:build linux

package main

import (
	"bufio"
	"bytes"
	"context"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
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

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info -type tls_sni_event bpf trace.bpf.c -- -I.

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
	EV_READ     = 21
	EV_WRITE    = 22
	EV_CLOSE    = 30

	EV_SKB_OUT = 40
)

var (
	flgPerfMB  = flag.Int("perfMB", 8, "perf buffer total budget in MB")
	flgTTL     = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep   = flag.Duration("sweep", 1*time.Second, "sweep interval")
	flgOnlyPID = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring (empty=all)")

	flgResolve        = flag.Bool("resolve", true, "reverse DNS resolve (PTR)")
	flgResolveTTL     = flag.Duration("resolveTTL", 30*time.Minute, "PTR positive TTL")
	flgResolveNegTTL  = flag.Duration("resolveNegTTL", 5*time.Minute, "PTR NXDOMAIN TTL")
	flgResolveWorkers = flag.Int("resolveWorkers", 4, "PTR workers")
	flgResolveTimeout = flag.Duration("resolveTimeout", 2*time.Second, "PTR timeout")
	flgResolveQ       = flag.Int("resolveQ", 4096, "PTR queue size")
	flgHostState      = flag.Bool("hostState", true, "show alias state: pending/miss/no-ptr/skip")

	flgHostsPrefill = flag.Bool("hostsPrefill", true, "prefill from /etc/hosts")
	flgHostsFile    = flag.String("hostsFile", "/etc/hosts", "hosts file")
	flgHostsTTL     = flag.Duration("hostsTTL", 24*time.Hour, "hosts prefill TTL")

	flgResolveSweepEach = flag.Int("resolveSweepEach", 500, "dns cache sweep per tick")
	flgResolvePokeEach  = flag.Int("resolvePokeEach", 256, "poke flows per tick to resolve alias")

	flgL3TTL       = flag.Duration("l3ttl", 10*time.Second, "TTL for skb-derived L3 hints (cookie->src/dst)")
	flgOpenDelay   = flag.Duration("openDelay", 200*time.Millisecond, "delay OPEN print to wait for skb hint")
	flgL3SweepEach = flag.Int("l3SweepEach", 500, "l3 hint sweep per tick")
)

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

func isSend(ev uint8) bool { return ev == EV_SENDTO || ev == EV_SENDMSG || ev == EV_WRITE }
func isRecv(ev uint8) bool { return ev == EV_RECVFROM || ev == EV_RECVMSG || ev == EV_READ }

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
func fmtIPv4FromKey(k [16]byte) string { return fmt.Sprintf("%d.%d.%d.%d", k[0], k[1], k[2], k[3]) }

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
func isIPv6LinkLocalMulticast(ip [16]byte) bool { return ip[0] == 0xff && (ip[1]&0x0f) == 0x02 }
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

/* ================= DNS PTR ================= */

type dnsKey struct {
	Family uint16
	IP     [16]byte
}
type dnsEntry struct {
	Name    string
	Exp     time.Time
	Pending bool
	Neg     bool
}
type dnsResolver struct {
	mu     sync.Mutex
	m      map[dnsKey]*dnsEntry
	q      chan dnsKey
	ttl    time.Duration
	negTtl time.Duration
	to     time.Duration
	wg     sync.WaitGroup
}

func newDNSResolver(qsize, workers int, ttl, negTtl, timeout time.Duration) *dnsResolver {
	if workers < 1 {
		workers = 1
	}
	if qsize < 64 {
		qsize = 64
	}
	r := &dnsResolver{
		m:      make(map[dnsKey]*dnsEntry, 8192),
		q:      make(chan dnsKey, qsize),
		ttl:    ttl,
		negTtl: negTtl,
		to:     timeout,
	}
	r.wg.Add(workers)
	for i := 0; i < workers; i++ {
		go func() { defer r.wg.Done(); r.worker() }()
	}
	return r
}
func (r *dnsResolver) Close() { close(r.q); r.wg.Wait() }

func ipToNetIP(family uint16, ip [16]byte) net.IP {
	if family == AF_INET {
		return net.IPv4(ip[0], ip[1], ip[2], ip[3]).To4()
	}
	b := make([]byte, 16)
	copy(b, ip[:])
	return net.IP(b)
}
func shouldResolveIP(family uint16, ip [16]byte) bool {
	if isAllZero16(ip) {
		return false
	}
	nip := ipToNetIP(family, ip)
	if nip == nil {
		return false
	}
	if nip.IsLoopback() || nip.IsMulticast() || nip.IsUnspecified() {
		return false
	}
	if family == AF_INET6 && needsScope6(ip) {
		return false
	}
	return true
}
func trimDot(s string) string {
	s = strings.TrimSpace(s)
	for strings.HasSuffix(s, ".") {
		s = strings.TrimSuffix(s, ".")
	}
	return s
}

func (r *dnsResolver) SetKnown(family uint16, ip [16]byte, name string, ttl time.Duration) {
	name = trimDot(name)
	if name == "" {
		return
	}
	k := dnsKey{Family: family, IP: ip}
	r.mu.Lock()
	r.m[k] = &dnsEntry{Name: name, Exp: time.Now().Add(ttl)}
	r.mu.Unlock()
}

func (r *dnsResolver) Get(family uint16, ip [16]byte) (string, bool) {
	if !shouldResolveIP(family, ip) {
		return "", false
	}
	k := dnsKey{Family: family, IP: ip}
	now := time.Now()

	r.mu.Lock()
	defer r.mu.Unlock()

	e := r.m[k]
	if e == nil {
		return "", false
	}
	if now.After(e.Exp) && !e.Pending {
		delete(r.m, k)
		return "", false
	}
	if e.Neg || e.Name == "" {
		return "", false
	}
	return e.Name, true
}

func (r *dnsResolver) Peek(family uint16, ip [16]byte) (name string, pending bool, neg bool, ok bool) {
	if !shouldResolveIP(family, ip) {
		return "", false, false, false
	}
	k := dnsKey{Family: family, IP: ip}
	now := time.Now()

	r.mu.Lock()
	defer r.mu.Unlock()

	e := r.m[k]
	if e == nil {
		return "", false, false, false
	}
	if now.After(e.Exp) && !e.Pending {
		delete(r.m, k)
		return "", false, false, false
	}
	return e.Name, e.Pending, e.Neg, true
}

func (r *dnsResolver) Ensure(family uint16, ip [16]byte) {
	if !shouldResolveIP(family, ip) {
		return
	}
	k := dnsKey{Family: family, IP: ip}
	now := time.Now()

	r.mu.Lock()
	if e := r.m[k]; e != nil {
		if e.Pending || now.Before(e.Exp) {
			r.mu.Unlock()
			return
		}
		e.Pending = true
		e.Name = ""
		e.Neg = false
		r.mu.Unlock()
	} else {
		r.m[k] = &dnsEntry{Pending: true}
		r.mu.Unlock()
	}

	select {
	case r.q <- k:
	default:
		r.mu.Lock()
		if e := r.m[k]; e != nil {
			e.Pending = false
			e.Exp = time.Now().Add(500 * time.Millisecond)
		}
		r.mu.Unlock()
	}
}

func (r *dnsResolver) worker() {
	for k := range r.q {
		ip := ipToNetIP(k.Family, k.IP)
		name := ""
		neg := false
		retrySoon := false

		ctx, cancel := context.WithTimeout(context.Background(), r.to)
		names, err := net.DefaultResolver.LookupAddr(ctx, ip.String())
		cancel()

		if err != nil {
			if dnsErr, ok := err.(*net.DNSError); ok {
				if dnsErr.IsNotFound {
					neg = true
				} else {
					retrySoon = true
				}
			} else {
				retrySoon = true
			}
		}

		if len(names) > 0 {
			name = trimDot(names[0])
			if name == "" {
				neg = true
				retrySoon = false
			}
		} else if !retrySoon {
			neg = true
		}

		exp := time.Now().Add(r.ttl)
		if neg {
			exp = time.Now().Add(r.negTtl)
		} else if retrySoon {
			exp = time.Now().Add(2 * time.Second)
		}

		r.mu.Lock()
		e := r.m[k]
		if e == nil {
			e = &dnsEntry{}
			r.m[k] = e
		}
		e.Pending = false
		e.Neg = neg
		e.Name = name
		e.Exp = exp
		r.mu.Unlock()
	}
}

func (r *dnsResolver) SweepExpired(limit int) {
	if limit <= 0 {
		return
	}
	now := time.Now()
	n := 0
	r.mu.Lock()
	for k, e := range r.m {
		if !e.Pending && now.After(e.Exp) {
			delete(r.m, k)
			n++
			if n >= limit {
				break
			}
		}
	}
	r.mu.Unlock()
}

func parseHostsPrefill(path string, ttl time.Duration, r *dnsResolver) {
	f, err := os.Open(path)
	if err != nil {
		log.Printf("hostsPrefill: open %s: %v", path, err)
		return
	}
	defer f.Close()

	sc := bufio.NewScanner(f)
	added := 0
	for sc.Scan() {
		line := sc.Text()
		if i := strings.IndexByte(line, '#'); i >= 0 {
			line = line[:i]
		}
		line = strings.TrimSpace(line)
		if line == "" {
			continue
		}
		fields := strings.Fields(line)
		if len(fields) < 2 {
			continue
		}
		ipStr := fields[0]
		name := fields[1]
		ip := net.ParseIP(ipStr)
		if ip == nil {
			continue
		}
		if v4 := ip.To4(); v4 != nil {
			var key [16]byte
			copy(key[:4], v4)
			r.SetKnown(AF_INET, key, name, ttl)
			added++
			continue
		}
		if v6 := ip.To16(); v6 != nil {
			var key [16]byte
			copy(key[:], v6)
			r.SetKnown(AF_INET6, key, name, ttl)
			added++
		}
	}
	log.Printf("hostsPrefill: added=%d from %s", added, path)
}

var dnsr *dnsResolver

/* ================= TLS SNI cache + parser ================= */

type sniCache struct {
	mu sync.Mutex
	m  map[uint64]string
}

func (c *sniCache) Set(cookie uint64, host string) {
	if cookie == 0 || host == "" {
		return
	}
	c.mu.Lock()
	if c.m == nil {
		c.m = make(map[uint64]string, 4096)
	}
	c.m[cookie] = host
	c.mu.Unlock()
}
func (c *sniCache) Get(cookie uint64) (string, bool) {
	c.mu.Lock()
	defer c.mu.Unlock()
	if c.m == nil {
		return "", false
	}
	v, ok := c.m[cookie]
	return v, ok
}
func (c *sniCache) Del(cookie uint64) {
	c.mu.Lock()
	if c.m != nil {
		delete(c.m, cookie)
	}
	c.mu.Unlock()
}

var snic sniCache

func parseTLSClientHelloSNI(b []byte) (string, bool) {
	if len(b) < 6 || b[0] != 0x16 || b[1] != 0x03 {
		return "", false
	}
	recLen := int(b[3])<<8 | int(b[4])
	if 5+recLen > len(b) {
		return "", false
	}
	hs := b[5 : 5+recLen]
	if len(hs) < 4 || hs[0] != 0x01 {
		return "", false
	}
	hsLen := int(hs[1])<<16 | int(hs[2])<<8 | int(hs[3])
	if 4+hsLen > len(hs) {
		return "", false
	}
	p := hs[4 : 4+hsLen]
	need := func(n int) bool { return len(p) >= n }
	adv := func(n int) []byte { x := p[:n]; p = p[n:]; return x }

	if !need(2 + 32 + 1) {
		return "", false
	}
	adv(2)  // ver
	adv(32) // random

	sidLen := int(adv(1)[0])
	if !need(sidLen + 2) {
		return "", false
	}
	adv(sidLen)

	if !need(2) {
		return "", false
	}
	csLen := int(p[0])<<8 | int(p[1])
	adv(2)
	if !need(csLen + 1) {
		return "", false
	}
	adv(csLen)

	compLen := int(adv(1)[0])
	if !need(compLen + 2) {
		return "", false
	}
	adv(compLen)

	if !need(2) {
		return "", false
	}
	extLen := int(p[0])<<8 | int(p[1])
	adv(2)
	if !need(extLen) {
		return "", false
	}
	exts := p[:extLen]

	for len(exts) >= 4 {
		typ := int(exts[0])<<8 | int(exts[1])
		ln := int(exts[2])<<8 | int(exts[3])
		exts = exts[4:]
		if ln > len(exts) {
			return "", false
		}
		body := exts[:ln]
		exts = exts[ln:]

		if typ != 0x0000 { // server_name
			continue
		}
		if len(body) < 2 {
			return "", false
		}
		listLen := int(body[0])<<8 | int(body[1])
		body = body[2:]
		if listLen > len(body) {
			return "", false
		}
		body = body[:listLen]
		if len(body) < 3 || body[0] != 0x00 {
			return "", false
		}
		hostLen := int(body[1])<<8 | int(body[2])
		body = body[3:]
		if hostLen > len(body) {
			return "", false
		}
		host := string(body[:hostLen])
		if host == "" {
			return "", false
		}
		return host, true
	}
	return "", false
}

/* ================= L3 hint ================= */

type l3Info struct {
	Family uint16
	Proto  uint8
	Src    [16]byte
	Sport  uint16
	SrcSc  uint32
	Dst    [16]byte
	Dport  uint16
	DstSc  uint32
	Seen   time.Time
}
func (l l3Info) expired(now time.Time, ttl time.Duration) bool { return now.Sub(l.Seen) > ttl }

/* ================= alias / endpoint ================= */

func specialAlias(family uint16, ip [16]byte) (string, bool) {
	if family == AF_INET {
		if ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 1 {
			return "localhost", true
		}
		if ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 53 {
			return "dnsstub", true
		}
	}
	return "", false
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

func aliasForIP(family uint16, ip [16]byte) string {
	if isAllZero16(ip) {
		return "any"
	}
	if a, ok := specialAlias(family, ip); ok {
		return a
	}
	if dnsr == nil {
		return "?"
	}
	if !shouldResolveIP(family, ip) {
		return "skip"
	}
	if h, ok := dnsr.Get(family, ip); ok && h != "" {
		return h
	}
	if name, pending, neg, ok := dnsr.Peek(family, ip); ok {
		if name != "" && !neg {
			return name
		}
		if pending {
			if *flgHostState {
				return "pending"
			}
			return "?"
		}
		if neg {
			if *flgHostState {
				return "no-ptr"
			}
			return "?"
		}
	}
	dnsr.Ensure(family, ip)
	if *flgHostState {
		return "miss"
	}
	return "?"
}

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

/* ================= Flow ================= */

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

	RemoteHost string // cached real PTR (not miss/pending/no-ptr)
}

func makeKey(ev bpfTraceInfo) FlowKey {
	k := FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}
	evt := uint8(ev.Event)

	// UDP/ICMP: pin peer when known (avoid mixing all remote peers in one flow)
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
		if !isAllZero16(remote) && (k.Proto != IPPROTO_UDP || rport != 0) {
			k.PeerMode = 1
			k.Remote = remote
			k.Rport = rport
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
	// match either direction when remote known
	if !isAllZero16(f.Remote) {
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
	// best-effort: fill local from src
	if isAllZero16(f.Local) && !isAllZero16(h.Src) {
		f.Local = h.Src
		f.LocalScope = h.SrcSc
		if f.Lport == 0 && h.Sport != 0 {
			f.Lport = h.Sport
		}
	}
}

func flowReadyBase(f *Flow) bool {
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
	if !flowReadyBase(f) {
		return false
	}
	if isAllZero16(f.Local) && time.Since(f.FirstSeen) < *flgOpenDelay {
		return false
	}
	return true
}

var lostTotal uint64
var lostGen uint64

func maybeLostNote(f *Flow) string {
	if f.InBytes == 0 && f.OutBytes == 0 && f.GenStart != atomic.LoadUint64(&lostGen) {
		return " maybe_lost=1"
	}
	return ""
}
func incompleteNote(f *Flow) string {
	switch f.Key.Proto {
	case IPPROTO_TCP, IPPROTO_UDP:
		if isAllZero16(f.Remote) || f.Lport == 0 || f.Rport == 0 {
			return " incomplete=1"
		}
	case IPPROTO_ICMP, IPPROTO_ICMPV6:
		if isAllZero16(f.Remote) {
			return " incomplete=1"
		}
	}
	return ""
}

func remoteAliasCached(f *Flow) string {
	// already got good PTR
	if f.RemoteHost != "" {
		return f.RemoteHost
	}

	a := aliasForIP(f.Key.Family, f.Remote)

	// SNI fallback: only when PTR says "no-ptr" and proto is TCP
	if f.Key.Proto == IPPROTO_TCP && a == "no-ptr" {
		if s, ok := snic.Get(f.Key.Cookie); ok && s != "" {
			return s
		}
	}

	// cache only “real” names
	switch a {
	case "", "?", "any", "skip", "pending", "no-ptr", "miss":
		return a
	default:
		f.RemoteHost = a
		return a
	}
}

func printOpen(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		incompleteNote(f),
		maybeLostNote(f),
	)
}
func printClose(f *Flow, reason string) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpointAll(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpointAll(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		incompleteNote(f),
		maybeLostNote(f),
	)
}

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
	// round down to pow2 pages
	p2 := 1
	for p2*2 <= pages {
		p2 *= 2
	}
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

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if *flgResolve {
		dnsr = newDNSResolver(*flgResolveQ, *flgResolveWorkers, *flgResolveTTL, *flgResolveNegTTL, *flgResolveTimeout)
		defer dnsr.Close()
		if *flgHostsPrefill {
			parseHostsPrefill(*flgHostsFile, *flgHostsTTL, dnsr)
		}
	}

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("load bpf objects: %v", err)
	}
	defer objs.Close()

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

	attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter)
	attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit)
	attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter)
	attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit)

	attach("syscalls", "sys_enter_close", objs.TraceCloseEnter)

	attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter)
	attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit)
	attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter)
	attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit)

	attach("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter)
	attach("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit)
	attach("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter)
	attach("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit)

	attach("syscalls", "sys_enter_write", objs.TraceWriteEnter)
	attach("syscalls", "sys_exit_write", objs.TraceWriteExit)
	attach("syscalls", "sys_enter_read", objs.TraceReadEnter)
	attach("syscalls", "sys_exit_read", objs.TraceReadExit)

	attach("net", "net_dev_queue", objs.TraceNetDevQueue)

	rd, perCPUBytes, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("perf reader trace: %v", err)
	}
	defer rd.Close()

	rdTLS, _, err := openPerfReaderTotalBudget(objs.TlsEvents, 2)
	if err != nil {
		log.Fatalf("perf reader tls: %v", err)
	}
	defer rdTLS.Close()

	log.Printf("perf trace per-cpu=%dKB total~=%dMB cpus=%d",
		perCPUBytes/1024, (perCPUBytes*runtime.NumCPU())/(1024*1024), runtime.NumCPU())

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	type evWrap struct {
		ev  bpfTraceInfo
		now time.Time
	}
	evCh := make(chan evWrap, 16384)

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
				total := atomic.AddUint64(&lostTotal, rec.LostSamples)
				gen := atomic.AddUint64(&lostGen, 1)
				log.Printf("PERF_LOST chunk=%d total=%d gen=%d", rec.LostSamples, total, gen)
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}
			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			evCh <- evWrap{ev: ev, now: time.Now()}
		}
	}()

	// TLS SNI reader
	go func() {
		for {
			rec, e := rdTLS.Read()
			if e != nil {
				if errors.Is(e, perf.ErrClosed) {
					return
				}
				continue
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTlsSniEvent{})) {
				continue
			}
			ev := *(*bpfTlsSniEvent)(unsafe.Pointer(&rec.RawSample[0]))
			n := int(ev.N)
			if n <= 0 {
				continue
			}
			if n > len(ev.Data) {
				n = len(ev.Data)
			}
			if host, ok := parseTLSClientHelloSNI(ev.Data[:n]); ok {
				snic.Set(ev.Cookie, host)
			}
		}
	}()

	selfName := filepathBase(os.Args[0])

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

	flows := make(map[FlowKey]*Flow, 8192)
	l3ByCookie := make(map[uint64]l3Info, 8192)

	closeByCookie := func(tgid uint32, cookie uint64, reason string) {
		for k, f := range flows {
			if k.TGID == tgid && k.Cookie == cookie {
				if h, ok := l3ByCookie[cookie]; ok {
					applyL3HintToFlow(f, h)
				}
				if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
					printOpen(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printClose(f, reason)
				}
				delete(flows, k)
			}
		}
		snic.Del(cookie)
	}

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	log.Println("OPEN/CLOSE (TCP/UDP/ICMP) + PTR + skb-hint + SNI(no-ptr fallback). Ctrl+C to exit")

	lastLost := uint64(0)
	lastTick := time.Now()

	for {
		select {
		case <-stop:
			_ = rd.Close()
			_ = rdTLS.Close()
			log.Printf("PERF_LOST_TOTAL total=%d gen=%d", atomic.LoadUint64(&lostTotal), atomic.LoadUint64(&lostGen))
			for _, f := range flows {
				if f.OpenedPrinted {
					printClose(f, "signal")
				}
			}
			log.Println("Exiting...")
			return

		case <-ticker.C:
			now := time.Now()

			total := atomic.LoadUint64(&lostTotal)
			delta := total - lastLost
			dt := now.Sub(lastTick)
			if delta > 0 {
				log.Printf("PERF_LOST_RATE lost=%d in=%s total=%d gen=%d evCh=%d/%d flows=%d",
					delta, dt.Truncate(time.Millisecond), total, atomic.LoadUint64(&lostGen),
					len(evCh), cap(evCh), len(flows),
				)
			}
			lastLost = total
			lastTick = now

			if dnsr != nil && *flgResolveSweepEach > 0 {
				dnsr.SweepExpired(*flgResolveSweepEach)
			}

			// sweep L3 hints
			if *flgL3SweepEach > 0 {
				n := 0
				for c, h := range l3ByCookie {
					if h.expired(now, *flgL3TTL) {
						delete(l3ByCookie, c)
						n++
						if n >= *flgL3SweepEach {
							break
						}
					}
				}
			}

			// poke resolve + l3 fill
			if *flgResolvePokeEach > 0 {
				n := 0
				for _, f := range flows {
					if h, ok := l3ByCookie[f.Key.Cookie]; ok {
						applyL3HintToFlow(f, h)
					}
					_ = remoteAliasCached(f)
					n++
					if n >= *flgResolvePokeEach {
						break
					}
				}
			}

			// TTL sweep flows
			for k, f := range flows {
				if now.Sub(f.LastSeen) > *flgTTL {
					if h, ok := l3ByCookie[f.Key.Cookie]; ok {
						applyL3HintToFlow(f, h)
					}
					if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
						printOpen(f)
						f.OpenedPrinted = true
					}
					if f.OpenedPrinted {
						printClose(f, "idle")
					}
					delete(flows, k)
					snic.Del(f.Key.Cookie)
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

			// skb-out hint
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
				continue
			}

			key := makeKey(ev)
			f := flows[key]
			if f == nil {
				f = &Flow{
					Key:       key,
					Comm:      comm,
					FirstSeen: w.now,
					LastSeen:  w.now,
					GenStart:  atomic.LoadUint64(&lostGen),
				}
				flows[key] = f
			}

			f.LastSeen = w.now
			applyEndpoints(f, ev)

			if h, ok := l3ByCookie[f.Key.Cookie]; ok {
				applyL3HintToFlow(f, h)
			}

			// accounting
			if isSend(evt) && ev.Ret > 0 {
				f.OutBytes += uint64(ev.Ret)
				f.OutPkts++
			} else if isRecv(evt) && ev.Ret > 0 {
				f.InBytes += uint64(ev.Ret)
				f.InPkts++
			}

			if !f.OpenedPrinted && flowReadyToPrintOpen(f) {
				printOpen(f)
				f.OpenedPrinted = true
			}
		}
	}
}

func filepathBase(p string) string {
	i := strings.LastIndexByte(p, '/')
	if i < 0 {
		return p
	}
	return p[i+1:]
}





//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_core_read.h"
#include "bpf/bpf_tracing.h"
#include <bpf/bpf_helpers.h>

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#define AF_INET  2
#define AF_INET6 10

#define IPPROTO_ICMP   1
#define IPPROTO_TCP    6
#define IPPROTO_UDP    17
#define IPPROTO_ICMPV6 58

#define EINPROGRESS 115
#define EALREADY    114

#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_BINDOK    20
#define EV_SENDMSG   11
#define EV_RECVMSG   12
#define EV_READ      21
#define EV_WRITE     22
#define EV_CLOSE     30
#define EV_SKB_OUT   40

#define EV_TLS_SNI   41
#define MAX_SNI_BYTES 512

#define MSG_PEEK 0x2

#define S_IFMT   0170000
#define S_IFSOCK 0140000

struct tp_net_dev_queue {
    __u16 common_type;
    __u8  common_flags;
    __u8  common_preempt_count;
    __s32 common_pid;

    __u64 skbaddr;
    __u32 len;
    char  name[IFNAMSIZ];
};

struct conn_info_t {
    __u32 tgid;
    __u32 fd;
    char  comm[64];
};

struct fd_key_t {
    __u32 tgid;
    __s32 fd;
};

struct fd_state_t {
    __u16 family;
    __u8  proto;
    __u8  _pad0;

    __u16 lport;   // host order
    __u16 rport;   // host order

    __u32 lip;     // net order
    __u32 rip;     // net order

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct addr_ptrlen_t {
    __u64 addr;
    __u32 len;
    __u32 _pad;
};

struct addr_recv_meta_t {
    __u64 addr;
    __u64 lenp;
    __u32 flags;
    __u32 _pad;
};

struct msg_ptrflags_t {
    __u64 msg;
    __u32 flags;
    __u32 _pad;
};

struct trace_info {
    __u64 ts_ns;
    __u64 cookie;

    __u32 tgid;
    __u32 tid;

    __u32 fd;
    __s32 _pad0;

    __s64 ret;

    __u16 family;
    __u16 sport;
    __u16 dport;

    __u8  proto;
    __u8  event;
    __u8  state;
    __u8  _pad1;

    __u32 src_ip4;
    __u32 dst_ip4;
    __u8  src_ip6[16];
    __u8  dst_ip6[16];

    __u32 src_scope;
    __u32 dst_scope;

    char  comm[32];
};

struct tls_sni_event {
    __u64 ts_ns;
    __u64 cookie;
    __u32 tgid;
    __u32 tid;

    __u16 n;
    __u16 _pad0;

    __u8  data[MAX_SNI_BYTES];
    char  comm[32];
};

struct user_iovec64 {
    __u64 iov_base;
    __u64 iov_len;
};
struct user_msghdr64 {
    __u64 msg_name;
    __u32 msg_namelen;
    __u32 _pad0;

    __u64 msg_iov;
    __u64 msg_iovlen;

    __u64 msg_control;
    __u64 msg_controllen;

    __u32 msg_flags;
    __u32 _pad1;
};

struct write_buf_t {
    __u64 buf;
    __u64 len;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, struct fd_state_t);
} fd_state_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64); // pid_tgid
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_ptrlen_t);
} addrConnect_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_ptrlen_t);
} addrBind_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_ptrlen_t);
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct addr_recv_meta_t);
} addrRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, __u64); // user msghdr*
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct msg_ptrflags_t);
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct write_buf_t);
} write_buf_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, __u64); // cookie
    __type(value, __u8); // tries
} sni_try_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} tls_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

static __always_inline int read_sys_exit_ret(struct trace_event_raw_sys_exit *ctx, __s64 *ret) {
    return BPF_CORE_READ_INTO(ret, ctx, ret);
}

/* ===== fd helpers ===== */

static __always_inline struct file *file_from_fd(int fd) {
    if (fd < 0) return 0;
    struct task_struct *task = (struct task_struct *)bpf_get_current_task_btf();
    if (!task) return 0;

    struct files_struct *files = BPF_CORE_READ(task, files);
    if (!files) return 0;

    struct fdtable *fdt = BPF_CORE_READ(files, fdt);
    if (!fdt) return 0;

    int max_fds = BPF_CORE_READ(fdt, max_fds);
    if (fd >= max_fds) return 0;

    struct file **fd_array = BPF_CORE_READ(fdt, fd);
    if (!fd_array) return 0;

    struct file *file = 0;
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file) return 0;
    return file;
}

static __always_inline int is_socket_fd(int fd) {
    struct file *file = file_from_fd(fd);
    if (!file) return 0;

    struct inode *inode = BPF_CORE_READ(file, f_inode);
    if (!inode) return 0;

    __u16 mode = BPF_CORE_READ(inode, i_mode);
    return (mode & S_IFMT) == S_IFSOCK;
}

/* cookie = socket inode number */
static __always_inline __u64 cookie_from_fd(int fd) {
    struct file *file = file_from_fd(fd);
    if (!file) return 0;
    struct inode *inode = BPF_CORE_READ(file, f_inode);
    if (!inode) return 0;
    return (__u64)BPF_CORE_READ(inode, i_ino);
}

static __always_inline struct sock *sock_from_fd(int fd) {
    struct file *file = file_from_fd(fd);
    if (!file) return 0;
    void *pd = BPF_CORE_READ(file, private_data);
    struct socket *sock = (struct socket *)pd;
    if (!sock) return 0;
    return BPF_CORE_READ(sock, sk);
}

static __always_inline __u32 ipv4_local_addr(struct sock *sk) {
    __u32 a = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
    if (a == 0) {
        struct inet_sock *inet = (struct inet_sock *)sk;
        a = BPF_CORE_READ(inet, inet_saddr);
    }
    return a;
}

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st) {
    struct sock *sk = sock_from_fd(fd);
    if (!sk) return -1;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);

    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);
    __u16 dport_be = BPF_CORE_READ(sk, __sk_common.skc_dport);
    st->rport = bpf_ntohs(dport_be);

    if (st->family == AF_INET) {
        st->lip = ipv4_local_addr(sk);
        st->rip = BPF_CORE_READ(sk, __sk_common.skc_daddr);
        return 0;
    }
    if (st->family == AF_INET6) {
        if (BPF_CORE_READ_INTO(&st->lip6, sk, __sk_common.skc_v6_rcv_saddr) < 0) return -1;
        if (BPF_CORE_READ_INTO(&st->rip6, sk, __sk_common.skc_v6_daddr) < 0) return -1;
        return 0;
    }
    return -1;
}

static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send_dir) {
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t tmp = {};
    struct fd_state_t *st = bpf_map_lookup_elem(&fd_state_map, &k);
    if (!st) {
        if (fill_fd_state(fd, &tmp) < 0) return -1;
        bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
        st = &tmp;
    }

    info->proto  = st->proto;
    info->family = st->family;

    if (st->family == AF_INET) {
        if (is_send_dir) {
            info->src_ip4 = st->lip;
            info->dst_ip4 = st->rip;
            info->sport   = st->lport;
            info->dport   = st->rport;
        } else {
            info->src_ip4 = st->rip;
            info->dst_ip4 = st->lip;
            info->sport   = st->rport;
            info->dport   = st->lport;
        }
        return 0;
    }

    if (st->family == AF_INET6) {
        if (is_send_dir) {
            __builtin_memcpy(info->src_ip6, &st->lip6, 16);
            __builtin_memcpy(info->dst_ip6, &st->rip6, 16);
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            __builtin_memcpy(info->src_ip6, &st->rip6, 16);
            __builtin_memcpy(info->dst_ip6, &st->lip6, 16);
            info->sport = st->rport;
            info->dport = st->lport;
        }
        return 0;
    }

    return -1;
}

static __always_inline void fill_ids_comm_cookie(struct trace_info *info, __u64 pid_tgid, int fd, const char *comm64_opt) {
    info->ts_ns  = bpf_ktime_get_ns();
    info->tgid   = pid_tgid >> 32;
    info->tid    = (__u32)pid_tgid;
    info->cookie = cookie_from_fd(fd);

    if (comm64_opt) __builtin_memcpy(info->comm, comm64_opt, sizeof(info->comm));
    else bpf_get_current_comm(info->comm, sizeof(info->comm));
}

/* ===== sockaddr user (AF_INET/AF_INET6) ===== */

static __always_inline int fill_from_sockaddr_user(struct trace_info *info, const void *uaddr, __u32 addrlen, int fill_dst) {
    __u16 family = 0;
    if (!uaddr || addrlen < sizeof(__u16)) return -1;
    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0) return -1;

    if (info->family == 0) info->family = family;

    if (family == AF_INET) {
        if (addrlen < sizeof(struct sockaddr_in)) return -1;
        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0) return -1;
        __u16 port = bpf_ntohs(sa.sin_port);
        if (fill_dst) { info->dst_ip4 = sa.sin_addr.s_addr; if (port) info->dport = port; }
        else         { info->src_ip4 = sa.sin_addr.s_addr; if (port) info->sport = port; }
        return 0;
    }

    if (family == AF_INET6) {
        if (addrlen < sizeof(struct sockaddr_in6)) return -1;
        struct sockaddr_in6 sa6 = {};
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0) return -1;
        __u16 port = bpf_ntohs(sa6.sin6_port);
        __u32 scope = sa6.sin6_scope_id;
        if (fill_dst) {
            __builtin_memcpy(info->dst_ip6, &sa6.sin6_addr, 16);
            if (port) info->dport = port;
            if (scope) info->dst_scope = scope;
        } else {
            __builtin_memcpy(info->src_ip6, &sa6.sin6_addr, 16);
            if (port) info->sport = port;
            if (scope) info->src_scope = scope;
        }
        return 0;
    }

    return -1;
}

/* ===== TLS SNI sniff (raw ClientHello bytes) ===== */

static __always_inline int looks_like_tls_clienthello(const __u8 *p, __u32 n) {
    if (n < 6) return 0;
    if (p[0] != 0x16) return 0;
    if (p[1] != 0x03) return 0;
    if (p[5] != 0x01) return 0; // handshake type ClientHello
    return 1;
}

static __always_inline void maybe_emit_tls_sni(void *ctx, __u64 pid_tgid, __u64 cookie, const void *ubuf, __u32 n) {
    if (!cookie || !ubuf) return;
    if (n < 80 || n > 4096) return;

    __u8 *triesp = bpf_map_lookup_elem(&sni_try_map, &cookie);
    __u8 tries = triesp ? *triesp : 0;
    if (tries >= 3) return;

    __u32 cap = n;
    if (cap > MAX_SNI_BYTES) cap = MAX_SNI_BYTES;

    __u8 hdr[6] = {};
    if (bpf_probe_read_user(hdr, sizeof(hdr), ubuf) != 0) return;
    if (!looks_like_tls_clienthello(hdr, cap)) return;

    struct tls_sni_event e = {};
    e.ts_ns  = bpf_ktime_get_ns();
    e.cookie = cookie;
    e.tgid   = pid_tgid >> 32;
    e.tid    = (__u32)pid_tgid;
    e.n      = cap;
    bpf_get_current_comm(e.comm, sizeof(e.comm));

    if (bpf_probe_read_user(e.data, cap, ubuf) != 0) return;

    tries++;
    bpf_map_update_elem(&sni_try_map, &cookie, &tries, BPF_ANY);
    bpf_perf_event_output(ctx, &tls_events, BPF_F_CURRENT_CPU, &e, sizeof(e));
}

/* ===== connect ===== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[1];
    __u32 addrlen = (__u32)ctx->args[2];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = { .addr = uaddr, .len = addrlen };
        bpf_map_update_elem(&addrConnect_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0) goto cleanup;
    if (ret < 0 && ret != -EINPROGRESS && ret != -EALREADY) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) < 0) goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct trace_info info = {};
    info.event = EV_CONNECT;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    info.proto  = st.proto;
    info.family = st.family;
    info.sport  = st.lport;
    info.dport  = st.rport;

    if (st.family == AF_INET) {
        info.src_ip4 = st.lip;
        info.dst_ip4 = st.rip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, &st.lip6, 16);
        __builtin_memcpy(info.dst_ip6, &st.rip6, 16);
    } else goto cleanup;

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void*)ap->addr, ap->len, 1);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== accept/accept4 ===== */

static __always_inline int accept_enter_common(struct trace_event_raw_sys_enter *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);
    return 0;
}

static __always_inline int accept_exit_common(struct trace_event_raw_sys_exit *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 newfd = 0;
    if (read_sys_exit_ret(ctx, &newfd) < 0 || newfd < 0) goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)newfd, &st) < 0) goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)newfd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.event = EV_ACCEPT;
    info.fd    = ci ? ci->fd : 0; // listen fd
    info.ret   = newfd;

    info.ts_ns  = bpf_ktime_get_ns();
    info.tgid   = tgid;
    info.tid    = (__u32)id;
    info.cookie = cookie_from_fd((int)newfd);

    if (ci) __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    else bpf_get_current_comm(info.comm, sizeof(info.comm));

    info.proto  = st.proto;
    info.family = st.family;

    // accept: peer -> local
    info.sport = st.rport;
    info.dport = st.lport;

    if (st.family == AF_INET) {
        info.src_ip4 = st.rip;
        info.dst_ip4 = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, &st.rip6, 16);
        __builtin_memcpy(info.dst_ip6, &st.lip6, 16);
    } else goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(ctx); }
SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }
SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx) { return accept_enter_common(ctx); }
SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx) { return accept_exit_common(ctx); }

/* ===== bind ===== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[1];
    __u32 addrlen = (__u32)ctx->args[2];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = { .addr = uaddr, .len = addrlen };
        bpf_map_update_elem(&addrBind_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret < 0) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) < 0) goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct trace_info info = {};
    info.event = EV_BINDOK;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    info.proto  = st.proto;
    info.family = st.family;

    // bind = local -> src
    info.sport = st.lport;
    if (st.family == AF_INET) info.src_ip4 = st.lip;
    else if (st.family == AF_INET6) __builtin_memcpy(info.src_ip6, &st.lip6, 16);

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrBind_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void*)ap->addr, ap->len, 0);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== sendto/recvfrom ===== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[4];
    __u32 addrlen = (__u32)ctx->args[5];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = {.addr = uaddr, .len = addrlen};
        bpf_map_update_elem(&addrSend_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct trace_info info = {};
    info.event = EV_SENDTO;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0) goto cleanup;

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void*)ap->addr, ap->len, 1);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u32 flags  = (__u32)ctx->args[3];
    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];

    if (uaddr && lenp_u) {
        struct addr_recv_meta_t m = {.addr = uaddr, .lenp = lenp_u, .flags = flags};
        bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);
    } else if (flags & MSG_PEEK) {
        struct addr_recv_meta_t m = {.addr = 0, .lenp = 0, .flags = flags};
        bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0) goto cleanup;

    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && (m->flags & MSG_PEEK)) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct trace_info info = {};
    info.event = EV_RECVFROM;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0) goto cleanup;

    if (m && m->addr && m->lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void*)m->lenp) == 0) {
            if (addrlen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void*)m->addr, addrlen, 0);
        }
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== sendmsg/recvmsg ===== */

static __always_inline int read_msghdr64(__u64 msg_u, struct user_msghdr64 *h) {
    if (!msg_u) return -1;
    return bpf_probe_read_user(h, sizeof(*h), (void*)msg_u);
}

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    if (msg_u) bpf_map_update_elem(&msgSend_map, &id, &msg_u, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct trace_info info = {};
    info.event = EV_SENDMSG;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0) goto cleanup;

    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(*msgp, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void*)mh.msg_name, mh.msg_namelen, 1);

            // SNI sniff via iov[0]
            if (info.proto == IPPROTO_TCP && mh.msg_iov && mh.msg_iovlen) {
                struct user_iovec64 iv0 = {};
                if (bpf_probe_read_user(&iv0, sizeof(iv0), (void*)mh.msg_iov) == 0 && iv0.iov_base) {
                    __u32 n = (__u32)ret;
                    __u32 l = (iv0.iov_len > 0xffffffffULL) ? 0xffffffffU : (__u32)iv0.iov_len;
                    if (n > l) n = l;
                    maybe_emit_tls_sni(ctx, id, info.cookie, (void*)iv0.iov_base, n);
                }
            }
        }
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    __u32 flags = (__u32)ctx->args[2];
    if (msg_u) {
        struct msg_ptrflags_t v = {.msg = msg_u, .flags = flags};
        bpf_map_update_elem(&msgRecv_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0) goto cleanup;

    struct msg_ptrflags_t *pv = bpf_map_lookup_elem(&msgRecv_map, &id);
    if (pv && (pv->flags & MSG_PEEK)) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct trace_info info = {};
    info.event = EV_RECVMSG;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0) goto cleanup;

    if (pv && pv->msg) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(pv->msg, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void*)mh.msg_name, mh.msg_namelen, 0);
        }
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== write/read (ONLY SOCKETS) ===== */

SEC("tracepoint/syscalls/sys_enter_write")
int trace_write_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];
    if (!is_socket_fd(fd)) return 0;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct write_buf_t wb = {};
    wb.buf = (__u64)ctx->args[1];
    wb.len = (__u64)ctx->args[2];
    bpf_map_update_elem(&write_buf_map, &id, &wb, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_write")
int trace_write_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct trace_info info = {};
    info.event = EV_WRITE;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0) goto cleanup;

    // SNI sniff from write buffer
    if (info.proto == IPPROTO_TCP) {
        struct write_buf_t *wb = bpf_map_lookup_elem(&write_buf_map, &id);
        if (wb && wb->buf && wb->len) {
            __u32 n = (__u32)ret;
            __u32 l = (wb->len > 0xffffffffULL) ? 0xffffffffU : (__u32)wb->len;
            if (n > l) n = l;
            maybe_emit_tls_sni(ctx, id, info.cookie, (void*)wb->buf, n);
        }
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&write_buf_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_read")
int trace_read_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];
    if (!is_socket_fd(fd)) return 0;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_read")
int trace_read_exit(struct trace_event_raw_sys_exit *ctx) {
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0) goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci) goto cleanup;

    struct trace_info info = {};
    info.event = EV_READ;
    info.fd    = ci->fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci->fd, ci->comm);
    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0) goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== close (ONLY SOCKETS) ===== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx) {
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    // drop cached fd state always
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);

    if (!is_socket_fd(fd)) return 0;

    // also drop SNI tries by cookie
    __u64 c = cookie_from_fd(fd);
    if (c) bpf_map_delete_elem(&sni_try_map, &c);

    struct trace_info info = {};
    info.event = EV_CLOSE;
    info.fd    = (__u32)fd;
    info.ret   = 0;

    fill_ids_comm_cookie(&info, id, fd, 0);

    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) == 0) {
        info.proto  = st.proto;
        info.family = st.family;
        info.sport  = st.lport;
        info.dport  = st.rport;

        if (st.family == AF_INET) {
            info.src_ip4 = st.lip;
            info.dst_ip4 = st.rip;
        } else if (st.family == AF_INET6) {
            __builtin_memcpy(info.src_ip6, &st.lip6, 16);
            __builtin_memcpy(info.dst_ip6, &st.rip6, 16);
        }
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }
    return 0;
}

/* ===== skb-out hint: net_dev_queue ===== */

static __always_inline __u64 inode_cookie_from_sock(struct sock *sk) {
    if (!sk) return 0;
    struct socket *sock = BPF_CORE_READ(sk, sk_socket);
    if (!sock) return 0;
    struct file *file = BPF_CORE_READ(sock, file);
    if (!file) return 0;
    struct inode *ino = BPF_CORE_READ(file, f_inode);
    if (!ino) return 0;
    return (__u64)BPF_CORE_READ(ino, i_ino);
}

SEC("tracepoint/net/net_dev_queue")
int trace_net_dev_queue(struct tp_net_dev_queue *ctx) {
    struct sk_buff *skb = (struct sk_buff *)(unsigned long)ctx->skbaddr;
    if (!skb) return 0;

    struct sock *sk = BPF_CORE_READ(skb, sk);
    if (!sk) return 0;

    __u64 cookie = inode_cookie_from_sock(sk);
    if (!cookie) return 0;

    __u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);

    struct trace_info e = {};
    e.event  = EV_SKB_OUT;
    e.cookie = cookie;
    e.family = family;

    void *head = (void*)BPF_CORE_READ(skb, head);
    __u16 nh   = BPF_CORE_READ(skb, network_header);
    __u16 th   = BPF_CORE_READ(skb, transport_header);
    if (!head) return 0;

    if (family == AF_INET) {
        struct iphdr iph = {};
        bpf_probe_read_kernel(&iph, sizeof(iph), head + nh);
        if (iph.version != 4) return 0;

        e.proto   = iph.protocol;
        e.src_ip4 = iph.saddr;
        e.dst_ip4 = iph.daddr;

        if (e.proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e.sport = bpf_ntohs(uh.source);
            e.dport = bpf_ntohs(uh.dest);
        } else if (e.proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e.sport = bpf_ntohs(thh.source);
            e.dport = bpf_ntohs(thh.dest);
        }
    } else if (family == AF_INET6) {
        struct ipv6hdr ip6h = {};
        bpf_probe_read_kernel(&ip6h, sizeof(ip6h), head + nh);

        e.proto = ip6h.nexthdr;
        __builtin_memcpy(e.src_ip6, &ip6h.saddr, 16);
        __builtin_memcpy(e.dst_ip6, &ip6h.daddr, 16);

        if (e.proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e.sport = bpf_ntohs(uh.source);
            e.dport = bpf_ntohs(uh.dest);
        } else if (e.proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e.sport = bpf_ntohs(thh.source);
            e.dport = bpf_ntohs(thh.dest);
        }
    } else {
        return 0;
    }

    if (e.proto != IPPROTO_TCP && e.proto != IPPROTO_UDP && e.proto != IPPROTO_ICMP && e.proto != IPPROTO_ICMPV6)
        return 0;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &e, sizeof(e));
    return 0;
}




lev@lev-VirtualBox:~/bpfgo$ bpf2go -output-dir . -tags linux   -type trace_info   -go-package=main -target amd6
4 bpf $(pwd)/trace.c -- -I$(pwd)
/home/lev/bpfgo/trace.c:329:17: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
    st->rport = bpf_ntohs(dport_be);
                ^
/home/lev/bpfgo/trace.c:413:22: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
        __u16 port = bpf_ntohs(sa.sin_port);
                     ^
/home/lev/bpfgo/trace.c:423:22: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
        __u16 port = bpf_ntohs(sa6.sin6_port);
                     ^
/home/lev/bpfgo/trace.c:1126:23: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
            e.sport = bpf_ntohs(uh.source);
                      ^
/home/lev/bpfgo/trace.c:1131:23: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
            e.sport = bpf_ntohs(thh.source);
                      ^
/home/lev/bpfgo/trace.c:1145:23: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
            e.sport = bpf_ntohs(uh.source);
                      ^
/home/lev/bpfgo/trace.c:1150:23: warning: implicit declaration of function 'bpf_ntohs' is invalid in C99 [-Wimplicit-function-declaration]
            e.sport = bpf_ntohs(thh.source);
                      ^
./trace.c:830:11: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
    __u64 id = bpf_get_current_pid_tgid();
          ^
./trace.c:833:11: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
    __s64 ret = 0;
          ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:829:5: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx) {
    ^
./trace.c:829:5: note: could not determine the original source location for ./trace.c:0:0
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
    struct trace_info info = {};
                      ^
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
./trace.c:839:23: error: Looks like the BPF stack limit of 512 bytes is exceeded. Please move large on stack variables into BPF per-cpu array map.
fatal error: too many errors emitted, stopping now [-ferror-limit=]
7 warnings and 20 errors generated.
