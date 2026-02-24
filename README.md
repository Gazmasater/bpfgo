
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





CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=50160  127.0.0.1(localhost):57371 -> 127.0.0.53(dnsstub):53  out=39B/1p in=104B/1p  age=8ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51097  10.0.2.15(lev-VirtualBox):55623 -> 10.0.2.3(vboxdns):53  out=46B/1p in=141B/1p  age=138ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51106  10.0.2.15(lev-VirtualBox):48590 -> 10.0.2.3(vboxdns):53  out=50B/1p in=131B/1p  age=26ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=51659  127.0.0.1(localhost):40349 -> 127.0.0.53(dnsstub):53  out=86B/2p in=278B/2p  age=126ms reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #21) cookie=52715  127.0.0.1(localhost):44237 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44237
OPEN  UDP   pid=524(systemd-resolve) cookie=51108  10.0.2.15(lev-VirtualBox):60382 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=52716  127.0.0.1(localhost):35001 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=51109  10.0.2.15(lev-VirtualBox):55135 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35001
OPEN  UDP   pid=524(systemd-resolve) cookie=51110  10.0.2.15(lev-VirtualBox):37708 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51096  10.0.2.15(lev-VirtualBox):50377 -> 10.0.2.3(vboxdns):53  out=46B/1p in=129B/1p  age=171ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #15) cookie=50156  127.0.0.1(localhost):49270 -> 127.0.0.53(dnsstub):53  out=92B/2p in=194B/2p  age=175ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51108  10.0.2.15(lev-VirtualBox):60382 -> 10.0.2.3(vboxdns):53  out=40B/1p in=337B/1p  age=25ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=52713  10.0.2.15(lev-VirtualBox):39378 -> 34.120.208.123(miss):443
CLOSE UDP   pid=524(systemd-resolve) cookie=51109  10.0.2.15(lev-VirtualBox):55135 -> 10.0.2.3(vboxdns):53  out=40B/1p in=196B/1p  age=26ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49908
OPEN  UDP   pid=524(systemd-resolve) cookie=51111  10.0.2.15(lev-VirtualBox):39968 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=5612(DNS Res~ver #21) cookie=52715  127.0.0.1(localhost):44237 -> 127.0.0.53(dnsstub):53  out=80B/2p in=368B/2p  age=42ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51110  10.0.2.15(lev-VirtualBox):37708 -> 10.0.2.3(vboxdns):53  out=40B/1p in=119B/1p  age=48ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=52716  127.0.0.1(localhost):35001 -> 127.0.0.53(dnsstub):53  out=40B/1p in=97B/1p  age=60ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51111  10.0.2.15(lev-VirtualBox):39968 -> 10.0.2.3(vboxdns):53  out=56B/1p in=136B/1p  age=42ms reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #18) cookie=51112  127.0.0.1(localhost):34620 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39827
OPEN  UDP   pid=5612(DNS Res~ver #19) cookie=50164  127.0.0.1(localhost):39827 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=51113  10.0.2.15(lev-VirtualBox):51732 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34620
OPEN  UDP   pid=524(systemd-resolve) cookie=51114  10.0.2.15(lev-VirtualBox):50248 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=51115  10.0.2.15(lev-VirtualBox):46561 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51113  10.0.2.15(lev-VirtualBox):51732 -> 10.0.2.3(vboxdns):53  out=49B/1p in=85B/1p  age=26ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51114  10.0.2.15(lev-VirtualBox):50248 -> 10.0.2.3(vboxdns):53  out=49B/1p in=133B/1p  age=30ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51115  10.0.2.15(lev-VirtualBox):46561 -> 10.0.2.3(vboxdns):53  out=49B/1p in=97B/1p  age=29ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=51112  127.0.0.1(localhost):34620 -> 127.0.0.53(dnsstub):53  out=49B/1p in=106B/1p  age=51ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=50164  127.0.0.1(localhost):39827 -> 127.0.0.53(dnsstub):53  out=98B/2p in=142B/2p  age=51ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=50167  10.0.2.15(lev-VirtualBox):60455 -> 74.125.205.119(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35147
OPEN  UDP   pid=524(systemd-resolve) cookie=51116  10.0.2.15(lev-VirtualBox):57057 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51116  10.0.2.15(lev-VirtualBox):57057 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=27ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=50172  10.0.2.15(lev-VirtualBox):34008 -> 64.233.164.95(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41156
OPEN  UDP   pid=524(systemd-resolve) cookie=51117  10.0.2.15(lev-VirtualBox):56790 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51117  10.0.2.15(lev-VirtualBox):56790 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=25ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=53275  10.0.2.15(lev-VirtualBox):42426 -> 74.125.205.119(le-in-f119.1e100.net):443
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=52730  127.0.0.1(localhost):51263 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=5612(DNS Res~ver #15) cookie=53284  127.0.0.1(localhost):39896 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51263
OPEN  UDP   pid=524(systemd-resolve) cookie=52731  10.0.2.15(lev-VirtualBox):36262 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39896
OPEN  UDP   pid=524(systemd-resolve) cookie=52732  10.0.2.15(lev-VirtualBox):39779 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=51156  10.0.2.15(lev-VirtualBox):52442 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=52732  10.0.2.15(lev-VirtualBox):39779 -> 10.0.2.3(vboxdns):53  out=46B/1p in=79B/1p  age=50ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52731  10.0.2.15(lev-VirtualBox):36262 -> 10.0.2.3(vboxdns):53  out=46B/1p in=88B/1p  age=54ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51156  10.0.2.15(lev-VirtualBox):52442 -> 10.0.2.3(vboxdns):53  out=46B/1p in=91B/1p  age=49ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=52730  127.0.0.1(localhost):51263 -> 127.0.0.53(dnsstub):53  out=46B/1p in=71B/1p  age=61ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #15) cookie=53284  127.0.0.1(localhost):39896 -> 127.0.0.53(dnsstub):53  out=92B/2p in=136B/2p  age=61ms reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=51158  127.0.0.1(localhost):40865 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=5612(DNS Res~ver #21) cookie=52733  127.0.0.1(localhost):42681 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40865
OPEN  UDP   pid=524(systemd-resolve) cookie=51159  10.0.2.15(lev-VirtualBox):46275 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=51160  10.0.2.15(lev-VirtualBox):41592 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42681
OPEN  UDP   pid=524(systemd-resolve) cookie=51161  10.0.2.15(lev-VirtualBox):49724 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51159  10.0.2.15(lev-VirtualBox):46275 -> 10.0.2.3(vboxdns):53  out=48B/1p in=83B/1p  age=29ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51160  10.0.2.15(lev-VirtualBox):41592 -> 10.0.2.3(vboxdns):53  out=48B/1p in=95B/1p  age=28ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=51158  127.0.0.1(localhost):40865 -> 127.0.0.53(dnsstub):53  out=96B/2p in=140B/2p  age=42ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=53287  10.0.2.15(lev-VirtualBox):46082 -> 74.125.205.94(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43241
OPEN  UDP   pid=524(systemd-resolve) cookie=51164  10.0.2.15(lev-VirtualBox):54312 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51161  10.0.2.15(lev-VirtualBox):49724 -> 10.0.2.3(vboxdns):53  out=48B/1p in=128B/1p  age=26ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #21) cookie=52733  127.0.0.1(localhost):42681 -> 127.0.0.53(dnsstub):53  out=48B/1p in=98B/1p  age=50ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51164  10.0.2.15(lev-VirtualBox):54312 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=23ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=53291  10.0.2.15(lev-VirtualBox):55541 -> 74.125.205.84(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56928
OPEN  UDP   pid=524(systemd-resolve) cookie=51166  10.0.2.15(lev-VirtualBox):51285 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51166  10.0.2.15(lev-VirtualBox):51285 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=24ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=53295  10.0.2.15(lev-VirtualBox):46958 -> 74.125.205.84(le-in-f84.1e100.net):443
OPEN  TCP   pid=5612(Socket Thread) cookie=52734  10.0.2.15(lev-VirtualBox):46966 -> 74.125.205.84(le-in-f84.1e100.net):443
OPEN  UDP   pid=5612(DNS Res~ver #19) cookie=53297  127.0.0.1(localhost):37749 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37749
OPEN  UDP   pid=524(systemd-resolve) cookie=51167  10.0.2.15(lev-VirtualBox):57374 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=5612(DNS Res~ver #18) cookie=51723  127.0.0.1(localhost):36609 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36609
OPEN  UDP   pid=524(systemd-resolve) cookie=51168  10.0.2.15(lev-VirtualBox):36109 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51167  10.0.2.15(lev-VirtualBox):57374 -> 10.0.2.3(vboxdns):53  out=58B/1p in=238B/1p  age=43ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51168  10.0.2.15(lev-VirtualBox):36109 -> 10.0.2.3(vboxdns):53  out=58B/1p in=286B/1p  age=39ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=51723  127.0.0.1(localhost):36609 -> 127.0.0.53(dnsstub):53  out=58B/1p in=122B/1p  age=43ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=53297  127.0.0.1(localhost):37749 -> 127.0.0.53(dnsstub):53  out=116B/2p in=292B/2p  age=46ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=52735  10.0.2.15(lev-VirtualBox):48590 -> 151.101.65.91(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57931
OPEN  UDP   pid=524(systemd-resolve) cookie=51169  10.0.2.15(lev-VirtualBox):43434 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51169  10.0.2.15(lev-VirtualBox):43434 -> 10.0.2.3(vboxdns):53  out=99B/2p in=278B/2p  age=27ms reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=51171  127.0.0.1(localhost):47711 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47711
OPEN  UDP   pid=524(systemd-resolve) cookie=51172  10.0.2.15(lev-VirtualBox):59700 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=51173  10.0.2.15(lev-VirtualBox):51197 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=5612(DNS Res~ver #15) cookie=53300  127.0.0.1(localhost):37051 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37051
OPEN  UDP   pid=524(systemd-resolve) cookie=51176  10.0.2.15(lev-VirtualBox):44460 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51172  10.0.2.15(lev-VirtualBox):59700 -> 10.0.2.3(vboxdns):53  out=43B/1p in=223B/1p  age=23ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51173  10.0.2.15(lev-VirtualBox):51197 -> 10.0.2.3(vboxdns):53  out=43B/1p in=211B/1p  age=24ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=51171  127.0.0.1(localhost):47711 -> 127.0.0.53(dnsstub):53  out=86B/2p in=294B/2p  age=27ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=51176  10.0.2.15(lev-VirtualBox):44460 -> 10.0.2.3(vboxdns):53  out=43B/1p in=82B/1p  age=26ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #15) cookie=53300  127.0.0.1(localhost):37051 -> 127.0.0.53(dnsstub):53  out=43B/1p in=68B/1p  age=29ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=52738  10.0.2.15(lev-VirtualBox):57222 -> 74.125.205.147(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50448
OPEN  UDP   pid=524(systemd-resolve) cookie=51181  10.0.2.15(lev-VirtualBox):40523 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51181  10.0.2.15(lev-VirtualBox):40523 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=29ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33604  out=113B/1p in=51B/1p  age=5.655s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35227  out=167B/2p in=86B/2p  age=5.694s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46259  out=98B/1p in=54B/1p  age=5.399s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33696  out=59B/1p in=43B/1p  age=5.604s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=52696  10.0.2.15(lev-VirtualBox):33562 -> 140.82.121.5(lb-140-82-121-5-fra.github.com):443  out=4588B/6p in=4337B/22p  age=5.604s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=53291  10.0.2.15(lev-VirtualBox):55541 -> 74.125.205.84(le-in-f84.1e100.net):443  out=3846B/4p in=8163B/6p  age=2.117s reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #21) cookie=52747  127.0.0.1(localhost):58547 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=5612(DNS Res~ver #18) cookie=51762  127.0.0.1(localhost):54058 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54058
OPEN  UDP   pid=524(systemd-resolve) cookie=54273  10.0.2.15(lev-VirtualBox):60692 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58547
OPEN  UDP   pid=524(systemd-resolve) cookie=54274  10.0.2.15(lev-VirtualBox):34333 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54275  10.0.2.15(lev-VirtualBox):58315 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54274  10.0.2.15(lev-VirtualBox):34333 -> 10.0.2.3(vboxdns):53  out=56B/1p in=228B/1p  age=44ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54275  10.0.2.15(lev-VirtualBox):58315 -> 10.0.2.3(vboxdns):53  out=56B/1p in=276B/1p  age=43ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54273  10.0.2.15(lev-VirtualBox):60692 -> 10.0.2.3(vboxdns):53  out=56B/1p in=108B/1p  age=45ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #21) cookie=52747  127.0.0.1(localhost):58547 -> 127.0.0.53(dnsstub):53  out=112B/2p in=288B/2p  age=49ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=51762  127.0.0.1(localhost):54058 -> 127.0.0.53(dnsstub):53  out=56B/1p in=81B/1p  age=50ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=53344  10.0.2.15(lev-VirtualBox):57118 -> 64.233.161.155(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58992
OPEN  UDP   pid=524(systemd-resolve) cookie=54287  10.0.2.15(lev-VirtualBox):33115 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54287  10.0.2.15(lev-VirtualBox):33115 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=25ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=53351  10.0.2.15(lev-VirtualBox):33910 -> 64.233.161.155(lh-in-f155.1e100.net):443
OPEN  UDP   pid=5612(DNS Res~ver #19) cookie=53355  127.0.0.1(localhost):56769 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56769
OPEN  UDP   pid=524(systemd-resolve) cookie=54291  10.0.2.15(lev-VirtualBox):50769 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54292  10.0.2.15(lev-VirtualBox):43135 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=54293  127.0.0.1(localhost):55121 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55121
OPEN  UDP   pid=524(systemd-resolve) cookie=54294  10.0.2.15(lev-VirtualBox):46786 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54291  10.0.2.15(lev-VirtualBox):50769 -> 10.0.2.3(vboxdns):53  out=62B/1p in=187B/1p  age=25ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54292  10.0.2.15(lev-VirtualBox):43135 -> 10.0.2.3(vboxdns):53  out=62B/1p in=199B/1p  age=29ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54294  10.0.2.15(lev-VirtualBox):46786 -> 10.0.2.3(vboxdns):53  out=62B/1p in=193B/1p  age=25ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=54293  127.0.0.1(localhost):55121 -> 127.0.0.53(dnsstub):53  out=62B/1p in=114B/1p  age=30ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=53355  127.0.0.1(localhost):56769 -> 127.0.0.53(dnsstub):53  out=124B/2p in=228B/2p  age=36ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36540
OPEN  UDP   pid=5612(DNS Res~ver #15) cookie=53359  127.0.0.1(localhost):36540 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=5612(DNS Res~ver #15) cookie=53359  127.0.0.1(localhost):36540 -> 127.0.0.53(dnsstub):53  out=120B/2p in=164B/2p  age=5ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=51764  10.0.2.15(lev-VirtualBox):45184 -> 173.194.57.167(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40309
OPEN  UDP   pid=524(systemd-resolve) cookie=54297  10.0.2.15(lev-VirtualBox):56896 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54297  10.0.2.15(lev-VirtualBox):56896 -> 10.0.2.3(vboxdns):53  out=56B/1p in=121B/1p  age=24ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40082  out=254B/2p in=88B/2p  age=5.14s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55403  out=155B/1p in=44B/1p  age=5.142s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=51769  10.0.2.15(lev-VirtualBox):41074 -> 173.194.57.167(dfw25s55-in-f7.1e100.net):443
OPEN  TCP   pid=5612(Socket Thread) cookie=51770  10.0.2.15(lev-VirtualBox):41080 -> 173.194.57.167(dfw25s55-in-f7.1e100.net):443
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34091  out=344B/2p in=96B/2p  age=5.398s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34725  out=270B/2p in=94B/2p  age=5.369s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48591  out=90B/1p in=56B/1p  age=5.998s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57371  out=104B/1p in=39B/1p  age=5.197s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50589  out=346B/2p in=88B/2p  age=5.465s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40349  out=278B/2p in=86B/2p  age=5.306s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41006  out=183B/1p in=44B/1p  age=5.447s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43599  out=282B/2p in=88B/2p  age=5.453s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35818  out=347B/2p in=118B/2p  age=5.331s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44237  out=368B/2p in=80B/2p  age=5.173s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49270  out=194B/2p in=92B/2p  age=5.327s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57932  out=159B/2p in=78B/2p  age=5.295s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43113  out=134B/1p in=59B/1p  age=5.332s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34620  out=106B/1p in=49B/1p  age=5.062s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=50840  10.0.2.15(lev-VirtualBox):37198 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=7815B/3p in=572B/6p  age=5.595s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54817  out=329B/2p in=98B/2p  age=5.682s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59147  out=128B/1p in=47B/1p  age=5.346s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35001  out=97B/1p in=40B/1p  age=5.169s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49908  out=109B/1p in=56B/1p  age=5.142s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50271  out=143B/1p in=44B/1p  age=5.469s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49415  out=126B/1p in=46B/1p  age=5.32s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39827  out=142B/2p in=98B/2p  age=5.075s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=52709  10.0.2.15(lev-VirtualBox):45974 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=2087B/4p in=8362B/12p  age=5.809s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49182  out=136B/1p in=43B/1p  age=5.302s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58111  out=100B/1p in=55B/1p  age=5.388s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39246  out=145B/1p in=48B/1p  age=5.384s reason=idle
OPEN  UDP   pid=5612(DNS Res~ver #21) cookie=52902  127.0.0.1(localhost):46157 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46157
OPEN  UDP   pid=5612(DNS Res~ver #18) cookie=51805  127.0.0.1(localhost):50442 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=52903  10.0.2.15(lev-VirtualBox):49040 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=52904  10.0.2.15(lev-VirtualBox):43921 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50442
OPEN  UDP   pid=524(systemd-resolve) cookie=52905  10.0.2.15(lev-VirtualBox):42130 -> 10.0.2.3(vboxdns):53
CLOSE TCP   pid=5612(Socket Thread) cookie=53275  10.0.2.15(lev-VirtualBox):42426 -> 74.125.205.119(le-in-f119.1e100.net):443  out=2083B/4p in=6296B/12p  age=5.818s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35147  out=90B/1p in=56B/1p  age=5.965s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41156  out=88B/1p in=55B/1p  age=5.879s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=50167  10.0.2.15(lev-VirtualBox):60455 -> 74.125.205.119(le-in-f119.1e100.net):443  out=4287B/6p in=10086B/9p  age=5.967s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=52713  10.0.2.15(lev-VirtualBox):39378 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=5232B/7p in=4863B/18p  age=6.317s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=50172  10.0.2.15(lev-VirtualBox):34008 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=3997B/5p in=6942B/7p  age=5.881s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=52905  10.0.2.15(lev-VirtualBox):42130 -> 10.0.2.3(vboxdns):53  out=51B/1p in=136B/1p  age=39ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=51805  127.0.0.1(localhost):50442 -> 127.0.0.53(dnsstub):53  out=51B/1p in=111B/1p  age=47ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52903  10.0.2.15(lev-VirtualBox):49040 -> 10.0.2.3(vboxdns):53  out=51B/1p in=127B/1p  age=45ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52904  10.0.2.15(lev-VirtualBox):43921 -> 10.0.2.3(vboxdns):53  out=51B/1p in=151B/1p  age=48ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #21) cookie=52902  127.0.0.1(localhost):46157 -> 127.0.0.53(dnsstub):53  out=102B/2p in=190B/2p  age=53ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=53751  10.0.2.15(lev-VirtualBox):43099 -> 108.177.14.148(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53249
OPEN  UDP   pid=524(systemd-resolve) cookie=52908  10.0.2.15(lev-VirtualBox):39308 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=52908  10.0.2.15(lev-VirtualBox):39308 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=25ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=52909  10.0.2.15(lev-VirtualBox):52140 -> 108.177.14.148(lt-in-f148.1e100.net):443
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43241  out=88B/1p in=55B/1p  age=5.893s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42681  out=98B/1p in=48B/1p  age=5.914s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56928  out=88B/1p in=55B/1p  age=5.854s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51263  out=71B/1p in=46B/1p  age=6.005s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=53295  10.0.2.15(lev-VirtualBox):46958 -> 74.125.205.84(le-in-f84.1e100.net):443  out=2091B/4p in=4564B/12p  age=5.84s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39896  out=136B/2p in=92B/2p  age=6.002s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40865  out=140B/2p in=96B/2p  age=5.937s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=53287  10.0.2.15(lev-VirtualBox):46082 -> 74.125.205.94(le-in-f94.1e100.net):443  out=3998B/5p in=6819B/6p  age=5.897s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57931  out=115B/1p in=55B/1p  age=5.658s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36609  out=122B/1p in=58B/1p  age=5.871s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=52734  10.0.2.15(lev-VirtualBox):46966 -> 74.125.205.84(le-in-f84.1e100.net):443  out=2568B/10p in=9848B/40p  age=6.531s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37051  out=68B/1p in=43B/1p  age=5.624s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47711  out=294B/2p in=86B/2p  age=5.628s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37749  out=292B/2p in=116B/2p  age=5.874s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=52735  10.0.2.15(lev-VirtualBox):48590 -> 151.101.65.91(no-ptr):443  out=3392B/7p in=2340B/14p  age=5.82s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50448  out=90B/1p in=56B/1p  age=5.578s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=51769  10.0.2.15(lev-VirtualBox):41074 -> 173.194.57.167(dfw25s55-in-f7.1e100.net):443  out=2006B/3p in=4625B/6p  age=5.013s reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56769  out=228B/2p in=124B/2p  age=5.306s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58547  out=288B/2p in=112B/2p  age=5.652s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36540  out=164B/2p in=120B/2p  age=5.248s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58992  out=90B/1p in=56B/1p  age=5.562s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=53351  10.0.2.15(lev-VirtualBox):33910 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=2099B/4p in=6119B/12p  age=5.533s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40309  out=94B/1p in=56B/1p  age=5.237s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54058  out=81B/1p in=56B/1p  age=5.653s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=53344  10.0.2.15(lev-VirtualBox):57118 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=4607B/9p in=7977B/9p  age=5.564s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=52738  10.0.2.15(lev-VirtualBox):57222 -> 74.125.205.147(le-in-f147.1e100.net):443  out=4615B/10p in=31350B/14p  age=6.581s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55121  out=114B/1p in=62B/1p  age=5.299s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=51770  10.0.2.15(lev-VirtualBox):41080 -> 173.194.57.167(dfw25s55-in-f7.1e100.net):443  out=2006B/3p in=4625B/6p  age=6.005s reason=close()
CLOSE UDP   pid=5612(Socket Thread) cookie=51764  10.0.2.15(lev-VirtualBox):45184 -> 173.194.57.167(dfw25s55-in-f7.1e100.net):443  out=9730B/12p in=7939B/19p  age=7.236s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=52705  10.0.2.15(lev-VirtualBox):42710 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=31832B/155p in=3317947B/606p  age=11.999s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=52909  10.0.2.15(lev-VirtualBox):52140 -> 108.177.14.148(lt-in-f148.1e100.net):443  out=2094B/4p in=6077B/12p  age=5.888s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46157  out=190B/2p in=102B/2p  age=6.022s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=53751  10.0.2.15(lev-VirtualBox):43099 -> 108.177.14.148(lt-in-f148.1e100.net):443  out=4375B/8p in=7084B/6p  age=5.957s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53249  out=90B/1p in=56B/1p  age=5.954s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50442  out=111B/1p in=51B/1p  age=6.018s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=48118  10.0.2.15(lev-VirtualBox):53222 -> 140.82.113.25(miss):443
CLOSE TCP   pid=5612(Socket Thread) cookie=48118  10.0.2.15(lev-VirtualBox):53222 -> 140.82.113.25(pending):443  out=30B/1p in=26B/2p  age=5.638s reason=idle
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54547
OPEN  UDP   pid=524(systemd-resolve) cookie=53770  10.0.2.15(lev-VirtualBox):34275 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53770  10.0.2.15(lev-VirtualBox):34275 -> 10.0.2.3(vboxdns):53  out=55B/1p in=126B/1p  age=41ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54547  out=100B/1p in=55B/1p  age=5.998s reason=idle
OPEN  UDP   pid=524(systemd-resolve) cookie=53809  10.0.2.15(lev-VirtualBox):44818 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53809  10.0.2.15(lev-VirtualBox):44818 -> 10.0.2.3(vboxdns):53  out=58B/1p in=553B/1p  age=42ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=52950  10.0.2.15(lev-VirtualBox):38104 -> 185.125.190.49(miss):80
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53060
OPEN  UDP   pid=524(systemd-resolve) cookie=53811  10.0.2.15(lev-VirtualBox):44321 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53811  10.0.2.15(lev-VirtualBox):44321 -> 10.0.2.3(vboxdns):53  out=56B/1p in=120B/1p  age=23ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51867  [fd00:0:0:0:baa6:3f8d:5eed:42d0](miss):49156 -> [2620:2d:4002:1:0:0:0:198](miss):80
OPEN  TCP   pid=675(NetworkManager) cookie=51868  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):47634 -> [2001:67c:1562:0:0:0:0:23](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51867  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):49156 -> [2620:2d:4002:1:0:0:0:198](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51869  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):48020 -> [2620:2d:4000:1:0:0:0:22](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51868  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):47634 -> [2001:67c:1562:0:0:0:0:23](pending):80  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57715
OPEN  UDP   pid=524(systemd-resolve) cookie=53813  10.0.2.15(lev-VirtualBox):43249 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58873
OPEN  UDP   pid=524(systemd-resolve) cookie=53814  10.0.2.15(lev-VirtualBox):43023 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=675(NetworkManager) cookie=51870  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):47246 -> [2620:2d:4000:1:0:0:0:23](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51869  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):48020 -> [2620:2d:4000:1:0:0:0:22](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51871  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):36218 -> [2001:67c:1562:0:0:0:0:24](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51870  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):47246 -> [2620:2d:4000:1:0:0:0:23](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51872  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):42826 -> [2620:2d:4000:1:0:0:0:2a](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51871  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):36218 -> [2001:67c:1562:0:0:0:0:24](pending):80  out=0B/0p in=0B/0p  age=7ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51873  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):46784 -> [2620:2d:4002:1:0:0:0:197](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51872  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):42826 -> [2620:2d:4000:1:0:0:0:2a](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51874  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):40704 -> [2620:2d:4000:1:0:0:0:96](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51873  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):46784 -> [2620:2d:4002:1:0:0:0:197](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50220
OPEN  UDP   pid=524(systemd-resolve) cookie=53817  10.0.2.15(lev-VirtualBox):46202 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39685
OPEN  UDP   pid=524(systemd-resolve) cookie=53818  10.0.2.15(lev-VirtualBox):40526 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=675(NetworkManager) cookie=51875  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):38222 -> [2620:2d:4000:1:0:0:0:2b](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51874  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):40704 -> [2620:2d:4000:1:0:0:0:96](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51876  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):47516 -> [2620:2d:4000:1:0:0:0:97](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51875  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):38222 -> [2620:2d:4000:1:0:0:0:2b](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=51877  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):55042 -> [2620:2d:4000:1:0:0:0:98](miss):80
CLOSE TCP   pid=675(NetworkManager) cookie=51876  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):47516 -> [2620:2d:4000:1:0:0:0:97](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
CLOSE TCP   pid=675(NetworkManager) cookie=51877  [fd00:0:0:0:baa6:3f8d:5eed:42d0](pending):55042 -> [2620:2d:4000:1:0:0:0:98](pending):80  out=0B/0p in=0B/0p  age=4ms reason=close()
CLOSE TCP   pid=675(NetworkManager) cookie=52950  10.0.2.15(lev-VirtualBox):38104 -> 185.125.190.49(fracktail.canonical.com):80  out=87B/1p in=189B/1p  age=115ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=53814  10.0.2.15(lev-VirtualBox):43023 -> 10.0.2.3(vboxdns):53  out=191B/2p in=362B/2p  age=60ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52345
OPEN  UDP   pid=524(systemd-resolve) cookie=51878  10.0.2.15(lev-VirtualBox):38103 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53813  10.0.2.15(lev-VirtualBox):43249 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=65ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54414
OPEN  UDP   pid=524(systemd-resolve) cookie=51879  10.0.2.15(lev-VirtualBox):48668 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53817  10.0.2.15(lev-VirtualBox):46202 -> 10.0.2.3(vboxdns):53  out=101B/1p in=207B/1p  age=64ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33762
OPEN  UDP   pid=524(systemd-resolve) cookie=54647  10.0.2.15(lev-VirtualBox):43229 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53818  10.0.2.15(lev-VirtualBox):40526 -> 10.0.2.3(vboxdns):53  out=101B/1p in=208B/1p  age=113ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36530
OPEN  UDP   pid=524(systemd-resolve) cookie=54648  10.0.2.15(lev-VirtualBox):58008 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51878  10.0.2.15(lev-VirtualBox):38103 -> 10.0.2.3(vboxdns):53  out=101B/1p in=210B/1p  age=68ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47812
OPEN  UDP   pid=524(systemd-resolve) cookie=52951  10.0.2.15(lev-VirtualBox):32980 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=51879  10.0.2.15(lev-VirtualBox):48668 -> 10.0.2.3(vboxdns):53  out=101B/1p in=209B/1p  age=77ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58467
OPEN  UDP   pid=524(systemd-resolve) cookie=52952  10.0.2.15(lev-VirtualBox):41051 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54647  10.0.2.15(lev-VirtualBox):43229 -> 10.0.2.3(vboxdns):53  out=101B/1p in=219B/1p  age=96ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52952  10.0.2.15(lev-VirtualBox):41051 -> 10.0.2.3(vboxdns):53  out=101B/1p in=219B/1p  age=24ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57012
OPEN  UDP   pid=524(systemd-resolve) cookie=52953  10.0.2.15(lev-VirtualBox):49560 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33182
OPEN  UDP   pid=524(systemd-resolve) cookie=52954  10.0.2.15(lev-VirtualBox):53870 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54648  10.0.2.15(lev-VirtualBox):58008 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=70ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52951  10.0.2.15(lev-VirtualBox):32980 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=66ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52954  10.0.2.15(lev-VirtualBox):53870 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=26ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=52953  10.0.2.15(lev-VirtualBox):49560 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=30ms reason=close()
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=52962  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):48969 -> [2001:4860:4860:0:0:0:0:8888](miss):443
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=52962  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):48969 -> [2001:4860:4860:0:0:0:0:8888](pending):443  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50947
OPEN  UDP   pid=524(systemd-resolve) cookie=52963  10.0.2.15(lev-VirtualBox):48366 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=52963  10.0.2.15(lev-VirtualBox):48366 -> 10.0.2.3(vboxdns):53  out=101B/1p in=197B/1p  age=40ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=52709  10.0.2.15(lev-VirtualBox):45974 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52709  10.0.2.15(lev-VirtualBox):45974 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.927s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=50840  10.0.2.15(lev-VirtualBox):37198 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=50840  10.0.2.15(lev-VirtualBox):37198 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=39B/1p in=39B/2p  age=5.927s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=53275  10.0.2.15(lev-VirtualBox):42426 -> 74.125.205.119(le-in-f119.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53275  10.0.2.15(lev-VirtualBox):42426 -> 74.125.205.119(le-in-f119.1e100.net):443  out=39B/1p in=39B/2p  age=5.922s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52713  10.0.2.15(lev-VirtualBox):39378 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52713  10.0.2.15(lev-VirtualBox):39378 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=39B/1p in=39B/2p  age=5.922s reason=idle
OPEN  ICMPv6 pid=675(NetworkManager) cookie=9766  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50220  out=135B/1p in=101B/1p  age=5.272s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54414  out=137B/1p in=101B/1p  age=5.21s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52345  out=138B/1p in=101B/1p  age=5.217s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57012  out=155B/1p in=101B/1p  age=5.102s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57715  out=155B/1p in=101B/1p  age=5.28s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36530  out=155B/1p in=101B/1p  age=5.156s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=53295  10.0.2.15(lev-VirtualBox):46958 -> 74.125.205.84(le-in-f84.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53295  10.0.2.15(lev-VirtualBox):46958 -> 74.125.205.84(le-in-f84.1e100.net):443  out=39B/1p in=39B/2p  age=5.92s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33182  out=155B/1p in=101B/1p  age=5.099s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47812  out=155B/1p in=101B/1p  age=5.145s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33762  out=147B/1p in=101B/1p  age=5.204s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39685  out=136B/1p in=101B/1p  age=5.272s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58467  out=147B/1p in=101B/1p  age=5.129s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58873  out=163B/1p in=101B/1p  age=5.28s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53060  out=93B/1p in=56B/1p  age=5.368s reason=idle
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=54658  127.0.0.1(localhost):57594 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57594
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=54658  127.0.0.1(localhost):57594 -> 127.0.0.53(dnsstub):53  out=88B/2p in=282B/2p  age=7ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=54659  10.0.2.15(lev-VirtualBox):54116 -> 64.233.164.198(lf-in-f198.1e100.net):443
OPEN  TCP   pid=5612(Socket Thread) cookie=54663  10.0.2.15(lev-VirtualBox):56546 -> 64.233.164.198(lf-in-f198.1e100.net):443
OPEN  TCP   pid=5612(Socket Thread) cookie=52734  10.0.2.15(lev-VirtualBox):46966 -> 74.125.205.84(le-in-f84.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52734  10.0.2.15(lev-VirtualBox):46966 -> 74.125.205.84(le-in-f84.1e100.net):443  out=39B/1p in=39B/2p  age=5.91s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52735  10.0.2.15(lev-VirtualBox):48590 -> 151.101.65.91(no-ptr):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52735  10.0.2.15(lev-VirtualBox):48590 -> 151.101.65.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.913s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50947  out=125B/1p in=101B/1p  age=5.1s reason=idle
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51916  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):46282 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51916  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):46282 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51917  127.0.0.1(localhost):21111 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51918  127.0.0.1(localhost):14714 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):21111
OPEN  UDP   pid=524(systemd-resolve) cookie=53894  10.0.2.15(lev-VirtualBox):43628 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51919  127.0.0.1(localhost):43706 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):14714
OPEN  UDP   pid=524(systemd-resolve) cookie=53895  10.0.2.15(lev-VirtualBox):43496 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43706
OPEN  UDP   pid=524(systemd-resolve) cookie=53896  10.0.2.15(lev-VirtualBox):33522 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53894  10.0.2.15(lev-VirtualBox):43628 -> 10.0.2.3(vboxdns):53  out=48B/1p in=168B/1p  age=53ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=53897  10.0.2.15(lev-VirtualBox):36771 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53896  10.0.2.15(lev-VirtualBox):33522 -> 10.0.2.3(vboxdns):53  out=48B/1p in=419B/1p  age=52ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51919  127.0.0.1(localhost):43706 -> 127.0.0.53(dnsstub):53  out=37B/1p in=200B/1p  age=54ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=53895  10.0.2.15(lev-VirtualBox):43496 -> 10.0.2.3(vboxdns):53  out=48B/1p in=217B/1p  age=54ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51918  127.0.0.1(localhost):14714 -> 127.0.0.53(dnsstub):53  out=37B/1p in=134B/1p  age=56ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51917  127.0.0.1(localhost):21111 -> 127.0.0.53(dnsstub):53  out=37B/1p in=0B/0p  age=69ms reason=close()
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51920  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):35373 -> [2603:1061:14:32:0:0:0:1](miss):80
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51921  10.0.2.15(lev-VirtualBox):42318 -> 13.107.226.44(miss):80
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=51922  10.0.2.15(lev-VirtualBox):58695 -> 13.107.253.44(miss):80
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51920  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):35373 -> [2603:1061:14:32:0:0:0:1](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51922  10.0.2.15(lev-VirtualBox):58695 -> 13.107.253.44(pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45916
OPEN  UDP   pid=524(systemd-resolve) cookie=53899  10.0.2.15(lev-VirtualBox):53902 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=51921  10.0.2.15(lev-VirtualBox):42318 -> 13.107.226.44(pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56307
OPEN  UDP   pid=524(systemd-resolve) cookie=53900  10.0.2.15(lev-VirtualBox):60518 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44120
OPEN  UDP   pid=524(systemd-resolve) cookie=53901  10.0.2.15(lev-VirtualBox):38165 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53899  10.0.2.15(lev-VirtualBox):53902 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=24ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=53901  10.0.2.15(lev-VirtualBox):38165 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=25ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=53897  10.0.2.15(lev-VirtualBox):36771 -> 10.0.2.3(vboxdns):53  out=50B/1p in=295B/1p  age=58ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=53902  10.0.2.15(lev-VirtualBox):35783 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53900  10.0.2.15(lev-VirtualBox):60518 -> 10.0.2.3(vboxdns):53  out=191B/2p in=420B/2p  age=51ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=53902  10.0.2.15(lev-VirtualBox):35783 -> 10.0.2.3(vboxdns):53  out=61B/1p in=151B/1p  age=25ms reason=close()
CLOSE UDP   pid=5612(Socket Thread) cookie=54659  10.0.2.15(lev-VirtualBox):54116 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=12520B/9p in=4910B/8p  age=1.297s reason=close()
OPEN  TCP   pid=2844(Chrome_ChildIOT) cookie=51925  10.0.2.15(lev-VirtualBox):44500 -> 13.107.226.44(no-ptr):443
OPEN  TCP   pid=5612(Socket Thread) cookie=53351  10.0.2.15(lev-VirtualBox):33910 -> 64.233.161.155(lh-in-f155.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53351  10.0.2.15(lev-VirtualBox):33910 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=39B/1p in=39B/2p  age=5.808s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52909  10.0.2.15(lev-VirtualBox):52140 -> 108.177.14.148(lt-in-f148.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52909  10.0.2.15(lev-VirtualBox):52140 -> 108.177.14.148(lt-in-f148.1e100.net):443  out=39B/1p in=39B/2p  age=5.849s reason=idle
CLOSE ICMPv6 pid=675(NetworkManager) cookie=9766  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.194s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57594  out=282B/2p in=88B/2p  age=5.809s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=54663  10.0.2.15(lev-VirtualBox):56546 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=8157B/8p in=1238B/20p  age=5.623s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44120  out=141B/1p in=55B/1p  age=5.453s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45916  out=141B/1p in=55B/1p  age=5.461s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56307  out=187B/1p in=101B/1p  age=5.459s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):21111  out=228B/1p in=37B/1p  age=5.532s reason=idle
CLOSE TCP   pid=2844(Chrome_ChildIOT) cookie=51925  10.0.2.15(lev-VirtualBox):44500 -> 13.107.226.44(no-ptr):443  out=2798B/6p in=74342B/21p  age=5.456s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43706  out=200B/1p in=37B/1p  age=5.53s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):14714  out=134B/1p in=37B/1p  age=5.531s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=48118  10.0.2.15(lev-VirtualBox):53222 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=48118  10.0.2.15(lev-VirtualBox):53222 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.635s reason=idle
OPEN  ICMPv6 pid=675(NetworkManager) cookie=9766  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE ICMPv6 pid=675(NetworkManager) cookie=9766  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=56B/1p  age=5.346s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=51501  10.0.2.15(lev-VirtualBox):51972 -> 34.107.243.93(miss):443
CLOSE TCP   pid=5612(Socket Thread) cookie=51501  10.0.2.15(lev-VirtualBox):51972 -> 34.107.243.93(pending):443  out=28B/1p in=24B/2p  age=5.356s reason=idle
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40567
OPEN  UDP   pid=524(systemd-resolve) cookie=53018  10.0.2.15(lev-VirtualBox):44598 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53018  10.0.2.15(lev-VirtualBox):44598 -> 10.0.2.3(vboxdns):53  out=55B/1p in=133B/1p  age=71ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40567  out=107B/1p in=55B/1p  age=5.953s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=50840  10.0.2.15(lev-VirtualBox):37198 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=50840  10.0.2.15(lev-VirtualBox):37198 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=102B/3p in=78B/4p  age=1.917s reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=53275  10.0.2.15(lev-VirtualBox):42426 -> 74.125.205.119(le-in-f119.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53275  10.0.2.15(lev-VirtualBox):42426 -> 74.125.205.119(le-in-f119.1e100.net):443  out=39B/1p in=39B/2p  age=5.339s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52709  10.0.2.15(lev-VirtualBox):45974 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52709  10.0.2.15(lev-VirtualBox):45974 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.338s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52713  10.0.2.15(lev-VirtualBox):39378 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52713  10.0.2.15(lev-VirtualBox):39378 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=39B/1p in=39B/2p  age=5.336s reason=idle
OPEN  UDP   pid=5612(DNS Res~ver #19) cookie=54816  127.0.0.1(localhost):45873 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45873
CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=54816  127.0.0.1(localhost):45873 -> 127.0.0.53(dnsstub):53  out=112B/2p in=288B/2p  age=7ms reason=close()
OPEN  UDP   pid=5612(Socket Thread) cookie=54017  10.0.2.15(lev-VirtualBox):52667 -> 64.233.161.155(lh-in-f155.1e100.net):443
OPEN  TCP   pid=5612(Socket Thread) cookie=53295  10.0.2.15(lev-VirtualBox):46958 -> 74.125.205.84(le-in-f84.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53295  10.0.2.15(lev-VirtualBox):46958 -> 74.125.205.84(le-in-f84.1e100.net):443  out=39B/1p in=39B/2p  age=5.328s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=54028  10.0.2.15(lev-VirtualBox):41298 -> 64.233.161.155(lh-in-f155.1e100.net):443
OPEN  TCP   pid=5612(Socket Thread) cookie=52735  10.0.2.15(lev-VirtualBox):48590 -> 151.101.65.91(no-ptr):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52735  10.0.2.15(lev-VirtualBox):48590 -> 151.101.65.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.327s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52734  10.0.2.15(lev-VirtualBox):46966 -> 74.125.205.84(le-in-f84.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52734  10.0.2.15(lev-VirtualBox):46966 -> 74.125.205.84(le-in-f84.1e100.net):443  out=39B/1p in=39B/2p  age=5.326s reason=idle
CLOSE UDP   pid=5612(Socket Thread) cookie=54017  10.0.2.15(lev-VirtualBox):52667 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=8764B/5p in=4888B/5p  age=1.053s reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=53351  10.0.2.15(lev-VirtualBox):33910 -> 64.233.161.155(lh-in-f155.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53351  10.0.2.15(lev-VirtualBox):33910 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=39B/1p in=39B/2p  age=5.324s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=52909  10.0.2.15(lev-VirtualBox):52140 -> 108.177.14.148(lt-in-f148.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=52909  10.0.2.15(lev-VirtualBox):52140 -> 108.177.14.148(lt-in-f148.1e100.net):443  out=39B/1p in=39B/2p  age=5.318s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45873  out=288B/2p in=112B/2p  age=5.109s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=54028  10.0.2.15(lev-VirtualBox):41298 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=1937B/8p in=1843B/26p  age=5.995s reason=idle
OPEN  UDP   pid=5612(DNS Res~ver #15) cookie=54870  127.0.0.1(localhost):49601 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49601
OPEN  UDP   pid=524(systemd-resolve) cookie=54050  10.0.2.15(lev-VirtualBox):38874 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54051  10.0.2.15(lev-VirtualBox):38186 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54051  10.0.2.15(lev-VirtualBox):38186 -> 10.0.2.3(vboxdns):53  out=45B/1p in=174B/1p  age=46ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=54052  10.0.2.15(lev-VirtualBox):54983 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54050  10.0.2.15(lev-VirtualBox):38874 -> 10.0.2.3(vboxdns):53  out=45B/1p in=121B/1p  age=51ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54052  10.0.2.15(lev-VirtualBox):54983 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=23ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #15) cookie=54870  127.0.0.1(localhost):49601 -> 127.0.0.53(dnsstub):53  out=90B/2p in=209B/2p  age=74ms reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #21) cookie=53114  127.0.0.1(localhost):49074 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=5612(DNS Res~ver #18) cookie=54871  127.0.0.1(localhost):36515 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36515
OPEN  UDP   pid=524(systemd-resolve) cookie=54053  10.0.2.15(lev-VirtualBox):53735 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49074
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=52038  127.0.0.1(localhost):45803 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54054  10.0.2.15(lev-VirtualBox):55887 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45803
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=52038  127.0.0.1(localhost):45803 -> 127.0.0.53(dnsstub):53  out=45B/1p in=80B/1p  age=2ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54054  10.0.2.15(lev-VirtualBox):55887 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=3ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #21) cookie=53114  127.0.0.1(localhost):49074 -> 127.0.0.53(dnsstub):53  out=90B/2p in=225B/2p  age=13ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54053  10.0.2.15(lev-VirtualBox):53735 -> 10.0.2.3(vboxdns):53  out=44B/1p in=141B/1p  age=23ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=54871  127.0.0.1(localhost):36515 -> 127.0.0.53(dnsstub):53  out=45B/1p in=164B/1p  age=26ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=54055  10.0.2.15(lev-VirtualBox):52572 -> 140.82.112.25(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39910
OPEN  UDP   pid=524(systemd-resolve) cookie=54056  10.0.2.15(lev-VirtualBox):56513 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54056  10.0.2.15(lev-VirtualBox):56513 -> 10.0.2.3(vboxdns):53  out=55B/1p in=126B/1p  age=25ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=54663  10.0.2.15(lev-VirtualBox):56546 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54663  10.0.2.15(lev-VirtualBox):56546 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.313s reason=idle
OPEN  UDP   pid=5612(DNS Res~ver #19) cookie=54873  127.0.0.1(localhost):39207 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39207
OPEN  UDP   pid=524(systemd-resolve) cookie=54057  10.0.2.15(lev-VirtualBox):40481 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54058  10.0.2.15(lev-VirtualBox):54599 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=54058  10.0.2.15(lev-VirtualBox):54599 -> 10.0.2.3(vboxdns):53  out=39B/1p in=123B/1p  age=5ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54057  10.0.2.15(lev-VirtualBox):40481 -> 10.0.2.3(vboxdns):53  out=39B/1p in=65B/1p  age=41ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=54873  127.0.0.1(localhost):39207 -> 127.0.0.53(dnsstub):53  out=78B/2p in=159B/2p  age=42ms reason=close()
OPEN  UDP   pid=5612(DNS Res~ver #15) cookie=54875  127.0.0.1(localhost):38618 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38618
CLOSE UDP   pid=5612(DNS Res~ver #15) cookie=54875  127.0.0.1(localhost):38618 -> 127.0.0.53(dnsstub):53  out=39B/1p in=55B/1p  age=3ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=48118  10.0.2.15(lev-VirtualBox):53222 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=48118  10.0.2.15(lev-VirtualBox):53222 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=69B/2p in=41B/2p  age=1.008s reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=54059  10.0.2.15(lev-VirtualBox):33446 -> 140.82.121.3(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49888
OPEN  UDP   pid=524(systemd-resolve) cookie=52040  10.0.2.15(lev-VirtualBox):36747 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=52040  10.0.2.15(lev-VirtualBox):36747 -> 10.0.2.3(vboxdns):53  out=54B/1p in=123B/1p  age=23ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54055  10.0.2.15(lev-VirtualBox):52572 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=3730B/6p in=3848B/18p  age=1.604s reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36515  out=164B/1p in=45B/1p  age=5.398s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45803  out=80B/1p in=45B/1p  age=5.393s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49074  out=225B/2p in=90B/2p  age=5.396s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49601  out=209B/2p in=90B/2p  age=5.475s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39910  out=100B/1p in=55B/1p  age=5.051s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38618  out=55B/1p in=39B/1p  age=5.782s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49888  out=98B/1p in=54B/1p  age=5.59s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39207  out=159B/2p in=78B/2p  age=5.87s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=54059  10.0.2.15(lev-VirtualBox):33446 -> 140.82.121.3(lb-140-82-121-3-fra.github.com):443  out=4038B/7p in=13050B/34p  age=5.782s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=3655B/4p in=3894B/20p  age=6.184s reason=idle
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56109
OPEN  UDP   pid=5612(DNS Res~ver #20) cookie=54097  127.0.0.1(localhost):56109 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54942  10.0.2.15(lev-VirtualBox):45753 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=5612(DNS Res~ver #21) cookie=53193  127.0.0.1(localhost):50869 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=54943  10.0.2.15(lev-VirtualBox):32986 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50869
CLOSE UDP   pid=524(systemd-resolve) cookie=54942  10.0.2.15(lev-VirtualBox):45753 -> 10.0.2.3(vboxdns):53  out=44B/1p in=247B/1p  age=95ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #21) cookie=53193  127.0.0.1(localhost):50869 -> 127.0.0.53(dnsstub):53  out=44B/1p in=144B/1p  age=108ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=54943  10.0.2.15(lev-VirtualBox):32986 -> 10.0.2.3(vboxdns):53  out=44B/1p in=295B/1p  age=108ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #20) cookie=54097  127.0.0.1(localhost):56109 -> 127.0.0.53(dnsstub):53  out=88B/2p in=336B/2p  age=111ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=54100  10.0.2.15(lev-VirtualBox):46146 -> 151.101.193.91(miss):443
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55304
OPEN  UDP   pid=524(systemd-resolve) cookie=53195  10.0.2.15(lev-VirtualBox):58953 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=53195  10.0.2.15(lev-VirtualBox):58953 -> 10.0.2.3(vboxdns):53  out=101B/2p in=280B/2p  age=37ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56109  out=336B/2p in=88B/2p  age=5.18s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50869  out=144B/1p in=44B/1p  age=5.177s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=54100  10.0.2.15(lev-VirtualBox):46146 -> 151.101.193.91(no-ptr):443  out=3003B/7p in=1991B/16p  age=6.065s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55304  out=116B/1p in=56B/1p  age=5.819s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=54028  10.0.2.15(lev-VirtualBox):41298 -> 64.233.161.155(lh-in-f155.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54028  10.0.2.15(lev-VirtualBox):41298 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=39B/1p in=39B/2p  age=5.986s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=54663  10.0.2.15(lev-VirtualBox):56546 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54663  10.0.2.15(lev-VirtualBox):56546 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.977s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.893s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=54100  10.0.2.15(lev-VirtualBox):46146 -> 151.101.193.91(no-ptr):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54100  10.0.2.15(lev-VirtualBox):46146 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.975s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=54028  10.0.2.15(lev-VirtualBox):41298 -> 64.233.161.155(lh-in-f155.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54028  10.0.2.15(lev-VirtualBox):41298 -> 64.233.161.155(lh-in-f155.1e100.net):443  out=39B/1p in=39B/2p  age=5.93s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.731s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=54100  10.0.2.15(lev-VirtualBox):46146 -> 151.101.193.91(no-ptr):443
CLOSE TCP   pid=5612(Socket Thread) cookie=54100  10.0.2.15(lev-VirtualBox):46146 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.076s reason=idle
OPEN  UDP   pid=5612(DNS Res~ver #19) cookie=55039  127.0.0.1(localhost):42429 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=5612(DNS Res~ver #18) cookie=54272  127.0.0.1(localhost):39121 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39121
OPEN  UDP   pid=524(systemd-resolve) cookie=55403  10.0.2.15(lev-VirtualBox):50904 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42429
OPEN  UDP   pid=524(systemd-resolve) cookie=55404  10.0.2.15(lev-VirtualBox):49251 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=55405  10.0.2.15(lev-VirtualBox):51727 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=55403  10.0.2.15(lev-VirtualBox):50904 -> 10.0.2.3(vboxdns):53  out=44B/1p in=199B/1p  age=42ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55404  10.0.2.15(lev-VirtualBox):49251 -> 10.0.2.3(vboxdns):53  out=44B/1p in=240B/1p  age=41ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=55406  10.0.2.15(lev-VirtualBox):55181 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=55405  10.0.2.15(lev-VirtualBox):51727 -> 10.0.2.3(vboxdns):53  out=44B/1p in=211B/1p  age=42ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #18) cookie=54272  127.0.0.1(localhost):39121 -> 127.0.0.53(dnsstub):53  out=88B/2p in=254B/2p  age=51ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55406  10.0.2.15(lev-VirtualBox):55181 -> 10.0.2.3(vboxdns):53  out=54B/1p in=136B/1p  age=25ms reason=close()
CLOSE UDP   pid=5612(DNS Res~ver #19) cookie=55039  127.0.0.1(localhost):42429 -> 127.0.0.53(dnsstub):53  out=44B/1p in=155B/1p  age=73ms reason=close()
OPEN  TCP   pid=5612(Socket Thread) cookie=55042  10.0.2.15(lev-VirtualBox):46342 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42429  out=155B/1p in=44B/1p  age=5.087s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39121  out=254B/2p in=88B/2p  age=5.09s reason=idle
CLOSE TCP   pid=5612(Socket Thread) cookie=55042  10.0.2.15(lev-VirtualBox):46342 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=6049B/8p in=2328B/20p  age=5.998s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.847s reason=idle
OPEN  UDP   pid=524(systemd-resolve) cookie=55430  10.0.2.15(lev-VirtualBox):57867 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=55430  10.0.2.15(lev-VirtualBox):57867 -> 10.0.2.3(vboxdns):53  out=58B/1p in=553B/1p  age=42ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=56368  10.0.2.15(lev-VirtualBox):38466 -> 185.125.190.96(miss):80
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48667
OPEN  UDP   pid=524(systemd-resolve) cookie=55432  10.0.2.15(lev-VirtualBox):54774 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=55432  10.0.2.15(lev-VirtualBox):54774 -> 10.0.2.3(vboxdns):53  out=56B/1p in=137B/1p  age=26ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=55436  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):32944 -> [2620:2d:4000:1:0:0:0:2a](is-content-cache-1.canonical.com):80
OPEN  TCP   pid=675(NetworkManager) cookie=57446  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):41722 -> [2620:2d:4000:1:0:0:0:98](ubuntu-content-cache-3.ps5.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=55436  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):32944 -> [2620:2d:4000:1:0:0:0:2a](is-content-cache-1.canonical.com):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57447  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):52430 -> [2620:2d:4002:1:0:0:0:198](ubuntu-content-cache-3.ps6.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57446  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):41722 -> [2620:2d:4000:1:0:0:0:98](ubuntu-content-cache-3.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57448  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):39026 -> [2001:67c:1562:0:0:0:0:24](blackcat.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57447  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):52430 -> [2620:2d:4002:1:0:0:0:198](ubuntu-content-cache-3.ps6.canonical.com):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57449  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):49476 -> [2620:2d:4000:1:0:0:0:96](ubuntu-content-cache-1.ps5.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57448  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):39026 -> [2001:67c:1562:0:0:0:0:24](blackcat.canonical.com):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57450  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):50336 -> [2620:2d:4000:1:0:0:0:22](gladys.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57449  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):49476 -> [2620:2d:4000:1:0:0:0:96](ubuntu-content-cache-1.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57451  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):35094 -> [2001:67c:1562:0:0:0:0:23](amyrose.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57450  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):50336 -> [2620:2d:4000:1:0:0:0:22](gladys.canonical.com):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57452  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):45492 -> [2620:2d:4000:1:0:0:0:2b](is-content-cache-2.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57451  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):35094 -> [2001:67c:1562:0:0:0:0:23](amyrose.canonical.com):80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57453  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):57030 -> [2620:2d:4002:1:0:0:0:197](ubuntu-content-cache-2.ps6.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57452  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):45492 -> [2620:2d:4000:1:0:0:0:2b](is-content-cache-2.canonical.com):80  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57454  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):59204 -> [2620:2d:4000:1:0:0:0:23](fracktail.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57453  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):57030 -> [2620:2d:4002:1:0:0:0:197](ubuntu-content-cache-2.ps6.canonical.com):80  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  TCP   pid=675(NetworkManager) cookie=57455  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):42710 -> [2620:2d:4000:1:0:0:0:97](ubuntu-content-cache-2.ps5.canonical.com):80
CLOSE TCP   pid=675(NetworkManager) cookie=57454  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):59204 -> [2620:2d:4000:1:0:0:0:23](fracktail.canonical.com):80  out=0B/0p in=0B/0p  age=3ms reason=close()
CLOSE TCP   pid=675(NetworkManager) cookie=57455  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):42710 -> [2620:2d:4000:1:0:0:0:97](ubuntu-content-cache-2.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=3ms reason=close()
CLOSE TCP   pid=675(NetworkManager) cookie=56368  10.0.2.15(lev-VirtualBox):38466 -> 185.125.190.96(ubuntu-content-cache-1.ps5.canonical.com):80  out=87B/1p in=189B/1p  age=322ms reason=close()
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=55157  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):42270 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=55157  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):42270 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443  out=0B/0p in=0B/0p  age=4ms reason=close()
OPEN  ICMPv6 pid=675(NetworkManager) cookie=9766  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48667  out=110B/1p in=56B/1p  age=5.348s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=55042  10.0.2.15(lev-VirtualBox):46342 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=5612(Socket Thread) cookie=55042  10.0.2.15(lev-VirtualBox):46342 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.619s reason=idle
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49651
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=56379  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):38705 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=56379  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):38705 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=56380  127.0.0.1(localhost):49651 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=55501  10.0.2.15(lev-VirtualBox):42513 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=55502  127.0.0.1(localhost):12592 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):12592
OPEN  UDP   pid=524(systemd-resolve) cookie=55503  10.0.2.15(lev-VirtualBox):54879 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=55504  127.0.0.1(localhost):45312 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45312
OPEN  UDP   pid=524(systemd-resolve) cookie=55505  10.0.2.15(lev-VirtualBox):56755 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=55505  10.0.2.15(lev-VirtualBox):56755 -> 10.0.2.3(vboxdns):53  out=48B/1p in=419B/1p  age=52ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55501  10.0.2.15(lev-VirtualBox):42513 -> 10.0.2.3(vboxdns):53  out=48B/1p in=168B/1p  age=66ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=55506  10.0.2.15(lev-VirtualBox):56785 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=55504  127.0.0.1(localhost):45312 -> 127.0.0.53(dnsstub):53  out=37B/1p in=200B/1p  age=58ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55503  10.0.2.15(lev-VirtualBox):54879 -> 10.0.2.3(vboxdns):53  out=48B/1p in=217B/1p  age=71ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=55502  127.0.0.1(localhost):12592 -> 127.0.0.53(dnsstub):53  out=37B/1p in=134B/1p  age=73ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=56380  127.0.0.1(localhost):49651 -> 127.0.0.53(dnsstub):53  out=37B/1p in=0B/0p  age=91ms reason=close()
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=55507  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):47207 -> [2603:1061:14:32:0:0:0:1](miss):80
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=55508  10.0.2.15(lev-VirtualBox):52703 -> 13.107.226.44(miss):80
OPEN  UDP   pid=2844(Chrome_ChildIOT) cookie=55509  10.0.2.15(lev-VirtualBox):47164 -> 13.107.253.44(miss):80
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=55507  [fd00:0:0:0:baa6:3f8d:5eed:42d0](lev-VirtualBox):47207 -> [2603:1061:14:32:0:0:0:1](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=55509  10.0.2.15(lev-VirtualBox):47164 -> 13.107.253.44(pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
CLOSE UDP   pid=2844(Chrome_ChildIOT) cookie=55508  10.0.2.15(lev-VirtualBox):52703 -> 13.107.226.44(pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54639
OPEN  UDP   pid=524(systemd-resolve) cookie=55510  10.0.2.15(lev-VirtualBox):38684 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45126
OPEN  UDP   pid=524(systemd-resolve) cookie=55511  10.0.2.15(lev-VirtualBox):43357 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51523
OPEN  UDP   pid=524(systemd-resolve) cookie=55512  10.0.2.15(lev-VirtualBox):57795 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=524(systemd-resolve) cookie=55506  10.0.2.15(lev-VirtualBox):56785 -> 10.0.2.3(vboxdns):53  out=61B/1p in=151B/1p  age=45ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55511  10.0.2.15(lev-VirtualBox):43357 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=27ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55512  10.0.2.15(lev-VirtualBox):57795 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=28ms reason=close()
CLOSE UDP   pid=524(systemd-resolve) cookie=55510  10.0.2.15(lev-VirtualBox):38684 -> 10.0.2.3(vboxdns):53  out=191B/2p in=420B/2p  age=52ms reason=close()
OPEN  TCP   pid=2844(Chrome_ChildIOT) cookie=55514  10.0.2.15(lev-VirtualBox):51040 -> 13.107.226.44(no-ptr):443
CLOSE ICMPv6 pid=675(NetworkManager) cookie=9766  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.049s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45312  out=200B/1p in=37B/1p  age=5.954s reason=idle
CLOSE TCP   pid=2844(Chrome_ChildIOT) cookie=55514  10.0.2.15(lev-VirtualBox):51040 -> 13.107.226.44(no-ptr):443  out=3532B/6p in=67057B/11p  age=5.859s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54639  out=187B/1p in=101B/1p  age=5.872s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49651  out=228B/1p in=37B/1p  age=5.966s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45126  out=141B/1p in=55B/1p  age=5.869s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):12592  out=134B/1p in=37B/1p  age=5.962s reason=idle
CLOSE UDP   pid=524(systemd-resolve) cookie=2965  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51523  out=141B/1p in=55B/1p  age=5.868s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=53115  10.0.2.15(lev-VirtualBox):40806 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.586s reason=idle
OPEN  TCP   pid=5612(Socket Thread) cookie=51501  10.0.2.15(lev-VirtualBox):51972 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
CLOSE TCP   pid=5612(Socket Thread) cookie=51501  10.0.2.15(lev-VirtualBox):51972 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=28B/1p in=24B/2p  age=5.077s reason=idle
