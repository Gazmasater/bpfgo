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



sudo cat /etc/nginx/sites-available/remontkotlov48



ls -la /var/www/html


root@calltracking-server:~# ls -la /var/www/html
total 12
drwxr-xr-x 2 root root 4096 Apr 22 11:33 .
drwxr-xr-x 4 root root 4096 Apr 22 12:37 ..
-rw-r--r-- 1 root root  612 Apr 22 11:33 index.nginx-debian.html
root@calltracking-server:~# 




