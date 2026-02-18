
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





//go:build ignore
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_helpers.h>

/* ===================== config / constants ===================== */

#define AF_INET  2
#define AF_INET6 10

#define EINPROGRESS 115
#define EALREADY    114

// events
#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_BINDOK    20
#define EV_SENDMSG   11
#define EV_RECVMSG   12

// tcp states (linux)
#define TCP_ESTABLISHED 1
#define TCP_SYN_SENT    2
#define TCP_CLOSE       7

// owner kinds
#define OWN_KIND_NONE    0
#define OWN_KIND_CONNECT 1
#define OWN_KIND_ACCEPT  2

// ethertype
#define ETH_P_IP    0x0800
#define ETH_P_IPV6  0x86DD

// ip proto
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

/* ===================== types ===================== */

struct conn_info_t {
    __u32 pid;     // tgid
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

    __u16 lport;   // local (host order)
    __u16 rport;   // remote (host order)

    __u32 lip;     // local IPv4 (net order)
    __u32 rip;     // remote IPv4 (net order)

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct inflight_fd_t {
    __s32 fd;
};

// owner by skaddr (inet_sock_set_state can run in softirq context)
struct sk_owner_t {
    __u32 pid;       // tgid
    __u32 fd;
    __s64 conn_ret;  // from sys_exit_connect (0 / -115 / ...)
    __u8  kind;      // OWN_KIND_*
    __u8  emitted;   // connect already emitted?
    __u8  _pad[6];
    char  comm[32];
};

struct trace_info {
    // IPv4
    struct in_addr  srcIP;
    struct in_addr  dstIP;

    // IPv6
    struct in6_addr srcIP6;
    struct in6_addr dstIP6;

    __u32 pid;
    __u32 proto;

    __u32 fd;
    __s32 _pad0;   // align ret
    __s64 ret;     // bytes / sys_exit return / etc.

    __u16 sport;
    __u16 dport;
    __u16 family;

    __u8  sysexit; // event code
    __u8  state;   // for TCP: newstate

    char  comm[32];
};

// keep type in BTF for bpf2go -type trace_info
const struct trace_info *unused __attribute__((unused));

/* user msghdr head (enough for msg_name + msg_namelen) */
struct user_msghdr_head {
    void *msg_name;     // 8 bytes on amd64
    __u32 msg_namelen;  // 4 bytes
    __u32 _pad;         // align
};

struct addr_ptrlen_t {
    __u64 addr;   // user sockaddr*
    __u32 len;    // sockaddr len
    __u32 _pad;
};

struct addr_recv_meta_t {
    __u64 addr;   // user sockaddr* (kernel fills)
    __u64 lenp;   // user socklen_t* (kernel fills)
};

// “real chosen src” cache by sk pointer (written at net:net_dev_queue)
struct sk_src_val_t {
    __u16 family;
    __u8  proto;
    __u8  _pad0;

    __u16 sport;     // host order
    __u16 _pad1;

    __u8  src[16];   // v4 in src[0..3]
    __u64 ts_ns;
};

/* ===================== maps ===================== */

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, struct fd_state_t);
} fd_state_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);   // pid_tgid
    __type(value, struct inflight_fd_t);
} connect_fd_map SEC(".maps");

/* connect: (sockaddr*, len) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrConnect_map SEC(".maps");

/* bind: (sockaddr*, len) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrBind_map SEC(".maps");

/* sendto: (sockaddr*, len) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrSend_map SEC(".maps");

/* recvfrom: (sockaddr*, socklen_t*) */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_recv_meta_t);
} addrRecv_map SEC(".maps");

/* stack-safe: separate maps for msghdr* */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);    // pid_tgid
    __type(value, __u64);  // user msghdr*
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);    // pid_tgid
    __type(value, __u64);  // user msghdr*
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64); // pid_tgid
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

// skaddr -> owner (LRU to avoid leaks)
struct {
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 65536);
    __type(key, __u64);                 // skaddr
    __type(value, struct sk_owner_t);
} sk_owner_map SEC(".maps");

// skaddr -> last seen src ip at queue time (LRU)
struct {
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 65536);
    __type(key, __u64);                 // skaddr
    __type(value, struct sk_src_val_t);
} sk_src_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ===================== helpers ===================== */

static __always_inline int read_sys_exit_ret(struct trace_event_raw_sys_exit *ctx, __s64 *ret)
{
    if (BPF_CORE_READ_INTO(ret, ctx, ret) < 0)
        return -1;
    return 0;
}

static __always_inline int read_msghdr_head(__u64 msg_u, struct user_msghdr_head *h)
{
    if (!msg_u)
        return -1;
    if (bpf_probe_read_user(h, sizeof(*h), (void *)msg_u) < 0)
        return -1;
    return 0;
}

