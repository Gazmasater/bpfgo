
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

var House = models.House{
	ID:          1,
	Name:        "🏡 Коттедж 120 м²",
	Description: "2 этажа, участок 6 соток, коммуникации подведены.",
	PhotoPath:   "data/1.jpg",
	PlanPath:    "data/2.jpg",
}

func HandleUpdate(bot *tgbotapi.BotAPI, update tgbotapi.Update) {
	if update.Message != nil && update.Message.Text == "/start" {
		sendHouseCard(bot, update.Message.Chat.ID)
		return
	}

	if update.CallbackQuery != nil {
		data := update.CallbackQuery.Data
		chatID := update.CallbackQuery.Message.Chat.ID
		messageID := update.CallbackQuery.Message.MessageID

		switch {
		case data == "back_to_list":
			editToHouseCard(bot, chatID, messageID)

		case strings.HasPrefix(data, "house_"):
			idStr := strings.TrimPrefix(data, "house_")
			id, _ := strconv.Atoi(idStr)
			if id == House.ID {
				editToHouseDetails(bot, chatID, messageID)
			}
		}
	}
}

func sendHouseCard(bot *tgbotapi.BotAPI, chatID int64) {
	photo := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath(House.PhotoPath))
	photo.Caption = fmt.Sprintf("*%s*\n%s", House.Name, House.Description)
	photo.ParseMode = "Markdown"
	photo.ReplyMarkup = tgbotapi.NewInlineKeyboardMarkup(
		tgbotapi.NewInlineKeyboardRow(
			tgbotapi.NewInlineKeyboardButtonData("📄 Подробнее", fmt.Sprintf("house_%d", House.ID)),
		),
	)
	bot.Send(photo)
}

func editToHouseDetails(bot *tgbotapi.BotAPI, chatID int64, messageID int) {
	media := tgbotapi.InputMediaPhoto{
		Type:      "photo",
		Media:     tgbotapi.FilePath(House.PlanPath),
		Caption:   fmt.Sprintf("*📐 Планировка*\n%s", House.Description),
		ParseMode: "Markdown",
	}

	edit := tgbotapi.EditMessageMediaConfig{
		BaseEdit: tgbotapi.BaseEdit{
			ChatID:    chatID,
			MessageID: messageID,
		},
		Media: media,
	}
	bot.Send(edit)

	replyMarkup := tgbotapi.NewEditMessageReplyMarkup(chatID, messageID,
		tgbotapi.NewInlineKeyboardMarkup(
			tgbotapi.NewInlineKeyboardRow(
				tgbotapi.NewInlineKeyboardButtonData("⬅️ Назад к списку", "back_to_list"),
			),
		),
	)
	bot.Send(replyMarkup)
}

func editToHouseCard(bot *tgbotapi.BotAPI, chatID int64, messageID int) {
	media := tgbotapi.InputMediaPhoto{
		Type:      "photo",
		Media:     tgbotapi.FilePath(House.PhotoPath),
		Caption:   fmt.Sprintf("*%s*\n%s", House.Name, House.Description),
		ParseMode: "Markdown",
	}

	edit := tgbotapi.EditMessageMediaConfig{
		BaseEdit: tgbotapi.BaseEdit{
			ChatID:    chatID,
			MessageID: messageID,
		},
		Media: media,
	}
	bot.Send(edit)

	replyMarkup := tgbotapi.NewEditMessageReplyMarkup(chatID, messageID,
		tgbotapi.NewInlineKeyboardMarkup(
			tgbotapi.NewInlineKeyboardRow(
				tgbotapi.NewInlineKeyboardButtonData("📄 Подробнее", fmt.Sprintf("house_%d", House.ID)),
			),
		),
	)
	bot.Send(replyMarkup)
}







