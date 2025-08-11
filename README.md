
mx0vglWtzbBOGF34or  апи кей
77658a3144bd469fa8050b9c91b9cd4e секр кей

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


https://github.com/mexcdevelop/websocket-proto



MAP=$(for f in *.proto; do printf -- "--go_opt=M%s=%s " "$f" "crypt_proto/pb"; done)

protoc -I=. \
  --go_out=. --go_opt=paths=source_relative \
  $MAP \
  *.proto


package main

import (
	"encoding/json"
	"fmt"
	"log"
	"strconv"
	"strings"
	"time"

	"github.com/gorilla/websocket"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/reflect/protoreflect"

	pb "crypt_proto/pb"
)

func main() {
	const wsURL = "wss://wbs-api.mexc.com/ws"

	conn, _, err := websocket.DefaultDialer.Dial(wsURL, nil)
	if err != nil {
		log.Fatal("dial:", err)
	}
	defer conn.Close()

	// подписка на 3 .pb-топика
	sub := map[string]any{
		"method": "SUBSCRIPTION",
		"params": []string{
			"spot@public.aggre.bookTicker.v3.api.pb@100ms@BTCUSDT",
			"spot@public.aggre.bookTicker.v3.api.pb@100ms@ETHUSDT",
			"spot@public.aggre.bookTicker.v3.api.pb@100ms@ETHBTC",
		},
	}
	if err := conn.WriteJSON(sub); err != nil {
		log.Fatal("send sub:", err)
	}

	// поддержка линка
	go func() {
		t := time.NewTicker(45 * time.Second)
		defer t.Stop()
		for range t.C {
			_ = conn.WriteMessage(websocket.PingMessage, []byte("hb"))
		}
	}()

	for {
		mt, raw, err := conn.ReadMessage()
		if err != nil {
			log.Fatal("read:", err)
		}

		// ACK / ошибки — TEXT/JSON
		if mt == websocket.TextMessage {
			var v any
			if json.Unmarshal(raw, &v) == nil {
				b, _ := json.MarshalIndent(v, "", "  ")
				fmt.Printf("ACK:\n%s\n", b)
			} else {
				fmt.Printf("TEXT:\n%s\n", string(raw))
			}
			continue
		}
		if mt != websocket.BinaryMessage {
			continue
		}

		// ====== 1) попытка: фрейм уже является PublicAggreBookTickerV3Api ======
		if bt, ok := tryUnmarshalBookTicker(raw); ok {
			printBookTicker("RAW", "", bt)
			continue
		}

		// ====== 2) общий случай: сначала обёртка, из неё вынимаем payload ======
		var w pb.PushDataV3ApiWrapper
		if err := proto.Unmarshal(raw, &w); err != nil {
			// если у тебя тип называется иначе (например PushDataV3Api), сюда можно добавить альтернативу
			log.Printf("wrapper unmarshal failed: %v", err)
			continue
		}

		channel := getWrapperChannel(&w)
		// соберём ВСЕ возможные payload (bytes/messages) рекурсивно
		payloads := collectPayloads(&w)

		if len(payloads) == 0 {
			log.Printf("no payloads found in wrapper (channel=%s)", channel)
			continue
		}

		decoded := false
		for _, p := range payloads {
			if bt, ok := tryUnmarshalBookTicker(p); ok {
				printBookTicker("WRAP", channel, bt)
				decoded = true
			}
		}
		if !decoded {
			log.Printf("no bookTicker decoded (channel=%s, candidates=%d)", channel, len(payloads))
		}
	}
}

// tryUnmarshalBookTicker пытается распарсить buffer как PublicAggreBookTickerV3Api
func tryUnmarshalBookTicker(buf []byte) (*pb.PublicAggreBookTickerV3Api, bool) {
	var bt pb.PublicAggreBookTickerV3Api
	if err := proto.Unmarshal(buf, &bt); err != nil {
		return nil, false
	}
	// быстрая sanity-проверка: есть ли в нём цены
	if bt.GetBidPrice() == "" && bt.GetAskPrice() == "" && bt.GetBidQuantity() == "" && bt.GetAskQuantity() == "" {
		return nil, false
	}
	return &bt, true
}

