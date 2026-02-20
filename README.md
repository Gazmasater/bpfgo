
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

#define AF_INET  2
#define AF_INET6 10

#define EINPROGRESS 115
#define EALREADY    114

#define IPPROTO_ICMP   1
#define IPPROTO_TCP    6
#define IPPROTO_UDP    17
#define IPPROTO_ICMPV6 58

#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_BINDOK    20
#define EV_SENDMSG   11
#define EV_RECVMSG   12

#define MSG_PEEK 0x2

/* ====== types ====== */

struct conn_info_t {
    __u32 tgid;
    __u32 tid;
    __u32 fd;
    __u32 _pad;
    char  comm[32];
};

struct fd_key_t {
    __u32 tgid;
    __s32 fd;
};

struct fd_state_t {
    __u16 family;
    __u8  proto;
    __u8  _pad0;

    __u16 lport;
    __u16 rport;

    __u32 lip;     // v4 net order
    __u32 rip;     // v4 net order

    struct in6_addr lip6;
    struct in6_addr rip6;
};

struct trace_info {
    struct in_addr  srcIP;
    struct in_addr  dstIP;

    struct in6_addr srcIP6;
    struct in6_addr dstIP6;

    __u32 tgid;
    __u32 tid;
    __u32 proto;
    __u32 fd;

    __s64 ret;
    __u64 cookie;  // ✅ socket cookie

    __u16 sport;
    __u16 dport;
    __u16 family;

    __u8  sysexit;
    __u8  state;

    char  comm[32];
};

const struct trace_info *unused __attribute__((unused));

struct user_msghdr_head {
    void *msg_name;
    __u32 msg_namelen;
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

/* ====== maps ====== */

// fd -> cookie (per tgid)  ✅ LRU
struct {
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 65536);
    __type(key, struct fd_key_t);
    __type(value, __u64);
} fd_cookie_map SEC(".maps");

// cookie -> state  ✅ LRU
struct {
    __uint(type, BPF_MAP_TYPE_LRU_HASH);
    __uint(max_entries, 131072);
    __type(key, __u64);
    __type(value, struct fd_state_t);
} cookie_state_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
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
    __type(value, __u64);
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct msg_ptrflags_t);
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ====== helpers ====== */

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

        info->family = AF_INET;
        if (is_dst) {
            info->dstIP.s_addr = sa.sin_addr.s_addr;
            info->dport = bpf_ntohs(sa.sin_port);
        } else {
            info->srcIP.s_addr = sa.sin_addr.s_addr;
            info->sport = bpf_ntohs(sa.sin_port);
        }
        return 0;
    }

    if (family == AF_INET6) {
        if (addrlen < sizeof(struct sockaddr_in6))
            return -1;

        struct sockaddr_in6 sa6 = {};
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0)
            return -1;

        info->family = AF_INET6;
        if (is_dst) {
            __builtin_memcpy(&info->dstIP6, &sa6.sin6_addr, sizeof(info->dstIP6));
            info->dport = bpf_ntohs(sa6.sin6_port);
        } else {
            __builtin_memcpy(&info->srcIP6, &sa6.sin6_addr, sizeof(info->srcIP6));
            info->sport = bpf_ntohs(sa6.sin6_port);
        }
        return 0;
    }

    return -1;
}

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

static __always_inline int read_state_from_sock(int fd, __u64 *cookie_out, struct fd_state_t *st)
{
    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return -1;

    __u64 cookie = bpf_get_socket_cookie(sk);
    if (cookie_out)
        *cookie_out = cookie;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);
    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);

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

static __always_inline int get_state_for_fd(__u32 tgid, int fd, __u64 *cookie_out, struct fd_state_t *st_out)
{
    struct fd_key_t fk = {.tgid = tgid, .fd = fd};

    __u64 *cookiep = bpf_map_lookup_elem(&fd_cookie_map, &fk);
    if (cookiep) {
        __u64 cookie = *cookiep;
        struct fd_state_t *cached = bpf_map_lookup_elem(&cookie_state_map, &cookie);
        if (cached) {
            if (cached->proto == IPPROTO_TCP && (cached->lport == 0 || cached->rport == 0)) {
                struct fd_state_t tmp = {};
                __u64 c2 = 0;
                if (read_state_from_sock(fd, &c2, &tmp) == 0) {
                    if (c2 != cookie) {
                        bpf_map_update_elem(&fd_cookie_map, &fk, &c2, BPF_ANY);
                        cookie = c2;
                    }
                    bpf_map_update_elem(&cookie_state_map, &cookie, &tmp, BPF_ANY);
                    *st_out = tmp;
                    if (cookie_out) *cookie_out = cookie;
                    return 0;
                }
            }

            *st_out = *cached;
            if (cookie_out) *cookie_out = cookie;
            return 0;
        }
    }

    struct fd_state_t tmp = {};
    __u64 cookie = 0;
    if (read_state_from_sock(fd, &cookie, &tmp) < 0)
        return -1;

    bpf_map_update_elem(&fd_cookie_map, &fk, &cookie, BPF_ANY);
    bpf_map_update_elem(&cookie_state_map, &cookie, &tmp, BPF_ANY);

    *st_out = tmp;
    if (cookie_out) *cookie_out = cookie;
    return 0;
}

