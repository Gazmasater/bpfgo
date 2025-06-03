
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

🔧 Шаги реализации
1. 🔁 Добавь в sendHouseDetails() кнопку "Назад"
go
Копировать
Редактировать
func sendHouseDetails(bot *tgbotapi.BotAPI, chatID int64) {
    // Планировка
    plan := tgbotapi.NewDocument(chatID, tgbotapi.FilePath(House.PlanPath))
    plan.Caption = "📐 Планировка"
    bot.Send(plan)

    // Фото комнат
    for _, roomPath := range House.RoomPhotos {
        roomPhoto := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath(roomPath))
        bot.Send(roomPhoto)
    }

    // Кнопка "Назад"
    msg := tgbotapi.NewMessage(chatID, "⬅️ Вернуться к списку домов")
    msg.ReplyMarkup = tgbotapi.NewInlineKeyboardMarkup(
        tgbotapi.NewInlineKeyboardRow(
            tgbotapi.NewInlineKeyboardButtonData("⬅️ Назад к списку", "back_to_list"),
        ),
    )
    bot.Send(msg)
}
2. 🏘 Функция sendHouseList() — отправка карточек всех домов
Добавь её (можно вызывать и из /start, и из back_to_list):

go
Копировать
Редактировать
func sendHouseList(bot *tgbotapi.BotAPI, chatID int64) {
    msg := tgbotapi.NewPhoto(chatID, tgbotapi.FilePath(House.PhotoPath))
    msg.Caption = fmt.Sprintf("*%s*\n%s", House.Name, House.Description)
    msg.ParseMode = "Markdown"
    msg.ReplyMarkup = tgbotapi.NewInlineKeyboardMarkup(
        tgbotapi.NewInlineKeyboardRow(
            tgbotapi.NewInlineKeyboardButtonData("📄 Подробнее", fmt.Sprintf("house_%d", House.ID)),
        ),
    )
    bot.Send(msg)
}
3. 🎯 Обработка callback back_to_list в HandleUpdate
В HandleUpdate, добавь:

go
Копировать
Редактировать
if update.CallbackQuery != nil {
    data := update.CallbackQuery.Data

    switch {
    case data == "back_to_list":
        sendHouseList(bot, update.CallbackQuery.Message.Chat.ID)

    case strings.HasPrefix(data, "house_"):
        idStr := strings.TrimPrefix(data, "house_")
        id, _ := strconv.Atoi(idStr)
        if id == House.ID {
            sendHouseDetails(bot, update.CallbackQuery.Message.Chat.ID)
        }
    }
}
✅ Готово!
Теперь:

/start → список домов (фото + кнопка "Подробнее")

"Подробнее" → планировка + комнаты + кнопка "Назад к списку"

"Назад к списку" → повторно показывает карточку дома










