

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

Как загрузить eBPF-программу с BPF_SK_LOOKUP и привязать её к network namespace
1. Скомпилировать eBPF-программу
Если твоя программа находится в test.c, скомпилируй её с помощью clang:


clang -O2 -g -target bpf -c test.c -o test.o
2. Создать bpffs (если его нет)
Чтобы eBPF-программы могли загружаться, убедись, что bpffs смонтирован:

sudo mount -t bpf none /sys/fs/bpf
Если он уже смонтирован, этот шаг можно пропустить.

3. Создать новое network namespace (если нужно)
Если хочешь тестировать программу в изолированном пространстве имен сети, создай его:


sudo ip netns add my_netns
Проверить список network namespace можно так:


ip netns list
4. Загрузить eBPF-программу в ядро
Используем bpftool:


sudo bpftool prog load test.o /sys/fs/bpf/test_prog type sk_lookup
Это загружает eBPF-программу в ядро и сохраняет её в bpffs.

Проверить, загружена ли программа:


bpftool prog show
Если sk_lookup присутствует, значит загрузка прошла успешно.

5. Привязать программу к network namespace
Чтобы eBPF-программа работала, её нужно прикрепить к сетевому пространству:


sudo bpftool net attach sk_lookup id $(bpftool prog show | grep sk_lookup | awk '{print $1}') netns /var/run/netns/my_netns
Где:

$(bpftool prog show | grep sk_lookup | awk '{print $1}') — получает ID загруженной eBPF-программы.
/var/run/netns/my_netns — путь к пространству имен сети (указывается вместо target_fd).
Проверить, что программа прикреплена:


6. Проверить работу
Теперь можно выполнить команду внутри network namespace, чтобы проверить, что программа работает:


sudo ip netns exec my_netns ping -c 3 8.8.8.8
Если eBPF-программа корректно перехватывает трафик, ping может не пройти или показать задержку.










