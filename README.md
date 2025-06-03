
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


______________________________________________________________________________________________
TG

📁 Структура проекта

telegram-house-bot/
├── main.go
├── go.mod
├── models/
│   └── house.go
├── bot/
│   └── handlers.go
├── data/
│   ├── house.jpg
│   ├── plan.pdf
│   ├── room1.jpg
│   └── room2.jpg
🔧 go.mod

module telegram-house-bot

go 1.20

require github.com/go-telegram-bot-api/telegram-bot-api/v5 v5.5.1
🧱 models/house.go


type House struct {
	ID          int
	Name        string
	Description string
	PhotoPath   string
	PlanPath    string
	RoomPhotos  []string
}
🤖 bot/handlers.go

package bot

import (
	"fmt"
	"strconv"
	"strings"
	"telegram-house-bot/models"

	tgbotapi "github.com/go-telegram-bot-api/telegram-bot-api/v5"
)

var House = models.House{
	ID:          1,
	Name:        "🏡 Коттедж 120 м²",
	Description: "2 этажа, участок 6 соток, коммуникации подведены.",
	PhotoPath:   "data/house.jpg",
	PlanPath:    "data/plan.pdf",
	RoomPhotos: []string{
		"data/room1.jpg",
		"data/room2.jpg",
	},
}

func HandleUpdate(bot *tgbotapi.BotAPI, update tgbotapi.Update) {
	if update.Message != nil && update.Message.Text == "/start" {
		msg := tgbotapi.NewPhoto(update.Message.Chat.ID, tgbotapi.FilePath(House.PhotoPath))
		msg.Caption = fmt.Sprintf("*%s*\n%s", House.Name, House.Description)
		msg.ParseMode = "Markdown"
		msg.ReplyMarkup = tgbotapi.NewInlineKeyboardMarkup(
			tgbotapi.NewInlineKeyboardRow(
				tgbotapi.NewInlineKeyboardButtonData("📄 Подробнее", "house_1"),
			),
		)
		bot.Send(msg)
	}

	if update.CallbackQuery != nil && strings.HasPrefix(update.CallbackQuery.Data, "house_") {
		idStr := strings.TrimPrefix(update.CallbackQuery.Data, "house_")
		id, _ := strconv.Atoi(idStr)
		if id == House.ID {
			sendHouseDetails(bot, update.CallbackQuery.Message.Chat.ID)
		}
	}
}

func sendHouseDetails(bot *tgbotapi.BotAPI, chatID int64) {
	// Планировка
	plan := tgbotapi.NewDocument(chatID, tgbotapi.FilePath(House.PlanPath))
	plan.Caption = "📐 Планировка"
	bot.Send(plan)

	// Фото комнат
	for _, photoPath := range House.RoomPhotos {
		photo := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath(photoPath))
		bot.Send(photo)
	}
}
🚀 main.go
go
Копировать
Редактировать
package main

import (
	"log"
	"os"
	"telegram-house-bot/bot"

	tgbotapi "github.com/go-telegram-bot-api/telegram-bot-api/v5"
)

func main() {
	botToken := os.Getenv("BOT_TOKEN")
	if botToken == "" {
		log.Fatal("Установи переменную окружения BOT_TOKEN")
	}

	botAPI, err := tgbotapi.NewBotAPI(botToken)
	if err != nil {
		log.Fatal(err)
	}

	u := tgbotapi.NewUpdate(0)
	u.Timeout = 60
	updates := botAPI.GetUpdatesChan(u)

	for update := range updates {
		bot.HandleUpdate(botAPI, update)
	}
}
🧪 Как запустить
Установи зависимости:


go mod tidy
Положи картинки и PDF:


data/
├── house.jpg
├── plan.pdf
├── room1.jpg
└── room2.jpg
Установи токен:


export BOT_TOKEN=твой_токен_от_BotFather
Запусти:


go run main.go
Если хочешь, я могу подготовить архив .zip со всеми файлами и заглушками, или помочь подключить S3/CDN, когда будешь готов.

Хочешь, чтобы я сгенерировал zip-архив этого проекта и дал тебе ссылку?


BOT_TOKEN=7848415216:AAE4pym6PRDMRFtI9JwpmGU-Rv5obyH-bmo