static __always_inline void fill_event_from_state(struct trace_info *info, const struct fd_state_t *st, int is_send)
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
    } else if (st->family == AF_INET6) {
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
    }
}

static __always_inline void save_conn_enter(__u64 id, int fd)
{
    __u32 tgid = id >> 32;
    __u32 tid  = (__u32)id;

    struct conn_info_t ci = {};
    ci.tgid = tgid;
    ci.tid  = tid;
    ci.fd   = (__u32)fd;
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);
}

/* ====== connect ====== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);

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
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0)
        goto cleanup;

    if (ret < 0 && ret != -EINPROGRESS && ret != -EALREADY)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_CONNECT;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = ret;
    info.state   = (ret < 0) ? 1 : 0;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)ci->fd, &cookie, &st) < 0)
        goto cleanup;

    info.cookie = cookie;
    fill_event_from_state(&info, &st, 1);

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== accept/accept4 ====== */

static __always_inline int accept_enter_common(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);
    return 0;
}

static __always_inline int accept_exit_common(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    __s64 newfd = 0;
    if (read_sys_exit_ret(ctx, &newfd) < 0 || newfd < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_ACCEPT;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = newfd;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)newfd, &cookie, &st) < 0)
        goto cleanup;

    info.cookie = cookie;
    fill_event_from_state(&info, &st, 0);

    if (info.family == 0)
        goto cleanup;

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

/* ====== close (fd->cookie cleanup only; cookie/state LRU handles rest) ====== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd = (int)ctx->args[0];

    struct fd_key_t fk = {.tgid = tgid, .fd = fd};
    bpf_map_delete_elem(&fd_cookie_map, &fk);
    return 0;
}

/* ====== bind ====== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);

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

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_BINDOK;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = ret;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)ci->fd, &cookie, &st) == 0) {
        info.cookie = cookie;
        info.proto  = st.proto;
    }

    (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);
    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrBind_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== sendto ====== */

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);

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

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_SENDTO;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = ret;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)ci->fd, &cookie, &st) < 0)
        goto cleanup;

    info.cookie = cookie;
    fill_event_from_state(&info, &st, 1);

    struct addr_ptrlen_t *ap = bpf_map_lookup_elem(&addrSend_map, &id);
    if (ap && ap->addr && ap->len)
        (void)fill_from_sockaddr_user(&info, (void *)ap->addr, ap->len, 1);

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvfrom (FILTER MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);

    __u32 flags  = (__u32)ctx->args[3];
    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];

    if ((flags & MSG_PEEK) || (uaddr && lenp_u)) {
        struct addr_recv_meta_t m = {.addr = uaddr, .lenp = lenp_u, .flags = flags};
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

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_RECVFROM;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = ret;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)ci->fd, &cookie, &st) < 0)
        goto cleanup;

    info.cookie = cookie;
    fill_event_from_state(&info, &st, 0);

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

/* ====== sendmsg ====== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);

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

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_SENDMSG;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = ret;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)ci->fd, &cookie, &st) < 0)
        goto cleanup;

    info.cookie = cookie;
    fill_event_from_state(&info, &st, 1);

    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(*msgp, &h) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16))
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 1);
        }
    }

    if (info.family == 0)
        goto cleanup;

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&msgSend_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ====== recvmsg (FILTER MSG_PEEK) ====== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    save_conn_enter(id, (int)ctx->args[0]);

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

    struct trace_info info = {};
    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));
    info.sysexit = EV_RECVMSG;
    info.tgid    = ci->tgid;
    info.tid     = ci->tid;
    info.fd      = ci->fd;
    info.ret     = ret;

    struct fd_state_t st = {};
    __u64 cookie = 0;
    if (get_state_for_fd(tgid, (int)ci->fd, &cookie, &st) < 0)
        goto cleanup;

    info.cookie = cookie;
    fill_event_from_state(&info, &st, 0);

    if (pv && pv->msg) {
        struct user_msghdr_head h = {};
        if (read_msghdr_head(pv->msg, &h) == 0) {
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
Assumes generated by bpf2go:
  - type bpfObjects struct { TraceEvents *ebpf.Map; TraceSendtoEnter *ebpf.Program; ... }
  - func loadBpfObjects(*bpfObjects, *ebpf.CollectionOptions) error
  - type bpfTraceInfo struct { ... } // from struct trace_info in C:
        SrcIP, DstIP, SrcIP6, DstIP6,
        Tgid, Tid, Proto, Fd, Ret, Cookie, Sport, Dport, Family, Sysexit, State, Comm
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
)

/* ===================== flags ===================== */

