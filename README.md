
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


dig -x 13.107.253.44 +short
dig -x 151.101.193.91 +short
dig -x 142.251.1.119 +short
sudo ./bpfgo 2>&1 | grep -F -C2 '*(any)'



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

// syscall events
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

// skb-derived hint
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

/* Minimal tracepoint ctx for net_dev_queue.
 * IMPORTANT: layout must match your kernel tracepoint. This one works for your logs already.
 */
struct tp_net_dev_queue {
    __u16 common_type;
    __u8  common_flags;
    __u8  common_preempt_count;
    __s32 common_pid;

    __u64 skbaddr;
    __u32 len;
    char  name[IFNAMSIZ];
};

/* ===== types ===== */

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

struct trace_info {
    __u64 ts_ns;
    __u64 cookie;

    __u32 tgid;
    __u32 tid;

    __u32 fd;
    __s32 _pad0;

    __s64 ret;     // bytes (mmsg: best-effort sum)

    __u16 family;
    __u16 sport;
    __u16 dport;

    __u8  proto;
    __u8  event;
    __u8  state;   // mmsg: packets count (clamped to 255); connect: 0/1 (EINPROGRESS)
    __u8  _pad1;

    __u32 src_ip4; // net order
    __u32 dst_ip4; // net order
    __u8  src_ip6[16];
    __u8  dst_ip6[16];

    __u32 src_scope; // ifindex for IPv6 link-local/mcast when known
    __u32 dst_scope; // ifindex for IPv6 link-local/mcast when known

    char  comm[32];
};

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
    __u64 addr;   // user sockaddr*
    __u32 len;    // sockaddr len
    __u32 _pad;
};

struct addr_recv_meta_t {
    __u64 addr;   // user sockaddr*
    __u64 lenp;   // user socklen_t*
    __u32 flags;
    __u32 _pad;
};

struct msg_ptrflags_t {
    __u64 msg;    // user msghdr*
    __u32 flags;
    __u32 _pad;
};

struct mmsg_ptrvlen_t {
    __u64 vec;    // user mmsghdr*
    __u32 vlen;
    __u32 flags;
};

/* ===== maps ===== */

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
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ===== helpers ===== */

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
    if (bpf_probe_read_user(h, sizeof(*h), (void *)msg_u) < 0) return -1;
    return 0;
}

static __always_inline int read_mmsghdr0(__u64 vec_u, struct user_mmsghdr64 *out)
{
    if (!vec_u) return -1;
    if (bpf_probe_read_user(out, sizeof(*out), (void *)vec_u) < 0) return -1;
    return 0;
}

static __always_inline __s64 sum_mmsg_len(__u64 vec_u, __u32 n)
{
    __s64 total = 0;

#pragma clang loop unroll(full)
    for (int i = 0; i < MAX_MMSG; i++) {
        if ((__u32)i >= n) continue;
        struct user_mmsghdr64 mh = {};
        __u64 p = vec_u + (__u64)i * (__u64)sizeof(struct user_mmsghdr64);
        if (bpf_probe_read_user(&mh, sizeof(mh), (void *)p) == 0)
            total += (__s64)mh.msg_len;
    }
    return total;
}

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
            if (addrlen < sizeof(struct sockaddr_in))
                return -1;

            struct sockaddr_in sa = {};
            if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
                return -1;

            __u16 port = bpf_ntohs(sa.sin_port);
            __u8 v6[16] = {};
            v6[10] = 0xff;
            v6[11] = 0xff;
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

    if (info->family == 0)
        info->family = family;

    if (family == AF_INET) {
        if (addrlen < sizeof(struct sockaddr_in))
            return -1;

        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            return -1;

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
        if (addrlen < sizeof(struct sockaddr_in6))
            return -1;

        struct sockaddr_in6 sa6 = {};
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0)
            return -1;

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

/* ---- loopback fallback ---- */

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
    for (int i = 0; i < 15; i++) {
        if (a[i] != 0) return 0;
    }
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

static __always_inline int load_fd_state_cached(__u32 tgid, int fd, struct fd_state_t *out)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };

    struct fd_state_t *p = bpf_map_lookup_elem(&fd_state_map, &k);
    if (p) {
        if (bpf_probe_read_kernel(out, sizeof(*out), p) != 0)
            return -1;

        // self-heal TCP ports sometimes 0 early
        if (out->proto == IPPROTO_TCP && (out->lport == 0 || out->rport == 0)) {
            struct fd_state_t tmp = {};
            if (fill_fd_state(fd, &tmp) == 0) {
                bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
                *out = tmp;
            }
        }
        return 0;
    }

    struct fd_state_t tmp = {};
    if (fill_fd_state(fd, &tmp) < 0)
        return -1;

    bpf_map_update_elem(&fd_state_map, &k, &tmp, BPF_ANY);
    *out = tmp;
    return 0;
}

