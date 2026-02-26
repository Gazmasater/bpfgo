
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



/* ===================== WRITE + TLS SNI (FULL) ===================== */

/* event id */
#ifndef EV_TLS_SNI
#define EV_TLS_SNI  60
#endif

/* limits */
#define TLS_ACC_MAX   2048   /* 1–2KB, как ты хотел */
#define TLS_SNI_MAX    256   /* max hostname bytes incl. \0 */
#define ACC_COPY_STEP   64   /* chunk size for bounded copy */
#define TLS_PARSE_STEPS 64   /* bounded ext scan steps */

/* userspace ABI: write args */
struct write_args_t {
    __s32 fd;
    __u32 _pad;
    __u64 buf;   /* user ptr */
    __u64 cnt;   /* size_t */
};

/* TLS accumulator (cookie -> first bytes of stream) */
struct tls_acc_t {
    __u32 len;
    __u32 _pad;
    __u64 ts_ns;
    __u8  data[TLS_ACC_MAX];
};



/* ---- maps ---- */

/* pid_tgid -> write args */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16384);
    __type(key, __u64);
    __type(value, struct write_args_t);
} write_args_map SEC(".maps");

/* cookie -> tls accumulator */
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 65536);
    __type(key, __u64);
    __type(value, struct tls_acc_t);
} tls_acc_map SEC(".maps");

/* perf events for TLS SNI */
struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} tls_events SEC(".maps");

/* scratch per-cpu to avoid stack overflow */
struct scratch_t {
    struct trace_info    info;
    struct tls_sni_event sni;
};

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, struct scratch_t);
} scratch_map SEC(".maps");

/* zero const (NO volatile) */
const struct tls_acc_t zero_tls_acc = {};

/* ---- small bounded helpers (NO memcpy/memset builtins) ---- */

static __always_inline __u8 rd_u8_acc(const struct tls_acc_t *a, __u32 i)
{
    if (!a || i >= a->len) return 0;
    return a->data[i];
}

static __always_inline __u16 rd_be16_acc(const struct tls_acc_t *a, __u32 i)
{
    __u16 hi = rd_u8_acc(a, i);
    __u16 lo = rd_u8_acc(a, i + 1);
    return (__u16)((hi << 8) | lo);
}

static __always_inline __u32 rd_be24_acc(const struct tls_acc_t *a, __u32 i)
{
    __u32 b0 = rd_u8_acc(a, i);
    __u32 b1 = rd_u8_acc(a, i + 1);
    __u32 b2 = rd_u8_acc(a, i + 2);
    return (b0 << 16) | (b1 << 8) | b2;
}

/* bounded copy from user into accumulator (avoid variable-size probe_read) */
static __always_inline void acc_copy_user(struct tls_acc_t *a, __u64 uptr, __u32 off, __u32 n)
{
    if (!a || !uptr || n == 0) return;
    if (off >= TLS_ACC_MAX) return;
    if (n > TLS_ACC_MAX - off) n = TLS_ACC_MAX - off;

#pragma clang loop unroll(full)
    for (int step = 0; step < (TLS_ACC_MAX / ACC_COPY_STEP); step++) {
        __u32 base = (__u32)step * ACC_COPY_STEP;
        if (base >= n) break;

        __u32 want = n - base;
        if (want > ACC_COPY_STEP) want = ACC_COPY_STEP;

        /* copy fixed 64 bytes, but only "want" is valid;
           still safe because we cap to TLS_ACC_MAX and user ptr range is checked by helper */
        bpf_probe_read_user(&a->data[off + base], want, (void *)(uptr + base));
    }
}

