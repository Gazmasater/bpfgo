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
tar -czf /tmp/site.tar.gz .
ls -lh /tmp/site.tar.gz
scp /tmp/site.tar.gz root@176.126.99.27:/root/





ssh root@176.126.99.27
mkdir -p /var/www/html
rm -rf /var/www/html/*
tar -xzf /root/site.tar.gz -C /var/www/html
chown -R www-data:www-data /var/www/html
chmod -R 755 /var/www/html
systemctl reload nginx



sudo nano /etc/nginx/sites-available/remontkotlov48



<script setup>
useHead({
  script: [
    {
      innerHTML: `(function(m,e,t,r,i,k,a){
        m[i]=m[i]||function(){(m[i].a=m[i].a||[]).push(arguments)};
        m[i].l=1*new Date();
        for (var j = 0; j < document.scripts.length; j++) {
          if (document.scripts[j].src === r) { return; }
        }
        k=e.createElement(t),a=e.getElementsByTagName(t)[0],
        k.async=1,k.src=r,a.parentNode.insertBefore(k,a)
      })(window, document,'script','https://mc.yandex.ru/metrika/tag.js?id=108736994','ym');

      ym(108736994, 'init', {
        ssr:true,
        webvisor:true,
        clickmap:true,
        ecommerce:"dataLayer",
        referrer: document.referrer,
        url: location.href,
        accurateTrackBounce:true,
        trackLinks:true
      });`,
      type: 'text/javascript'
    }
  ],
  noscript: [
    {
      innerHTML: '<div><img src="https://mc.yandex.ru/watch/108736994" style="position:absolute; left:-9999px;" alt="" /></div>'
    }
  ]
})
</script>

<template>
  <NuxtPage />
</template>




az358@gaz358-BOD-WXX9:~/myprog/gazmaster-site/.output/public$ tar -czf /tmp/site.tar.gz
tar: Cowardly refusing to create an empty archive
Try 'tar --help' or 'tar --usage' for more information.
gaz358@gaz358-BOD-WXX9:~/myprog/gazmaster-site/.output/publi
