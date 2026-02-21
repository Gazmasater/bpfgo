
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



CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:39151  out=354B/2p in=88B/2p  age=5.448s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:39156  out=194B/2p in=92B/2p  age=5.362s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:49472  out=136B/1p in=43B/1p  age=5.311s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=34374  10.0.2.15:40884 -> 140.82.113.22:443  out=7641B/3p in=576B/6p  age=5.715s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:35715  out=143B/1p in=44B/1p  age=5.457s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:56514  out=155B/1p in=44B/1p  age=5.456s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:46194  out=159B/2p in=78B/2p  age=5.255s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:34763  out=97B/1p in=40B/1p  age=5.248s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:49340  out=142B/2p in=98B/2p  age=5.163s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:55311  out=134B/1p in=59B/1p  age=5.346s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:32812  out=128B/1p in=47B/1p  age=5.365s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:38667  out=126B/1p in=46B/1p  age=5.353s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:49614  out=344B/2p in=96B/2p  age=5.436s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:34674  out=270B/2p in=94B/2p  age=5.369s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:37087  out=106B/1p in=49B/1p  age=5.152s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:40979  out=331B/2p in=118B/2p  age=5.349s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:44183  out=104B/1p in=39B/1p  age=5.212s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:40702  out=145B/1p in=48B/1p  age=5.424s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:45564  out=261B/2p in=98B/2p  age=5.72s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:50856  out=342B/2p in=86B/2p  age=5.309s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:51830  out=410B/2p in=88B/2p  age=5.455s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42308  10.0.2.15:57184 -> 74.125.131.84:443  out=2091B/4p in=4566B/12p  age=5.55s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:43689  out=68B/1p in=43B/1p  age=5.074s reason=idle
CLOSE UDP   pid=2920(Socket Thread) cookie=42304  *:49167 -> 74.125.131.84:443  out=6321B/13p in=0B/0p  age=5.608s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:57434  out=140B/2p in=96B/2p  age=5.655s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42285  10.0.2.15:35434 -> 34.120.208.123:443  out=5216B/7p in=1584B/18p  age=6.324s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:57137  out=71B/1p in=46B/1p  age=5.675s reason=idle
CLOSE UDP   pid=2920(Socket Thread) cookie=42291  *:47544 -> 64.233.164.95:443  out=3999B/5p in=0B/0p  age=6.064s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:48478  out=98B/1p in=48B/1p  age=5.655s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:37012  out=136B/2p in=92B/2p  age=5.673s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:59208  out=294B/2p in=86B/2p  age=5.076s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42295  10.0.2.15:42620 -> 142.251.1.119:443  out=2083B/4p in=6296B/12p  age=6.021s reason=idle
CLOSE UDP   pid=2920(Socket Thread) cookie=42287  *:48753 -> 142.251.1.119:443  out=4222B/4p in=0B/0p  age=6.109s reason=idle
CLOSE UDP   pid=2920(Socket Thread) cookie=42300  *:57042 -> 74.125.205.94:443  out=3993B/5p in=0B/0p  age=5.61s reason=idle
CLOSE UDP   pid=2920(Socket Thread) cookie=42360  *:46442 -> 64.233.163.104:443  out=3994B/5p in=0B/0p  age=6.034s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:51226  out=122B/1p in=58B/1p  age=5.66s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:55116  out=292B/2p in=116B/2p  age=5.662s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42530  10.0.2.15:50730 -> 151.101.129.91:443  out=3394B/7p in=2340B/14p  age=5.66s reason=idle
CLOSE UDP   pid=2920(Socket Thread) cookie=41789  *:34344 -> 64.233.164.198:443  out=30468B/324p in=0B/0p  age=15.504s reason=idle
OPEN  TCP   pid=2920(Socket Thread) cookie=41094  10.0.2.15:53934 -> 140.82.113.26:443
CLOSE TCP   pid=2920(Socket Thread) cookie=41094  10.0.2.15:53934 -> 140.82.113.26:443  out=29B/1p in=25B/2p  age=5.691s reason=idle
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44055  [fd00:0:0:0:7287:273b:7326:4219]:34021 -> [2001:4860:4860:0:0:0:0:8888]:443
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44056  127.0.0.1:29163 -> 127.0.0.53:53
OPEN  UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:29163
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44057  127.0.0.1:65430 -> 127.0.0.53:53
OPEN  UDP   pid=500(systemd-resolve) cookie=36700  10.0.2.15:32928 -> 10.0.2.3:53
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44058  127.0.0.1:61815 -> 127.0.0.53:53
OPEN  UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:65430
OPEN  UDP   pid=500(systemd-resolve) cookie=36701  10.0.2.15:40808 -> 10.0.2.3:53
OPEN  UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:61815
OPEN  UDP   pid=500(systemd-resolve) cookie=36702  10.0.2.15:37587 -> 10.0.2.3:53
CLOSE UDP   pid=500(systemd-resolve) cookie=36701  10.0.2.15:40808 -> 10.0.2.3:53  out=0B/0p in=443B/1p  age=49ms reason=close()
CLOSE UDP   pid=500(systemd-resolve) cookie=36700  10.0.2.15:32928 -> 10.0.2.3:53  out=0B/0p in=168B/1p  age=53ms reason=close()
CLOSE UDP   pid=2726(Chrome_ChildIOT) cookie=44057  127.0.0.1:65430 -> 127.0.0.53:53  out=37B/1p in=224B/1p  age=53ms reason=close()
OPEN  UDP   pid=500(systemd-resolve) cookie=36703  10.0.2.15:45104 -> 10.0.2.3:53
CLOSE UDP   pid=500(systemd-resolve) cookie=36702  10.0.2.15:37587 -> 10.0.2.3:53  out=0B/0p in=419B/1p  age=52ms reason=close()
CLOSE UDP   pid=2726(Chrome_ChildIOT) cookie=44058  127.0.0.1:61815 -> 127.0.0.53:53  out=37B/1p in=200B/1p  age=56ms reason=close()
CLOSE UDP   pid=2726(Chrome_ChildIOT) cookie=44056  127.0.0.1:29163 -> 127.0.0.53:53  out=37B/1p in=0B/0p  age=71ms reason=close()
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44059  [fd00:0:0:0:7287:273b:7326:4219]:45344 -> [2620:1ec:48:1:0:0:0:44]:80
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44060  [fd00:0:0:0:7287:273b:7326:4219]:37658 -> [2620:1ec:29:1:0:0:0:44]:80
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44061  10.0.2.15:49039 -> 13.107.253.44:80
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44062  10.0.2.15:37173 -> 13.107.226.44:80
OPEN  TCP   pid=2726(Chrome_ChildIOT) cookie=44063  [fd00:0:0:0:7287:273b:7326:4219]:43128 -> [2620:1ec:48:1:0:0:0:44]:443
CLOSE TCP   pid=2726(Chrome_ChildIOT) cookie=44063  [fd00:0:0:0:7287:273b:7326:4219]:43128 -> [2620:1ec:48:1:0:0:0:44]:443  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  TCP   pid=2726(Chrome_ChildIOT) cookie=44064  [fd00:0:0:0:7287:273b:7326:4219]:48876 -> [2620:1ec:29:1:0:0:0:44]:443
CLOSE TCP   pid=2726(Chrome_ChildIOT) cookie=44064  [fd00:0:0:0:7287:273b:7326:4219]:48876 -> [2620:1ec:29:1:0:0:0:44]:443  out=0B/0p in=0B/0p  age=0s reason=close()
OPEN  TCP   pid=2726(Chrome_ChildIOT) cookie=44065  10.0.2.15:41464 -> 13.107.253.44:443
CLOSE UDP   pid=500(systemd-resolve) cookie=36703  10.0.2.15:45104 -> 10.0.2.3:53  out=0B/0p in=151B/1p  age=25ms reason=close()
OPEN  UDP   pid=6242(go) cookie=36714  127.0.0.1:36613 -> 127.0.0.53:53
OPEN  UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:36613
OPEN  UDP   pid=6242(go) cookie=42649  127.0.0.1:48380 -> 127.0.0.53:53
OPEN  UDP   pid=500(systemd-resolve) cookie=36715  10.0.2.15:50681 -> 10.0.2.3:53
OPEN  UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:48380
OPEN  UDP   pid=500(systemd-resolve) cookie=36716  10.0.2.15:39463 -> 10.0.2.3:53
CLOSE UDP   pid=500(systemd-resolve) cookie=36715  10.0.2.15:50681 -> 10.0.2.3:53  out=0B/0p in=89B/1p  age=35ms reason=close()
CLOSE UDP   pid=500(systemd-resolve) cookie=36716  10.0.2.15:39463 -> 10.0.2.3:53  out=0B/0p in=77B/1p  age=35ms reason=close()
OPEN  UDP   pid=6242(go) cookie=44087  [fd00:0:0:0:7287:273b:7326:4219]:37265 -> [2a00:1450:4010:c0e:0:0:0:8d]:53
OPEN  UDP   pid=6242(go) cookie=44088  10.0.2.15:43214 -> 74.125.131.141:53
OPEN  TCP   pid=6242(go) cookie=44089  10.0.2.15:46988 -> 74.125.131.141:443
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:65430  out=224B/1p in=37B/1p  age=5.449s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:29163  out=228B/1p in=37B/1p  age=5.451s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:61815  out=200B/1p in=37B/1p  age=5.448s reason=idle
CLOSE TCP   pid=2726(Chrome_ChildIOT) cookie=44065  10.0.2.15:41464 -> 13.107.253.44:443  out=3404B/6p in=0B/0p  age=5.377s reason=idle
OPEN  ICMPv6 pid=673(NetworkManager) cookie=8343  * -> fe80:0:0:0:0:0:0:2
CLOSE TCP   pid=6242(go) cookie=44089  10.0.2.15:46988 -> 74.125.131.141:443  out=0B/0p in=0B/0p  age=5.01s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:36613  out=73B/1p in=45B/1p  age=5.053s reason=idle
CLOSE UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:48380  out=61B/1p in=45B/1p  age=5.051s reason=idle
OPEN  UDP   pid=2726(Chrome_ChildIOT) cookie=44110  [fd00:0:0:0:7287:273b:7326:4219]:42386 -> [2001:4860:4860:0:0:0:0:8888]:443
CLOSE ICMPv6 pid=673(NetworkManager) cookie=8343  * -> fe80:0:0:0:0:0:0:2  out=0B/0p in=32B/1p  age=5.96s reason=idle
OPEN  TCP   pid=2920(Socket Thread) cookie=42530  10.0.2.15:50730 -> 151.101.129.91:443
OPEN  TCP   pid=2920(Socket Thread) cookie=42295  10.0.2.15:42620 -> 142.251.1.119:443
OPEN  TCP   pid=2920(Socket Thread) cookie=34374  10.0.2.15:40884 -> 140.82.113.22:443
OPEN  TCP   pid=2920(Socket Thread) cookie=42285  10.0.2.15:35434 -> 34.120.208.123:443
OPEN  TCP   pid=2920(Socket Thread) cookie=42308  10.0.2.15:57184 -> 74.125.131.84:443
CLOSE TCP   pid=2920(Socket Thread) cookie=34374  10.0.2.15:40884 -> 140.82.113.22:443  out=39B/1p in=39B/2p  age=5.355s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42308  10.0.2.15:57184 -> 74.125.131.84:443  out=39B/1p in=39B/2p  age=5.354s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42285  10.0.2.15:35434 -> 34.120.208.123:443  out=39B/1p in=39B/2p  age=5.354s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42530  10.0.2.15:50730 -> 151.101.129.91:443  out=39B/1p in=39B/2p  age=5.357s reason=idle
CLOSE TCP   pid=2920(Socket Thread) cookie=42295  10.0.2.15:42620 -> 142.251.1.119:443  out=39B/1p in=39B/2p  age=5.356s reason=idle
OPEN  TCP   pid=2920(Socket Thread) cookie=25469  10.0.2.15:57298 -> 34.107.243.93:443
OPEN  TCP   pid=2920(Socket Thread) cookie=41094  10.0.2.15:53934 -> 140.82.113.26:443
OPEN  ICMPv6 pid=673(NetworkManager) cookie=8343  * -> fe80:0:0:0:0:0:0:2
OPEN  UDP   pid=2920(DNS Resolver #1) cookie=44148  127.0.0.1:35792 -> 127.0.0.53:53
OPEN  UDP   pid=500(systemd-resolve) cookie=6488  127.0.0.53:53 -> 127.0.0.1:35792
OPEN  UDP   pid=500(systemd-resolve) cookie=36766  10.0.2.15:43027 -> 10.0.2.3:53
OPEN  UDP   pid=500(systemd-resolve) cookie=36767  10.0.2.15:56892 -> 10.0.2.3:53
CLOSE UDP   pid=500(systemd-resolve) cookie=36766  10.0.2.15:43027 -> 10.0.2.3:53  out=0B/0p in=199B/1p  age=14ms reason=close()