// IMPORTANT: do NOT fail when port==0.
// Just "don't overwrite" ports if they are already known from fd_state.
static __always_inline int fill_from_sockaddr_user(struct trace_info *info,
                                                   const void *uaddr,
                                                   __u32 addrlen,
                                                   int is_dst)
{
    __u16 family = 0;

    if (!uaddr || addrlen < sizeof(__u16))
        return -1;

    if (bpf_probe_read_user(&family, sizeof(family), uaddr) < 0)
        return -1;

    if (family == AF_INET) {
        if (addrlen < sizeof(struct sockaddr_in))
            return -1;

        struct sockaddr_in sa = {};
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa.sin_port);

        info->family = AF_INET;
        if (is_dst) {
            info->dstIP.s_addr = sa.sin_addr.s_addr; // net order
            if (port) info->dport = port;
        } else {
            info->srcIP.s_addr = sa.sin_addr.s_addr; // net order
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

        info->family = AF_INET6;
        if (is_dst) {
            __builtin_memcpy(&info->dstIP6, &sa6.sin6_addr, sizeof(info->dstIP6));
            if (port) info->dport = port;
        } else {
            __builtin_memcpy(&info->srcIP6, &sa6.sin6_addr, sizeof(info->srcIP6));
            if (port) info->sport = port;
        }
        return 0;
    }

    return -1;
}

/* ---------- fd -> sock (best effort) ---------- */

static __always_inline struct sock *sock_from_fd(int fd)
{
    if (fd < 0)
        return 0;

    struct task_struct *task = (struct task_struct *)bpf_get_current_task_btf();
    if (!task)
        return 0;

    struct files_struct *files = BPF_CORE_READ(task, files);
    if (!files)
        return 0;

    struct fdtable *fdt = BPF_CORE_READ(files, fdt);
    if (!fdt)
        return 0;

    int max_fds = BPF_CORE_READ(fdt, max_fds);
    if (fd >= max_fds)
        return 0;

    struct file **fd_array = BPF_CORE_READ(fdt, fd);
    if (!fd_array)
        return 0;

    struct file *file = 0;
    if (bpf_probe_read_kernel(&file, sizeof(file), &fd_array[fd]) < 0 || !file)
        return 0;

    struct socket *sock = 0;
    if (bpf_probe_read_kernel(&sock, sizeof(sock), &file->private_data) < 0 || !sock)
        return 0;

    return BPF_CORE_READ(sock, sk);
}

static __always_inline int fill_fd_state_from_sock(struct sock *sk, struct fd_state_t *st)
{
    if (!sk)
        return -1;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);

    // local port in host order
    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);

    // remote port in network order -> host
    __u16 dport_be = BPF_CORE_READ(sk, __sk_common.skc_dport);
    st->rport = bpf_ntohs(dport_be);

    if (st->family == AF_INET) {
        st->lip = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
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

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st)
{
    struct sock *sk = sock_from_fd(fd);
    return fill_fd_state_from_sock(sk, st);
}

/* ---------- fd_state_map helpers ---------- */

static __always_inline int apply_state_to_info(struct trace_info *info,
                                               const struct fd_state_t *st,
                                               int is_send)
{
    info->proto  = st->proto;
    info->family = st->family;

    if (st->family == AF_INET) {
        if (is_send) {
            info->srcIP.s_addr = st->lip;
            info->dstIP.s_addr = st->rip;
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            info->srcIP.s_addr = st->rip;
            info->dstIP.s_addr = st->lip;
            info->sport = st->rport;
            info->dport = st->lport;
        }
        return 0;
    }

    if (st->family == AF_INET6) {
        if (is_send) {
            __builtin_memcpy(&info->srcIP6, &st->lip6, sizeof(info->srcIP6));
            __builtin_memcpy(&info->dstIP6, &st->rip6, sizeof(info->dstIP6));
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            __builtin_memcpy(&info->srcIP6, &st->rip6, sizeof(info->srcIP6));
            __builtin_memcpy(&info->dstIP6, &st->lip6, sizeof(info->dstIP6));
            info->sport = st->rport;
            info->dport = st->lport;
        }
        return 0;
    }

    return -1;
}

// try map, else snapshot from fd and cache
static __always_inline int fill_from_fd_state_any(struct trace_info *info, __u32 tgid, int fd, int is_send)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t *stp = bpf_map_lookup_elem(&fd_state_map, &k);
    if (stp)
        return apply_state_to_info(info, stp, is_send);

    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) < 0)
        return -1;

    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
    return apply_state_to_info(info, &st, is_send);
}

/* ---------- sk_owner_map ---------- */

static __always_inline void remember_owner_for_fd(__u32 tgid, int fd, __s64 conn_ret, __u8 kind)
{
    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return;

    __u64 skk = (__u64)(unsigned long)(const void *)sk;

    struct sk_owner_t own = {};
    own.pid = tgid;
    own.fd  = (__u32)fd;
    own.conn_ret = conn_ret;
    own.kind = kind;
    own.emitted = 0;

    bpf_get_current_comm(own.comm, sizeof(own.comm));
    bpf_map_update_elem(&sk_owner_map, &skk, &own, BPF_ANY);
}

/* ---------- real-src cache (net_dev_queue) ---------- */

static __always_inline int is_zero_v6(const struct in6_addr *a)
{
    const __u8 *p = (const __u8 *)a;
#pragma unroll
    for (int i = 0; i < 16; i++) {
        if (p[i])
            return 0;
    }
    return 1;
}