var (
	flgTracePort     = flag.Uint("tracePort", 0, "port to focus/resolve owners via /proc snapshot (0=off)")
	flgSnapshotEvery = flag.Duration("snapshotEvery", 0, "refresh UDP owners snapshot (0=only once)")
	flgTTL           = flag.Duration("ttl", 3*time.Second, "TTL for in-memory caches")
	flgDebug         = flag.Bool("debug", false, "debug logs")
	flgPerfMB        = flag.Int("perfMB", 4, "perf ring buffer size in MB")
	flgPrint         = flag.Bool("print", true, "print events")
	flgSample        = flag.Uint64("sample", 1, "print every Nth matched event (>=1)")
	flgMatchOnly     = flag.Bool("matchOnly", false, "if tracePort>0, only print when sport/dport==tracePort")
	flgStats         = flag.Duration("stats", 0, "print stats every interval (0=off)")

	flgPprof     = flag.Bool("pprof", true, "enable pprof")
	flgPprofAddr = flag.String("pprofAddr", "127.0.0.1:6060", "pprof listen addr (use 127.0.0.1 to avoid v4/v6 weirdness)")

	flgRouteSrc = flag.Bool("routeSrc", true, "if srcIP is wildcard (*), infer outgoing srcIP by routing to dstIP (cached)")
)

func dbg(format string, args ...any) {
	if *flgDebug {
		log.Printf("[DBG] "+format, args...)
	}
}

/* ===================== types ===================== */

type Proc struct {
	Pid  uint32 // tgid
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

type PendingConnect struct {
	Client   Proc
	ClientEp string
	ServerEp string
	Seen     time.Time

	Fd     uint32
	Ret    int64
	Cookie uint64

	Family  uint16
	SrvIP   [16]byte
	SrvPort uint16
}

/* ===================== caches ===================== */

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096)

	tcpMu       sync.Mutex
	tcpClientBy = make(map[ConnKey]Proc, 16384)
	tcpServerBy = make(map[ConnKey]Proc, 16384)

	// pending:
	tcpPendingMu      sync.Mutex
	tcpPendByTuple    = make(map[ConnKey]PendingConnect, 16384)
	tcpPendByCookie   = make(map[uint64]PendingConnect, 16384)

	udpMu     sync.Mutex
	udpByPort = make(map[uint16]Proc, 65536)

	udpOwnerAny atomic.Value // map[uint16]Proc snapshot

	routeMu    sync.Mutex
	routeCache = make(map[[18]byte]routeVal, 4096)
)

type routeVal struct {
	ipStr  string
	ipKey  [16]byte
	expire time.Time
}

func setUDPOwners(m map[uint16]Proc) { udpOwnerAny.Store(m) }
func getUDPOwnerSnapshot(port uint16) (Proc, bool) {
	m, _ := udpOwnerAny.Load().(map[uint16]Proc)
	if m == nil {
		return Proc{}, false
	}
	p, ok := m[port]
	return p, ok
}

/* ===================== comm/proto/event ===================== */

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

/* ===================== endpoint helpers ===================== */

