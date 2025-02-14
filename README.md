gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go trace bpf/trace.c -- -target bpf
Compiled /home/gaz358/myprog/bpfgo/trace_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/trace_bpfel.o
Error: can't write /home/gaz358/myprog/bpfgo/trace_bpfel.go: can't generate types: template: common:17:4: executing "common" at <$.TypeDeclaration>: error calling TypeDeclaration: Struct:"conn_info_t": field 0: type *btf.Pointer: not supported
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 






Ошибка missing package, you should either set the go-package flag or the GOPACKAGE env означает, что для успешной генерации файла с кодом Go необходимо указать пакет Go, в котором будет размещен сгенерированный код.

Чтобы исправить эту ошибку, можно выполнить следующие шаги:

Использование флага -go-package:

При использовании bpf2go для генерации Go-кода необходимо явно указать имя Go-пакета с помощью флага -go-package. Например:

bash
Копировать
Редактировать
bpf2go trace bpf/trace.c -- -target bpf -go-package mypkg
В этом примере mypkg — это имя пакета, в который будет размещен сгенерированный код Go. Убедитесь, что вы выбрали имя, которое подходит для вашего проекта.

Установка переменной окружения GOPACKAGE:

В качестве альтернативы можно установить переменную окружения GOPACKAGE, указав имя пакета. Например:

bash
Копировать
Редактировать
export GOPACKAGE=mypkg
bpf2go trace bpf/trace.c -- -target bpf
В обоих случаях сгенерированный код будет помещен в пакет с именем mypkg.

Проверьте, что исходный файл trace.c существует:

Убедитесь, что файл bpf/trace.c существует в вашем проекте, и путь к нему указан правильно.

После выполнения этих шагов ошибка должна исчезнуть, и вы сможете сгенерировать Go-код для вашего BPF-программы.















nano ~/.bashrc   # для bash

export PATH=$PATH:$HOME/go/bin


bpf2go --version

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ go install github.com/cilium/ebpf/cmd/bpf2go@latest
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go --version
bpf2go: command not found


bpf2go trace ebpf/trace.c -- -target bpf




1. Установка bpf2go
Если у тебя ещё нет bpf2go, установи его:

sh
Копировать
Редактировать
go install github.com/cilium/ebpf/cmd/bpf2go@latest
2. Создание структуры проекта
Допустим, у тебя проект bpfgo, создадим его структуру:

sh
Копировать
Редактировать
bpfgo/
├── bpf/
│   ├── trace.c  # eBPF-программа (твой код)
├── main.go      # Go-программа, загружающая eBPF
3. Добавление eBPF-кода (bpf/trace.c)
Используем твой код trace.c без изменений, но убедимся, что он компилируется в bpf2go.

Сохрани его в bpf/trace.c.

4. Генерация eBPF-кода для Go
Запускаем bpf2go, чтобы сгенерировать Go-код:

sh
Копировать
Редактировать
cd bpfgo
bpf2go -target native bpf ./bpf/trace.c -- -I/usr/include
Это создаст файлы:

bpf_bpfel.go (если little-endian)
bpf_bpfeb.go (если big-endian)
bpf_bpf.o (скомпилированный eBPF)
5. Написание Go-кода (main.go)
Создадим main.go, который загружает eBPF-программу:

go
Копировать
Редактировать
package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
)

//go:generate bpf2go -target native bpf ./bpf/trace.c -- -I/usr/include

type ConnInfo struct {
	Pid    uint32
	SrcIP  uint32
	DstIP  uint32
	Sport  uint16
	Dport  uint16
	Comm   [64]byte
}

func main() {
	// Загружаем eBPF-программу
	objs := bpfObjects{}
	if err := loadBpfObjects(&objs, nil); err != nil {
		log.Fatalf("Ошибка загрузки BPF: %v", err)
	}
	defer objs.Close()

	// Открываем канал событий
	rd, err := perf.NewReader(objs.TraceEvents, 4096)
	if err != nil {
		log.Fatalf("Ошибка создания перф-ридера: %v", err)
	}
	defer rd.Close()

	log.Println("eBPF-программа успешно загружена! Ожидание событий...")

	// Обработка SIGINT (Ctrl+C)
	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGTERM)

	// Читаем события в цикле
	go func() {
		for {
			record, err := rd.Read()
			if err != nil {
				log.Printf("Ошибка чтения события: %v", err)
				continue
			}
			if record.LostSamples > 0 {
				log.Printf("Потеряно %d событий", record.LostSamples)
				continue
			}

			var info ConnInfo
			if err := binary.Read(bytes.NewBuffer(record.RawSample), binary.LittleEndian, &info); err != nil {
				log.Printf("Ошибка парсинга данных: %v", err)
				continue
			}

			fmt.Printf("PID: %d, SrcIP: %d, DstIP: %d, Sport: %d, Dport: %d, Comm: %s\n",
				info.Pid, info.SrcIP, info.DstIP, info.Sport, info.Dport, info.Comm)
		}
	}()

	<-stop
	log.Println("Завершаем работу.")
}
6. Сборка и запуск
Компилируем Go-программу:

sh
Копировать
Редактировать
go build -o bpfgo main.go
Запускаем:

sh
Копировать
Редактировать
sudo ./bpfgo
7. Проверка работы
Теперь, когда в системе происходят события sendto и recvfrom, они будут перехватываться eBPF-программой и выводиться в терминал.

Итог:
eBPF-код (trace.c) компилируется с помощью bpf2go.
В main.go загружается eBPF-программа, открывается perf event.
Данные передаются из eBPF в Go и выводятся в консоль.
Если будут ошибки, можешь запустить с RUST_BACKTRACE=1 и проверить, всё ли нормально с компиляцией bpf2go. 🚀


gaz358@gaz358-BOD-WXX9:~/Downloads$ ls
ecc  go1.24.0.linux-amd64.tar.gz


1. Удаление старой версии Go (если есть)
bash
Копировать
Редактировать
sudo rm -rf /usr/local/go
2. Распаковка архива в /usr/local
bash
Копировать
Редактировать
sudo tar -C /usr/local -xzf go1.24.0.linux-amd64.tar.gz
3. Добавление Go в PATH
Добавь в ~/.bashrc (или ~/.profile, если используешь другой шелл) строку:

bash
Копировать
Редактировать
export PATH=$PATH:/usr/local/go/bin
Затем примени изменения:

bash
Копировать
Редактировать
source ~/.bashrc

Чтобы запомнить изменения в nano, делай так:

Открой файл (например, ~/.bashrc):
bash
Копировать
Редактировать
nano ~/.bashrc
Добавь строку:
bash
Копировать
Редактировать
export PATH=$PATH:/usr/local/go/bin
Сохранение:
Нажми Ctrl + X (выход).
Нажми Y (сохранить).
Нажми Enter (подтвердить имя файла).
Применение изменений:
bash
Копировать
Редактировать
source ~/.bashrc







