
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



Пересобери генерацию типов (важно, чтобы появился bpfTlsChunkT):

go generate ./...
go build -o bpfgo .
sudo ./bpfgo -comm openssl -sni=1

Тест:

# сервер (любой TLS). Например:
openssl s_server -accept 8443 -cert cert.pem -key key.pem

# клиент с SNI:
openssl s_client -connect 127.0.0.1:8443 -servername test.local </dev/null



openssl s_server -accept 8443 -cert cert.pem -key key.pem


for i in $(seq 1 5); do
  openssl s_client -connect 127.0.0.1:8443 -servername default.exp-tas.com -no_ticket </dev/null | head -n 1
done





package main

import (
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"os"
	"os/signal"
	"path/filepath"
	"runtime"
	"strings"
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

	EV_SKB_OUT = 40

	// must match trace.c
	EV_TLS_CHUNK = 50
)

var (
	flgPerfMB = flag.Int("perfMB", 8, "perf ring total budget in MB (per reader)")
	flgTTL    = flag.Duration("ttl", 5*time.Second, "idle TTL for flow close")
	flgSweep  = flag.Duration("sweep", 1*time.Second, "sweep interval")

	flgOnlyPID  = flag.Int("pid", 0, "only this pid (0=all)")
	flgOnlyComm = flag.String("comm", "", "only comm containing substring (empty=all)")

	flgMmsg = flag.Bool("mmsg", true, "trace sendmmsg/recvmmsg")
	flgRW   = flag.Bool("rw", true, "trace read/write on socket fds")

	flgSNI     = flag.Bool("sni", true, "parse SNI from TLS ClientHello")
	flgSniTTL  = flag.Duration("sniTTL", 30*time.Second, "SNI cache TTL")
	flgTlsKeep = flag.Duration("tlsKeep", 2*time.Second, "keep partial TLS buffers for this long")
)

/* --------------------- small helpers --------------------- */

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

// kernel u32 IPv4 is network-order but appears swapped on little-endian
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

func addrStr(family uint16, ip [16]byte) string {
	if isAllZero16(ip) {
		return "*"
	}
	if family == AF_INET6 {
		return fmtIPv6Full(ip)
	}
	return fmtIPv4FromKey(ip)
}

func fmtEndpoint(family uint16, ip [16]byte, port uint16, proto uint8) string {
	if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
		return addrStr(family, ip)
	}
	if family == AF_INET6 && !isAllZero16(ip) {
		return fmt.Sprintf("[%s]:%d", addrStr(family, ip), port)
	}
	return fmt.Sprintf("%s:%d", addrStr(family, ip), port)
}

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

/* --------------------- TLS SNI parsing --------------------- */

func u16(b []byte, i int) (uint16, bool) {
	if i+2 > len(b) {
		return 0, false
	}
	return uint16(b[i])<<8 | uint16(b[i+1]), true
}

// expects one TLS record starting at buf[0] (type 0x16)
func parseSNIFromTLSRecord(buf []byte) string {
	if len(buf) < 5 {
		return ""
	}
	if buf[0] != 0x16 { // Handshake
		return ""
	}
	if buf[1] != 0x03 {
		return ""
	}
	recLenU, ok := u16(buf, 3)
	if !ok {
		return ""
	}
	total := int(recLenU) + 5
	if total < 6 || len(buf) < total {
		return ""
	}

	p := 5
	if p+4 > total {
		return ""
	}
	if buf[p] != 0x01 { // ClientHello
		return ""
	}
	hsLen := int(buf[p+1])<<16 | int(buf[p+2])<<8 | int(buf[p+3])
	p += 4
	if p+hsLen > total {
		return ""
	}

	// version(2)+random(32)
	if p+34 > total {
		return ""
	}
	p += 34

	// session id
	if p+1 > total {
		return ""
	}
	sidLen := int(buf[p])
	p++
	if p+sidLen > total {
		return ""
	}
	p += sidLen

	// cipher suites
	csLenU, ok := u16(buf, p)
	if !ok {
		return ""
	}
	csLen := int(csLenU)
	p += 2
	if p+csLen > total {
		return ""
	}
	p += csLen

	// compression
	if p+1 > total {
		return ""
	}
	cmLen := int(buf[p])
	p++
	if p+cmLen > total {
		return ""
	}
	p += cmLen

	// extensions
	extLenU, ok := u16(buf, p)
	if !ok {
		return ""
	}
	extLen := int(extLenU)
	p += 2
	if p+extLen > total {
		return ""
	}
	extEnd := p + extLen

	for p+4 <= extEnd {
		typU, _ := u16(buf, p)
		lU, _ := u16(buf, p+2)
		typ := int(typU)
		l := int(lU)
		p += 4
		if p+l > extEnd {
			return ""
		}

		if typ == 0x0000 { // server_name
			if p+2 > extEnd {
				return ""
			}
			listLenU, _ := u16(buf, p)
			listLen := int(listLenU)
			q := p + 2
			end := q + listLen
			if end > p+l {
				return ""
			}
			for q+3 <= end {
				nameType := buf[q]
				q++
				nameLenU, _ := u16(buf, q)
				nameLen := int(nameLenU)
				q += 2
				if q+nameLen > end {
					return ""
				}
				if nameType == 0 {
					host := strings.TrimSpace(string(buf[q : q+nameLen]))
					if host != "" {
						return host
					}
				}
				q += nameLen
			}
			return ""
		}

		p += l
	}

	return ""
}

