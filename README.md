bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


Сообщение об ошибке указывает, что EUNOMIA_HOME не установлен, и программа не может найти путь к "eunomia data home". Попробуйте выполнить следующие шаги для решения проблемы:

Установите переменную окружения EUNOMIA_HOME:
Определите, где находится eunomia data, и укажите этот путь в переменной EUNOMIA_HOME. Например, в командной строке выполните:

bash
Копировать код
export EUNOMIA_HOME=/path/to/eunomia/data
Замените /path/to/eunomia/data на реальный путь.

Проверьте файл конфигурации Eunomia:
Возможно, путь к eunomia data задан в конфигурации или может быть уточнен в документации Eunomia.

Запустите с трассировкой ошибок: Если ошибка остается, включите трассировку для получения дополнительной информации:

bash
Копировать код
RUST_BACKTRACE=1 ./ecc ebpf_test.c

gaz358@gaz358-BOD-WXX9:/$ ls /home/gaz358/.local/share/eunomia
gaz358@gaz358-BOD-WXX9:/$ cd /home/gaz358/.local/share/eunomia
gaz358@gaz358-BOD-WXX9:~/.local/share/eunomia$ ls
gaz358@gaz358-BOD-WXX9:~/.local/share/eunomia$ 



