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
GIT

git reset --hard HEAD
____________________________________________________



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


func resolveHost(ip net.IP) string {

	key := ip.String()

	cacheMu.RLock()
	if host, ok := resolveCache[key]; ok {
		cacheMu.RUnlock()
		return host
	}
	cacheMu.RUnlock()

	var host string
	if ip.To4() != nil {
		if ip.IsLoopback() {
			host = "localhost"
		} else {
			host = pkg.ResolveIP(ip)

		}
	} else {
		var err error
		host, err = pkg.ResolveIP_n(ip)
		if err != nil {
			host = "unknown"
		}
	}

	cacheMu.Lock()
	resolveCache[key] = host
	cacheMu.Unlock()

	return host
}







_______________________________________________________________________________________________


go tool pprof http://localhost:6060/debug/pprof/profile?seconds=30
go tool pprof http://localhost:6060/debug/pprof/heap



"net/http"      // добавлено для pprof
	_ "net/http/pprof" // регистрирует pprof-эндпоинты

     _ "net/http/pprof" // blank-import: регистрирует pprof-хэндлеры в DefaultServeMux



git checkout ProcNet_monitor
git push --force origin ProcNet_monitor


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

gaz358@gaz358-BOD-WXX9:~/myprog/workmate/repository/memory$ go tool pprof cpu.out
File: memory.test
Build ID: 2f6c1199a29a5976356fbe59410e4e2dff661832
Type: cpu
Time: 2025-07-06 07:10:00 MSK
Duration: 9.23s, Total samples = 43.01s (465.85%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 17630ms, 40.99% of 43010ms total
Dropped 246 nodes (cum <= 215.05ms)
Showing top 10 nodes out of 157
      flat  flat%   sum%        cum   cum%
    6450ms 15.00% 15.00%     6450ms 15.00%  runtime.procyield
    2950ms  6.86% 21.86%     2950ms  6.86%  runtime.futex
    1530ms  3.56% 25.41%     5990ms 13.93%  runtime.lock2
    1490ms  3.46% 28.88%     1820ms  4.23%  runtime.casgstatus
    1060ms  2.46% 31.34%    10310ms 23.97%  internal/sync.(*Mutex).lockSlow
    1010ms  2.35% 33.69%     1010ms  2.35%  internal/runtime/maps.ctrlGroup.matchH2
     880ms  2.05% 35.74%     1100ms  2.56%  runtime.findObject
     790ms  1.84% 37.57%      790ms  1.84%  runtime.memmove
     740ms  1.72% 39.29%      740ms  1.72%  runtime.cansemacquire (inline)
     730ms  1.70% 40.99%     2930ms  6.81%  runtime.scanobject
(pprof) 


gaz358@gaz358-BOD-WXX9:~/myprog/workmate/repository/memory$ go tool pprof cpu.out
File: memory.test
Build ID: 15a16dd8b0e468d66fe1cb84fe795405a625d0b4
Type: cpu
Time: 2025-07-06 07:21:45 MSK
Duration: 5.03s, Total samples = 25.54s (507.39%)
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 8130ms, 31.83% of 25540ms total
Dropped 203 nodes (cum <= 127.70ms)
Showing top 10 nodes out of 137
      flat  flat%   sum%        cum   cum%
    1260ms  4.93%  4.93%     1530ms  5.99%  runtime.casgstatus
    1020ms  3.99%  8.93%     3790ms 14.84%  runtime.scanobject
     870ms  3.41% 12.33%      870ms  3.41%  runtime.memmove
     860ms  3.37% 15.70%      860ms  3.37%  internal/runtime/maps.ctrlGroup.matchH2
     860ms  3.37% 19.07%     1000ms  3.92%  runtime.findObject
     690ms  2.70% 21.77%      690ms  2.70%  runtime.(*mspan).base (inline)
     660ms  2.58% 24.35%      950ms  3.72%  runtime.runqgrab
     650ms  2.55% 26.90%      650ms  2.55%  runtime.(*mspan).heapBitsSmallForAddr
     650ms  2.55% 29.44%      650ms  2.55%  sync/atomic.(*Int32).Add
     610ms  2.39% 31.83%      610ms  2.39%  sync/atomic.(*Uint64).Add
(pprof) 


go test -run TestInMemoryRepo_Concurrency -memprofile=mem.out
go tool pprof mem.out

gaz358@gaz358-BOD-WXX9:~/myprog/workmate/repository/memory$ go tool pprof mem.out
File: memory.test
Build ID: 60b88d25411cc64e42284368b18d7593c2476995
Type: alloc_space
Time: 2025-07-06 07:45:24 MSK
Entering interactive mode (type "help" for commands, "o" for options)
(pprof) top
Showing nodes accounting for 865.82MB, 99.54% of 869.83MB total
Dropped 16 nodes (cum <= 4.35MB)
      flat  flat%   sum%        cum   cum%
  237.04MB 27.25% 27.25%   237.04MB 27.25%  github.com/gaz358/myprog/workmate/repository/memory.(*InMemoryRepo).Create
  221.51MB 25.47% 52.72%   409.26MB 47.05%  github.com/gaz358/myprog/workmate/repository/memory.TestInMemoryRepo_Concurrency
  187.75MB 21.59% 74.30%   187.75MB 21.59%  github.com/gaz358/myprog/workmate/repository/memory.(*InMemoryRepo).List
  144.02MB 16.56% 90.86%   144.02MB 16.56%  github.com/gaz358/myprog/workmate/repository/memory.(*InMemoryRepo).Update
      47MB  5.40% 96.26%       47MB  5.40%  fmt.Sprintf
      11MB  1.26% 97.53%   170.02MB 19.55%  github.com/gaz358/myprog/workmate/repository/memory.TestInMemoryRepo_Concurrency.func2
    9.50MB  1.09% 98.62%   260.04MB 29.90%  github.com/gaz358/myprog/workmate/repository/memory.TestInMemoryRepo_Concurrency.func1
       8MB  0.92% 99.54%    26.50MB  3.05%  github.com/gaz358/myprog/workmate/repository/memory.TestInMemoryRepo_Concurrency.func3
         0     0% 99.54%   409.26MB 47.05%  testing.tRunner
(pprof) 




Пример секции для README.md
markdown
Копировать код
## Профилирование и оптимизация

- В проекте поддерживается профилирование с помощью [pprof](https://pkg.go.dev/net/http/pprof).  
  Для анализа производительности используются стандартные инструменты Go:  
  `go test -run TestInMemoryRepo_Concurrency -cpuprofile=cpu.out -memprofile=mem.out`
- Для просмотра профилей:
  ```sh
  go tool pprof cpu.out
  go tool pprof mem.out
Архитектурные решения
In-memory репозиторий реализован с использованием шардинга (sharded map, 16-32 shard’а), что существенно уменьшает lock contention при высокой конкурентной нагрузке.

Оптимизация позволила снизить время выполнения теста в ~5 раз по сравнению с вариантом с одним mutex.

Результаты профилирования
CPU профилирование:
После внедрения шардинга основное время выполнения теста распределяется между атомарными операциями, работой с памятью и стандартными функциями рантайма Go. Затраты на блокировки (mutex) перестали быть bottleneck:

matlab
Копировать код
flat  flat%   sum%        cum   cum%
1260ms  4.93%  ... runtime.casgstatus
1020ms  3.99%  ... runtime.scanobject
650ms   2.55%  ... sync/atomic.(*Int32).Add
Memory profile:
Основные затраты по памяти приходятся на создание и копирование задач, а также генерацию id. После удаления задач память корректно освобождается.

matlab
Копировать код
flat      flat%   sum%   cum%
237MB     27.25%  ... github.com/your-repo/memory.(*InMemoryRepo).Create
188MB     21.59%  ... github.com/your-repo/memory.(*InMemoryRepo).List
144MB     16.56%  ... github.com/your-repo/memory.(*InMemoryRepo).Update
47MB      5.40%   ... fmt.Sprintf
Профили подтверждают:

_____________________________________________________________________


go test -cover ./...

go test -coverprofile=coverage.out ./...



dockerfile

Отлично, образ собран на Ubuntu-only. Чтобы проверить, что всё работает:

Запустите контейнер:

bash
Копировать код
docker run -d --name workmate_app -p 8080:8080 workmate:latest
Убедитесь, что он поднялся:

bash
Копировать код
docker ps
— вы должны увидеть workmate_app со статусом Up и портом 0.0.0.0:8080->8080/tcp.

Посмотрите логи, чтобы убедиться, что Swagger-доки сгенерировались и приложение слушает:

bash
Копировать код
docker logs workmate_app
В браузере откройте


bash
Копировать код
http://localhost:8080/docs/index.html
— должна загрузиться Swagger UI.

Или проверка curl-ом (например, health-эндпойнт):

bash
Копировать код
curl http://localhost:8080/health
Если всё прошло без ошибок — вы успешно запустили Go-приложение со Swagger UI в одном контейнере на Ubuntu.

sudo docker exec -it workmate_app sh -c "ls -R /app/cmd/server/docs && head -n 20 /app/cmd/server/docs/index.html"


Запустите контейнер (если он ещё не запущен):

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



  gaz358@gaz358-BOD-WXX9:~/myprog/workmate$ sudo docker build -t workmate:latest .
[sudo] password for gaz358: 
DEPRECATED: The legacy builder is deprecated and will be removed in a future release.
            Install the buildx component to build images with BuildKit:
            https://docs.docker.com/go/buildx/

Sending build context to Docker daemon  10.35MB
Step 1/13 : FROM ubuntu:20.04
20.04: Pulling from library/ubuntu
13b7e930469f: Pull complete 
Digest: sha256:8feb4d8ca5354def3d8fce243717141ce31e2c428701f6682bd2fafe15388214
Status: Downloaded newer image for ubuntu:20.04
 ---> b7bab04fd9aa
Step 2/13 : ENV DEBIAN_FRONTEND=noninteractive     GOPATH=/go     PATH="/usr/local/go/bin:${GOPATH}/bin:${PATH}"     GOLANG_VERSION=1.21.7
 ---> Running in 6c05ea5f593c
 ---> Removed intermediate container 6c05ea5f593c
 ---> 2263e9c3e3a3
Step 3/13 : RUN apt-get update &&     apt-get install -y --no-install-recommends curl git make ca-certificates &&     curl -fsSL https://go.dev/dl/go${GOLANG_VERSION}.linux-amd64.tar.gz       | tar -C /usr/local -xz &&     rm -rf /var/lib/apt/lists/*
 ---> Running in 00ddfc32423b
Get:1 http://security.ubuntu.com/ubuntu focal-security InRelease [128 kB]
Get:2 http://archive.ubuntu.com/ubuntu focal InRelease [265 kB]
Get:3 http://security.ubuntu.com/ubuntu focal-security/main amd64 Packages [4431 kB]
Get:4 http://archive.ubuntu.com/ubuntu focal-updates InRelease [128 kB]
Get:5 http://security.ubuntu.com/ubuntu focal-security/restricted amd64 Packages [4801 kB]
Get:6 http://archive.ubuntu.com/ubuntu focal-backports InRelease [128 kB]
Get:7 http://archive.ubuntu.com/ubuntu focal/restricted amd64 Packages [33.4 kB]
Get:8 http://archive.ubuntu.com/ubuntu focal/main amd64 Packages [1275 kB]
Get:9 http://security.ubuntu.com/ubuntu focal-security/universe amd64 Packages [1308 kB]
Get:10 http://security.ubuntu.com/ubuntu focal-security/multiverse amd64 Packages [33.1 kB]
Get:11 http://archive.ubuntu.com/ubuntu focal/universe amd64 Packages [11.3 MB]
Get:12 http://archive.ubuntu.com/ubuntu focal/multiverse amd64 Packages [177 kB]
Get:13 http://archive.ubuntu.com/ubuntu focal-updates/universe amd64 Packages [1599 kB]
Get:14 http://archive.ubuntu.com/ubuntu focal-updates/restricted amd64 Packages [4998 kB]
Get:15 http://archive.ubuntu.com/ubuntu focal-updates/multiverse amd64 Packages [36.8 kB]
Get:16 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 Packages [4919 kB]
Get:17 http://archive.ubuntu.com/ubuntu focal-backports/universe amd64 Packages [28.6 kB]
Get:18 http://archive.ubuntu.com/ubuntu focal-backports/main amd64 Packages [55.2 kB]
Fetched 35.7 MB in 10s (3665 kB/s)
Reading package lists...
Reading package lists...
Building dependency tree...
Reading state information...
The following additional packages will be installed:
  git-man libasn1-8-heimdal libbrotli1 libcurl3-gnutls libcurl4 liberror-perl
  libexpat1 libgdbm-compat4 libgdbm6 libgssapi-krb5-2 libgssapi3-heimdal
  libhcrypto4-heimdal libheimbase1-heimdal libheimntlm0-heimdal
  libhx509-5-heimdal libk5crypto3 libkeyutils1 libkrb5-26-heimdal libkrb5-3
  libkrb5support0 libldap-2.4-2 libldap-common libnghttp2-14 libperl5.30
  libpsl5 libroken18-heimdal librtmp1 libsasl2-2 libsasl2-modules-db
  libsqlite3-0 libssh-4 libssl1.1 libwind0-heimdal openssl perl
  perl-modules-5.30
Suggested packages:
  gettext-base git-daemon-run | git-daemon-sysvinit git-doc git-el git-email
  git-gui gitk gitweb git-cvs git-mediawiki git-svn gdbm-l10n krb5-doc
  krb5-user make-doc perl-doc libterm-readline-gnu-perl
  | libterm-readline-perl-perl libb-debug-perl liblocale-codes-perl
Recommended packages:
  patch less ssh-client krb5-locales publicsuffix libsasl2-modules netbase
The following NEW packages will be installed:
  ca-certificates curl git git-man libasn1-8-heimdal libbrotli1
  libcurl3-gnutls libcurl4 liberror-perl libexpat1 libgdbm-compat4 libgdbm6
  libgssapi-krb5-2 libgssapi3-heimdal libhcrypto4-heimdal libheimbase1-heimdal
  libheimntlm0-heimdal libhx509-5-heimdal libk5crypto3 libkeyutils1
  libkrb5-26-heimdal libkrb5-3 libkrb5support0 libldap-2.4-2 libldap-common
  libnghttp2-14 libperl5.30 libpsl5 libroken18-heimdal librtmp1 libsasl2-2
  libsasl2-modules-db libsqlite3-0 libssh-4 libssl1.1 libwind0-heimdal make
  openssl perl perl-modules-5.30
0 upgraded, 40 newly installed, 0 to remove and 4 not upgraded.
Need to get 18.2 MB of archives.
After this operation, 103 MB of additional disk space will be used.
Get:1 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 perl-modules-5.30 all 5.30.0-9ubuntu0.5 [2739 kB]
Get:2 http://archive.ubuntu.com/ubuntu focal/main amd64 libgdbm6 amd64 1.18.1-5 [27.4 kB]
Get:3 http://archive.ubuntu.com/ubuntu focal/main amd64 libgdbm-compat4 amd64 1.18.1-5 [6244 B]
Get:4 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libperl5.30 amd64 5.30.0-9ubuntu0.5 [3941 kB]
Get:5 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 perl amd64 5.30.0-9ubuntu0.5 [224 kB]
Get:6 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libssl1.1 amd64 1.1.1f-1ubuntu2.24 [1323 kB]
Get:7 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 openssl amd64 1.1.1f-1ubuntu2.24 [621 kB]
Get:8 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 ca-certificates all 20240203~20.04.1 [159 kB]
Get:9 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libexpat1 amd64 2.2.9-1ubuntu0.8 [75.6 kB]
Get:10 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libsqlite3-0 amd64 3.31.1-4ubuntu0.7 [549 kB]
Get:11 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libkrb5support0 amd64 1.17-6ubuntu4.11 [31.5 kB]
Get:12 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libk5crypto3 amd64 1.17-6ubuntu4.11 [80.3 kB]
Get:13 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libkeyutils1 amd64 1.6-6ubuntu1.1 [10.3 kB]
Get:14 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libkrb5-3 amd64 1.17-6ubuntu4.11 [330 kB]
Get:15 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libgssapi-krb5-2 amd64 1.17-6ubuntu4.11 [121 kB]
Get:16 http://archive.ubuntu.com/ubuntu focal/main amd64 libpsl5 amd64 0.21.0-1ubuntu1 [51.5 kB]
Get:17 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libbrotli1 amd64 1.0.7-6ubuntu0.1 [267 kB]
Get:18 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libroken18-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [42.5 kB]
Get:19 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libasn1-8-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [181 kB]
Get:20 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libheimbase1-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [30.4 kB]
Get:21 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libhcrypto4-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [88.1 kB]
Get:22 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libwind0-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [47.7 kB]
Get:23 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libhx509-5-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [107 kB]
Get:24 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libkrb5-26-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [207 kB]
Get:25 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libheimntlm0-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [15.1 kB]
Get:26 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libgssapi3-heimdal amd64 7.7.0+dfsg-1ubuntu1.4 [96.5 kB]
Get:27 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libsasl2-modules-db amd64 2.1.27+dfsg-2ubuntu0.1 [14.7 kB]
Get:28 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libsasl2-2 amd64 2.1.27+dfsg-2ubuntu0.1 [49.3 kB]
Get:29 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libldap-common all 2.4.49+dfsg-2ubuntu1.10 [16.5 kB]
Get:30 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libldap-2.4-2 amd64 2.4.49+dfsg-2ubuntu1.10 [155 kB]
Get:31 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libnghttp2-14 amd64 1.40.0-1ubuntu0.3 [79.9 kB]
Get:32 http://archive.ubuntu.com/ubuntu focal/main amd64 librtmp1 amd64 2.4+20151223.gitfa8646d.1-2build1 [54.9 kB]
Get:33 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libssh-4 amd64 0.9.3-2ubuntu2.5 [171 kB]
Get:34 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libcurl4 amd64 7.68.0-1ubuntu2.25 [235 kB]
Get:35 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 curl amd64 7.68.0-1ubuntu2.25 [162 kB]
Get:36 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 libcurl3-gnutls amd64 7.68.0-1ubuntu2.25 [233 kB]
Get:37 http://archive.ubuntu.com/ubuntu focal/main amd64 liberror-perl all 0.17029-1 [26.5 kB]
Get:38 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 git-man all 1:2.25.1-1ubuntu3.14 [887 kB]
Get:39 http://archive.ubuntu.com/ubuntu focal-updates/main amd64 git amd64 1:2.25.1-1ubuntu3.14 [4613 kB]
Get:40 http://archive.ubuntu.com/ubuntu focal/main amd64 make amd64 4.2.1-1.2 [162 kB]
debconf: delaying package configuration, since apt-utils is not installed
Fetched 18.2 MB in 10s (1915 kB/s)
Selecting previously unselected package perl-modules-5.30.
(Reading database ... 4124 files and directories currently installed.)
Preparing to unpack .../00-perl-modules-5.30_5.30.0-9ubuntu0.5_all.deb ...
Unpacking perl-modules-5.30 (5.30.0-9ubuntu0.5) ...
Selecting previously unselected package libgdbm6:amd64.
Preparing to unpack .../01-libgdbm6_1.18.1-5_amd64.deb ...
Unpacking libgdbm6:amd64 (1.18.1-5) ...
Selecting previously unselected package libgdbm-compat4:amd64.
Preparing to unpack .../02-libgdbm-compat4_1.18.1-5_amd64.deb ...
Unpacking libgdbm-compat4:amd64 (1.18.1-5) ...
Selecting previously unselected package libperl5.30:amd64.
Preparing to unpack .../03-libperl5.30_5.30.0-9ubuntu0.5_amd64.deb ...
Unpacking libperl5.30:amd64 (5.30.0-9ubuntu0.5) ...
Selecting previously unselected package perl.
Preparing to unpack .../04-perl_5.30.0-9ubuntu0.5_amd64.deb ...
Unpacking perl (5.30.0-9ubuntu0.5) ...
Selecting previously unselected package libssl1.1:amd64.
Preparing to unpack .../05-libssl1.1_1.1.1f-1ubuntu2.24_amd64.deb ...
Unpacking libssl1.1:amd64 (1.1.1f-1ubuntu2.24) ...
Selecting previously unselected package openssl.
Preparing to unpack .../06-openssl_1.1.1f-1ubuntu2.24_amd64.deb ...
Unpacking openssl (1.1.1f-1ubuntu2.24) ...
Selecting previously unselected package ca-certificates.
Preparing to unpack .../07-ca-certificates_20240203~20.04.1_all.deb ...
Unpacking ca-certificates (20240203~20.04.1) ...
Selecting previously unselected package libexpat1:amd64.
Preparing to unpack .../08-libexpat1_2.2.9-1ubuntu0.8_amd64.deb ...
Unpacking libexpat1:amd64 (2.2.9-1ubuntu0.8) ...
Selecting previously unselected package libsqlite3-0:amd64.
Preparing to unpack .../09-libsqlite3-0_3.31.1-4ubuntu0.7_amd64.deb ...
Unpacking libsqlite3-0:amd64 (3.31.1-4ubuntu0.7) ...
Selecting previously unselected package libkrb5support0:amd64.
Preparing to unpack .../10-libkrb5support0_1.17-6ubuntu4.11_amd64.deb ...
Unpacking libkrb5support0:amd64 (1.17-6ubuntu4.11) ...
Selecting previously unselected package libk5crypto3:amd64.
Preparing to unpack .../11-libk5crypto3_1.17-6ubuntu4.11_amd64.deb ...
Unpacking libk5crypto3:amd64 (1.17-6ubuntu4.11) ...
Selecting previously unselected package libkeyutils1:amd64.
Preparing to unpack .../12-libkeyutils1_1.6-6ubuntu1.1_amd64.deb ...
Unpacking libkeyutils1:amd64 (1.6-6ubuntu1.1) ...
Selecting previously unselected package libkrb5-3:amd64.
Preparing to unpack .../13-libkrb5-3_1.17-6ubuntu4.11_amd64.deb ...
Unpacking libkrb5-3:amd64 (1.17-6ubuntu4.11) ...
Selecting previously unselected package libgssapi-krb5-2:amd64.
Preparing to unpack .../14-libgssapi-krb5-2_1.17-6ubuntu4.11_amd64.deb ...
Unpacking libgssapi-krb5-2:amd64 (1.17-6ubuntu4.11) ...
Selecting previously unselected package libpsl5:amd64.
Preparing to unpack .../15-libpsl5_0.21.0-1ubuntu1_amd64.deb ...
Unpacking libpsl5:amd64 (0.21.0-1ubuntu1) ...
Selecting previously unselected package libbrotli1:amd64.
Preparing to unpack .../16-libbrotli1_1.0.7-6ubuntu0.1_amd64.deb ...
Unpacking libbrotli1:amd64 (1.0.7-6ubuntu0.1) ...
Selecting previously unselected package libroken18-heimdal:amd64.
Preparing to unpack .../17-libroken18-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libroken18-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libasn1-8-heimdal:amd64.
Preparing to unpack .../18-libasn1-8-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libasn1-8-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libheimbase1-heimdal:amd64.
Preparing to unpack .../19-libheimbase1-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libheimbase1-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libhcrypto4-heimdal:amd64.
Preparing to unpack .../20-libhcrypto4-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libhcrypto4-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libwind0-heimdal:amd64.
Preparing to unpack .../21-libwind0-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libwind0-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libhx509-5-heimdal:amd64.
Preparing to unpack .../22-libhx509-5-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libhx509-5-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libkrb5-26-heimdal:amd64.
Preparing to unpack .../23-libkrb5-26-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libkrb5-26-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libheimntlm0-heimdal:amd64.
Preparing to unpack .../24-libheimntlm0-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libheimntlm0-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libgssapi3-heimdal:amd64.
Preparing to unpack .../25-libgssapi3-heimdal_7.7.0+dfsg-1ubuntu1.4_amd64.deb ...
Unpacking libgssapi3-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Selecting previously unselected package libsasl2-modules-db:amd64.
Preparing to unpack .../26-libsasl2-modules-db_2.1.27+dfsg-2ubuntu0.1_amd64.deb ...
Unpacking libsasl2-modules-db:amd64 (2.1.27+dfsg-2ubuntu0.1) ...
Selecting previously unselected package libsasl2-2:amd64.
Preparing to unpack .../27-libsasl2-2_2.1.27+dfsg-2ubuntu0.1_amd64.deb ...
Unpacking libsasl2-2:amd64 (2.1.27+dfsg-2ubuntu0.1) ...
Selecting previously unselected package libldap-common.
Preparing to unpack .../28-libldap-common_2.4.49+dfsg-2ubuntu1.10_all.deb ...
Unpacking libldap-common (2.4.49+dfsg-2ubuntu1.10) ...
Selecting previously unselected package libldap-2.4-2:amd64.
Preparing to unpack .../29-libldap-2.4-2_2.4.49+dfsg-2ubuntu1.10_amd64.deb ...
Unpacking libldap-2.4-2:amd64 (2.4.49+dfsg-2ubuntu1.10) ...
Selecting previously unselected package libnghttp2-14:amd64.
Preparing to unpack .../30-libnghttp2-14_1.40.0-1ubuntu0.3_amd64.deb ...
Unpacking libnghttp2-14:amd64 (1.40.0-1ubuntu0.3) ...
Selecting previously unselected package librtmp1:amd64.
Preparing to unpack .../31-librtmp1_2.4+20151223.gitfa8646d.1-2build1_amd64.deb ...
Unpacking librtmp1:amd64 (2.4+20151223.gitfa8646d.1-2build1) ...
Selecting previously unselected package libssh-4:amd64.
Preparing to unpack .../32-libssh-4_0.9.3-2ubuntu2.5_amd64.deb ...
Unpacking libssh-4:amd64 (0.9.3-2ubuntu2.5) ...
Selecting previously unselected package libcurl4:amd64.
Preparing to unpack .../33-libcurl4_7.68.0-1ubuntu2.25_amd64.deb ...
Unpacking libcurl4:amd64 (7.68.0-1ubuntu2.25) ...
Selecting previously unselected package curl.
Preparing to unpack .../34-curl_7.68.0-1ubuntu2.25_amd64.deb ...
Unpacking curl (7.68.0-1ubuntu2.25) ...
Selecting previously unselected package libcurl3-gnutls:amd64.
Preparing to unpack .../35-libcurl3-gnutls_7.68.0-1ubuntu2.25_amd64.deb ...
Unpacking libcurl3-gnutls:amd64 (7.68.0-1ubuntu2.25) ...
Selecting previously unselected package liberror-perl.
Preparing to unpack .../36-liberror-perl_0.17029-1_all.deb ...
Unpacking liberror-perl (0.17029-1) ...
Selecting previously unselected package git-man.
Preparing to unpack .../37-git-man_1%3a2.25.1-1ubuntu3.14_all.deb ...
Unpacking git-man (1:2.25.1-1ubuntu3.14) ...
Selecting previously unselected package git.
Preparing to unpack .../38-git_1%3a2.25.1-1ubuntu3.14_amd64.deb ...
Unpacking git (1:2.25.1-1ubuntu3.14) ...
Selecting previously unselected package make.
Preparing to unpack .../39-make_4.2.1-1.2_amd64.deb ...
Unpacking make (4.2.1-1.2) ...
Setting up libexpat1:amd64 (2.2.9-1ubuntu0.8) ...
Setting up libkeyutils1:amd64 (1.6-6ubuntu1.1) ...
Setting up libpsl5:amd64 (0.21.0-1ubuntu1) ...
Setting up perl-modules-5.30 (5.30.0-9ubuntu0.5) ...
Setting up libssl1.1:amd64 (1.1.1f-1ubuntu2.24) ...
Setting up libbrotli1:amd64 (1.0.7-6ubuntu0.1) ...
Setting up libsqlite3-0:amd64 (3.31.1-4ubuntu0.7) ...
Setting up libnghttp2-14:amd64 (1.40.0-1ubuntu0.3) ...
Setting up libldap-common (2.4.49+dfsg-2ubuntu1.10) ...
Setting up libkrb5support0:amd64 (1.17-6ubuntu4.11) ...
Setting up libsasl2-modules-db:amd64 (2.1.27+dfsg-2ubuntu0.1) ...
Setting up make (4.2.1-1.2) ...
Setting up librtmp1:amd64 (2.4+20151223.gitfa8646d.1-2build1) ...
Setting up libk5crypto3:amd64 (1.17-6ubuntu4.11) ...
Setting up libsasl2-2:amd64 (2.1.27+dfsg-2ubuntu0.1) ...
Setting up libroken18-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up git-man (1:2.25.1-1ubuntu3.14) ...
Setting up libkrb5-3:amd64 (1.17-6ubuntu4.11) ...
Setting up openssl (1.1.1f-1ubuntu2.24) ...
Setting up libheimbase1-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up libgdbm6:amd64 (1.18.1-5) ...
Setting up libasn1-8-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up libhcrypto4-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up ca-certificates (20240203~20.04.1) ...
Updating certificates in /etc/ssl/certs...
146 added, 0 removed; done.
Setting up libwind0-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up libgssapi-krb5-2:amd64 (1.17-6ubuntu4.11) ...
Setting up libgdbm-compat4:amd64 (1.18.1-5) ...
Setting up libssh-4:amd64 (0.9.3-2ubuntu2.5) ...
Setting up libperl5.30:amd64 (5.30.0-9ubuntu0.5) ...
Setting up libhx509-5-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up perl (5.30.0-9ubuntu0.5) ...
Setting up libkrb5-26-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up libheimntlm0-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up liberror-perl (0.17029-1) ...
Setting up libgssapi3-heimdal:amd64 (7.7.0+dfsg-1ubuntu1.4) ...
Setting up libldap-2.4-2:amd64 (2.4.49+dfsg-2ubuntu1.10) ...
Setting up libcurl3-gnutls:amd64 (7.68.0-1ubuntu2.25) ...
Setting up git (1:2.25.1-1ubuntu3.14) ...
Setting up libcurl4:amd64 (7.68.0-1ubuntu2.25) ...
Setting up curl (7.68.0-1ubuntu2.25) ...
Processing triggers for libc-bin (2.31-0ubuntu9.17) ...
Processing triggers for ca-certificates (20240203~20.04.1) ...
Updating certificates in /etc/ssl/certs...
0 added, 0 removed; done.
Running hooks in /etc/ca-certificates/update.d...
done.
 ---> Removed intermediate container 00ddfc32423b
 ---> 206f1c261ca9
Step 4/13 : WORKDIR /app
 ---> Running in 0ff7d33b7273
 ---> Removed intermediate container 0ff7d33b7273
 ---> ffce60d230bb
Step 5/13 : COPY go.mod go.sum ./
 ---> cda97d40b82e
Step 6/13 : RUN go mod download
 ---> Running in df848d7e3047
go: downloading go1.24.0 (linux/amd64)
 ---> Removed intermediate container df848d7e3047
 ---> 701ff58f4792
Step 7/13 : COPY . .
 ---> 64f0e3adebfd
Step 8/13 : WORKDIR /app
 ---> Running in 9aef21a5ce19
 ---> Removed intermediate container 9aef21a5ce19
 ---> e4d119d8b904
Step 9/13 : RUN go install github.com/swaggo/swag/cmd/swag@latest &&     swag init -g cmd/server/main.go -o cmd/server/docs &&     sed -i '/LeftDelim:/d; /RightDelim:/d' cmd/server/docs/docs.go &&     mkdir -p docs/swagger-ui &&     curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui.css   -o docs/swagger-ui/swagger-ui.css &&     curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui-bundle.js -o docs/swagger-ui/swagger-ui-bundle.js &&     curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui-standalone-preset.js -o docs/swagger-ui/swagger-ui-standalone-preset.js &&     printf '<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Swagger UI</title><link rel="stylesheet" href="swagger-ui/swagger-ui.css"></head><body><div id="swagger-ui"></div><script src="swagger-ui/swagger-ui-bundle.js"></script><script src="swagger-ui/swagger-ui-standalone-preset.js"></script><script>window.onload=function(){SwaggerUIBundle({url:"swagger.json",dom_id:"#swagger-ui",presets:[SwaggerUIBundle.presets.apis,SwaggerUIStandalonePreset]});};</script></body></html>' > docs/index.html
 ---> Running in 9c9b1395536f
go: downloading github.com/swaggo/swag v1.16.4
go: downloading github.com/urfave/cli/v2 v2.3.0
go: downloading github.com/go-openapi/spec v0.20.4
go: downloading golang.org/x/text v0.14.0
go: downloading sigs.k8s.io/yaml v1.3.0
go: downloading golang.org/x/tools v0.7.0
go: downloading github.com/go-openapi/jsonreference v0.19.6
go: downloading github.com/cpuguy83/go-md2man/v2 v2.0.0-20190314233015-f79a8a8ca69d
go: downloading golang.org/x/sys v0.18.0
go: downloading github.com/PuerkitoBio/purell v1.1.1
go: downloading github.com/russross/blackfriday/v2 v2.0.1
go: downloading github.com/PuerkitoBio/urlesc v0.0.0-20170810143723-de5bf2ad4578
go: downloading golang.org/x/net v0.23.0
go: downloading github.com/shurcooL/sanitized_anchor_name v1.0.0
/bin/sh: 1: swag: not found
The command '/bin/sh -c go install github.com/swaggo/swag/cmd/swag@latest &&     swag init -g cmd/server/main.go -o cmd/server/docs &&     sed -i '/LeftDelim:/d; /RightDelim:/d' cmd/server/docs/docs.go &&     mkdir -p docs/swagger-ui &&     curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui.css   -o docs/swagger-ui/swagger-ui.css &&     curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui-bundle.js -o docs/swagger-ui/swagger-ui-bundle.js &&     curl -fsSL https://unpkg.com/swagger-ui-dist@4/swagger-ui-standalone-preset.js -o docs/swagger-ui/swagger-ui-standalone-preset.js &&     printf '<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Swagger UI</title><link rel="stylesheet" href="swagger-ui/swagger-ui.css"></head><body><div id="swagger-ui"></div><script src="swagger-ui/swagger-ui-bundle.js"></script><script src="swagger-ui/swagger-ui-standalone-preset.js"></script><script>window.onload=function(){SwaggerUIBundle({url:"swagger.json",dom_id:"#swagger-ui",presets:[SwaggerUIBundle.presets.apis,SwaggerUIStandalonePreset]});};</script></body></html>' > docs/index.html' returned a non-zero code: 127
gaz358@gaz358-BOD-WXX9:~/myprog/workmate$ 













