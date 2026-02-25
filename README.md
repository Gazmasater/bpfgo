
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





//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
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

// syscalls events
#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_BINDOK    20
#define EV_SENDMSG   11
#define EV_RECVMSG   12
#define EV_SENDMMSG  13
#define EV_RECVMMSG  14
#define EV_READ      21
#define EV_WRITE     22
#define EV_CLOSE     30

// skb out hint
#define EV_SKB_OUT   40

// socket flags
#define MSG_PEEK 0x2

// file modes
#define S_IFMT   0170000
#define S_IFSOCK 0140000

// cmsg constants (Linux)
#define SOL_IP        0
#define SOL_IPV6      41
#define IP_PKTINFO    8
#define IPV6_PKTINFO  50

// bounded parsing limits
#define MAX_MMSG 16
#define MAX_IOV  4

#define CMSG_ALIGN(len) (((len) + sizeof(__u64) - 1) & ~(sizeof(__u64) - 1))

// payload capture
#define MAX_PAYLOAD     160
#define MAX_SNI_CHUNKS  6   // first N chunks per cookie (then stop)

/* ===== tracepoint net_dev_queue ctx (stable subset) ===== */
struct tp_net_dev_queue {
    __u16 common_type;
    __u8  common_flags;
    __u8  common_preempt_count;
    __s32 common_pid;

    __u64 skbaddr;
    __u32 len;
    char  name[IFNAMSIZ];
};

/* ====== minimal sockaddr for user read ===== */
struct in_addr_u { __u32 s_addr; };

struct sockaddr_in_u {
    __u16            sin_family;
    __u16            sin_port;
    struct in_addr_u sin_addr;
    __u8             pad[8];
};

struct sockaddr_in6_u {
    __u16            sin6_family;
    __u16            sin6_port;
    __u32            sin6_flowinfo;
    struct in6_addr  sin6_addr;
    __u32            sin6_scope_id;
};

/* ====== types ====== */

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

struct inflight_fd_t {
    __s32 fd;
};

/* EVENT (to userspace) */
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
    __u8  state;   // mmsg packets count (clamped), connect: 0/1
    __u8  _pad1;

    __u32 src_ip4; // net order
    __u32 dst_ip4; // net order
    __u8  src_ip6[16];
    __u8  dst_ip6[16];

    __u32 src_scope;
    __u32 dst_scope;

    char  comm[32];

    __u16 payload_len;
    __u8  payload[MAX_PAYLOAD];
};

const struct trace_info *unused __attribute__((unused));

/* ---- userspace ABI (amd64) ---- */

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

struct user_cmsghdr64 {
    __u64 cmsg_len;
    __s32 cmsg_level;
    __s32 cmsg_type;
};

struct user_in6_pktinfo {
    __u8  ipi6_addr[16];
    __u32 ipi6_ifindex;
};

struct user_in_pktinfo {
    __s32 ipi_ifindex;
    __u32 ipi_spec_dst;
    __u32 ipi_addr;
};

/* ---- mmsg (amd64) ---- */

struct user_iovec64 {
    __u64 iov_base;
    __u64 iov_len;
};

struct user_mmsghdr64 {
    struct user_msghdr64 msg_hdr;
    __u32 msg_len;
    __u32 _pad;
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

struct mmsg_ptrvlen_t {
    __u64 vec;
    __u32 vlen;
    __u32 flags;
};

struct payload_ptrlen_t {
    __u64 ptr;
    __u32 len;
    __u32 _pad;
};

/* ====== maps ====== */

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, struct fd_state_t);
} fd_state_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct inflight_fd_t);
} connect_fd_map SEC(".maps");

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
    __type(value, __u64);
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
    __type(value, struct mmsg_ptrvlen_t);
} mmsgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct mmsg_ptrvlen_t);
} mmsgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64); // pid_tgid
    __type(value, struct payload_ptrlen_t);
} write_args_map SEC(".maps");

// cookie(inode) -> state: 0..MAX_SNI_CHUNKS, 0xff stop
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64); // cookie
    __type(value, __u8);
} sni_state_map SEC(".maps");

/* ---- per-cpu scratch to avoid stack >512B ---- */
struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, struct trace_info);
} scratch_event SEC(".maps");

