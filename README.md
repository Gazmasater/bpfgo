
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


sudo apt install -y protobuf-compiler
go install google.golang.org/protobuf/cmd/protoc-gen-go@latest

protoc --go_out=. --go_opt=paths=source_relative PrivateDealsV3Api.proto

syntax = "proto3";
option go_package = "crypt_proto/pb";

sudo apt install ntpdate
sudo ntpdate time.cloudflare.com
curl -s https://api.mexc.com/api/v3/time
date +%s%3N


// spot@public.aggre.deals.v3.api.pb

// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.6
// 	protoc        v3.21.12
// source: PublicAggreDealsV3Api.proto

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

type PublicAggreDealsV3Api struct {
	state         protoimpl.MessageState       `protogen:"open.v1"`
	Deals         []*PublicAggreDealsV3ApiItem `protobuf:"bytes,1,rep,name=deals,proto3" json:"deals,omitempty"`
	EventType     string                       `protobuf:"bytes,2,opt,name=eventType,proto3" json:"eventType,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *PublicAggreDealsV3Api) Reset() {
	*x = PublicAggreDealsV3Api{}
	mi := &file_PublicAggreDealsV3Api_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *PublicAggreDealsV3Api) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*PublicAggreDealsV3Api) ProtoMessage() {}

func (x *PublicAggreDealsV3Api) ProtoReflect() protoreflect.Message {
	mi := &file_PublicAggreDealsV3Api_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use PublicAggreDealsV3Api.ProtoReflect.Descriptor instead.
func (*PublicAggreDealsV3Api) Descriptor() ([]byte, []int) {
	return file_PublicAggreDealsV3Api_proto_rawDescGZIP(), []int{0}
}

func (x *PublicAggreDealsV3Api) GetDeals() []*PublicAggreDealsV3ApiItem {
	if x != nil {
		return x.Deals
	}
	return nil
}

func (x *PublicAggreDealsV3Api) GetEventType() string {
	if x != nil {
		return x.EventType
	}
	return ""
}

type PublicAggreDealsV3ApiItem struct {
	state         protoimpl.MessageState `protogen:"open.v1"`
	Price         string                 `protobuf:"bytes,1,opt,name=price,proto3" json:"price,omitempty"`
	Quantity      string                 `protobuf:"bytes,2,opt,name=quantity,proto3" json:"quantity,omitempty"`
	TradeType     int32                  `protobuf:"varint,3,opt,name=tradeType,proto3" json:"tradeType,omitempty"`
	Time          int64                  `protobuf:"varint,4,opt,name=time,proto3" json:"time,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *PublicAggreDealsV3ApiItem) Reset() {
	*x = PublicAggreDealsV3ApiItem{}
	mi := &file_PublicAggreDealsV3Api_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *PublicAggreDealsV3ApiItem) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*PublicAggreDealsV3ApiItem) ProtoMessage() {}