static __always_inline int maybe_fill_src_from_sk_cache(struct trace_info *info, int fd)
{
    if (info->family == 0)
        return -1;

    if (info->family == AF_INET && info->srcIP.s_addr != 0)
        return 0;
    if (info->family == AF_INET6 && !is_zero_v6(&info->srcIP6))
        return 0;

    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return -1;

    __u64 skk = (__u64)(unsigned long)(const void *)sk;
    struct sk_src_val_t *v = bpf_map_lookup_elem(&sk_src_map, &skk);
    if (!v)
        return -1;

    __u64 now = bpf_ktime_get_ns();
    if (now - v->ts_ns > 500000000ULL) // 500ms window
        return -1;

    if (v->family == AF_INET) {
        __u32 a = 0;
        __builtin_memcpy(&a, &v->src[0], 4);
        if (a)
            info->srcIP.s_addr = a;
    } else if (v->family == AF_INET6) {
        __builtin_memcpy(&info->srcIP6, v->src, 16);
    } else {
        return -1;
    }

    if (info->sport == 0 && v->sport)
        info->sport = v->sport;

    return 0;
}

/* ===================== net:net_dev_queue (real chosen src ip) ===================== */

SEC("tracepoint/net/net_dev_queue")
int trace_net_dev_queue(struct trace_event_raw_net_dev_queue *ctx)
{
    // Most kernels expose skbaddr here.
    const void *skb_p = BPF_CORE_READ(ctx, skbaddr);
    struct sk_buff *skb = (struct sk_buff *)skb_p;
    if (!skb)
        return 0;

    struct sock *sk = BPF_CORE_READ(skb, sk);
    if (!sk)
        return 0;

    __u64 skk = (__u64)(unsigned long)(const void *)sk;

    __be16 proto_be = BPF_CORE_READ(skb, protocol);
    __u16 l3 = bpf_ntohs(proto_be);

    void *head = BPF_CORE_READ(skb, head);
    __u16 nh   = BPF_CORE_READ(skb, network_header);
    if (!head)
        return 0;

    __u8 *iph = (__u8 *)head + nh;

    struct sk_src_val_t v = {};
    v.ts_ns = bpf_ktime_get_ns();

    if (l3 == ETH_P_IP) {
        __u8 vihl = 0;
        if (bpf_probe_read_kernel(&vihl, sizeof(vihl), iph) < 0)
            return 0;
        if ((vihl >> 4) != 4)
            return 0;

        __u32 ihl = (__u32)(vihl & 0x0f) * 4;
        if (ihl < 20)
            return 0;

        __u8 ipproto = 0;
        if (bpf_probe_read_kernel(&ipproto, sizeof(ipproto), iph + 9) < 0)
            return 0;

        __u32 saddr = 0;
        if (bpf_probe_read_kernel(&saddr, sizeof(saddr), iph + 12) < 0)
            return 0;

        v.family = AF_INET;
        v.proto  = ipproto;
        __builtin_memcpy(&v.src[0], &saddr, 4);

        if (ipproto == IPPROTO_UDP || ipproto == IPPROTO_TCP) {
            __u16 sport_be = 0;
            if (bpf_probe_read_kernel(&sport_be, sizeof(sport_be), iph + ihl) == 0)
                v.sport = bpf_ntohs(sport_be);
        }

        if (saddr != 0)
            bpf_map_update_elem(&sk_src_map, &skk, &v, BPF_ANY);

        return 0;
    }

    if (l3 == ETH_P_IPV6) {
        __u8 ver = 0;
        if (bpf_probe_read_kernel(&ver, sizeof(ver), iph) < 0)
            return 0;
        if ((ver >> 4) != 6)
            return 0;

        __u8 nexth = 0;
        if (bpf_probe_read_kernel(&nexth, sizeof(nexth), iph + 6) < 0)
            return 0;

        v.family = AF_INET6;
        v.proto  = nexth;

        if (bpf_probe_read_kernel(&v.src[0], 16, iph + 8) < 0)
            return 0;

        if (nexth == IPPROTO_UDP || nexth == IPPROTO_TCP) {
            __u16 sport_be = 0;
            if (bpf_probe_read_kernel(&sport_be, sizeof(sport_be), iph + 40) == 0)
                v.sport = bpf_ntohs(sport_be);
        }

        __u8 zero = 1;
#pragma unroll
        for (int i = 0; i < 16; i++) {
            if (v.src[i]) { zero = 0; break; }
        }
        if (!zero)
            bpf_map_update_elem(&sk_src_map, &skk, &v, BPF_ANY);

        return 0;
    }

    return 0;
}

/* ===================== connect ===================== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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

// NOTE: we do NOT emit EV_CONNECT here.
// We only populate fd_state_map + sk_owner_map.
// Real "TCP CONNECT" print comes from inet_sock_set_state when local port is assigned.
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

    // snapshot & cache
    struct fd_state_t st = {};
    if (fill_fd_state(in->fd, &st) == 0) {
        struct fd_key_t k = { .tgid = tgid, .fd = in->fd };
        bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

        // only TCP uses inet_sock_set_state
        if (st.proto == IPPROTO_TCP)
            remember_owner_for_fd(tgid, in->fd, ret, OWN_KIND_CONNECT);
    } else {
        // best effort: still try to remember owner (TCP) if sock_from_fd works
        remember_owner_for_fd(tgid, in->fd, ret, OWN_KIND_CONNECT);
    }

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== inet_sock_set_state ===================== */