static __always_inline int fill_from_fd_state(struct trace_info *info, __u32 tgid, int fd, int is_send_dir)
{
    struct fd_state_t st = {};
    if (load_fd_state_cached(tgid, fd, &st) < 0)
        return -1;

    info->proto  = st.proto;
    info->family = st.family;

    if (st.family == AF_INET) {
        if (is_send_dir) {
            info->src_ip4 = st.lip;
            info->dst_ip4 = st.rip;
            info->sport   = st.lport;
            info->dport   = st.rport;
        } else {
            info->src_ip4 = st.rip;
            info->dst_ip4 = st.lip;
            info->sport   = st.rport;
            info->dport   = st.lport;
        }
        loopback_fallback(info, is_send_dir);
        return 0;
    }

    if (st.family == AF_INET6) {
        if (is_send_dir) {
            __builtin_memcpy(info->src_ip6, &st.lip6, 16);
            __builtin_memcpy(info->dst_ip6, &st.rip6, 16);
            info->sport = st.lport;
            info->dport = st.rport;
        } else {
            __builtin_memcpy(info->src_ip6, &st.rip6, 16);
            __builtin_memcpy(info->dst_ip6, &st.lip6, 16);
            info->sport = st.rport;
            info->dport = st.lport;
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

/* ---- parse pktinfo cmsg (bounded, no loops) ---- */

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

    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();
    CMSG_TRY_STEP();

#undef CMSG_TRY_STEP
}

/* ===== connect ===== */

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
        struct addr_ptrlen_t v = { .addr = uaddr, .len = addrlen };
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

    struct inflight_fd_t *in_p = bpf_map_lookup_elem(&connect_fd_map, &id);
    if (!in_p) goto cleanup;
    struct inflight_fd_t in = {};
    if (bpf_probe_read_kernel(&in, sizeof(in), in_p) != 0) goto cleanup;

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    struct conn_info_t ci = {};
    if (ci_p) (void)bpf_probe_read_kernel(&ci, sizeof(ci), ci_p);

    struct fd_state_t st = {};
    if (fill_fd_state(in.fd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = in.fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct trace_info info = {};
    info.event = EV_CONNECT;
    info.state = (ret < 0) ? 1 : 0;
    info.fd    = (__u32)in.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, in.fd, ci.comm[0] ? ci.comm : 0);

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
    } else {
        goto cleanup;
    }

    struct addr_ptrlen_t *ap_p = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (ap_p) {
        struct addr_ptrlen_t ap = {};
        if (bpf_probe_read_kernel(&ap, sizeof(ap), ap_p) == 0) {
            if (ap.addr && ap.len >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)ap.addr, ap.len, 1);
        }
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== accept / accept4 ===== */

static __always_inline int accept_enter_common(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0]; // listen fd
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

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    struct conn_info_t ci = {};
    if (ci_p) (void)bpf_probe_read_kernel(&ci, sizeof(ci), ci_p);

    struct trace_info info = {};
    info.event = EV_ACCEPT;
    info.fd    = ci.fd;        // listen fd (may be 0 if missing)
    info.ret   = newfd;        // new fd in ret field

    info.ts_ns  = bpf_ktime_get_ns();
    info.tgid   = tgid;
    info.tid    = (__u32)id;
    info.cookie = cookie_from_fd((int)newfd);

    if (ci.comm[0]) __builtin_memcpy(info.comm, ci.comm, sizeof(info.comm));
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
    } else {
        goto cleanup;
    }

    loopback_fallback(&info, 0);
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
        struct addr_ptrlen_t v = { .addr = uaddr, .len = addrlen };
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

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct fd_state_t st = {};
    if (fill_fd_state((int)ci.fd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)ci.fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct trace_info info = {};
    info.event = EV_BINDOK;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    (void)fill_from_fd_state(&info, tgid, (int)ci.fd, 1);

    struct addr_ptrlen_t *ap_p = bpf_map_lookup_elem(&addrBind_map, &id);
    if (ap_p) {
        struct addr_ptrlen_t ap = {};
        if (bpf_probe_read_kernel(&ap, sizeof(ap), ap_p) == 0) {
            if (ap.addr && ap.len >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)ap.addr, ap.len, 0); // local -> SRC
        }
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== sendto ===== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[4];
    __u32 addrlen = (__u32)ctx->args[5];
    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = { .addr = uaddr, .len = addrlen };
        bpf_map_update_elem(&addrSend_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_SENDTO;
    info.fd    = ci.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 1) < 0)
        goto cleanup;

    struct addr_ptrlen_t *ap_p = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap_p) {
        struct addr_ptrlen_t ap = {};
        if (bpf_probe_read_kernel(&ap, sizeof(ap), ap_p) == 0) {
            if (ap.addr && ap.len >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)ap.addr, ap.len, 1); // DST
        }
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== recvfrom (filter MSG_PEEK) ===== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u32 flags  = (__u32)ctx->args[3];
    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];

    struct addr_recv_meta_t m = {};
    m.addr  = uaddr;
    m.lenp  = lenp_u;
    m.flags = flags;
    bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct addr_recv_meta_t *mp = bpf_map_lookup_elem(&addrRecv_map, &id);
    struct addr_recv_meta_t m = {};
    if (mp) (void)bpf_probe_read_kernel(&m, sizeof(m), mp);
    if (m.flags & MSG_PEEK)
        goto cleanup;

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_RECVFROM;
    info.fd    = ci.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 0) < 0)
        goto cleanup;

    if (m.addr && m.lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void *)m.lenp) == 0) {
            if (addrlen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)m.addr, addrlen, 0); // SRC (peer)
        }
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== sendmsg ===== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
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
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_SENDMSG;
    info.fd    = ci.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 1) < 0)
        goto cleanup;

    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    __u64 msg_u = 0;
    if (msgp) (void)bpf_probe_read_kernel(&msg_u, sizeof(msg_u), msgp);

    if (msg_u) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(msg_u, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)mh.msg_name, mh.msg_namelen, 1);

            if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
                parse_pktinfo_cmsg(&info, mh.msg_control, mh.msg_controllen, 0 /* set SRC */);
        }
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== recvmsg (filter MSG_PEEK) ===== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.fd   = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    __u32 flags = (__u32)ctx->args[2];

    if (msg_u) {
        struct msg_ptrflags_t v = { .msg = msg_u, .flags = flags };
        bpf_map_update_elem(&msgRecv_map, &id, &v, BPF_ANY);
    }
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvmsg")
int trace_recvmsg_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct msg_ptrflags_t *pv_p = bpf_map_lookup_elem(&msgRecv_map, &id);
    struct msg_ptrflags_t pv = {};
    if (pv_p) (void)bpf_probe_read_kernel(&pv, sizeof(pv), pv_p);
    if (pv.flags & MSG_PEEK)
        goto cleanup;

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_RECVMSG;
    info.fd    = ci.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 0) < 0)
        goto cleanup;

    if (pv.msg) {
        struct user_msghdr64 mh = {};
        if (read_msghdr64(pv.msg, &mh) == 0) {
            if (mh.msg_name && mh.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)mh.msg_name, mh.msg_namelen, 0); // SRC (peer)

            if (mh.msg_control && mh.msg_controllen >= sizeof(struct user_cmsghdr64))
                parse_pktinfo_cmsg(&info, mh.msg_control, mh.msg_controllen, 1 /* set DST(local) */);
        }
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== sendmmsg ===== */