func ip4BytesFromU32Net(x uint32) (b [4]byte) {
	// важно: x приходит как uint32, но в little-endian => кладём LittleEndian
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

func formatIPOnly(ipStr string, ipKey [16]byte) string {
	if isAllZero16(ipKey) {
		return "*"
	}
	return ipStr
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

/* ===================== route-based src ===================== */

func routeKey(family uint16, dst [16]byte) (k [18]byte) {
	k[0] = byte(family >> 8)
	k[1] = byte(family)
	copy(k[2:], dst[:])
	return
}

func tryRouteLocalIP(family uint16, dstIPKey [16]byte, dport uint16) (string, [16]byte, bool) {
	var outKey [16]byte
	if isAllZero16(dstIPKey) {
		return "", outKey, false
	}

	now := time.Now()
	k := routeKey(family, dstIPKey)

	routeMu.Lock()
	if v, ok := routeCache[k]; ok && now.Before(v.expire) {
		routeMu.Unlock()
		return v.ipStr, v.ipKey, true
	}
	routeMu.Unlock()

	port := int(dport)
	if port == 0 {
		port = 443
	}

	var (
		lip net.IP
		err error
	)

	if family == AF_INET {
		dst := net.IPv4(dstIPKey[0], dstIPKey[1], dstIPKey[2], dstIPKey[3])
		c, e := net.DialUDP("udp4", nil, &net.UDPAddr{IP: dst, Port: port})
		if e != nil {
			err = e
		} else {
			lip = c.LocalAddr().(*net.UDPAddr).IP
			_ = c.Close()
		}
	} else if family == AF_INET6 {
		dst := net.IP(dstIPKey[:])
		c, e := net.DialUDP("udp6", nil, &net.UDPAddr{IP: dst, Port: port})
		if e != nil {
			err = e
		} else {
			lip = c.LocalAddr().(*net.UDPAddr).IP
			_ = c.Close()
		}
	} else {
		return "", outKey, false
	}

	if err != nil || lip == nil {
		return "", outKey, false
	}

	if family == AF_INET {
		l4 := lip.To4()
		if l4 == nil {
			return "", outKey, false
		}
		copy(outKey[:4], l4)
		ipStr := fmt.Sprintf("%d.%d.%d.%d", l4[0], l4[1], l4[2], l4[3])

		routeMu.Lock()
		routeCache[k] = routeVal{ipStr: ipStr, ipKey: outKey, expire: now.Add(30 * time.Second)}
		routeMu.Unlock()
		return ipStr, outKey, true
	}

	l16 := lip.To16()
	if l16 == nil {
		return "", outKey, false
	}
	copy(outKey[:], l16)
	ipStr := net.IP(l16).String()

	routeMu.Lock()
	routeCache[k] = routeVal{ipStr: ipStr, ipKey: outKey, expire: now.Add(30 * time.Second)}
	routeMu.Unlock()
	return ipStr, outKey, true
}

/* ===================== caches ops ===================== */

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
	// wildcard fallback
	var zero [16]byte
	k2 := EndpKey{Family: family, Port: port, IP: zero}
	listenMu.Lock()
	p2, ok2 := listenBy[k2]
	listenMu.Unlock()
	return p2, ok2
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

func tcpSaveClient(key ConnKey, p Proc) {
	p.Seen = time.Now()
	tcpMu.Lock()
	tcpClientBy[key] = p
	tcpMu.Unlock()
}

func tcpSaveServer(key ConnKey, p Proc) {
	p.Seen = time.Now()
	tcpMu.Lock()
	tcpServerBy[key] = p
	tcpMu.Unlock()
}

func tcpGetClient(key ConnKey) (Proc, bool) {
	tcpMu.Lock()
	p, ok := tcpClientBy[key]
	tcpMu.Unlock()
	return p, ok
}

func tcpGetServer(key ConnKey) (Proc, bool) {
	tcpMu.Lock()
	p, ok := tcpServerBy[key]
	tcpMu.Unlock()
	return p, ok
}

func tcpSavePendingTuple(key ConnKey, pc PendingConnect) {
	pc.Seen = time.Now()
	tcpPendingMu.Lock()
	tcpPendByTuple[key] = pc
	tcpPendingMu.Unlock()
}

func tcpTakePendingTuple(key ConnKey) (PendingConnect, bool) {
	tcpPendingMu.Lock()
	v, ok := tcpPendByTuple[key]
	if ok {
		delete(tcpPendByTuple, key)
	}
	tcpPendingMu.Unlock()
	return v, ok
}

func tcpSavePendingCookie(cookie uint64, pc PendingConnect) {
	pc.Seen = time.Now()
	tcpPendingMu.Lock()
	tcpPendByCookie[cookie] = pc
	tcpPendingMu.Unlock()
}

func tcpTakePendingCookie(cookie uint64) (PendingConnect, bool) {
	tcpPendingMu.Lock()
	v, ok := tcpPendByCookie[cookie]
	if ok {
		delete(tcpPendByCookie, cookie)
	}
	tcpPendingMu.Unlock()
	return v, ok
}

func tcpPeekPendingCookie(cookie uint64) (PendingConnect, bool) {
	tcpPendingMu.Lock()
	v, ok := tcpPendByCookie[cookie]
	tcpPendingMu.Unlock()
	return v, ok
}

/* ===================== printing ===================== */

func printConnect(p Proc, tid uint32, fd uint32, cookie uint64, ret int64, clientEp, serverEp string, server Proc) {
	srv := server.String()
	fmt.Printf("TCP CONNECT pid=%s tid=%d fd=%d cookie=%#x ret=%d  %s -> %s  server=%s\n",
		p.String(), tid, fd, cookie, ret, clientEp, serverEp, srv)
}

func printAccept(server Proc, tid uint32, fd uint32, cookie uint64, ret int64, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  pid=%s tid=%d fd=%d cookie=%#x ret=%d  %s -> %s  client=%s\n",
		server.String(), tid, fd, cookie, ret, serverEp, clientEp, client.String())
}

func printSendLine(proto, kind string, p Proc, tid uint32, fd uint32, cookie uint64, ret int64, srcEp, dstEp string, dst Proc, ok bool) {
	d := "?"
	if ok {
		d = dst.String()
	}
	fmt.Printf("%s %-7s pid=%s tid=%d fd=%d cookie=%#x ret=%d  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), tid, fd, cookie, ret, p.String(), srcEp, dstEp, d)
}

func printRecvLine(proto, kind string, p Proc, tid uint32, fd uint32, cookie uint64, ret int64, src Proc, ok bool, srcEp, dstEp string) {
	s := "?"
	if ok {
		s = src.String()
	}
	fmt.Printf("%s %-7s pid=%s tid=%d fd=%d cookie=%#x ret=%d  src=%s  %s -> %s  dst=%s\n",
		proto, kind, p.String(), tid, fd, cookie, ret, s, srcEp, dstEp, p.String())
}

func printICMP(proto, kind string, p Proc, tid uint32, fd uint32, cookie uint64, ret int64, srcIPOnly, dstIPOnly string) {
	fmt.Printf("%s %-7s pid=%s tid=%d fd=%d cookie=%#x ret=%d  %s -> %s\n",
		proto, kind, p.String(), tid, fd, cookie, ret, srcIPOnly, dstIPOnly)
}

/* ===================== cleanup (TTL) ===================== */

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

		tcpMu.Lock()
		for k, v := range tcpClientBy {
			if v.Seen.Before(cut) {
				delete(tcpClientBy, k)
			}
		}
		for k, v := range tcpServerBy {
			if v.Seen.Before(cut) {
				delete(tcpServerBy, k)
			}
		}
		tcpMu.Unlock()

		udpMu.Lock()
		for port, v := range udpByPort {
			if v.Seen.Before(cut) {
				delete(udpByPort, port)
			}
		}
		udpMu.Unlock()

		// pending tuple
		tcpPendingMu.Lock()
		for k, v := range tcpPendByTuple {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT pid=%s tid=? fd=%d cookie=%#x ret=%d  %s -> %s  server=? (timeout)\n",
					v.Client.String(), v.Fd, v.Cookie, v.Ret, v.ClientEp, v.ServerEp)
				delete(tcpPendByTuple, k)
			}
		}
		// pending cookie
		for ck, v := range tcpPendByCookie {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT pid=%s tid=? fd=%d cookie=%#x ret=%d  %s -> %s  server=? (timeout)\n",
					v.Client.String(), v.Fd, v.Cookie, v.Ret, v.ClientEp, v.ServerEp)
				delete(tcpPendByCookie, ck)
			}
		}
		tcpPendingMu.Unlock()

		routeMu.Lock()
		now := time.Now()
		for k, v := range routeCache {
			if now.After(v.expire) {
				delete(routeCache, k)
			}
		}
		routeMu.Unlock()
	}
}