SEC("tracepoint/sock/inet_sock_set_state")
int trace_inet_sock_set_state(struct trace_event_raw_inet_sock_set_state *ctx)
{
    const void *skp = BPF_CORE_READ(ctx, skaddr);
    if (!skp)
        return 0;

    __u64 skaddr = (__u64)(unsigned long)skp; // key for maps
    __u32 newstate = BPF_CORE_READ(ctx, newstate);

    struct sk_owner_t *own = bpf_map_lookup_elem(&sk_owner_map, &skaddr);
    if (!own)
        return 0;

    if (newstate != TCP_SYN_SENT && newstate != TCP_ESTABLISHED && newstate != TCP_CLOSE)
        return 0;

    struct sock *sk = (struct sock *)skp;

    // keep fd_state_map fresh (most accurate)
    struct fd_state_t st = {};
    if (fill_fd_state_from_sock(sk, &st) < 0)
        return 0;

    struct fd_key_t fk = { .tgid = own->pid, .fd = (__s32)own->fd };
    bpf_map_update_elem(&fd_state_map, &fk, &st, BPF_ANY);

    // cleanup on CLOSE
    if (newstate == TCP_CLOSE) {
        bpf_map_delete_elem(&sk_owner_map, &skaddr);
        return 0;
    }

    // emit CONNECT only for sockets that came from sys_exit_connect
    if (own->kind != OWN_KIND_CONNECT)
        return 0;

    // emit once when local port is known
    // prefer SYN_SENT, but if lport==0 there, emit later on ESTABLISHED
    if (!own->emitted && st.lport != 0 &&
        (newstate == TCP_SYN_SENT || newstate == TCP_ESTABLISHED))
    {
        struct trace_info info = {};
        info.sysexit = EV_CONNECT;
        info.state   = (__u8)newstate;

        info.pid = own->pid;
        info.fd  = own->fd;
        info.ret = own->conn_ret;

        __builtin_memcpy(info.comm, own->comm, sizeof(info.comm));

        info.proto  = st.proto;
        info.family = st.family;
        info.sport  = st.lport;
        info.dport  = st.rport;

        if (st.family == AF_INET) {
            info.srcIP.s_addr = st.lip;
            info.dstIP.s_addr = st.rip;
        } else if (st.family == AF_INET6) {
            __builtin_memcpy(&info.srcIP6, &st.lip6, sizeof(info.srcIP6));
            __builtin_memcpy(&info.dstIP6, &st.rip6, sizeof(info.dstIP6));
        } else {
            return 0;
        }

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));
        own->emitted = 1;
    }

    return 0;
}

/* ===================== accept/accept4 ===================== */

static __always_inline int accept_enter_common(void)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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

    // snapshot state
    struct fd_state_t st = {};
    if (fill_fd_state((int)newfd, &st) < 0)
        goto cleanup;

    struct fd_key_t k = { .tgid = tgid, .fd = (int)newfd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);

    struct conn_info_t *conn = bpf_map_lookup_elem(&conn_info_map, &id);

    struct trace_info info = {};
    info.sysexit = EV_ACCEPT;
    info.state   = 0;

    info.pid     = tgid;
    info.fd      = (__u32)newfd;
    info.ret     = newfd; // accept() return = new fd

    // accept: src=remote, dst=local
    if (apply_state_to_info(&info, &st, /*is_send=*/0) < 0)
        goto cleanup;

    if (conn) __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else bpf_get_current_comm(info.comm, sizeof(info.comm));

    // remember owner to keep fd_state fresh on state changes (no EV_CONNECT emit)
    remember_owner_for_fd(tgid, (int)newfd, 0, OWN_KIND_ACCEPT);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx)
{
    (void)ctx;
    return accept_enter_common();
}

SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct trace_event_raw_sys_exit *ctx)
{
    return accept_exit_common(ctx);
}

SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept_enter(struct trace_event_raw_sys_enter *ctx)
{
    (void)ctx;
    return accept_enter_common();
}

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx)
{
    return accept_exit_common(ctx);
}

