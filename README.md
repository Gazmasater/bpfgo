
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


sudo ss -tnp | grep "pid=3215"

sudo strace -ff -p 3215 -e trace=write,writev,sendmsg,sendmmsg -s 0 2>&1 \
 | egrep ' (write|writev|sendmsg|sendmmsg)\((22|108|127|109|129|124|107|112),'

 sudo bpftool map show | grep -n "tls_peek"
 sudo bpftool map dump id <ID>



while true; do
  openssl s_client -connect example.com:443 -servername example.com </dev/null >/dev/null 2>&1
done
sudo strace -f -e trace=sendmsg -s 0 openssl s_client -connect example.com:443 -servername example.com </dev/null >/dev/null


sudo bpftool map show | grep -n "tls_peek_map"

sudo bpftool map dump id 188



strace -f -e trace=write,writev,sendmsg,sendto -s 200 openssl s_client -connect example.com:443 -servername example.com </dev/null >/dev/null


cd ~/myprog
rm -rf gazmaster-site
npx nuxi@latest init gazmaster-site
cd gazmaster-site

Выбирай minimal – Minimal setup for Nuxt 4 (recommended) ✅

Это самый правильный старт под твою задачу (SSR + SEO + свои страницы ремонта), без лишних модулей и “контентного” слоя, который тебе сейчас только помешает.

Что дальше обычно выбрать в мастере (чтобы не усложнять):

TypeScript: Yes (удобно, меньше ошибок)

ESLint: Yes (по желанию, но полезно)

Prettier: Yes (по желанию)

Testing: No на старте (потом добавишь)

Package manager: Yarn

После создания проекта:

cd <папка-проекта>
yarn
yarn dev

Открываешь http://localhost:3000.

Если хочешь “ремонт Protherm Липецк” прямо сегодня — после запуска скажи, как называется папка проекта (или просто напиши “запустилось”), и я дам точный список файлов, которые нужно создать в Nuxt, чтобы сразу открывалось:
/lipeck/remont/protherm/oshobka-f28 и рендерился PageModel.

папка gazmaster-site

Отлично. Тогда дальше так (в папке gazmaster-site).

1) Запусти проект
cd gazmaster-site
yarn
yarn dev

Открой http://localhost:3000

2) Добавим первую SEO-страницу ремонта:

/lipeck/remont/protherm/oshobka-f28

Создай файлы.

A) Страница

pages/[region]/remont/[brand]/oshobka-[code].vue

<script setup lang="ts">
import { useRoute } from "vue-router";

const route = useRoute();
const region = String(route.params.region || "");
const brand = String(route.params.brand || "");
const code = String(route.params.code || "");

const slug = `remont-${brand}-oshibka-${code}-${region}`;

const { data, error } = await useFetch(`/api/repair/page/${slug}`, { key: slug });

if (error.value) {
  throw createError({ statusCode: 404, statusMessage: "Page not found" });
}

const page = computed(() => data.value);

useSeo(page);
useJsonLd(page);
</script>

<template>
  <main class="container">
    <Breadcrumbs v-if="page?.breadcrumbs" :items="page.breadcrumbs" />
    <h1 class="h1">{{ page?.h1 }}</h1>
    <PageRenderer v-if="page?.blocks" :blocks="page.blocks" />
  </main>
</template>

<style scoped>
.container { max-width: 920px; margin: 0 auto; padding: 24px 16px; }
.h1 { font-size: 32px; line-height: 1.15; margin: 16px 0 18px; }
</style>
B) Моковый API (чтобы работало без Go)

server/api/repair/page/[slug].get.ts