/* ===================== /proc UDP owners snapshot ===================== */

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

func buildInodeToProc(selfPID uint32) (map[uint64]Proc, int, int) {
	out := make(map[uint64]Proc, 4096)

	ents, err := os.ReadDir("/proc")
	if err != nil {
		dbg("readdir /proc: %v", err)
		return out, 0, 0
	}

	scanned, skipped := 0, 0

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
			skipped++
			continue
		}

		scanned++
		comm := commFromProc(pid)

		fdDir := fmt.Sprintf("/proc/%d/fd", pid)
		fds, err := os.ReadDir(fdDir)
		if err != nil {
			continue
		}
		for _, fd := range fds {
			linkPath := fdDir + "/" + fd.Name()
			target, err := os.Readlink(linkPath)
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

	return out, scanned, skipped
}

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

	var out []inodePort
	lines := 0

	for sc.Scan() {
		lines++
		line := strings.TrimSpace(sc.Text())
		if line == "" {
			continue
		}
		fields := strings.Fields(line)
		if len(fields) < 10 {
			continue
		}

		local := fields[1]
		state := fields[3]
		if wantState != "" && state != wantState {
			continue
		}

		parts := strings.Split(local, ":")
		if len(parts) != 2 {
			continue
		}

		port64, err := strconv.ParseUint(parts[1], 16, 16)
		if err != nil {
			continue
		}
		inode, err := strconv.ParseUint(fields[9], 10, 64)
		if err != nil || inode == 0 {
			continue
		}

		out = append(out, inodePort{Inode: inode, Port: uint16(port64)})
	}

	dbg("%s: scanned_lines=%d parsed_entries=%d", path, lines, len(out))
	return out
}

func snapshotUDPOwners(tracePort uint16) {
	selfPID := uint32(os.Getpid())

	inode2proc, scanned, skipped := buildInodeToProc(selfPID)
	dbg("snapshotUDPOwners: procs_scanned=%d procs_skipped=%d unique_inodes=%d", scanned, skipped, len(inode2proc))

	udp4 := parseProcNetInodes("/proc/net/udp", "")
	udp6 := parseProcNetInodes("/proc/net/udp6", "")

	owners := make(map[uint16]Proc, 256)

	for _, it := range udp4 {
		if p, ok := inode2proc[it.Inode]; ok {
			owners[it.Port] = p
			if tracePort != 0 && it.Port == tracePort {
				dbg("UDP owner learned (udp4): port=%d proc=%s inode=%d", it.Port, p.String(), it.Inode)
			}
		}
	}
	for _, it := range udp6 {
		if p, ok := inode2proc[it.Inode]; ok {
			if _, exists := owners[it.Port]; !exists {
				owners[it.Port] = p
			}
			if tracePort != 0 && it.Port == tracePort {
				dbg("UDP owner learned (udp6): port=%d proc=%s inode=%d", it.Port, p.String(), it.Inode)
			}
		}
	}

	setUDPOwners(owners)
}

/* ===================== stats ===================== */

var (
	cntAll   uint64
	cntMatch uint64
	cntLost  uint64

	cntUDPSend    uint64
	cntUDPRecv    uint64
	cntUDPSendMsg uint64
	cntUDPRecvMsg uint64
	cntTCPConn    uint64
	cntTCPAcc     uint64
)

/* ===================== TCP peer resolution ===================== */