/* ===================== close ===================== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u32 tgid = bpf_get_current_pid_tgid() >> 32;
    int fd     = (int)ctx->args[0];

    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);

    // best-effort remove sk_owner + sk_src cache
    struct sock *sk = sock_from_fd(fd);
    if (sk) {
        __u64 skk = (__u64)(unsigned long)(const void *)sk;
        bpf_map_delete_elem(&sk_owner_map, &skk);
        bpf_map_delete_elem(&sk_src_map, &skk);
    }

    return 0;
}

/* ===================== bind ===================== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrBind_map, &id);
    if (!ap || !ap->addr || !ap->len)
        goto cleanup;

    // cache fd state after successful bind (helps show local ip/port for UDP)
    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) == 0) {
        struct fd_key_t k = { .tgid = tgid, .fd = (int)ci->fd };
        bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
    }

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.pid     = ci->pid;
    info.sysexit = EV_BINDOK;

    info.fd  = ci->fd;
    info.ret = ret;

    // for bind we put bound address into dst fields (server side)
    (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    if (info.family == 0) {
        // fallback: if sockaddr parsing failed, try fd state
        (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 1);
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== sendto ===================== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret <= 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_SENDTO;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // fill from fd state (TCP connected sockets work here)
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 1);

    // override dst from user sockaddr if provided (UDP sendto)
    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    // ✅ final fallback: real chosen src ip from net_dev_queue cache
    (void)maybe_fill_src_from_sk_cache(&info, (int)ci->fd);

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== recvfrom ===================== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];
    if (uaddr && lenp_u) {
        struct addr_recv_meta_t m = {.addr = uaddr, .lenp = lenp_u};
        bpf_map_update_elem(&addrRecv_map, &id, &m, BPF_ANY);
    }

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

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_RECVFROM;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // recv: src=remote, dst=local
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 0);

    // override src from user sockaddr if kernel filled it
    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && m->addr && m->lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void *)m->lenp) == 0) {
            if (addrlen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, (void *)m->addr, addrlen, 0);
        }
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== sendmsg ===================== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
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

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_SENDMSG;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // try fd_state for connected sockets
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 1);

    // if msg_name present -> dst from it (often UDP)
    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(*msgp, &h) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 1);
        }
    }

    // ✅ final fallback: real chosen src ip from net_dev_queue cache
    (void)maybe_fill_src_from_sk_cache(&info, (int)ci->fd);

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===================== recvmsg ===================== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 msg_u = (__u64)ctx->args[1];
    if (msg_u)
        bpf_map_update_elem(&msgRecv_map, &id, &msg_u, BPF_ANY);

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

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_RECVMSG;
    info.pid     = ci->pid;

    info.fd  = ci->fd;
    info.ret = ret;

    // recv: src=remote, dst=local
    (void)fill_from_fd_state_any(&info, tgid, (int)ci->fd, 0);

    // if msg_name present -> src from it (depends on caller setup)
    __u64 *msgp = bpf_map_lookup_elem(&msgRecv_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(*msgp, &h) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 0);
        }
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgRecv_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}




// net:net_dev_queue tracepoint ctx (minimal)
// common_* всегда первые 8 байт, дальше поля как в tracepoint format
struct tp_net_dev_queue_ctx {
    __u16 common_type;
    __u8  common_flags;
    __u8  common_preempt_count;
    __s32 common_pid;

    const void *skbaddr;   // offset обычно 8
    __u32 len;             // дальше
    __u32 name_loc;        // __data_loc char[] name (может быть, просто чтобы выровнять)
};



SEC("tracepoint/net/net_dev_queue")
int trace_net_dev_queue(struct tp_net_dev_queue_ctx *ctx)
{
    const struct sk_buff *skb = (const struct sk_buff *)(unsigned long)ctx->skbaddr;
    if (!skb)
        return 0;

    // ... твоя логика с skb ...
    return 0;
}




package main

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"errors"
	"flag"
	"fmt"
	"log"
	"net"
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
Assumes generated by bpf2go from NEW trace.c:

  - type bpfObjects struct {
        TraceEvents *ebpf.Map
        TraceNetDevQueue *ebpf.Program       // NEW (tracepoint/net/net_dev_queue)
        TraceSendtoEnter *ebpf.Program
        TraceSendtoExit  *ebpf.Program
        ...
        TraceInetSockSetState *ebpf.Program
    }

  - func loadBpfObjects(*bpfObjects, *ebpf.CollectionOptions) error
  - type bpfTraceInfo struct { ... } // from struct trace_info in C
*/

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

	TCP_ESTABLISHED = 1
	TCP_SYN_SENT    = 2
	TCP_CLOSE       = 7
)

var (
	flgTracePort     = flag.Uint("tracePort", 0, "UDP port to resolve dst/src owner via /proc snapshot (0=off)")
	flgSnapshotEvery = flag.Duration("snapshotEvery", 0, "refresh UDP owners snapshot every duration (0=only once)")
	flgTTL           = flag.Duration("ttl", 3*time.Second, "TTL for in-memory caches")
	flgDebug         = flag.Bool("debug", false, "debug logs")
	flgPerfMB        = flag.Int("perfMB", 4, "perf ring buffer size in MB")
	flgSample        = flag.Uint64("sample", 1, "print every Nth event (>=1)")

	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", ":6060", "pprof listen addr")
)

func dbg(format string, args ...any) {
	if *flgDebug {
		log.Printf("[DBG] "+format, args...)
	}
}

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

/* ===== caches ===== */

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096) // (family, ip, port)->proc

	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384) // (client->server)->client proc

	pendMu sync.Mutex
	pendBy = make(map[ConnKey]PendingConnect, 16384)

	udpMu     sync.Mutex
	udpByPort = make(map[uint16]Proc, 65536)

	udpOwnerAny atomic.Value // map[uint16]Proc snapshot
)

