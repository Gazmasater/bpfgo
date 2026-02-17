
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

#include "bpf/bpf_helpers.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_core_read.h"
#include "bpf/bpf_endian.h"

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/* ===== constants ===== */

#define AF_INET  2
#define AF_INET6 10

#define EINPROGRESS 115
#define EALREADY    114

#define EV_SENDTO    1
#define EV_RECVFROM  2
#define EV_CONNECT   3
#define EV_ACCEPT    4
#define EV_SENDMSG   11
#define EV_RECVMSG   12
#define EV_BINDOK    20

/* ===== types ===== */

struct conn_info_t {
    __u32 pid;
    __u32 fd;
    char  comm[32];
};

struct fd_key_t {
    __u32 tgid;
    __s32 fd;
};

struct fd_state_t {
    __u16 family;
    __u8  proto;
    __u8  _pad;

    __u16 lport;   // local
    __u16 rport;   // remote

    __u32 lip;     // local IPv4 (network order)
    __u32 rip;     // remote IPv4 (network order)

    __u8  lip6[16];
    __u8  rip6[16];

    __u64 cookie;  // skc_cookie (best-effort)
};

struct inflight_fd_t {
    __s32 fd;
};

struct trace_info {
    // IPv4 in network order
    __u32 src_ip4;
    __u32 dst_ip4;

    // IPv6 bytes
    __u8  src_ip6[16];
    __u8  dst_ip6[16];

    __u32 pid;
    __u32 proto;
    __u16 sport;
    __u16 dport;
    __u16 family;

    __u8  sysexit;
    __u8  state;

    __u32 bytes;      // ret (send/recv)
    __u32 _pad0;

    __u64 ts_ns;      // bpf_ktime_get_ns()
    __u64 sock_cookie;// from sk->__sk_common.skc_cookie (CO-RE)

    char  comm[32];
};

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
    __u64 lenp;   // user socklen_t*  (kernel fills)
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
    __type(key, __u64);   // pid_tgid
    __type(value, struct inflight_fd_t);
} connect_fd_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user pointer to sockaddr
} addrConnect_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user pointer to sockaddr
} addrBind_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_ptrlen_t);
} addrSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, __u64);                 // pid_tgid
    __type(value, struct addr_recv_meta_t);
} addrRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user msghdr*
} msgSend_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, __u64);   // pid_tgid
    __type(value, __u64); // user msghdr*
} msgRecv_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 4096);
    __type(key, __u64); // pid_tgid
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");

/* ===== helpers ===== */