// collectPayloads рекурсивно собирает кандидаты на payload из message:
// bytes, repeated bytes, message (маршалим обратно), repeated message, активные oneof
func collectPayloads(m proto.Message) [][]byte {
	var out [][]byte
	rm := m.ProtoReflect()

	// поля
	fds := rm.Descriptor().Fields()
	for i := 0; i < fds.Len(); i++ {
		fd := fds.Get(i)
		if !rm.Has(fd) {
			continue
		}
		val := rm.Get(fd)

		switch {
		case fd.IsList():
			list := val.List()
			for j := 0; j < list.Len(); j++ {
				iv := list.Get(j)
				switch fd.Kind() {
				case protoreflect.BytesKind:
					out = append(out, append([]byte(nil), iv.Bytes()...))
				case protoreflect.MessageKind:
					if b, err := proto.Marshal(iv.Message().Interface()); err == nil {
						out = append(out, b)
						// рекурсивно доберёмся до вложенных байт/сообщений
						out = append(out, collectPayloads(iv.Message().Interface())...)
					}
				}
			}

		case fd.Kind() == protoreflect.BytesKind:
			out = append(out, append([]byte(nil), val.Bytes()...))

		case fd.Kind() == protoreflect.MessageKind:
			if b, err := proto.Marshal(val.Message().Interface()); err == nil {
				out = append(out, b)
				out = append(out, collectPayloads(val.Message().Interface())...)
			}
		}
	}

	// oneof
	ods := rm.Descriptor().Oneofs()
	for i := 0; i < ods.Len(); i++ {
		od := ods.Get(i)
		fv := rm.WhichOneof(od)
		if fv == nil {
			continue
		}
		val := rm.Get(fv)
		switch fv.Kind() {
		case protoreflect.BytesKind:
			out = append(out, append([]byte(nil), val.Bytes()...))
		case protoreflect.MessageKind:
			if b, err := proto.Marshal(val.Message().Interface()); err == nil {
				out = append(out, b)
				out = append(out, collectPayloads(val.Message().Interface())...)
			}
		}
	}

	return out
}

// getWrapperChannel пытается вытащить строковое поле канала (обычно "c"), иначе ""
func getWrapperChannel(m proto.Message) string {
	rm := m.ProtoReflect()
	// часто поле называется "c" или "channel"
	for _, name := range []protoreflect.Name{"c", "channel", "topic"} {
		if fd := rm.Descriptor().Fields().ByName(name); fd != nil && fd.Kind() == protoreflect.StringKind && rm.Has(fd) {
			return rm.Get(fd).String()
		}
	}
	// если в обёртке нет строки канала — вернём пусто
	return ""
}

func printBookTicker(src, channel string, bt *pb.PublicAggreBookTickerV3Api) {
	// символ: из канала (последний сегмент после '@'), иначе пытаемся взять из payload если есть (часто его нет)
	sym := ""
	if channel != "" {
		parts := strings.Split(channel, "@")
		if len(parts) > 0 {
			sym = parts[len(parts)-1]
		}
	}
	bps := bt.GetBidPrice()
	aps := bt.GetAskPrice()
	bqs := bt.GetBidQuantity()
	aqs := bt.GetAskQuantity()

	bid, _ := strconv.ParseFloat(bps, 64)
	ask, _ := strconv.ParseFloat(aps, 64)
	bq, _ := strconv.ParseFloat(bqs, 64)
	aq, _ := strconv.ParseFloat(aqs, 64)

	fmt.Printf("[%s] %s  bid=%.8f (%.6f)  ask=%.8f (%.6f)\n", src, sym, bid, bq, ask, aq)
}


