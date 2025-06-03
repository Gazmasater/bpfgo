



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
	"log"
	"strconv"
	"tg/models"

	tgbotapi "github.com/go-telegram-bot-api/telegram-bot-api/v5"
)

var Houses = []models.House{
	{ID: 1, Name: "🏡 Дом 120 м²", Description: "2 этажа, участок 6 соток", PhotoURL: "https://terem-dom.ru/d/cimg6172.jpg"},
	{ID: 2, Name: "🏠 Дом 95 м²", Description: "Компактный и тёплый", PhotoURL: "https://terem-dom.ru/d/cimg6177.jpg"},
	{ID: 3, Name: "🏘 Дом с террасой", Description: "С видом на реку", PhotoURL: "https://terem-dom.ru/d/cimg6169.jpg"},
	{ID: 4, Name: "🏕 Коттедж", Description: "Для семьи и отдыха", PhotoURL: "https://terem-dom.ru/d/cimg6170.jpg"},
}

func HandleUpdate(bot *tgbotapi.BotAPI, update tgbotapi.Update) {
	if update.InlineQuery != nil {
		handleInlineQuery(bot, update.InlineQuery)
	}
}

func handleInlineQuery(bot *tgbotapi.BotAPI, query *tgbotapi.InlineQuery) {
	var results []interface{}

	log.Printf("InlineQuery received: %+v", query)

	for _, house := range Houses {
		result := tgbotapi.NewInlineQueryResultPhoto(
			fmt.Sprintf("house_%d", house.ID),
			house.PhotoURL,
		)
		result.Title = house.Name
		result.Description = house.Description
		result.Caption = fmt.Sprintf("*%s*\n%s", house.Name, house.Description)
		result.ParseMode = "Markdown"
		result.ThumbURL = house.PhotoURL

		result.ReplyMarkup = &tgbotapi.InlineKeyboardMarkup{
			InlineKeyboard: [][]tgbotapi.InlineKeyboardButton{
				{
					tgbotapi.NewInlineKeyboardButtonURL("📄 Подробнее", "https://example.com/house?id="+strconv.Itoa(house.ID)),
				},
			},
		}

		results = append(results, result)
	}

	inlineConf := tgbotapi.InlineConfig{
		InlineQueryID: query.ID,
		IsPersonal:    true,
		CacheTime:     0,
		Results:       results,
	}

	if _, err := bot.Request(inlineConf); err != nil {
		log.Println("inline send error:", err)
	}
}