export default defineEventHandler((event) => {
  const slug = getRouterParam(event, "slug") || "";

  if (slug !== "remont-protherm-oshibka-f28-lipeck") {
    throw createError({ statusCode: 404, statusMessage: "Not found" });
  }

  const region = "Липецк";
  const phone = "+7 900 000-00-00";
  const canonicalUrl = `http://localhost:3000/lipeck/remont/protherm/oshobka-f28`;

  return {
    slug,
    title: "Ошибка F28 Protherm — ремонт в Липецке, причины и решение",
    h1: "Ошибка F28 на котле Protherm — что означает и как устранить (Липецк)",
    meta_description:
      "Ошибка F28 Protherm: частые причины, безопасные проверки и когда нужен мастер. Выезд по Липецку.",
    canonical_url: canonicalUrl,
    breadcrumbs: [
      { title: "Ремонт котлов", url: "/lipeck/remont/" },
      { title: "Protherm", url: "/lipeck/remont/protherm/" },
      { title: "Ошибка F28", url: "/lipeck/remont/protherm/oshobka-f28" },
    ],
    local_business: {
      "@context": "https://schema.org",
      "@type": "LocalBusiness",
      name: "Ремонт котлов Protherm в Липецке",
      areaServed: region,
      telephone: phone,
      address: { "@type": "PostalAddress", addressLocality: region, addressCountry: "RU" },
    },
    blocks: [
      {
        type: "intro",
        text:
          "Ошибка F28 на котлах Protherm чаще всего связана с розжигом/подачей газа. Ниже — частые причины, безопасные проверки и признаки, когда нужна диагностика мастера.",
      },
      {
        type: "causes",
        items: [
          { title: "Нет/недостаточно газа, закрыт кран", probability: 0.28 },
          { title: "Сбой розжига: электрод/ионизация, загрязнение", probability: 0.22 },
          { title: "Проблема с газовым клапаном", probability: 0.16 },
          { title: "Низкое давление газа / проблема у поставщика", probability: 0.14 },
          { title: "Ошибки по дымоудалению/тяге (зависит от модели)", probability: 0.10 },
        ],
      },
      {
        type: "steps",
        items: [
          { step: 1, title: "Проверьте газовый кран и наличие газа (например, у плиты)", safety: "low", can_user_do: true },
          { step: 2, title: "Сделайте сброс ошибки и повторный запуск котла", safety: "low", can_user_do: true },
          { step: 3, title: "Если F28 повторяется — нужна проверка узла розжига и газового клапана", safety: "high", can_user_do: false },
        ],
      },
      {
        type: "cta",
        primary: "Вызвать мастера в Липецке",
        secondary: "Консультация по F28",
        phone,
        region,
        brand: "Protherm",
        code: "F28",
      },
      {
        type: "faq",
        items: [
          {
            q: "Можно ли просто сбросить F28 и пользоваться дальше?",
            a: "Если ошибка появляется снова — это признак проблемы. Лучше найти причину, чтобы избежать повторных остановок и повреждения узлов.",
          },
          {
            q: "Опасно ли это?",
            a: "Самостоятельно ограничьтесь безопасными проверками (кран/сброс). Работы с газовой частью должен выполнять специалист.",
          },
        ],
      },
    ],
  };
});
C) Рендер блоков

Создай эти файлы:

components/PageRenderer.vue

<script setup lang="ts">
type Block =
  | { type: "intro"; text: string }
  | { type: "causes"; items: { title: string; probability?: number }[] }
  | { type: "steps"; items: { step: number; title: string; safety: "low" | "med" | "high"; can_user_do: boolean }[] }
  | { type: "faq"; items: { q: string; a: string }[] }
  | { type: "cta"; primary: string; secondary?: string; phone: string; region: string; brand: string; code: string };

defineProps<{ blocks: Block[] }>();

const map: Record<string, any> = {
  intro: resolveComponent("blocks-IntroBlock"),
  causes: resolveComponent("blocks-CausesBlock"),
  steps: resolveComponent("blocks-StepsBlock"),
  faq: resolveComponent("blocks-FaqBlock"),
  cta: resolveComponent("blocks-CtaBlock"),
};
</script>

<template>
  <section class="stack">
    <component v-for="(b, i) in blocks" :key="i" :is="map[b.type]" v-bind="b" />
  </section>
</template>

<style scoped>
.stack { display: grid; gap: 16px; }
</style>

components/Breadcrumbs.vue

<script setup lang="ts">
defineProps<{ items: { title: string; url: string }[] }>();
</script>