Если всё ещё будет «пусто», пришли сюда первые ~40 строк pb/PushDataV3ApiWrapper.pb.go (или выведем wrapper в JSON через protojson и посмотрим реальные поля) — подстрою под твою конкретную схему.


gaz358@gaz358-BOD-WXX9:~/myprog/crypt_proto$ go run .
ACK:
{
  "code": 0,
  "id": 0,
  "msg": "spot@public.aggre.bookTicker.v3.api.pb@100ms@ETHUSDT,spot@public.aggre.bookTicker.v3.api.pb@100ms@ETHBTC,spot@public.aggre.bookTicker.v3.api.pb@100ms@BTCUSDT"
}
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)
[RAW]   bid=0.00000000 (0.000000)  ask=0.00000000 (0.000000)



// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.7
// 	protoc        v3.21.12
// source: PushDataV3ApiWrapper.proto

package pb

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
	unsafe "unsafe"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type PushDataV3ApiWrapper struct {
	state protoimpl.MessageState `protogen:"open.v1"`
	// *
	// 频道
	Channel string `protobuf:"bytes,1,opt,name=channel,proto3" json:"channel,omitempty"`
	// *
	// 数据，NOTE：因为不能重复，所以类型和变量名尽量使用全名
	//
	// Types that are valid to be assigned to Body:
	//
	//	*PushDataV3ApiWrapper_PublicDeals
	//	*PushDataV3ApiWrapper_PublicIncreaseDepths
	//	*PushDataV3ApiWrapper_PublicLimitDepths
	//	*PushDataV3ApiWrapper_PrivateOrders
	//	*PushDataV3ApiWrapper_PublicBookTicker
	//	*PushDataV3ApiWrapper_PrivateDeals
	//	*PushDataV3ApiWrapper_PrivateAccount
	//	*PushDataV3ApiWrapper_PublicSpotKline
	//	*PushDataV3ApiWrapper_PublicMiniTicker
	//	*PushDataV3ApiWrapper_PublicMiniTickers
	//	*PushDataV3ApiWrapper_PublicBookTickerBatch
	//	*PushDataV3ApiWrapper_PublicIncreaseDepthsBatch
	//	*PushDataV3ApiWrapper_PublicAggreDepths
	//	*PushDataV3ApiWrapper_PublicAggreDeals
	//	*PushDataV3ApiWrapper_PublicAggreBookTicker
	Body isPushDataV3ApiWrapper_Body `protobuf_oneof:"body"`
	// *
	// 交易对
	Symbol *string `protobuf:"bytes,3,opt,name=symbol,proto3,oneof" json:"symbol,omitempty"`
	// *
	// 交易对ID
	SymbolId *string `protobuf:"bytes,4,opt,name=symbolId,proto3,oneof" json:"symbolId,omitempty"`
	// *
	// 消息生成时间
	CreateTime *int64 `protobuf:"varint,5,opt,name=createTime,proto3,oneof" json:"createTime,omitempty"`
	// *
	// 消息推送时间
	SendTime      *int64 `protobuf:"varint,6,opt,name=sendTime,proto3,oneof" json:"sendTime,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *PushDataV3ApiWrapper) Reset() {
	*x = PushDataV3ApiWrapper{}
	mi := &file_PushDataV3ApiWrapper_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *PushDataV3ApiWrapper) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*PushDataV3ApiWrapper) ProtoMessage() {}

func (x *PushDataV3ApiWrapper) ProtoReflect() protoreflect.Message {
	mi := &file_PushDataV3ApiWrapper_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use PushDataV3ApiWrapper.ProtoReflect.Descriptor instead.
func (*PushDataV3ApiWrapper) Descriptor() ([]byte, []int) {
	return file_PushDataV3ApiWrapper_proto_rawDescGZIP(), []int{0}
}

func (x *PushDataV3ApiWrapper) GetChannel() string {
	if x != nil {
		return x.Channel
	}
	return ""
}

func (x *PushDataV3ApiWrapper) GetBody() isPushDataV3ApiWrapper_Body {
	if x != nil {
		return x.Body
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicDeals() *PublicDealsV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicDeals); ok {
			return x.PublicDeals
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicIncreaseDepths() *PublicIncreaseDepthsV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicIncreaseDepths); ok {
			return x.PublicIncreaseDepths
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicLimitDepths() *PublicLimitDepthsV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicLimitDepths); ok {
			return x.PublicLimitDepths
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPrivateOrders() *PrivateOrdersV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PrivateOrders); ok {
			return x.PrivateOrders
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicBookTicker() *PublicBookTickerV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicBookTicker); ok {
			return x.PublicBookTicker
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPrivateDeals() *PrivateDealsV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PrivateDeals); ok {
			return x.PrivateDeals
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPrivateAccount() *PrivateAccountV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PrivateAccount); ok {
			return x.PrivateAccount
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicSpotKline() *PublicSpotKlineV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicSpotKline); ok {
			return x.PublicSpotKline
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicMiniTicker() *PublicMiniTickerV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicMiniTicker); ok {
			return x.PublicMiniTicker
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicMiniTickers() *PublicMiniTickersV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicMiniTickers); ok {
			return x.PublicMiniTickers
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicBookTickerBatch() *PublicBookTickerBatchV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicBookTickerBatch); ok {
			return x.PublicBookTickerBatch
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicIncreaseDepthsBatch() *PublicIncreaseDepthsBatchV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicIncreaseDepthsBatch); ok {
			return x.PublicIncreaseDepthsBatch
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicAggreDepths() *PublicAggreDepthsV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicAggreDepths); ok {
			return x.PublicAggreDepths
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicAggreDeals() *PublicAggreDealsV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicAggreDeals); ok {
			return x.PublicAggreDeals
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetPublicAggreBookTicker() *PublicAggreBookTickerV3Api {
	if x != nil {
		if x, ok := x.Body.(*PushDataV3ApiWrapper_PublicAggreBookTicker); ok {
			return x.PublicAggreBookTicker
		}
	}
	return nil
}

func (x *PushDataV3ApiWrapper) GetSymbol() string {
	if x != nil && x.Symbol != nil {
		return *x.Symbol
	}
	return ""
}

func (x *PushDataV3ApiWrapper) GetSymbolId() string {
	if x != nil && x.SymbolId != nil {
		return *x.SymbolId
	}
	return ""
}

func (x *PushDataV3ApiWrapper) GetCreateTime() int64 {
	if x != nil && x.CreateTime != nil {
		return *x.CreateTime
	}
	return 0
}

func (x *PushDataV3ApiWrapper) GetSendTime() int64 {
	if x != nil && x.SendTime != nil {
		return *x.SendTime
	}
	return 0
}

type isPushDataV3ApiWrapper_Body interface {
	isPushDataV3ApiWrapper_Body()
}

type PushDataV3ApiWrapper_PublicDeals struct {
	PublicDeals *PublicDealsV3Api `protobuf:"bytes,301,opt,name=publicDeals,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicIncreaseDepths struct {
	PublicIncreaseDepths *PublicIncreaseDepthsV3Api `protobuf:"bytes,302,opt,name=publicIncreaseDepths,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicLimitDepths struct {
	PublicLimitDepths *PublicLimitDepthsV3Api `protobuf:"bytes,303,opt,name=publicLimitDepths,proto3,oneof"`
}

type PushDataV3ApiWrapper_PrivateOrders struct {
	PrivateOrders *PrivateOrdersV3Api `protobuf:"bytes,304,opt,name=privateOrders,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicBookTicker struct {
	PublicBookTicker *PublicBookTickerV3Api `protobuf:"bytes,305,opt,name=publicBookTicker,proto3,oneof"`
}