static __always_inline struct trace_info *evt_new(void)
{
    __u32 k = 0;
    struct trace_info *e = bpf_map_lookup_elem(&scratch_event, &k);
    if (!e) return 0;
    __builtin_memset(e, 0, sizeof(*e));
    return e;
}

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ====== small helpers ====== */

static __always_inline __u32 min_u32(__u32 a, __u32 b) { return a < b ? a : b; }

static __always_inline int read_sys_exit_ret(struct trace_event_raw_sys_exit *ctx, __s64 *ret)
{
    if (BPF_CORE_READ_INTO(ret, ctx, ret) < 0)
        return -1;
    return 0;
}

static __always_inline int read_msghdr64(__u64 msg_u, struct user_msghdr64 *h)
{
    if (!msg_u) return -1;
    if (bpf_probe_read_user(h, sizeof(*h), (void *)msg_u) < 0)
        return -1;
    return 0;
}

static __always_inline int read_mmsghdr0(__u64 vec_u, struct user_mmsghdr64 *out)
{
    if (!vec_u) return -1;
    if (bpf_probe_read_user(out, sizeof(*out), (void *)vec_u) < 0)
        return -1;
    return 0;
}

/* recv: sum msg_len */
static __always_inline __s64 sum_mmsg_len(__u64 vec_u, __u32 n)
{
    __s64 total = 0;

#pragma clang loop unroll(full)
    for (int i = 0; i < MAX_MMSG; i++) {
        if ((__u32)i >= n) continue;
        __u32 msg_len = 0;
        __u64 p = vec_u + (__u64)i * (__u64)sizeof(struct user_mmsghdr64)
                + (__u64)offsetof(struct user_mmsghdr64, msg_len);
        if (bpf_probe_read_user(&msg_len, sizeof(msg_len), (void *)p) == 0)
            total += (__s64)msg_len;
    }
    return total;
}

/* send: best-effort sum first MAX_IOV iov_len (clamped) */
static __always_inline __s64 sum_mmsg_iov_bytes(__u64 vec_u, __u32 n)
{
    __s64 total = 0;

#pragma clang loop unroll(full)
    for (int i = 0; i < MAX_MMSG; i++) {
        if ((__u32)i >= n) continue;

        struct user_mmsghdr64 mh = {};
        __u64 p = vec_u + (__u64)i * (__u64)sizeof(struct user_mmsghdr64);
        if (bpf_probe_read_user(&mh, sizeof(mh), (void *)p) != 0)
            continue;

        if (!mh.msg_hdr.msg_iov || mh.msg_hdr.msg_iovlen == 0)
            continue;

        __u32 iovcnt = (mh.msg_hdr.msg_iovlen > 0xffffffffULL) ? 0xffffffffU : (__u32)mh.msg_hdr.msg_iovlen;
        iovcnt = min_u32(iovcnt, MAX_IOV);

#pragma clang loop unroll(full)
        for (int j = 0; j < MAX_IOV; j++) {
            if ((__u32)j >= iovcnt) continue;
            struct user_iovec64 iv = {};
            __u64 ip = mh.msg_hdr.msg_iov + (__u64)j * (__u64)sizeof(struct user_iovec64);
            if (bpf_probe_read_user(&iv, sizeof(iv), (void *)ip) == 0)
                total += (__s64)iv.iov_len;
        }
    }
    return total;
}

/* ---- fd->file helpers ---- */

static __always_inline struct file *file_from_fd(int fd)
{
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
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file)
        return 0;

    return file;
}

static __always_inline int is_socket_fd(int fd)
{
    struct file *file = file_from_fd(fd);
    if (!file) return 0;

    struct inode *inode = BPF_CORE_READ(file, f_inode);
    if (!inode) return 0;

    __u16 mode = BPF_CORE_READ(inode, i_mode);
    return (mode & S_IFMT) == S_IFSOCK;
}

/* cookie = socket inode number */
static __always_inline __u64 cookie_from_fd(int fd)
{
    struct file *file = file_from_fd(fd);
    if (!file) return 0;

    struct inode *inode = BPF_CORE_READ(file, f_inode);
    if (!inode) return 0;

    return (__u64)BPF_CORE_READ(inode, i_ino);
}

