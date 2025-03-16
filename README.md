
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

package main

import (
	"fmt"
	"log"
	"os"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/link"
)

func main() {
	// Открываем файловый дескриптор для сетевого пространства имен
	targetFD, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("failed to open network namespace: %v", err)
	}
	defer targetFD.Close()
	fmt.Println("Opened network namespace successfully.")

	// Проверяем, загружена ли программа BPF
	if objs.EchoDispatch == nil {
		log.Fatal("BPF program EchoDispatch is nil")
	}

	// Получаем дескриптор программы
	progFD := objs.EchoDispatch.FD()
	fmt.Printf("Program FD: %d\n", progFD)
	if progFD <= 0 {
		log.Fatalf("invalid program FD: %d", progFD)
	}

	// Проверяем тип загруженной программы BPF
	info, err := objs.EchoDispatch.Info()
	if err != nil {
		log.Fatalf("failed to get program info: %v", err)
	}
	fmt.Printf("Program Type: %v\n", info.Type)

	// Привязываем программу BPF к пространству имен сети
	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  int(targetFD.Fd()),
		Program: objs.EchoDispatch,
		Attach:  ebpf.AttachSkLookup,
	})

	if err != nil {
		log.Fatalf("failed to attach program: %v", err)
	}

	fmt.Println("BPF program attached successfully.")
}



1. Ядро не поддерживает BPF_SK_LOOKUP
Проверим, поддерживается ли sk_lookup в системе:


grep CONFIG_BPF_SK_LOOKUP /boot/config-$(uname -r)
Если вернется # CONFIG_BPF_SK_LOOKUP is not set, значит, эта возможность в ядре отсутствует. В таком случае нужно либо обновить ядро, либо пересобрать его с этой опцией.

Также попробуй проверить версию ядра:


uname -r
Для BPF_SK_LOOKUP нужно ядро 5.9+, но лучше 5.11+, так как там исправлены баги.

2. Попробовать dmesg для уточнения ошибки
После ошибки запусти:

dmesg | tail -n 20
Если там что-то вроде bpf: unknown attach type, значит, ядро не поддерживает BPF_SK_LOOKUP.

3. Попробовать unshare -n
Некоторые версии ядра не позволяют sk_lookup работать в глобальном пространстве имен. Запусти:


sudo unshare -n ./bpfgo
Если в этом режиме программа заработает, значит, проблема в политике безопасности ядра.

4. Попробовать setcap вместо sudo
Запуск через sudo может менять некоторые контексты, так что попробуй:

sudo setcap cap_net_admin+ep ./bpfgo
./bpfgo
Если работает, проблема была в правах.

5. Попробовать другой Target
Вместо targetFD.Fd(), попробуй:


err = link.RawAttachProgram(link.RawAttachProgramOptions{
	Target:  0, // Используем 0 как альтернативный способ привязки
	Program: objs.EchoDispatch,
	Attach:  ebpf.AttachSkLookup,
})
Если заработает, значит, проблема была в файловом дескрипторе.

Что делать дальше?
Запусти dmesg | tail -n 20 после ошибки и покажи, что там.











