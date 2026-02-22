
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
[sudo] password for lev: 
2026/02/22 21:03:33 dotenv loaded: .env
2026/02/22 21:03:33.899320 hostsPrefill: added=7 from /etc/hosts
2026/02/22 21:03:34.348577 pprof on :6060
2026/02/22 21:03:34.355637 perf ring per-cpu=512KB total~=2MB cpus=4
2026/02/22 21:03:34.359154 OPEN/CLOSE (TCP/UDP/ICMP) + PTR + skb-hint. Ctrl+C to exit
OPEN  UDP   pid=514(systemd-resolve) cookie=50855  10.0.2.15(miss):44484 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56500
OPEN  UDP   pid=514(systemd-resolve) cookie=50856  10.0.2.15(pending):44669 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50855  10.0.2.15(pending):44484 -> 10.0.2.3(vboxdns):53  out=58B/1p in=598B/1p  age=43ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=51532  10.0.2.15(pending):42534 -> 185.125.190.97(miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52557
OPEN  UDP   pid=514(systemd-resolve) cookie=50864  10.0.2.15(pending):59543 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50856  10.0.2.15(pending):44669 -> 10.0.2.3(vboxdns):53  out=91B/2p in=91B/2p  age=69ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50864  10.0.2.15(lev-VirtualBox):59543 -> 10.0.2.3(vboxdns):53  out=56B/1p in=137B/1p  age=35ms reason=close()
CLOSE TCP   pid=674(NetworkManager) cookie=51532  10.0.2.15(lev-VirtualBox):42534 -> 185.125.190.97(ubuntu-content-cache-2.ps5.canonical.com):80  out=87B/1p in=189B/1p  age=283ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=49148  127.0.0.1(localhost):47245 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47245
OPEN  UDP   pid=514(systemd-resolve) cookie=50865  10.0.2.15(lev-VirtualBox):56929 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #5) cookie=51535  127.0.0.1(localhost):35446 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50866  10.0.2.15(lev-VirtualBox):44792 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35446
OPEN  UDP   pid=514(systemd-resolve) cookie=50867  10.0.2.15(lev-VirtualBox):55004 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50868  10.0.2.15(lev-VirtualBox):45104 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #9) cookie=49150  127.0.0.1(localhost):39692 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39692
OPEN  UDP   pid=514(systemd-resolve) cookie=50869  10.0.2.15(lev-VirtualBox):33431 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50870  10.0.2.15(lev-VirtualBox):48168 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #11) cookie=50135  127.0.0.1(localhost):51248 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51248
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=50871  127.0.0.1(localhost):54495 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Resolver #6) cookie=50136  127.0.0.1(localhost):45673 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54495
OPEN  UDP   pid=514(systemd-resolve) cookie=49151  10.0.2.15(lev-VirtualBox):47757 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #10) cookie=50137  127.0.0.1(localhost):51466 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50866  10.0.2.15(lev-VirtualBox):44792 -> 10.0.2.3(vboxdns):53  out=44B/1p in=320B/1p  age=43ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51466
OPEN  UDP   pid=514(systemd-resolve) cookie=49152  10.0.2.15(lev-VirtualBox):58451 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50865  10.0.2.15(lev-VirtualBox):56929 -> 10.0.2.3(vboxdns):53  out=44B/1p in=272B/1p  age=56ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50867  10.0.2.15(lev-VirtualBox):55004 -> 10.0.2.3(vboxdns):53  out=44B/1p in=199B/1p  age=46ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=49148  127.0.0.1(localhost):47245 -> 127.0.0.53(dnsstub):53  out=88B/2p in=346B/2p  age=60ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50868  10.0.2.15(lev-VirtualBox):45104 -> 10.0.2.3(vboxdns):53  out=44B/1p in=211B/1p  age=34ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45673
OPEN  UDP   pid=514(systemd-resolve) cookie=50138  10.0.2.15(lev-VirtualBox):41142 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2995(DNS Resolver #5) cookie=51535  127.0.0.1(localhost):35446 -> 127.0.0.53(dnsstub):53  out=88B/2p in=254B/2p  age=60ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=49152  10.0.2.15(lev-VirtualBox):58451 -> 10.0.2.3(vboxdns):53  out=49B/1p in=196B/1p  age=18ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50139  10.0.2.15(lev-VirtualBox):54522 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #5) cookie=51539  127.0.0.1(localhost):35196 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35196
OPEN  UDP   pid=514(systemd-resolve) cookie=50140  10.0.2.15(lev-VirtualBox):34392 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=52226  127.0.0.1(localhost):49227 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50141  10.0.2.15(lev-VirtualBox):39586 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49227
OPEN  UDP   pid=514(systemd-resolve) cookie=50142  10.0.2.15(lev-VirtualBox):32868 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50870  10.0.2.15(lev-VirtualBox):48168 -> 10.0.2.3(vboxdns):53  out=49B/1p in=196B/1p  age=59ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50143  10.0.2.15(lev-VirtualBox):52126 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50869  10.0.2.15(lev-VirtualBox):33431 -> 10.0.2.3(vboxdns):53  out=49B/1p in=157B/1p  age=63ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #11) cookie=50135  127.0.0.1(localhost):51248 -> 127.0.0.53(dnsstub):53  out=49B/1p in=98B/1p  age=61ms reason=close()
OPEN  UDP   pid=2995(DNS Res~ver #11) cookie=50145  127.0.0.1(localhost):41301 -> 127.0.0.53(dnsstub):53
OPEN  TCP   pid=2995(Socket Thread) cookie=52227  10.0.2.15(lev-VirtualBox):53638 -> 140.82.113.21(miss):443
CLOSE UDP   pid=514(systemd-resolve) cookie=49151  10.0.2.15(lev-VirtualBox):47757 -> 10.0.2.3(vboxdns):53  out=44B/1p in=191B/1p  age=62ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50146  10.0.2.15(lev-VirtualBox):43879 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41301
OPEN  UDP   pid=514(systemd-resolve) cookie=50147  10.0.2.15(lev-VirtualBox):37082 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50148  10.0.2.15(lev-VirtualBox):39478 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49413
OPEN  UDP   pid=514(systemd-resolve) cookie=50149  10.0.2.15(lev-VirtualBox):35870 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50138  10.0.2.15(lev-VirtualBox):41142 -> 10.0.2.3(vboxdns):53  out=54B/1p in=136B/1p  age=86ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50139  10.0.2.15(lev-VirtualBox):54522 -> 10.0.2.3(vboxdns):53  out=58B/1p in=142B/1p  age=81ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50143  10.0.2.15(lev-VirtualBox):52126 -> 10.0.2.3(vboxdns):53  out=58B/1p in=155B/1p  age=55ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #6) cookie=50136  127.0.0.1(localhost):45673 -> 127.0.0.53(dnsstub):53  out=44B/1p in=183B/1p  age=109ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #6) cookie=50150  127.0.0.1(localhost):39260 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=2995(DNS Res~ver #10) cookie=50137  127.0.0.1(localhost):51466 -> 127.0.0.53(dnsstub):53  out=49B/1p in=147B/1p  age=107ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39260
OPEN  UDP   pid=514(systemd-resolve) cookie=50151  10.0.2.15(lev-VirtualBox):54447 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2995(DNS Resolver #9) cookie=49150  127.0.0.1(localhost):39692 -> 127.0.0.53(dnsstub):53  out=98B/2p in=329B/2p  age=126ms reason=close()
OPEN  UDP   pid=2995(DNS Res~ver #10) cookie=51541  127.0.0.1(localhost):59732 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59732
OPEN  UDP   pid=514(systemd-resolve) cookie=50152  10.0.2.15(lev-VirtualBox):59601 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50153  10.0.2.15(lev-VirtualBox):49807 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #9) cookie=52229  127.0.0.1(localhost):60143 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60143
OPEN  UDP   pid=514(systemd-resolve) cookie=50154  10.0.2.15(lev-VirtualBox):58670 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50142  10.0.2.15(lev-VirtualBox):32868 -> 10.0.2.3(vboxdns):53  out=48B/1p in=184B/1p  age=115ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50155  10.0.2.15(lev-VirtualBox):38245 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50140  10.0.2.15(lev-VirtualBox):34392 -> 10.0.2.3(vboxdns):53  out=48B/1p in=255B/1p  age=121ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50148  10.0.2.15(lev-VirtualBox):39478 -> 10.0.2.3(vboxdns):53  out=47B/1p in=231B/1p  age=94ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50146  10.0.2.15(lev-VirtualBox):43879 -> 10.0.2.3(vboxdns):53  out=56B/1p in=147B/1p  age=98ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=50871  127.0.0.1(localhost):54495 -> 127.0.0.53(dnsstub):53  out=44B/1p in=143B/1p  age=160ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50147  10.0.2.15(lev-VirtualBox):37082 -> 10.0.2.3(vboxdns):53  out=47B/1p in=183B/1p  age=99ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=50873  127.0.0.1(localhost):42470 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=2995(DNS Res~ver #11) cookie=50145  127.0.0.1(localhost):41301 -> 127.0.0.53(dnsstub):53  out=94B/2p in=270B/2p  age=105ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50149  10.0.2.15(lev-VirtualBox):35870 -> 10.0.2.3(vboxdns):53  out=55B/1p in=126B/1p  age=96ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50151  10.0.2.15(lev-VirtualBox):54447 -> 10.0.2.3(vboxdns):53  out=47B/1p in=149B/1p  age=54ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #6) cookie=50150  127.0.0.1(localhost):39260 -> 127.0.0.53(dnsstub):53  out=47B/1p in=128B/1p  age=57ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42470
OPEN  UDP   pid=514(systemd-resolve) cookie=50157  10.0.2.15(lev-VirtualBox):33106 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50158  10.0.2.15(lev-VirtualBox):56055 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #6) cookie=50159  127.0.0.1(localhost):44894 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44894
OPEN  UDP   pid=514(systemd-resolve) cookie=50160  10.0.2.15(lev-VirtualBox):59672 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #11) cookie=50162  127.0.0.1(localhost):46303 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46303
OPEN  UDP   pid=514(systemd-resolve) cookie=50163  10.0.2.15(lev-VirtualBox):48164 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50164  10.0.2.15(lev-VirtualBox):36232 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50154  10.0.2.15(lev-VirtualBox):58670 -> 10.0.2.3(vboxdns):53  out=46B/1p in=185B/1p  age=86ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50165  10.0.2.15(lev-VirtualBox):52698 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50141  10.0.2.15(lev-VirtualBox):39586 -> 10.0.2.3(vboxdns):53  out=48B/1p in=303B/1p  age=171ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #5) cookie=51539  127.0.0.1(localhost):35196 -> 127.0.0.53(dnsstub):53  out=96B/2p in=344B/2p  age=176ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50152  10.0.2.15(lev-VirtualBox):59601 -> 10.0.2.3(vboxdns):53  out=46B/1p in=129B/1p  age=96ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #5) cookie=51544  127.0.0.1(localhost):37297 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37297
OPEN  UDP   pid=514(systemd-resolve) cookie=50166  10.0.2.15(lev-VirtualBox):60075 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #12) cookie=51545  127.0.0.1(localhost):47908 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=50168  127.0.0.1(localhost):32955 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47908
OPEN  UDP   pid=514(systemd-resolve) cookie=50169  10.0.2.15(lev-VirtualBox):39503 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32955
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=50168  127.0.0.1(localhost):32955 -> 127.0.0.53(dnsstub):53  out=88B/2p in=282B/2p  age=7ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50157  10.0.2.15(lev-VirtualBox):33106 -> 10.0.2.3(vboxdns):53  out=43B/1p in=240B/1p  age=86ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50158  10.0.2.15(lev-VirtualBox):56055 -> 10.0.2.3(vboxdns):53  out=43B/1p in=173B/1p  age=84ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50172  10.0.2.15(lev-VirtualBox):60391 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50160  10.0.2.15(lev-VirtualBox):59672 -> 10.0.2.3(vboxdns):53  out=43B/1p in=173B/1p  age=77ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=50173  10.0.2.15(lev-VirtualBox):39340 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50163  10.0.2.15(lev-VirtualBox):48164 -> 10.0.2.3(vboxdns):53  out=39B/1p in=65B/1p  age=68ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50155  10.0.2.15(lev-VirtualBox):38245 -> 10.0.2.3(vboxdns):53  out=51B/1p in=132B/1p  age=108ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50169  10.0.2.15(lev-VirtualBox):39503 -> 10.0.2.3(vboxdns):53  out=54B/1p in=136B/1p  age=24ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #12) cookie=51545  127.0.0.1(localhost):47908 -> 127.0.0.53(dnsstub):53  out=44B/1p in=183B/1p  age=26ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=52226  127.0.0.1(localhost):49227 -> 127.0.0.53(dnsstub):53  out=48B/1p in=145B/1p  age=230ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=52230  10.0.2.15(lev-VirtualBox):33604 -> 64.233.161.198(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53191
OPEN  UDP   pid=514(systemd-resolve) cookie=50174  10.0.2.15(lev-VirtualBox):49652 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50164  10.0.2.15(lev-VirtualBox):36232 -> 10.0.2.3(vboxdns):53  out=39B/1p in=123B/1p  age=112ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50165  10.0.2.15(lev-VirtualBox):52698 -> 10.0.2.3(vboxdns):53  out=47B/1p in=137B/1p  age=103ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #11) cookie=50162  127.0.0.1(localhost):46303 -> 127.0.0.53(dnsstub):53  out=78B/2p in=159B/2p  age=123ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50166  10.0.2.15(lev-VirtualBox):60075 -> 10.0.2.3(vboxdns):53  out=39B/1p in=123B/1p  age=81ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #9) cookie=52229  127.0.0.1(localhost):60143 -> 127.0.0.53(dnsstub):53  out=46B/1p in=126B/1p  age=199ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #5) cookie=51544  127.0.0.1(localhost):37297 -> 127.0.0.53(dnsstub):53  out=39B/1p in=104B/1p  age=82ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50153  10.0.2.15(lev-VirtualBox):49807 -> 10.0.2.3(vboxdns):53  out=46B/1p in=141B/1p  age=206ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #10) cookie=51541  127.0.0.1(localhost):59732 -> 127.0.0.53(dnsstub):53  out=92B/2p in=194B/2p  age=209ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50172  10.0.2.15(lev-VirtualBox):60391 -> 10.0.2.3(vboxdns):53  out=50B/1p in=131B/1p  age=78ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=50873  127.0.0.1(localhost):42470 -> 127.0.0.53(dnsstub):53  out=86B/2p in=278B/2p  age=173ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50174  10.0.2.15(lev-VirtualBox):49652 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=62ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50173  10.0.2.15(lev-VirtualBox):39340 -> 10.0.2.3(vboxdns):53  out=50B/1p in=131B/1p  age=84ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #6) cookie=50159  127.0.0.1(localhost):44894 -> 127.0.0.53(dnsstub):53  out=43B/1p in=136B/1p  age=170ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=52232  10.0.2.15(lev-VirtualBox):33608 -> 64.233.161.198(lh-in-f198.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52233  10.0.2.15(lev-VirtualBox):33612 -> 64.233.161.198(lh-in-f198.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52232  10.0.2.15(lev-VirtualBox):33608 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=2097B/4p in=8363B/12p  age=306ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=52233  10.0.2.15(lev-VirtualBox):33612 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=2097B/4p in=8361B/12p  age=316ms reason=close()
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=50176  127.0.0.1(localhost):46103 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46103
OPEN  UDP   pid=514(systemd-resolve) cookie=53249  10.0.2.15(lev-VirtualBox):37064 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=53250  10.0.2.15(lev-VirtualBox):33801 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53249  10.0.2.15(lev-VirtualBox):37064 -> 10.0.2.3(vboxdns):53  out=59B/1p in=209B/1p  age=28ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53250  10.0.2.15(lev-VirtualBox):33801 -> 10.0.2.3(vboxdns):53  out=59B/1p in=259B/1p  age=29ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=53251  10.0.2.15(lev-VirtualBox):42197 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53251  10.0.2.15(lev-VirtualBox):42197 -> 10.0.2.3(vboxdns):53  out=74B/1p in=185B/1p  age=33ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=50176  127.0.0.1(localhost):46103 -> 127.0.0.53(dnsstub):53  out=118B/2p in=331B/2p  age=69ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=50875  *(any):46422 -> 64.233.161.198(lh-in-f198.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42327
OPEN  UDP   pid=514(systemd-resolve) cookie=53254  10.0.2.15(lev-VirtualBox):54562 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53254  10.0.2.15(lev-VirtualBox):54562 -> 10.0.2.3(vboxdns):53  out=56B/1p in=136B/1p  age=44ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=52230  10.0.2.15(lev-VirtualBox):33604 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=2739B/8p in=87801B/181p  age=973ms reason=close()
OPEN  UDP   pid=2995(DNS Res~ver #12) cookie=51558  127.0.0.1(localhost):57611 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33008
OPEN  UDP   pid=514(systemd-resolve) cookie=53255  10.0.2.15(lev-VirtualBox):49576 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=52256  127.0.0.1(localhost):33008 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=53256  10.0.2.15(lev-VirtualBox):55904 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57611
OPEN  UDP   pid=514(systemd-resolve) cookie=53257  10.0.2.15(lev-VirtualBox):44555 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53255  10.0.2.15(lev-VirtualBox):49576 -> 10.0.2.3(vboxdns):53  out=40B/1p in=337B/1p  age=36ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53256  10.0.2.15(lev-VirtualBox):55904 -> 10.0.2.3(vboxdns):53  out=40B/1p in=196B/1p  age=32ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=52256  127.0.0.1(localhost):33008 -> 127.0.0.53(dnsstub):53  out=80B/2p in=368B/2p  age=38ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53257  10.0.2.15(lev-VirtualBox):44555 -> 10.0.2.3(vboxdns):53  out=40B/1p in=119B/1p  age=36ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #12) cookie=51558  127.0.0.1(localhost):57611 -> 127.0.0.53(dnsstub):53  out=40B/1p in=97B/1p  age=55ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #9) cookie=52260  127.0.0.1(localhost):53134 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53134
OPEN  UDP   pid=514(systemd-resolve) cookie=53258  10.0.2.15(lev-VirtualBox):53668 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=53259  10.0.2.15(lev-VirtualBox):44435 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #11) cookie=53260  127.0.0.1(localhost):53533 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53533
OPEN  UDP   pid=514(systemd-resolve) cookie=53261  10.0.2.15(lev-VirtualBox):58188 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53258  10.0.2.15(lev-VirtualBox):53668 -> 10.0.2.3(vboxdns):53  out=49B/1p in=85B/1p  age=30ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53259  10.0.2.15(lev-VirtualBox):44435 -> 10.0.2.3(vboxdns):53  out=49B/1p in=97B/1p  age=30ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53261  10.0.2.15(lev-VirtualBox):58188 -> 10.0.2.3(vboxdns):53  out=49B/1p in=133B/1p  age=31ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #9) cookie=52260  127.0.0.1(localhost):53134 -> 127.0.0.53(dnsstub):53  out=98B/2p in=142B/2p  age=43ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #11) cookie=53260  127.0.0.1(localhost):53533 -> 127.0.0.53(dnsstub):53  out=49B/1p in=106B/1p  age=37ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=53266  10.0.2.15(lev-VirtualBox):44210 -> 142.251.1.119(miss):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53267  10.0.2.15(lev-VirtualBox):44220 -> 142.251.1.119(pending):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60657
OPEN  UDP   pid=514(systemd-resolve) cookie=53269  10.0.2.15(lev-VirtualBox):41163 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52557  out=110B/1p in=56B/1p  age=5.58s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56500  out=113B/1p in=51B/1p  age=5.629s reason=idle
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35551
OPEN  UDP   pid=514(systemd-resolve) cookie=53270  10.0.2.15(lev-VirtualBox):53282 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53269  10.0.2.15(lev-VirtualBox):41163 -> 10.0.2.3(vboxdns):53  out=55B/1p in=115B/1p  age=44ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53270  10.0.2.15(lev-VirtualBox):53282 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=36ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443
OPEN  UDP   pid=2995(Socket Thread) cookie=53262  10.0.2.15(lev-VirtualBox):53747 -> 64.233.164.95(lf-in-f95.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=53267  10.0.2.15(lev-VirtualBox):44220 -> 142.251.1.119(lb-in-f119.1e100.net):443  out=2093B/4p in=6296B/12p  age=638ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #5) cookie=51588  127.0.0.1(localhost):44983 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Res~ver #10) cookie=51590  127.0.0.1(localhost):39283 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44983
OPEN  UDP   pid=514(systemd-resolve) cookie=53273  10.0.2.15(lev-VirtualBox):44782 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=53274  10.0.2.15(lev-VirtualBox):53902 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39283
OPEN  UDP   pid=514(systemd-resolve) cookie=53275  10.0.2.15(lev-VirtualBox):33964 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=50915  127.0.0.1(localhost):60508 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60508
OPEN  UDP   pid=514(systemd-resolve) cookie=53276  10.0.2.15(lev-VirtualBox):45036 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=53277  10.0.2.15(lev-VirtualBox):49614 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Resolver #6) cookie=53278  127.0.0.1(localhost):41484 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41484
OPEN  UDP   pid=514(systemd-resolve) cookie=53279  10.0.2.15(lev-VirtualBox):35615 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53273  10.0.2.15(lev-VirtualBox):44782 -> 10.0.2.3(vboxdns):53  out=46B/1p in=79B/1p  age=47ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53274  10.0.2.15(lev-VirtualBox):53902 -> 10.0.2.3(vboxdns):53  out=46B/1p in=91B/1p  age=47ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53275  10.0.2.15(lev-VirtualBox):33964 -> 10.0.2.3(vboxdns):53  out=46B/1p in=88B/1p  age=38ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #5) cookie=51588  127.0.0.1(localhost):44983 -> 127.0.0.53(dnsstub):53  out=92B/2p in=136B/2p  age=58ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #10) cookie=51590  127.0.0.1(localhost):39283 -> 127.0.0.53(dnsstub):53  out=46B/1p in=71B/1p  age=60ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=50915  127.0.0.1(localhost):60508 -> 127.0.0.53(dnsstub):53  out=96B/2p in=140B/2p  age=36ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53276  10.0.2.15(lev-VirtualBox):45036 -> 10.0.2.3(vboxdns):53  out=48B/1p in=83B/1p  age=32ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53277  10.0.2.15(lev-VirtualBox):49614 -> 10.0.2.3(vboxdns):53  out=48B/1p in=95B/1p  age=32ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=53279  10.0.2.15(lev-VirtualBox):35615 -> 10.0.2.3(vboxdns):53  out=48B/1p in=128B/1p  age=35ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #6) cookie=53278  127.0.0.1(localhost):41484 -> 127.0.0.53(dnsstub):53  out=48B/1p in=98B/1p  age=46ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=52277  10.0.2.15(lev-VirtualBox):37062 -> 142.251.1.119(lb-in-f119.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58850
OPEN  UDP   pid=514(systemd-resolve) cookie=53281  10.0.2.15(lev-VirtualBox):41303 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=53281  10.0.2.15(lev-VirtualBox):41303 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=35ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=52287  10.0.2.15(lev-VirtualBox):34562 -> 74.125.205.84(le-in-f84.1e100.net):443
OPEN  UDP   pid=2995(Socket Thread) cookie=52291  10.0.2.15(lev-VirtualBox):40082 -> 209.85.233.94(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40576
OPEN  UDP   pid=514(systemd-resolve) cookie=51610  10.0.2.15(lev-VirtualBox):52794 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=51610  10.0.2.15(lev-VirtualBox):52794 -> 10.0.2.3(vboxdns):53  out=55B/1p in=114B/1p  age=30ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=53266  10.0.2.15(lev-VirtualBox):44210 -> 142.251.1.119(lb-in-f119.1e100.net):443  out=2464B/8p in=6430B/12p  age=1.03s reason=close()
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=53282  127.0.0.1(localhost):52573 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52573
OPEN  UDP   pid=514(systemd-resolve) cookie=51616  10.0.2.15(lev-VirtualBox):43148 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #12) cookie=51615  127.0.0.1(localhost):55863 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55863
OPEN  UDP   pid=514(systemd-resolve) cookie=51619  10.0.2.15(lev-VirtualBox):51128 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=51620  10.0.2.15(lev-VirtualBox):36629 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=51616  10.0.2.15(lev-VirtualBox):43148 -> 10.0.2.3(vboxdns):53  out=43B/1p in=82B/1p  age=30ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=53282  127.0.0.1(localhost):52573 -> 127.0.0.53(dnsstub):53  out=43B/1p in=68B/1p  age=34ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=51619  10.0.2.15(lev-VirtualBox):51128 -> 10.0.2.3(vboxdns):53  out=43B/1p in=223B/1p  age=32ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=51620  10.0.2.15(lev-VirtualBox):36629 -> 10.0.2.3(vboxdns):53  out=43B/1p in=211B/1p  age=31ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #12) cookie=51615  127.0.0.1(localhost):55863 -> 127.0.0.53(dnsstub):53  out=86B/2p in=294B/2p  age=40ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=51627  10.0.2.15(lev-VirtualBox):55553 -> 108.177.14.147(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60327
OPEN  UDP   pid=514(systemd-resolve) cookie=51631  10.0.2.15(lev-VirtualBox):41003 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=51631  10.0.2.15(lev-VirtualBox):41003 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=47ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=43554  10.0.2.15(lev-VirtualBox):43572 -> 140.82.114.26(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43293
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51466  out=147B/1p in=49B/1p  age=5.497s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35196  out=344B/2p in=96B/2p  age=5.469s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41301  out=270B/2p in=94B/2p  age=5.443s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44894  out=136B/1p in=43B/1p  age=5.324s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49413  out=100B/1p in=55B/1p  age=5.438s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46303  out=159B/2p in=78B/2p  age=5.312s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37297  out=104B/1p in=39B/1p  age=5.27s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47245  out=346B/2p in=88B/2p  age=5.548s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45673  out=183B/1p in=44B/1p  age=5.487s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60143  out=126B/1p in=46B/1p  age=5.385s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47908  out=183B/1p in=44B/1p  age=5.265s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53191  out=90B/1p in=56B/1p  age=5.233s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39260  out=128B/1p in=47B/1p  age=5.393s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35446  out=254B/2p in=88B/2p  age=5.537s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49227  out=145B/1p in=48B/1p  age=5.466s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32955  out=282B/2p in=88B/2p  age=5.263s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51248  out=98B/1p in=49B/1p  age=5.507s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59732  out=194B/2p in=92B/2p  age=5.391s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54495  out=143B/1p in=44B/1p  age=5.501s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39692  out=329B/2p in=98B/2p  age=5.512s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42470  out=278B/2p in=86B/2p  age=5.336s reason=idle
OPEN  UDP   pid=2995(DNS Resolver #9) cookie=51654  127.0.0.1(localhost):52748 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32894
OPEN  UDP   pid=2995(DNS Res~ver #11) cookie=53304  127.0.0.1(localhost):32894 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50951  10.0.2.15(lev-VirtualBox):33475 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50952  10.0.2.15(lev-VirtualBox):38523 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52748
OPEN  UDP   pid=2995(DNS Resolver #8) cookie=50954  127.0.0.1(localhost):45726 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Res~ver #10) cookie=51655  127.0.0.1(localhost):34379 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45726
OPEN  UDP   pid=514(systemd-resolve) cookie=50955  10.0.2.15(lev-VirtualBox):59699 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34379
OPEN  UDP   pid=514(systemd-resolve) cookie=50956  10.0.2.15(lev-VirtualBox):44124 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=50957  10.0.2.15(lev-VirtualBox):53665 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50951  10.0.2.15(lev-VirtualBox):33475 -> 10.0.2.3(vboxdns):53  out=58B/1p in=238B/1p  age=54ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50952  10.0.2.15(lev-VirtualBox):38523 -> 10.0.2.3(vboxdns):53  out=58B/1p in=286B/1p  age=52ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #9) cookie=51654  127.0.0.1(localhost):52748 -> 127.0.0.53(dnsstub):53  out=58B/1p in=122B/1p  age=55ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #11) cookie=53304  127.0.0.1(localhost):32894 -> 127.0.0.53(dnsstub):53  out=116B/2p in=292B/2p  age=59ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50955  10.0.2.15(lev-VirtualBox):59699 -> 10.0.2.3(vboxdns):53  out=62B/1p in=187B/1p  age=36ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50956  10.0.2.15(lev-VirtualBox):44124 -> 10.0.2.3(vboxdns):53  out=62B/1p in=193B/1p  age=40ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=50957  10.0.2.15(lev-VirtualBox):53665 -> 10.0.2.3(vboxdns):53  out=62B/1p in=199B/1p  age=39ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #10) cookie=51655  127.0.0.1(localhost):34379 -> 127.0.0.53(dnsstub):53  out=62B/1p in=114B/1p  age=48ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #8) cookie=50954  127.0.0.1(localhost):45726 -> 127.0.0.53(dnsstub):53  out=124B/2p in=228B/2p  age=50ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59906
OPEN  UDP   pid=514(systemd-resolve) cookie=50959  10.0.2.15(lev-VirtualBox):41959 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50959  10.0.2.15(lev-VirtualBox):41959 -> 10.0.2.3(vboxdns):53  out=101B/2p in=280B/2p  age=6ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #5) cookie=51658  127.0.0.1(localhost):59878 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59878
CLOSE UDP   pid=2995(DNS Resolver #5) cookie=51658  127.0.0.1(localhost):59878 -> 127.0.0.53(dnsstub):53  out=120B/2p in=164B/2p  age=5ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=51665  10.0.2.15(lev-VirtualBox):60434 -> 173.194.24.7(miss):443
OPEN  TCP   pid=2995(Socket Thread) cookie=51666  10.0.2.15(lev-VirtualBox):60436 -> 173.194.24.7(pending):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35432
OPEN  UDP   pid=514(systemd-resolve) cookie=50961  10.0.2.15(lev-VirtualBox):42323 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=50961  10.0.2.15(lev-VirtualBox):42323 -> 10.0.2.3(vboxdns):53  out=54B/1p in=117B/1p  age=35ms reason=close()
OPEN  UDP   pid=2995(Socket Thread) cookie=51661  10.0.2.15(lev-VirtualBox):34314 -> 173.194.24.7(dfw25s40-in-f7.1e100.net):443
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53533  out=106B/1p in=49B/1p  age=5.102s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42327  out=109B/1p in=56B/1p  age=5.411s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33008  out=368B/2p in=80B/2p  age=5.207s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53134  out=142B/2p in=98B/2p  age=5.112s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57611  out=97B/1p in=40B/1p  age=5.203s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46103  out=331B/2p in=118B/2p  age=5.595s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52227  10.0.2.15(lev-VirtualBox):53638 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=10440B/6p in=6383B/23p  age=6.455s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58850  out=88B/1p in=55B/1p  age=5.236s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60657  out=89B/1p in=55B/1p  age=6.138s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=2108B/4p in=6279B/12p  age=6.063s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60508  out=140B/2p in=96B/2p  age=5.343s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44983  out=136B/2p in=92B/2p  age=5.371s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=53262  10.0.2.15(lev-VirtualBox):53747 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=4469B/9p in=24327B/14p  age=6.162s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41484  out=98B/1p in=48B/1p  age=5.327s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39283  out=71B/1p in=46B/1p  age=5.361s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=5239B/7p in=1461B/18p  age=6.513s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):40576  out=88B/1p in=55B/1p  age=5.141s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35551  out=88B/1p in=55B/1p  age=6.104s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52573  out=68B/1p in=43B/1p  age=5.486s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443  out=2091B/4p in=4567B/12p  age=6.203s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=52291  10.0.2.15(lev-VirtualBox):40082 -> 209.85.233.94(lr-in-f94.1e100.net):443  out=3999B/5p in=8064B/6p  age=6.263s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55863  out=294B/2p in=86B/2p  age=5.48s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60327  out=90B/1p in=56B/1p  age=5.296s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=52277  10.0.2.15(lev-VirtualBox):37062 -> 142.251.1.119(lb-in-f119.1e100.net):443  out=3979B/4p in=8660B/6p  age=6.409s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=52287  10.0.2.15(lev-VirtualBox):34562 -> 74.125.205.84(le-in-f84.1e100.net):443  out=6338B/13p in=15209B/16p  age=6.268s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=51627  10.0.2.15(lev-VirtualBox):55553 -> 108.177.14.147(lt-in-f147.1e100.net):443  out=3997B/5p in=7678B/5p  age=5.425s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43293  out=100B/1p in=55B/1p  age=5.219s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=43554  10.0.2.15(lev-VirtualBox):43572 -> 140.82.114.26(lb-140-82-114-26-iad.github.com):443  out=30B/1p in=26B/2p  age=5.223s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=51665  10.0.2.15(lev-VirtualBox):60434 -> 173.194.24.7(dfw25s40-in-f7.1e100.net):443  out=2006B/3p in=4624B/6p  age=5.143s reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=51666  10.0.2.15(lev-VirtualBox):60436 -> 173.194.24.7(dfw25s40-in-f7.1e100.net):443  out=2006B/3p in=4624B/6p  age=5.143s reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34379  out=114B/1p in=62B/1p  age=5.783s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35432  out=92B/1p in=54B/1p  age=5.668s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59906  out=116B/1p in=56B/1p  age=5.734s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443  out=3397B/7p in=2340B/14p  age=5.736s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45726  out=228B/2p in=124B/2p  age=5.787s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52748  out=122B/1p in=58B/1p  age=5.809s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32894  out=292B/2p in=116B/2p  age=5.813s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59878  out=164B/2p in=120B/2p  age=5.724s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=51661  10.0.2.15(lev-VirtualBox):34314 -> 173.194.24.7(dfw25s40-in-f7.1e100.net):443  out=9792B/15p in=7919B/18p  age=6.717s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=50875  10.0.2.15(lev-VirtualBox):46422 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=19333B/60p in=153919B/89p  age=15.664s reason=idle
OPEN  UDP   pid=2995(Socket Thread) cookie=53262  10.0.2.15(lev-VirtualBox):53747 -> 64.233.164.95(lf-in-f95.1e100.net):443
CLOSE UDP   pid=2995(Socket Thread) cookie=53262  10.0.2.15(lev-VirtualBox):53747 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=490B/3p in=2945B/7p  age=6.244s reason=idle
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53240  [fd00:0:0:0:10dd:174a:ea91:a63f](miss):33866 -> [2001:4860:4860:0:0:0:0:8888](miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53813
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53241  127.0.0.1(localhost):62797 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):62797
OPEN  UDP   pid=514(systemd-resolve) cookie=52023  10.0.2.15(lev-VirtualBox):37113 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53242  127.0.0.1(localhost):9817 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48684
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53243  127.0.0.1(localhost):63364 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=52024  10.0.2.15(lev-VirtualBox):42422 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):9817
OPEN  UDP   pid=514(systemd-resolve) cookie=52025  10.0.2.15(lev-VirtualBox):37903 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):63364
OPEN  UDP   pid=514(systemd-resolve) cookie=52026  10.0.2.15(lev-VirtualBox):60256 -> 10.0.2.3(vboxdns):53
OPEN  ICMPv6 pid=674(NetworkManager) cookie=8981  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=514(systemd-resolve) cookie=52024  10.0.2.15(lev-VirtualBox):42422 -> 10.0.2.3(vboxdns):53  out=191B/2p in=362B/2p  age=26ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52025  10.0.2.15(lev-VirtualBox):37903 -> 10.0.2.3(vboxdns):53  out=48B/1p in=217B/1p  age=56ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=53242  127.0.0.1(localhost):9817 -> 127.0.0.53(dnsstub):53  out=37B/1p in=134B/1p  age=62ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52023  10.0.2.15(lev-VirtualBox):37113 -> 10.0.2.3(vboxdns):53  out=48B/1p in=168B/1p  age=61ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=52027  10.0.2.15(lev-VirtualBox):53651 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=52026  10.0.2.15(lev-VirtualBox):60256 -> 10.0.2.3(vboxdns):53  out=48B/1p in=419B/1p  age=62ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=53243  127.0.0.1(localhost):63364 -> 127.0.0.53(dnsstub):53  out=37B/1p in=200B/1p  age=70ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=53241  127.0.0.1(localhost):62797 -> 127.0.0.53(dnsstub):53  out=37B/1p in=0B/0p  age=92ms reason=close()
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53244  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):44364 -> [2603:1061:14:32:0:0:0:1](miss):80
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53245  10.0.2.15(lev-VirtualBox):35979 -> 13.107.226.44(miss):80
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=53246  10.0.2.15(lev-VirtualBox):41820 -> 13.107.253.44(miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55415
OPEN  UDP   pid=514(systemd-resolve) cookie=52029  10.0.2.15(lev-VirtualBox):35092 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35965
OPEN  UDP   pid=514(systemd-resolve) cookie=52030  10.0.2.15(lev-VirtualBox):46543 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57154
OPEN  UDP   pid=514(systemd-resolve) cookie=52031  10.0.2.15(lev-VirtualBox):33284 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=53247  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):41700 -> [2603:1061:14:32:0:0:0:1](pending):443
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=53247  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):41700 -> [2603:1061:14:32:0:0:0:1](pending):443  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=53248  10.0.2.15(lev-VirtualBox):43690 -> 13.107.226.44(pending):443
CLOSE UDP   pid=514(systemd-resolve) cookie=52030  10.0.2.15(lev-VirtualBox):46543 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=34ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52031  10.0.2.15(lev-VirtualBox):33284 -> 10.0.2.3(vboxdns):53  out=99B/2p in=326B/2p  age=37ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52027  10.0.2.15(lev-VirtualBox):53651 -> 10.0.2.3(vboxdns):53  out=50B/1p in=295B/1p  age=70ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=52032  10.0.2.15(lev-VirtualBox):32972 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=52029  10.0.2.15(lev-VirtualBox):35092 -> 10.0.2.3(vboxdns):53  out=191B/2p in=420B/2p  age=60ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52032  10.0.2.15(lev-VirtualBox):32972 -> 10.0.2.3(vboxdns):53  out=61B/1p in=151B/1p  age=33ms reason=close()
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=53248  10.0.2.15(lev-VirtualBox):43690 -> 13.107.226.44(no-ptr):443  out=3404B/6p in=67056B/13p  age=5.713s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57154  out=141B/1p in=55B/1p  age=5.72s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):62797  out=228B/1p in=37B/1p  age=5.815s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53813  out=125B/1p in=101B/1p  age=5.819s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55415  out=187B/1p in=101B/1p  age=5.725s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):9817  out=134B/1p in=37B/1p  age=5.812s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):63364  out=200B/1p in=37B/1p  age=5.811s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48684  out=163B/1p in=101B/1p  age=5.815s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35965  out=141B/1p in=55B/1p  age=5.723s reason=idle
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=52081  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):50519 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE ICMPv6 pid=674(NetworkManager) cookie=8981  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=88B/2p  age=10.661s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52227  10.0.2.15(lev-VirtualBox):53638 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=43554  10.0.2.15(lev-VirtualBox):43572 -> 140.82.114.26(lb-140-82-114-26-iad.github.com):443
OPEN  UDP   pid=2995(DNS Resolver #7) cookie=51122  127.0.0.1(localhost):57139 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57139
OPEN  UDP   pid=514(systemd-resolve) cookie=52115  10.0.2.15(lev-VirtualBox):58818 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=52116  10.0.2.15(lev-VirtualBox):35651 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=52116  10.0.2.15(lev-VirtualBox):35651 -> 10.0.2.3(vboxdns):53  out=45B/1p in=174B/1p  age=48ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=52117  10.0.2.15(lev-VirtualBox):59204 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=52115  10.0.2.15(lev-VirtualBox):58818 -> 10.0.2.3(vboxdns):53  out=45B/1p in=121B/1p  age=53ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52117  10.0.2.15(lev-VirtualBox):59204 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=30ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #7) cookie=51122  127.0.0.1(localhost):57139 -> 127.0.0.53(dnsstub):53  out=90B/2p in=209B/2p  age=85ms reason=close()
OPEN  UDP   pid=2995(DNS Resolver #6) cookie=53455  127.0.0.1(localhost):38174 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2995(DNS Res~ver #13) cookie=53456  127.0.0.1(localhost):47157 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47157
OPEN  UDP   pid=514(systemd-resolve) cookie=52118  10.0.2.15(lev-VirtualBox):34538 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=2995(DNS Res~ver #12) cookie=52120  127.0.0.1(localhost):37441 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38174
OPEN  UDP   pid=514(systemd-resolve) cookie=52121  10.0.2.15(lev-VirtualBox):55001 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37441
CLOSE UDP   pid=2995(DNS Res~ver #12) cookie=52120  127.0.0.1(localhost):37441 -> 127.0.0.53(dnsstub):53  out=45B/1p in=80B/1p  age=9ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52121  10.0.2.15(lev-VirtualBox):55001 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=6ms reason=close()
CLOSE UDP   pid=2995(DNS Resolver #6) cookie=53455  127.0.0.1(localhost):38174 -> 127.0.0.53(dnsstub):53  out=90B/2p in=225B/2p  age=20ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53466
OPEN  UDP   pid=514(systemd-resolve) cookie=52123  10.0.2.15(lev-VirtualBox):60871 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=52118  10.0.2.15(lev-VirtualBox):34538 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=32ms reason=close()
CLOSE UDP   pid=2995(DNS Res~ver #13) cookie=53456  127.0.0.1(localhost):47157 -> 127.0.0.53(dnsstub):53  out=45B/1p in=145B/1p  age=36ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=52123  10.0.2.15(lev-VirtualBox):60871 -> 10.0.2.3(vboxdns):53  out=55B/1p in=126B/1p  age=32ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=43554  10.0.2.15(lev-VirtualBox):43572 -> 140.82.114.26(lb-140-82-114-26-iad.github.com):443  out=69B/2p in=41B/2p  age=1.006s reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=39B/1p in=39B/2p  age=5.161s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443  out=39B/1p in=39B/2p  age=5.162s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.164s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=39B/1p in=39B/2p  age=5.162s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57139  out=209B/2p in=90B/2p  age=5.108s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37441  out=80B/1p in=45B/1p  age=5.004s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52227  10.0.2.15(lev-VirtualBox):53638 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=39B/1p in=39B/2p  age=6.164s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53466  out=100B/1p in=55B/1p  age=5.984s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47157  out=145B/1p in=45B/1p  age=6.012s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38174  out=225B/2p in=90B/2p  age=6.007s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=3653B/4p in=3898B/20p  age=6.99s reason=idle
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39760
OPEN  UDP   pid=2995(DNS Resolver #9) cookie=53498  127.0.0.1(localhost):39760 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=2995(DNS Resolver #9) cookie=53498  127.0.0.1(localhost):39760 -> 127.0.0.53(dnsstub):53  out=88B/2p in=282B/2p  age=4ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443
OPEN  UDP   pid=2995(Socket Thread) cookie=53509  10.0.2.15(lev-VirtualBox):51832 -> 64.233.161.198(lh-in-f198.1e100.net):443
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39760  out=282B/2p in=88B/2p  age=5.054s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=2684B/5p in=1951B/12p  age=5.839s reason=idle
CLOSE UDP   pid=2995(Socket Thread) cookie=53509  10.0.2.15(lev-VirtualBox):51832 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=10274B/7p in=5490B/11p  age=5.943s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52227  10.0.2.15(lev-VirtualBox):53638 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52227  10.0.2.15(lev-VirtualBox):53638 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=63B/2p in=39B/2p  age=5ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.344s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443  out=39B/1p in=39B/2p  age=5.337s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=39B/1p in=39B/2p  age=5.342s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=39B/1p in=39B/2p  age=5.335s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.064s reason=idle
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=53248  10.0.2.15(lev-VirtualBox):43690 -> 13.107.226.44(no-ptr):443
CLOSE TCP   pid=2776(Chrome_ChildIOT) cookie=53248  10.0.2.15(lev-VirtualBox):43690 -> 13.107.226.44(no-ptr):443  out=0B/0p in=63B/1p  age=2ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.384s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443
OPEN  TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52295  10.0.2.15(lev-VirtualBox):37170 -> 74.125.205.84(le-in-f84.1e100.net):443  out=63B/2p in=0B/0p  age=3ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=53252  10.0.2.15(lev-VirtualBox):53644 -> 34.120.208.123(123.208.120.34.bc.googleusercontent.com):443  out=63B/2p in=0B/0p  age=6ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=53271  10.0.2.15(lev-VirtualBox):41628 -> 64.233.164.95(lf-in-f95.1e100.net):443  out=63B/2p in=0B/0p  age=5ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443
CLOSE TCP   pid=2995(Socket Thread) cookie=51656  10.0.2.15(lev-VirtualBox):47900 -> 151.101.193.91(no-ptr):443  out=63B/2p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.089s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=39B/1p in=39B/2p  age=5.808s reason=idle
OPEN  UDP   pid=514(systemd-resolve) cookie=56462  10.0.2.15(lev-VirtualBox):49437 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56462  10.0.2.15(lev-VirtualBox):49437 -> 10.0.2.3(vboxdns):53  out=58B/1p in=742B/1p  age=45ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54705  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):33042 -> [2620:2d:4000:1:0:0:0:96](miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36855
OPEN  TCP   pid=674(NetworkManager) cookie=54706  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):43710 -> [2620:2d:4000:1:0:0:0:98](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=54705  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):33042 -> [2620:2d:4000:1:0:0:0:96](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46996
CLOSE TCP   pid=674(NetworkManager) cookie=54706  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):43710 -> [2620:2d:4000:1:0:0:0:98](ubuntu-content-cache-3.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=11ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54710  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):54434 -> [2620:2d:4000:1:0:0:0:2b](miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55509
OPEN  TCP   pid=674(NetworkManager) cookie=54711  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):35510 -> [2620:2d:4000:1:0:0:0:97](miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54352
CLOSE TCP   pid=674(NetworkManager) cookie=54710  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):54434 -> [2620:2d:4000:1:0:0:0:2b](is-content-cache-2.canonical.com):80  out=0B/0p in=0B/0p  age=22ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54713  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):44018 -> [2620:2d:4002:1:0:0:0:197](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=54711  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):35510 -> [2620:2d:4000:1:0:0:0:97](pending):80  out=0B/0p in=0B/0p  age=4ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54714  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):39776 -> [2620:2d:4000:1:0:0:0:2a](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=54713  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):44018 -> [2620:2d:4002:1:0:0:0:197](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54715  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):46948 -> [2620:2d:4000:1:0:0:0:22](miss):80
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42022
CLOSE TCP   pid=674(NetworkManager) cookie=54714  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):39776 -> [2620:2d:4000:1:0:0:0:2a](pending):80  out=0B/0p in=0B/0p  age=5ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54717  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):49014 -> [2620:2d:4002:1:0:0:0:198](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=54715  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):46948 -> [2620:2d:4000:1:0:0:0:22](pending):80  out=0B/0p in=0B/0p  age=5ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=54718  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):36126 -> [2620:2d:4002:1:0:0:0:196](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=54717  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):49014 -> [2620:2d:4002:1:0:0:0:198](pending):80  out=0B/0p in=0B/0p  age=0s reason=close()
CLOSE TCP   pid=674(NetworkManager) cookie=54718  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):36126 -> [2620:2d:4002:1:0:0:0:196](pending):80  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57828
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35730
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59951
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37497
OPEN  ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36855  out=155B/1p in=101B/1p  age=5.125s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46996  out=155B/1p in=101B/1p  age=5.124s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57828  out=147B/1p in=101B/1p  age=5.08s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37497  out=155B/1p in=101B/1p  age=5.074s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59951  out=155B/1p in=101B/1p  age=5.076s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35730  out=155B/1p in=101B/1p  age=5.08s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55509  out=147B/1p in=101B/1p  age=5.106s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54352  out=155B/1p in=101B/1p  age=5.09s reason=idle
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42022  out=135B/1p in=101B/1p  age=5.082s reason=idle
CLOSE ICMPv6 pid=674(NetworkManager) cookie=8981  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.351s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=45921  10.0.2.15(lev-VirtualBox):58392 -> 34.107.243.93(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34512
OPEN  UDP   pid=514(systemd-resolve) cookie=56493  10.0.2.15(lev-VirtualBox):56289 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=56493  10.0.2.15(lev-VirtualBox):56289 -> 10.0.2.3(vboxdns):53  out=55B/1p in=133B/1p  age=49ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34512  out=107B/1p in=55B/1p  age=5.944s reason=idle
CLOSE TCP   pid=2995(Socket Thread) cookie=45921  10.0.2.15(lev-VirtualBox):58392 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=28B/1p in=24B/2p  age=5.946s reason=idle
OPEN  TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443
OPEN  TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443
CLOSE TCP   pid=2995(Socket Thread) cookie=53513  10.0.2.15(lev-VirtualBox):40234 -> 64.233.161.198(lh-in-f198.1e100.net):443  out=63B/2p in=0B/0p  age=1ms reason=close()
CLOSE TCP   pid=2995(Socket Thread) cookie=52122  10.0.2.15(lev-VirtualBox):42430 -> 140.82.112.25(lb-140-82-112-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.104s reason=idle
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=56582  [fd00:0:0:0:10dd:174a:ea91:a63f](lev-VirtualBox):57826 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=56583  127.0.0.1(localhost):4815 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=56584  127.0.0.1(localhost):49167 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):4815
OPEN  UDP   pid=2776(Chrome_ChildIOT) cookie=56585  127.0.0.1(localhost):28829 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=514(systemd-resolve) cookie=54956  10.0.2.15(lev-VirtualBox):57742 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49167
OPEN  UDP   pid=514(systemd-resolve) cookie=54957  10.0.2.15(lev-VirtualBox):41736 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):28829
OPEN  UDP   pid=514(systemd-resolve) cookie=54958  10.0.2.15(lev-VirtualBox):37507 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=56584  127.0.0.1(localhost):49167 -> 127.0.0.53(dnsstub):53  out=32B/1p in=97B/1p  age=49ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=54957  10.0.2.15(lev-VirtualBox):41736 -> 10.0.2.3(vboxdns):53  out=43B/1p in=127B/1p  age=45ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=54956  10.0.2.15(lev-VirtualBox):57742 -> 10.0.2.3(vboxdns):53  out=43B/1p in=127B/1p  age=46ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=56583  127.0.0.1(localhost):4815 -> 127.0.0.53(dnsstub):53  out=32B/1p in=97B/1p  age=57ms reason=close()
CLOSE UDP   pid=514(systemd-resolve) cookie=54958  10.0.2.15(lev-VirtualBox):37507 -> 10.0.2.3(vboxdns):53  out=43B/1p in=73B/1p  age=57ms reason=close()
CLOSE UDP   pid=2776(Chrome_ChildIOT) cookie=56585  127.0.0.1(localhost):28829 -> 127.0.0.53(dnsstub):53  out=32B/1p in=48B/1p  age=65ms reason=close()
OPEN  TCP   pid=2776(Chrome_ChildIOT) cookie=56586  10.0.2.15(lev-VirtualBox):33506 -> 140.82.121.6(miss):443
OPEN  UDP   pid=514(systemd-resolve) cookie=7265  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41014
OPEN  UDP   pid=514(systemd-resolve) cookie=54960  10.0.2.15(lev-VirtualBox):39076 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=514(systemd-resolve) cookie=54960  10.0.2.15(lev-VirtualBox):39076 -> 10.0.2.3(vboxdns):53  out=54B/1p in=123B/1p  age=29ms reason=close()