/* ===== outgoing IP cache (userspace fallback) ===== */

type ipCacheEnt struct {
	IP   string
	Seen time.Time
}

var (
	outIPMu    sync.Mutex
	outIPCache = make(map[string]ipCacheEnt, 1024) // key "4|dst" or "6|dst"
)

// В НОВОМ eBPF src обычно уже корректный (через net_dev_queue + sk_src_map внутри eBPF).
// Оставляем userspace fallback только если src всё ещё "*" и dst не локальный.
func resolveOutboundIP(family uint16, dst string) string {
	key := fmt.Sprintf("%d|%s", family, dst)

	outIPMu.Lock()
	if v, ok := outIPCache[key]; ok && time.Since(v.Seen) < 10*time.Second {
		outIPMu.Unlock()
		return v.IP
	}
	outIPMu.Unlock()

	var ip string
	rip := net.ParseIP(dst)
	if rip == nil {
		return ""
	}

	if family == AF_INET {
		c, err := net.DialUDP("udp4", nil, &net.UDPAddr{IP: rip, Port: 9})
		if err == nil {
			if la, ok := c.LocalAddr().(*net.UDPAddr); ok && la.IP != nil {
				ip = la.IP.String()
			}
			_ = c.Close()
		}
	} else if family == AF_INET6 {
		c, err := net.DialUDP("udp6", nil, &net.UDPAddr{IP: rip, Port: 9})
		if err == nil {
			if la, ok := c.LocalAddr().(*net.UDPAddr); ok && la.IP != nil {
				ip = la.IP.String()
			}
			_ = c.Close()
		}
	}

	if ip != "" {
		outIPMu.Lock()
		outIPCache[key] = ipCacheEnt{IP: ip, Seen: time.Now()}
		outIPMu.Unlock()
	}
	return ip
}

/* ===== helpers ===== */

func setUDPOwners(m map[uint16]Proc) { udpOwnerAny.Store(m) }
func getUDPOwnerSnapshot(port uint16) (Proc, bool) {
	m, _ := udpOwnerAny.Load().(map[uint16]Proc)
	if m == nil {
		return Proc{}, false
	}
	p, ok := m[port]
	return p, ok
}

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

func tcpStateName(s uint8) string {
	switch s {
	case TCP_SYN_SENT:
		return "SYN_SENT"
	case TCP_ESTABLISHED:
		return "ESTABLISHED"
	case TCP_CLOSE:
		return "CLOSE"
	default:
		if s == 0 {
			return "-"
		}
		return fmt.Sprintf("%d", s)
	}
}