static __always_inline int read_sys_exit_ret(struct trace_event_raw_sys_exit *ctx, __s64 *ret)
{
    return BPF_CORE_READ_INTO(ret, ctx, ret);
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

        __u16 port = bpf_ntohs(sa.sin_port);
        if (port == 0)
            return -1;

        info->family = AF_INET;

        if (is_dst) {
            info->dport   = port;
            info->dst_ip4 = sa.sin_addr.s_addr; // network order
        } else {
            info->sport   = port;
            info->src_ip4 = sa.sin_addr.s_addr; // network order
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
        if (port == 0)
            return -1;

        info->family = AF_INET6;

        if (is_dst) {
            info->dport = port;
            __builtin_memcpy(info->dst_ip6, &sa6.sin6_addr, 16);
        } else {
            info->sport = port;
            __builtin_memcpy(info->src_ip6, &sa6.sin6_addr, 16);
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

static __always_inline __u64 read_sock_cookie(struct sock *sk)
{
    __u64 cookie = 0;
    if (!sk)
        return 0;

    // skc_cookie может быть atomic64_t — читаем в __u64 через CO-RE
    BPF_CORE_READ_INTO(&cookie, sk, __sk_common.skc_cookie);
    return cookie;
}

static __always_inline int fill_fd_state(int fd, struct fd_state_t *st)
{
    struct sock *sk = sock_from_fd(fd);
    if (!sk)
        return -1;

    st->family = BPF_CORE_READ(sk, __sk_common.skc_family);
    st->proto  = BPF_CORE_READ(sk, sk_protocol);

    st->cookie = read_sock_cookie(sk);

    st->lport  = BPF_CORE_READ(sk, __sk_common.skc_num);

    __u16 dport_be = BPF_CORE_READ(sk, __sk_common.skc_dport);
    st->rport = bpf_ntohs(dport_be);

    if (st->family == AF_INET) {
        st->lip = BPF_CORE_READ(sk, __sk_common.skc_rcv_saddr);
        st->rip = BPF_CORE_READ(sk, __sk_common.skc_daddr);
        return 0;
    }

    if (st->family == AF_INET6) {
        struct in6_addr a = {};
        struct in6_addr b = {};
        if (BPF_CORE_READ_INTO(&a, sk, __sk_common.skc_v6_rcv_saddr) < 0)
            return -1;
        if (BPF_CORE_READ_INTO(&b, sk, __sk_common.skc_v6_daddr) < 0)
            return -1;
        __builtin_memcpy(st->lip6, &a, 16);
        __builtin_memcpy(st->rip6, &b, 16);
        return 0;
    }

    return -1;
}

static __always_inline int fill_from_fd_state_map(struct trace_info *info, __u32 tgid, int fd, int is_send)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t *st = bpf_map_lookup_elem(&fd_state_map, &k);
    if (!st)
        return -1;

    info->proto       = st->proto;
    info->family      = st->family;
    info->sock_cookie = st->cookie;

    if (st->family == AF_INET) {
        if (is_send) {
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
        if (is_send) {
            __builtin_memcpy(info->src_ip6, st->lip6, 16);
            __builtin_memcpy(info->dst_ip6, st->rip6, 16);
            info->sport = st->lport;
            info->dport = st->rport;
        } else {
            __builtin_memcpy(info->src_ip6, st->rip6, 16);
            __builtin_memcpy(info->dst_ip6, st->lip6, 16);
            info->sport = st->rport;
            info->dport = st->lport;
        }
        return 0;
    }

    return -1;
}

static __always_inline void fill_local_src_from_fd(struct trace_info *info, int fd)
{
    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) < 0)
        return;

    info->proto       = st.proto;
    info->family      = st.family;
    info->sock_cookie = st.cookie;

    if (st.family == AF_INET) {
        info->src_ip4 = st.lip;
        info->sport   = st.lport;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info->src_ip6, st.lip6, 16);
        info->sport = st.lport;
    }
}

static __always_inline void fill_local_dst_from_fd(struct trace_info *info, int fd)
{
    struct fd_state_t st = {};
    if (fill_fd_state(fd, &st) < 0)
        return;

    info->proto       = st.proto;
    info->family      = st.family;
    info->sock_cookie = st.cookie;

    if (st.family == AF_INET) {
        info->dst_ip4 = st.lip;
        info->dport   = st.lport;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info->dst_ip6, st.lip6, 16);
        info->dport = st.lport;
    }
}

/* ===== connect ===== */

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    struct inflight_fd_t in = {};
    in.fd = (int)ctx->args[0];
    bpf_map_update_elem(&connect_fd_map, &id, &in, BPF_ANY);

    __u64 uaddr = (__u64)ctx->args[1];
    if (uaddr)
        bpf_map_update_elem(&addrConnect_map, &id, &uaddr, BPF_ANY);

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

    struct trace_info info = {};
    info.sysexit     = EV_CONNECT;
    info.pid         = tgid;
    info.proto       = st.proto;
    info.family      = st.family;
    info.sport       = st.lport;
    info.dport       = st.rport;
    info.state       = (ret < 0) ? 1 : 0;
    info.ts_ns       = bpf_ktime_get_ns();
    info.bytes       = 0;
    info.sock_cookie = st.cookie;

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm(info.comm, sizeof(info.comm));

    if (st.family == AF_INET) {
        info.src_ip4 = st.lip;
        info.dst_ip4 = st.rip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, st.lip6, 16);
        __builtin_memcpy(info.dst_ip6, st.rip6, 16);
    } else {
        goto cleanup;
    }

    __u64 *uaddrp = bpf_map_lookup_elem(&addrConnect_map, &id);
    if (uaddrp && *uaddrp)
        (void)fill_from_sockaddr_user(&info, (void *)*uaddrp, 128 /*enough*/, 1);

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&addrConnect_map, &id);
    bpf_map_delete_elem(&connect_fd_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

/* ===== accept / accept4 ===== */

static __always_inline int accept_enter_common(void)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
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

    struct trace_info info = {};
    info.sysexit     = EV_ACCEPT;
    info.pid         = tgid;
    info.proto       = st.proto;
    info.family      = st.family;
    info.ts_ns       = bpf_ktime_get_ns();
    info.bytes       = 0;
    info.sock_cookie = st.cookie;

    // incoming: src=remote, dst=local
    info.sport = st.rport;
    info.dport = st.lport;

    if (conn)
        __builtin_memcpy(info.comm, conn->comm, sizeof(info.comm));
    else
        bpf_get_current_comm(info.comm, sizeof(info.comm));

    if (st.family == AF_INET) {
        info.src_ip4 = st.rip;
        info.dst_ip4 = st.lip;
    } else if (st.family == AF_INET6) {
        __builtin_memcpy(info.src_ip6, st.rip6, 16);
        __builtin_memcpy(info.dst_ip6, st.lip6, 16);
    } else {
        goto cleanup;
    }

    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &info, sizeof(info));