func tcpPeerForTuple(p Proc, family uint16, srcIP [16]byte, sport uint16, dstIP [16]byte, dport uint16) (peer Proc, ok bool) {
	// role=client (src=client dst=server)
	k1 := ConnKey{Family: family, ClientIP: srcIP, ClientPort: sport, ServerIP: dstIP, ServerPort: dport}
	if c, okc := tcpGetClient(k1); okc && c.Pid == p.Pid {
		if s, oks := tcpGetServer(k1); oks {
			return s, true
		}
		if s2, ok2 := lookupListen(family, dstIP, dport); ok2 {
			return s2, true
		}
		return Proc{}, false
	}

	// role=server (src=server dst=client) => reverse
	k2 := ConnKey{Family: family, ClientIP: dstIP, ClientPort: dport, ServerIP: srcIP, ServerPort: sport}
	if s, oks := tcpGetServer(k2); oks && s.Pid == p.Pid {
		if c, okc := tcpGetClient(k2); okc {
			return c, true
		}
		return Proc{}, false
	}

	if s, ok2 := lookupListen(family, dstIP, dport); ok2 {
		return s, true
	}
	return Proc{}, false
}

/* ===== resolve pending connect when sport becomes known ===== */

func tryResolvePendingConnectByCookie(cookie uint64, p Proc, tid uint32, fd uint32, ret int64, family uint16,
	localIP [16]byte, localPort uint16, remoteIP [16]byte, remotePort uint16,
	localEp, remoteEp string,
) {
	pc, ok := tcpPeekPendingCookie(cookie)
	if !ok {
		return
	}

	// if still no local port => nothing to do
	if localPort == 0 || isAllZero16(remoteIP) || remotePort == 0 {
		return
	}

	key := ConnKey{Family: family, ClientIP: localIP, ClientPort: localPort, ServerIP: remoteIP, ServerPort: remotePort}
	tcpSaveClient(key, p)

	if server, okS := lookupListen(family, remoteIP, remotePort); okS {
		tcpSaveServer(key, server)
		_ , _ = tcpTakePendingCookie(cookie)
		printConnect(p, tid, fd, cookie, pc.Ret, localEp, remoteEp, server)
		return
	}

	// если сервер уже известен по accept/tuple — тоже печатаем
	if server, okSrv := tcpGetServer(key); okSrv {
		_, _ = tcpTakePendingCookie(cookie)
		printConnect(p, tid, fd, cookie, pc.Ret, localEp, remoteEp, server)
		return
	}

	// иначе ждём (TTL распечатает server=?)
	_ = ret
}

/* ===================== main ===================== */

