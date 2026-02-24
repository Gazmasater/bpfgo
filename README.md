
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

sudo ./bpfgo -resolve=false | stdbuf -oL egrep --line-buffered 'udp_mmsg|python3|nc|mmsg'

gcc -O2 -Wall -o udp_pktinfo_server udp_pktinfo_server.c
./udp_pktinfo_server

echo -n "ping" | nc -u -w1 10.0.2.15 9999


#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *m) { perror(m); exit(1); }

static void ip6_to_str(const struct in6_addr *a, char *out, size_t n) {
    if (!inet_ntop(AF_INET6, a, out, n)) strncpy(out, "?", n);
}

int main(int argc, char **argv) {
    int port = 9997;
    if (argc > 1) port = atoi(argv[1]);

    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) die("socket");

    int one = 1;
    // получать IPV6_PKTINFO на recvmsg
    if (setsockopt(s, IPPROTO_IPV6, IPV6_RECVPKTINFO, &one, sizeof(one)) < 0)
        die("setsockopt(IPV6_RECVPKTINFO)");

    struct sockaddr_in6 sa = {0};
    sa.sin6_family = AF_INET6;
    sa.sin6_addr = in6addr_any;      // bind ::
    sa.sin6_port = htons((uint16_t)port);

    if (bind(s, (struct sockaddr*)&sa, sizeof(sa)) < 0) die("bind");

    fprintf(stderr, "udp6_pktinfo_server listening on [::]:%d\n", port);

    for (;;) {
        char buf[2048];
        struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};

        char cbuf[CMSG_SPACE(sizeof(struct in6_pktinfo))];
        memset(cbuf, 0, sizeof(cbuf));

        struct sockaddr_in6 peer = {0};
        struct msghdr msg = {0};
        msg.msg_name = &peer;
        msg.msg_namelen = sizeof(peer);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cbuf;
        msg.msg_controllen = sizeof(cbuf);

        ssize_t n = recvmsg(s, &msg, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            die("recvmsg");
        }

        // Extract local dst + ifindex from IPV6_PKTINFO
        struct in6_addr local = {0};
        unsigned ifidx = 0;

        for (struct cmsghdr *c = CMSG_FIRSTHDR(&msg); c; c = CMSG_NXTHDR(&msg, c)) {
            if (c->cmsg_level == IPPROTO_IPV6 && c->cmsg_type == IPV6_PKTINFO) {
                struct in6_pktinfo *pi = (struct in6_pktinfo*)CMSG_DATA(c);
                local = pi->ipi6_addr;
                ifidx = pi->ipi6_ifindex;
                break;
            }
        }

        char peer_ip[128], local_ip[128];
        ip6_to_str(&peer.sin6_addr, peer_ip, sizeof(peer_ip));
        ip6_to_str(&local, local_ip, sizeof(local_ip));

        fprintf(stderr, "recv %zdB from [%s]:%d local_dst=[%s] ifindex=%u\n",
                n, peer_ip, ntohs(peer.sin6_port), local_ip, ifidx);

        // echo back with sendmsg + IPV6_PKTINFO to set source and ifindex
        struct msghdr sm = {0};
        struct iovec siov = {.iov_base = buf, .iov_len = (size_t)n};

        char scbuf[CMSG_SPACE(sizeof(struct in6_pktinfo))];
        memset(scbuf, 0, sizeof(scbuf));

        sm.msg_name = &peer;
        sm.msg_namelen = sizeof(peer);
        sm.msg_iov = &siov;
        sm.msg_iovlen = 1;
        sm.msg_control = scbuf;
        sm.msg_controllen = sizeof(scbuf);

        struct cmsghdr *sc = CMSG_FIRSTHDR(&sm);
        sc->cmsg_level = IPPROTO_IPV6;
        sc->cmsg_type  = IPV6_PKTINFO;
        sc->cmsg_len   = CMSG_LEN(sizeof(struct in6_pktinfo));

        struct in6_pktinfo *spi = (struct in6_pktinfo*)CMSG_DATA(sc);
        memset(spi, 0, sizeof(*spi));
        spi->ipi6_addr = local;   // хотим такой src
        spi->ipi6_ifindex = ifidx; // важно для link-local

        ssize_t sn = sendmsg(s, &sm, 0);
        if (sn < 0) die("sendmsg");
    }
}