/* parse SNI from TLS ClientHello, write into out[TLS_SNI_MAX] (out must be zeroed) */
static __always_inline int tls_try_parse_sni_into(const struct tls_acc_t *a, char out[TLS_SNI_MAX])
{
    if (!a) return -1;
    if (a->len < 5) return -1;

    /* TLS record header */
    __u8 ct = rd_u8_acc(a, 0);
    if (ct != 22) return -1;              /* handshake */

    __u16 rec_len = rd_be16_acc(a, 3);
    if (rec_len < 4) return -1;
    if (5 + (__u32)rec_len > a->len) return -1;

    /* handshake header */
    __u8 hs_type = rd_u8_acc(a, 5);
    if (hs_type != 1) return -1;          /* ClientHello */

    __u32 hs_len = rd_be24_acc(a, 6);
    if (hs_len < 42) return -1;
    if (9 + hs_len > a->len) return -1;

    __u32 p = 9;

    /* client_version(2) + random(32) */
    p += 2 + 32;
    if (p + 1 > a->len) return -1;

    /* session_id */
    __u8 sid_len = rd_u8_acc(a, p); p += 1;
    p += sid_len;
    if (p + 2 > a->len) return -1;

    /* cipher_suites */
    __u16 cs_len = rd_be16_acc(a, p); p += 2;
    p += cs_len;
    if (p + 1 > a->len) return -1;

    /* compression_methods */
    __u8 comp_len = rd_u8_acc(a, p); p += 1;
    p += comp_len;
    if (p + 2 > a->len) return -1;

    /* extensions */
    __u16 ext_len = rd_be16_acc(a, p); p += 2;
    if (p + ext_len > a->len) return -1;

    __u32 ext_end = p + ext_len;

#pragma clang loop unroll(full)
    for (int step = 0; step < TLS_PARSE_STEPS; step++) {
        if (p + 4 > ext_end) break;

        __u16 et = rd_be16_acc(a, p);
        __u16 el = rd_be16_acc(a, p + 2);
        p += 4;

        if (p + el > ext_end) break;

        /* server_name extension */
        if (et == 0x0000) {
            if (el < 2) return -1;

            __u16 list_len = rd_be16_acc(a, p);
            __u32 q = p + 2;
            __u32 list_end = p + 2 + list_len;
            __u32 hard_end = p + el;
            if (list_end > hard_end) list_end = hard_end;

            if (q + 3 > list_end) return -1;

            __u8  nt = rd_u8_acc(a, q); q += 1;     /* name_type */
            __u16 nl = rd_be16_acc(a, q); q += 2;   /* name_len */

            if (nt != 0) return -1;                 /* host_name */
            if (q + nl > list_end) return -1;

            __u32 copy = nl;
            if (copy > (TLS_SNI_MAX - 1)) copy = TLS_SNI_MAX - 1;

#pragma clang loop unroll(full)
            for (int i = 0; i < TLS_SNI_MAX - 1; i++) {
                if ((__u32)i >= copy) break;
                out[i] = (char)rd_u8_acc(a, q + (__u32)i);
            }
            out[copy] = 0;
            return 0;
        }

        p += el;
    }

    return -1;
}

/* ===================== write enter/exit ===================== */

/* sys_enter_write(fd, buf, count) */
SEC("tracepoint/syscalls/sys_enter_write")
int trace_write_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 id   = bpf_get_current_pid_tgid();
    __u32 tgid = id >> 32;

    int fd = (int)ctx->args[0];

    /* если хочешь строгий фильтр "только сокеты" — оставь.
       если начнёт грузить стек/верификатор — можно заменить на sock_from_fd(fd)!=0 */
    if (!is_socket_fd(fd))
        return 0;

    struct write_args_t wa = {};
    wa.fd  = fd;
    wa.buf = (__u64)ctx->args[1];
    wa.cnt = (__u64)ctx->args[2];
    bpf_map_update_elem(&write_args_map, &id, &wa, BPF_ANY);

    /* conn_info_map: fd + comm (как у тебя везде) */
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

    struct write_args_t *wa = bpf_map_lookup_elem(&write_args_map, &id);
    struct conn_info_t  *ci = bpf_map_lookup_elem(&conn_info_map, &id);
    if (!wa || !ci)
        goto cleanup;

    /* ---- per-cpu scratch: NO big stack frames ---- */
    __u32 z = 0;
    struct scratch_t *sc = bpf_map_lookup_elem(&scratch_map, &z);
    if (!sc)
        goto cleanup;

    struct trace_info *info = &sc->info;

    /* минимальная инициализация без memset */
    info->event = EV_WRITE;
    info->fd    = ci->fd;
    info->ret   = ret;

    info->ts_ns  = bpf_ktime_get_ns();
    info->tgid   = tgid;
    info->tid    = (__u32)id;
    info->cookie = cookie_from_fd((int)ci->fd);

    /* comm: копия без memcpy */
