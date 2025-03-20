lsns -t net


sudo ip netns list



nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


1. Проверяем, загружена ли программа sk_lookup
Выполни в терминале:


bpftool prog show
Если программа загружена, ты увидишь что-то вроде:


ID   NAME         TYPE       TAG               GPR   REFS  MEM  LOCK
42   sk_lookup    sk_lookup  c2d9a4ff33bc5...  0     1     2kB  2kB
👉 Если программы нет — она не загружается, проверь ошибки при loadBpfObjects(&objs, nil).

2. Проверяем, привязана ли программа к netns
Проверяем список сетевых неймспейсов и прикреплённых программ:


bpftool net list
Если программа sk_lookup не отображается здесь, значит она не привязана к netns.

Попробуй вручную привязать eBPF к netns:
Получи ID программы:

bpftool prog show
Получи ID netns:

lsns -t net
Привяжи программу:

bpftool prog attach ID_HERE netns ID_HERE
(замени ID_HERE на реальные ID)
После этого попробуй снова cat /sys/kernel/debug/tracing/trace_pipe и отправь трафик.

3. Убедись, что sk_lookup вообще может сработать
Программа sk_lookup срабатывает только при поиске сокета, значит в netns должен быть активный сокет.

Попробуй открыть сервер в netns:


ip netns exec myns nc -l -p 1234
Затем подключись к нему:


nc 127.0.0.1 1234
Если sk_lookup подключён правильно, он должен сработать.

4. Проверяем trace_pipe
Запусти в отдельном терминале:


cat /sys/kernel/debug/tracing/trace_pipe
Затем искуственно вызови sk_lookup, например, отправив HTTP-запрос внутри netns:


ip netns exec myns curl http://127.0.0.1:1234
Если программа работает, bpf_printk появится.

Вывод
Если bpf_printk всё ещё молчит:

Проверь bpftool prog show — загружена ли программа?
Проверь bpftool net list — привязана ли к netns?
Создай активный сокет в netns, иначе sk_lookup не сработает.
Проверь trace_pipe во время запроса.


ls -l /proc/<PID>/ns/net

package main

import (
	"fmt"
	"log"
	"os"
	"syscall"
	"bufio"
)

func main() {
	// Создание нового сетевого неймспейса
	if err := syscall.Unshare(syscall.CLONE_NEWNET); err != nil {
		log.Fatalf("Ошибка создания нового network namespace: %v", err)
	}
	fmt.Println("Создано новое сетевое пространство")

	// Чтение символической ссылки, указывающей на неймспейс
	nsLink, err := os.Readlink("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("Ошибка чтения символической ссылки: %v", err)
	}

	// Выводим идентификатор неймспейса (после `net:`)
	fmt.Printf("Идентификатор нового сетевого неймспейса: %s\n", nsLink)
}





















