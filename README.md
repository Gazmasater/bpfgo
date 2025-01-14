2. Проверьте правильность установки Eunomia:
Если Eunomia еще не установлена, скачайте и установите её:

Установка Eunomia:
Перейдите на официальный репозиторий Eunomia.
Следуйте инструкциям по установке.
Альтернативный способ установки через git:
bash
Копировать код
git clone https://github.com/eunomia-bpf/eunomia-bpf.git
cd eunomia-bpf
make install
3. Добавьте настройку в профиль:
Чтобы сохранять настройку между сеансами, добавьте следующую строку в файл ~/.bashrc или ~/.zshrc:

bash
Копировать код
echo 'export EUNOMIA_HOME=/path/to/eunomia' >> ~/.bashrc
source ~/.bashrc
4. Повторный запуск программы:
После установки и настройки переменной окружения попробуйте снова запустить команду:

bash
Копировать код
./ecc tracepoint.c
Проверка корректности:
Если ошибка сохраняется, используйте переменную RUST_BACKTRACE=1 для получения детального сообщения об ошибке:

bash
Копировать код
RUST_BACKTRACE=1 ./ecc tracepoint.c
Это поможет выяснить дополнительные причины проблемы, если ошибка связана не только с переменной окружения.

az358@gaz358-BOD-WXX9:~/myprog/bpfgo/eunomia-bpf$ make install
make: *** No rule to make target 'install'.  Stop.
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/eunomia-bpf$ 

______________________________________________________________________________

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/eunomia-bpf$ ls
bpf-loader-rs       compiler         ecli          flake.lock  Makefile      third_party
codecov.yaml        CONTRIBUTING.md  eunomia-sdks  flake.nix   README.md
CODE_OF_CONDUCT.md  documents        examples      LICENSE     README_zh.md
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/eunomia-bpf$ 


sudo apt update
sudo apt install build-essential


Чтобы запустить Makefile, выполните следующие шаги:

1. Перейдите в папку с файлом Makefile:
Вы уже находитесь в нужной директории (eunomia-bpf), так что можно сразу выполнять команды.

2. Посмотрите доступные цели (targets) Makefile:
bash
Копировать код
make help
Если цель help не определена, откройте файл Makefile и посмотрите его содержимое:

bash
Копировать код
cat Makefile
3. Запуск команды make:
Запустите стандартную сборку:

bash
Копировать код
make
4. Установка проекта (если предусмотрено):
Если нужна установка:

bash
Копировать код
make install
5. Частые команды:
Очистка старой сборки:
bash
Копировать код
make clean
Удаление установленного:
bash
Копировать код
make uninstall


.PHONY: ecli bpf-loader-rs eunomia-exporter help install-deps clean all
.DEFAULT_GOAL := all
all: bpf-loader-rs ecli ## Build all binaries

define BROWSER_PYSCRIPT
import os, webbrowser, sys

try:
	from urllib import pathname2url
except:
	from urllib.request import pathname2url

webbrowser.open("file://" + pathname2url(os.path.abspath(sys.argv[1])))
endef
export BROWSER_PYSCRIPT

define PRINT_HELP_PYSCRIPT
import re, sys

for line in sys.stdin:
	match = re.match(r'^([a-zA-Z_-]+):.*?## (.*)$$', line)
	if match:
		target, help = match.groups()
		print("%-20s %s" % (target, help))
endef
export PRINT_HELP_PYSCRIPT

BROWSER := python -c "$$BROWSER_PYSCRIPT"
INSTALL_LOCATION := ~/.local

help:
	@python -c "$$PRINT_HELP_PYSCRIPT" < $(MAKEFILE_LIST)

install-deps: ## install deps
	apt update
	apt-get install libcurl4-openssl-dev libelf-dev clang llvm cmake zlib1g-dev

ecli: ## build the command line tool for eunomia-bpf
	make -C ecli build

bpf-loader-rs: ## build the core library for eunomia-bpf
	make -C bpf-loader-rs

ecc: ## build the core library for eunomia-bpf
	make -C compiler

clean: ## clean all build projects
	make -C bpf-loader-rs clean
	make -C ecli clean
	make -C examples clean
eunomia-exporter: ## build the exporter for custom metric
	make -C bpf-loader-rs
	cd eunomia-exporter && cargo build --release

XDG_DATA_HOME ?= ${HOME}/.local/share
EUNOMIA_HOME := $(XDG_DATA_HOME)/eunomia

release:
	make -C ecli install
	make -C compiler install
	cp -R $(EUNOMIA_HOME) eunomia
	tar -czvf eunomia.tar.gz eunomia
	rm -rf eunomia

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/eunomia-bpf$ sudo make
make -C bpf-loader-rs
make[1]: Entering directory '/home/gaz358/myprog/bpfgo/eunomia-bpf/bpf-loader-rs'
sudo apt-get update 
Hit:1 http://ru.archive.ubuntu.com/ubuntu noble InRelease
Hit:2 http://ru.archive.ubuntu.com/ubuntu noble-updates InRelease            
Hit:3 http://ru.archive.ubuntu.com/ubuntu noble-backports InRelease          
Hit:4 http://security.ubuntu.com/ubuntu noble-security InRelease
Reading package lists... Done
sudo apt install libelf-dev zlib1g-dev -y
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
libelf-dev is already the newest version (0.190-1.1build4).
zlib1g-dev is already the newest version (1:1.3.dfsg-3.1ubuntu2.1).
The following packages were automatically installed and are no longer required:
  amd64-microcode bpfcc-tools bpftrace ieee-data intel-microcode iucode-tool libbpfcc
  linux-headers-generic-hwe-24.04 python3-bpfcc python3-netaddr thermald ubuntu-kernel-accessories
Use 'sudo apt autoremove' to remove them.
0 upgraded, 0 newly installed, 0 to remove and 105 not upgraded.
cargo build --release
make[1]: cargo: No such file or directory
make[1]: *** [Makefile:2: build] Error 127
make[1]: Leaving directory '/home/gaz358/myprog/bpfgo/eunomia-bpf/bpf-loader-rs'
make: *** [Makefile:42: bpf-loader-rs] Error 2
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/eunomia-bpf$ 






