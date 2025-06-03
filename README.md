
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

package bot

import (
	"fmt"
	"strconv"
	"strings"
	"telegram-house-bot/models"

	tgbotapi "github.com/go-telegram-bot-api/telegram-bot-api/v5"
)

var Houses = []models.House{
	{ID: 1, Name: "🏡 Дом 120 м²", Description: "Уютный дом, 2 этажа, 6 соток.", PhotoPath: "data/1.jpg", PlanPath: "data/2.jpg"},
	{ID: 2, Name: "🏠 Дом 95 м²", Description: "Компактный и тёплый, всё включено.", PhotoPath: "data/1.jpg", PlanPath: "data/2.jpg"},
	{ID: 3, Name: "🏘 Дом с террасой", Description: "С видом на лес и реку.", PhotoPath: "data/1.jpg", PlanPath: "data/2.jpg"},
	{ID: 4, Name: "🏕 Коттедж", Description: "Для отдыха и семьи.", PhotoPath: "data/1.jpg", PlanPath: "data/2.jpg"},
}

func HandleUpdate(bot *tgbotapi.BotAPI, update tgbotapi.Update) {
	if update.Message != nil && update.Message.Text == "/start" {
		sendShowcase(bot, update.Message.Chat.ID)
		return
	}

	if update.CallbackQuery != nil {
		chatID := update.CallbackQuery.Message.Chat.ID
		data := update.CallbackQuery.Data

		switch {
		case data == "back_to_catalog":
			sendShowcase(bot, chatID)
		case strings.HasPrefix(data, "house_"):
			idStr := strings.TrimPrefix(data, "house_")
			id, _ := strconv.Atoi(idStr)
			for _, h := range Houses {
				if h.ID == id {
					showHouseDetails(bot, chatID, h)
					break
				}
			}
		}
	}
}

func sendShowcase(bot *tgbotapi.BotAPI, chatID int64) {
	// Шапка
	header := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath("data/3.jpg"))
	header.Caption = "*🏘 Каталог домов*\nВыберите интересующий вариант:"
	header.ParseMode = "Markdown"
	bot.Send(header)

	// Карточки домов
	for _, house := range Houses {
		msg := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath(house.PhotoPath))
		msg.Caption = fmt.Sprintf("*%s*\n%s", house.Name, house.Description)
		msg.ParseMode = "Markdown"
		msg.ReplyMarkup = tgbotapi.NewInlineKeyboardMarkup(
			tgbotapi.NewInlineKeyboardRow(
				tgbotapi.NewInlineKeyboardButtonData("📄 Подробнее", fmt.Sprintf("house_%d", house.ID)),
			),
		)
		bot.Send(msg)
	}
}

func showHouseDetails(bot *tgbotapi.BotAPI, chatID int64, house models.House) {
	// План
	plan := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath(house.PlanPath))
	plan.Caption = fmt.Sprintf("*📐 Планировка*\n%s", house.Description)
	plan.ParseMode = "Markdown"
	plan.ReplyMarkup = tgbotapi.NewInlineKeyboardMarkup(
		tgbotapi.NewInlineKeyboardRow(
			tgbotapi.NewInlineKeyboardButtonData("⬅️ Назад к списку", "back_to_catalog"),
		),
	)
	bot.Send(plan)
}






