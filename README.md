git clone https://github.com/quii/learn-go-with-tests.git
cd learn-go-with-tests


package github.com/quii/learn-go-with-tests/select/v2 is not a main package



🚀 ШАГ 1. Подключение к серверу
Открой терминал (у тебя Linux — идеально) и введи:
ssh root@176.126.99.27
👉 Он спросит:


1. Установка certbot:
sudo apt update
sudo apt install certbot python3-certbot-nginx -y
2. Получить сертификат:
sudo certbot --nginx -d remontkotlov48.ru -d www.remontkotlov48.ru
3. После этого:


IP Address:    176.126.99.27
Username:      root
Password:      0NjYWLbE38yq
SSH Port:      22 (default)


sudo nano /etc/nginx/sites-available/remontkotlov48



siteGaz/
├── app/
├── public/
├── package.json
├── nuxt.config.ts
├── nginx/
│   ├── nginx.conf
│   └── sites-available/
│       └── remontkotlov48
└── deploy/
 34
 35
 36
 37
 38
 39
 40
 41
 42
 43
 44
 45
 46
🚀 ШАГ 1. Подключение к серверу
Открой терминал (у тебя Linux — идеально) и введи:
ssh root@176.126.99.27
👉 Он спросит:


1. Установка certbot:
sudo apt update
sudo apt install certbot python3-certbot-nginx -y
2. Получить сертификат:
sudo certbot --nginx -d remontkotlov48.ru -d www.remontkotlov48.ru
3. После этого:


IP Address:    176.126.99.27
Username:      root
Password:      0NjYWLbE38yq
SSH Port:      22 (default)


sudo nano /etc/nginx/sites-available/remontkotlov48



gaz358@gaz358-BOD-WXX9:~/myprog$ ls
arb    btn    crypt_proto  ebpf-nftrace    go.mod      go.sum            mql     olimp  remontkotlov48-infra  TG
bpfgo  crypt  db_trace     gazmaster-site  Go_nachalo  kucoin-ising-bot  nft-go  pars   sobes                 workmate
gaz358@gaz358-BOD-WXX9:~/myprog$ 


gaz358@gaz358-BOD-WXX9:~/myprog/gazmaster-site$ mkdir -p nginx/sites-available deploy
gaz358@gaz358-BOD-WXX9:~/myprog/gazmaster-site$ cp -r /myprog/remontkotlov48-infra/project/nginx/* ./nginx/
cp: cannot stat '/myprog/remontkotlov48-infra/project/nginx/*': No such file or directory
