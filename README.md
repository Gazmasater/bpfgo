
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





CLOSE TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443  out=2861B/5p in=2787B/16p  age=5.57s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53475  out=116B/1p in=56B/1p  age=5.563s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59303  out=360B/2p in=112B/2p  age=5.579s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53798  out=100B/1p in=55B/1p  age=5.519s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=41360  10.0.2.15(lev-VirtualBox):41652 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=39B/1p in=39B/2p  age=5.567s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.521s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47839  out=100B/1p in=55B/1p  age=5.564s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47108  out=107B/1p in=55B/1p  age=5.103s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=42014  10.0.2.15(lev-VirtualBox):43368 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=28B/1p in=24B/2p  age=5.107s reason=idle
OPEN  UDP   pid=2907(libuv-worker) cookie=44224  127.0.0.1(localhost):54295 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54295
OPEN  UDP   pid=511(systemd-resolve) cookie=41796  10.0.2.15(lev-VirtualBox):41998 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=41797  10.0.2.15(lev-VirtualBox):34333 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=41796  10.0.2.15(lev-VirtualBox):41998 -> 10.0.2.3(vboxdns):53  out=48B/1p in=225B/1p  age=38ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=41797  10.0.2.15(lev-VirtualBox):34333 -> 10.0.2.3(vboxdns):53  out=48B/1p in=274B/1p  age=39ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=41798  10.0.2.15(lev-VirtualBox):56256 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=41798  10.0.2.15(lev-VirtualBox):56256 -> 10.0.2.3(vboxdns):53  out=48B/1p in=132B/1p  age=2ms reason=close()
CLOSE UDP   pid=2907(libuv-worker) cookie=44224  127.0.0.1(localhost):54295 -> 127.0.0.53(dnsstub):53  out=96B/2p in=301B/2p  age=49ms reason=close()
OPEN  TCP   pid=2907(code) cookie=42721  10.0.2.15(lev-VirtualBox):38576 -> 13.107.5.93(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53812
OPEN  UDP   pid=511(systemd-resolve) cookie=41799  10.0.2.15(lev-VirtualBox):54048 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=41799  10.0.2.15(lev-VirtualBox):54048 -> 10.0.2.3(vboxdns):53  out=95B/2p in=322B/2p  age=25ms reason=close()
CLOSE TCP   pid=2907(code) cookie=42721  10.0.2.15(lev-VirtualBox):38576 -> 13.107.5.93(no-ptr):443  out=1468B/3p in=6394B/4p  age=4.336s reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53812  out=139B/1p in=53B/1p  age=5.817s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54295  out=301B/2p in=96B/2p  age=5.872s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=40844  10.0.2.15(lev-VirtualBox):46266 -> 185.199.110.133(cdn-185-199-110-133.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=41360  10.0.2.15(lev-VirtualBox):41652 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=40844  10.0.2.15(lev-VirtualBox):46266 -> 185.199.110.133(cdn-185-199-110-133.github.com):443  out=39B/1p in=39B/2p  age=5.36s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=41360  10.0.2.15(lev-VirtualBox):41652 -> 140.82.113.21(lb-140-82-113-21-iad.github.com):443  out=102B/3p in=78B/4p  age=2.305s reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.22s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.531s reason=idle
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=44078  10.0.2.15(lev-VirtualBox):55996 -> 150.171.109.53(miss):443
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=44078  10.0.2.15(lev-VirtualBox):55996 -> 150.171.109.53(pending):443  out=0B/0p in=63B/1p  age=0s reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35708
OPEN  UDP   pid=511(systemd-resolve) cookie=41876  10.0.2.15(lev-VirtualBox):48012 -> 10.0.2.3(vboxdns):53
OPEN  ICMPv6 pid=674(NetworkManager) cookie=9545  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=511(systemd-resolve) cookie=41876  10.0.2.15(lev-VirtualBox):48012 -> 10.0.2.3(vboxdns):53  out=101B/2p in=330B/2p  age=40ms reason=close()
CLOSE ICMPv6 pid=674(NetworkManager) cookie=9545  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=56B/1p  age=5.102s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35708  out=142B/1p in=56B/1p  age=5.105s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=40844  10.0.2.15(lev-VirtualBox):46266 -> 185.199.110.133(cdn-185-199-110-133.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=40844  10.0.2.15(lev-VirtualBox):46266 -> 185.199.110.133(cdn-185-199-110-133.github.com):443  out=63B/2p in=0B/0p  age=8ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.663s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=29B/1p in=25B/2p  age=5.541s reason=idle
OPEN  UDP   pid=511(systemd-resolve) cookie=45092  10.0.2.15(lev-VirtualBox):47938 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=45092  10.0.2.15(lev-VirtualBox):47938 -> 10.0.2.3(vboxdns):53  out=58B/1p in=742B/1p  age=39ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=42932  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39762 -> [2620:2d:4002:1:0:0:0:196](miss):80
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59524
OPEN  UDP   pid=511(systemd-resolve) cookie=43877  10.0.2.15(lev-VirtualBox):51918 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=674(NetworkManager) cookie=42934  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):43960 -> [2001:67c:1562:0:0:0:0:24](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=42932  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39762 -> [2620:2d:4002:1:0:0:0:196](pending):80  out=0B/0p in=0B/0p  age=13ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37945
OPEN  UDP   pid=511(systemd-resolve) cookie=43878  10.0.2.15(lev-VirtualBox):54694 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=674(NetworkManager) cookie=45093  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):48152 -> [2620:2d:4002:1:0:0:0:197](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=42934  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):43960 -> [2001:67c:1562:0:0:0:0:24](pending):80  out=0B/0p in=0B/0p  age=11ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51399
OPEN  TCP   pid=674(NetworkManager) cookie=45094  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39018 -> [2001:67c:1562:0:0:0:0:23](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45093  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):48152 -> [2620:2d:4002:1:0:0:0:197](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=43879  10.0.2.15(lev-VirtualBox):43595 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35701
OPEN  UDP   pid=511(systemd-resolve) cookie=43880  10.0.2.15(lev-VirtualBox):49040 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=674(NetworkManager) cookie=45095  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):46868 -> [2620:2d:4002:1:0:0:0:198](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45094  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39018 -> [2001:67c:1562:0:0:0:0:23](pending):80  out=0B/0p in=0B/0p  age=14ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45096  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):58154 -> [2620:2d:4000:1:0:0:0:23](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45095  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):46868 -> [2620:2d:4002:1:0:0:0:198](pending):80  out=0B/0p in=0B/0p  age=4ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45097  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39260 -> [2620:2d:4000:1:0:0:0:2a](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45096  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):58154 -> [2620:2d:4000:1:0:0:0:23](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45098  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):36760 -> [2620:2d:4000:1:0:0:0:98](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45097  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39260 -> [2620:2d:4000:1:0:0:0:2a](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45099  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):40850 -> [2620:2d:4000:1:0:0:0:22](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45098  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):36760 -> [2620:2d:4000:1:0:0:0:98](pending):80  out=0B/0p in=0B/0p  age=12ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45100  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39828 -> [2620:2d:4000:1:0:0:0:96](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45099  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):40850 -> [2620:2d:4000:1:0:0:0:22](pending):80  out=0B/0p in=0B/0p  age=3ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45101  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):57770 -> [2620:2d:4000:1:0:0:0:2b](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45100  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):39828 -> [2620:2d:4000:1:0:0:0:96](pending):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45102  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):36776 -> [2620:2d:4000:1:0:0:0:97](miss):80
CLOSE TCP   pid=674(NetworkManager) cookie=45101  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):57770 -> [2620:2d:4000:1:0:0:0:2b](pending):80  out=0B/0p in=0B/0p  age=11ms reason=close()
CLOSE TCP   pid=674(NetworkManager) cookie=45102  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):36776 -> [2620:2d:4000:1:0:0:0:97](pending):80  out=0B/0p in=0B/0p  age=1ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=43877  10.0.2.15(lev-VirtualBox):51918 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=75ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38100
OPEN  UDP   pid=511(systemd-resolve) cookie=43881  10.0.2.15(lev-VirtualBox):35758 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43879  10.0.2.15(lev-VirtualBox):43595 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=73ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57810
OPEN  UDP   pid=511(systemd-resolve) cookie=43882  10.0.2.15(lev-VirtualBox):54760 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43880  10.0.2.15(lev-VirtualBox):49040 -> 10.0.2.3(vboxdns):53  out=101B/1p in=208B/1p  age=92ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47502
OPEN  UDP   pid=511(systemd-resolve) cookie=43883  10.0.2.15(lev-VirtualBox):54270 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43878  10.0.2.15(lev-VirtualBox):54694 -> 10.0.2.3(vboxdns):53  out=101B/1p in=209B/1p  age=115ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37389
OPEN  UDP   pid=511(systemd-resolve) cookie=43884  10.0.2.15(lev-VirtualBox):43599 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43881  10.0.2.15(lev-VirtualBox):35758 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=67ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38510
OPEN  UDP   pid=511(systemd-resolve) cookie=43885  10.0.2.15(lev-VirtualBox):46356 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43882  10.0.2.15(lev-VirtualBox):54760 -> 10.0.2.3(vboxdns):53  out=101B/1p in=210B/1p  age=70ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=43883  10.0.2.15(lev-VirtualBox):54270 -> 10.0.2.3(vboxdns):53  out=101B/1p in=219B/1p  age=71ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46746
OPEN  UDP   pid=511(systemd-resolve) cookie=43886  10.0.2.15(lev-VirtualBox):55124 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32950
OPEN  UDP   pid=511(systemd-resolve) cookie=43887  10.0.2.15(lev-VirtualBox):55594 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43884  10.0.2.15(lev-VirtualBox):43599 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=74ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38908
OPEN  UDP   pid=511(systemd-resolve) cookie=43888  10.0.2.15(lev-VirtualBox):42876 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43885  10.0.2.15(lev-VirtualBox):46356 -> 10.0.2.3(vboxdns):53  out=101B/1p in=207B/1p  age=66ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=43886  10.0.2.15(lev-VirtualBox):55124 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=66ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=43887  10.0.2.15(lev-VirtualBox):55594 -> 10.0.2.3(vboxdns):53  out=101B/1p in=219B/1p  age=67ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=43888  10.0.2.15(lev-VirtualBox):42876 -> 10.0.2.3(vboxdns):53  out=101B/1p in=227B/1p  age=65ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38100  out=155B/1p in=101B/1p  age=5.288s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57810  out=138B/1p in=101B/1p  age=5.274s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46746  out=155B/1p in=101B/1p  age=5.175s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37945  out=137B/1p in=101B/1p  age=5.361s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47502  out=147B/1p in=101B/1p  age=5.247s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35701  out=136B/1p in=101B/1p  age=5.348s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):32950  out=147B/1p in=101B/1p  age=5.169s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37389  out=155B/1p in=101B/1p  age=5.241s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59524  out=155B/1p in=101B/1p  age=5.375s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38908  out=155B/1p in=101B/1p  age=5.162s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51399  out=155B/1p in=101B/1p  age=5.351s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38510  out=135B/1p in=101B/1p  age=5.217s reason=idle
OPEN  ICMPv6 pid=674(NetworkManager) cookie=9545  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE ICMPv6 pid=674(NetworkManager) cookie=9545  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.87s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443
CLOSE TCP   pid=3008(Socket Thread) cookie=41686  10.0.2.15(lev-VirtualBox):42290 -> 151.101.193.91(no-ptr):443  out=63B/2p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.517s reason=idle
OPEN  UDP   pid=670(avahi-daemon) cookie=5592  [fd00:0:0:0:bf53:d82b:4102:d10b](miss):5353 -> [ff02:0:0:0:0:0:0:fb](skip):5353
OPEN  UDP   pid=670(avahi-daemon) cookie=5591  10.0.2.15(lev-VirtualBox):5353 -> 224.0.0.251(skip):5353
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44431
OPEN  UDP   pid=670(avahi-daemon) cookie=5592  [ff02:0:0:0:0:0:0:fb%enp0s3](skip):5353 -> [fd00:0:0:0:bf53:d82b:4102:d10b](pending):5353
OPEN  UDP   pid=670(avahi-daemon) cookie=5591  10.0.2.15(lev-VirtualBox):5353 -> 10.0.2.15(lev-VirtualBox):5353
OPEN  UDP   pid=670(avahi-daemon) cookie=5591  127.0.0.1(localhost):5353 -> 127.0.0.1(localhost):5353
OPEN  UDP   pid=511(systemd-resolve) cookie=43972  10.0.2.15(lev-VirtualBox):42069 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=43972  10.0.2.15(lev-VirtualBox):42069 -> 10.0.2.3(vboxdns):53  out=191B/2p in=362B/2p  age=71ms reason=close()
CLOSE UDP   pid=670(avahi-daemon) cookie=5592  [ff02:0:0:0:0:0:0:fb%enp0s3](skip):5353 -> [fd00:0:0:0:bf53:d82b:4102:d10b](lev-VirtualBox):5353  out=0B/0p in=45B/1p  age=5.382s reason=idle
CLOSE UDP   pid=670(avahi-daemon) cookie=5591  10.0.2.15(lev-VirtualBox):5353 -> 224.0.0.251(skip):5353  out=90B/2p in=0B/0p  age=5.384s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44431  out=163B/1p in=101B/1p  age=5.382s reason=idle
CLOSE UDP   pid=670(avahi-daemon) cookie=5591  10.0.2.15(lev-VirtualBox):5353 -> 10.0.2.15(lev-VirtualBox):5353  out=0B/0p in=45B/1p  age=5.381s reason=idle
CLOSE UDP   pid=670(avahi-daemon) cookie=5591  127.0.0.1(localhost):5353 -> 127.0.0.1(localhost):5353  out=0B/0p in=45B/1p  age=5.381s reason=idle
CLOSE UDP   pid=670(avahi-daemon) cookie=5592  [fd00:0:0:0:bf53:d82b:4102:d10b](lev-VirtualBox):5353 -> [ff02:0:0:0:0:0:0:fb](skip):5353  out=45B/1p in=0B/0p  age=5.384s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
OPEN  UDP   pid=511(systemd-resolve) cookie=44002  10.0.2.15(lev-VirtualBox):53882 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=44002  10.0.2.15(lev-VirtualBox):53882 -> 10.0.2.3(vboxdns):53  out=58B/1p in=598B/1p  age=44ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45248  10.0.2.15(lev-VirtualBox):46970 -> 185.125.190.48(miss):80
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35439
OPEN  UDP   pid=511(systemd-resolve) cookie=44003  10.0.2.15(lev-VirtualBox):56095 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=44003  10.0.2.15(lev-VirtualBox):56095 -> 10.0.2.3(vboxdns):53  out=56B/1p in=117B/1p  age=24ms reason=close()
CLOSE TCP   pid=674(NetworkManager) cookie=45248  10.0.2.15(lev-VirtualBox):46970 -> 185.125.190.48(gladys.canonical.com):80  out=87B/1p in=185B/1p  age=279ms reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.661s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):35439  out=90B/1p in=56B/1p  age=5.592s reason=idle
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=46233  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):57759 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=46234  127.0.0.1(localhost):49584 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49584
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=46235  127.0.0.1(localhost):4172 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=47125  10.0.2.15(lev-VirtualBox):47145 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):4172
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=46236  127.0.0.1(localhost):44759 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=47126  10.0.2.15(lev-VirtualBox):47102 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44759
OPEN  UDP   pid=511(systemd-resolve) cookie=47127  10.0.2.15(lev-VirtualBox):43089 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=47127  10.0.2.15(lev-VirtualBox):43089 -> 10.0.2.3(vboxdns):53  out=48B/1p in=205B/1p  age=50ms reason=close()
CLOSE UDP   pid=2820(Chrome_ChildIOT) cookie=46236  127.0.0.1(localhost):44759 -> 127.0.0.53(dnsstub):53  out=37B/1p in=122B/1p  age=54ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47126  10.0.2.15(lev-VirtualBox):47102 -> 10.0.2.3(vboxdns):53  out=48B/1p in=443B/1p  age=56ms reason=close()
CLOSE UDP   pid=2820(Chrome_ChildIOT) cookie=46235  127.0.0.1(localhost):4172 -> 127.0.0.53(dnsstub):53  out=37B/1p in=224B/1p  age=64ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47125  10.0.2.15(lev-VirtualBox):47145 -> 10.0.2.3(vboxdns):53  out=48B/1p in=168B/1p  age=75ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=47128  10.0.2.15(lev-VirtualBox):57721 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=2820(Chrome_ChildIOT) cookie=46234  127.0.0.1(localhost):49584 -> 127.0.0.53(dnsstub):53  out=37B/1p in=0B/0p  age=83ms reason=close()
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47129  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):59827 -> [2620:1ec:48:1:0:0:0:44](miss):80
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47130  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):51466 -> [2620:1ec:29:1:0:0:0:44](miss):80
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46475
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=44660  10.0.2.15(lev-VirtualBox):38522 -> 150.171.109.51(miss):80
OPEN  UDP   pid=511(systemd-resolve) cookie=47131  10.0.2.15(lev-VirtualBox):56900 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=44661  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):43248 -> [2620:1ec:48:1:0:0:0:44](pending):443
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=44661  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):43248 -> [2620:1ec:48:1:0:0:0:44](pending):443  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55345
OPEN  UDP   pid=511(systemd-resolve) cookie=47132  10.0.2.15(lev-VirtualBox):60997 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=44662  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):50306 -> [2620:1ec:29:1:0:0:0:44](pending):443
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=44662  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):50306 -> [2620:1ec:29:1:0:0:0:44](pending):443  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50535
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=44663  10.0.2.15(lev-VirtualBox):38632 -> 150.171.109.51(pending):443
OPEN  UDP   pid=511(systemd-resolve) cookie=47133  10.0.2.15(lev-VirtualBox):39988 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=47128  10.0.2.15(lev-VirtualBox):57721 -> 10.0.2.3(vboxdns):53  out=61B/1p in=151B/1p  age=33ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47132  10.0.2.15(lev-VirtualBox):60997 -> 10.0.2.3(vboxdns):53  out=101B/2p in=330B/2p  age=25ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47131  10.0.2.15(lev-VirtualBox):56900 -> 10.0.2.3(vboxdns):53  out=191B/2p in=432B/2p  age=39ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47133  10.0.2.15(lev-VirtualBox):39988 -> 10.0.2.3(vboxdns):53  out=191B/2p in=432B/2p  age=41ms reason=close()
OPEN  ICMPv6 pid=674(NetworkManager) cookie=9545  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55345  out=142B/1p in=56B/1p  age=5.872s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50535  out=187B/1p in=101B/1p  age=5.868s reason=idle
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=44663  10.0.2.15(lev-VirtualBox):38632 -> 150.171.109.51(no-ptr):443  out=3340B/6p in=67056B/13p  age=5.868s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46475  out=187B/1p in=101B/1p  age=5.883s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):4172  out=224B/1p in=37B/1p  age=5.964s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):49584  out=228B/1p in=37B/1p  age=5.969s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44759  out=122B/1p in=37B/1p  age=5.961s reason=idle
CLOSE ICMPv6 pid=674(NetworkManager) cookie=9545  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.727s reason=idle
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=44710  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):47082 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=45325  127.0.0.1(localhost):52147 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52147
OPEN  UDP   pid=511(systemd-resolve) cookie=47198  10.0.2.15(lev-VirtualBox):43298 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=46270  127.0.0.1(localhost):55631 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=47199  10.0.2.15(lev-VirtualBox):56244 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55631
CLOSE UDP   pid=511(systemd-resolve) cookie=47198  10.0.2.15(lev-VirtualBox):43298 -> 10.0.2.3(vboxdns):53  out=44B/1p in=247B/1p  age=41ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=46270  127.0.0.1(localhost):55631 -> 127.0.0.53(dnsstub):53  out=44B/1p in=144B/1p  age=38ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47199  10.0.2.15(lev-VirtualBox):56244 -> 10.0.2.3(vboxdns):53  out=44B/1p in=295B/1p  age=42ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=45325  127.0.0.1(localhost):52147 -> 127.0.0.53(dnsstub):53  out=88B/2p in=336B/2p  age=50ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51699
OPEN  UDP   pid=511(systemd-resolve) cookie=47200  10.0.2.15(lev-VirtualBox):33269 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=47200  10.0.2.15(lev-VirtualBox):33269 -> 10.0.2.3(vboxdns):53  out=101B/2p in=280B/2p  age=28ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55631  out=144B/1p in=44B/1p  age=5.561s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):51699  out=116B/1p in=56B/1p  age=5.5s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52147  out=336B/2p in=88B/2p  age=5.568s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443  out=3003B/7p in=1991B/16p  age=5.505s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=42014  10.0.2.15(lev-VirtualBox):43368 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=30B/1p in=26B/2p  age=5.524s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=42014  10.0.2.15(lev-VirtualBox):43368 -> 34.107.243.93(93.243.107.34.bc.googleusercontent.com):443  out=28B/1p in=24B/2p  age=5.918s reason=idle
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47303  127.0.0.1(localhost):59349 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59349
OPEN  UDP   pid=511(systemd-resolve) cookie=47305  10.0.2.15(lev-VirtualBox):39260 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44761  127.0.0.1(localhost):41277 -> 127.0.0.53(dnsstub):53
OPEN  TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41277
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=47308  127.0.0.1(localhost):37984 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=44762  10.0.2.15(lev-VirtualBox):34327 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=44763  10.0.2.15(lev-VirtualBox):60643 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33228
OPEN  UDP   pid=511(systemd-resolve) cookie=44764  10.0.2.15(lev-VirtualBox):34505 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37984
CLOSE UDP   pid=511(systemd-resolve) cookie=44762  10.0.2.15(lev-VirtualBox):34327 -> 10.0.2.3(vboxdns):53  out=58B/1p in=103B/1p  age=32ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=47308  127.0.0.1(localhost):37984 -> 127.0.0.53(dnsstub):53  out=49B/1p in=163B/1p  age=34ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47305  10.0.2.15(lev-VirtualBox):39260 -> 10.0.2.3(vboxdns):53  out=58B/1p in=142B/1p  age=45ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47303  127.0.0.1(localhost):59349 -> 127.0.0.53(dnsstub):53  out=49B/1p in=147B/1p  age=49ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=44763  10.0.2.15(lev-VirtualBox):60643 -> 10.0.2.3(vboxdns):53  out=58B/1p in=155B/1p  age=41ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=44764  10.0.2.15(lev-VirtualBox):34505 -> 10.0.2.3(vboxdns):53  out=55B/1p in=126B/1p  age=41ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44761  127.0.0.1(localhost):41277 -> 127.0.0.53(dnsstub):53  out=98B/2p in=394B/2p  age=56ms reason=close()
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47319  127.0.0.1(localhost):60868 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=46322  127.0.0.1(localhost):57360 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57360
OPEN  UDP   pid=511(systemd-resolve) cookie=45346  10.0.2.15(lev-VirtualBox):57363 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60868
OPEN  UDP   pid=511(systemd-resolve) cookie=45347  10.0.2.15(lev-VirtualBox):48525 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=45348  10.0.2.15(lev-VirtualBox):59369 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=47321  127.0.0.1(localhost):54828 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54828
OPEN  TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38499
OPEN  UDP   pid=511(systemd-resolve) cookie=45349  10.0.2.15(lev-VirtualBox):52207 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=45348  10.0.2.15(lev-VirtualBox):59369 -> 10.0.2.3(vboxdns):53  out=39B/1p in=123B/1p  age=15ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=45346  10.0.2.15(lev-VirtualBox):57363 -> 10.0.2.3(vboxdns):53  out=39B/1p in=136B/1p  age=38ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=46322  127.0.0.1(localhost):57360 -> 127.0.0.53(dnsstub):53  out=39B/1p in=123B/1p  age=46ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=45349  10.0.2.15(lev-VirtualBox):52207 -> 10.0.2.3(vboxdns):53  out=54B/1p in=123B/1p  age=32ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=45347  10.0.2.15(lev-VirtualBox):48525 -> 10.0.2.3(vboxdns):53  out=39B/1p in=65B/1p  age=55ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=47321  127.0.0.1(localhost):54828 -> 127.0.0.53(dnsstub):53  out=39B/1p in=55B/1p  age=50ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47319  127.0.0.1(localhost):60868 -> 127.0.0.53(dnsstub):53  out=78B/2p in=159B/2p  age=63ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=47325  127.0.0.1(localhost):55530 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=46335  127.0.0.1(localhost):44877 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47327  127.0.0.1(localhost):38033 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55530
OPEN  UDP   pid=511(systemd-resolve) cookie=46336  10.0.2.15(lev-VirtualBox):57318 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38033
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44877
OPEN  UDP   pid=511(systemd-resolve) cookie=46337  10.0.2.15(lev-VirtualBox):46938 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47327  127.0.0.1(localhost):38033 -> 127.0.0.53(dnsstub):53  out=116B/2p in=292B/2p  age=14ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46336  10.0.2.15(lev-VirtualBox):57318 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=19ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46337  10.0.2.15(lev-VirtualBox):46938 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=9ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=47325  127.0.0.1(localhost):55530 -> 127.0.0.53(dnsstub):53  out=52B/1p in=117B/1p  age=29ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=47329  127.0.0.1(localhost):48011 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48011
OPEN  UDP   pid=511(systemd-resolve) cookie=46338  10.0.2.15(lev-VirtualBox):52571 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=46335  127.0.0.1(localhost):44877 -> 127.0.0.53(dnsstub):53  out=104B/2p in=233B/2p  age=33ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46338  10.0.2.15(lev-VirtualBox):52571 -> 10.0.2.3(vboxdns):53  out=58B/1p in=153B/1p  age=3ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=47329  127.0.0.1(localhost):48011 -> 127.0.0.53(dnsstub):53  out=58B/1p in=123B/1p  age=15ms reason=close()
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47331  127.0.0.1(localhost):50775 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44776  127.0.0.1(localhost):43818 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50775
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=47333  127.0.0.1(localhost):34239 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46340  10.0.2.15(lev-VirtualBox):55261 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43818
OPEN  UDP   pid=511(systemd-resolve) cookie=46341  10.0.2.15(lev-VirtualBox):44834 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46342  10.0.2.15(lev-VirtualBox):49220 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34239
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=47333  127.0.0.1(localhost):34239 -> 127.0.0.53(dnsstub):53  out=124B/2p in=300B/2p  age=9ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44779  127.0.0.1(localhost):54693 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54693
OPEN  UDP   pid=511(systemd-resolve) cookie=46343  10.0.2.15(lev-VirtualBox):49732 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46343  10.0.2.15(lev-VirtualBox):49732 -> 10.0.2.3(vboxdns):53  out=62B/1p in=157B/1p  age=2ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44779  127.0.0.1(localhost):54693 -> 127.0.0.53(dnsstub):53  out=62B/1p in=127B/1p  age=7ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46340  10.0.2.15(lev-VirtualBox):55261 -> 10.0.2.3(vboxdns):53  out=58B/1p in=295B/1p  age=26ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=46344  10.0.2.15(lev-VirtualBox):51224 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46341  10.0.2.15(lev-VirtualBox):44834 -> 10.0.2.3(vboxdns):53  out=58B/1p in=535B/1p  age=27ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46342  10.0.2.15(lev-VirtualBox):49220 -> 10.0.2.3(vboxdns):53  out=58B/1p in=295B/1p  age=27ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=46345  10.0.2.15(lev-VirtualBox):47639 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46344  10.0.2.15(lev-VirtualBox):51224 -> 10.0.2.3(vboxdns):53  out=57B/1p in=169B/1p  age=36ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47331  127.0.0.1(localhost):50775 -> 127.0.0.53(dnsstub):53  out=58B/1p in=186B/1p  age=66ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46345  10.0.2.15(lev-VirtualBox):47639 -> 10.0.2.3(vboxdns):53  out=57B/1p in=169B/1p  age=55ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44776  127.0.0.1(localhost):43818 -> 127.0.0.53(dnsstub):53  out=116B/2p in=422B/2p  age=101ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33228  out=100B/1p in=55B/1p  age=5.348s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):41277  out=394B/2p in=98B/2p  age=5.353s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59349  out=147B/1p in=49B/1p  age=5.358s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37984  out=163B/1p in=49B/1p  age=5.348s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=45350  10.0.2.15(lev-VirtualBox):54298 -> 185.199.111.154(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39440
OPEN  UDP   pid=511(systemd-resolve) cookie=46347  10.0.2.15(lev-VirtualBox):33592 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46347  10.0.2.15(lev-VirtualBox):33592 -> 10.0.2.3(vboxdns):53  out=57B/1p in=129B/1p  age=73ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38499  out=98B/1p in=54B/1p  age=5.785s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60868  out=159B/2p in=78B/2p  age=5.798s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54828  out=55B/1p in=39B/1p  age=5.789s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57360  out=123B/1p in=39B/1p  age=5.8s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44877  out=233B/2p in=104B/2p  age=5.621s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38033  out=292B/2p in=116B/2p  age=5.63s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48011  out=123B/1p in=58B/1p  age=5.603s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34239  out=300B/2p in=124B/2p  age=5.585s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43818  out=422B/2p in=116B/2p  age=5.588s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):54693  out=127B/1p in=62B/1p  age=5.571s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):55530  out=117B/1p in=52B/1p  age=5.632s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):50775  out=186B/1p in=58B/1p  age=5.591s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=401B/2p in=146B/4p  age=6.091s reason=idle
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=46348  127.0.0.1(localhost):56901 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44791  127.0.0.1(localhost):47169 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56901
OPEN  UDP   pid=511(systemd-resolve) cookie=46349  10.0.2.15(lev-VirtualBox):50551 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47169
OPEN  UDP   pid=511(systemd-resolve) cookie=46350  10.0.2.15(lev-VirtualBox):37954 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44793  127.0.0.1(localhost):52219 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46351  10.0.2.15(lev-VirtualBox):40215 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52219
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59973
CLOSE UDP   pid=511(systemd-resolve) cookie=46349  10.0.2.15(lev-VirtualBox):50551 -> 10.0.2.3(vboxdns):53  out=43B/1p in=140B/1p  age=22ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46351  10.0.2.15(lev-VirtualBox):40215 -> 10.0.2.3(vboxdns):53  out=43B/1p in=127B/1p  age=19ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=46348  127.0.0.1(localhost):56901 -> 127.0.0.53(dnsstub):53  out=43B/1p in=127B/1p  age=26ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46350  10.0.2.15(lev-VirtualBox):37954 -> 10.0.2.3(vboxdns):53  out=43B/1p in=73B/1p  age=54ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44791  127.0.0.1(localhost):47169 -> 127.0.0.53(dnsstub):53  out=86B/2p in=167B/2p  age=56ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44793  127.0.0.1(localhost):52219 -> 127.0.0.53(dnsstub):53  out=43B/1p in=59B/1p  age=52ms reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443  out=7542B/18p in=118073B/203p  age=9.741s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=45350  10.0.2.15(lev-VirtualBox):54298 -> 185.199.111.154(cdn-185-199-111-154.github.com):443  out=2837B/5p in=3445B/16p  age=5.784s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39440  out=101B/1p in=57B/1p  age=5.779s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443  out=31125B/22p in=9718B/55p  age=11.352s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443
OPEN  TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=4687B/6p in=4338B/22p  age=5.776s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):47169  out=167B/2p in=86B/2p  age=5.78s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59973  out=98B/1p in=54B/1p  age=5.772s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56901  out=127B/1p in=43B/1p  age=5.781s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52219  out=59B/1p in=43B/1p  age=5.776s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.761s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60183
OPEN  UDP   pid=511(systemd-resolve) cookie=46382  10.0.2.15(lev-VirtualBox):51986 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46382  10.0.2.15(lev-VirtualBox):51986 -> 10.0.2.3(vboxdns):53  out=57B/1p in=129B/1p  age=38ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=46387  127.0.0.1(localhost):59856 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59856
OPEN  UDP   pid=511(systemd-resolve) cookie=46388  10.0.2.15(lev-VirtualBox):60402 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46389  10.0.2.15(lev-VirtualBox):55741 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46389  10.0.2.15(lev-VirtualBox):55741 -> 10.0.2.3(vboxdns):53  out=45B/1p in=174B/1p  age=39ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=46390  10.0.2.15(lev-VirtualBox):34317 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46388  10.0.2.15(lev-VirtualBox):60402 -> 10.0.2.3(vboxdns):53  out=45B/1p in=121B/1p  age=44ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46390  10.0.2.15(lev-VirtualBox):34317 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=23ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=46387  127.0.0.1(localhost):59856 -> 127.0.0.53(dnsstub):53  out=90B/2p in=209B/2p  age=68ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44839  127.0.0.1(localhost):46974 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46974
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44840  127.0.0.1(localhost):58351 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46391  10.0.2.15(lev-VirtualBox):56410 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47378  127.0.0.1(localhost):36076 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58351
OPEN  UDP   pid=511(systemd-resolve) cookie=46392  10.0.2.15(lev-VirtualBox):35380 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36076
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47378  127.0.0.1(localhost):36076 -> 127.0.0.53(dnsstub):53  out=45B/1p in=80B/1p  age=4ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46391  10.0.2.15(lev-VirtualBox):56410 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=6ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(miss):443
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44839  127.0.0.1(localhost):46974 -> 127.0.0.53(dnsstub):53  out=90B/2p in=225B/2p  age=13ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38028
OPEN  UDP   pid=511(systemd-resolve) cookie=46394  10.0.2.15(lev-VirtualBox):34530 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46392  10.0.2.15(lev-VirtualBox):35380 -> 10.0.2.3(vboxdns):53  out=44B/1p in=128B/1p  age=24ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44840  127.0.0.1(localhost):58351 -> 127.0.0.53(dnsstub):53  out=45B/1p in=145B/1p  age=28ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46394  10.0.2.15(lev-VirtualBox):34530 -> 10.0.2.3(vboxdns):53  out=55B/1p in=126B/1p  age=24ms reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=43011  10.0.2.15(lev-VirtualBox):54008 -> 140.82.113.25(lb-140-82-113-25-iad.github.com):443  out=425B/4p in=125B/6p  age=6.263s reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60183  out=101B/1p in=57B/1p  age=5.779s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443  out=2976B/5p in=1941B/14p  age=5.784s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58351  out=145B/1p in=45B/1p  age=5.515s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):38028  out=100B/1p in=55B/1p  age=5.507s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36076  out=80B/1p in=45B/1p  age=5.513s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443  out=2948B/8p in=78224B/124p  age=7.527s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59856  out=209B/2p in=90B/2p  age=5.593s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46974  out=225B/2p in=90B/2p  age=5.52s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443  out=3623B/4p in=3896B/20p  age=6.509s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443  out=11541B/10p in=2192B/20p  age=6.873s reason=idle
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=44663  10.0.2.15(lev-VirtualBox):38632 -> 150.171.109.51(no-ptr):443
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=44663  10.0.2.15(lev-VirtualBox):38632 -> 150.171.109.51(no-ptr):443  out=0B/0p in=63B/1p  age=0s reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=12864B/6p in=2132B/12p  age=10.502s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47472  127.0.0.1(localhost):59423 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59423
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44887  127.0.0.1(localhost):34212 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46432  10.0.2.15(lev-VirtualBox):55175 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34212
OPEN  UDP   pid=511(systemd-resolve) cookie=46433  10.0.2.15(lev-VirtualBox):59618 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44889  127.0.0.1(localhost):57850 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46432  10.0.2.15(lev-VirtualBox):55175 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=28ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47472  127.0.0.1(localhost):59423 -> 127.0.0.53(dnsstub):53  out=104B/2p in=233B/2p  age=39ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46433  10.0.2.15(lev-VirtualBox):59618 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=23ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57850
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44887  127.0.0.1(localhost):34212 -> 127.0.0.53(dnsstub):53  out=52B/1p in=117B/1p  age=46ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44890  127.0.0.1(localhost):53546 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53546
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44889  127.0.0.1(localhost):57850 -> 127.0.0.53(dnsstub):53  out=116B/2p in=292B/2p  age=18ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=46434  10.0.2.15(lev-VirtualBox):60678 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443
CLOSE UDP   pid=511(systemd-resolve) cookie=46434  10.0.2.15(lev-VirtualBox):60678 -> 10.0.2.3(vboxdns):53  out=58B/1p in=153B/1p  age=3ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44890  127.0.0.1(localhost):53546 -> 127.0.0.53(dnsstub):53  out=58B/1p in=123B/1p  age=52ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44894  127.0.0.1(localhost):60632 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60632
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44895  127.0.0.1(localhost):36455 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46435  10.0.2.15(lev-VirtualBox):39762 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=46437  127.0.0.1(localhost):42249 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36455
OPEN  UDP   pid=511(systemd-resolve) cookie=46438  10.0.2.15(lev-VirtualBox):47360 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42249
CLOSE UDP   pid=511(systemd-resolve) cookie=46435  10.0.2.15(lev-VirtualBox):39762 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=9ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46438  10.0.2.15(lev-VirtualBox):47360 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=7ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44894  127.0.0.1(localhost):60632 -> 127.0.0.53(dnsstub):53  out=104B/2p in=233B/2p  age=18ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44895  127.0.0.1(localhost):36455 -> 127.0.0.53(dnsstub):53  out=52B/1p in=117B/1p  age=19ms reason=close()
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44897  127.0.0.1(localhost):56630 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44899  127.0.0.1(localhost):37596 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56630
OPEN  UDP   pid=511(systemd-resolve) cookie=46439  10.0.2.15(lev-VirtualBox):44508 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37596
OPEN  UDP   pid=511(systemd-resolve) cookie=46440  10.0.2.15(lev-VirtualBox):60777 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46441  10.0.2.15(lev-VirtualBox):52093 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46439  10.0.2.15(lev-VirtualBox):44508 -> 10.0.2.3(vboxdns):53  out=58B/1p in=153B/1p  age=9ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46441  10.0.2.15(lev-VirtualBox):52093 -> 10.0.2.3(vboxdns):53  out=57B/1p in=169B/1p  age=4ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44897  127.0.0.1(localhost):56630 -> 127.0.0.53(dnsstub):53  out=58B/1p in=123B/1p  age=13ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=46437  127.0.0.1(localhost):42249 -> 127.0.0.53(dnsstub):53  out=116B/2p in=292B/2p  age=35ms reason=close()
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44900  127.0.0.1(localhost):60823 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60823
OPEN  UDP   pid=511(systemd-resolve) cookie=46442  10.0.2.15(lev-VirtualBox):46530 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46442  10.0.2.15(lev-VirtualBox):46530 -> 10.0.2.3(vboxdns):53  out=57B/1p in=169B/1p  age=5ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44900  127.0.0.1(localhost):60823 -> 127.0.0.53(dnsstub):53  out=58B/1p in=186B/1p  age=10ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58959
CLOSE UDP   pid=511(systemd-resolve) cookie=46440  10.0.2.15(lev-VirtualBox):60777 -> 10.0.2.3(vboxdns):53  out=57B/1p in=409B/1p  age=28ms reason=close()
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44902  127.0.0.1(localhost):58959 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44899  127.0.0.1(localhost):37596 -> 127.0.0.53(dnsstub):53  out=116B/2p in=500B/2p  age=37ms reason=close()
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44904  127.0.0.1(localhost):39444 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44902  127.0.0.1(localhost):58959 -> 127.0.0.53(dnsstub):53  out=124B/2p in=300B/2p  age=8ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39444
OPEN  UDP   pid=511(systemd-resolve) cookie=46443  10.0.2.15(lev-VirtualBox):54646 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46443  10.0.2.15(lev-VirtualBox):54646 -> 10.0.2.3(vboxdns):53  out=62B/1p in=157B/1p  age=3ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44904  127.0.0.1(localhost):39444 -> 127.0.0.53(dnsstub):53  out=62B/1p in=127B/1p  age=24ms reason=close()
OPEN  TCP   pid=3008(Socket Thread) cookie=47476  10.0.2.15(lev-VirtualBox):55334 -> 185.199.111.154(cdn-185-199-111-154.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=767B/2p in=1018B/4p  age=5.445s reason=idle
OPEN  UDP   pid=3008(DNS Res~ver #13) cookie=47486  127.0.0.1(localhost):33252 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44930  127.0.0.1(localhost):48122 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33252
OPEN  UDP   pid=511(systemd-resolve) cookie=46453  10.0.2.15(lev-VirtualBox):60982 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48122
OPEN  UDP   pid=3008(DNS Res~ver #14) cookie=44931  127.0.0.1(localhost):46623 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46623
OPEN  UDP   pid=3008(DNS Res~ver #18) cookie=47487  127.0.0.1(localhost):45890 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Res~ver #16) cookie=46454  127.0.0.1(localhost):58178 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=3008(DNS Res~ver #17) cookie=45491  127.0.0.1(localhost):42640 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44930  127.0.0.1(localhost):48122 -> 127.0.0.53(dnsstub):53  out=116B/2p in=292B/2p  age=6ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=46455  10.0.2.15(lev-VirtualBox):41612 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Res~ver #15) cookie=45493  127.0.0.1(localhost):52993 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42640
OPEN  UDP   pid=511(systemd-resolve) cookie=46456  10.0.2.15(lev-VirtualBox):41676 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45890
OPEN  UDP   pid=511(systemd-resolve) cookie=46457  10.0.2.15(lev-VirtualBox):39927 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52993
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58178
OPEN  UDP   pid=511(systemd-resolve) cookie=46458  10.0.2.15(lev-VirtualBox):55431 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=3008(DNS Resolver #1) cookie=44934  127.0.0.1(localhost):57622 -> 127.0.0.53(dnsstub):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46456  10.0.2.15(lev-VirtualBox):41676 -> 10.0.2.3(vboxdns):53  out=57B/1p in=169B/1p  age=10ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #15) cookie=45493  127.0.0.1(localhost):52993 -> 127.0.0.53(dnsstub):53  out=124B/2p in=300B/2p  age=13ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46453  10.0.2.15(lev-VirtualBox):60982 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=25ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46457  10.0.2.15(lev-VirtualBox):39927 -> 10.0.2.3(vboxdns):53  out=58B/1p in=153B/1p  age=13ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46455  10.0.2.15(lev-VirtualBox):41612 -> 10.0.2.3(vboxdns):53  out=52B/1p in=142B/1p  age=19ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46458  10.0.2.15(lev-VirtualBox):55431 -> 10.0.2.3(vboxdns):53  out=57B/1p in=169B/1p  age=7ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57622
CLOSE UDP   pid=3008(DNS Res~ver #17) cookie=45491  127.0.0.1(localhost):42640 -> 127.0.0.53(dnsstub):53  out=116B/2p in=500B/2p  age=19ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #14) cookie=44931  127.0.0.1(localhost):46623 -> 127.0.0.53(dnsstub):53  out=104B/2p in=233B/2p  age=24ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=46459  10.0.2.15(lev-VirtualBox):58855 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46459  10.0.2.15(lev-VirtualBox):58855 -> 10.0.2.3(vboxdns):53  out=62B/1p in=157B/1p  age=4ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #16) cookie=46454  127.0.0.1(localhost):58178 -> 127.0.0.53(dnsstub):53  out=58B/1p in=186B/1p  age=27ms reason=close()
CLOSE UDP   pid=3008(DNS Resolver #1) cookie=44934  127.0.0.1(localhost):57622 -> 127.0.0.53(dnsstub):53  out=62B/1p in=127B/1p  age=15ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #18) cookie=47487  127.0.0.1(localhost):45890 -> 127.0.0.53(dnsstub):53  out=58B/1p in=123B/1p  age=32ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #13) cookie=47486  127.0.0.1(localhost):33252 -> 127.0.0.53(dnsstub):53  out=52B/1p in=117B/1p  age=40ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):34212  out=117B/1p in=52B/1p  age=5.51s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57850  out=292B/2p in=116B/2p  age=5.481s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):53546  out=123B/1p in=58B/1p  age=5.472s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):59423  out=233B/2p in=104B/2p  age=5.525s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):39444  out=127B/1p in=62B/1p  age=5.79s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60632  out=233B/2p in=104B/2p  age=5.874s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56630  out=123B/1p in=58B/1p  age=5.849s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42249  out=292B/2p in=116B/2p  age=5.861s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):36455  out=117B/1p in=52B/1p  age=5.869s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47476  10.0.2.15(lev-VirtualBox):55334 -> 185.199.111.154(cdn-185-199-111-154.github.com):443  out=2538B/4p in=1606B/12p  age=5.431s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37596  out=500B/2p in=116B/2p  age=5.844s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):60823  out=186B/1p in=58B/1p  age=5.831s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58959  out=300B/2p in=124B/2p  age=5.816s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443  out=218B/2p in=305B/4p  age=6.468s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):58178  out=186B/1p in=58B/1p  age=5.285s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):45890  out=123B/1p in=58B/1p  age=5.291s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):52993  out=300B/2p in=124B/2p  age=5.289s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):48122  out=292B/2p in=116B/2p  age=5.302s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):33252  out=117B/1p in=52B/1p  age=5.303s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):57622  out=127B/1p in=62B/1p  age=5.277s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443  out=393B/2p in=146B/4p  age=7.241s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):46623  out=233B/2p in=104B/2p  age=5.299s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):42640  out=500B/2p in=116B/2p  age=5.295s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443  out=43601B/23p in=4921B/44p  age=15.423s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443  out=5439B/12p in=183152B/291p  age=12.507s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=3013B/3p in=1066B/6p  age=5.522s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=45350  10.0.2.15(lev-VirtualBox):54298 -> 185.199.111.154(cdn-185-199-111-154.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443
CLOSE TCP   pid=3008(Socket Thread) cookie=45350  10.0.2.15(lev-VirtualBox):54298 -> 185.199.111.154(cdn-185-199-111-154.github.com):443  out=39B/1p in=39B/2p  age=5.945s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=47322  10.0.2.15(lev-VirtualBox):60126 -> 140.82.121.4(lb-140-82-121-4-fra.github.com):443  out=63B/2p in=39B/2p  age=1ms reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443  out=39B/1p in=39B/2p  age=5.874s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=47345  10.0.2.15(lev-VirtualBox):41598 -> 140.82.121.6(lb-140-82-121-6-fra.github.com):443  out=63B/2p in=39B/2p  age=1ms reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443  out=29B/1p in=25B/2p  age=5.052s reason=idle
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47637  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):35614 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47638  127.0.0.1(localhost):5555 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47639  127.0.0.1(localhost):56879 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):5555
OPEN  UDP   pid=511(systemd-resolve) cookie=46589  10.0.2.15(lev-VirtualBox):41855 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56879
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47640  127.0.0.1(localhost):43064 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=46590  10.0.2.15(lev-VirtualBox):38886 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43064
OPEN  UDP   pid=511(systemd-resolve) cookie=46591  10.0.2.15(lev-VirtualBox):33823 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46591  10.0.2.15(lev-VirtualBox):33823 -> 10.0.2.3(vboxdns):53  out=61B/1p in=301B/1p  age=29ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=46589  10.0.2.15(lev-VirtualBox):41855 -> 10.0.2.3(vboxdns):53  out=61B/1p in=338B/1p  age=36ms reason=close()
CLOSE UDP   pid=2820(Chrome_ChildIOT) cookie=47640  127.0.0.1(localhost):43064 -> 127.0.0.53(dnsstub):53  out=50B/1p in=173B/1p  age=37ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=47641  10.0.2.15(lev-VirtualBox):53952 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=46590  10.0.2.15(lev-VirtualBox):38886 -> 10.0.2.3(vboxdns):53  out=61B/1p in=345B/1p  age=38ms reason=close()
OPEN  UDP   pid=511(systemd-resolve) cookie=47642  10.0.2.15(lev-VirtualBox):38325 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=47641  10.0.2.15(lev-VirtualBox):53952 -> 10.0.2.3(vboxdns):53  out=71B/1p in=172B/1p  age=24ms reason=close()
CLOSE UDP   pid=2820(Chrome_ChildIOT) cookie=47638  127.0.0.1(localhost):5555 -> 127.0.0.53(dnsstub):53  out=50B/1p in=214B/1p  age=66ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=47642  10.0.2.15(lev-VirtualBox):38325 -> 10.0.2.3(vboxdns):53  out=74B/1p in=179B/1p  age=37ms reason=close()
CLOSE UDP   pid=2820(Chrome_ChildIOT) cookie=47639  127.0.0.1(localhost):56879 -> 127.0.0.53(dnsstub):53  out=50B/1p in=218B/1p  age=78ms reason=close()
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=46592  10.0.2.15(lev-VirtualBox):52856 -> 13.89.179.8(miss):443
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37583
OPEN  UDP   pid=511(systemd-resolve) cookie=47643  10.0.2.15(lev-VirtualBox):39156 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=47643  10.0.2.15(lev-VirtualBox):39156 -> 10.0.2.3(vboxdns):53  out=95B/2p in=304B/2p  age=25ms reason=close()
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=45706  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):44367 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):37583  out=127B/1p in=53B/1p  age=6.182s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):56879  out=218B/1p in=50B/1p  age=6.262s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):43064  out=173B/1p in=50B/1p  age=6.26s reason=idle
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):5555  out=214B/1p in=50B/1p  age=6.262s reason=idle
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=46592  10.0.2.15(lev-VirtualBox):52856 -> 13.89.179.8(no-ptr):443  out=9971B/9p in=7039B/6p  age=9.962s reason=idle
OPEN  UDP   pid=2820(Chrome_ChildIOT) cookie=47704  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):46570 -> [2001:4860:4860:0:0:0:0:8888](dns.google):443
OPEN  TCP   pid=2820(Chrome_ChildIOT) cookie=46592  10.0.2.15(lev-VirtualBox):52856 -> 13.89.179.8(no-ptr):443
OPEN  TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=47476  10.0.2.15(lev-VirtualBox):55334 -> 185.199.111.154(cdn-185-199-111-154.github.com):443
CLOSE TCP   pid=2820(Chrome_ChildIOT) cookie=46592  10.0.2.15(lev-VirtualBox):52856 -> 13.89.179.8(no-ptr):443  out=2853B/3p in=98B/1p  age=5.942s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443
OPEN  UDP   pid=511(systemd-resolve) cookie=47715  10.0.2.15(lev-VirtualBox):53909 -> 10.0.2.3(vboxdns):53
CLOSE UDP   pid=511(systemd-resolve) cookie=47715  10.0.2.15(lev-VirtualBox):53909 -> 10.0.2.3(vboxdns):53  out=58B/1p in=742B/1p  age=40ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45734  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):54314 -> [2620:2d:4000:1:0:0:0:2a](is-content-cache-1.canonical.com):80
OPEN  TCP   pid=674(NetworkManager) cookie=45735  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):58316 -> [2620:2d:4000:1:0:0:0:22](gladys.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45734  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):54314 -> [2620:2d:4000:1:0:0:0:2a](is-content-cache-1.canonical.com):80  out=0B/0p in=0B/0p  age=11ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45736  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):37834 -> [2001:67c:1562:0:0:0:0:23](amyrose.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45735  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):58316 -> [2620:2d:4000:1:0:0:0:22](gladys.canonical.com):80  out=0B/0p in=0B/0p  age=11ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45737  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):51898 -> [2620:2d:4000:1:0:0:0:97](ubuntu-content-cache-2.ps5.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45736  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):37834 -> [2001:67c:1562:0:0:0:0:23](amyrose.canonical.com):80  out=0B/0p in=0B/0p  age=11ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45738  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):33780 -> [2620:2d:4000:1:0:0:0:98](ubuntu-content-cache-3.ps5.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45737  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):51898 -> [2620:2d:4000:1:0:0:0:97](ubuntu-content-cache-2.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=11ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45739  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):52646 -> [2620:2d:4002:1:0:0:0:198](ubuntu-content-cache-3.ps6.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45738  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):33780 -> [2620:2d:4000:1:0:0:0:98](ubuntu-content-cache-3.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=12ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45740  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):44734 -> [2620:2d:4000:1:0:0:0:96](ubuntu-content-cache-1.ps5.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45739  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):52646 -> [2620:2d:4002:1:0:0:0:198](ubuntu-content-cache-3.ps6.canonical.com):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45741  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):53586 -> [2620:2d:4000:1:0:0:0:23](fracktail.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45740  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):44734 -> [2620:2d:4000:1:0:0:0:96](ubuntu-content-cache-1.ps5.canonical.com):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45742  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):33414 -> [2001:67c:1562:0:0:0:0:24](blackcat.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45741  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):53586 -> [2620:2d:4000:1:0:0:0:23](fracktail.canonical.com):80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45743  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):37506 -> [2620:2d:4000:1:0:0:0:2b](is-content-cache-2.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45742  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):33414 -> [2001:67c:1562:0:0:0:0:24](blackcat.canonical.com):80  out=0B/0p in=0B/0p  age=12ms reason=close()
OPEN  TCP   pid=674(NetworkManager) cookie=45744  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):35770 -> [2620:2d:4002:1:0:0:0:196](ubuntu-content-cache-1.ps6.canonical.com):80
CLOSE TCP   pid=674(NetworkManager) cookie=45743  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):37506 -> [2620:2d:4000:1:0:0:0:2b](is-content-cache-2.canonical.com):80  out=0B/0p in=0B/0p  age=11ms reason=close()
CLOSE TCP   pid=674(NetworkManager) cookie=45744  [fd00:0:0:0:2bfb:57a3:ca97:6303](lev-VirtualBox):35770 -> [2620:2d:4002:1:0:0:0:196](ubuntu-content-cache-1.ps6.canonical.com):80  out=0B/0p in=0B/0p  age=1ms reason=close()
CLOSE TCP   pid=3008(Socket Thread) cookie=47476  10.0.2.15(lev-VirtualBox):55334 -> 185.199.111.154(cdn-185-199-111-154.github.com):443  out=39B/1p in=39B/2p  age=6.038s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443  out=39B/1p in=39B/2p  age=6.039s reason=idle
OPEN  ICMPv6 pid=674(NetworkManager) cookie=9545  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443  out=39B/1p in=39B/2p  age=5.82s reason=idle
CLOSE ICMPv6 pid=674(NetworkManager) cookie=9545  fe80:0:0:0:7d27:9ada:6974:f568%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=32B/1p  age=5.332s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=45350  10.0.2.15(lev-VirtualBox):54298 -> 185.199.111.154(cdn-185-199-111-154.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443
CLOSE TCP   pid=3008(Socket Thread) cookie=45326  10.0.2.15(lev-VirtualBox):49376 -> 151.101.129.91(no-ptr):443  out=63B/2p in=0B/0p  age=6ms reason=close()
OPEN  ICMPv6 pid=674(NetworkManager) cookie=9545  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)
CLOSE TCP   pid=3008(Socket Thread) cookie=45350  10.0.2.15(lev-VirtualBox):54298 -> 185.199.111.154(cdn-185-199-111-154.github.com):443  out=39B/1p in=39B/2p  age=5.916s reason=idle
CLOSE ICMPv6 pid=674(NetworkManager) cookie=9545  ff02:0:0:0:0:0:0:1%enp0s3(skip) -> fe80:0:0:0:0:0:0:2%enp0s3(skip)  out=0B/0p in=56B/1p  age=5.91s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=44841  10.0.2.15(lev-VirtualBox):38576 -> 140.82.113.26(lb-140-82-113-26-iad.github.com):443  out=29B/1p in=25B/2p  age=5.068s reason=idle
OPEN  UDP   pid=3008(DNS Res~ver #17) cookie=45834  127.0.0.1(localhost):44959 -> 127.0.0.53(dnsstub):53
OPEN  UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44959
OPEN  UDP   pid=511(systemd-resolve) cookie=48340  10.0.2.15(lev-VirtualBox):50818 -> 10.0.2.3(vboxdns):53
OPEN  UDP   pid=511(systemd-resolve) cookie=48341  10.0.2.15(lev-VirtualBox):38515 -> 10.0.2.3(vboxdns):53
OPEN  TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443
CLOSE UDP   pid=511(systemd-resolve) cookie=48341  10.0.2.15(lev-VirtualBox):38515 -> 10.0.2.3(vboxdns):53  out=58B/1p in=155B/1p  age=19ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=48340  10.0.2.15(lev-VirtualBox):50818 -> 10.0.2.3(vboxdns):53  out=58B/1p in=103B/1p  age=37ms reason=close()
CLOSE UDP   pid=3008(DNS Res~ver #17) cookie=45834  127.0.0.1(localhost):44959 -> 127.0.0.53(dnsstub):53  out=98B/2p in=394B/2p  age=38ms reason=close()
CLOSE UDP   pid=511(systemd-resolve) cookie=8240  127.0.0.53(dnsstub):53 -> 127.0.0.1(localhost):44959  out=394B/2p in=98B/2p  age=5.295s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47306  10.0.2.15(lev-VirtualBox):58146 -> 140.82.114.21(lb-140-82-114-21-iad.github.com):443  out=7739B/3p in=572B/6p  age=5.292s reason=idle
OPEN  TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443
OPEN  TCP   pid=3008(Socket Thread) cookie=47476  10.0.2.15(lev-VirtualBox):55334 -> 185.199.111.154(cdn-185-199-111-154.github.com):443
CLOSE TCP   pid=3008(Socket Thread) cookie=44837  10.0.2.15(lev-VirtualBox):40298 -> 185.199.108.133(cdn-185-199-108-133.github.com):443  out=39B/1p in=39B/2p  age=5.881s reason=idle
CLOSE TCP   pid=3008(Socket Thread) cookie=47476  10.0.2.15(lev-VirtualBox):55334 -> 185.199.111.154(cdn-185-199-111-154.github.com):443  out=39B/1p in=39B/2p  age=5.882s reason=idle






