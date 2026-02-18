
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



Тут нужно 2 правки в BPF:

Обновлять fd_state_map на каждом успешном send/recv (или хотя бы когда dport/sport==0), чтобы для TCP подтягивался peer (skc_daddr/skc_dport). Сейчас ты читаешь fd_state_map, но если он пустой/устарел — получаешь *:0.

Разрешить port==0 в fill_from_sockaddr_user(), иначе для ICMP/ICMPv6 sendto() всегда с sin_port=0, и ты теряешь IP назначения (получается * -> *).

Ниже — точечный патч.

1) FIX: разрешаем port==0, но IP/Family заполняем

Заменяешь внутри fill_from_sockaddr_user() вот эти куски:

Было (IPv4/IPv6)
__u16 port = bpf_ntohs(sa.sin_port);
if (port == 0)
    return -1;
...
info->dport = port;
info->dstIP.s_addr = sa.sin_addr.s_addr;

Стало (IPv4)
__u16 port = bpf_ntohs(sa.sin_port);

info->family = AF_INET;

if (is_dst) {
    info->dport = port; // может быть 0 (ICMP)
    info->dstIP.s_addr = sa.sin_addr.s_addr;
} else {
    info->sport = port; // может быть 0 (редко, но ок)
    info->srcIP.s_addr = sa.sin_addr.s_addr;
}
return 0;

Стало (IPv6)
__u16 port = bpf_ntohs(sa6.sin6_port);

info->family = AF_INET6;

if (is_dst) {
    info->dport = port; // может быть 0 (ICMPv6)
    __builtin_memcpy(&info->dstIP6, &sa6.sin6_addr, sizeof(info->dstIP6));
} else {
    info->sport = port;
    __builtin_memcpy(&info->srcIP6, &sa6.sin6_addr, sizeof(info->srcIP6));
}
return 0;


👉 Это сразу починит ICMP/ICMPv6 srcIP -> dstIP (у тебя перестанет быть * -> *).

2) FIX: refresh fd_state_map в sendto/recvfrom exit

Суть: после успешного syscall (ret>0) делаем fill_fd_state(fd,&st) и update map, а потом уже берём src/dst из fd_state_map.

trace_sendto_exit — вставка

Прямо после info.fd/info.ret добавь:

int fd = (int)ci->fd;

/* refresh fd_state_map for this fd (important for TCP peer daddr/dport) */
struct fd_state_t st = {};
if (fill_fd_state(fd, &st) == 0) {
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
}


И дальше оставляешь твою логику, но лучше чуть поменять порядок:

/* now read from map (fresh) */
if (fill_from_fd_state_map(&info, tgid, fd, 1) < 0)
    fill_local_src_from_fd(&info, fd);


Полный кусок в контексте:

info.fd  = ci->fd;
info.ret = ret;

int fd = (int)ci->fd;

/* refresh */
struct fd_state_t st = {};
if (fill_fd_state(fd, &st) == 0) {
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
}

/* use fresh map */
if (fill_from_fd_state_map(&info, tgid, fd, 1) < 0)
    fill_local_src_from_fd(&info, fd);


Почему так важно: для TCP sendto() чаще всего без sockaddr, и единственный источник peer — это skc_daddr/skc_dport. Если fd_state_map не обновлять — легко получить dst=* dport=0.

trace_recvfrom_exit — аналогично

После info.fd/info.ret:

int fd = (int)ci->fd;

/* refresh */
struct fd_state_t st = {};
if (fill_fd_state(fd, &st) == 0) {
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    bpf_map_update_elem(&fd_state_map, &k, &st, BPF_ANY);
}

/* use fresh map */
if (fill_from_fd_state_map(&info, tgid, fd, 0) < 0)
    fill_local_dst_from_fd(&info, fd);

Важный userspace фикс (чтобы не было 10.0.2.15:0 на RECV)

Ты раньше хотел “реальный исходящий IP вместо *”. Делай это только на SEND, иначе на RECV ты подменяешь “неизвестный src” на свой IP и остаётся port=0.

Условие: if (isSend && src==* && dst!=*) { route... }

Что получится после этих правок

TCP SENDTO/RECVFROM перестанут массово печатать *:0 (peer подтянется из сокета).

ICMP/ICMPv6 перестанут быть * -> * — IP назначения будет читаться из sockaddr даже при port=0.

“порт 0” останется только там, где он реально нормален (например connect до назначения ephemeral порта, или raw/неподключённые сокеты).

Если хочешь — скинь ещё sendmsg/recvmsg exit (у тебя там тот же паттерн) — туда точно так же надо вставить refresh fd_state_map, и тогда QUIC/UDP443 у Chrome будет ещё чище.




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
        struct sockaddr_in sa = {};

        if (addrlen < sizeof(sa))
            return -1;
        if (bpf_probe_read_user(&sa, sizeof(sa), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa.sin_port);

        info->family = AF_INET;

        if (is_dst) {
            info->dstIP.s_addr = sa.sin_addr.s_addr; // net order
            if (port) info->dport = port;            // НЕ затираем порт нулём
        } else {
            info->srcIP.s_addr = sa.sin_addr.s_addr; // net order
            if (port) info->sport = port;
        }
        return 0;
    }

    if (family == AF_INET6) {
        struct sockaddr_in6 sa6 = {};

        if (addrlen < sizeof(sa6))
            return -1;
        if (bpf_probe_read_user(&sa6, sizeof(sa6), uaddr) < 0)
            return -1;

        __u16 port = bpf_ntohs(sa6.sin6_port);

        info->family = AF_INET6;

        if (is_dst) {
            __builtin_memcpy(&info->dstIP6, &sa6.sin6_addr, sizeof(info->dstIP6));
            if (port) info->dport = port;            // НЕ затираем порт нулём
        } else {
            __builtin_memcpy(&info->srcIP6, &sa6.sin6_addr, sizeof(info->srcIP6));
            if (port) info->sport = port;
        }
        return 0;
    }

    return -1;
}

static __always_inline int fill_from_fd_state_map(struct trace_info *info,
                                                  __u32 tgid,
                                                  int fd,
                                                  int is_send)
{
    struct fd_key_t k = { .tgid = tgid, .fd = fd };
    struct fd_state_t *st = bpf_map_lookup_elem(&fd_state_map, &k);
    if (!st)
        return -1;

    info->proto  = st->proto;
    info->family = st->family;

    if (st->family == AF_INET) {
        if (is_send) {
            info->srcIP.s_addr = st->lip;
            info->dstIP.s_addr = st->rip;

            if (st->lport) info->sport = st->lport; // local
            if (st->rport) info->dport = st->rport; // remote (не затираем 0)
        } else {
            info->srcIP.s_addr = st->rip;
            info->dstIP.s_addr = st->lip;

            if (st->rport) info->sport = st->rport; // remote
            if (st->lport) info->dport = st->lport; // local
        }
        return 0;
    }

    if (st->family == AF_INET6) {
        if (is_send) {
            __builtin_memcpy(&info->srcIP6, &st->lip6, sizeof(info->srcIP6));
            __builtin_memcpy(&info->dstIP6, &st->rip6, sizeof(info->dstIP6));

            if (st->lport) info->sport = st->lport;
            if (st->rport) info->dport = st->rport;
        } else {
            __builtin_memcpy(&info->srcIP6, &st->rip6, sizeof(info->srcIP6));
            __builtin_memcpy(&info->dstIP6, &st->lip6, sizeof(info->dstIP6));

            if (st->rport) info->sport = st->rport;
            if (st->lport) info->dport = st->lport;
        }
        return 0;
    }

    return -1;
}


