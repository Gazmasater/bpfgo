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



🔧 1. Создаём конфиг nginx
Открой файл:
sudo nano /etc/nginx/sites-available/remontkotlov48
Вставь:
server {    listen 80;    server_name remontkotlov48.ru www.remontkotlov48.ru;    root /var/www/html;    index index.html;    location / {        try_files $uri $uri/ =404;    }}

🔗 2. Активируем
sudo ln -s /etc/nginx/sites-available/remontkotlov48 /etc/nginx/sites-enabled/

🔄 3. Перезапуск nginx
sudo nginx -tsudo systemctl reload nginx

🔐 4. Теперь снова ставим SSL
sudo certbot --nginx -d remontkotlov48.ru -d www.remontkotlov48.ru

✅ Что должно получиться


nginx начнёт слушать домен


certbot сам добавит:


HTTPS (443)


редирект с HTTP → HTTPS





⚠️ После этого
👉 В Cloudflare обязательно:
SSL/TLS → поставить:
👉 Full (не Flexible!)

💡 Почему ошибка была
У тебя nginx сейчас:


работает по IP


но не привязан к домену



🚀 Финал
Сделай шаги выше и напиши:
👉 что выдал второй запуск certbot
Если что — добьём до полностью рабочего HTTPS 🔥




root@calltracking-server:~# sudo ln -s /etc/nginx/sites-available/remontkotlov48 /etc/nginx/sites-enabled/
ln: failed to create symbolic link '/etc/nginx/sites-enabled/remontkotlov48': File exists
root@calltracking-server:~# sudo nginx -tsudo systemctl reload nginx
nginx: invalid option: "-s udo"
root@calltracking-server:~# 