static __always_inline struct sock *sock_from_fd(int fd)
{
    struct file *file = file_from_fd(fd);
    if (!file) return 0;

    void *pd = BPF_CORE_READ(file, private_data);
    struct socket *sock = (struct socket *)pd;
    if (!sock) return 0;

    return BPF_CORE_READ(sock, sk);
}

/* ---- sockaddr from user ---- */

static __always_inline int fill_from_sockaddr_user(struct trace_info *info,
                                                   const void *uaddr,
                                                   __u32 addrlen,
                                                   int fill_dst)
{
    __u16 family = 0;
    if (!uaddr || addrlen < sizeof(__u16))
        return -1;

    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0)
        return -1;

    if (info->family != 0 && family != info->family) {
        if (info->family == AF_INET6 && family == AF_INET) {
            if (addrlen < sizeof(struct sockaddr_in_u)) return -1;

            struct sockaddr_in_u sa = {};
            if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0) return -1;

            __u16 port = bpf_ntohs(sa.sin_port);

            __u8 v6[16] = {};
            v6[10] = 0xff; v6[11] = 0xff;
            __builtin_memcpy(&v6[12], &sa.sin_addr.s_addr, 4);

            if (fill_dst) {
                __builtin_memcpy(info->dst_ip6, v6, 16);
                if (port) info->dport = port;
            } else {
                __builtin_memcpy(info->src_ip6, v6, 16);
                if (port) info->sport = port;
            }
            return 0;
        }
        return -1;
    }

    if (info->family == 0) info->family = family;

    if (family == AF_INET) {
        if (addrlen < sizeof(struct sockaddr_in_u)) return -1;

        struct sockaddr_in_u sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0) return -1;

        __u16 port = bpf_ntohs(sa.sin_port);

        if (fill_dst) {
            info->dst_ip4 = sa.sin_addr.s_addr;
            if (port) info->dport = port;
        } else {
            info->src_ip4 = sa.sin_addr.s_addr;
            if (port) info->sport = port;
        }
        return 0;
    }

    if (family == AF_INET6) {
        if (addrlen < sizeof(struct sockaddr_in6_u)) return -1;

        struct sockaddr_in6_u sa6 = {};
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

/* ---- local addr helpers ---- */

static __always_inline __u32 ipv4_local_addr(struct sock *sk)
{
    __u32 a = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
    if (a == 0) {
        struct inet_sock *inet = (struct inet_sock *)sk;
        a = BPF_CORE_READ(inet, inet_saddr);
    }
    return a;
}

static __always_inline int is_ipv4_loopback(__u32 addr_be)
{
    if (addr_be == 0) return 0;
    __u32 h = bpf_ntohl(addr_be);
    return ((h >> 24) == 127);
}

static __always_inline int is_ipv6_loopback(const __u8 a[16])
{
#pragma clang loop unroll(full)
    for (int i = 0; i < 15; i++) if (a[i] != 0) return 0;
    return a[15] == 1;
}

static __always_inline void loopback_fallback(struct trace_info *info, int is_send_dir)
{
    if (info->family == AF_INET) {
        if (is_send_dir) {
            if (info->src_ip4 == 0 && is_ipv4_loopback(info->dst_ip4))
                info->src_ip4 = bpf_htonl(0x7f000001);
        } else {
            if (info->dst_ip4 == 0 && is_ipv4_loopback(info->src_ip4))
                info->dst_ip4 = bpf_htonl(0x7f000001);
        }
    } else if (info->family == AF_INET6) {
        if (is_send_dir) {
            if (is_ipv6_loopback(info->dst_ip6)) {
                int all0 = 1;
#pragma clang loop unroll(full)
                for (int i = 0; i < 16; i++) if (info->src_ip6[i] != 0) all0 = 0;
                if (all0) {
#pragma clang loop unroll(full)
                    for (int i = 0; i < 15; i++) info->src_ip6[i] = 0;
                    info->src_ip6[15] = 1;
                }
            }
        } else {
            int all0 = 1;
#pragma clang loop unroll(full)
            for (int i = 0; i < 16; i++) if (info->dst_ip6[i] != 0) all0 = 0;
            if (all0 && is_ipv6_loopback(info->src_ip6)) {
#pragma clang loop unroll(full)
                for (int i = 0; i < 15; i++) info->dst_ip6[i] = 0;
                info->dst_ip6[15] = 1;
            }
        }
    }
}

/* ---- fd state ---- */

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st)
{
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
        if (BPF_CORE_READ_INTO(&st->lip6, sk, __sk_common.skc_v6_rcv_saddr) < 0)
            return -1;
        if (BPF_CORE_READ_INTO(&st->rip6, sk, __sk_common.skc_v6_daddr) < 0)
            return -1;
        return 0;
    }

    return -1;
}