SEC("tracepoint/syscalls/sys_enter_sendmmsg")
int trace_sendmmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
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

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    struct mmsg_ptrvlen_t *pv_p = bpf_map_lookup_elem(&mmsgSend_map, &id);
    if (!ci_p || !pv_p) goto cleanup;

    struct conn_info_t ci = {};
    struct mmsg_ptrvlen_t pv = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0) goto cleanup;
    if (bpf_probe_read_kernel(&pv, sizeof(pv), pv_p) != 0) goto cleanup;
    if (!pv.vec) goto cleanup;

    __u32 cnt = (__u32)ret;
    cnt = min_u32(cnt, pv.vlen);
    cnt = min_u32(cnt, MAX_MMSG);

    struct trace_info info = {};
    info.event = EV_SENDMMSG;
    info.fd    = ci.fd;
    info.state = (ret > 255) ? 255 : (__u8)ret;
    info.ret   = sum_mmsg_iov_bytes(pv.vec, cnt);

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 1) < 0)
        goto cleanup;

    struct user_mmsghdr64 m0 = {};
    if (read_mmsghdr0(pv.vec, &m0) == 0) {
        if (m0.msg_hdr.msg_name && m0.msg_hdr.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(&info, (void *)m0.msg_hdr.msg_name, m0.msg_hdr.msg_namelen, 1);

        if (m0.msg_hdr.msg_control && m0.msg_hdr.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(&info, m0.msg_hdr.msg_control, m0.msg_hdr.msg_controllen, 0 /* set SRC */);
    }

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&mmsgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== recvmmsg (filter MSG_PEEK) ===== */

SEC("tracepoint/syscalls/sys_enter_recvmmsg")
int trace_recvmmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
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

    struct mmsg_ptrvlen_t *pv_p = bpf_map_lookup_elem(&mmsgRecv_map, &id);
    if (!pv_p) goto cleanup;

    struct mmsg_ptrvlen_t pv = {};
    if (bpf_probe_read_kernel(&pv, sizeof(pv), pv_p) != 0) goto cleanup;
    if (pv.flags & MSG_PEEK) goto cleanup;

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0) goto cleanup;
    if (!pv.vec) goto cleanup;

    __u32 cnt = (__u32)ret;
    cnt = min_u32(cnt, pv.vlen);
    cnt = min_u32(cnt, MAX_MMSG);

    struct trace_info info = {};
    info.event = EV_RECVMMSG;
    info.fd    = ci.fd;
    info.state = (ret > 255) ? 255 : (__u8)ret;
    info.ret   = sum_mmsg_len(pv.vec, cnt);

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 0) < 0)
        goto cleanup;

    struct user_mmsghdr64 m0 = {};
    if (read_mmsghdr0(pv.vec, &m0) == 0) {
        if (m0.msg_hdr.msg_name && m0.msg_hdr.msg_namelen >= sizeof(__u16))
            (void)fill_from_sockaddr_user(&info, (void *)m0.msg_hdr.msg_name, m0.msg_hdr.msg_namelen, 0);

        if (m0.msg_hdr.msg_control && m0.msg_hdr.msg_controllen >= sizeof(struct user_cmsghdr64))
            parse_pktinfo_cmsg(&info, m0.msg_hdr.msg_control, m0.msg_hdr.msg_controllen, 1 /* set DST(local) */);
    }

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&mmsgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== write/read (ONLY SOCKETS) ===== */

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

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_WRITE;
    info.fd    = ci.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 1) < 0)
        goto cleanup;

    loopback_fallback(&info, 1);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
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

    struct conn_info_t *ci_p = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci_p) goto cleanup;

    struct conn_info_t ci = {};
    if (bpf_probe_read_kernel(&ci, sizeof(ci), ci_p) != 0)
        goto cleanup;

    struct trace_info info = {};
    info.event = EV_READ;
    info.fd    = ci.fd;
    info.ret   = ret;

    fill_ids_comm_cookie(&info, id, (int)ci.fd, ci.comm);

    if (fill_from_fd_state(&info, tgid, (int)ci.fd, 0) < 0)
        goto cleanup;

    loopback_fallback(&info, 0);
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== close (ONLY SOCKETS) ===== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    // always drop cached fd state
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);

    if (!is_socket_fd(fd))
        return 0;

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

        loopback_fallback(&info, 1);
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
    }

    return 0;
}