func main() {
	flag.Parse()
	log.SetFlags(log.LstdFlags | log.Lmicroseconds)

	tracePort := uint16(*flgTracePort)
	if *flgSample < 1 {
		*flgSample = 1
	}

	log.Printf("bpfgo start: debug=%v tracePort=%d perfMB=%d print=%v sample=%d matchOnly=%v snapshotEvery=%v ttl=%v stats=%v routeSrc=%v",
		*flgDebug, tracePort, *flgPerfMB, *flgPrint, *flgSample, *flgMatchOnly, *flgSnapshotEvery, *flgTTL, *flgStats, *flgRouteSrc)

	if err := rlimit.RemoveMemlock(); err != nil {
		log.Fatalf("failed to remove memlock: %v", err)
	}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load bpf objects: %v", err)
	}
	defer objs.Close()

	if *flgPprof {
		go func() {
			log.Printf("pprof on http://%s/debug/pprof/", *flgPprofAddr)
			_ = http.ListenAndServe(*flgPprofAddr, nil)
		}()
	}

	go cleanupTTL(*flgTTL)

	// 1) UDP snapshot, чтобы dst не был ? если сервер стартовал раньше bpfgo
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

	if *flgStats > 0 {
		go func() {
			t := time.NewTicker(*flgStats)
			defer t.Stop()
			var lastAll, lastMatch, lastLost uint64
			for range t.C {
				all := atomic.LoadUint64(&cntAll)
				match := atomic.LoadUint64(&cntMatch)
				lost := atomic.LoadUint64(&cntLost)

				epsAll := all - lastAll
				epsMatch := match - lastMatch
				lostDelta := lost - lastLost
				lastAll, lastMatch, lastLost = all, match, lost

				fmt.Printf("[STAT] eps_all=%d eps_match=%d lost=%d (+%d) udp_send=%d udp_recv=%d udp_sendmsg=%d udp_recvmsg=%d tcp_conn=%d tcp_acc=%d tracePort=%d\n",
					epsAll, epsMatch, lost, lostDelta,
					atomic.LoadUint64(&cntUDPSend),
					atomic.LoadUint64(&cntUDPRecv),
					atomic.LoadUint64(&cntUDPSendMsg),
					atomic.LoadUint64(&cntUDPRecvMsg),
					atomic.LoadUint64(&cntTCPConn),
					atomic.LoadUint64(&cntTCPAcc),
					tracePort,
				)
			}
		}()
	}

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
			atomic.AddUint64(&cntLost, uint64(rec.LostSamples))
			continue
		}
		if len(rec.RawSample) < int(unsafe.Sizeof(bpfTraceInfo{})) {
			continue
		}

		atomic.AddUint64(&cntAll, 1)

		ev := *(*bpfTraceInfo)(unsafe.Pointer(&rec.RawSample[0]))
		comm := commString(ev.Comm)
		if comm == selfName {
			continue
		}

		// matchOnly filter
		if tracePort != 0 && *flgMatchOnly {
			if uint16(ev.Sport) != tracePort && uint16(ev.Dport) != tracePort {
				continue
			}
		}
		atomic.AddUint64(&cntMatch, 1)

		p := Proc{Pid: ev.Tgid, Comm: comm}
		tid := ev.Tid
		fd := uint32(ev.Fd)
		ret := int64(ev.Ret)
		cookie := uint64(ev.Cookie)

		family := uint16(ev.Family)
		sport := uint16(ev.Sport)
		dport := uint16(ev.Dport)

		proto := uint8(ev.Proto)
		pr := protoStr(proto)
		evn := evName(ev.Sysexit)

		// endpoints
		var (
			srcEp, dstEp       string
			srcIPKey, dstIPKey [16]byte

			srcIPStr, dstIPStr   string
			srcIPOnly, dstIPOnly string
		)

		if family == AF_INET {
			sIP, sKey, sP := endpFromEvIPv4(ev.SrcIP.S_addr, sport)
			dIP, dKey, dP := endpFromEvIPv4(ev.DstIP.S_addr, dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP
			srcEp = formatEndp(AF_INET, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET, dIP, dKey, dP)
		} else if family == AF_INET6 {
			sIP, sKey, sP := endpFromEvIPv6(ev.SrcIP6.In6U.U6Addr8, sport)
			dIP, dKey, dP := endpFromEvIPv6(ev.DstIP6.In6U.U6Addr8, dport)

			srcIPStr, dstIPStr = sIP, dIP
			srcIPOnly = formatIPOnly(sIP, sKey)
			dstIPOnly = formatIPOnly(dIP, dKey)

			srcIPKey, dstIPKey = sKey, dKey
			sport, dport = sP, dP
			srcEp = formatEndp(AF_INET6, sIP, sKey, sP)
			dstEp = formatEndp(AF_INET6, dIP, dKey, dP)
		} else {
			continue
		}

		// 4) routeSrc: если src wildcard — попробуем вычислить реальный srcIP
		if *flgRouteSrc && isAllZero16(srcIPKey) && !isAllZero16(dstIPKey) {
			if rip, rkey, ok := tryRouteLocalIP(family, dstIPKey, dport); ok {
				srcIPStr = rip
				srcIPKey = rkey
				srcEp = formatEndp(family, srcIPStr, srcIPKey, sport)
				srcIPOnly = formatIPOnly(srcIPStr, srcIPKey)
			}
		}

		// counters
		switch ev.Sysexit {
		case EV_SENDTO:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPSend, 1)
			}
		case EV_RECVFROM:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPRecv, 1)
			}
		case EV_SENDMSG:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPSendMsg, 1)
			}
		case EV_RECVMSG:
			if proto == IPPROTO_UDP {
				atomic.AddUint64(&cntUDPRecvMsg, 1)
			}
		case EV_CONNECT:
			if proto == IPPROTO_TCP {
				atomic.AddUint64(&cntTCPConn, 1)
			}
		case EV_ACCEPT:
			if proto == IPPROTO_TCP {
				atomic.AddUint64(&cntTCPAcc, 1)
			}
		}

		// printing gate
		if !*flgPrint {
			continue
		}
		n := atomic.AddUint64(&printed, 1)
		if n%*flgSample != 0 {
			continue
		}

		switch ev.Sysexit {

		case EV_BINDOK:
			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, p)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, p)

		case EV_CONNECT:
			if proto != IPPROTO_TCP {
				break
			}
			// если sport==0 (EINPROGRESS и порт ещё не назначен) — кладём pending по cookie и ждём первый send/recv
			if sport == 0 || isAllZero16(srcIPKey) {
				pc := PendingConnect{
					Client:   p,
					ClientEp: srcEp,
					ServerEp: dstEp,
					Seen:     time.Now(),
					Fd:       fd,
					Ret:      ret,
					Cookie:   cookie,
					Family:   family,
					SrvIP:    dstIPKey,
					SrvPort:  dport,
				}
				tcpSavePendingCookie(cookie, pc)
				break
			}

			key := ConnKey{Family: family, ClientIP: srcIPKey, ClientPort: sport, ServerIP: dstIPKey, ServerPort: dport}
			tcpSaveClient(key, p)

			if server, ok := lookupListen(family, dstIPKey, dport); ok {
				tcpSaveServer(key, server)
				printConnect(p, tid, fd, cookie, ret, srcEp, dstEp, server)
			} else {
				tcpSavePendingTuple(key, PendingConnect{
					Client:   p,
					ClientEp: srcEp,
					ServerEp: dstEp,
					Seen:     time.Now(),
					Fd:       fd,
					Ret:      ret,
					Cookie:   cookie,
					Family:   family,
					SrvIP:    dstIPKey,
					SrvPort:  dport,
				})
			}

		case EV_ACCEPT:
			if proto != IPPROTO_TCP {
				break
			}
			server := p
			// accept event emitted as src=client dst=server
			key := ConnKey{Family: family, ClientIP: srcIPKey, ClientPort: sport, ServerIP: dstIPKey, ServerPort: dport}
			tcpSaveServer(key, server)

			saveListen(EndpKey{Family: family, Port: dport, IP: dstIPKey}, server)
			var zero [16]byte
			saveListen(EndpKey{Family: family, Port: dport, IP: zero}, server)

			if pc, ok := tcpTakePendingTuple(key); ok {
				printConnect(pc.Client, tid, pc.Fd, pc.Cookie, pc.Ret, pc.ClientEp, pc.ServerEp, server)
				printAccept(server, tid, fd, cookie, ret, pc.ServerEp, pc.ClientEp, pc.Client)
				break
			}

			client, ok := tcpGetClient(key)
			if !ok {
				client = Proc{}
			}
			printAccept(server, tid, fd, cookie, ret, dstEp, srcEp, client)

		case EV_SENDTO, EV_SENDMSG:
			// ICMP
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				printICMP(pr, evn, p, tid, fd, cookie, ret, srcIPOnly, dstIPOnly)
				break
			}

			// UDP learn owner by local port
			if proto == IPPROTO_UDP {
				udpSetOwner(sport, p)
			}

			// TCP: если был pending connect по cookie и теперь появился sport — резолвим
			if proto == IPPROTO_TCP {
				tryResolvePendingConnectByCookie(cookie, p, tid, fd, ret, family,
					srcIPKey, sport, dstIPKey, dport,
					srcEp, dstEp,
				)
			}

			var dstOwner Proc
			var ok bool

			if proto == IPPROTO_UDP && isLocalish(family, dstIPKey) {
				if dstOwner, ok = udpGetOwner(dport); !ok {
					if dstOwner, ok = getUDPOwnerSnapshot(dport); !ok {
						dstOwner, ok = lookupListen(family, dstIPKey, dport)
					}
				}
			} else if proto == IPPROTO_TCP && isLocalish(family, dstIPKey) {
				dstOwner, ok = tcpPeerForTuple(p, family, srcIPKey, sport, dstIPKey, dport)
			}

			printSendLine(pr, evn, p, tid, fd, cookie, ret, srcEp, dstEp, dstOwner, ok)

		case EV_RECVFROM, EV_RECVMSG:
			// ICMP
			if proto == IPPROTO_ICMP || proto == IPPROTO_ICMPV6 {
				printICMP(pr, evn, p, tid, fd, cookie, ret, srcIPOnly, dstIPOnly)
				break
			}

			// UDP learn receiver by local port
			if proto == IPPROTO_UDP {
				udpSetOwner(dport, p)
			}

			// TCP: pending connect может “проявиться” на первом recv тоже
			if proto == IPPROTO_TCP {
				// local is dst for recv
				localEp := formatEndp(family, dstIPStr, dstIPKey, dport)
				remoteEp := formatEndp(family, srcIPStr, srcIPKey, sport)
				tryResolvePendingConnectByCookie(cookie, p, tid, fd, ret, family,
					dstIPKey, dport, srcIPKey, sport,
					localEp, remoteEp,
				)
			}

			var srcOwner Proc
			var ok bool

			if proto == IPPROTO_UDP && isLocalish(family, srcIPKey) {
				if srcOwner, ok = udpGetOwner(sport); !ok {
					if srcOwner, ok = getUDPOwnerSnapshot(sport); !ok {
						srcOwner, ok = lookupListen(family, srcIPKey, sport)
					}
				}
			} else if proto == IPPROTO_TCP && isLocalish(family, srcIPKey) {
				// for recv: local is dst, so reverse tuple when asking peer
				srcOwner, ok = tcpPeerForTuple(p, family, dstIPKey, dport, srcIPKey, sport)
			}

			printRecvLine(pr, evn, p, tid, fd, cookie, ret, srcOwner, ok,
				fmt.Sprintf("%s:%d", srcIPStr, sport),
				fmt.Sprintf("%s:%d", dstIPStr, dport),
			)
		}
	}
}




ev@lev-VirtualBox:~/bpfgo$ sudo ./bpfgo
[sudo] password for lev: 
2026/02/20 03:41:41.595532 bpfgo start: debug=false tracePort=0 perfMB=4 print=true sample=1 matchOnly=false snapshotEvery=0s ttl=3s stats=0s routeSrc=true
2026/02/20 03:41:41.841752 failed to load bpf objects: field TraceAccept4Exit: program trace_accept4_exit: load program: invalid argument: unknown func bpf_get_socket_cookie#46 (256 line(s) omitted)
lev@lev-VirtualBox:~/bpfgo$ 