#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void die(const char *m) { perror(m); exit(1); }

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s <dst_ipv6%%if> <port> <msg>\n", argv[0]);
        fprintf(stderr, "example: %s fe80::1234%%enp0s3 9997 ping\n", argv[0]);
        return 2;
    }
    const char *dst = argv[1];
    int port = atoi(argv[2]);
    const char *msg = argv[3];

    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) die("socket");

    struct sockaddr_in6 to = {0};
    to.sin6_family = AF_INET6;
    to.sin6_port = htons((uint16_t)port);

    // inet_pton не понимает %iface, поэтому режем scope вручную
    char addrbuf[256];
    strncpy(addrbuf, dst, sizeof(addrbuf)-1);
    addrbuf[sizeof(addrbuf)-1] = 0;

    char *pct = strchr(addrbuf, '%');
    if (pct) *pct = 0;

    if (inet_pton(AF_INET6, addrbuf, &to.sin6_addr) != 1) {
        fprintf(stderr, "bad ipv6: %s\n", addrbuf);
        return 2;
    }

    // scope id обязателен для fe80::
    if (strchr(dst, '%')) {
        const char *scope = strchr(dst, '%') + 1;
        unsigned ifidx = if_nametoindex(scope);
        if (ifidx == 0) {
            fprintf(stderr, "bad iface in scope: %s\n", scope);
            return 2;
        }
        to.sin6_scope_id = ifidx;
    }

    if (sendto(s, msg, strlen(msg), 0, (struct sockaddr*)&to, sizeof(to)) < 0)
        die("sendto");

    char buf[2048];
    struct sockaddr_in6 from = {0};
    socklen_t fl = sizeof(from);
    ssize_t n = recvfrom(s, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &fl);
    if (n < 0) die("recvfrom");
    buf[n] = 0;

    char from_ip[128];
    if (!inet_ntop(AF_INET6, &from.sin6_addr, from_ip, sizeof(from_ip)))
        strncpy(from_ip, "?", sizeof(from_ip));

    printf("reply %zdB from [%s]:%d: %s\n", n, from_ip, ntohs(from.sin6_port), buf);
    close(s);
    return 0;
}


sudo ./bpfgo -resolve=false | stdbuf -oL egrep --line-buffered 'udp6_pktinfo|OPEN|CLOSE|ICMPv6|UDP'


ip -6 addr show dev enp0s3 scope link



