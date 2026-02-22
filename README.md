
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




lev@lev-VirtualBox:~/bpfgo$ sudo ./bpfgo
2026/02/22 21:15:51 dotenv loaded: .env
2026/02/22 21:15:51.632775 hostsPrefill: added=7 from /etc/hosts
2026/02/22 21:15:52.059825 pprof on :6060
2026/02/22 21:15:52.061504 perf ring per-cpu=512KB total~=2MB cpus=4
2026/02/22 21:15:52.064249 OPEN/CLOSE (TCP/UDP/ICMP) + PTR + skb-hint. Ctrl+C to exit
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=55219  10.0.2.15(miss):37882 -> 150.171.109.50(miss):443
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=55219  10.0.2.15(pending):37882 -> 150.171.109.50(pending):443  out=0B/0p in=63B/1p  age=3ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43060
OPEN  UDP   pid=514(systemd-resolve) cookie=56009  10.0.2.15(pending):45754 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50341
OPEN  UDP   pid=514(systemd-resolve) cookie=56010  10.0.2.15(pending):32994 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56010  10.0.2.15(pending):32994 -> 10.0.2.3(vboxdns):53  out=101B/2p in=330B/2p  age=65ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=56009  10.0.2.15(pending):45754 -> 10.0.2.3(vboxdns):53  out=91B/2p in=91B/2p  age=76ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43060  out=113B/1p in=51B/1p  age=5.328s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50341  out=142B/1p in=56B/1p  age=5.319s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=54107  10.0.2.15(lev-VirtualBox):41336 -> 64.233.164.198(miss):443
CLOSE TCP   pid=2995(Socket Thread) cookie=54107  10.0.2.15(lev-VirtualBox):41336 -> 64.233.164.198(pending):443  out=63B/2p in=0B/0p  age=3ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47626
OPEN  UDP   pid=514(systemd-resolve) cookie=58472  10.0.2.15(lev-VirtualBox):54630 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=58472  10.0.2.15(lev-VirtualBox):54630 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=46ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=55824  10.0.2.15(lev-VirtualBox):40996 -> 151.101.1.91(miss):443
OPEN  TCP   pid=2995(Socket Thread) cookie=55238  10.0.2.15(lev-VirtualBox):59912 -> 151.101.193.91(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37240
OPEN  UDP   pid=514(systemd-resolve) cookie=58496  10.0.2.15(lev-VirtualBox):59333 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55495
OPEN  UDP   pid=514(systemd-resolve) cookie=58497  10.0.2.15(lev-VirtualBox):34339 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=58497  10.0.2.15(lev-VirtualBox):34339 -> 10.0.2.3(vboxdns):53  out=101B/2p in=280B/2p  age=20ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=58496  10.0.2.15(lev-VirtualBox):59333 -> 10.0.2.3(vboxdns):53  out=97B/2p in=276B/2p  age=48ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60543
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47626  out=90B/1p in=56B/1p  age=5.45s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=55824  10.0.2.15(lev-VirtualBox):40996 -> 151.101.1.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.551s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37240  out=114B/1p in=54B/1p  age=5.547s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=55238  10.0.2.15(lev-VirtualBox):59912 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.551s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55495  out=116B/1p in=56B/1p  age=5.543s reason=idle
OPEN  UDP   pid=671(avahi-daemon) cookie=7547  [fd00:0:0:0:bf53:d82b:4102:d10b](miss):5353 -> [ff02:0:0:0:0:0:0:fb](skip):5353
OPEN  UDP   pid=671(avahi-daemon) cookie=7546  10.0.2.15(lev-VirtualBox):5353 -> 224.0.0.251(skip):5353
OPEN  UDP   pid=671(avahi-daemon) cookie=7547  [ff02:0:0:0:0:0:0:fb%enp0s3](skip):5353 -> [fd00:0:0:0:bf53:d82b:4102:d10b](pending):5353
OPEN  UDP   pid=671(avahi-daemon) cookie=7546  10.0.2.15(lev-VirtualBox):5353 -> 10.0.2.15(lev-VirtualBox):5353
OPEN  UDP   pid=671(avahi-daemon) cookie=7546  127.0.0.1(localhost):5353 -> 127.0.0.1(localhost):5353
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55514
OPEN  UDP   pid=514(systemd-resolve) cookie=57622  10.0.2.15(lev-VirtualBox):47209 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=57622  10.0.2.15(lev-VirtualBox):47209 -> 10.0.2.3(vboxdns):53  out=191B/2p in=362B/2p  age=83ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60543  out=100B/1p in=55B/1p  age=5.071s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.08s reason=idle
CLOSE UDP   pid=671(avahi-daemon) cookie=7546  10.0.2.15(lev-VirtualBox):5353 -> 10.0.2.15(lev-VirtualBox):5353  out=0B/0p in=45B/1p  age=5.474s reason=idle
CLOSE UDP   pid=671(avahi-daemon) cookie=7547  [fd00:0:0:0:bf53:d82b:4102:d10b](lev-VirtualBox):5353 -> [ff02:0:0:0:0:0:0:fb](skip):5353  out=45B/1p in=0B/0p  age=5.474s reason=idle
CLOSE UDP   pid=671(avahi-daemon) cookie=7547  [ff02:0:0:0:0:0:0:fb%enp0s3](skip):5353 -> [fd00:0:0:0:bf53:d82b:4102:d10b](lev-VirtualBox):5353  out=0B/0p in=45B/1p  age=5.474s reason=idle
CLOSE UDP   pid=671(avahi-daemon) cookie=7546  127.0.0.1(localhost):5353 -> 127.0.0.1(localhost):5353  out=0B/0p in=45B/1p  age=5.474s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55514  out=163B/1p in=101B/1p  age=5.47s reason=idle
CLOSE UDP   pid=671(avahi-daemon) cookie=7546  10.0.2.15(lev-VirtualBox):5353 -> 224.0.0.251(skip):5353  out=90B/2p in=0B/0p  age=5.474s reason=idle
OPEN  UDP   pid=514(systemd-resolve) cookie=56112  10.0.2.15(lev-VirtualBox):51183 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56112  10.0.2.15(lev-VirtualBox):51183 -> 10.0.2.3(vboxdns):53  out=58B/1p in=742B/1p  age=53ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=57699  [fd00:0:0:0:10dd:174a:ea91:a63f](miss):41458 -> [2620:2d:4000:1:0:0:0:2a](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=57699  [fd00:0:0:0:10dd:174a:ea91:a63f](pending):41458 -> [2620:2d:4000:1:0:0:0:2a](is-content-cache-1.canonical.com):80  out=0B/0p in=0B/0p  age=6ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46351
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54972
OPEN  UDP   pid=514(systemd-resolve) cookie=56114  10.0.2.15(lev-VirtualBox):49567 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56114  10.0.2.15(lev-VirtualBox):49567 -> 10.0.2.3(vboxdns):53  out=191B/2p in=362B/2p  age=12ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46351  out=147B/1p in=101B/1p  age=5.239s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54972  out=163B/1p in=101B/1p  age=5.24s reason=idle
OPEN  ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=6.016s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=53988  10.0.2.15(lev-VirtualBox):48872 -> 34.107.243.93(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53623
OPEN  UDP   pid=514(systemd-resolve) cookie=56168  10.0.2.15(lev-VirtualBox):47477 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56168  10.0.2.15(lev-VirtualBox):47477 -> 10.0.2.3(vboxdns):53  out=55B/1p in=133B/1p  age=45ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=55824  10.0.2.15(lev-VirtualBox):40996 -> 151.101.1.91(no-ptr):443
OPEN  TCP   pid=2995(Socket Thread) cookie=55238  10.0.2.15(lev-VirtualBox):59912 -> 151.101.193.91(no-ptr):443
CLOSE TCP   pid=2995(Socket Thread) cookie=55238  10.0.2.15(lev-VirtualBox):59912 -> 151.101.193.91(no-ptr):443  out=63B/2p in=0B/0p  age=8ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=55824  10.0.2.15(lev-VirtualBox):40996 -> 151.101.1.91(no-ptr):443  out=63B/2p in=0B/0p  age=19ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53623  out=107B/1p in=55B/1p  age=5.357s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53988  10.0.2.15(lev-VirtualBox):48872 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=28B/1p in=24B/2p  age=5.365s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.125s reason=idle
OPEN  UDP   pid=514(systemd-resolve) cookie=56292  10.0.2.15(lev-VirtualBox):44173 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56292  10.0.2.15(lev-VirtualBox):44173 -> 10.0.2.3(vboxdns):53  out=58B/1p in=598B/1p  age=44ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=57311  10.0.2.15(lev-VirtualBox):46416 -> 185.125.190.96(miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51658
OPEN  UDP   pid=514(systemd-resolve) cookie=57834  10.0.2.15(lev-VirtualBox):33355 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=57834  10.0.2.15(lev-VirtualBox):33355 -> 10.0.2.3(vboxdns):53  out=56B/1p in=137B/1p  age=32ms reason=close()
CLOSE TCP   pid=674(NetworkManager) cookie=57311  10.0.2.15(lev-VirtualBox):46416 -> 185.125.190.96(ubuntu-content-cache-1.ps5.canonical.com):80  out=87B/1p in=189B/1p  age=569ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51658  out=110B/1p in=56B/1p  age=5.276s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.011s reason=idle
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=58780  127.0.0.1(localhost):46665 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=57862  127.0.0.1(localhost):37927 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46665
OPEN  UDP   pid=514(systemd-resolve) cookie=58781  10.0.2.15(lev-VirtualBox):60257 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37927
OPEN  UDP   pid=514(systemd-resolve) cookie=58782  10.0.2.15(lev-VirtualBox):59475 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=58783  10.0.2.15(lev-VirtualBox):45522 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=58782  10.0.2.15(lev-VirtualBox):59475 -> 10.0.2.3(vboxdns):53  out=44B/1p in=199B/1p  age=47ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=58783  10.0.2.15(lev-VirtualBox):45522 -> 10.0.2.3(vboxdns):53  out=44B/1p in=211B/1p  age=48ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=58781  10.0.2.15(lev-VirtualBox):60257 -> 10.0.2.3(vboxdns):53  out=44B/1p in=240B/1p  age=53ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=58784  10.0.2.15(lev-VirtualBox):40695 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=57862  127.0.0.1(localhost):37927 -> 127.0.0.53(dnsstub):53  out=88B/2p in=254B/2p  age=58ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=58786  127.0.0.1(localhost):41247 -> 127.0.0.53(dnsstub):53
OPEN  TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE UDP   pid=514(systemd-resolve) cookie=58784  10.0.2.15(lev-VirtualBox):40695 -> 10.0.2.3(vboxdns):53  out=54B/1p in=136B/1p  age=33ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41247
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=58780  127.0.0.1(localhost):46665 -> 127.0.0.53(dnsstub):53  out=44B/1p in=155B/1p  age=94ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=58786  127.0.0.1(localhost):41247 -> 127.0.0.53(dnsstub):53  out=44B/1p in=171B/1p  age=8ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=5906B/8p in=5375B/24p  age=5.544s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46665  out=155B/1p in=44B/1p  age=5.63s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37927  out=254B/2p in=88B/2p  age=5.628s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41247  out=171B/1p in=44B/1p  age=5.54s reason=idle
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60511  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):43458 -> [2001:4860:4860:0:0:0:0:8888](miss):443
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60512  127.0.0.1(localhost):61815 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35514
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):61815
OPEN  UDP   pid=514(systemd-resolve) cookie=58809  10.0.2.15(lev-VirtualBox):51361 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60513  127.0.0.1(localhost):15898 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):15898
OPEN  UDP   pid=514(systemd-resolve) cookie=57928  10.0.2.15(lev-VirtualBox):46496 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60514  127.0.0.1(localhost):48838 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48838
OPEN  UDP   pid=514(systemd-resolve) cookie=58810  10.0.2.15(lev-VirtualBox):60685 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=58809  10.0.2.15(lev-VirtualBox):51361 -> 10.0.2.3(vboxdns):53  out=48B/1p in=168B/1p  age=77ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=60515  10.0.2.15(lev-VirtualBox):47269 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=57928  10.0.2.15(lev-VirtualBox):46496 -> 10.0.2.3(vboxdns):53  out=48B/1p in=217B/1p  age=74ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=60513  127.0.0.1(localhost):15898 -> 127.0.0.53(dnsstub):53  out=37B/1p in=134B/1p  age=82ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=58810  10.0.2.15(lev-VirtualBox):60685 -> 10.0.2.3(vboxdns):53  out=48B/1p in=419B/1p  age=75ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=60514  127.0.0.1(localhost):48838 -> 127.0.0.53(dnsstub):53  out=37B/1p in=200B/1p  age=82ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=60512  127.0.0.1(localhost):61815 -> 127.0.0.53(dnsstub):53  out=37B/1p in=0B/0p  age=111ms reason=close()
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60516  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):46365 -> [2603:1061:14:34:0:0:0:1](miss):80
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60517  10.0.2.15(lev-VirtualBox):50763 -> 13.107.226.44(miss):80
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=60518  10.0.2.15(lev-VirtualBox):35889 -> 13.107.253.44(miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44856
OPEN  UDP   pid=514(systemd-resolve) cookie=60519  10.0.2.15(lev-VirtualBox):51822 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51207
OPEN  UDP   pid=514(systemd-resolve) cookie=60520  10.0.2.15(lev-VirtualBox):55255 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34171
OPEN  UDP   pid=514(systemd-resolve) cookie=60521  10.0.2.15(lev-VirtualBox):38857 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60515  10.0.2.15(lev-VirtualBox):47269 -> 10.0.2.3(vboxdns):53  out=50B/1p in=50B/1p  age=50ms reason=close()
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=60523  10.0.2.15(lev-VirtualBox):38792 -> 13.107.226.44(pending):443
CLOSE UDP   pid=514(systemd-resolve) cookie=60520  10.0.2.15(lev-VirtualBox):55255 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=31ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60521  10.0.2.15(lev-VirtualBox):38857 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=32ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60519  10.0.2.15(lev-VirtualBox):51822 -> 10.0.2.3(vboxdns):53  out=191B/2p in=420B/2p  age=56ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51207  out=141B/1p in=55B/1p  age=5.136s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35514  out=125B/1p in=101B/1p  age=5.257s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44856  out=187B/1p in=101B/1p  age=5.142s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):15898  out=134B/1p in=37B/1p  age=5.25s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48838  out=200B/1p in=37B/1p  age=5.246s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):61815  out=106B/1p in=37B/1p  age=5.255s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34171  out=141B/1p in=55B/1p  age=5.134s reason=idle
OPEN  ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=60523  10.0.2.15(lev-VirtualBox):38792 -> 13.107.226.44(no-ptr):443  out=3404B/6p in=67056B/17p  age=6.036s reason=idle
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59542  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):36385 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.012s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.182s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.515s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
OPEN  TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.844s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.479s reason=idle
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=60523  10.0.2.15(lev-VirtualBox):38792 -> 13.107.226.44(no-ptr):443
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=60523  10.0.2.15(lev-VirtualBox):38792 -> 13.107.226.44(no-ptr):443  out=0B/0p in=63B/1p  age=1ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=58237  127.0.0.1(localhost):52635 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52635
OPEN  UDP   pid=514(systemd-resolve) cookie=58239  10.0.2.15(lev-VirtualBox):56598 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=58240  10.0.2.15(lev-VirtualBox):44883 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=58240  10.0.2.15(lev-VirtualBox):44883 -> 10.0.2.3(vboxdns):53  out=44B/1p in=211B/1p  age=16ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443
CLOSE UDP   pid=514(systemd-resolve) cookie=58239  10.0.2.15(lev-VirtualBox):56598 -> 10.0.2.3(vboxdns):53  out=44B/1p in=199B/1p  age=49ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=58237  127.0.0.1(localhost):52635 -> 127.0.0.53(dnsstub):53  out=88B/2p in=254B/2p  age=57ms reason=close()
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=59029  127.0.0.1(localhost):55482 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=58244  127.0.0.1(localhost):58313 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55482
OPEN  UDP   pid=514(systemd-resolve) cookie=60709  10.0.2.15(lev-VirtualBox):36372 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=60710  10.0.2.15(lev-VirtualBox):57864 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58313
OPEN  UDP   pid=514(systemd-resolve) cookie=60711  10.0.2.15(lev-VirtualBox):35871 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=58246  127.0.0.1(localhost):36840 -> 127.0.0.53(dnsstub):53
OPEN  TCP   pid=2995(Socket Thread) cookie=60712  10.0.2.15(lev-VirtualBox):45276 -> 151.101.193.91(no-ptr):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36840
OPEN  UDP   pid=514(systemd-resolve) cookie=60713  10.0.2.15(lev-VirtualBox):58701 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=60714  10.0.2.15(lev-VirtualBox):33210 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60709  10.0.2.15(lev-VirtualBox):36372 -> 10.0.2.3(vboxdns):53  out=40B/1p in=283B/1p  age=41ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60710  10.0.2.15(lev-VirtualBox):57864 -> 10.0.2.3(vboxdns):53  out=40B/1p in=196B/1p  age=41ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60711  10.0.2.15(lev-VirtualBox):35871 -> 10.0.2.3(vboxdns):53  out=40B/1p in=119B/1p  age=38ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=59029  127.0.0.1(localhost):55482 -> 127.0.0.53(dnsstub):53  out=80B/2p in=336B/2p  age=55ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=58244  127.0.0.1(localhost):58313 -> 127.0.0.53(dnsstub):53  out=40B/1p in=97B/1p  age=55ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=58248  127.0.0.1(localhost):49992 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49992
CLOSE UDP   pid=514(systemd-resolve) cookie=60713  10.0.2.15(lev-VirtualBox):58701 -> 10.0.2.3(vboxdns):53  out=66B/1p in=291B/1p  age=33ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=58248  127.0.0.1(localhost):49992 -> 127.0.0.53(dnsstub):53  out=66B/1p in=166B/1p  age=8ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60714  10.0.2.15(lev-VirtualBox):33210 -> 10.0.2.3(vboxdns):53  out=66B/1p in=339B/1p  age=47ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=58246  127.0.0.1(localhost):36840 -> 127.0.0.53(dnsstub):53  out=132B/2p in=380B/2p  age=67ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=59652  10.0.2.15(lev-VirtualBox):44346 -> 64.233.165.119(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60260
OPEN  UDP   pid=514(systemd-resolve) cookie=60716  10.0.2.15(lev-VirtualBox):46986 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60716  10.0.2.15(lev-VirtualBox):46986 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=31ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=59648  10.0.2.15(lev-VirtualBox):53268 -> 64.233.165.119(lg-in-f119.1e100.net):443
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52635  out=254B/2p in=88B/2p  age=5.295s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58313  out=97B/1p in=40B/1p  age=5.96s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=60712  10.0.2.15(lev-VirtualBox):45276 -> 151.101.193.91(no-ptr):443  out=2870B/5p in=1835B/14p  age=5.948s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49992  out=166B/1p in=66B/1p  age=5.909s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55482  out=336B/2p in=80B/2p  age=5.971s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60260  out=90B/1p in=56B/1p  age=5.721s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36840  out=380B/2p in=132B/2p  age=5.943s reason=idle
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=59103  127.0.0.1(localhost):44672 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=58289  127.0.0.1(localhost):32941 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32941
OPEN  UDP   pid=514(systemd-resolve) cookie=60736  10.0.2.15(lev-VirtualBox):43458 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=60737  10.0.2.15(lev-VirtualBox):41082 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44672
OPEN  UDP   pid=514(systemd-resolve) cookie=60738  10.0.2.15(lev-VirtualBox):46222 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=59109  127.0.0.1(localhost):55320 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55320
OPEN  UDP   pid=514(systemd-resolve) cookie=60739  10.0.2.15(lev-VirtualBox):56642 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=60740  10.0.2.15(lev-VirtualBox):50516 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60737  10.0.2.15(lev-VirtualBox):41082 -> 10.0.2.3(vboxdns):53  out=62B/1p in=199B/1p  age=60ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60736  10.0.2.15(lev-VirtualBox):43458 -> 10.0.2.3(vboxdns):53  out=62B/1p in=187B/1p  age=68ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60738  10.0.2.15(lev-VirtualBox):46222 -> 10.0.2.3(vboxdns):53  out=62B/1p in=193B/1p  age=62ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=58289  127.0.0.1(localhost):32941 -> 127.0.0.53(dnsstub):53  out=124B/2p in=228B/2p  age=72ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=59103  127.0.0.1(localhost):44672 -> 127.0.0.53(dnsstub):53  out=62B/1p in=114B/1p  age=77ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=58292  127.0.0.1(localhost):60572 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60740  10.0.2.15(lev-VirtualBox):50516 -> 10.0.2.3(vboxdns):53  out=43B/1p in=211B/1p  age=39ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60572
CLOSE UDP   pid=514(systemd-resolve) cookie=60739  10.0.2.15(lev-VirtualBox):56642 -> 10.0.2.3(vboxdns):53  out=43B/1p in=223B/1p  age=49ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=59109  127.0.0.1(localhost):55320 -> 127.0.0.53(dnsstub):53  out=86B/2p in=294B/2p  age=78ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=58292  127.0.0.1(localhost):60572 -> 127.0.0.53(dnsstub):53  out=120B/2p in=164B/2p  age=27ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=59737  10.0.2.15(lev-VirtualBox):40758 -> 173.194.57.233(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33483
OPEN  TCP   pid=2995(Socket Thread) cookie=59738  10.0.2.15(lev-VirtualBox):40766 -> 173.194.57.233(pending):443
OPEN  UDP   pid=514(systemd-resolve) cookie=60741  10.0.2.15(lev-VirtualBox):50763 -> 10.0.2.3(vboxdns):53
CLOSE TCP   pid=2995(Socket Thread) cookie=59652  10.0.2.15(lev-VirtualBox):44346 -> 64.233.165.119(lg-in-f119.1e100.net):443  out=2680B/5p in=1951B/12p  age=6.725s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=60741  10.0.2.15(lev-VirtualBox):50763 -> 10.0.2.3(vboxdns):53  out=56B/1p in=121B/1p  age=34ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=59739  10.0.2.15(lev-VirtualBox):43530 -> 108.177.14.105(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34997
OPEN  UDP   pid=514(systemd-resolve) cookie=60742  10.0.2.15(lev-VirtualBox):37231 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60742  10.0.2.15(lev-VirtualBox):37231 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=37ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=59733  10.0.2.15(lev-VirtualBox):49650 -> 173.194.57.233(dfw41s01-in-f9.1e100.net):443
CLOSE UDP   pid=2995(Socket Thread) cookie=59648  10.0.2.15(lev-VirtualBox):53268 -> 64.233.165.119(lg-in-f119.1e100.net):443  out=5264B/10p in=6527B/11p  age=10.778s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44672  out=114B/1p in=62B/1p  age=5.155s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60572  out=164B/2p in=120B/2p  age=5.073s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55320  out=294B/2p in=86B/2p  age=5.127s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32941  out=228B/2p in=124B/2p  age=5.161s reason=idle
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=59175  127.0.0.1(localhost):43018 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=58332  127.0.0.1(localhost):36640 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43018
OPEN  UDP   pid=514(systemd-resolve) cookie=60759  10.0.2.15(lev-VirtualBox):47799 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36640
OPEN  UDP   pid=514(systemd-resolve) cookie=60760  10.0.2.15(lev-VirtualBox):33955 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=60761  10.0.2.15(lev-VirtualBox):48782 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60759  10.0.2.15(lev-VirtualBox):47799 -> 10.0.2.3(vboxdns):53  out=40B/1p in=66B/1p  age=44ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=59175  127.0.0.1(localhost):43018 -> 127.0.0.53(dnsstub):53  out=40B/1p in=55B/1p  age=47ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60760  10.0.2.15(lev-VirtualBox):33955 -> 10.0.2.3(vboxdns):53  out=40B/1p in=148B/1p  age=42ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60761  10.0.2.15(lev-VirtualBox):48782 -> 10.0.2.3(vboxdns):53  out=40B/1p in=196B/1p  age=73ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=58332  127.0.0.1(localhost):36640 -> 127.0.0.53(dnsstub):53  out=80B/2p in=256B/2p  age=84ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=58333  10.0.2.15(lev-VirtualBox):42550 -> 64.233.164.93(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44905
OPEN  UDP   pid=514(systemd-resolve) cookie=60764  10.0.2.15(lev-VirtualBox):60214 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60764  10.0.2.15(lev-VirtualBox):60214 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=31ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=59737  10.0.2.15(lev-VirtualBox):40758 -> 173.194.57.233(dfw41s01-in-f9.1e100.net):443  out=2006B/3p in=4624B/6p  age=5.418s reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=59738  10.0.2.15(lev-VirtualBox):40766 -> 173.194.57.233(dfw41s01-in-f9.1e100.net):443  out=2006B/3p in=4624B/6p  age=5.415s reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33483  out=94B/1p in=56B/1p  age=6.02s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=59739  10.0.2.15(lev-VirtualBox):43530 -> 108.177.14.105(lt-in-f105.1e100.net):443  out=5040B/3p in=4902B/6p  age=6.018s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34997  out=90B/1p in=56B/1p  age=5.862s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=59733  10.0.2.15(lev-VirtualBox):49650 -> 173.194.57.233(dfw41s01-in-f9.1e100.net):443  out=9801B/15p in=7895B/15p  age=7.044s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=9268B/21p in=2244777B/2010p  age=16.256s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=58333  10.0.2.15(lev-VirtualBox):42550 -> 64.233.164.93(lf-in-f93.1e100.net):443  out=2427B/6p in=10735B/20p  age=5.894s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43018  out=55B/1p in=40B/1p  age=5.994s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44905  out=88B/1p in=55B/1p  age=5.892s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36640  out=256B/2p in=80B/2p  age=5.989s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59792  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):57782 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59793  127.0.0.1(localhost):1801 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):1801
OPEN  UDP   pid=514(systemd-resolve) cookie=60798  10.0.2.15(lev-VirtualBox):40026 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38281
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59794  127.0.0.1(localhost):38281 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59795  127.0.0.1(localhost):6362 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=60799  10.0.2.15(lev-VirtualBox):54003 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):6362
OPEN  UDP   pid=514(systemd-resolve) cookie=60800  10.0.2.15(lev-VirtualBox):48175 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60799  10.0.2.15(lev-VirtualBox):54003 -> 10.0.2.3(vboxdns):53  out=43B/1p in=127B/1p  age=16ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=59794  127.0.0.1(localhost):38281 -> 127.0.0.53(dnsstub):53  out=32B/1p in=97B/1p  age=19ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60798  10.0.2.15(lev-VirtualBox):40026 -> 10.0.2.3(vboxdns):53  out=43B/1p in=127B/1p  age=48ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=59793  127.0.0.1(localhost):1801 -> 127.0.0.53(dnsstub):53  out=32B/1p in=97B/1p  age=69ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60800  10.0.2.15(lev-VirtualBox):48175 -> 10.0.2.3(vboxdns):53  out=43B/1p in=73B/1p  age=86ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=59795  127.0.0.1(localhost):6362 -> 127.0.0.53(dnsstub):53  out=32B/1p in=48B/1p  age=94ms reason=close()
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=59796  10.0.2.15(lev-VirtualBox):47914 -> 140.82.121.6(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48712
OPEN  UDP   pid=514(systemd-resolve) cookie=60801  10.0.2.15(lev-VirtualBox):50474 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60801  10.0.2.15(lev-VirtualBox):50474 -> 10.0.2.3(vboxdns):53  out=54B/1p in=123B/1p  age=33ms reason=close()
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59192  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):36591 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59193  127.0.0.1(localhost):54398 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59194  127.0.0.1(localhost):36948 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54398
OPEN  UDP   pid=514(systemd-resolve) cookie=60802  10.0.2.15(lev-VirtualBox):38313 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=59195  127.0.0.1(localhost):59022 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36948
OPEN  UDP   pid=514(systemd-resolve) cookie=60803  10.0.2.15(lev-VirtualBox):55236 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59022
OPEN  UDP   pid=514(systemd-resolve) cookie=60804  10.0.2.15(lev-VirtualBox):48722 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60804  10.0.2.15(lev-VirtualBox):48722 -> 10.0.2.3(vboxdns):53  out=61B/1p in=295B/1p  age=42ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=59195  127.0.0.1(localhost):59022 -> 127.0.0.53(dnsstub):53  out=50B/1p in=170B/1p  age=48ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60803  10.0.2.15(lev-VirtualBox):55236 -> 10.0.2.3(vboxdns):53  out=61B/1p in=339B/1p  age=48ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=60805  10.0.2.15(lev-VirtualBox):46270 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60802  10.0.2.15(lev-VirtualBox):38313 -> 10.0.2.3(vboxdns):53  out=61B/1p in=339B/1p  age=55ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=60806  10.0.2.15(lev-VirtualBox):46263 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60805  10.0.2.15(lev-VirtualBox):46270 -> 10.0.2.3(vboxdns):53  out=71B/1p in=173B/1p  age=30ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=59194  127.0.0.1(localhost):36948 -> 127.0.0.53(dnsstub):53  out=50B/1p in=215B/1p  age=85ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=60806  10.0.2.15(lev-VirtualBox):46263 -> 10.0.2.3(vboxdns):53  out=71B/1p in=173B/1p  age=32ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=59193  127.0.0.1(localhost):54398 -> 127.0.0.53(dnsstub):53  out=50B/1p in=215B/1p  age=98ms reason=close()
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=60807  10.0.2.15(lev-VirtualBox):36224 -> 20.42.65.90(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52421
OPEN  UDP   pid=514(systemd-resolve) cookie=60808  10.0.2.15(lev-VirtualBox):35843 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60808  10.0.2.15(lev-VirtualBox):35843 -> 10.0.2.3(vboxdns):53  out=95B/2p in=326B/2p  age=35ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.853s reason=idle
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=61531  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):50101 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):6362  out=48B/1p in=32B/1p  age=5.612s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38281  out=97B/1p in=32B/1p  age=5.615s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48712  out=98B/1p in=54B/1p  age=5.518s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):1801  out=97B/1p in=32B/1p  age=5.615s reason=idle
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=59796  10.0.2.15(lev-VirtualBox):47914 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=2542B/6p in=6042B/4p  age=6.521s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59022  out=170B/1p in=50B/1p  age=5.633s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36948  out=215B/1p in=50B/1p  age=5.635s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52421  out=139B/1p in=53B/1p  age=5.538s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54398  out=215B/1p in=50B/1p  age=5.638s reason=idle
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=60807  10.0.2.15(lev-VirtualBox):36224 -> 20.42.65.90(no-ptr):443  out=5747B/7p in=6943B/6p  age=6.547s reason=idle
OPEN  UDP   pid=514(systemd-resolve) cookie=60879  10.0.2.15(lev-VirtualBox):49010 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=60879  10.0.2.15(lev-VirtualBox):49010 -> 10.0.2.3(vboxdns):53  out=58B/1p in=742B/1p  age=47ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=59854  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):40522 -> [2620:2d:4000:1:0:0:0:98](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=59854  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):40522 -> [2620:2d:4000:1:0:0:0:98](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39743
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39743  out=155B/1p in=101B/1p  age=5.254s reason=idle
OPEN  ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
OPEN  TCP   pid=2995(Socket Thread) cookie=59652  10.0.2.15(lev-VirtualBox):44346 -> 64.233.165.119(lg-in-f119.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=60712  10.0.2.15(lev-VirtualBox):45276 -> 151.101.193.91(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60942
OPEN  UDP   pid=514(systemd-resolve) cookie=59302  10.0.2.15(lev-VirtualBox):60805 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=59302  10.0.2.15(lev-VirtualBox):60805 -> 10.0.2.3(vboxdns):53  out=101B/2p in=280B/2p  age=43ms reason=close()
CLOSE ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.968s reason=idle
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=59796  10.0.2.15(lev-VirtualBox):47914 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=59796  10.0.2.15(lev-VirtualBox):47914 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=0B/0p in=39B/1p  age=2ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60942  out=116B/1p in=56B/1p  age=5.782s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=59652  10.0.2.15(lev-VirtualBox):44346 -> 64.233.165.119(lg-in-f119.1e100.net):443  out=39B/1p in=39B/2p  age=5.786s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=60712  10.0.2.15(lev-VirtualBox):45276 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.786s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=58333  10.0.2.15(lev-VirtualBox):42550 -> 64.233.164.93(lf-in-f93.1e100.net):443
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=59317  127.0.0.1(localhost):53315 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53315
OPEN  UDP   pid=514(systemd-resolve) cookie=59318  10.0.2.15(lev-VirtualBox):55193 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=59319  10.0.2.15(lev-VirtualBox):34262 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=59319  10.0.2.15(lev-VirtualBox):34262 -> 10.0.2.3(vboxdns):53  out=44B/1p in=211B/1p  age=8ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=59318  10.0.2.15(lev-VirtualBox):55193 -> 10.0.2.3(vboxdns):53  out=44B/1p in=199B/1p  age=47ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=59317  127.0.0.1(localhost):53315 -> 127.0.0.53(dnsstub):53  out=88B/2p in=254B/2p  age=51ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=53988  10.0.2.15(lev-VirtualBox):48872 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=58333  10.0.2.15(lev-VirtualBox):42550 -> 64.233.164.93(lf-in-f93.1e100.net):443  out=39B/1p in=39B/2p  age=5.98s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53315  out=254B/2p in=88B/2p  age=5.94s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=57865  10.0.2.15(lev-VirtualBox):59094 -> 64.233.164.198(lf-in-f198.1e100.net):443  out=6187B/5p in=242B/10p  age=10.984s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53988  10.0.2.15(lev-VirtualBox):48872 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=28B/1p in=24B/2p  age=5.088s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.813s reason=idle
