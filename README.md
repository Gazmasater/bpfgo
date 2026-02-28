
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




<template>
  <NuxtLayout>
    <NuxtPage />
  </NuxtLayout>
</template>



// server/api/repair/page/[slug].get.ts
// Универсальный мок-API под страницы вида:
// /:region/remont/:brand/oshybka-:code
// slug формируем так:  remont-${brand}-oshybka-${code}-${region}
// пример: remont-protherm-oshybka-f28-lipeck

type Safety = "low" | "med" | "high";

type Block =
  | {
      type: "hero";
      title: string;
      subtitle: string;
      img?: string;
      alt?: string;
      bullets?: string[];
    }
  | { type: "intro"; text: string }
  | { type: "causes"; items: { title: string; probability?: number }[] }
  | {
      type: "steps";
      items: { step: number; title: string; safety?: Safety; can_user_do?: boolean }[];
    }
  | {
      type: "cta";
      primary: string;
      phone: string;
      region: string;
      brand: string;
      code: string;
    }
  | { type: "faq"; items: { q: string; a: string }[] };

type PageDto = {
  slug: string;
  title: string;
  h1: string;
  meta_description: string;
  canonical_url: string;
  breadcrumbs: { title: string; url: string }[];
  local_business?: any;
  blocks: Block[];
};

function cap(s: string) {
  if (!s) return s;
  return s.slice(0, 1).toUpperCase() + s.slice(1);
}

function normCode(codeRaw: string) {
  // "f28" -> "F28"
  const c = codeRaw.trim().toUpperCase();
  return c.startsWith("F") ? c : `F${c}`;
}

function regionName(regionSlug: string) {
  // пока мок: добавишь регионы как хочешь
  const map: Record<string, string> = {
    lipeck: "Липецк",
    moscow: "Москва",
    spb: "Санкт-Петербург",
  };
  return map[regionSlug] || cap(regionSlug);
}

function brandName(brandSlug: string) {
  const map: Record<string, string> = {
    protherm: "Protherm",
    baxi: "Baxi",
    vaillant: "Vaillant",
    viessmann: "Viessmann",
  };
  return map[brandSlug] || cap(brandSlug);
}

function phoneForRegion(regionSlug: string) {
  // мок: сделаешь реальную таблицу регионов позже
  const map: Record<string, string> = {
    lipeck: "+7 900 000-00-00",
  };
  return map[regionSlug] || "+7 900 000-00-00";
}

function makeCommonBlocks(params: {
  region: string;
  regionSlug: string;
  brand: string;
  brandSlug: string;
  code: string;
  heroImg?: string;
  intro: string;
  causes: { title: string; probability?: number }[];
  steps: { title: string; safety?: Safety; can_user_do?: boolean }[];
  faq: { q: string; a: string }[];
}): Block[] {
  const phone = phoneForRegion(params.regionSlug);

  return [
    {
      type: "hero",
      title: `Ошибка ${params.code} ${params.brand} — ремонт в ${params.region}`,
      subtitle:
        "Частые причины, безопасные проверки и когда нужен мастер. Выезд по городу и области.",
      img: params.heroImg, // можно не указывать — HeroBlock покажет плейсхолдер
      alt: `Ремонт котлов ${params.brand} в ${params.region}`,
      bullets: ["Выезд в день обращения", "Диагностика", "Гарантия на работы"],
    },
    { type: "intro", text: params.intro },
    { type: "causes", items: params.causes },
    {
      type: "steps",
      items: params.steps.map((s, i) => ({
        step: i + 1,
        title: s.title,
        safety: s.safety ?? "low",
        can_user_do: s.can_user_do,
      })),
    },
    {
      type: "cta",
      primary: "Вызвать мастера",
      phone,
      region: params.region,
      brand: params.brand,
      code: params.code,
    },
    { type: "faq", items: params.faq },
  ];
}