/* --------------------- TLS reassembly (fix for "rare SNI") --------------------- */

type tlsReasm struct {
	buf  []byte
	seen time.Time
	done bool // SNI already found for this cookie
}

const (
	tlsMaxBuf = 16 * 1024 // cap memory per cookie
)

// find TLS handshake record header in b (16 03 xx len len)
func findTLSHeader(b []byte) int {
	// naive scan is fast enough for <=16KB
	for i := 0; i+5 <= len(b); i++ {
		if b[i] == 0x16 && b[i+1] == 0x03 {
			v := b[i+2]
			if v >= 0x01 && v <= 0x04 { // TLS1.0..1.3 record version bytes
				return i
			}
		}
	}
	return -1
}

// Feed new chunk, try to extract complete TLS records and parse SNI.
// Returns (sni, ok).
func (t *tlsReasm) Feed(now time.Time, chunk []byte) (string, bool) {
	if t.done {
		return "", false
	}
	t.seen = now

	// append with cap
	if len(chunk) > 0 {
		need := len(t.buf) + len(chunk)
		if need > tlsMaxBuf {
			// keep tail (most recent) to avoid endless growth
			drop := need - tlsMaxBuf
			if drop >= len(t.buf) {
				t.buf = t.buf[:0]
			} else {
				t.buf = append([]byte(nil), t.buf[drop:]...)
			}
		}
		t.buf = append(t.buf, chunk...)
	}

	// reassembly loop: align to header, wait full record, parse, consume
	for {
		if len(t.buf) < 5 {
			return "", false
		}

		hi := findTLSHeader(t.buf)
		if hi < 0 {
			// keep last 4 bytes just in case header crosses boundary
			if len(t.buf) > 4 {
				t.buf = append([]byte(nil), t.buf[len(t.buf)-4:]...)
			}
			return "", false
		}
		if hi > 0 {
			// drop garbage before header
			t.buf = t.buf[hi:]
			if len(t.buf) < 5 {
				return "", false
			}
		}

		recLen := int(t.buf[3])<<8 | int(t.buf[4])
		total := 5 + recLen
		if recLen <= 0 || total > tlsMaxBuf {
			// invalid length -> drop 1 byte and resync
			t.buf = t.buf[1:]
			continue
		}
		if len(t.buf) < total {
			// wait more
			return "", false
		}

		rec := t.buf[:total]
		if sni := parseSNIFromTLSRecord(rec); sni != "" {
			t.done = true
			return sni, true
		}

		// consume this record and continue scanning next records
		t.buf = t.buf[total:]
	}
}

type sniEntry struct {
	Name string
	Seen time.Time
}

/* --------------------- Flow model --------------------- */

type FlowKey struct {
	TGID   uint32
	Cookie uint64
	Proto  uint8
	Family uint16
}

type Flow struct {
	Key  FlowKey
	Comm string

	Local  [16]byte
	Lport  uint16
	Remote [16]byte
	Rport  uint16

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
	return FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}
}

func applyEndpoints(f *Flow, ev bpfTraceInfo) {
	evt := uint8(ev.Event)

	switch {
	case isSend(evt) || evt == EV_CONNECT:
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Local = srcKeyFromEvent(ev)
		}
		if !isAllZero16(dstKeyFromEvent(ev)) {
			f.Remote = dstKeyFromEvent(ev)
		}
		if ev.Sport != 0 {
			f.Lport = uint16(ev.Sport)
		}
		if ev.Dport != 0 {
			f.Rport = uint16(ev.Dport)
		}

	case isRecv(evt):
		if !isAllZero16(dstKeyFromEvent(ev)) {
			f.Local = dstKeyFromEvent(ev)
		}
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Remote = srcKeyFromEvent(ev)
		}
		if ev.Dport != 0 {
			f.Lport = uint16(ev.Dport)
		}
		if ev.Sport != 0 {
			f.Rport = uint16(ev.Sport)
		}

	case evt == EV_BINDOK:
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Local = srcKeyFromEvent(ev)
		}
		if ev.Sport != 0 {
			f.Lport = uint16(ev.Sport)
		}

	case evt == EV_ACCEPT:
		if !isAllZero16(dstKeyFromEvent(ev)) {
			f.Local = dstKeyFromEvent(ev)
		}
		if !isAllZero16(srcKeyFromEvent(ev)) {
			f.Remote = srcKeyFromEvent(ev)
		}
		if ev.Dport != 0 {
			f.Lport = uint16(ev.Dport)
		}
		if ev.Sport != 0 {
			f.Rport = uint16(ev.Sport)
		}
	}
}