// in_addr.s_addr from kernel is net-order but comes as uint32 on little-endian.
// We want bytes in normal order for printing: 127.0.0.1 etc.
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
	if isAllZero16(ip) {
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

func endpFromEvIPv6(ip16 [16]byte, port uint16) (ipStr string, ipKey [16]byte, portU16 uint16) {
	copy(ipKey[:], ip16[:])
	return net.IP(ipKey[:]).String(), ipKey, port
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

func formatIPOnly(ipStr string, ipKey [16]byte) string {
	if isAllZero16(ipKey) {
		return "*"
	}
	return ipStr
}

/* ===== cache ops ===== */

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

/* ===== /proc UDP owners snapshot ===== */

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

func buildInodeToProc(selfPID uint32) map[uint64]Proc {
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
		if pid == selfPID {
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
			trim := strings.TrimSuffix(strings.TrimPrefix(target, "socket:["), "]")
			inode, err := strconv.ParseUint(trim, 10, 64)
			if err != nil || inode == 0 {
				continue
			}
			out[inode] = Proc{Pid: pid, Comm: comm}
		}
	}
	return out
}

func parseProcNetInodes(path string) []inodePort {
	f, err := os.Open(path)
	if err != nil {
		return nil
	}
	defer f.Close()

	sc := bufio.NewScanner(f)
	if !sc.Scan() {
		return nil
	}

	var out []inodePort
	for sc.Scan() {
		fields := strings.Fields(strings.TrimSpace(sc.Text()))
		if len(fields) < 10 {
			continue
		}
		local := fields[1]
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

func snapshotUDPOwners(tracePort uint16) {
	selfPID := uint32(os.Getpid())
	inode2proc := buildInodeToProc(selfPID)

	udp4 := parseProcNetInodes("/proc/net/udp")
	udp6 := parseProcNetInodes("/proc/net/udp6")

	owners := make(map[uint16]Proc, 256)

	for _, it := range udp4 {
		if p, ok := inode2proc[it.Inode]; ok {
			owners[it.Port] = p
		}
	}
	for _, it := range udp6 {
		if p, ok := inode2proc[it.Inode]; ok {
			if _, exists := owners[it.Port]; !exists {
				owners[it.Port] = p
			}
		}
	}

	setUDPOwners(owners)

	if tracePort != 0 {
		if p, ok := getUDPOwnerSnapshot(tracePort); ok {
			dbg("snapshotUDPOwners: port=%d owner=%s", tracePort, p.String())
		} else {
			dbg("snapshotUDPOwners: port=%d owner NOT FOUND", tracePort)
		}
	}
}

/* ===== printing ===== */

func printTCPConnect(p Proc, fd uint32, ret int64, state uint8, clientEp, serverEp string, server Proc, ok bool) {
	s := "?"
	if ok {
		s = server.String()
	}
	fmt.Printf("TCP CONNECT client=%s fd=%d ret=%d state=%s  %s -> %s  server=%s\n",
		p.String(), fd, ret, tcpStateName(state), clientEp, serverEp, s)
}

func printTCPAccept(server Proc, fd uint32, ret int64, serverEp, clientEp string, client Proc, ok bool) {
	c := "?"
	if ok {
		c = client.String()
	}
	fmt.Printf("TCP ACCEPT  server=%s fd=%d ret=%d  %s -> %s  client=%s\n",
		server.String(), fd, ret, serverEp, clientEp, c)
}

func printSendLine(proto, kind string, p Proc, fd uint32, ret int64, srcEp, dstEp string, dst Proc, ok bool) {
	d := "?"
	if ok {
		d = dst.String()
	}
	fmt.Printf("%s %-7s pid=%s fd=%d ret=%d  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), fd, ret, p.String(), srcEp, dstEp, d)
}

func printRecvLine(proto, kind string, p Proc, fd uint32, ret int64, src Proc, ok bool, srcEp, dstEp string) {
	s := "?"
	if ok {
		s = src.String()
	}
	fmt.Printf("%s %-7s pid=%s fd=%d ret=%d  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), fd, ret, s, srcEp, dstEp, p.String())
}

func printICMP(proto, kind string, p Proc, fd uint32, ret int64, srcIPOnly, dstIPOnly string) {
	fmt.Printf("%s %-7s pid=%s fd=%d ret=%d  %s -> %s\n",
		proto, kind, p.String(), fd, ret, srcIPOnly, dstIPOnly)
}

/* ===== raw field access (stable vs bpf2go renames) ===== */

func evInAddrU32(ptr unsafe.Pointer) uint32 {
	return *(*uint32)(ptr) // first 4 bytes of struct in_addr
}

func evIn6AddrBytes(ptr unsafe.Pointer) (out [16]byte) {
	copy(out[:], (*[16]byte)(ptr)[:])
	return
}

/* ===== main ===== */

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	if *flgSample < 1 {
		*flgSample = 1
	}

	tracePort := uint16(*flgTracePort)

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

	go cleanupTTL(*flgTTL)

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

	attachOptional := func(cat, name string, prog *ebpf.Program) {
		if prog == nil {
			dbg("optional attach %s/%s: prog=nil", cat, name)
			return
		}
		l, err := link.Tracepoint(cat, name, prog, nil)
		if err != nil {
			log.Printf("[WARN] optional attach %s/%s failed: %v (continue)", cat, name, err)
			return
		}
		dbg("optional attached %s/%s", cat, name)
		links = append(links, l)
	}

	// NEW: real-chosen src-ip cache inside eBPF
	attachOptional("net", "net_dev_queue", objs.TraceNetDevQueue)

	attach("syscalls", "sys_enter_bind", objs.TraceBindEnter)
	attach("syscalls", "sys_exit_bind", objs.TraceBindExit)

	attach("syscalls", "sys_enter_connect", objs.TraceConnectEnter)
	attach("syscalls", "sys_exit_connect", objs.TraceConnectExit)

	attach("sock", "inet_sock_set_state", objs.TraceInetSockSetState)

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

		n := atomic.AddUint64(&printed, 1)
		if n%*flgSample != 0 {
			continue
		}

		p := Proc{Pid: ev.Pid, Comm: comm}
		fd := uint32(ev.Fd)
		ret := int64(ev.Ret)

		family := uint16(ev.Family)
		sport := uint16(ev.Sport)
		dport := uint16(ev.Dport)

		var (
			srcEp, dstEp         string
			srcIPKey, dstIPKey   [16]byte
			srcIPStr, dstIPStr   string
			srcIPOnly, dstIPOnly string
		)

		switch family {
		case AF_INET:
			// не привязываемся к имени поля S_addr/SAddr — читаем первые 4 байта struct in_addr
			s4 := evInAddrU32(unsafe.Pointer(&ev.SrcIP))
			d4 := evInAddrU32(unsafe.Pointer(&ev.DstIP))

			sIP, sKey, sP := endpFromEvIPv4(s4, sport)
			dIP, dKey, dP := endpFromEvIPv4(d4, dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP

			srcEp = formatEndp(AF_INET, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET, dIP, dKey, dP)
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

		case AF_INET6:
			s6 := evIn6AddrBytes(unsafe.Pointer(&ev.SrcIP6))
			d6 := evIn6AddrBytes(unsafe.Pointer(&ev.DstIP6))

			sIP, sKey, sP := endpFromEvIPv6(s6, sport)
			dIP, dKey, dP := endpFromEvIPv6(d6, dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP

			srcEp = formatEndp(AF_INET6, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET6, dIP, dKey, dP)
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

		default:
			continue
		}

		// Userspace fallback: только если src всё ещё "*" и dst не локальный.
		// В новом eBPF чаще всего это уже не нужно.
		if isAllZero16(srcIPKey) && !isLocalish(family, dstIPKey) && dstIPStr != "" {
			if ip := resolveOutboundIP(family, dstIPStr); ip != "" {
				if family == AF_INET {
					if v4 := net.ParseIP(ip).To4(); v4 != nil {
						srcIPStr = ip
						copy(srcIPKey[:4], v4)
						srcEp = formatEndp(AF_INET, srcIPStr, srcIPKey, sport)
						srcIPOnly = formatIPOnly(srcIPStr, srcIPKey)
					}
				} else if family == AF_INET6 {
					if v6 := net.ParseIP(ip).To16(); v6 != nil {
						srcIPStr = ip
						copy(srcIPKey[:], v6)
						srcEp = formatEndp(AF_INET6, srcIPStr, srcIPKey, sport)
						srcIPOnly = formatIPOnly(srcIPStr, srcIPKey)
					}
				}
			}
		}

		// proto in new trace_info is u32; values are still 6/17/...
		proto := uint8(ev.Proto)
		pr := protoStr(proto)
		evn := evName(uint8(ev.Sysexit))
		state := uint8(ev.State)

		switch uint8(ev.Sysexit) {

		case EV_BINDOK:
			// remember listener (bind writes bound addr into dst fields in eBPF)
			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, p)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, p)

		case EV_CONNECT:
			// comes from inet_sock_set_state; should have real sport now (for TCP)
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
				printTCPConnect(p, fd, ret, state, srcEp, dstEp, server, true)
			} else {
				savePending(key, PendingConnect{
					Client:   p,
					ClientEp: srcEp,
					ServerEp: dstEp,
					Seen:     time.Now(),
				})
				printTCPConnect(p, fd, ret, state, srcEp, dstEp, Proc{}, false)
			}

		case EV_ACCEPT:
			if proto != IPPROTO_TCP {
				break
			}
			server := p

			// keep server listen cache best-effort
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
				printTCPConnect(pc.Client, fd, 0, 0, pc.ClientEp, pc.ServerEp, server, true)
				printTCPAccept(server, fd, ret, pc.ServerEp, pc.ClientEp, pc.Client, true)
				break
			}

			client, ok := lookupConn(key)
			printTCPAccept(server, fd, ret, dstEp, srcEp, client, ok)

		case EV_SENDTO, EV_SENDMSG:
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				printICMP(pr, evn, p, fd, ret, srcIPOnly, dstIPOnly)
				break
			}

			// learn UDP sender by source port
			if proto == IPPROTO_UDP && sport != 0 {
				udpSetOwner(sport, p)
			}

			// resolve dst pid for UDP local and local TCP(loopback)
			var dstOwner Proc
			var ok bool

			if proto == IPPROTO_UDP && isLocalish(family, dstIPKey) && dport != 0 {
				if dstOwner, ok = udpGetOwner(dport); !ok {
					if dstOwner, ok = getUDPOwnerSnapshot(dport); !ok {
						dstOwner, ok = lookupListen(family, dstIPKey, dport)
					}
				}
			} else if proto == IPPROTO_TCP && isLocalish(family, dstIPKey) && dport != 0 {
				dstOwner, ok = lookupListen(family, dstIPKey, dport)
			}

			printSendLine(pr, evn, p, fd, ret, srcEp, dstEp, dstOwner, ok)

		case EV_RECVFROM, EV_RECVMSG:
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				printICMP(pr, evn, p, fd, ret, srcIPOnly, dstIPOnly)
				break
			}

			// learn UDP receiver by local port
			if proto == IPPROTO_UDP && dport != 0 {
				udpSetOwner(dport, p)
			}

			var srcOwner Proc
			var ok bool

			if proto == IPPROTO_UDP && isLocalish(family, srcIPKey) && sport != 0 {
				if srcOwner, ok = udpGetOwner(sport); !ok {
					if srcOwner, ok = getUDPOwnerSnapshot(sport); !ok {
						srcOwner, ok = lookupListen(family, srcIPKey, sport)
					}
				}
			} else if proto == IPPROTO_TCP {
				// try resolve local peer on loopback
				if isLocalish(family, srcIPKey) && isLocalish(family, dstIPKey) && sport != 0 && dport != 0 {
					key1 := ConnKey{Family: family, ClientIP: srcIPKey, ClientPort: sport, ServerIP: dstIPKey, ServerPort: dport}
					if c, ok1 := lookupConn(key1); ok1 {
						srcOwner, ok = c, true
					} else {
						key2 := ConnKey{Family: family, ClientIP: dstIPKey, ClientPort: dport, ServerIP: srcIPKey, ServerPort: sport}
						if _, ok2 := lookupConn(key2); ok2 {
							// then "src" is server; resolve server by listen(sport)
							if s, okS := lookupListen(family, srcIPKey, sport); okS {
								srcOwner, ok = s, true
							}
						}
					}
				}
			}

			printRecvLine(pr, evn, p, fd, ret, srcOwner, ok, srcEp, dstEp)
		}
	}
}

