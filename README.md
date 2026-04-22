git clone https://github.com/quii/learn-go-with-tests.git
cd learn-go-with-tests


package github.com/quii/learn-go-with-tests/select/v2 is not a main package



🚀 ШАГ 1. Подключение к серверу
Открой терминал (у тебя Linux — идеально) и введи:
ssh root@176.126.99.27
👉 Он спросит:


yes/no → пиши yes


пароль → вставь пароль из панели



⚠️ Если не пускает
Иногда бывает:


порт не 22


пароль не тот


👉 тогда зайди в панель EDIS Global и посмотри:


root password


SSH port



🧱 ШАГ 2. Обновление системы
apt update && apt upgrade -y

🔐 ШАГ 3. Базовая безопасность (очень желательно)
Создай пользователя (не работай всегда от root):
adduser devusermod -aG sudo dev

🌐 ШАГ 4. Установка веб-сервера
apt install -y nginx
Проверка:
systemctl status nginx
👉 теперь открой в браузере:
http://176.126.99.27
Если видишь страницу — всё ок

🗄 ШАГ 5. База данных
apt install -y postgresql
Проверка:
systemctl status postgresql

⚙️ ШАГ 6. Установка Go
(тебе это важно)
apt install -y golang-go
Проверка:
go version

🧠 Что у тебя уже есть после этого
Ты получаешь базу для коллтрекинга:
[Интернет]     ↓   Nginx     ↓   Go API (будет)     ↓ PostgreSQL

🔥 Следующий логический шаг
Теперь тебе нужно решить:
👉 Как принимать звонки
Варианты:
1. Быстрый (рекомендую на старте)


взять провайдера (UIS / SIP)


они шлют вебхук → твой Go сервер


2. Сложный (свой)


ставить Asterisk


настраивать SIP



💡 Я тебе советую
👉 сначала сделать:


сайт


подмену номера


вебхук


запись в БД


👉 без Asterisk

⚡ Дальше могу тебе помочь
Следующий шаг — самый важный:
👉 как связать:
пользователь → ключевик → номер → звонок
Если хочешь — разложу тебе:


структуру БД


схему подмены номера


и Go API под твой проект


Это уже ядро твоего бизнеса.


ssh root@176.126.99.27


IP Address:    176.126.99.27
Username:      root
Password:      0NjYWLbE38yq
SSH Port:      22 (default)




👉 Сообщение:

Could not get lock /var/lib/dpkg/lock-frontend. It is held by process 1297 (apt)

значит, что в системе уже идёт установка/обновление пакетов.

🔍 Что происходит

Обычно это:

автообновление (apt upgrade по cron)
или ты сам в другом терминале запустил apt
или зависший процесс
✅ Что делать
1. Посмотреть, что это за процесс
ps -p 1297 -o pid,cmd

Если увидишь что-то вроде:

apt upgrade

👉 просто подожди 1–2 минуты

2. Если зависло (долго висит)

Проверь:

top

или:

htop

Если процесс реально не работает (0% CPU долгое время):

👉 можно аккуратно убить:

kill -9 1297
3. После этого ОБЯЗАТЕЛЬНО почини dpkg
dpkg --configure -a
4. И только потом ставь nginx
apt update
apt install -y nginx
❗ Важно

Не делай сразу rm lock — это ломает систему пакетов.
Правильный порядок:

понять процесс
убить (если завис)
dpkg --configure -a
⚡ Быстрый чек-лист (как для продакшена)
ps aux | grep apt
kill -9 <pid>        # если завис
dpkg --configure -a
apt update
apt install nginx



Package configuration


 ┌──────────────────────────────────────────────────┤ Configuring openssh-server ├─────────────────────────────────────────────
 │                                                                                                                             
 │ A new version (/tmp/tmp.bNfeqXZtBU) of configuration file /etc/ssh/sshd_config is available, but the version installed curre
 │ has been locally modified.                                                                                                  
 │                                                                                                                             
 │                                                              <Ok>                                                           
 │                                                                                                                             
 └─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
                                                                                                                               