func flowReadyToOpen(f *Flow) bool {
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

func printOpen(f *Flow) {
	sni := ""
	if f.TLSSNI != "" {
		sni = " sni=" + f.TLSSNI
	}
	fmt.Printf("OPEN  %-5s pid=%d(%s) cookie=%d  %s -> %s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.Key.Proto),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.Key.Proto),
		sni,
	)
}

func printClose(f *Flow, reason string) {
	sni := ""
	if f.TLSSNI != "" {
		sni = " sni=" + f.TLSSNI
	}
	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)
	fmt.Printf("CLOSE %-5s pid=%d(%s) cookie=%d  %s -> %s  out=%dB/%dp in=%dB/%dp  age=%s reason=%s%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.Key.Proto),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.Key.Proto),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
		sni,
	)
}

/* --------------------- perf reader helpers --------------------- */

func openPerfReaderTotalBudget(m *ebpf.Map, totalMB int) (*perf.Reader, int, error) {
	totalBytes := totalMB * 1024 * 1024
	if totalBytes < 256*1024 {
		totalBytes = 256 * 1024
	}
	nCPU := runtime.NumCPU()
	perCPU := totalBytes / nCPU
	if perCPU < 256*1024 {
		perCPU = 256 * 1024
	}
	rd, err := perf.NewReader(m, perCPU)
	return rd, perCPU, err
}

/* --------------------- main --------------------- */