/* ===== skb out hint (net_dev_queue) ===== */

SEC("tracepoint/net/net_dev_queue")
int trace_net_dev_queue(struct tp_net_dev_queue *ctx)
{
    struct sk_buff *skb = (struct sk_buff *)(unsigned long)ctx->skbaddr;
    if (!skb) return 0;

    struct sock *sk = BPF_CORE_READ(skb, sk);
    if (!sk) return 0;

    // best-effort cookie read (may differ across kernels; works on your setup)
    __u64 cookie = 0;
    cookie = BPF_CORE_READ(sk, __sk_common.skc_cookie.counter);
    if (!cookie) return 0;

    __u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);

    struct trace_info e = {};
    e.event  = EV_SKB_OUT;
    e.cookie = cookie;
    e.family = family;

    void *head = (void *)BPF_CORE_READ(skb, head);
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








package main

import (
	"bytes"
	"context"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"math/bits"
	"net"
	"net/http"
	_ "net/http/pprof"
	"os"
	"os/signal"
	"path/filepath"
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

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -target amd64 -type trace_info bpf trace.c -- -I.

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
)

var (
	flgPerfMB = flag.Int("perfMB", 8, "perf ring total budget MB (best effort per-cpu)")
	flgTTL    = flag.Duration("ttl", 5*time.Second, "idle TTL -> CLOSE reason=idle")
	flgTick   = flag.Duration("tick", 1*time.Second, "sweep interval")
	flgPID    = flag.Int("pid", 0, "only pid (0=all)")
	flgComm   = flag.String("comm", "", "only comm substring")
	flgPprof  = flag.Bool("pprof", true, "enable pprof")
	flgPAddr  = flag.String("pprofAddr", ":6060", "pprof addr")

	// PREOPEN/FIX
	flgOpenDelay = flag.Duration("openDelay", 700*time.Millisecond, "delay before PREOPEN when local is still unknown")

	// PTR resolve
	flgResolve       = flag.Bool("resolve", true, "reverse DNS resolve (PTR)")
	flgHostState     = flag.Bool("hostState", true, "show miss/pending/no-ptr states")
	flgResolveTTL    = flag.Duration("resolveTTL", 30*time.Minute, "PTR positive TTL")
	flgResolveNegTTL = flag.Duration("resolveNegTTL", 5*time.Minute, "PTR negative TTL (NXDOMAIN)")
	flgResolveTO     = flag.Duration("resolveTimeout", 2*time.Second, "PTR timeout")
	flgResolveW      = flag.Int("resolveWorkers", 4, "PTR workers")
	flgResolveQ      = flag.Int("resolveQ", 4096, "PTR queue size")

	// skb-hint ttl
	flgL3TTL = flag.Duration("l3ttl", 10*time.Second, "TTL for skb-hint cache")
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

func isSend(ev uint8) bool {
	return ev == EV_SENDTO || ev == EV_SENDMSG || ev == EV_SENDMMSG || ev == EV_WRITE || ev == EV_CONNECT
}
func isRecv(ev uint8) bool {
	return ev == EV_RECVFROM || ev == EV_RECVMSG || ev == EV_RECVMMSG || ev == EV_READ || ev == EV_ACCEPT
}

func isAllZero16(b [16]byte) bool {
	for i := 0; i < 16; i++ {
		if b[i] != 0 {
			return false
		}
	}
	return true
}

func ip4KeyFromU32Net(x uint32) (key [16]byte) {
	// kernel gives network-order, but in Go we normalize into [a,b,c,d]
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

func isIPv6LinkLocalUnicast(ip [16]byte) bool { return ip[0] == 0xfe && (ip[1]&0xc0) == 0x80 }
func isIPv6LinkLocalMulticast(ip [16]byte) bool {
	return ip[0] == 0xff && (ip[1]&0x0f) == 0x02
}
func needsScope6(ip [16]byte) bool { return isIPv6LinkLocalUnicast(ip) || isIPv6LinkLocalMulticast(ip) }

func isIPv6Loopback(ip [16]byte) bool {
	for i := 0; i < 15; i++ {
		if ip[i] != 0 {
			return false
		}
	}
	return ip[15] == 1
}

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

func srcKey(ev bpfTraceInfo) (k [16]byte) {
	if uint16(ev.Family) == AF_INET {
		return ip4KeyFromU32Net(ev.SrcIp4)
	}
	copy(k[:], ev.SrcIp6[:])
	return
}
func dstKey(ev bpfTraceInfo) (k [16]byte) {
	if uint16(ev.Family) == AF_INET {
		return ip4KeyFromU32Net(ev.DstIp4)
	}
	copy(k[:], ev.DstIp6[:])
	return
}
func srcScope(ev bpfTraceInfo) uint32 {
	if uint16(ev.Family) == AF_INET6 {
		return uint32(ev.SrcScope)
	}
	return 0
}
func dstScope(ev bpfTraceInfo) uint32 {
	if uint16(ev.Family) == AF_INET6 {
		return uint32(ev.DstScope)
	}
	return 0
}

/* ===== PTR resolver ===== */

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

func (r *dnsResolver) Get(family uint16, ip [16]byte) (string, bool) {
	name, pending, neg, ok := r.Peek(family, ip)
	if !ok || pending || neg || name == "" {
		return "", false
	}
	return name, true
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

var dnsr *dnsResolver

/* ===== endpoint formatting ===== */

func specialAlias(family uint16, ip [16]byte) (string, bool) {
	if family == AF_INET {
		if ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 1 {
			return "localhost", true
		}
		if ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 53 {
			return "dnsstub", true
		}
		if ip[0] == 10 && ip[1] == 0 && ip[2] == 2 && ip[3] == 3 {
			return "vboxdns", true
		}
		if ip[0] == 10 && ip[1] == 0 && ip[2] == 2 && ip[3] == 2 {
			return "vboxgw", true
		}
	}
	if family == AF_INET6 && isIPv6Loopback(ip) {
		return "localhost", true
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
	name, pending, neg, ok := dnsr.Peek(family, ip)
	if ok {
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

func fmtEndpoint(family uint16, ip [16]byte, port uint16, scope uint32, proto uint8, alias string) string {
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

/* ===== L3 hint ===== */

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

func (l l3Info) expired(now time.Time, ttl time.Duration) bool {
	return now.Sub(l.Seen) > ttl
}

/* ===== Flow model ===== */

type FlowKey struct {
	TGID   uint32
	Cookie uint64
	Proto  uint8
	Family uint16

	// for UDP/ICMP: peer scoping to reduce collisions
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

	// PREOPEN/FIX/OPEN state
	PreOpenedPrinted bool
	OpenedPrinted    bool
	PreLocal         [16]byte
	PreLport         uint16
	PreLocalScope    uint32

	RemoteHost string
	GenStart    uint64
}

func makeKey(ev bpfTraceInfo) FlowKey {
	k := FlowKey{
		TGID:   ev.Tgid,
		Cookie: ev.Cookie,
		Proto:  uint8(ev.Proto),
		Family: uint16(ev.Family),
	}

	evt := uint8(ev.Event)

	// Scope peer for UDP/ICMP to avoid collisions on same cookie (rare but бывает на быстрых udp)
	if k.Proto == IPPROTO_UDP || k.Proto == IPPROTO_ICMP || k.Proto == IPPROTO_ICMPV6 {
		var remote [16]byte
		var rport uint16
		var rscope uint32

		if isRecv(evt) {
			remote = srcKey(ev)
			rport = uint16(ev.Sport)
			rscope = srcScope(ev)
		} else {
			remote = dstKey(ev)
			rport = uint16(ev.Dport)
			rscope = dstScope(ev)
		}

		if !isAllZero16(remote) {
			k.PeerMode = 1
			k.Remote = remote
			if k.Proto == IPPROTO_UDP {
				k.Rport = rport
			}
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
	case isSend(evt):
		localIP = srcKey(ev)
		remoteIP = dstKey(ev)
		lport = uint16(ev.Sport)
		rport = uint16(ev.Dport)
		localScope = srcScope(ev)
		remoteScope = dstScope(ev)

	case isRecv(evt):
		localIP = dstKey(ev)
		remoteIP = srcKey(ev)
		lport = uint16(ev.Dport)
		rport = uint16(ev.Sport)
		localScope = dstScope(ev)
		remoteScope = srcScope(ev)

	case evt == EV_BINDOK:
		localIP = srcKey(ev)
		lport = uint16(ev.Sport)
		localScope = srcScope(ev)
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

func applyL3Hint(f *Flow, h l3Info) {
	if h.Family != f.Key.Family || h.Proto != f.Key.Proto {
		return
	}

	// if remote known: match by remote+port
	if !isAllZero16(f.Remote) {
		// send direction: remote == dst
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
		// recv direction: remote == src
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

	// fallback: at least fill local from src
	if isAllZero16(f.Local) && !isAllZero16(h.Src) {
		f.Local = h.Src
		f.LocalScope = h.SrcSc
		if f.Lport == 0 && h.Sport != 0 {
			f.Lport = h.Sport
		}
	}
}

/* ===== Printing ===== */

func flowBaseReady(f *Flow) bool {
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

func remoteAliasCached(f *Flow) string {
	if f.RemoteHost != "" {
		return f.RemoteHost
	}
	a := aliasForIP(f.Key.Family, f.Remote)
	switch a {
	case "", "?", "any", "skip", "pending", "no-ptr", "miss":
		return a
	default:
		f.RemoteHost = a
		return a
	}
}

func printPreOpen(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local) // will be "any"
	rAlias := remoteAliasCached(f)
	fmt.Printf("PREOPEN %-5s pid=%d(%s) cookie=%d  %s -> %s  local=pending\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
	)
}

func printFix(f *Flow) {
	oldAlias := aliasForIP(f.Key.Family, f.PreLocal)
	newAlias := aliasForIP(f.Key.Family, f.Local)
	peerAlias := remoteAliasCached(f)

	fmt.Printf("FIX    %-5s pid=%d(%s) cookie=%d  local %s -> %s  peer=%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.PreLocal, f.PreLport, f.PreLocalScope, f.Key.Proto, oldAlias),
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, newAlias),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, peerAlias),
	)
}

func printOpen(f *Flow) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)

	fmt.Printf("OPEN   %-5s pid=%d(%s) cookie=%d  %s -> %s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
	)
}

func printClose(f *Flow, reason string) {
	lAlias := aliasForIP(f.Key.Family, f.Local)
	rAlias := remoteAliasCached(f)
	age := time.Since(f.FirstSeen).Truncate(time.Millisecond)

	fmt.Printf("CLOSE  %-5s pid=%d(%s) cookie=%d  %s -> %s  →out=%dB/%dp ←in=%dB/%dp  age=%s reason=%s\n",
		protoStr(f.Key.Proto),
		f.Key.TGID, f.Comm, f.Key.Cookie,
		fmtEndpoint(f.Key.Family, f.Local, f.Lport, f.LocalScope, f.Key.Proto, lAlias),
		fmtEndpoint(f.Key.Family, f.Remote, f.Rport, f.RemoteScope, f.Key.Proto, rAlias),
		f.OutBytes, f.OutPkts, f.InBytes, f.InPkts,
		age, reason,
	)
}

// state machine: PREOPEN (after openDelay) -> FIX (when local becomes known) -> OPEN
func maybePrintStart(f *Flow) {
	if f.OpenedPrinted {
		return
	}

	// if PREOPEN already printed: wait for local -> FIX and mark opened
	if f.PreOpenedPrinted {
		if !isAllZero16(f.Local) {
			printFix(f)
			f.OpenedPrinted = true
		}
		return
	}

	// if local known and base ready -> OPEN immediately
	if flowBaseReady(f) && !isAllZero16(f.Local) {
		printOpen(f)
		f.OpenedPrinted = true
		return
	}

	// else PREOPEN after openDelay
	if flowBaseReady(f) && isAllZero16(f.Local) && time.Since(f.FirstSeen) >= *flgOpenDelay {
		f.PreOpenedPrinted = true
		f.PreLocal = f.Local
		f.PreLport = f.Lport
		f.PreLocalScope = f.LocalScope
		printPreOpen(f)
	}
}

/* ===== perf reader ===== */

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
	p2 := 1 << (bits.Len(uint(pages)) - 1)
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

var lostTotal uint64
var lostGen uint64

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if *flgResolve {
		dnsr = newDNSResolver(*flgResolveQ, *flgResolveW, *flgResolveTTL, *flgResolveNegTTL, *flgResolveTO)
		defer dnsr.Close()
	}

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("load bpf objects: %v", err)
	}
	defer objs.Close()

	if *flgPprof {
		go func() {
			log.Printf("pprof on %s", *flgPAddr)
			_ = http.ListenAndServe(*flgPAddr, nil)
		}()
	}

	attach := func(cat, name string, prog *ebpf.Program) link.Link {
		l, err := link.Tracepoint(cat, name, prog, nil)
		if err != nil {
			log.Fatalf("attach %s/%s: %v", cat, name, err)
		}
		return l
	}

	var links []link.Link
	links = append(links,
		attach("syscalls", "sys_enter_bind", objs.TraceBindEnter),
		attach("syscalls", "sys_exit_bind", objs.TraceBindExit),

		attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter),
		attach("syscalls", "sys_exit_connect", objs.TraceConnectExit),

		attach("syscalls", "sys_enter_accept4", objs.TraceAccept4Enter),
		attach("syscalls", "sys_exit_accept4", objs.TraceAccept4Exit),
		attach("syscalls", "sys_enter_accept", objs.TraceAcceptEnter),
		attach("syscalls", "sys_exit_accept", objs.TraceAcceptExit),

		attach("syscalls", "sys_enter_close", objs.TraceCloseEnter),

		attach("syscalls", "sys_enter_sendto", objs.TraceSendtoEnter),
		attach("syscalls", "sys_exit_sendto", objs.TraceSendtoExit),
		attach("syscalls", "sys_enter_recvfrom", objs.TraceRecvfromEnter),
		attach("syscalls", "sys_exit_recvfrom", objs.TraceRecvfromExit),

		attach("syscalls", "sys_enter_sendmsg", objs.TraceSendmsgEnter),
		attach("syscalls", "sys_exit_sendmsg", objs.TraceSendmsgExit),
		attach("syscalls", "sys_enter_recvmsg", objs.TraceRecvmsgEnter),
		attach("syscalls", "sys_exit_recvmsg", objs.TraceRecvmsgExit),

		attach("syscalls", "sys_enter_sendmmsg", objs.TraceSendmmsgEnter),
		attach("syscalls", "sys_exit_sendmmsg", objs.TraceSendmmsgExit),
		attach("syscalls", "sys_enter_recvmmsg", objs.TraceRecvmmsgEnter),
		attach("syscalls", "sys_exit_recvmmsg", objs.TraceRecvmmsgExit),

		attach("syscalls", "sys_enter_write", objs.TraceWriteEnter),
		attach("syscalls", "sys_exit_write", objs.TraceWriteExit),
		attach("syscalls", "sys_enter_read", objs.TraceReadEnter),
		attach("syscalls", "sys_exit_read", objs.TraceReadExit),

		attach("net", "net_dev_queue", objs.TraceNetDevQueue),
	)
	defer func() {
		for _, l := range links {
			_ = l.Close()
		}
	}()

	rd, perCPUBytes, err := openPerfReaderTotalBudget(objs.TraceEvents, *flgPerfMB)
	if err != nil {
		log.Fatalf("perf reader: %v", err)
	}
	defer rd.Close()

	log.Printf("perf ring per-cpu=%dKB total~=%dMB cpus=%d", perCPUBytes/1024, (perCPUBytes*runtime.NumCPU())/(1024*1024), runtime.NumCPU())
	log.Printf("PREOPEN/FIX/OPEN enabled openDelay=%s ttl=%s", flgOpenDelay.String(), flgTTL.String())
	log.Println("Ctrl+C to exit")

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
				atomic.AddUint64(&lostTotal, rec.LostSamples)
				atomic.AddUint64(&lostGen, 1)
			}
			if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
				continue
			}
			ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
			evCh <- evWrap{ev: ev, now: time.Now()}
		}
	}()

	selfName := filepath.Base(os.Args[0])

	shouldKeep := func(pid uint32, comm string) bool {
		if comm == "" || comm == selfName {
			return false
		}
		if *flgPID != 0 && int(pid) != *flgPID {
			return false
		}
		if *flgComm != "" && !strings.Contains(comm, *flgComm) {
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
					applyL3Hint(f, h)
				}
				maybePrintStart(f) // may print PREOPEN or FIX or OPEN
				printClose(f, reason) // ALWAYS
				delete(flows, k)
			}
		}
	}

	ticker := time.NewTicker(*flgTick)
	defer ticker.Stop()

	lastSweep := time.Now()

	for {
		select {
		case <-stop:
			_ = rd.Close()
			// flush all
			for _, f := range flows {
				maybePrintStart(f)
				printClose(f, "signal")
			}
			log.Printf("PERF_LOST_TOTAL total=%d gen=%d", atomic.LoadUint64(&lostTotal), atomic.LoadUint64(&lostGen))
			return

		case <-ticker.C:
			now := time.Now()

			// sweep dns
			if dnsr != nil {
				dnsr.SweepExpired(500)
			}

			// sweep l3 hints
			for c, h := range l3ByCookie {
				if h.expired(now, *flgL3TTL) {
					delete(l3ByCookie, c)
				}
			}

			// TTL sweep flows
			for k, f := range flows {
				if now.Sub(f.LastSeen) > *flgTTL {
					if h, ok := l3ByCookie[f.Key.Cookie]; ok {
						applyL3Hint(f, h)
					}
					maybePrintStart(f)
					printClose(f, "idle")
					delete(flows, k)
				}
			}

			lastSweep = now
			_ = lastSweep

		case w, ok := <-evCh:
			if !ok {
				return
			}
			ev := w.ev
			evt := uint8(ev.Event)
			proto := uint8(ev.Proto)
			family := uint16(ev.Family)

			if !protoAllowed(proto) || (family != AF_INET && family != AF_INET6) {
				continue
			}

			// skb-out hint
			if evt == EV_SKB_OUT {
				if ev.Cookie == 0 {
					continue
				}
				h := l3Info{
					Family: family, Proto: proto,
					Src:   srcKey(ev), Sport: uint16(ev.Sport), SrcSc: srcScope(ev),
					Dst:   dstKey(ev), Dport: uint16(ev.Dport), DstSc: dstScope(ev),
					Seen:  w.now,
				}
				l3ByCookie[ev.Cookie] = h
				for _, f := range flows {
					if f.Key.Cookie == ev.Cookie {
						applyL3Hint(f, h)
						maybePrintStart(f) // might trigger FIX
					}
				}
				continue
			}

			comm := commString(ev.Comm)
			if !shouldKeep(ev.Tgid, comm) {
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

			// apply l3 hint if available
			if h, ok := l3ByCookie[f.Key.Cookie]; ok {
				applyL3Hint(f, h)
			}

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

			// print state machine
			maybePrintStart(f)
		}
	}
}




lev@lev-VirtualBox:~/bpfgo$ sudo ./bpfgo
2026/02/23 03:03:34.466112 load bpf objects: field TraceConnectExit: program trace_connect_exit: load program: permission denied: 349: (61) r2 = *(u32 *)(r9 +28): R9 invalid mem access 'scalar' (472 line(s) omitted)
lev@lev-VirtualBox:~/bpfgo$ 





