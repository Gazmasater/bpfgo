git clone https://github.com/quii/learn-go-with-tests.git
cd learn-go-with-tests


package github.com/quii/learn-go-with-tests/select/v2 is not a main package



🚀 ШАГ 1. Подключение к серверу
Открой терминал (у тебя Linux — идеально) и введи:
ssh root@176.126.99.27
👉 Он спросит:


yes/no → пиши yes


пароль → вставь пароль из панели



apt install -y git curl certbot python3-certbot-nginx unzip






2. Собери сайт у себя локально

На своём компьютере в папке сайта:

yarn install
yarn generate

или если без yarn:

npm install
npm run generate

После этого появится папка со статикой. Обычно это:

.output/public
или
dist

У Nuxt 4 чаще смотри сначала:

ls -la .output
ls -la dist

Нужна папка, где лежат готовые html/js/css.

3. Создай папку сайта на сервере

На сервере:

mkdir -p /var/www/site
4. Залей статические файлы на сервер

С компьютера на сервер копируй содержимое папки сборки.

Пример, если сборка лежит в .output/public:

scp -r .output/public/* root@IP_СЕРВЕРА:/var/www/site/

Если сборка в dist:

scp -r dist/* root@IP_СЕРВЕРА:/var/www/site/
5. Проверь, что файлы реально на месте

На сервере:

ls -la /var/www/site

Там должен быть index.html и папки со статикой.

6. Настрой nginx на сайт

Создай конфиг:

nano /etc/nginx/sites-available/site

Вставь:

server {
    listen 80;
    server_name _;

    root /var/www/site;
    index index.html;

    location / {
        try_files $uri $uri/ /index.html;
    }
}

Сохрани.

Включи сайт:

ln -s /etc/nginx/sites-available/site /etc/nginx/sites-enabled/site

Если есть дефолтный конфиг, лучше убрать:

rm -f /etc/nginx/sites-enabled/default

Проверь конфиг:

nginx -t

Перезапусти nginx:

systemctl reload nginx
7. Открой сайт по IP

В браузере открой:

http://IP_СЕРВЕРА

Если всё ок — сайт уже работает.

8. Потом добавишь домен

Когда домен будет привязан к серверу, поменяешь:

server_name _;

на

server_name твой-домен.ru www.твой-домен.ru;
9. Включи HTTPS

После того как домен начнёт смотреть на сервер:

certbot --nginx -d твой-домен.ru -d www.твой-домен.ru
10. Go backend запускай отдельно

Когда дойдёшь до API, схема будет такая:

сайт: nginx отдаёт /var/www/site
API: nginx проксирует /api на Go, например 127.0.0.1:8080

Пример будущего блока для nginx:

location /api/ {
    proxy_pass http://127.0.0.1:8080/;
    proxy_http_version 1.1;
    proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    proxy_set_header X-Forwarded-Proto $scheme;
}
11. Что делать прямо сейчас

Вот твой короткий маршрут:

Локально выполнить yarn generate
Найти папку готовой статики
Залить её в /var/www/site
Создать nginx-конфиг
nginx -t
systemctl reload nginx
Открыть сайт по IP
Важный момент по твоему архиву

У тебя в проекте есть server/api/..., то есть проект в целом умеет больше, чем просто статика. Но раз тебе сейчас нужен простой одностраничник, самый надёжный путь — не поднимать Nuxt как сервер, а отдать готовые файлы через nginx.

Если хочешь, следующим сообщением я дам тебе готовые команды именно для Windows: как собрать Nuxt у себя и как залить папку на сервер без путаницы.
