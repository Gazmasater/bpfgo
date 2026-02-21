
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



lev@lev-VirtualBox:~/bpfgo$ sudo ./bpfgo
[sudo] password for lev: 
2026/02/21 04:00:41.084685 pprof on :6060
2026/02/21 04:00:41.164681 OPEN/CLOSE only (TCP/UDP/ICMP) + PERF_LOST generation. Press Ctrl+C to exit
OPEN  UDP   pid=516(systemd-resolve) cookie=115867  10.0.2.15:36200 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=115867  10.0.2.15:36200 -> 10.0.2.3:53  out=0B/0p in=742B/1p  age=50ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115871  [fd00:0:0:0:f971:c3ee:46ee:9b71]:57974 -> [2620:2d:4000:1:0:0:0:2b]:80
OPEN  TCP   pid=669(NetworkManager) cookie=115872  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2620:2d:4000:1:0:0:0:97]:80 incomplete=1
CLOSE TCP   pid=669(NetworkManager) cookie=115871  [fd00:0:0:0:f971:c3ee:46ee:9b71]:57974 -> [2620:2d:4000:1:0:0:0:2b]:80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115873  [fd00:0:0:0:f971:c3ee:46ee:9b71]:59086 -> [2620:2d:4000:1:0:0:0:98]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115872  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2620:2d:4000:1:0:0:0:97]:80  out=0B/0p in=0B/0p  age=2ms reason=close() incomplete=1
OPEN  TCP   pid=669(NetworkManager) cookie=115874  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2620:2d:4002:1:0:0:0:196]:80 incomplete=1
CLOSE TCP   pid=669(NetworkManager) cookie=115873  [fd00:0:0:0:f971:c3ee:46ee:9b71]:59086 -> [2620:2d:4000:1:0:0:0:98]:80  out=0B/0p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115875  [fd00:0:0:0:f971:c3ee:46ee:9b71]:43802 -> [2620:2d:4002:1:0:0:0:197]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115874  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2620:2d:4002:1:0:0:0:196]:80  out=0B/0p in=0B/0p  age=2ms reason=close() incomplete=1
OPEN  TCP   pid=669(NetworkManager) cookie=115876  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2001:67c:1562:0:0:0:0:24]:80 incomplete=1
CLOSE TCP   pid=669(NetworkManager) cookie=115875  [fd00:0:0:0:f971:c3ee:46ee:9b71]:43802 -> [2620:2d:4002:1:0:0:0:197]:80  out=0B/0p in=0B/0p  age=4ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115877  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2620:2d:4002:1:0:0:0:198]:80 incomplete=1
CLOSE TCP   pid=669(NetworkManager) cookie=115876  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2001:67c:1562:0:0:0:0:24]:80  out=0B/0p in=0B/0p  age=5ms reason=close() incomplete=1
OPEN  TCP   pid=669(NetworkManager) cookie=115878  [fd00:0:0:0:f971:c3ee:46ee:9b71]:44374 -> [2620:2d:4000:1:0:0:0:22]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115877  [fd00:0:0:0:f971:c3ee:46ee:9b71]:0 -> [2620:2d:4002:1:0:0:0:198]:80  out=0B/0p in=0B/0p  age=2ms reason=close() incomplete=1
OPEN  TCP   pid=669(NetworkManager) cookie=115879  [fd00:0:0:0:f971:c3ee:46ee:9b71]:47324 -> [2620:2d:4000:1:0:0:0:96]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115878  [fd00:0:0:0:f971:c3ee:46ee:9b71]:44374 -> [2620:2d:4000:1:0:0:0:22]:80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115880  [fd00:0:0:0:f971:c3ee:46ee:9b71]:53166 -> [2620:2d:4000:1:0:0:0:2a]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115879  [fd00:0:0:0:f971:c3ee:46ee:9b71]:47324 -> [2620:2d:4000:1:0:0:0:96]:80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115881  [fd00:0:0:0:f971:c3ee:46ee:9b71]:60404 -> [2001:67c:1562:0:0:0:0:23]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115880  [fd00:0:0:0:f971:c3ee:46ee:9b71]:53166 -> [2620:2d:4000:1:0:0:0:2a]:80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  TCP   pid=669(NetworkManager) cookie=115882  [fd00:0:0:0:f971:c3ee:46ee:9b71]:46790 -> [2620:2d:4000:1:0:0:0:23]:80
CLOSE TCP   pid=669(NetworkManager) cookie=115881  [fd00:0:0:0:f971:c3ee:46ee:9b71]:60404 -> [2001:67c:1562:0:0:0:0:23]:80  out=0B/0p in=0B/0p  age=0s reason=close()
CLOSE TCP   pid=669(NetworkManager) cookie=115882  [fd00:0:0:0:f971:c3ee:46ee:9b71]:46790 -> [2620:2d:4000:1:0:0:0:23]:80  out=0B/0p in=0B/0p  age=1ms reason=close()
OPEN  ICMPv6 pid=669(NetworkManager) cookie=7103  * -> fe80:0:0:0:0:0:0:2
CLOSE ICMPv6 pid=669(NetworkManager) cookie=7103  * -> fe80:0:0:0:0:0:0:2  out=0B/0p in=32B/1p  age=5.857s reason=idle
OPEN  TCP   pid=4131(Socket Thread) cookie=109702  10.0.2.15:52056 -> 64.233.164.198:443
CLOSE TCP   pid=4131(Socket Thread) cookie=109702  10.0.2.15:52056 -> 64.233.164.198:443  out=63B/2p in=0B/0p  age=2ms reason=close()
OPEN  TCP   pid=4131(Socket Thread) cookie=112994  10.0.2.15:57710 -> 140.82.113.22:443
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=112996  127.0.0.1:59005 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114042  127.0.0.1:58216 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:59005
OPEN  UDP   pid=516(systemd-resolve) cookie=112997  10.0.2.15:40078 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:58216
OPEN  UDP   pid=516(systemd-resolve) cookie=112998  10.0.2.15:50582 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=112998  10.0.2.15:50582 -> 10.0.2.3:53  out=0B/0p in=142B/1p  age=15ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=112997  10.0.2.15:40078 -> 10.0.2.3:53  out=0B/0p in=103B/1p  age=47ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=114042  127.0.0.1:58216 -> 127.0.0.53:53  out=0B/0p in=310B/2p  age=57ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=112996  127.0.0.1:59005 -> 127.0.0.53:53  out=49B/1p in=163B/1p  age=62ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114044  127.0.0.1:45839 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:45839
OPEN  UDP   pid=516(systemd-resolve) cookie=114046  10.0.2.15:49849 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=114047  10.0.2.15:38060 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114046  10.0.2.15:49849 -> 10.0.2.3:53  out=0B/0p in=199B/1p  age=3ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114047  10.0.2.15:38060 -> 10.0.2.3:53  out=0B/0p in=211B/1p  age=6ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #28) cookie=114044  127.0.0.1:45839 -> 127.0.0.53:53  out=0B/0p in=254B/2p  age=20ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114049  10.0.2.15:35179 -> 64.233.164.198:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114051  [fd00:0:0:0:f971:c3ee:46ee:9b71]:35084 -> [2a00:1450:4010:c07:0:0:0:c6]:0 incomplete=1
OPEN  UDP   pid=4131(Socket Thread) cookie=114045  *:43449 -> 64.233.164.198:443
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114052  127.0.0.1:56711 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=113001  127.0.0.1:40848 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:56711
OPEN  UDP   pid=516(systemd-resolve) cookie=114053  10.0.2.15:48493 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:40848
OPEN  UDP   pid=516(systemd-resolve) cookie=114054  10.0.2.15:35660 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=114055  10.0.2.15:37991 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114053  10.0.2.15:48493 -> 10.0.2.3:53  out=0B/0p in=119B/1p  age=34ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=114052  127.0.0.1:56711 -> 127.0.0.53:53  out=40B/1p in=97B/1p  age=38ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114054  10.0.2.15:35660 -> 10.0.2.3:53  out=0B/0p in=337B/1p  age=36ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114055  10.0.2.15:37991 -> 10.0.2.3:53  out=0B/0p in=196B/1p  age=39ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #26) cookie=113001  127.0.0.1:40848 -> 127.0.0.53:53  out=0B/0p in=368B/2p  age=53ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=113002  10.0.2.15:43119 -> 64.233.162.119:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114056  [fd00:0:0:0:f971:c3ee:46ee:9b71]:46025 -> [2a00:1450:4010:c01:0:0:0:77]:0 incomplete=1
OPEN  UDP   pid=4131(Socket Thread) cookie=114057  *:42388 -> 74.125.205.119:443
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114087  127.0.0.1:50202 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:50202
OPEN  UDP   pid=516(systemd-resolve) cookie=114088  10.0.2.15:58999 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=114090  10.0.2.15:57622 -> 10.0.2.3:53
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=113003  127.0.0.1:44479 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:44479
OPEN  UDP   pid=516(systemd-resolve) cookie=114092  10.0.2.15:59954 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114090  10.0.2.15:57622 -> 10.0.2.3:53  out=0B/0p in=95B/1p  age=46ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114088  10.0.2.15:58999 -> 10.0.2.3:53  out=0B/0p in=83B/1p  age=57ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114092  10.0.2.15:59954 -> 10.0.2.3:53  out=0B/0p in=128B/1p  age=48ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #28) cookie=114087  127.0.0.1:50202 -> 127.0.0.53:53  out=0B/0p in=140B/2p  age=67ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=113003  127.0.0.1:44479 -> 127.0.0.53:53  out=48B/1p in=98B/1p  age=62ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114096  [fd00:0:0:0:f971:c3ee:46ee:9b71]:49703 -> [2a00:1450:4010:c02:0:0:0:54]:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114096  *:58200 -> 74.125.205.84:0 incomplete=1
OPEN  UDP   pid=4131(Socket Thread) cookie=116103  *:42705 -> 74.125.205.84:443
OPEN  TCP   pid=4131(Socket Thread) cookie=116107  10.0.2.15:57496 -> 74.125.205.84:443
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114098  127.0.0.1:56141 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:56141
OPEN  UDP   pid=516(systemd-resolve) cookie=114099  10.0.2.15:48900 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=114100  10.0.2.15:45456 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114100  10.0.2.15:45456 -> 10.0.2.3:53  out=0B/0p in=211B/1p  age=45ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114099  10.0.2.15:48900 -> 10.0.2.3:53  out=0B/0p in=223B/1p  age=50ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=114098  127.0.0.1:56141 -> 127.0.0.53:53  out=0B/0p in=294B/2p  age=51ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114119  [fd00:0:0:0:f971:c3ee:46ee:9b71]:42808 -> [2a00:1450:4010:c02:0:0:0:69]:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114119  *:47799 -> 108.177.14.103:0 incomplete=1
OPEN  UDP   pid=4131(Socket Thread) cookie=114129  *:40849 -> 108.177.14.103:443
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114211  127.0.0.1:39864 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=115059  127.0.0.1:56332 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:39864
OPEN  UDP   pid=516(systemd-resolve) cookie=114212  10.0.2.15:42361 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=113025  10.0.2.15:50980 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:56332
OPEN  UDP   pid=516(systemd-resolve) cookie=113026  10.0.2.15:56658 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=113025  10.0.2.15:50980 -> 10.0.2.3:53  out=0B/0p in=276B/1p  age=62ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114212  10.0.2.15:42361 -> 10.0.2.3:53  out=0B/0p in=228B/1p  age=66ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=113026  10.0.2.15:56658 -> 10.0.2.3:53  out=0B/0p in=108B/1p  age=60ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=115059  127.0.0.1:56332 -> 127.0.0.53:53  out=56B/1p in=81B/1p  age=71ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #26) cookie=114211  127.0.0.1:39864 -> 127.0.0.53:53  out=0B/0p in=288B/2p  age=75ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114213  [fd00:0:0:0:f971:c3ee:46ee:9b71]:55650 -> [2a00:1450:4010:c0f:0:0:0:9c]:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114213  *:56979 -> 108.177.14.155:0 incomplete=1
OPEN  UDP   pid=4131(Socket Thread) cookie=113027  *:40619 -> 108.177.14.155:443
OPEN  TCP   pid=4131(Socket Thread) cookie=113031  10.0.2.15:40066 -> 108.177.14.155:443
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114215  127.0.0.1:55604 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114216  127.0.0.1:55250 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:55604
OPEN  UDP   pid=516(systemd-resolve) cookie=113034  10.0.2.15:48652 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=113035  10.0.2.15:59926 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:55250
OPEN  UDP   pid=516(systemd-resolve) cookie=113036  10.0.2.15:42807 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=113034  10.0.2.15:48652 -> 10.0.2.3:53  out=0B/0p in=187B/1p  age=46ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=113035  10.0.2.15:59926 -> 10.0.2.3:53  out=0B/0p in=199B/1p  age=50ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=114215  127.0.0.1:55604 -> 127.0.0.53:53  out=0B/0p in=228B/2p  age=59ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114217  10.0.2.15:49746 -> 173.194.18.9:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114218  [fd00:0:0:0:f971:c3ee:46ee:9b71]:47650 -> [2a00:1450:4006:6:0:0:0:9]:0 incomplete=1
CLOSE UDP   pid=516(systemd-resolve) cookie=113036  10.0.2.15:42807 -> 10.0.2.3:53  out=0B/0p in=193B/1p  age=42ms reason=close()
OPEN  TCP   pid=4131(Socket Thread) cookie=113037  10.0.2.15:34096 -> 173.194.18.9:443
CLOSE UDP   pid=4131(DNS Res~ver #28) cookie=114216  127.0.0.1:55250 -> 127.0.0.53:53  out=62B/1p in=114B/1p  age=68ms reason=close()
OPEN  TCP   pid=4131(Socket Thread) cookie=113038  10.0.2.15:34102 -> 173.194.18.9:443
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114220  127.0.0.1:43951 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:43951
CLOSE TCP   pid=4131(Socket Thread) cookie=113037  10.0.2.15:34096 -> 173.194.18.9:443  out=0B/0p in=0B/0p  age=21ms reason=close()
CLOSE TCP   pid=4131(Socket Thread) cookie=113038  10.0.2.15:34102 -> 173.194.18.9:443  out=0B/0p in=0B/0p  age=15ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=114220  127.0.0.1:43951 -> 127.0.0.53:53  out=0B/0p in=164B/2p  age=16ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114221  10.0.2.15:40843 -> 173.194.18.9:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114222  [fd00:0:0:0:f971:c3ee:46ee:9b71]:45024 -> [2a00:1450:4006:6:0:0:0:9]:0 incomplete=1
OPEN  UDP   pid=4131(Socket Thread) cookie=113039  *:40498 -> 173.194.18.9:443
OPEN  TCP   pid=4131(Socket Thread) cookie=113043  10.0.2.15:34104 -> 173.194.18.9:443
OPEN  TCP   pid=4131(Socket Thread) cookie=113044  10.0.2.15:34120 -> 173.194.18.9:443
CLOSE TCP   pid=4131(Socket Thread) cookie=112994  10.0.2.15:57710 -> 140.82.113.22:443  out=10514B/6p in=6383B/22p  age=5.798s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:56711  out=97B/1p in=40B/1p  age=5.435s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:45839  out=254B/2p in=88B/2p  age=5.694s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:58216  out=310B/2p in=98B/2p  age=5.772s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:40848  out=368B/2p in=80B/2p  age=5.433s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=114057  *:42388 -> 74.125.205.119:443  out=5389B/7p in=0B/0p  age=5.335s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:59005  out=163B/1p in=49B/1p  age=5.775s reason=idle
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114224  127.0.0.1:41041 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=114225  127.0.0.1:48775 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:41041
OPEN  UDP   pid=516(systemd-resolve) cookie=113045  10.0.2.15:50802 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=113046  10.0.2.15:52250 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:48775
OPEN  UDP   pid=516(systemd-resolve) cookie=113047  10.0.2.15:44525 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=113045  10.0.2.15:50802 -> 10.0.2.3:53  out=0B/0p in=71B/1p  age=36ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=113046  10.0.2.15:52250 -> 10.0.2.3:53  out=0B/0p in=83B/1p  age=36ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #26) cookie=114224  127.0.0.1:41041 -> 127.0.0.53:53  out=0B/0p in=128B/2p  age=51ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114226  10.0.2.15:56396 -> 142.251.1.94:0 incomplete=1
CLOSE UDP   pid=516(systemd-resolve) cookie=113047  10.0.2.15:44525 -> 10.0.2.3:53  out=0B/0p in=80B/1p  age=38ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114227  [fd00:0:0:0:f971:c3ee:46ee:9b71]:40506 -> [2a00:1450:4010:c1e:0:0:0:5e]:0 incomplete=1
OPEN  TCP   pid=4131(Socket Thread) cookie=113048  10.0.2.15:57186 -> 142.251.1.94:443
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=114225  127.0.0.1:48775 -> 127.0.0.53:53  out=42B/1p in=67B/1p  age=71ms reason=close()
OPEN  UDP   pid=4131(Socket Thread) cookie=114228  *:45679 -> 142.251.1.94:443
CLOSE TCP   pid=4131(Socket Thread) cookie=113048  10.0.2.15:57186 -> 142.251.1.94:443  out=2646B/7p in=6683B/20p  age=367ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:50202  out=140B/2p in=96B/2p  age=5.439s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=116107  10.0.2.15:57496 -> 74.125.205.84:443  out=2688B/5p in=1951B/12p  age=5.314s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:44479  out=98B/1p in=48B/1p  age=5.425s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=116103  *:42705 -> 74.125.205.84:443  out=6834B/10p in=0B/0p  age=6.318s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:56141  out=294B/2p in=86B/2p  age=5.393s reason=idle
OPEN  TCP   pid=4131(Socket Thread) cookie=102736  10.0.2.15:32798 -> 34.107.243.93:443
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:39864  out=288B/2p in=112B/2p  age=5.121s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:56332  out=81B/1p in=56B/1p  age=5.114s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=113031  10.0.2.15:40066 -> 108.177.14.155:443  out=2099B/4p in=6117B/12p  age=5.938s reason=idle
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:47455
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=114272  127.0.0.1:47455 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114273  127.0.0.1:37203 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=113064  10.0.2.15:48446 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=113065  10.0.2.15:49822 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:37203
OPEN  UDP   pid=516(systemd-resolve) cookie=113066  10.0.2.15:58153 -> 10.0.2.3:53
CLOSE TCP   pid=4131(Socket Thread) cookie=113044  10.0.2.15:34120 -> 173.194.18.9:443  out=2006B/3p in=4626B/6p  age=5.222s reason=close()
CLOSE TCP   pid=4131(Socket Thread) cookie=113043  10.0.2.15:34104 -> 173.194.18.9:443  out=2006B/3p in=4626B/6p  age=5.234s reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=113065  10.0.2.15:49822 -> 10.0.2.3:53  out=0B/0p in=196B/1p  age=48ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=113064  10.0.2.15:48446 -> 10.0.2.3:53  out=0B/0p in=148B/1p  age=54ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #28) cookie=114272  127.0.0.1:47455 -> 127.0.0.53:53  out=0B/0p in=256B/2p  age=63ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=115408  [fd00:0:0:0:f971:c3ee:46ee:9b71]:52136 -> [2a00:1450:4010:c07:0:0:0:5d]:0 incomplete=1
CLOSE UDP   pid=516(systemd-resolve) cookie=113066  10.0.2.15:58153 -> 10.0.2.3:53  out=0B/0p in=66B/1p  age=92ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=114273  127.0.0.1:37203 -> 127.0.0.53:53  out=40B/1p in=55B/1p  age=100ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=114275  127.0.0.1:46905 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:46905
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=115408  *:40992 -> 64.233.164.93:0 incomplete=1
CLOSE UDP   pid=4131(DNS Res~ver #26) cookie=114275  127.0.0.1:46905 -> 127.0.0.53:53  out=40B/1p in=104B/1p  age=6ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=113067  10.0.2.15:43928 -> 64.233.164.136:0 incomplete=1
OPEN  TCP   pid=4131(Socket Thread) cookie=115409  10.0.2.15:60648 -> 64.233.164.136:443
OPEN  TCP   pid=4131(Socket Thread) cookie=115410  10.0.2.15:60660 -> 64.233.164.136:443
CLOSE UDP   pid=4131(Socket Thread) cookie=113027  *:40619 -> 108.177.14.155:443  out=7082B/9p in=0B/0p  age=6.975s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:43951  out=164B/2p in=120B/2p  age=5.512s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:55250  out=114B/1p in=62B/1p  age=5.583s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:55604  out=228B/2p in=124B/2p  age=5.59s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:48775  out=67B/1p in=42B/1p  age=5.959s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=113039  *:40498 -> 173.194.18.9:443  out=9755B/14p in=0B/0p  age=6.463s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=114129  *:40849 -> 108.177.14.103:443  out=5734B/5p in=0B/0p  age=9.271s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=114228  *:45679 -> 142.251.1.94:443  out=3996B/5p in=0B/0p  age=5.861s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:41041  out=128B/2p in=84B/2p  age=5.97s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=102736  10.0.2.15:32798 -> 34.107.243.93:443  out=28B/1p in=24B/2p  age=5.935s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:47455  out=256B/2p in=80B/2p  age=5.245s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:37203  out=55B/1p in=40B/1p  age=5.241s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=114045  *:43449 -> 64.233.164.198:443  out=21409B/197p in=0B/0p  age=15.666s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:46905  out=104B/1p in=40B/1p  age=5.135s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=115410  10.0.2.15:60660 -> 64.233.164.136:443  out=3002B/6p in=4325B/20p  age=6.098s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=115409  10.0.2.15:60648 -> 64.233.164.136:443  out=2680B/5p in=1951B/12p  age=6.106s reason=idle
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=116637  127.0.0.1:35577 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114362  127.0.0.1:53705 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:35577
OPEN  UDP   pid=516(systemd-resolve) cookie=114363  10.0.2.15:46785 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:53705
OPEN  UDP   pid=516(systemd-resolve) cookie=114364  10.0.2.15:50504 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=114365  10.0.2.15:34690 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114363  10.0.2.15:46785 -> 10.0.2.3:53  out=0B/0p in=136B/1p  age=47ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=116637  127.0.0.1:35577 -> 127.0.0.53:53  out=51B/1p in=111B/1p  age=52ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114364  10.0.2.15:50504 -> 10.0.2.3:53  out=0B/0p in=127B/1p  age=47ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114365  10.0.2.15:34690 -> 10.0.2.3:53  out=0B/0p in=151B/1p  age=46ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=114362  127.0.0.1:53705 -> 127.0.0.53:53  out=0B/0p in=190B/2p  age=59ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114366  10.0.2.15:42783 -> 108.177.14.148:0 incomplete=1
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=114367  [fd00:0:0:0:f971:c3ee:46ee:9b71]:54638 -> [2a00:1450:4010:c0f:0:0:0:95]:0 incomplete=1
OPEN  TCP   pid=4131(Socket Thread) cookie=113133  10.0.2.15:38634 -> 108.177.14.148:443
OPEN  UDP   pid=4131(Socket Thread) cookie=114045  *:43449 -> 64.233.164.198:443
OPEN  UDP   pid=4131(Socket Thread) cookie=115470  *:55578 -> 108.177.14.148:443
CLOSE TCP   pid=4131(Socket Thread) cookie=113133  10.0.2.15:38634 -> 108.177.14.148:443  out=2513B/8p in=6268B/14p  age=521ms reason=close()
OPEN  TCP   pid=4131(Socket Thread) cookie=111390  10.0.2.15:54134 -> 151.101.65.91:443
OPEN  TCP   pid=4131(Socket Thread) cookie=111377  10.0.2.15:54130 -> 151.101.65.91:443
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:53705  out=190B/2p in=102B/2p  age=5.876s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=114045  *:43449 -> 64.233.164.198:443  out=1481B/5p in=0B/0p  age=5.762s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:35577  out=111B/1p in=51B/1p  age=5.878s reason=idle
CLOSE UDP   pid=4131(Socket Thread) cookie=115470  *:55578 -> 108.177.14.148:443  out=5250B/6p in=0B/0p  age=5.4s reason=idle
OPEN  TCP   pid=4131(Socket Thread) cookie=90891  10.0.2.15:51126 -> 140.82.113.25:443
OPEN  UDP   pid=4131(DNS Res~ver #28) cookie=115489  127.0.0.1:41964 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:41964
OPEN  UDP   pid=516(systemd-resolve) cookie=114478  10.0.2.15:48199 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=114479  10.0.2.15:48414 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114478  10.0.2.15:48199 -> 10.0.2.3:53  out=0B/0p in=121B/1p  age=45ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114479  10.0.2.15:48414 -> 10.0.2.3:53  out=0B/0p in=174B/1p  age=45ms reason=close()
OPEN  UDP   pid=516(systemd-resolve) cookie=114480  10.0.2.15:54325 -> 10.0.2.3:53
CLOSE UDP   pid=516(systemd-resolve) cookie=114480  10.0.2.15:54325 -> 10.0.2.3:53  out=0B/0p in=128B/1p  age=32ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #28) cookie=115489  127.0.0.1:41964 -> 127.0.0.53:53  out=0B/0p in=209B/2p  age=82ms reason=close()
OPEN  UDP   pid=4131(DNS Res~ver #23) cookie=116680  127.0.0.1:45651 -> 127.0.0.53:53
OPEN  UDP   pid=4131(DNS Res~ver #26) cookie=113287  127.0.0.1:43610 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:45651
OPEN  UDP   pid=516(systemd-resolve) cookie=114481  10.0.2.15:40170 -> 10.0.2.3:53
OPEN  UDP   pid=4131(DNS Res~ver #25) cookie=115491  127.0.0.1:37648 -> 127.0.0.53:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:43610
OPEN  UDP   pid=516(systemd-resolve) cookie=114482  10.0.2.15:37441 -> 10.0.2.3:53
OPEN  UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:37648
CLOSE UDP   pid=4131(DNS Res~ver #25) cookie=115491  127.0.0.1:37648 -> 127.0.0.53:53  out=45B/1p in=145B/1p  age=3ms reason=close()
CLOSE UDP   pid=516(systemd-resolve) cookie=114482  10.0.2.15:37441 -> 10.0.2.3:53  out=0B/0p in=128B/1p  age=2ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #26) cookie=113287  127.0.0.1:43610 -> 127.0.0.53:53  out=0B/0p in=274B/2p  age=12ms reason=close()
OPEN  TCP   pid=4131(Socket Thread) cookie=113288  10.0.2.15:42440 -> 140.82.114.26:443
CLOSE UDP   pid=516(systemd-resolve) cookie=114481  10.0.2.15:40170 -> 10.0.2.3:53  out=0B/0p in=141B/1p  age=33ms reason=close()
CLOSE UDP   pid=4131(DNS Res~ver #23) cookie=116680  127.0.0.1:45651 -> 127.0.0.53:53  out=45B/1p in=213B/1p  age=36ms reason=close()
CLOSE TCP   pid=4131(Socket Thread) cookie=90891  10.0.2.15:51126 -> 140.82.113.25:443  out=69B/2p in=41B/2p  age=1.02s reason=close()
CLOSE TCP   pid=4131(Socket Thread) cookie=111377  10.0.2.15:54130 -> 151.101.65.91:443  out=39B/1p in=39B/2p  age=5.796s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=111390  10.0.2.15:54134 -> 151.101.65.91:443  out=39B/1p in=39B/2p  age=5.797s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:45651  out=213B/1p in=45B/1p  age=5.248s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:43610  out=274B/2p in=90B/2p  age=5.247s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:37648  out=145B/1p in=45B/1p  age=5.246s reason=idle
CLOSE UDP   pid=516(systemd-resolve) cookie=7402  127.0.0.53:53 -> 127.0.0.1:41964  out=209B/2p in=90B/2p  age=5.334s reason=idle
CLOSE TCP   pid=4131(Socket Thread) cookie=113288  10.0.2.15:42440 -> 140.82.114.26:443  out=3653B/4p in=3896B/20p  age=6.234s reason=idle
^Z