<template>
  <nav class="bc" aria-label="breadcrumbs">
    <a v-for="(it, i) in items" :key="i" class="link" :href="it.url">
      {{ it.title }}<span v-if="i < items.length - 1" class="sep">/</span>
    </a>
  </nav>
</template>

<style scoped>
.bc { display: flex; gap: 6px; flex-wrap: wrap; font-size: 13px; color: #666; }
.link { color: inherit; text-decoration: none; }
.link:hover { text-decoration: underline; }
.sep { margin-left: 6px; color: #bbb; }
</style>

Создай папку components/blocks/ и файлы:

components/blocks/IntroBlock.vue

<script setup lang="ts">
defineProps<{ text: string }>();
</script>

<template>
  <section class="card">
    <p class="p">{{ text }}</p>
  </section>
</template>

<style scoped>
.card { border: 1px solid #e6e6e6; border-radius: 14px; padding: 16px; }
.p { margin: 0; font-size: 16px; line-height: 1.5; }
</style>

components/blocks/CausesBlock.vue

<script setup lang="ts">
defineProps<{ items: { title: string; probability?: number }[] }>();
const pct = (v?: number) => (typeof v === "number" ? `${Math.round(v * 100)}%` : "");
</script>

<template>
  <section class="card">
    <h2 class="h2">Частые причины</h2>
    <ul class="list">
      <li v-for="(it, i) in items" :key="i" class="row">
        <span>{{ it.title }}</span>
        <span class="muted">{{ pct(it.probability) }}</span>
      </li>
    </ul>
  </section>
</template>

<style scoped>
.card { border: 1px solid #e6e6e6; border-radius: 14px; padding: 16px; }
.h2 { margin: 0 0 10px; font-size: 18px; }
.list { margin: 0; padding-left: 18px; display: grid; gap: 8px; }
.row { display: flex; justify-content: space-between; gap: 12px; }
.muted { color: #666; white-space: nowrap; }
</style>

components/blocks/StepsBlock.vue

<script setup lang="ts">
defineProps<{ items: { step: number; title: string; safety: "low" | "med" | "high"; can_user_do: boolean }[] }>();
const badge = (s: "low"|"med"|"high") => (s === "low" ? "Безопасно" : s === "med" ? "Осторожно" : "Только мастер");
</script>

<template>
  <section class="card">
    <h2 class="h2">Что можно сделать</h2>
    <ol class="steps">
      <li v-for="it in items" :key="it.step" class="step">
        <div class="top">
          <strong>Шаг {{ it.step }}.</strong>
          <span class="tag">{{ badge(it.safety) }}</span>
        </div>
        <div class="title">{{ it.title }}</div>
        <div class="muted">{{ it.can_user_do ? "Можно выполнить самостоятельно" : "Лучше доверить мастеру" }}</div>
      </li>
    </ol>
  </section>
</template>

<style scoped>
.card { border: 1px solid #e6e6e6; border-radius: 14px; padding: 16px; }
.h2 { margin: 0 0 10px; font-size: 18px; }
.steps { margin: 0; padding-left: 18px; display: grid; gap: 10px; }
.step { padding: 10px 0; border-bottom: 1px dashed #eee; }
.step:last-child { border-bottom: 0; }
.top { display: flex; justify-content: space-between; gap: 10px; }
.tag { font-size: 12px; padding: 2px 8px; border-radius: 999px; border: 1px solid #ddd; }
.title { margin-top: 4px; }
.muted { color: #666; font-size: 13px; margin-top: 4px; }
</style>

components/blocks/FaqBlock.vue

<script setup lang="ts">
defineProps<{ items: { q: string; a: string }[] }>();
</script>

<template>
  <section class="card">
    <h2 class="h2">FAQ</h2>
    <details v-for="(it, i) in items" :key="i" class="qa">
      <summary class="q">{{ it.q }}</summary>
      <p class="a">{{ it.a }}</p>
    </details>
  </section>
</template>

<style scoped>
.card { border: 1px solid #e6e6e6; border-radius: 14px; padding: 16px; }
.h2 { margin: 0 0 10px; font-size: 18px; }
.qa { padding: 8px 0; border-top: 1px solid #f1f1f1; }
.qa:first-of-type { border-top: 0; }
.q { cursor: pointer; }
.a { margin: 8px 0 0; color: #333; line-height: 1.5; }
</style>

components/blocks/CtaBlock.vue

<script setup lang="ts">
const props = defineProps<{
  primary: string;
  secondary?: string;
  phone: string;
  region: string;
  brand: string;
  code: string;
}>();

const telHref = computed(() => `tel:${props.phone.replace(/\s+/g, "")}`);
</script>

<template>
  <section class="card cta">
    <div>
      <h2 class="h2">{{ primary }}</h2>
      <p class="muted">Регион: {{ region }} · {{ brand }} · {{ code.toUpperCase() }}</p>
    </div>

    <div class="actions">
      <a class="btn" :href="telHref">Позвонить {{ phone }}</a>
      <button class="btn ghost" type="button" @click="alert('Дальше подключим /api/leads')">
        {{ secondary || "Оставить заявку" }}
      </button>
    </div>
  </section>
</template>

<style scoped>
.card { border: 1px solid #e6e6e6; border-radius: 14px; padding: 16px; }
.cta { display: flex; align-items: center; justify-content: space-between; gap: 12px; flex-wrap: wrap; }
.h2 { margin: 0; font-size: 18px; }
.muted { margin: 6px 0 0; color: #666; font-size: 13px; }
.actions { display: flex; gap: 10px; }
.btn { border: 1px solid #111; border-radius: 12px; padding: 10px 14px; background: #111; color: #fff; text-decoration: none; cursor: pointer; }
.btn.ghost { background: #fff; color: #111; }
</style>
D) SEO composables

Создай:

composables/useSeo.ts

export function useSeo(page: any) {
  if (!page?.value) return;
  const p = page.value;

  useHead({
    title: p.title,
    meta: [
      { name: "description", content: p.meta_description || "" },
      { property: "og:title", content: p.title },
      { property: "og:description", content: p.meta_description || "" },
      { property: "og:type", content: "article" },
    ],
    link: [{ rel: "canonical", href: p.canonical_url || "" }],
  });
}

composables/useJsonLd.ts

export function useJsonLd(page: any) {
  if (!page?.value) return;
  const p = page.value;

  const scripts: any[] = [];

  if (p.local_business) {
    scripts.push({ type: "application/ld+json", children: JSON.stringify(p.local_business) });
  }

  const faq = (p.blocks || []).find((b: any) => b.type === "faq");
  if (faq?.items?.length) {
    scripts.push({
      type: "application/ld+json",
      children: JSON.stringify({
        "@context": "https://schema.org",
        "@type": "FAQPage",
        mainEntity: faq.items.map((x: any) => ({
          "@type": "Question",
          name: x.q,
          acceptedAnswer: { "@type": "Answer", text: x.a },
        })),
      }),
    });
  }

  useHead({ script: scripts });
}
3) Проверь страницу

Открой:

http://localhost:3000/lipeck/remont/protherm/oshobka-f28

Если открылось — супер.




1) Проверь Node.js
node -v
npm -v

Если Node есть — ставим Yarn через npm (самый простой и правильный путь).

2) Установи Yarn (Classic)
sudo npm i -g yarn
yarn -v
3) Запусти проект

В папке gazmaster-site:

yarn dev
Если sudo npm i -g yarn ругается на права/политику

Тогда ставим через Corepack (лучше для современных окружений):

corepack enable
corepack prepare yarn@stable --activate
yarn -v
yarn dev
Если вдруг corepack не найден

Значит Node старый. Тогда обнови Node до LTS и снова:

sudo apt-get update
sudo apt-get install -y nodejs npm
sudo npm i -g yarn

После yarn dev открой http://localhost:3000.

Напиши, что выводит node -v (одна строка) и сработал ли sudo npm i -g yarn — и двинемся дальше к странице /lipeck/remont/protherm/oshobka-f28.