lev@lev-VirtualBox:~/bpfgo$ sudo ./bpfgo
2026/02/24 04:20:01 dotenv loaded: .env
2026/02/24 04:20:01.363692 hostsPrefill: added=7 from /etc/hosts
2026/02/24 04:20:01.871953 pprof on :6060
2026/02/24 04:20:01.873130 perf ring per-cpu=512KB total~=2MB cpus=4
2026/02/24 04:20:01.878339 OPEN/CLOSE (TCP/UDP/ICMP) + PTR + skb-hint. Ctrl+C to exit
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=70772  [fd00:0:0:0:63c4:dda4:4be2:b7c7](miss):57907 -> [2001:4860:4860:0:0:0:0:8888](miss):443
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=70773  127.0.0.1(localhost):27382 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):27382
OPEN  UDP   pid=534(systemd-resolve) cookie=68156  10.0.2.15(miss):39105 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=70774  127.0.0.1(localhost):19672 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=70775  127.0.0.1(localhost):57687 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33811
OPEN  UDP   pid=534(systemd-resolve) cookie=70071  10.0.2.15(pending):39193 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):19672
OPEN  UDP   pid=534(systemd-resolve) cookie=70072  10.0.2.15(pending):49220 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48861
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51953
OPEN  UDP   pid=534(systemd-resolve) cookie=70073  10.0.2.15(pending):47457 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57687
OPEN  UDP   pid=534(systemd-resolve) cookie=70074  10.0.2.15(pending):36286 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=70071  10.0.2.15(pending):39193 -> 10.0.2.3(vboxdns):53  out=191B/2p in=362B/2p  age=17ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=70072  10.0.2.15(pending):49220 -> 10.0.2.3(vboxdns):53  out=48B/1p in=217B/1p  age=48ms reason=close()
CLOSE UDP   pid=2831(Chrome_ChildIOT) cookie=70774  127.0.0.1(localhost):19672 -> 127.0.0.53(dnsstub):53  out=37B/1p in=134B/1p  age=56ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=68156  10.0.2.15(pending):39105 -> 10.0.2.3(vboxdns):53  out=48B/1p in=168B/1p  age=58ms reason=close()
OPEN  UDP   pid=534(systemd-resolve) cookie=70075  10.0.2.15(pending):46890 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=70074  10.0.2.15(pending):36286 -> 10.0.2.3(vboxdns):53  out=48B/1p in=419B/1p  age=47ms reason=close()
CLOSE UDP   pid=2831(Chrome_ChildIOT) cookie=70775  127.0.0.1(localhost):57687 -> 127.0.0.53(dnsstub):53  out=37B/1p in=200B/1p  age=61ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=70073  10.0.2.15(pending):47457 -> 10.0.2.3(vboxdns):53  out=91B/2p in=91B/2p  age=62ms reason=close()
CLOSE UDP   pid=2831(Chrome_ChildIOT) cookie=70773  127.0.0.1(localhost):27382 -> 127.0.0.53(dnsstub):53  out=37B/1p in=0B/0p  age=79ms reason=close()
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=68159  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):49411 -> [2603:1061:14:32:0:0:0:1](miss):80
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=68160  10.0.2.15(lev-VirtualBox):54280 -> 13.107.253.44(miss):80
OPEN  UDP   pid=2831(Chrome_ChildIOT) cookie=68161  10.0.2.15(lev-VirtualBox):38601 -> 13.107.226.44(miss):80
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58973
OPEN  UDP   pid=534(systemd-resolve) cookie=68163  10.0.2.15(lev-VirtualBox):55379 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47577
OPEN  UDP   pid=534(systemd-resolve) cookie=68164  10.0.2.15(lev-VirtualBox):38723 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42871
OPEN  UDP   pid=534(systemd-resolve) cookie=68165  10.0.2.15(lev-VirtualBox):52453 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=2831(Chrome_ChildIOT) cookie=70076  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):36740 -> [2603:1061:14:32:0:0:0:1](pending):443
CLOSE UDP   pid=534(systemd-resolve) cookie=68165  10.0.2.15(lev-VirtualBox):52453 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=8ms reason=close()
CLOSE TCP   pid=2831(Chrome_ChildIOT) cookie=70076  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):36740 -> [2603:1061:14:32:0:0:0:1](pending):443  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=2831(Chrome_ChildIOT) cookie=70077  10.0.2.15(lev-VirtualBox):39416 -> 13.107.253.44(pending):443
CLOSE UDP   pid=534(systemd-resolve) cookie=70075  10.0.2.15(lev-VirtualBox):46890 -> 10.0.2.3(vboxdns):53  out=50B/1p in=50B/1p  age=44ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=68164  10.0.2.15(lev-VirtualBox):38723 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=28ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=68163  10.0.2.15(lev-VirtualBox):55379 -> 10.0.2.3(vboxdns):53  out=191B/2p in=420B/2p  age=48ms reason=close()
OPEN  ICMPv6 pid=675(NetworkManager) cookie=9698  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57687  out=200B/1p in=37B/1p  age=5.533s reason=idle
CLOSE TCP   pid=2831(Chrome_ChildIOT) cookie=70077  10.0.2.15(lev-VirtualBox):39416 -> 13.107.253.44(no-ptr):443  out=3404B/6p in=67056B/17p  age=5.446s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58973  out=187B/1p in=101B/1p  age=5.467s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42871  out=141B/1p in=55B/1p  age=5.462s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):19672  out=134B/1p in=37B/1p  age=5.54s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51953  out=113B/1p in=51B/1p  age=5.535s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47577  out=141B/1p in=55B/1p  age=5.463s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48861  out=125B/1p in=101B/1p  age=5.539s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):27382  out=106B/1p in=37B/1p  age=5.547s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33811  out=163B/1p in=101B/1p  age=5.546s reason=idle
CLOSE ICMPv6 pid=675(NetworkManager) cookie=9698  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.439s reason=idle
OPEN  UDP   pid=3337(DNS Res~ver #23) cookie=70865  127.0.0.1(localhost):56652 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3337(DNS Res~ver #24) cookie=70145  127.0.0.1(localhost):36471 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36471
OPEN  UDP   pid=534(systemd-resolve) cookie=68218  10.0.2.15(lev-VirtualBox):39471 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56652
OPEN  UDP   pid=534(systemd-resolve) cookie=68219  10.0.2.15(lev-VirtualBox):59530 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=68219  10.0.2.15(lev-VirtualBox):59530 -> 10.0.2.3(vboxdns):53  out=44B/1p in=295B/1p  age=43ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=68218  10.0.2.15(lev-VirtualBox):39471 -> 10.0.2.3(vboxdns):53  out=44B/1p in=247B/1p  age=47ms reason=close()
CLOSE UDP   pid=3337(DNS Res~ver #24) cookie=70145  127.0.0.1(localhost):36471 -> 127.0.0.53(dnsstub):53  out=44B/1p in=144B/1p  age=55ms reason=close()
CLOSE UDP   pid=3337(DNS Res~ver #23) cookie=70865  127.0.0.1(localhost):56652 -> 127.0.0.53(dnsstub):53  out=88B/2p in=336B/2p  age=57ms reason=close()
OPEN  TCP   pid=3337(Socket Thread) cookie=70147  10.0.2.15(lev-VirtualBox):44392 -> 151.101.1.91(miss):443
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33765
OPEN  UDP   pid=534(systemd-resolve) cookie=68220  10.0.2.15(lev-VirtualBox):37246 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=68220  10.0.2.15(lev-VirtualBox):37246 -> 10.0.2.3(vboxdns):53  out=97B/2p in=276B/2p  age=30ms reason=close()
CLOSE TCP   pid=3337(Socket Thread) cookie=70147  10.0.2.15(lev-VirtualBox):44392 -> 151.101.1.91(no-ptr):443  out=3003B/7p in=1991B/16p  age=5.443s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36471  out=144B/1p in=44B/1p  age=5.508s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56652  out=336B/2p in=88B/2p  age=5.507s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33765  out=114B/1p in=54B/1p  age=5.44s reason=idle
OPEN  TCP   pid=3337(Socket Thread) cookie=65892  10.0.2.15(lev-VirtualBox):38530 -> 140.82.112.26(miss):443
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55238
OPEN  TCP   pid=3337(Socket Thread) cookie=61087  10.0.2.15(lev-VirtualBox):59324 -> 34.107.243.93(miss):443
CLOSE TCP   pid=3337(Socket Thread) cookie=61087  10.0.2.15(lev-VirtualBox):59324 -> 34.107.243.93(pending):443  out=24B/1p in=0B/0p  age=6ms reason=close()
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39286
OPEN  UDP   pid=534(systemd-resolve) cookie=68252  10.0.2.15(lev-VirtualBox):58694 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=68252  10.0.2.15(lev-VirtualBox):58694 -> 10.0.2.3(vboxdns):53  out=55B/1p in=133B/1p  age=40ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55238  out=100B/1p in=55B/1p  age=5.722s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=65892  10.0.2.15(lev-VirtualBox):38530 -> 140.82.112.26(lb-140-82-112-26-iad.github.com):443  out=30B/1p in=26B/2p  age=5.724s reason=idle
OPEN  UDP   pid=3337(DNS Res~ver #24) cookie=69124  127.0.0.1(localhost):36672 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36672
OPEN  UDP   pid=534(systemd-resolve) cookie=68280  10.0.2.15(lev-VirtualBox):50758 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=68281  10.0.2.15(lev-VirtualBox):50504 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=68280  10.0.2.15(lev-VirtualBox):50758 -> 10.0.2.3(vboxdns):53  out=54B/1p in=95B/1p  age=41ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=68281  10.0.2.15(lev-VirtualBox):50504 -> 10.0.2.3(vboxdns):53  out=54B/1p in=158B/1p  age=45ms reason=close()
CLOSE UDP   pid=3337(DNS Res~ver #24) cookie=69124  127.0.0.1(localhost):36672 -> 127.0.0.53(dnsstub):53  out=108B/2p in=205B/2p  age=49ms reason=close()
OPEN  TCP   pid=3337(Socket Thread) cookie=70184  10.0.2.15(lev-VirtualBox):49786 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
OPEN  TCP   pid=3337(Socket Thread) cookie=70185  10.0.2.15(lev-VirtualBox):49800 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
OPEN  UDP   pid=3337(DNS Res~ver #23) cookie=70912  127.0.0.1(localhost):41735 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3337(DNS Res~ver #24) cookie=69126  127.0.0.1(localhost):39714 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41735
OPEN  UDP   pid=534(systemd-resolve) cookie=68282  10.0.2.15(lev-VirtualBox):48479 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=68283  10.0.2.15(lev-VirtualBox):40551 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39714
CLOSE UDP   pid=534(systemd-resolve) cookie=68282  10.0.2.15(lev-VirtualBox):48479 -> 10.0.2.3(vboxdns):53  out=66B/1p in=291B/1p  age=28ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=68283  10.0.2.15(lev-VirtualBox):40551 -> 10.0.2.3(vboxdns):53  out=66B/1p in=339B/1p  age=28ms reason=close()
CLOSE UDP   pid=3337(DNS Res~ver #24) cookie=69126  127.0.0.1(localhost):39714 -> 127.0.0.53(dnsstub):53  out=66B/1p in=166B/1p  age=30ms reason=close()
CLOSE UDP   pid=3337(DNS Res~ver #23) cookie=70912  127.0.0.1(localhost):41735 -> 127.0.0.53(dnsstub):53  out=132B/2p in=380B/2p  age=37ms reason=close()
OPEN  TCP   pid=3337(Socket Thread) cookie=70914  10.0.2.15(lev-VirtualBox):42810 -> 151.101.129.91(miss):443
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57885
OPEN  UDP   pid=534(systemd-resolve) cookie=68284  10.0.2.15(lev-VirtualBox):47979 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=68284  10.0.2.15(lev-VirtualBox):47979 -> 10.0.2.3(vboxdns):53  out=101B/2p in=280B/2p  age=31ms reason=close()
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39286  out=107B/1p in=55B/1p  age=5.968s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41735  out=380B/2p in=132B/2p  age=5.145s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36672  out=205B/2p in=108B/2p  age=5.974s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70185  10.0.2.15(lev-VirtualBox):49800 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=2065B/4p in=1242B/12p  age=5.702s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57885  out=116B/1p in=56B/1p  age=5.102s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70184  10.0.2.15(lev-VirtualBox):49786 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=2712B/4p in=861B/12p  age=5.919s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39714  out=166B/1p in=66B/1p  age=5.142s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70914  10.0.2.15(lev-VirtualBox):42810 -> 151.101.129.91(no-ptr):443  out=723B/5p in=4342B/20p  age=6.103s reason=idle
OPEN  TCP   pid=2831(Chrome_ChildIOT) cookie=70077  10.0.2.15(lev-VirtualBox):39416 -> 13.107.253.44(no-ptr):443
CLOSE TCP   pid=2831(Chrome_ChildIOT) cookie=70077  10.0.2.15(lev-VirtualBox):39416 -> 13.107.253.44(no-ptr):443  out=0B/0p in=63B/1p  age=5ms reason=close()
OPEN  TCP   pid=3337(Socket Thread) cookie=70147  10.0.2.15(lev-VirtualBox):44392 -> 151.101.1.91(no-ptr):443
CLOSE TCP   pid=3337(Socket Thread) cookie=70147  10.0.2.15(lev-VirtualBox):44392 -> 151.101.1.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.921s reason=idle
OPEN  TCP   pid=3337(Socket Thread) cookie=65892  10.0.2.15(lev-VirtualBox):38530 -> 140.82.112.26(lb-140-82-112-26-iad.github.com):443
OPEN  TCP   pid=3337(Socket Thread) cookie=70184  10.0.2.15(lev-VirtualBox):49786 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
OPEN  TCP   pid=3337(Socket Thread) cookie=70914  10.0.2.15(lev-VirtualBox):42810 -> 151.101.129.91(no-ptr):443
CLOSE TCP   pid=3337(Socket Thread) cookie=65892  10.0.2.15(lev-VirtualBox):38530 -> 140.82.112.26(lb-140-82-112-26-iad.github.com):443  out=30B/1p in=26B/2p  age=5.722s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70184  10.0.2.15(lev-VirtualBox):49786 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=39B/1p in=39B/2p  age=5.916s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70914  10.0.2.15(lev-VirtualBox):42810 -> 151.101.129.91(no-ptr):443  out=46B/1p in=46B/2p  age=5.915s reason=idle
OPEN  TCP   pid=3337(Socket Thread) cookie=70147  10.0.2.15(lev-VirtualBox):44392 -> 151.101.1.91(no-ptr):443
CLOSE TCP   pid=3337(Socket Thread) cookie=70147  10.0.2.15(lev-VirtualBox):44392 -> 151.101.1.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.911s reason=idle
OPEN  UDP   pid=534(systemd-resolve) cookie=68479  10.0.2.15(lev-VirtualBox):33423 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=534(systemd-resolve) cookie=68479  10.0.2.15(lev-VirtualBox):33423 -> 10.0.2.3(vboxdns):53  out=58B/1p in=685B/1p  age=40ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=70286  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):41356 -> [2620:2d:4000:1:0:0:0:97](miss):80
OPEN  TCP   pid=675(NetworkManager) cookie=70287  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):35922 -> [2001:67c:1562:0:0:0:0:24](miss):80
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56366
CLOSE TCP   pid=675(NetworkManager) cookie=70286  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):41356 -> [2620:2d:4000:1:0:0:0:97](pending):80  out=0B/0p in=0B/0p  age=7ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=71143  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):51690 -> [2620:2d:4000:1:0:0:0:96](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=70287  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):35922 -> [2001:67c:1562:0:0:0:0:24](pending):80  out=0B/0p in=0B/0p  age=4ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=71144  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):42532 -> [2001:67c:1562:0:0:0:0:23](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=71143  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):51690 -> [2620:2d:4000:1:0:0:0:96](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=70290  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):57632 -> [2620:2d:4000:1:0:0:0:2b](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=71144  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):42532 -> [2001:67c:1562:0:0:0:0:23](pending):80  out=0B/0p in=0B/0p  age=6ms reason=close()
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47380
OPEN  TCP   pid=675(NetworkManager) cookie=70291  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):40686 -> [2620:2d:4002:1:0:0:0:198](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=70290  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):57632 -> [2620:2d:4000:1:0:0:0:2b](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=70292  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):47720 -> [2620:2d:4000:1:0:0:0:2a](miss):80
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57758
CLOSE TCP   pid=675(NetworkManager) cookie=70291  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):40686 -> [2620:2d:4002:1:0:0:0:198](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=70293  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):49388 -> [2620:2d:4000:1:0:0:0:22](miss):80
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50130
CLOSE TCP   pid=675(NetworkManager) cookie=70292  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):47720 -> [2620:2d:4000:1:0:0:0:2a](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43543
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43638
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44979
OPEN  TCP   pid=675(NetworkManager) cookie=70295  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):41588 -> [2620:2d:4000:1:0:0:0:98](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=70293  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):49388 -> [2620:2d:4000:1:0:0:0:22](pending):80  out=0B/0p in=0B/0p  age=12ms reason=close()
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35124
OPEN  TCP   pid=675(NetworkManager) cookie=70296  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):56856 -> [2620:2d:4000:1:0:0:0:23](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=70295  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):41588 -> [2620:2d:4000:1:0:0:0:98](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=70297  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):51502 -> [2620:2d:4002:1:0:0:0:197](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=70296  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):56856 -> [2620:2d:4000:1:0:0:0:23](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
CLOSE TCP   pid=675(NetworkManager) cookie=70297  [fd00:0:0:0:63c4:dda4:4be2:b7c7](lev-VirtualBox):51502 -> [2620:2d:4002:1:0:0:0:197](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60832
OPEN  UDP   pid=534(systemd-resolve) cookie=71148  10.0.2.15(lev-VirtualBox):59963 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40563
OPEN  UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42600
CLOSE UDP   pid=534(systemd-resolve) cookie=71148  10.0.2.15(lev-VirtualBox):59963 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=5ms reason=close()
OPEN  ICMPv6 pid=675(NetworkManager) cookie=9698  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44979  out=135B/1p in=101B/1p  age=5.925s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35124  out=147B/1p in=101B/1p  age=5.925s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47380  out=137B/1p in=101B/1p  age=5.939s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56366  out=155B/1p in=101B/1p  age=5.948s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57758  out=136B/1p in=101B/1p  age=5.937s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40563  out=138B/1p in=101B/1p  age=5.92s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43638  out=155B/1p in=101B/1p  age=5.925s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42600  out=155B/1p in=101B/1p  age=5.917s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43543  out=147B/1p in=101B/1p  age=5.935s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50130  out=155B/1p in=101B/1p  age=5.936s reason=idle
CLOSE UDP   pid=534(systemd-resolve) cookie=3869  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60832  out=155B/1p in=101B/1p  age=5.921s reason=idle
OPEN  TCP   pid=3337(Socket Thread) cookie=65892  10.0.2.15(lev-VirtualBox):38530 -> 140.82.112.26(lb-140-82-112-26-iad.github.com):443
CLOSE ICMPv6 pid=675(NetworkManager) cookie=9698  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.576s reason=idle
OPEN  TCP   pid=3337(Socket Thread) cookie=70184  10.0.2.15(lev-VirtualBox):49786 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
OPEN  TCP   pid=3337(Socket Thread) cookie=70914  10.0.2.15(lev-VirtualBox):42810 -> 151.101.129.91(no-ptr):443
CLOSE TCP   pid=3337(Socket Thread) cookie=65892  10.0.2.15(lev-VirtualBox):38530 -> 140.82.112.26(lb-140-82-112-26-iad.github.com):443  out=30B/1p in=26B/2p  age=5.726s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70184  10.0.2.15(lev-VirtualBox):49786 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=39B/1p in=39B/2p  age=5.909s reason=idle
CLOSE TCP   pid=3337(Socket Thread) cookie=70914  10.0.2.15(lev-VirtualBox):42810 -> 151.101.129.91(no-ptr):443  out=46B/1p in=46B/2p  age=5.908s reason=idle
^C2026/02/24 04:23:19.305682 PERF_LOST_TOTAL total=0 gen=0