static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send_dir)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };

    struct fd_state_t tmp = {};
    struct fd_state_t *st = bpf_map_lookup_elem(&fd_state_map, &k);

    if (!st) {
        if (fill_fd_state(fd, &tmp) < 0) return -1;
        bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
        st = &tmp;
    } else {
        if (st->proto == IPPROTO_TCP && (st->lport == 0 || st->rport == 0)) {
            if (fill_fd_state(fd, &tmp) == 0) {
                bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
                st = &tmp;
            }
        }
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
        loopback_fallback(info, is_send_dir);
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
        loopback_fallback(info, is_send_dir);
        return 0;
    }

    return -1;
}

static __always_inline void fill_ids_comm_cookie(struct trace_info *info, __u64 pid_tgid, int fd, const char *comm64_opt)
{
    info->ts_ns  = bpf_ktime_get_ns();
    info->tgid   = pid_tgid >> 32;
    info->tid    = (__u32)pid_tgid;
    info->cookie = cookie_from_fd(fd);

    if (comm64_opt) {
        __builtin_memcpy(info->comm, comm64_opt, sizeof(info->comm));
    } else {
        bpf_get_current_comm(info->comm, sizeof(info->comm));
    }
}

/* ---- parse pktinfo cmsg (bounded steps) ---- */

static __always_inline void parse_pktinfo_cmsg(struct trace_info *info, __u64 ctrl, __u64 controllen, int set_dst)
{
    if (!ctrl || controllen < sizeof(struct user_cmsghdr64))
        return;

    __u64 p = ctrl;
    __u64 left = controllen;

#define CMSG_TRY_STEP() do { \
    if (left < sizeof(struct user_cmsghdr64)) return; \
    struct user_cmsghdr64 ch = {}; \
    if (bpf_probe_read_user(&ch, sizeof(ch), (void *)p) != 0) return; \
    if (ch.cmsg_len < sizeof(struct user_cmsghdr64) || ch.cmsg_len > left) return; \
    __u64 data = p + sizeof(struct user_cmsghdr64); \
    __u64 datalen = ch.cmsg_len - sizeof(struct user_cmsghdr64); \
    if (info->family == AF_INET6 && ch.cmsg_level == SOL_IPV6 && ch.cmsg_type == IPV6_PKTINFO && datalen >= sizeof(struct user_in6_pktinfo)) { \
        struct user_in6_pktinfo pi6 = {}; \
        if (bpf_probe_read_user(&pi6, sizeof(pi6), (void *)data) == 0) { \
            if (set_dst) { \
                __builtin_memcpy(info->dst_ip6, &pi6.ipi6_addr, 16); \
                if (pi6.ipi6_ifindex) info->dst_scope = pi6.ipi6_ifindex; \
            } else { \
                __builtin_memcpy(info->src_ip6, &pi6.ipi6_addr, 16); \
                if (pi6.ipi6_ifindex) info->src_scope = pi6.ipi6_ifindex; \
            } \
        } \
        return; \
    } \
    if (info->family == AF_INET && ch.cmsg_level == SOL_IP && ch.cmsg_type == IP_PKTINFO && datalen >= sizeof(struct user_in_pktinfo)) { \
        struct user_in_pktinfo pi4 = {}; \
        if (bpf_probe_read_user(&pi4, sizeof(pi4), (void *)data) == 0) { \
            __u32 local = pi4.ipi_spec_dst; \
            if (set_dst) info->dst_ip4 = local; else info->src_ip4 = local; \
        } \
        return; \
    } \
    __u64 step = CMSG_ALIGN(ch.cmsg_len); \
    if (step == 0 || step > left) return; \
    p += step; \
    left -= step; \
} while (0)

    CMSG_TRY_STEP(); CMSG_TRY_STEP(); CMSG_TRY_STEP();
    CMSG_TRY_STEP(); CMSG_TRY_STEP(); CMSG_TRY_STEP();

