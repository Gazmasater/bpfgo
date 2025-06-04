



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

🚀 Пример по шагам
1. Зарегистрируйся на github.com
2. Создай репозиторий, например house-bot-webapp
3. Клонируй к себе:
bash
Копировать
Редактировать
git clone https://github.com/<твоё_имя>/house-bot-webapp.git
cd house-bot-webapp
4. Добавь туда index.html (из предыдущего ответа)
5. Закоммить и запушь:
bash
Копировать
Редактировать
git add .
git commit -m "init webapp"
git push origin main
6. Включи GitHub Pages:
Зайди в настройки репозитория → "Pages"

Выбери ветку main и папку /root

Сохрани

🟢 Через 30 секунд появится ссылка на сайт.

________________________________________________________________________________-



<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Витрина домов</title>
  <style>
    body {
      margin: 0;
      font-family: sans-serif;
      background: #f5f5f5;
    }
    .header {
      background: #ffffff;
      padding: 16px;
      text-align: center;
      font-size: 1.2em;
      font-weight: bold;
      border-bottom: 1px solid #ddd;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(2, 1fr);
      gap: 12px;
      padding: 16px;
    }
    .card {
      background: white;
      border-radius: 12px;
      overflow: hidden;
      box-shadow: 0 1px 4px rgba(0, 0, 0, 0.1);
      text-align: center;
    }
    .card img {
      width: 100%;
      height: 120px;
      object-fit: cover;
    }
    .card p {
      margin: 0;
      padding: 8px;
      font-weight: 500;
    }
  </style>
</head>
<body>
  <div class="header">Выберите дом</div>
  <div class="grid">
    <div class="card">
      <img src="https://terem-dom.ru/d/cimg6172.jpg" alt="Дом 1">
      <p>🏡 Дом 120 м²</p>
    </div>
    <div class="card">
      <img src="https://terem-dom.ru/d/cimg6177.jpg" alt="Дом 2">
      <p>🏠 Дом 95 м²</p>
    </div>
    <div class="card">
      <img src="https://terem-dom.ru/d/cimg6169.jpg" alt="Дом 3">
      <p>🏘 Дом с террасой</p>
    </div>
    <div class="card">
      <img src="https://terem-dom.ru/d/cimg6170.jpg" alt="Дом 4">
      <p>🏕 Коттедж</p>
    </div>
  </div>
</body>
</html>

___________________________________________________________________________________________

module dommechty

go 1.24.0

require (
	github.com/go-telegram-bot-api/telegram-bot-api/v5 v5.5.1
	github.com/joho/godotenv v1.5.1
)

[{
	"resource": "/home/gaz358/myprog/dommechty/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredImportedName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredImportedName"
		}
	},
	"severity": 8,
	"message": "undefined: tgbotapi.NewInlineKeyboardButtonWebApp",
	"source": "compiler",
	"startLineNumber": 38,
	"startColumn": 15,
	"endLineNumber": 38,
	"endColumn": 44
}]

[{
	"resource": "/home/gaz358/myprog/dommechty/main.go",
	"owner": "_generated_diagnostic_collection_name_#0",
	"code": {
		"value": "UndeclaredImportedName",
		"target": {
			"$mid": 1,
			"path": "/golang.org/x/tools/internal/typesinternal",
			"scheme": "https",
			"authority": "pkg.go.dev",
			"fragment": "UndeclaredImportedName"
		}
	},
	"severity": 8,
	"message": "undefined: tgbotapi.WebAppInfo",
	"source": "compiler",
	"startLineNumber": 38,
	"startColumn": 77,
	"endLineNumber": 38,
	"endColumn": 87
}]







