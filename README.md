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


Сборка образа
В корне проекта (там, где лежит ваш Dockerfile и go.mod) выполните:


docker build -t workmate:latest .
Здесь:

-t workmate:latest — задаёт тег образа (workmate:latest),

. — указывает, что контекст сборки — текущая папка.

Запуск контейнера
После успешной сборки запустите контейнер, пробросив порт 8080:

bash
Копировать код
docker run -d \
  --name workmate_app \
  -p 8080:8080 \
  workmate:latest
Где:

-d — запускает контейнер в фоне (detached),

--name workmate_app — даёт контейнеру понятное имя,

-p 8080:8080 — мапит порт 8080 хоста на 8080 внутри контейнера,

workmate:latest — имя и тег образа, который мы только что собрали.

Проверка, что контейнер запущен


docker ps
Вы должны увидеть строку с workmate_app, его статус Up, и колонку PORTS вида 0.0.0.0:8080->8080/tcp.

Логи приложения
Чтобы убедиться, что приложение стартовало без ошибок:


docker logs workmate_app
Вы увидите вывод вашего сервера, например:


2025/07/07 12:34:56 Сервер запущен на :8080, Swagger UI доступен по http://localhost:8080/docs/index.html
Проверка работоспособности

Через браузер
Откройте в нём


http://localhost:8080/docs/index.html
— должна отобразиться Swagger UI с документацией вашего API.

Через curl
Например, если у вас есть health-endpoint /health:


curl http://localhost:8080/health
Ожидаемый ответ — что-то вроде:

json
Копировать код
{"status":"ok"}
Остановка и удаление контейнера
Когда закончите проверку, контейнер можно остановить и удалить:


docker stop workmate_app
docker rm workmate_app


gaz358@gaz358-BOD-WXX9:~/myprog/workmate$ sudo docker build -t workmate:latest .
DEPRECATED: The legacy builder is deprecated and will be removed in a future release.
            Install the buildx component to build images with BuildKit:
            https://docs.docker.com/go/buildx/

Sending build context to Docker daemon  10.34MB
Step 1/16 : FROM golang:1.21.7-slim AS builder
manifest for golang:1.21.7-slim not found: manifest unknown: manifest unknown
gaz358@gaz358-BOD-WXX9:~/myprog/workmate$ 

_____________________________________________________________
dockerfile



Вот единый многоэтапный Dockerfile, который и собирает ваш Go-проект со Swagger-доками, и разворачивает финальный минимальный образ, при этом финальный образ остаётся на scratch без уязвимых пакетов:

dockerfile
Копировать код

Как пользоваться
Сборка образа

bash
Копировать код
docker build -t workmate:latest .
Запуск контейнера

bash
Копировать код
docker run -d --name workmate_app -p 8080:8080 workmate:latest
Проверка

docker ps — контейнер должен быть в статусе Up с портом 0.0.0.0:8080->8080/tcp.

docker logs workmate_app — смотрим логи запуска.

В браузере открываем http://localhost:8080/docs/index.html (или curl http://localhost:8080/health).


__________________________________________________________

# syntax=docker/dockerfile:1.4

################################################################################
# === СТАДИЯ 1: Сборка на Ubuntu 20.04 с ручной установкой Go и Swagger CLI   ===
################################################################################
FROM ubuntu:20.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV GOLANG_VERSION=1.21.7
ENV GOPATH=/go
# Добавляем в PATH: официальный Go и GOPATH/bin
ENV PATH="/usr/local/go/bin:${GOPATH}/bin:${PATH}"

# 1) Устанавливаем утилиты, скачиваем и распаковываем Go
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      curl \
      git \
      make \
      ca-certificates && \
    curl -fsSL https://go.dev/dl/go${GOLANG_VERSION}.linux-amd64.tar.gz \
       | tar -C /usr/local -xz && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# 2) Kопируем go.mod/go.sum и подтягиваем зависимости
COPY go.mod go.sum ./
RUN go mod download

# 3) Копируем весь исходный код
COPY . .

# 4) Устанавливаем Swagger CLI
RUN go install github.com/swaggo/swag/cmd/swag@latest

# 5) Генерируем Swagger-доки
RUN swag init -g cmd/server/main.go -o cmd/server/docs

# 6) Удаляем из docs.go строки LeftDelim и RightDelim
RUN sed -i '/LeftDelim:/d; /RightDelim:/d' cmd/server/docs/docs.go

# 7) Сборка статического бинарника
RUN CGO_ENABLED=0 GOOS=linux go build -ldflags="-s -w" -o workmate cmd/server/main.go


################################################################################
# === СТАДИЯ 2: Рантайм на Ubuntu 20.04                                      ===
################################################################################
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# 1) Устанавливаем только сертификаты для HTTPS
RUN apt-get update && \
    apt-get install -y --no-install-recommends ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /

# 2) Копируем бинарник и Swagger-доки из билд-стадии
COPY --from=builder /app/workmate /usr/local/bin/workmate
COPY --from=builder /app/cmd/server/docs /docs

# 3) Пробрасываем порт приложения
EXPOSE 8080

# 4) Точка входа
ENTRYPOINT ["/usr/local/bin/workmate", "--swagger-dir", "/docs"]


