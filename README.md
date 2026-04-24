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



cd siteGaz
mkdir -p nginx/sites-available deploy
cp -r /путь/к/remontkotlov48-infra-main/project/nginx/* ./nginx/

git add nginx
git commit -m "add production nginx config"

cp -r project/site/img/* siteGaz/public/img/


<img src="/img/hero.webp">