#pragma clang loop unroll(full)
    for (int i = 0; i < 32; i++)
        info->comm[i] = ci->comm[i];

    /* endpoints */
    if (fill_from_fd_state_map(info, tgid, (int)ci->fd, 1) == 0) {
        loopback_fallback(info, 1);
        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, info, sizeof(*info));
    } else {
        goto cleanup;
    }

    /* ---- TLS accumulate + parse SNI (TCP only, port 443) ---- */
    if (info->proto != IPPROTO_TCP || info->cookie == 0)
        goto cleanup;

    if (!(info->dport == 443 || info->sport == 443))
        goto cleanup;

    /* clamp how many bytes we try to take from this write */
    __u32 n = (__u32)ret;
    if (n > TLS_ACC_MAX) n = TLS_ACC_MAX;

    struct tls_acc_t *acc = bpf_map_lookup_elem(&tls_acc_map, &info->cookie);
    if (!acc) {
        bpf_map_update_elem(&tls_acc_map, &info->cookie, &zero_tls_acc, BPF_NOEXIST);
        acc = bpf_map_lookup_elem(&tls_acc_map, &info->cookie);
        if (!acc) goto cleanup;
        acc->len   = 0;
        acc->ts_ns = bpf_ktime_get_ns();
    }

    __u32 off = acc->len;
    if (off < TLS_ACC_MAX) {
        __u32 add = n;
        if (add > (TLS_ACC_MAX - off)) add = TLS_ACC_MAX - off;

        /* bounded user copy */
        acc_copy_user(acc, wa->buf, off, add);
        acc->len = off + add;
    }

    /* try parse when enough bytes */
    if (acc->len >= 200) {
        struct tls_sni_event *sev = &sc->sni;

        sev->ts_ns  = bpf_ktime_get_ns();
        sev->cookie = info->cookie;
        sev->tgid   = tgid;
        sev->tid    = (__u32)id;

        sev->family = info->family;
        sev->proto  = info->proto;
        sev->event  = EV_TLS_SNI;

        sev->sport  = info->sport;
        sev->dport  = info->dport;

        /* copy addrs */
        sev->src_ip4 = info->src_ip4;
        sev->dst_ip4 = info->dst_ip4;

#pragma clang loop unroll(full)
        for (int i = 0; i < 16; i++) {
            sev->src_ip6[i] = info->src_ip6[i];
            sev->dst_ip6[i] = info->dst_ip6[i];
        }

        /* comm */
#pragma clang loop unroll(full)
        for (int i = 0; i < 32; i++)
            sev->comm[i] = info->comm[i];

        /* zero sni without memset */
#pragma clang loop unroll(full)
        for (int i = 0; i < TLS_SNI_MAX; i++)
            sev->sni[i] = 0;

        if (tls_try_parse_sni_into(acc, sev->sni) == 0) {
            bpf_perf_event_output(ctx, &tls_events, BPF_F_CURRENT_CPU, sev, sizeof(*sev));
            bpf_map_delete_elem(&tls_acc_map, &info->cookie);
        } else {
            /* если уже забили весь буфер и не нашли — выкидываем */
            if (acc->len >= TLS_ACC_MAX)
                bpf_map_delete_elem(&tls_acc_map, &info->cookie);
        }
    }

cleanup:
    bpf_map_delete_elem(&write_args_map, &id);
    bpf_map_delete_elem(&conn_info_map, &id);
    return 0;
}



lev@lev-VirtualBox:~/bpfgo$ bpf2go -output-dir . -tags linux -type trace_info -type tls_sni_event  -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)
./trace.c:1673:13: warning: loop not unrolled: the optimizer was unable to perform the requested transformation; the transformation might be disabled or specified as part of an unsupported transformation ordering [-Wpass-failed=transform-warning]
            for (int i = 0; i < TLS_SNI_MAX - 1; i++) {
            ^
1 warning generated.
lev@lev-VirtualBox:~/bpfgo$ 