func (x *PublicAggreDealsV3ApiItem) ProtoReflect() protoreflect.Message {
	mi := &file_PublicAggreDealsV3Api_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

// Deprecated: Use PublicAggreDealsV3ApiItem.ProtoReflect.Descriptor instead.
func (*PublicAggreDealsV3ApiItem) Descriptor() ([]byte, []int) {
	return file_PublicAggreDealsV3Api_proto_rawDescGZIP(), []int{1}
}

func (x *PublicAggreDealsV3ApiItem) GetPrice() string {
	if x != nil {
		return x.Price
	}
	return ""
}

func (x *PublicAggreDealsV3ApiItem) GetQuantity() string {
	if x != nil {
		return x.Quantity
	}
	return ""
}

func (x *PublicAggreDealsV3ApiItem) GetTradeType() int32 {
	if x != nil {
		return x.TradeType
	}
	return 0
}

func (x *PublicAggreDealsV3ApiItem) GetTime() int64 {
	if x != nil {
		return x.Time
	}
	return 0
}

var File_PublicAggreDealsV3Api_proto protoreflect.FileDescriptor

const file_PublicAggreDealsV3Api_proto_rawDesc = "" +
	"\n" +
	"\x1bPublicAggreDealsV3Api.proto\"g\n" +
	"\x15PublicAggreDealsV3Api\x120\n" +
	"\x05deals\x18\x01 \x03(\v2\x1a.PublicAggreDealsV3ApiItemR\x05deals\x12\x1c\n" +
	"\teventType\x18\x02 \x01(\tR\teventType\"\x7f\n" +
	"\x19PublicAggreDealsV3ApiItem\x12\x14\n" +
	"\x05price\x18\x01 \x01(\tR\x05price\x12\x1a\n" +
	"\bquantity\x18\x02 \x01(\tR\bquantity\x12\x1c\n" +
	"\ttradeType\x18\x03 \x01(\x05R\ttradeType\x12\x12\n" +
	"\x04time\x18\x04 \x01(\x03R\x04timeBN\n" +
	"\x1ccom.mxc.push.common.protobufB\x1aPublicAggreDealsV3ApiProtoH\x01P\x01Z\x0ecrypt_proto/pbb\x06proto3"

var (
	file_PublicAggreDealsV3Api_proto_rawDescOnce sync.Once
	file_PublicAggreDealsV3Api_proto_rawDescData []byte
)

func file_PublicAggreDealsV3Api_proto_rawDescGZIP() []byte {
	file_PublicAggreDealsV3Api_proto_rawDescOnce.Do(func() {
		file_PublicAggreDealsV3Api_proto_rawDescData = protoimpl.X.CompressGZIP(unsafe.Slice(unsafe.StringData(file_PublicAggreDealsV3Api_proto_rawDesc), len(file_PublicAggreDealsV3Api_proto_rawDesc)))
	})
	return file_PublicAggreDealsV3Api_proto_rawDescData
}

var file_PublicAggreDealsV3Api_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_PublicAggreDealsV3Api_proto_goTypes = []any{
	(*PublicAggreDealsV3Api)(nil),     // 0: PublicAggreDealsV3Api
	(*PublicAggreDealsV3ApiItem)(nil), // 1: PublicAggreDealsV3ApiItem
}
var file_PublicAggreDealsV3Api_proto_depIdxs = []int32{
	1, // 0: PublicAggreDealsV3Api.deals:type_name -> PublicAggreDealsV3ApiItem
	1, // [1:1] is the sub-list for method output_type
	1, // [1:1] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_PublicAggreDealsV3Api_proto_init() }
func file_PublicAggreDealsV3Api_proto_init() {
	if File_PublicAggreDealsV3Api_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: unsafe.Slice(unsafe.StringData(file_PublicAggreDealsV3Api_proto_rawDesc), len(file_PublicAggreDealsV3Api_proto_rawDesc)),
			NumEnums:      0,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_PublicAggreDealsV3Api_proto_goTypes,
		DependencyIndexes: file_PublicAggreDealsV3Api_proto_depIdxs,
		MessageInfos:      file_PublicAggreDealsV3Api_proto_msgTypes,
	}.Build()
	File_PublicAggreDealsV3Api_proto = out.File
	file_PublicAggreDealsV3Api_proto_goTypes = nil
	file_PublicAggreDealsV3Api_proto_depIdxs = nil
}


package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	"github.com/gorilla/websocket"
	"google.golang.org/protobuf/proto"

	pb "crypt_proto/pb" // путь к PrivateOrderPush
)

func makeSignature(secret, apiKey string, ts int64) string {
	msg := []byte(fmt.Sprintf("%d%s", ts, apiKey))
	mac := hmac.New(sha256.New, []byte(secret))
	mac.Write(msg)
	return hex.EncodeToString(mac.Sum(nil))
}

func main() {
	apiKey := os.Getenv("MEXC_API_KEY")
	secret := os.Getenv("MEXC_SECRET_KEY")

	header := http.Header{}
	header.Set("Sec-WebSocket-Protocol", "protobuf")

	conn, _, err := websocket.DefaultDialer.Dial("wss://wbs.mexc.com/ws", header)
	if err != nil {
		log.Fatal("❌ dial:", err)
	}
	defer conn.Close()

	// 1. Авторизация
	ts := time.Now().UnixMilli()
	sig := makeSignature(secret, apiKey, ts)
	auth := map[string]interface{}{
		"method": "AUTH",
		"params": map[string]interface{}{
			"apiKey":    apiKey,
			"timestamp": ts,
			"signature": sig,
		},
		"id": 1,
	}
	if err := conn.WriteJSON(auth); err != nil {
		log.Fatal("❌ auth send:", err)
	}
	log.Println("✅ AUTH sent")

	// 2. Подписка на приватные ордера
	sub := map[string]interface{}{
		"method": "SUBSCRIPTION",
		"params": []string{"spot@private.order.v3.api"},
		"id":     2,
	}
	if err := conn.WriteJSON(sub); err != nil {
		log.Fatal("❌ sub send:", err)
	}
	log.Println("✅ SUBSCRIBE sent")

	// 3. Обработка бинарных сообщений
	for {
		mt, data, err := conn.ReadMessage()
		if err != nil {
			log.Println("❌ read:", err)
			break
		}
		if mt != websocket.BinaryMessage {
			log.Printf("⚠️  Non-binary message: %s", data)
			continue
		}

		var msg pb.PublicAggreDealsV3Api
		if err := proto.Unmarshal(data, &msg); err != nil {
			log.Println("❌ proto.Unmarshal:", err)
			continue
		}

		log.Printf("📥 Trade: %s %s amount=%s | maker=%v | fee=%s %s | time=%d",
			msg.Price,
			msg.Quantity,
			msg.Amount,
			msg.IsMaker,
			msg.FeeAmount,
			msg.FeeCurrency,
			msg.Time,
		)
	}
}