type PushDataV3ApiWrapper_PrivateDeals struct {
	PrivateDeals *PrivateDealsV3Api `protobuf:"bytes,306,opt,name=privateDeals,proto3,oneof"`
}

type PushDataV3ApiWrapper_PrivateAccount struct {
	PrivateAccount *PrivateAccountV3Api `protobuf:"bytes,307,opt,name=privateAccount,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicSpotKline struct {
	PublicSpotKline *PublicSpotKlineV3Api `protobuf:"bytes,308,opt,name=publicSpotKline,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicMiniTicker struct {
	PublicMiniTicker *PublicMiniTickerV3Api `protobuf:"bytes,309,opt,name=publicMiniTicker,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicMiniTickers struct {
	PublicMiniTickers *PublicMiniTickersV3Api `protobuf:"bytes,310,opt,name=publicMiniTickers,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicBookTickerBatch struct {
	PublicBookTickerBatch *PublicBookTickerBatchV3Api `protobuf:"bytes,311,opt,name=publicBookTickerBatch,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicIncreaseDepthsBatch struct {
	PublicIncreaseDepthsBatch *PublicIncreaseDepthsBatchV3Api `protobuf:"bytes,312,opt,name=publicIncreaseDepthsBatch,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicAggreDepths struct {
	PublicAggreDepths *PublicAggreDepthsV3Api `protobuf:"bytes,313,opt,name=publicAggreDepths,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicAggreDeals struct {
	PublicAggreDeals *PublicAggreDealsV3Api `protobuf:"bytes,314,opt,name=publicAggreDeals,proto3,oneof"`
}

type PushDataV3ApiWrapper_PublicAggreBookTicker struct {
	PublicAggreBookTicker *PublicAggreBookTickerV3Api `protobuf:"bytes,315,opt,name=publicAggreBookTicker,proto3,oneof"`
}

func (*PushDataV3ApiWrapper_PublicDeals) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicIncreaseDepths) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicLimitDepths) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PrivateOrders) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicBookTicker) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PrivateDeals) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PrivateAccount) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicSpotKline) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicMiniTicker) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicMiniTickers) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicBookTickerBatch) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicIncreaseDepthsBatch) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicAggreDepths) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicAggreDeals) isPushDataV3ApiWrapper_Body() {}

func (*PushDataV3ApiWrapper_PublicAggreBookTicker) isPushDataV3ApiWrapper_Body() {}

var File_PushDataV3ApiWrapper_proto protoreflect.FileDescriptor

const file_PushDataV3ApiWrapper_proto_rawDesc = "" +
	"\n" +
	"\x1aPushDataV3ApiWrapper.proto\x12\x02pb\x1a\x16PublicDealsV3Api.proto\x1a\x1fPublicIncreaseDepthsV3Api.proto\x1a\x1cPublicLimitDepthsV3Api.proto\x1a\x18PrivateOrdersV3Api.proto\x1a\x1bPublicBookTickerV3Api.proto\x1a\x17PrivateDealsV3Api.proto\x1a\x19PrivateAccountV3Api.proto\x1a\x1aPublicSpotKlineV3Api.proto\x1a\x1bPublicMiniTickerV3Api.proto\x1a\x1cPublicMiniTickersV3Api.proto\x1a PublicBookTickerBatchV3Api.proto\x1a$PublicIncreaseDepthsBatchV3Api.proto\x1a\x1cPublicAggreDepthsV3Api.proto\x1a\x1bPublicAggreDealsV3Api.proto\x1a PublicAggreBookTickerV3Api.proto\"\xe7\n" +
	"\n" +
	"\x14PushDataV3ApiWrapper\x12\x18\n" +
	"\achannel\x18\x01 \x01(\tR\achannel\x129\n" +
	"\vpublicDeals\x18\xad\x02 \x01(\v2\x14.pb.PublicDealsV3ApiH\x00R\vpublicDeals\x12T\n" +
	"\x14publicIncreaseDepths\x18\xae\x02 \x01(\v2\x1d.pb.PublicIncreaseDepthsV3ApiH\x00R\x14publicIncreaseDepths\x12K\n" +
	"\x11publicLimitDepths\x18\xaf\x02 \x01(\v2\x1a.pb.PublicLimitDepthsV3ApiH\x00R\x11publicLimitDepths\x12?\n" +
	"\rprivateOrders\x18\xb0\x02 \x01(\v2\x16.pb.PrivateOrdersV3ApiH\x00R\rprivateOrders\x12H\n" +
	"\x10publicBookTicker\x18\xb1\x02 \x01(\v2\x19.pb.PublicBookTickerV3ApiH\x00R\x10publicBookTicker\x12<\n" +
	"\fprivateDeals\x18\xb2\x02 \x01(\v2\x15.pb.PrivateDealsV3ApiH\x00R\fprivateDeals\x12B\n" +
	"\x0eprivateAccount\x18\xb3\x02 \x01(\v2\x17.pb.PrivateAccountV3ApiH\x00R\x0eprivateAccount\x12E\n" +
	"\x0fpublicSpotKline\x18\xb4\x02 \x01(\v2\x18.pb.PublicSpotKlineV3ApiH\x00R\x0fpublicSpotKline\x12H\n" +
	"\x10publicMiniTicker\x18\xb5\x02 \x01(\v2\x19.pb.PublicMiniTickerV3ApiH\x00R\x10publicMiniTicker\x12K\n" +
	"\x11publicMiniTickers\x18\xb6\x02 \x01(\v2\x1a.pb.PublicMiniTickersV3ApiH\x00R\x11publicMiniTickers\x12W\n" +
	"\x15publicBookTickerBatch\x18\xb7\x02 \x01(\v2\x1e.pb.PublicBookTickerBatchV3ApiH\x00R\x15publicBookTickerBatch\x12c\n" +
	"\x19publicIncreaseDepthsBatch\x18\xb8\x02 \x01(\v2\".pb.PublicIncreaseDepthsBatchV3ApiH\x00R\x19publicIncreaseDepthsBatch\x12K\n" +
	"\x11publicAggreDepths\x18\xb9\x02 \x01(\v2\x1a.pb.PublicAggreDepthsV3ApiH\x00R\x11publicAggreDepths\x12H\n" +
	"\x10publicAggreDeals\x18\xba\x02 \x01(\v2\x19.pb.PublicAggreDealsV3ApiH\x00R\x10publicAggreDeals\x12W\n" +
	"\x15publicAggreBookTicker\x18\xbb\x02 \x01(\v2\x1e.pb.PublicAggreBookTickerV3ApiH\x00R\x15publicAggreBookTicker\x12\x1b\n" +
	"\x06symbol\x18\x03 \x01(\tH\x01R\x06symbol\x88\x01\x01\x12\x1f\n" +
	"\bsymbolId\x18\x04 \x01(\tH\x02R\bsymbolId\x88\x01\x01\x12#\n" +
	"\n" +
	"createTime\x18\x05 \x01(\x03H\x03R\n" +
	"createTime\x88\x01\x01\x12\x1f\n" +
	"\bsendTime\x18\x06 \x01(\x03H\x04R\bsendTime\x88\x01\x01B\x06\n" +
	"\x04bodyB\t\n" +
	"\a_symbolB\v\n" +
	"\t_symbolIdB\r\n" +
	"\v_createTimeB\v\n" +
	"\t_sendTimeBM\n" +
	"\x1ccom.mxc.push.common.protobufB\x19PushDataV3ApiWrapperProtoH\x01P\x01Z\x0ecrypt_proto/pbb\x06proto3"

var (
	file_PushDataV3ApiWrapper_proto_rawDescOnce sync.Once
	file_PushDataV3ApiWrapper_proto_rawDescData []byte
)

func file_PushDataV3ApiWrapper_proto_rawDescGZIP() []byte {
	file_PushDataV3ApiWrapper_proto_rawDescOnce.Do(func() {
		file_PushDataV3ApiWrapper_proto_rawDescData = protoimpl.X.CompressGZIP(unsafe.Slice(unsafe.StringData(file_PushDataV3ApiWrapper_proto_rawDesc), len(file_PushDataV3ApiWrapper_proto_rawDesc)))
	})
	return file_PushDataV3ApiWrapper_proto_rawDescData
}

var file_PushDataV3ApiWrapper_proto_msgTypes = make([]protoimpl.MessageInfo, 1)
var file_PushDataV3ApiWrapper_proto_goTypes = []any{
	(*PushDataV3ApiWrapper)(nil),           // 0: pb.PushDataV3ApiWrapper
	(*PublicDealsV3Api)(nil),               // 1: pb.PublicDealsV3Api
	(*PublicIncreaseDepthsV3Api)(nil),      // 2: pb.PublicIncreaseDepthsV3Api
	(*PublicLimitDepthsV3Api)(nil),         // 3: pb.PublicLimitDepthsV3Api
	(*PrivateOrdersV3Api)(nil),             // 4: pb.PrivateOrdersV3Api
	(*PublicBookTickerV3Api)(nil),          // 5: pb.PublicBookTickerV3Api
	(*PrivateDealsV3Api)(nil),              // 6: pb.PrivateDealsV3Api
	(*PrivateAccountV3Api)(nil),            // 7: pb.PrivateAccountV3Api
	(*PublicSpotKlineV3Api)(nil),           // 8: pb.PublicSpotKlineV3Api
	(*PublicMiniTickerV3Api)(nil),          // 9: pb.PublicMiniTickerV3Api
	(*PublicMiniTickersV3Api)(nil),         // 10: pb.PublicMiniTickersV3Api
	(*PublicBookTickerBatchV3Api)(nil),     // 11: pb.PublicBookTickerBatchV3Api
	(*PublicIncreaseDepthsBatchV3Api)(nil), // 12: pb.PublicIncreaseDepthsBatchV3Api
	(*PublicAggreDepthsV3Api)(nil),         // 13: pb.PublicAggreDepthsV3Api
	(*PublicAggreDealsV3Api)(nil),          // 14: pb.PublicAggreDealsV3Api
	(*PublicAggreBookTickerV3Api)(nil),     // 15: pb.PublicAggreBookTickerV3Api
}
var file_PushDataV3ApiWrapper_proto_depIdxs = []int32{
	1,  // 0: pb.PushDataV3ApiWrapper.publicDeals:type_name -> pb.PublicDealsV3Api
	2,  // 1: pb.PushDataV3ApiWrapper.publicIncreaseDepths:type_name -> pb.PublicIncreaseDepthsV3Api
	3,  // 2: pb.PushDataV3ApiWrapper.publicLimitDepths:type_name -> pb.PublicLimitDepthsV3Api
	4,  // 3: pb.PushDataV3ApiWrapper.privateOrders:type_name -> pb.PrivateOrdersV3Api
	5,  // 4: pb.PushDataV3ApiWrapper.publicBookTicker:type_name -> pb.PublicBookTickerV3Api
	6,  // 5: pb.PushDataV3ApiWrapper.privateDeals:type_name -> pb.PrivateDealsV3Api
	7,  // 6: pb.PushDataV3ApiWrapper.privateAccount:type_name -> pb.PrivateAccountV3Api
	8,  // 7: pb.PushDataV3ApiWrapper.publicSpotKline:type_name -> pb.PublicSpotKlineV3Api
	9,  // 8: pb.PushDataV3ApiWrapper.publicMiniTicker:type_name -> pb.PublicMiniTickerV3Api
	10, // 9: pb.PushDataV3ApiWrapper.publicMiniTickers:type_name -> pb.PublicMiniTickersV3Api
	11, // 10: pb.PushDataV3ApiWrapper.publicBookTickerBatch:type_name -> pb.PublicBookTickerBatchV3Api
	12, // 11: pb.PushDataV3ApiWrapper.publicIncreaseDepthsBatch:type_name -> pb.PublicIncreaseDepthsBatchV3Api
	13, // 12: pb.PushDataV3ApiWrapper.publicAggreDepths:type_name -> pb.PublicAggreDepthsV3Api
	14, // 13: pb.PushDataV3ApiWrapper.publicAggreDeals:type_name -> pb.PublicAggreDealsV3Api
	15, // 14: pb.PushDataV3ApiWrapper.publicAggreBookTicker:type_name -> pb.PublicAggreBookTickerV3Api
	15, // [15:15] is the sub-list for method output_type
	15, // [15:15] is the sub-list for method input_type
	15, // [15:15] is the sub-list for extension type_name
	15, // [15:15] is the sub-list for extension extendee
	0,  // [0:15] is the sub-list for field type_name
}

func init() { file_PushDataV3ApiWrapper_proto_init() }
func file_PushDataV3ApiWrapper_proto_init() {
	if File_PushDataV3ApiWrapper_proto != nil {
		return
	}
	file_PublicDealsV3Api_proto_init()
	file_PublicIncreaseDepthsV3Api_proto_init()
	file_PublicLimitDepthsV3Api_proto_init()
	file_PrivateOrdersV3Api_proto_init()
	file_PublicBookTickerV3Api_proto_init()
	file_PrivateDealsV3Api_proto_init()
	file_PrivateAccountV3Api_proto_init()
	file_PublicSpotKlineV3Api_proto_init()
	file_PublicMiniTickerV3Api_proto_init()
	file_PublicMiniTickersV3Api_proto_init()
	file_PublicBookTickerBatchV3Api_proto_init()
	file_PublicIncreaseDepthsBatchV3Api_proto_init()
	file_PublicAggreDepthsV3Api_proto_init()
	file_PublicAggreDealsV3Api_proto_init()
	file_PublicAggreBookTickerV3Api_proto_init()
	file_PushDataV3ApiWrapper_proto_msgTypes[0].OneofWrappers = []any{
		(*PushDataV3ApiWrapper_PublicDeals)(nil),
		(*PushDataV3ApiWrapper_PublicIncreaseDepths)(nil),
		(*PushDataV3ApiWrapper_PublicLimitDepths)(nil),
		(*PushDataV3ApiWrapper_PrivateOrders)(nil),
		(*PushDataV3ApiWrapper_PublicBookTicker)(nil),
		(*PushDataV3ApiWrapper_PrivateDeals)(nil),
		(*PushDataV3ApiWrapper_PrivateAccount)(nil),
		(*PushDataV3ApiWrapper_PublicSpotKline)(nil),
		(*PushDataV3ApiWrapper_PublicMiniTicker)(nil),
		(*PushDataV3ApiWrapper_PublicMiniTickers)(nil),
		(*PushDataV3ApiWrapper_PublicBookTickerBatch)(nil),
		(*PushDataV3ApiWrapper_PublicIncreaseDepthsBatch)(nil),
		(*PushDataV3ApiWrapper_PublicAggreDepths)(nil),
		(*PushDataV3ApiWrapper_PublicAggreDeals)(nil),
		(*PushDataV3ApiWrapper_PublicAggreBookTicker)(nil),
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: unsafe.Slice(unsafe.StringData(file_PushDataV3ApiWrapper_proto_rawDesc), len(file_PushDataV3ApiWrapper_proto_rawDesc)),
			NumEnums:      0,
			NumMessages:   1,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_PushDataV3ApiWrapper_proto_goTypes,
		DependencyIndexes: file_PushDataV3ApiWrapper_proto_depIdxs,
		MessageInfos:      file_PushDataV3ApiWrapper_proto_msgTypes,
	}.Build()
	File_PushDataV3ApiWrapper_proto = out.File
	file_PushDataV3ApiWrapper_proto_goTypes = nil
	file_PushDataV3ApiWrapper_proto_depIdxs = nil
}