cleanup:
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct trace_event_raw_sys_enter *ctx)
{
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
    return accept_enter_common();
}

SEC("tracepoint/syscalls/sys_exit_accept")
int trace_accept_exit(struct trace_event_raw_sys_exit *ctx)
{
    return accept_exit_common(ctx);
}

/* ===== close ===== */

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;
    int fd     = (int)ctx->args[0];

    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_delete_elem(&fd_state_map, &k);
    return 0;
}

/* ===== bind ===== */

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr = (__u64)ctx->args[1];
    if (uaddr)
        bpf_map_update_elem(&addrBind_map, &id, &uaddr, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();

    __s64 ret = 0;
    if (read_sys_exit_ret(ctx, &ret) < 0 || ret < 0)
        goto cleanup;

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    __u64 *uaddrp = bpf_map_lookup_elem(&addrBind_map, &id);
    if (!uaddrp || !*uaddrp)
        goto cleanup;

    struct trace_info info = {};
    info.sysexit = EV_BINDOK;
    info.pid     = ci->pid;
    info.ts_ns   = bpf_ktime_get_ns();
    info.bytes   = 0;

    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));

    // cookie/proto best-effort from fd
    struct fd_state_t st = {};
    if (fill_fd_state((int)ci->fd, &st) == 0) {
        info.proto       = st.proto;
        info.sock_cookie = st.cookie;
    }

    // bind: заполняем dst как локальный endpoint
    if (fill_from_sockaddr_user(&info, (void *)*uaddrp, 128 /*enough*/, 1) < 0)
        goto cleanup;

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
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr   = (__u64)ctx->args[4];
    __u32 addrlen = (__u32)ctx->args[5];

    if (uaddr && addrlen >= sizeof(__u16)) {
        struct addr_ptrlen_t v = {};
        v.addr = uaddr;
        v.len  = addrlen;
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
    info.sysexit = EV_SENDTO;
    info.pid     = ci->pid;
    info.ts_ns   = bpf_ktime_get_ns();
    info.bytes   = (__u32)ret;

    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        fill_local_src_from_fd(&info, (int)ci->fd);

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

/* ===== recvfrom ===== */

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
    bpf_map_update_elem(&conn_info_map, &id, &ci, BPF_ANY);

    __u64 uaddr  = (__u64)ctx->args[4];
    __u64 lenp_u = (__u64)ctx->args[5];

    if (uaddr && lenp_u) {
        struct addr_recv_meta_t m = {};
        m.addr = uaddr;
        m.lenp = lenp_u;
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
    info.sysexit = EV_RECVFROM;
    info.pid     = ci->pid;
    info.ts_ns   = bpf_ktime_get_ns();
    info.bytes   = (__u32)ret;

    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        fill_local_dst_from_fd(&info, (int)ci->fd);

    struct addr_recv_meta_t *m = bpf_map_lookup_elem(&addrRecv_map, &id);
    if (m && m->addr && m->lenp) {
        __u32 addrlen = 0;
        if (bpf_probe_read_user(&addrlen, sizeof(addrlen), (void *)m->lenp) == 0) {
            if (addrlen >= sizeof(__u16)) {
                (void)fill_from_sockaddr_user(&info, (void *)m->addr, addrlen, 0);
            }
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

/* ===== sendmsg ===== */

SEC("tracepoint/syscalls/sys_enter_sendmsg")
int trace_sendmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
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

    struct conn_info_t *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!ci)
        goto cleanup;

    struct trace_info info = {};
    info.sysexit = EV_SENDMSG;
    info.pid     = ci->pid;
    info.ts_ns   = bpf_ktime_get_ns();
    info.bytes   = (__u32)ret;

    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 1) < 0)
        fill_local_src_from_fd(&info, (int)ci->fd);

    __u64 *msgp = bpf_map_lookup_elem(&msgSend_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (bpf_probe_read_user(&h, sizeof(h), (void *)*msgp) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16)) {
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 1);
            }
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

