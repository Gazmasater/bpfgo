



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

sudo apt install mitmproxy

mitmproxy
http://mitm.it




✅ Цель:
Зайти на https://ozon.ru в браузере.

Перехватить и увидеть все отправленные запросы и то, что отправилось сайту: заголовки, cookies, IP и прочее.

🧰 Что нужно:
mitmproxy (или mitmweb)

Сертификат установлен в систему или браузер

Браузер настроен на использование прокси

🔧 Шаг 1: Запусти mitmproxy (или mitmweb)
В терминале:

mitmweb
Это откроет веб-интерфейс: http://127.0.0.1:8081

Прокси работает на 127.0.0.1:8080

🔧 Шаг 2: Настрой браузер на использование прокси
В Firefox:
Настройки → Сеть → Настроить соединение

Прокси-сервер:

HTTP Proxy: 127.0.0.1

Port: 8080

Также использовать для HTTPS

В Chrome (через системные настройки):
Linux/macOS:

google-chrome --proxy-server="http=127.0.0.1:8080;https=127.0.0.1:8080"
🔧 Шаг 3: Установи сертификат mitmproxy, если не установил
Перейди в браузере на:

http://mitm.it
Выбери Linux/macOS → скачай и установи сертификат (инструкции выше).

🔎 Шаг 4: Зайди вручную на https://www.ozon.ru
Открой в браузере Ozon, полазай по сайту (главная, авторизация, каталог и т.д.)

Все запросы будут автоматически видны в mitmproxy / mitmweb.

👁 Что смотреть в mitmweb
Перейди на http://127.0.0.1:8081:

Нажми на нужный запрос → справа:

Request → заголовки (User-Agent, Cookie, и т.д.)

Response → тело ответа

Также можно экспортировать запрос или сохранить лог.

🧠 Что это даёт
Теперь ты точно видишь, что получает сайт:

Твои cookies и ID сессии

Что отправляется при входе, поиске, фильтрации и т.д.

Можно сохранить User-Agent, Cookie и использовать их в автоматических скриптах позже
________________________________________________________________________________

gaz358@gaz358-BOD-WXX9:~/myprog/dommechty$ sudo apt install mitmproxy
[sudo] password for gaz358: 
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following packages were automatically installed and are no longer required:
  amd64-microcode bpfcc-tools bpftrace ieee-data intel-microcode iucode-tool libbpfcc
  linux-headers-6.11.0-25-generic linux-headers-generic-hwe-24.04 linux-hwe-6.11-headers-6.11.0-25
  python3-bpfcc python3-netaddr thermald ubuntu-kernel-accessories
Use 'sudo apt autoremove' to remove them.
The following additional packages will be installed:
  fonts-font-awesome javascript-common libjs-jquery python3-asgiref python3-brotli python3-flask python3-h11
  python3-h2 python3-hpack python3-hyperframe python3-itsdangerous python3-kaitaistruct python3-ldap3
  python3-msgpack python3-openssl python3-passlib python3-protobuf python3-publicsuffix2 python3-pyasn1
  python3-pyasyncore python3-pyinotify python3-pyperclip python3-ruamel.yaml python3-ruamel.yaml.clib
  python3-simplejson python3-sortedcontainers python3-tornado python3-urwid python3-wcwidth python3-werkzeug
  python3-wsproto python3-zstandard
Suggested packages:
  apache2 | lighttpd | httpd python3-dotenv python-flask-doc python-openssl-doc python3-openssl-dbg
  python-pyinotify-doc python-sortedcontainers-doc python3-pycurl python-tornado-doc python3-twisted
  python-urwid-doc ipython3 python-werkzeug-doc python3-lxml python3-watchdog python-zstandard-doc
The following NEW packages will be installed:
  fonts-font-awesome javascript-common libjs-jquery mitmproxy python3-asgiref python3-brotli python3-flask
  python3-h11 python3-h2 python3-hpack python3-hyperframe python3-itsdangerous python3-kaitaistruct
  python3-ldap3 python3-msgpack python3-openssl python3-passlib python3-protobuf python3-publicsuffix2
  python3-pyasn1 python3-pyasyncore python3-pyinotify python3-pyperclip python3-ruamel.yaml
  python3-ruamel.yaml.clib python3-simplejson python3-sortedcontainers python3-tornado python3-urwid
  python3-wcwidth python3-werkzeug python3-wsproto python3-zstandard
