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