var lostTrace uint64
var lostTLS uint64
var lostGen uint64

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("rlimit.RemoveMemlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("loadBpfObjects: %v", err)
	}
	defer objs.Close()

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

	// baseline
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

	if *flgMmsg {
		attach("syscalls", "sys_enter_sendmmsg", objs.TraceSendmmsgEnter)
		attach("syscalls", "sys_exit_sendmmsg", objs.TraceSendmmsgExit)
		attach("syscalls", "sys_enter_recvmmsg", objs.TraceRecvmmsgEnter)
		attach("syscalls", "sys_exit_recvmmsg", objs.TraceRecvmmsgExit)
	}
	if *flgRW {
		attach("syscalls", "sys_enter_write", objs.TraceWriteEnter)
		attach("syscalls", "sys_exit_write", objs.TraceWriteExit)
		attach("syscalls", "sys_enter_read", objs.TraceReadEnter)
		attach("syscalls", "sys_exit_read", objs.TraceReadExit)
	}

	// skb hint (можешь вернуть обработку позже)
	attach("net", "net_dev_queue", objs.TraceNetDevQueue)

	// perf readers
	traceRd, perCPU, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("perf.NewReader trace: %v", err)
	}
	defer traceRd.Close()

	var tlsRd *perf.Reader
	if *flgSNI {
		tlsRd, _, err = openPerfReaderTotalBudget(objs.TlsEvents, *flgPerfMB)
		if err != nil {
			log.Fatalf("perf.NewReader tls: %v", err)
		}
		defer tlsRd.Close()
	}

	log.Printf("perf trace per-cpu=%dKB total~=%dMB cpus=%d", perCPU/1024, (perCPU*runtime.NumCPU())/(1024*1024), runtime.NumCPU())
	log.Printf("started. Ctrl+C to exit")

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
			rec, e := traceRd.Read()
			if e != nil {
				if errors.Is(e, perf.ErrClosed) {
					return
				}
				continue
			}
			if rec.LostSamples != 0 {
				atomic.AddUint64(&lostTrace, rec.LostSamples)
				atomic.AddUint64(&lostGen, 1)
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}
			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			evCh <- evWrap{ev: ev, now: time.Now()}
		}
	}()

	type tlsWrap struct {
		ev  bpfTlsChunkT
		now time.Time
	}
	tlsCh := make(chan tlsWrap, 8192)

	if *flgSNI {
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
					atomic.AddUint64(&lostTLS, rec.LostSamples)
					atomic.AddUint64(&lostGen, 1)
				}
				if len(rec.RawSample) < int(unsafe.Sizeof(bpfTlsChunkT{})) {
					continue
				}
				ev := *(*bpfTlsChunkT)(unsafe.Pointer(&rec.RawSample[0]))
				tlsCh <- tlsWrap{ev: ev, now: time.Now()}
			}
		}()
	}

	flows := make(map[FlowKey]*Flow, 8192)

	// TLS + SNI state
	tlsByCookie := make(map[uint64]*tlsReasm, 8192)
	sniByCookie := make(map[uint64]sniEntry, 8192)

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

	attachSNIToFlow := func(f *Flow, now time.Time) {
		if f.TLSSNI != "" {
			return
		}
		if se, ok := sniByCookie[f.Key.Cookie]; ok && now.Sub(se.Seen) < *flgSniTTL {
			f.TLSSNI = se.Name
		}
	}

	closeByCookie := func(tgid uint32, cookie uint64, reason string) {
		for k, f := range flows {
			if k.TGID == tgid && k.Cookie == cookie {
				attachSNIToFlow(f, time.Now())
				if !f.OpenedPrinted && flowReadyToOpen(f) {
					printOpen(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printClose(f, reason)
				}
				delete(flows, k)
			}
		}
	}

	ticker := time.NewTicker(*flgSweep)
	defer ticker.Stop()

	for {
		select {
		case <-stop:
			_ = traceRd.Close()
			if tlsRd != nil {
				_ = tlsRd.Close()
			}
			for _, f := range flows {
				attachSNIToFlow(f, time.Now())
				if !f.OpenedPrinted && flowReadyToOpen(f) {
					printOpen(f)
					f.OpenedPrinted = true
				}
				if f.OpenedPrinted {
					printClose(f, "signal")
				}
			}
			log.Printf("lost trace=%d tls=%d gen=%d", atomic.LoadUint64(&lostTrace), atomic.LoadUint64(&lostTLS), atomic.LoadUint64(&lostGen))
			return

		case tw, ok := <-tlsCh:
			if !ok {
				tlsCh = nil
				continue
			}
			ev := tw.ev
			if ev.Cookie == 0 || uint8(ev.Event) != EV_TLS_CHUNK {
				continue
			}

			ln := int(ev.Len)
			if ln <= 0 {
				continue
			}
			if ln > len(ev.Data) {
				ln = len(ev.Data)
			}
			chunk := ev.Data[:ln]

			tr := tlsByCookie[ev.Cookie]
			if tr == nil {
				tr = &tlsReasm{buf: make([]byte, 0, 2048)}
				tlsByCookie[ev.Cookie] = tr
			}

			if sni, ok := tr.Feed(tw.now, chunk); ok {
				sniByCookie[ev.Cookie] = sniEntry{Name: sni, Seen: tw.now}
				delete(tlsByCookie, ev.Cookie)

				log.Printf("SNI cookie=%d %s", ev.Cookie, sni)

				// update existing flows
				for _, f := range flows {
					if f.Key.Cookie == ev.Cookie && f.TLSSNI == "" {
						f.TLSSNI = sni
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

			if evt == EV_SKB_OUT {
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

				// attach SNI if already known
				attachSNIToFlow(f, w.now)
			}

			f.LastSeen = w.now
			applyEndpoints(f, ev)

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

			if !f.OpenedPrinted && flowReadyToOpen(f) {
				printOpen(f)
				f.OpenedPrinted = true
			}

		case <-ticker.C:
			now := time.Now()

			// sweep flows by TTL
			for k, f := range flows {
				if now.Sub(f.LastSeen) > *flgTTL {
					attachSNIToFlow(f, now)
					if !f.OpenedPrinted && flowReadyToOpen(f) {
						printOpen(f)
						f.OpenedPrinted = true
					}
					if f.OpenedPrinted {
						printClose(f, "idle")
					}
					delete(flows, k)
				}
			}

			// sweep partial TLS buffers (handshake is short)
			for c, tr := range tlsByCookie {
				if now.Sub(tr.seen) > *flgTlsKeep {
					delete(tlsByCookie, c)
				}
			}

			// sweep sni cache
			for c, se := range sniByCookie {
				if now.Sub(se.Seen) > *flgSniTTL {
					delete(sniByCookie, c)
				}
			}

			// periodic loss note (optional)
			lt := atomic.LoadUint64(&lostTrace)
			ll := atomic.LoadUint64(&lostTLS)
			if lt != 0 || ll != 0 {
				// можно закомментировать если шумит
				// log.Printf("lost trace=%d tls=%d gen=%d", lt, ll, atomic.LoadUint64(&lostGen))
			}
		}
	}
}




2026/02/25 07:26:44.714714 SNI cookie=159420 mobile.events.data.microsoft.com