/* ===== recvmsg ===== */

SEC("tracepoint/syscalls/sys_enter_recvmsg")
int trace_recvmsg_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    struct conn_info_t ci = {};
    ci.pid = tgid;
    ci.fd  = (__u32)ctx->args[0];
    bpf_get_current_comm(&ci.comm, sizeof(ci.comm));
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
    info.sysexit = EV_RECVMSG;
    info.pid     = ci->pid;
    info.ts_ns   = bpf_ktime_get_ns();
    info.bytes   = (__u32)ret;

    __builtin_memcpy(info.comm, ci->comm, sizeof(info.comm));

    if (fill_from_fd_state_map(&info, tgid, (int)ci->fd, 0) < 0)
        fill_local_dst_from_fd(&info, (int)ci->fd);

    __u64 *msgp = bpf_map_lookup_elem(&msgRecv_map, &id);
    if (msgp && *msgp) {
        struct user_msghdr_head h = {};
        if (bpf_probe_read_user(&h, sizeof(h), (void *)*msgp) == 0) {
            if (h.msg_name && h.msg_namelen >= sizeof(__u16)) {
                (void)fill_from_sockaddr_user(&info, h.msg_name, h.msg_namelen, 0);
            }
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

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf ./trace.c -- -I.

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
	EV_SENDMSG  = 11
	EV_RECVMSG  = 12
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

type FlowKey struct {
	Family uint16
	SrcIP  [16]byte
	SrcP   uint16
	DstIP  [16]byte
	DstP   uint16
	Bytes  uint32
}

type UDPSend struct {
	Src  Proc
	Seen time.Time
	TsNs uint64
}

var (
	commMu    sync.RWMutex
	commCache = make(map[[32]int8]string)

	listenMu sync.Mutex
	listenBy = make(map[EndpKey]Proc, 4096) // bind() owner

	connMu sync.Mutex
	connBy = make(map[ConnKey]Proc, 16384)

	pendMu sync.Mutex
	pendBy = make(map[ConnKey]PendingConnect, 16384)

	udpMu    sync.Mutex
	udpOwner = make(map[EndpKey]Proc, 16384)  // ephemeral owners (learned from send/recv)
	udpSend  = make(map[FlowKey]UDPSend, 65536) // recent send->recv matcher
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

func protoStr(p uint32) string {
	switch uint8(p) {
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
	// kernel gives network-order bytes, read into uint32 on little-endian => reverse.
	// restore raw bytes as they were in memory:
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
	// simple (not compressed) representation
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

func udpSaveOwner(ep EndpKey, p Proc) {
	p.Seen = time.Now()
	udpMu.Lock()
	udpOwner[ep] = p
	udpMu.Unlock()
}

func udpLookupOwner(ep EndpKey) (Proc, bool) {
	udpMu.Lock()
	p, ok := udpOwner[ep]
	udpMu.Unlock()
	return p, ok
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

func udpRememberSend(fk FlowKey, src Proc, tsNs uint64) {
	udpMu.Lock()
	udpSend[fk] = UDPSend{Src: src, Seen: time.Now(), TsNs: tsNs}
	udpMu.Unlock()
}

func udpMatchSend(fk FlowKey, nowTs uint64) (Proc, bool) {
	udpMu.Lock()
	s, ok := udpSend[fk]
	udpMu.Unlock()
	if !ok {
		return Proc{}, false
	}
	// окно: 50ms по ts (наносек)
	if nowTs >= s.TsNs && nowTs-s.TsNs <= uint64(50*time.Millisecond) {
		return s.Src, true
	}
	return Proc{}, false
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

		pendMu.Lock()
		for k, v := range pendBy {
			if v.Seen.Before(cut) {
				fmt.Printf("TCP CONNECT client=%s  %s -> %s  server=?\n",
					v.Client.String(), v.ClientEp, v.ServerEp)
				delete(pendBy, k)
			}
		}
		pendMu.Unlock()

		udpMu.Lock()
		for k, v := range udpOwner {
			if v.Seen.Before(cut) {
				delete(udpOwner, k)
			}
		}
		for k, v := range udpSend {
			if v.Seen.Before(cut) {
				delete(udpSend, k)
			}
		}
		udpMu.Unlock()
	}
}

/* ===== prints ===== */

func printTCPConnect(ev *bpfTraceInfo, client Proc, clientEp, serverEp string, server Proc) {
	fmt.Printf("TCP CONNECT bytes=%d ts=%d cookie=%x  client=%s  %s -> %s  server=%s\n",
		ev.Bytes, ev.TsNs, ev.SockCookie,
		client.String(), clientEp, serverEp, server.String())
}

func printTCPAccept(ev *bpfTraceInfo, server Proc, serverEp, clientEp string, client Proc) {
	fmt.Printf("TCP ACCEPT  bytes=%d ts=%d cookie=%x  server=%s  %s -> %s  client=%s\n",
		ev.Bytes, ev.TsNs, ev.SockCookie,
		server.String(), serverEp, clientEp, client.String())
}

func printUDP(ev *bpfTraceInfo, kind string, srcProc, dstProc Proc, srcEp, dstEp string) {
	fmt.Printf("UDP %-7s bytes=%d ts=%d cookie=%x  src=%s  %s -> %s  dst=%s\n",
		kind, ev.Bytes, ev.TsNs, ev.SockCookie,
		srcProc.String(), srcEp, dstEp, dstProc.String())
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

			switch ev.Family {
			case AF_INET:
				sIP, sKey, sP := endpFromEvIPv4(ev.SrcIp4, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv4(ev.DstIp4, ev.Dport)
				srcEp = formatEndp(AF_INET, sIP, sP)
				dstEp = formatEndp(AF_INET, dIP, dP)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
			case AF_INET6:
				sIP, sKey, sP := endpFromEvIPv6(ev.SrcIp6, ev.Sport)
				dIP, dKey, dP := endpFromEvIPv6(ev.DstIp6, ev.Dport)
				srcEp = formatEndp(AF_INET6, sIP, sP)
				dstEp = formatEndp(AF_INET6, dIP, dP)
				srcIPKey, dstIPKey = sKey, dKey
				sport, dport = sP, dP
			default:
				continue
			}

			switch ev.Sysexit {

			case EV_BINDOK:
				// bind: сохраняем "кто владеет портом" (и TCP и UDP)
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: dstIPKey}, p)
				var zero [16]byte
				saveListen(EndpKey{Family: uint16(ev.Family), Port: dport, IP: zero}, p)

			case EV_CONNECT:
				if uint8(ev.Proto) != IPPROTO_TCP {
					continue
				}
				key := ConnKey{
					Family:     uint16(ev.Family),
					ClientIP:   srcIPKey,
					ClientPort: sport,
					ServerIP:   dstIPKey,
					ServerPort: dport,
				}
				saveConn(key, p)

				if server, ok := lookupListen(uint16(ev.Family), dstIPKey, dport); ok {
					printTCPConnect(&ev, p, srcEp, dstEp, server)
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
					Family:     uint16(ev.Family),
					ClientIP:   srcIPKey,
					ClientPort: sport,
					ServerIP:   dstIPKey,
					ServerPort: dport,
				}

				if pc, ok := takePending(key); ok {
					printTCPConnect(&ev, pc.Client, pc.ClientEp, pc.ServerEp, server)
					printTCPAccept(&ev, server, pc.ServerEp, pc.ClientEp, pc.Client)
					continue
				}

				client, ok := lookupConn(key)
				if !ok {
					client = Proc{}
				}
				printTCPAccept(&ev, server, dstEp, srcEp, client)

			case EV_SENDTO, EV_SENDMSG, EV_RECVFROM, EV_RECVMSG:
				if uint8(ev.Proto) != IPPROTO_UDP {
					continue
				}

				srcKey := EndpKey{Family: uint16(ev.Family), Port: sport, IP: srcIPKey}
				dstKey := EndpKey{Family: uint16(ev.Family), Port: dport, IP: dstIPKey}

				isSend := ev.Sysexit == EV_SENDTO || ev.Sysexit == EV_SENDMSG

				// 1) обновляем "владельца" endpoint'а (клиентский ephemeral)
				if isSend {
					udpSaveOwner(srcKey, p)
				} else {
					udpSaveOwner(dstKey, p)
				}

				// 2) send->recv “склейка” (одна машина): запоминаем send по 5-tuple+bytes
				fk := FlowKey{
					Family: uint16(ev.Family),
					SrcIP:  srcIPKey, SrcP: sport,
					DstIP:  dstIPKey, DstP: dport,
					Bytes:  ev.Bytes,
				}
				if isSend {
					udpRememberSend(fk, p, ev.TsNs)
				}

				// 3) определяем src/dst процессы для печати
				srcProc := Proc{}
				dstProc := Proc{}

				if isSend {
					srcProc = p
					// dst: сначала bind-слушатель, потом owner по endpoint
					if d, ok := lookupListen(uint16(ev.Family), dstIPKey, dport); ok {
						dstProc = d
					} else if d, ok := udpLookupOwner(dstKey); ok {
						dstProc = d
					}
				} else {
					dstProc = p

					// src: сначала матч по недавнему SEND (точнее для localhost)
					if s, ok := udpMatchSend(fk, ev.TsNs); ok {
						srcProc = s
					} else if s, ok := udpLookupOwner(srcKey); ok {
						srcProc = s
					}
				}

				printUDP(&ev, evName(ev.Sysexit), srcProc, dstProc, srcEp, dstEp)

			default:
				_ = protoStr(ev.Proto)
			}
		}
	}()

	fmt.Println("Press Ctrl+C to exit")
	<-stop
	fmt.Println("Exiting...")
}




[{
	"resource": "/home/lev/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "MissingFieldOrMethod",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "MissingFieldOrMethod"
		}
	},
	"severity": 8,
	"message": "ev.SrcIp4 undefined (type bpfTraceInfo has no field or method SrcIp4)",
	"source": "compiler",
	"startLineNumber": 474,
	"startColumn": 40,
	"endLineNumber": 474,
	"endColumn": 46,
	"modelVersionId": 6,
	"origin": "extHost1"
}]