#undef CMSG_TRY_STEP
}

/* ===== payload helpers (verifier-friendly) ===== */

static __always_inline int looks_like_tls_clienthello(const void *p)
{
    __u8 h[3] = {};
    if (!p) return 0;
    if (bpf_probe_read_user(h, sizeof(h), p) != 0)
        return 0;
    if (h[0] != 0x16) return 0;
    if (h[1] != 0x03) return 0;
    return 1;
}

static __always_inline void maybe_attach_payload(struct trace_info *info, const void *p, __u32 len)
{
    if (!p) return;
    if (len == 0) return;
    if (info->proto != IPPROTO_TCP) return;

    __u64 cookie = info->cookie;
    if (!cookie) return;

    __u8 *stp = bpf_map_lookup_elem(&sni_state_map, &cookie);
    __u8 st = stp ? *stp : 0;

    if (st == 0xff || st >= MAX_SNI_CHUNKS)
        return;

    if (st == 0 && !looks_like_tls_clienthello(p)) {
        __u8 stop = 0xff;
        bpf_map_update_elem(&sni_state_map, &cookie, &stop, BPF_ANY);
        return;
    }

    // verifier-friendly bound:
    // 1) mask
    // 2) explicit clamp to constant MAX_PAYLOAD
    __u32 n = len;
    n &= 0xff; // 0..255
    if (n > (__u32)MAX_PAYLOAD)
        n = (__u32)MAX_PAYLOAD;
    if (n == 0)
        return;

    if (bpf_probe_read_user(info->payload, n, p) == 0) {
        info->payload_len = (__u16)n;
        __u8 nst = st + 1;
        bpf_map_update_elem(&sni_state_map, &cookie, &nst, BPF_ANY);
    }
}

/* ===================== CONNECT ===================== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct inflight_fd_t in = {};
    in.fd = (int)ctx->args[0];
    bpf_map_update_elem(&connect_fd_map, &id, &in, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[1];
    __u32 addrlen = (__u32)ctx->args[2];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = {.addr = uaddr, .len = addrlen};
        bpf_map_update_elem(&addrConnect_map, &id, &v, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0)
        goto cleanup;

    if (ret < 0 && ret != -EINPROGRESS && ret != -EALREADY)
        goto cleanup;

    struct inflight_fd_t *in = bpf_map_lookup_elem(&connect_fd_map, &id);
    if (!in)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state(in->fd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = in->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_CONNECT;
    info->state = (ret < 0) ? 1 : 0;
    info->fd    = (__u32)in->fd;
    info->ret   = ret;

    fill_ids_comm_cookie(info, id, (int)info->fd, conn ? conn->comm : 0);

    info->proto  = st.proto;
    info->family = st.family;
    info->sport  = st.lport;
    info->dport  = st.rport;

    if (st.family == AF_INET) {
        info->src_ip4 = st.lip;
        info->dst_ip4 = st.rip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info->src_ip6, &st.lip6, 16);
        __builtin_memcpy(info->dst_ip6, &st.rip6, 16);
    } else {
        goto cleanup;
    }

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(info, (void *)ap->addr, ap->len, 1);

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== ACCEPT ===================== */

static __always_inline int accept_enter_common(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);
    return 0;
}

