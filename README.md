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



cd .output/public
tar -czf site.tar.gz .
scp site.tar.gz root@176.126.99.27:/root/



ssh root@176.126.99.27
mkdir -p /var/www/html
rm -rf /var/www/html/*
tar -xzf /root/site.tar.gz -C /var/www/html
chown -R www-data:www-data /var/www/html
chmod -R 755 /var/www/html
systemctl reload nginx


