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



Welcome to Ubuntu 22.04.5 LTS (GNU/Linux 5.15.0-161-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/pro

 System information as of Thu Apr 23 08:28:44 UTC 2026

  System load:  0.0                Processes:             98
  Usage of /:   16.5% of 19.20GB   Users logged in:       0
  Memory usage: 18%                IPv4 address for ens3: 176.126.99.27
  Swap usage:   0%                 IPv6 address for ens3: 2a03:f80:48:98ab::1

 * Strictly confined Kubernetes makes edge and IoT secure. Learn how MicroK8s
   just raised the bar for easy, resilient and secure K8s cluster deployment.

   https://ubuntu.com/engage/secure-kubernetes-at-the-edge

Expanded Security Maintenance for Applications is not enabled.

0 updates can be applied immediately.

Enable ESM Apps to receive additional future security updates.
See https://ubuntu.com/esm or run: sudo pro status

New release '24.04.4 LTS' available.
Run 'do-release-upgrade' to upgrade to it.


*** System restart required ***
Last login: Thu Apr 23 08:13:02 2026 from 95.26.188.11
root@calltracking-server:~# sudo cat /etc/nginx/sites-available/remontkotlov48
server {    server_name remontkotlov48.ru www.remontkotlov48.ru;    root /var/www/html;    index index.html;    location / {        try_files $uri $uri/ =404;    }
    listen 443 ssl; # managed by Certbot
    ssl_certificate /etc/letsencrypt/live/remontkotlov48.ru/fullchain.pem; # managed by Certbot
    ssl_certificate_key /etc/letsencrypt/live/remontkotlov48.ru/privkey.pem; # managed by Certbot
    include /etc/letsencrypt/options-ssl-nginx.conf; # managed by Certbot
    ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem; # managed by Certbot


}

server {
    if ($host = www.remontkotlov48.ru) {
        return 301 https://$host$request_uri;
    } # managed by Certbot


    if ($host = remontkotlov48.ru) {
        return 301 https://$host$request_uri;
    } # managed by Certbot

    listen 80;    server_name remontkotlov48.ru www.remontkotlov48.ru;
    return 404; # managed by Certbot




}root@calltracking-server:~# 