0 upgraded, 33 newly installed, 0 to remove and 230 not upgraded.
Need to get 4,824 kB of archives.
After this operation, 22.1 MB of additional disk space will be used.
Do you want to continue? [Y/n] Y
Get:1 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 fonts-font-awesome all 5.0.10+really4.7.0~dfsg-4.1 [516 kB]
Get:2 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-hpack all 4.0.0-2 [22.8 kB]
Get:3 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-hyperframe all 6.0.0-1 [12.2 kB]
Get:4 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-h2 all 4.1.0-4 [73.6 kB]
Get:5 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-asgiref all 3.7.2-1 [24.8 kB]
Get:6 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-brotli amd64 1.1.0-2build2 [332 kB]
Get:7 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-itsdangerous all 2.1.2-4 [14.6 kB]
Get:8 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 libjs-jquery all 3.6.1+dfsg+~3.5.14-1 [328 kB]
Get:9 http://ru.archive.ubuntu.com/ubuntu noble-updates/main amd64 python3-werkzeug all 3.0.1-3ubuntu0.2 [170 kB]
Get:10 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-flask all 3.0.2-1ubuntu1 [82.9 kB]
Get:11 http://ru.archive.ubuntu.com/ubuntu noble-updates/universe amd64 python3-h11 all 0.14.0-1ubuntu0.24.04.1 [51.7 kB]
Get:12 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-kaitaistruct all 0.10-1 [8,474 B]
Get:13 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-pyasn1 all 0.4.8-4 [51.2 kB]
Get:14 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-ldap3 all 2.9.1-2 [241 kB]
Get:15 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-msgpack amd64 1.0.3-3build2 [80.1 kB]
Get:16 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-openssl all 23.2.0-1 [47.8 kB]
Get:17 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-passlib all 1.7.4-4 [476 kB]
Get:18 http://ru.archive.ubuntu.com/ubuntu noble-updates/universe amd64 python3-protobuf amd64 3.21.12-8.2ubuntu0.1 [199 kB]
Get:19 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-publicsuffix2 all 2.20191221-3 [11.6 kB]
Get:20 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-pyperclip all 1.8.2-2 [11.5 kB]
Get:21 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-ruamel.yaml.clib amd64 0.2.8-1build1 [132 kB]
Get:22 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-ruamel.yaml all 0.17.21-1 [86.6 kB]
Get:23 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-sortedcontainers all 2.4.0-2 [27.6 kB]
Ign:24 http://ru.archive.ubuntu.com/ubuntu noble-updates/main amd64 python3-tornado amd64 6.4.0-1ubuntu0.1
Get:25 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-wcwidth all 0.2.5+dfsg1-1.1ubuntu1 [22.5 kB]
Get:26 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-urwid amd64 2.6.10-1 [183 kB]
Get:27 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-wsproto all 1.2.0-1 [23.6 kB]
Get:28 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 python3-zstandard amd64 0.22.0-1build1 [360 kB]
Err:24 http://security.ubuntu.com/ubuntu noble-updates/main amd64 python3-tornado amd64 6.4.0-1ubuntu0.1
  404  Not Found [IP: 213.180.204.183 80]
Get:29 http://ru.archive.ubuntu.com/ubuntu noble/universe amd64 mitmproxy all 8.1.1-2 [839 kB]
Get:30 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 javascript-common all 11+nmu1 [5,936 B]
Get:31 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-pyasyncore all 1.0.2-2 [10.1 kB]
Get:32 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-pyinotify all 0.9.6-2ubuntu1 [25.0 kB]
Get:33 http://ru.archive.ubuntu.com/ubuntu noble/main amd64 python3-simplejson amd64 3.19.2-1build2 [54.5 kB]
Fetched 4,526 kB in 2s (2,891 kB/s)            
E: Failed to fetch http://security.ubuntu.com/ubuntu/pool/main/p/python-tornado/python3-tornado_6.4.0-1ubuntu0.1_amd64.deb  404  Not Found [IP: 213.180.204.183 80]
E: Unable to fetch some archives, maybe run apt-get update or try with --fix-missing?
gaz358@gaz358-BOD-WXX9:~/myprog/dommechty$ 