function pageFor(params: {
  slug: string;
  regionSlug: string;
  brandSlug: string;
  codeRaw: string;
}): PageDto {
  const region = regionName(params.regionSlug);
  const brand = brandName(params.brandSlug);
  const code = normCode(params.codeRaw);

  const baseUrl = `/${params.regionSlug}/remont/${params.brandSlug}/oshybka-${params.codeRaw.toLowerCase()}`;
  const canonical = `http://localhost:3000${baseUrl}`;

  const breadcrumbs = [
    { title: "Ремонт котлов", url: `/${params.regionSlug}/remont/` },
    { title: brand, url: `/${params.regionSlug}/remont/${params.brandSlug}/` },
    { title: `Ошибка ${code}`, url: baseUrl },
  ];

  // Моковый JSON-LD (потом сделаем красиво под региональные контакты)
  const phone = phoneForRegion(params.regionSlug);
  const local_business = {
    "@context": "https://schema.org",
    "@type": "LocalBusiness",
    name: `Ремонт котлов ${brand} в ${region}`,
    areaServed: region,
    telephone: phone,
    address: { "@type": "PostalAddress", addressLocality: region, addressCountry: "RU" },
  };

  // Контент по коду (можно расширять)
  const key = code.toUpperCase();

  if (params.brandSlug === "protherm") {
    switch (key) {
      case "F28": {
        const blocks = makeCommonBlocks({
          region,
          regionSlug: params.regionSlug,
          brand,
          brandSlug: params.brandSlug,
          code,
          heroImg: "/img/repair/protherm/hero.jpg",
          intro:
            "Ошибка F28 на котлах Protherm чаще всего связана с розжигом или подачей газа. Ниже — частые причины и безопасные действия, которые можно сделать без вмешательства в газовую часть.",
          causes: [
            { title: "Нет/недостаточно газа, закрыт кран", probability: 0.28 },
            { title: "Сбой розжига: электрод/ионизация, загрязнение", probability: 0.22 },
            { title: "Проблема с газовым клапаном", probability: 0.16 },
            { title: "Просадка давления газа у поставщика", probability: 0.14 },
          ],
          steps: [
            { title: "Проверьте газовый кран и наличие газа (например, у плиты)", safety: "low", can_user_do: true },
            { title: "Сделайте сброс ошибки и повторный запуск котла", safety: "low", can_user_do: true },
            { title: "Если ошибка повторяется — нужна диагностика розжига/клапана", safety: "high", can_user_do: false },
          ],
          faq: [
            {
              q: "Можно ли просто сбросить F28 и пользоваться дальше?",
              a: "Если F28 появляется снова — проблема остаётся. Лучше диагностировать причину, чтобы котёл не уходил в останов и не изнашивал узлы.",
            },
            {
              q: "Опасно ли разбирать котёл самому?",
              a: "Газовую часть и настройки должен делать специалист. Самостоятельно ограничьтесь безопасными проверками (кран/сброс/давление по манометру).",
            },
          ],
        });

        return {
          slug: params.slug,
          title: `Ошибка ${code} ${brand} — ремонт в ${region}, причины и решение`,
          h1: `Ошибка ${code} на котле ${brand} — что означает и как устранить (${region})`,
          meta_description: `Ошибка ${code} ${brand}: причины, безопасные проверки и когда нужен мастер. Выезд по ${region}.`,
          canonical_url: canonical,
          breadcrumbs,
          local_business,
          blocks,
        };
      }

      case "F29": {
        const blocks = makeCommonBlocks({
          region,
          regionSlug: params.regionSlug,
          brand,
          brandSlug: params.brandSlug,
          code,
          heroImg: "/img/repair/protherm/hero.jpg",
          intro:
            "Ошибка F29 обычно означает потерю пламени после розжига. Часто причина — нестабильная подача газа, ионизация или влияние дымоудаления (в зависимости от модели).",
          causes: [
            { title: "Нестабильная подача газа / просадки давления", probability: 0.26 },
            { title: "Электрод ионизации/контакты/загрязнение", probability: 0.22 },
            { title: "Проблемы дымоудаления/тяги", probability: 0.18 },
            { title: "Газовый клапан / настройка", probability: 0.14 },
          ],
          steps: [
            { title: "Проверьте, нет ли перебоев с газом", safety: "low", can_user_do: true },
            { title: "Перезапустите котёл (сброс ошибки)", safety: "low", can_user_do: true },
            { title: "При повторе — диагностика ионизации/газового узла/тяги", safety: "high", can_user_do: false },
          ],
          faq: [
            { q: "F29 появляется периодически — это нормально?", a: "Нет. Периодическая потеря пламени — признак проблемы, лучше проверить, чтобы избежать остановок и срывов работы." },
            { q: "Причина всегда в газе?", a: "Не всегда: бывает ионизация, тяга, настройки/узлы. Нужна диагностика по месту." },
          ],
        });

        return {
          slug: params.slug,
          title: `Ошибка ${code} ${brand} — ремонт в ${region}, причины и решение`,
          h1: `Ошибка ${code} на котле ${brand} — почему гаснет пламя (${region})`,
          meta_description: `Ошибка ${code} ${brand}: причины потери пламени и безопасные проверки. Выезд по ${region}.`,
          canonical_url: canonical,
          breadcrumbs,
          local_business,
          blocks,
        };
      }

      case "F75": {
        const blocks = makeCommonBlocks({
          region,
          regionSlug: params.regionSlug,
          brand,
          brandSlug: params.brandSlug,
          code,
          heroImg: "/img/repair/protherm/hero.jpg",
          intro:
            "Ошибка F75 связана с давлением/циркуляцией: котёл не видит ожидаемого изменения давления при запуске насоса. Частые причины — воздух в системе, датчик давления, насос, фильтр.",
          causes: [
            { title: "Воздух в системе / завоздушивание", probability: 0.28 },
            { title: "Неисправен датчик давления", probability: 0.20 },
            { title: "Насос: заклинивание/износ/питание", probability: 0.18 },
            { title: "Забит фильтр/грязевик", probability: 0.12 },
          ],
          steps: [
            { title: "Проверьте давление в системе (часто 1–1.5 бар)", safety: "low", can_user_do: true },
            { title: "Если умеете — аккуратно развоздушьте радиаторы", safety: "med", can_user_do: true },
            { title: "Если ошибка не уходит — проверка насоса/датчика/фильтра", safety: "high", can_user_do: false },
          ],
          faq: [
            { q: "F75 — это обязательно насос?", a: "Не всегда. Часто виноваты воздух, датчик давления или засор фильтра/гидравлики." },
            { q: "Можно ли продолжать пользоваться?", a: "Лучше не игнорировать: проблемы с циркуляцией ведут к перегревам и остановкам." },
          ],
        });

        return {
          slug: params.slug,
          title: `Ошибка ${code} ${brand} — ремонт в ${region}, причины и решение`,
          h1: `Ошибка ${code} на котле ${brand} — давление, насос, датчик (${region})`,
          meta_description: `Ошибка ${code} ${brand}: причины по давлению/насосу и безопасные действия. Выезд по ${region}.`,
          canonical_url: canonical,
          breadcrumbs,
          local_business,
          blocks,
        };
      }
    }
  }

  // Дефолтная страница для любых брендов/кодов (чтобы не было 404 при расширении)
  const blocks: Block[] = makeCommonBlocks({
    region,
    regionSlug: params.regionSlug,
    brand,
    brandSlug: params.brandSlug,
    code,
    heroImg: `/img/repair/${params.brandSlug}/hero.jpg`,
    intro: `Страница по ошибке ${code} для котлов ${brand}. Заполним конкретикой позже: причины, проверки, цены и сроки по региону ${region}.`,
    causes: [
      { title: "Недостаточно данных по модели (заполним после диагностики)", probability: 0.34 },
      { title: "Электрика/датчики/контакты", probability: 0.22 },
      { title: "Гидравлика/циркуляция/давление", probability: 0.18 },
    ],
    steps: [
      { title: "Сфотографируйте дисплей с ошибкой и модель котла", safety: "low", can_user_do: true },
      { title: "Сделайте перезапуск и проверьте давление/кран газа (если применимо)", safety: "low", can_user_do: true },
      { title: "Если ошибка повторяется — вызов мастера для диагностики", safety: "high", can_user_do: false },
    ],
    faq: [
      { q: "Можно ли устранить самому?", a: "Зависит от причины. Самостоятельно делайте только безопасные проверки. Газовую часть должен обслуживать специалист." },
      { q: "Сколько стоит ремонт?", a: "Стоимость зависит от причины/запчастей. Сначала делается диагностика, затем согласуется цена." },
    ],
  });

  return {
    slug: params.slug,
    title: `Ошибка ${code} ${brand} — ремонт в ${region}`,
    h1: `Ошибка ${code} на котле ${brand} (${region})`,
    meta_description: `Ошибка ${code} ${brand}: причины и решение. Выезд по ${region}.`,
    canonical_url: canonical,
    breadcrumbs,
    local_business,
    blocks,
  };
}

export default defineEventHandler((event): PageDto => {
  const slug = getRouterParam(event, "slug") || "";

  // ожидаемый формат: remont-{brand}-oshybka-{code}-{region}
  const m = slug.match(/^remont-([a-z0-9-]+)-oshybka-([a-z0-9-]+)-([a-z0-9-]+)$/i);
  if (!m) {
    throw createError({ statusCode: 404, statusMessage: `Bad slug: ${slug}` });
  }

  const [, brandSlug, codeRaw, regionSlug] = m;

  return pageFor({
    slug,
    regionSlug: regionSlug.toLowerCase(),
    brandSlug: brandSlug.toLowerCase(),
    codeRaw: codeRaw.toLowerCase(),
  });
});