[{
	"resource": "/home/lev/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "MissingFieldOrMethod",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "MissingFieldOrMethod"
		}
	},
	"severity": 8,
	"message": "ev.DstIp4 undefined (type bpfTraceInfo has no field or method DstIp4)",
	"source": "compiler",
	"startLineNumber": 475,
	"startColumn": 40,
	"endLineNumber": 475,
	"endColumn": 46,
	"modelVersionId": 6,
	"origin": "extHost1"
}]


[{
	"resource": "/home/lev/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "MissingFieldOrMethod",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "MissingFieldOrMethod"
		}
	},
	"severity": 8,
	"message": "ev.SrcIp6 undefined (type bpfTraceInfo has no field or method SrcIp6, but does have field SrcIP6)",
	"source": "compiler",
	"startLineNumber": 481,
	"startColumn": 40,
	"endLineNumber": 481,
	"endColumn": 46,
	"modelVersionId": 6,
	"origin": "extHost1"
}]

[{
	"resource": "/home/lev/bpfgo/main.go",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "MissingFieldOrMethod",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "MissingFieldOrMethod"
		}
	},
	"severity": 8,
	"message": "ev.DstIp6 undefined (type bpfTraceInfo has no field or method DstIp6, but does have field DstIP6)",
	"source": "compiler",
	"startLineNumber": 482,
	"startColumn": 40,
	"endLineNumber": 482,
	"endColumn": 46,
	"modelVersionId": 6,
	"origin": "extHost1"
}]