static __always_inline int accept_exit_common(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 newfd = 0;
    if (read_sys_exit_ret(ctx, &newfd) < 0 || newfd < 0)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)newfd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)newfd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_ACCEPT;
    info->fd  = conn ? conn->fd : 0;
    info->ret = newfd;

    info->ts_ns  = bpf_ktime_get_ns();
    info->tgid   = tgid;
    info->tid    = (__u32)id;
    info->cookie = cookie_from_fd((int)newfd);

    if (conn)
        __builtin_memcpy(info->comm, conn->comm, sizeof(info->comm));
    else
        bpf_get_current_comm(info->comm, sizeof(info->comm));

    info->proto  = st.proto;
    info->family = st.family;

    info->sport = st.rport;
    info->dport = st.lport;

    if (st.family == AF_INET) {
        info->src_ip4 = st.rip;
        info->dst_ip4 = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info->src_ip6, &st.rip6, 16);
        __builtin_memcpy(info->dst_ip6, &st.lip6, 16);
    } else {
        goto cleanup;
    }

    loopback_fallback(info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

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

/* ===================== BIND ===================== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
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
        struct addr_ptrlen_t v = {.addr = uaddr, .len = addrlen};
        bpf_map_update_elem(&addrBind_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_BINDOK;
    info->fd    = ci->fd;
    info->ret   = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    info->proto  = st.proto;
    info->family = st.family;

    info->sport = st.lport;
    if (st.family == AF_INET) {
        info->src_ip4 = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info->src_ip6, &st.lip6, 16);
    }

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrBind_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(info, (void *)ap->addr, ap->len, 0);

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== SENDTO / RECVFROM ===================== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
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
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_SENDTO;
    info->fd = ci->fd;
    info->ret = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(info, (void *)ap->addr, ap->len, 1);

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
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
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && (m->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_RECVFROM;
    info->fd = ci->fd;
    info->ret = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    if (m && m->addr && m->lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void *)m->lenp) == 0) {
            if (addrlen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(info, (void *)m->addr, addrlen, 0);
        }
    }

    loopback_fallback(info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== SENDMSG / RECVMSG ===================== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    if (msg_u)
        bpf_map_update_elem(&msgSend_map, &id, &msg_u, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmsg")
int trace_sendmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_SENDMSG;
    info->fd = ci->fd;
    info->ret = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(*msgp, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(info, (void *)mh.msg_name, mh.msg_namelen, 1);

            if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
                parse_pktinfo_cmsg(info, mh.msg_control, mh.msg_controllen, 0 /* set SRC */);

            // payload from first iov — STRICT bounded for verifier
            if (mh.msg_iov && mh.msg_iovlen > 0) {
                struct user_iovec64 iv = {};
                if (bpf_probe_read_user(&iv, sizeof(iv), (void *)mh.msg_iov) == 0) {
                    __u32 sent_u = (ret > 0xffffffffLL) ? 0xffffffffU : (__u32)ret;

                    __u32 L = (__u32)iv.iov_len;
                    L &= 0xffff; // var &= const (verifier hint)

                    if (L > (__u32)MAX_PAYLOAD) L = (__u32)MAX_PAYLOAD;
                    if (L > sent_u) L = sent_u;
                    if (L > (__u32)MAX_PAYLOAD) L = (__u32)MAX_PAYLOAD;

                    if (iv.iov_base && L)
                        maybe_attach_payload(info, (void *)iv.iov_base, L);
                }
            }
        }
    }

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();

    struct conn_info_t ci = {};
    ci.tgid = id >> 32;
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
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct msg_ptrflags_t *pv = bpf_map_lookup_elem(&msgRecv_map, &id);
    if (pv && (pv->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_RECVMSG;
    info->fd = ci->fd;
    info->ret = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    if (pv && pv->msg) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(pv->msg, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(info, (void *)mh.msg_name, mh.msg_namelen, 0);

            if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
                parse_pktinfo_cmsg(info, mh.msg_control, mh.msg_controllen, 1 /* set DST(local) */);
        }
    }

    loopback_fallback(info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&msgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== SENDMMSG / RECVMMSG ===================== */

SEC("tracepoint/syscalls/sys_enter_sendmmsg")
int trace_sendmmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();

    struct conn_info_t ci = {};
    ci.tgid = id >> 32;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct mmsg_ptrvlen_t v = {};
    v.vec   = (__u64)ctx->args[1];
    v.vlen  = (__u32)ctx->args[2];
    v.flags = (__u32)ctx->args[3];

    if (v.vec && v.vlen)
        bpf_map_update_elem(&mmsgSend_map, &id, &v, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendmmsg")
int trace_sendmmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    struct mmsg_ptrvlen_t *pv = bpf_map_lookup_elem(&mmsgSend_map, &id);
    if (!ci || !pv || !pv->vec)
        goto cleanup;

    __u32 cnt = (__u32)ret;
    cnt = min_u32(cnt, pv->vlen);
    cnt = min_u32(cnt, MAX_MMSG);

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_SENDMMSG;
    info->fd    = ci->fd;
    info->state = (ret > 255) ? 255 : (__u8)ret;
    info->ret   = sum_mmsg_iov_bytes(pv->vec, cnt);

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    struct user_mmsghdr64 m0 = {};
    if (read_mmsghdr0(pv->vec, &m0) == 0) {
        if (m0.msg_hdr.msg_name && m0.msg_hdr.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(info, (void *)m0.msg_hdr.msg_name, m0.msg_hdr.msg_namelen, 1);

        if (m0.msg_hdr.msg_control && m0.msg_hdr.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(info, m0.msg_hdr.msg_control, m0.msg_hdr.msg_controllen, 0 /* set SRC */);
    }

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&mmsgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvmmsg")
int trace_recvmmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();

    struct conn_info_t ci = {};
    ci.tgid = id >> 32;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct mmsg_ptrvlen_t v = {};
    v.vec   = (__u64)ctx->args[1];
    v.vlen  = (__u32)ctx->args[2];
    v.flags = (__u32)ctx->args[3];

    if (v.vec && v.vlen)
        bpf_map_update_elem(&mmsgRecv_map, &id, &v, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmmsg")
int trace_recvmmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct mmsg_ptrvlen_t *pv = bpf_map_lookup_elem(&mmsgRecv_map, &id);
    if (pv && (pv->flags & MSG_PEEK))
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci || !pv || !pv->vec)
        goto cleanup;

    __u32 cnt = (__u32)ret;
    cnt = min_u32(cnt, pv->vlen);
    cnt = min_u32(cnt, MAX_MMSG);

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_RECVMMSG;
    info->fd    = ci->fd;
    info->state = (ret > 255) ? 255 : (__u8)ret;
    info->ret   = sum_mmsg_len(pv->vec, cnt);

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    struct user_mmsghdr64 m0 = {};
    if (read_mmsghdr0(pv->vec, &m0) == 0) {
        if (m0.msg_hdr.msg_name && m0.msg_hdr.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(info, (void *)m0.msg_hdr.msg_name, m0.msg_hdr.msg_namelen, 0);

        if (m0.msg_hdr.msg_control && m0.msg_hdr.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(info, m0.msg_hdr.msg_control, m0.msg_hdr.msg_controllen, 1 /* set DST(local) */);
    }

    loopback_fallback(info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&mmsgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== WRITE / READ ===================== */

SEC("tracepoint/syscalls/sys_enter_write")
int trace_write_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    if (!is_socket_fd(fd))
        return 0;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct payload_ptrlen_t a = {
        .ptr = (__u64)ctx->args[1],
        .len = (__u32)ctx->args[2],
    };
    if (a.ptr && a.len)
        bpf_map_update_elem(&write_args_map, &id, &a, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_write")
int trace_write_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_WRITE;
    info->fd    = ci->fd;
    info->ret   = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1) < 0)
        goto cleanup;

    // STRICT bounded length for verifier (NO min_u32)
    struct payload_ptrlen_t *a = bpf_map_lookup_elem(&write_args_map, &id);
    if (a && a->ptr && a->len) {
        __u32 sent_u = (ret > 0xffffffffLL) ? 0xffffffffU : (__u32)ret;

        __u32 L = a->len;
        L &= 0xffff; // var &= const

        if (L > (__u32)MAX_PAYLOAD) L = (__u32)MAX_PAYLOAD;
        if (L > sent_u) L = sent_u;
        if (L > (__u32)MAX_PAYLOAD) L = (__u32)MAX_PAYLOAD;

        if (L)
            maybe_attach_payload(info, (void *)a->ptr, L);
    }

    loopback_fallback(info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&write_args_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_read")
int trace_read_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    if (!is_socket_fd(fd))
        return 0;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_read")
int trace_read_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info *info = evt_new();
    if (!info) goto cleanup;

    info->event = EV_READ;
    info->fd    = ci->fd;
    info->ret   = ret;

    fill_ids_comm_cookie(info, id, (int)ci->fd, ci->comm);

    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 0) < 0)
        goto cleanup;

    loopback_fallback(info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== CLOSE ===================== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    // always drop cached fd state
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);

    // drop sni state for this cookie
    __u64 c = cookie_from_fd(fd);
    if (c)
        bpf_map_delete_elem(&sni_state_map, &c);

    if (!is_socket_fd(fd))
        return 0;

    struct trace_info *info = evt_new();
    if (!info)
        return 0;

    info->event = EV_CLOSE;
    info->fd    = (__u32)fd;
    info->ret   = 0;

    fill_ids_comm_cookie(info, id, fd, 0);

    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) == 0) {
        info->proto  = st.proto;
        info->family = st.family;
        info->sport  = st.lport;
        info->dport  = st.rport;

        if (st.family == AF_INET) {
            info->src_ip4 = st.lip;
            info->dst_ip4 = st.rip;
        } else if (st.family == AF_INET6) {
            __builtin_memcpy(info->src_ip6, &st.lip6, 16);
            __builtin_memcpy(info->dst_ip6, &st.rip6, 16);
        }

        loopback_fallback(info, 1);
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));
    }

    return 0;
}

/* ===================== net_dev_queue ===================== */

static __always_inline __u64 inode_cookie_from_sock(struct sock *sk)
{
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
int trace_net_dev_queue(struct tp_net_dev_queue *ctx)
{
    struct sk_buff *skb = (struct sk_buff *)(unsigned long)ctx->skbaddr;
    if (!skb) return 0;

    struct sock *sk = BPF_CORE_READ(skb, sk);
    if (!sk) return 0;

    __u64 cookie = inode_cookie_from_sock(sk);
    if (!cookie) return 0;

    __u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);

    struct trace_info *e = evt_new();
    if (!e) return 0;

    e->event  = EV_SKB_OUT;
    e->cookie = cookie;
    e->family = family;

    void *head = (void *)BPF_CORE_READ(skb, head);
    __u16 nh   = BPF_CORE_READ(skb, network_header);
    __u16 th   = BPF_CORE_READ(skb, transport_header);

    if (!head) return 0;

    if (family == AF_INET) {
        struct iphdr iph = {};
        bpf_probe_read_kernel(&iph, sizeof(iph), head + nh);
        if (iph.version != 4) return 0;

        e->proto   = iph.protocol;
        e->src_ip4 = iph.saddr;
        e->dst_ip4 = iph.daddr;

        if (e->proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e->sport = bpf_ntohs(uh.source);
            e->dport = bpf_ntohs(uh.dest);
        } else if (e->proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e->sport = bpf_ntohs(thh.source);
            e->dport = bpf_ntohs(thh.dest);
        }
    } else if (family == AF_INET6) {
        struct ipv6hdr ip6h = {};
        bpf_probe_read_kernel(&ip6h, sizeof(ip6h), head + nh);

        e->proto = ip6h.nexthdr;
        __builtin_memcpy(e->src_ip6, &ip6h.saddr, sizeof(e->src_ip6));
        __builtin_memcpy(e->dst_ip6, &ip6h.daddr, sizeof(e->dst_ip6));

        if (e->proto == IPPROTO_UDP) {
            struct udphdr uh = {};
            bpf_probe_read_kernel(&uh, sizeof(uh), head + th);
            e->sport = bpf_ntohs(uh.source);
            e->dport = bpf_ntohs(uh.dest);
        } else if (e->proto == IPPROTO_TCP) {
            struct tcphdr thh = {};
            bpf_probe_read_kernel(&thh, sizeof(thh), head + th);
            e->sport = bpf_ntohs(thh.source);
            e->dport = bpf_ntohs(thh.dest);
        }
    } else {
        return 0;
    }

    if (e->proto != IPPROTO_TCP && e->proto != IPPROTO_UDP &&
        e->proto != IPPROTO_ICMP && e->proto != IPPROTO_ICMPV6)
        return 0;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, e, sizeof(*e));
    return 0;
}
