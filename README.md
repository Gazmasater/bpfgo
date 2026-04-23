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



Welcome to Ubuntu 22.04.5 LTS (GNU/Linux 5.15.0-161-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/pro

 System information as of Thu Apr 23 07:55:12 UTC 2026

  System load:  0.0                Processes:             99
  Usage of /:   16.4% of 19.20GB   Users logged in:       0
  Memory usage: 17%                IPv4 address for ens3: 176.126.99.27
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
Last login: Wed Apr 22 12:35:59 2026 from 95.26.188.15
root@calltracking-server:~# sudo apt update
Hit:1 http://archive.ubuntu.com/ubuntu jammy InRelease
Get:2 http://security.ubuntu.com/ubuntu jammy-security InRelease [129 kB]
Get:3 http://archive.ubuntu.com/ubuntu jammy-updates InRelease [128 kB]
Hit:4 http://archive.ubuntu.com/ubuntu jammy-backports InRelease    
Get:5 http://security.ubuntu.com/ubuntu jammy-security/main amd64 Packages [3171 kB]
Get:6 http://archive.ubuntu.com/ubuntu jammy-updates/main amd64 Packages [3436 kB]
Get:7 http://archive.ubuntu.com/ubuntu jammy-updates/universe amd64 Packages [1268 kB]
Fetched 8131 kB in 2s (5206 kB/s)                        
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
All packages are up to date.
root@calltracking-server:~# sudo apt install certbot python3-certbot-nginx -y
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following additional packages will be installed:
  python3-acme python3-certbot python3-configargparse python3-icu python3-josepy python3-parsedatetime python3-requests-toolbelt
  python3-rfc3339 python3-zope.component python3-zope.event python3-zope.hookable
Suggested packages:
  python-certbot-doc python3-certbot-apache python-acme-doc python-certbot-nginx-doc
The following NEW packages will be installed:
  certbot python3-acme python3-certbot python3-certbot-nginx python3-configargparse python3-icu python3-josepy python3-parsedatetime
  python3-requests-toolbelt python3-rfc3339 python3-zope.component python3-zope.event python3-zope.hookable
0 upgraded, 13 newly installed, 0 to remove and 0 not upgraded.
Need to get 993 kB of archives.
After this operation, 5077 kB of additional disk space will be used.
Get:1 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-josepy all 1.10.0-1 [22.0 kB]
Get:2 http://archive.ubuntu.com/ubuntu jammy/main amd64 python3-requests-toolbelt all 0.9.1-1 [38.0 kB]
Get:3 http://archive.ubuntu.com/ubuntu jammy/main amd64 python3-rfc3339 all 1.1-3 [7110 B]
Get:4 http://archive.ubuntu.com/ubuntu jammy-updates/universe amd64 python3-acme all 1.21.0-1ubuntu0.1 [36.4 kB]
Get:5 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-configargparse all 1.5.3-1 [26.9 kB]
Get:6 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-parsedatetime all 2.6-2 [32.9 kB]
Get:7 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-zope.hookable amd64 5.1.0-1build1 [11.6 kB]
Get:8 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-zope.event all 4.4-3 [8180 B]
Get:9 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-zope.component all 4.3.0-3 [38.3 kB]
Get:10 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-certbot all 1.21.0-1build1 [175 kB]
Get:11 http://archive.ubuntu.com/ubuntu jammy/universe amd64 certbot all 1.21.0-1build1 [21.3 kB]
Get:12 http://archive.ubuntu.com/ubuntu jammy/universe amd64 python3-certbot-nginx all 1.21.0-1 [35.4 kB]
Get:13 http://archive.ubuntu.com/ubuntu jammy/main amd64 python3-icu amd64 2.8.1-0ubuntu2 [540 kB]
Fetched 993 kB in 0s (3967 kB/s)  
Preconfiguring packages ...
Selecting previously unselected package python3-josepy.
(Reading database ... 114826 files and directories currently installed.)
Preparing to unpack .../00-python3-josepy_1.10.0-1_all.deb ...
Unpacking python3-josepy (1.10.0-1) ...
Selecting previously unselected package python3-requests-toolbelt.
Preparing to unpack .../01-python3-requests-toolbelt_0.9.1-1_all.deb ...
Unpacking python3-requests-toolbelt (0.9.1-1) ...
Selecting previously unselected package python3-rfc3339.
Preparing to unpack .../02-python3-rfc3339_1.1-3_all.deb ...
Unpacking python3-rfc3339 (1.1-3) ...
Selecting previously unselected package python3-acme.
Preparing to unpack .../03-python3-acme_1.21.0-1ubuntu0.1_all.deb ...
Unpacking python3-acme (1.21.0-1ubuntu0.1) ...
Selecting previously unselected package python3-configargparse.
Preparing to unpack .../04-python3-configargparse_1.5.3-1_all.deb ...
Unpacking python3-configargparse (1.5.3-1) ...
Selecting previously unselected package python3-parsedatetime.
Preparing to unpack .../05-python3-parsedatetime_2.6-2_all.deb ...
Unpacking python3-parsedatetime (2.6-2) ...
Selecting previously unselected package python3-zope.hookable.
Preparing to unpack .../06-python3-zope.hookable_5.1.0-1build1_amd64.deb ...
Unpacking python3-zope.hookable (5.1.0-1build1) ...
Selecting previously unselected package python3-zope.event.
Preparing to unpack .../07-python3-zope.event_4.4-3_all.deb ...
Unpacking python3-zope.event (4.4-3) ...
Selecting previously unselected package python3-zope.component.
Preparing to unpack .../08-python3-zope.component_4.3.0-3_all.deb ...
Unpacking python3-zope.component (4.3.0-3) ...
Selecting previously unselected package python3-certbot.
Preparing to unpack .../09-python3-certbot_1.21.0-1build1_all.deb ...
Unpacking python3-certbot (1.21.0-1build1) ...
Selecting previously unselected package certbot.
Preparing to unpack .../10-certbot_1.21.0-1build1_all.deb ...
Unpacking certbot (1.21.0-1build1) ...
Selecting previously unselected package python3-certbot-nginx.
Preparing to unpack .../11-python3-certbot-nginx_1.21.0-1_all.deb ...
Unpacking python3-certbot-nginx (1.21.0-1) ...
Selecting previously unselected package python3-icu.
Preparing to unpack .../12-python3-icu_2.8.1-0ubuntu2_amd64.deb ...
Unpacking python3-icu (2.8.1-0ubuntu2) ...
Setting up python3-configargparse (1.5.3-1) ...
Setting up python3-requests-toolbelt (0.9.1-1) ...
Setting up python3-parsedatetime (2.6-2) ...
Setting up python3-icu (2.8.1-0ubuntu2) ...
Setting up python3-zope.event (4.4-3) ...
Setting up python3-zope.hookable (5.1.0-1build1) ...
Setting up python3-josepy (1.10.0-1) ...
Setting up python3-rfc3339 (1.1-3) ...
Setting up python3-zope.component (4.3.0-3) ...
Setting up python3-acme (1.21.0-1ubuntu0.1) ...
Setting up python3-certbot (1.21.0-1build1) ...
Setting up certbot (1.21.0-1build1) ...
Created symlink /etc/systemd/system/timers.target.wants/certbot.timer → /lib/systemd/system/certbot.timer.
Setting up python3-certbot-nginx (1.21.0-1) ...
Processing triggers for man-db (2.10.2-1) ...
Scanning processes...                                                                                                                       
Scanning candidates...                                                                                                                      
Scanning linux images...                                                                                                                    

Restarting services...
Service restarts being deferred:
 /etc/needrestart/restart.d/dbus.service
 systemctl restart getty@tty1.service
 systemctl restart networkd-dispatcher.service
 systemctl restart systemd-logind.service
 systemctl restart unattended-upgrades.service

No containers need to be restarted.

No user sessions are running outdated binaries.

No VM guests are running outdated hypervisor (qemu) binaries on this host.
root@calltracking-server:~# sudo certbot --nginx -d remontkotlov48.ru -d www.remontkotlov48.ru
Saving debug log to /var/log/letsencrypt/letsencrypt.log
Enter email address (used for urgent renewal and security notices)
 (Enter 'c' to cancel): tolkoplus@gmail.com

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Please read the Terms of Service at
https://letsencrypt.org/documents/LE-SA-v1.6-August-18-2025.pdf. You must agree
in order to register with the ACME server. Do you agree?
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
(Y)es/(N)o: Yes

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Would you be willing, once your first certificate is successfully issued, to
share your email address with the Electronic Frontier Foundation, a founding
partner of the Let's Encrypt project and the non-profit organization that
develops Certbot? We'd like to send you email about our work encrypting the web,
EFF news, campaigns, and ways to support digital freedom.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
(Y)es/(N)o: Yes
Account registered.
Requesting a certificate for remontkotlov48.ru and www.remontkotlov48.ru

Successfully received certificate.
Certificate is saved at: /etc/letsencrypt/live/remontkotlov48.ru/fullchain.pem
Key is saved at:         /etc/letsencrypt/live/remontkotlov48.ru/privkey.pem
This certificate expires on 2026-07-22.
These files will be updated when the certificate renews.
Certbot has set up a scheduled task to automatically renew this certificate in the background.

Deploying certificate
Could not install certificate

NEXT STEPS:
- The certificate was saved, but could not be installed (installer: nginx). After fixing the error shown below, try installing it again by running:
  certbot install --cert-name remontkotlov48.ru

Could not automatically find a matching server block for remontkotlov48.ru. Set the `server_name` directive to use the Nginx installer.
Ask for help or search for solutions at https://community.letsencrypt.org. See the logfile /var/log/letsencrypt/letsencrypt.log or re-run Certbot with -v for more details.
root@calltracking-server:~# 
