
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



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo dmesg | tail -n 20
[    7.455864] wlp0s20f3: authenticated
[    7.456233] wlp0s20f3: associate with 0c:73:29:b7:d6:ed (try 1/3)
[    7.458395] wlp0s20f3: RX AssocResp from 0c:73:29:b7:d6:ed (capab=0x1031 status=0 aid=3)
[    7.468792] wlp0s20f3: associated
[   13.612284] systemd-journald[312]: /var/log/journal/1326c6779edf4f3ea9509f461f818711/user-1000.journal: Journal file uses a different sequence number ID, rotating.
[   13.695036] kauditd_printk_skb: 152 callbacks suppressed
[   13.695047] audit: type=1400 audit(1742165156.242:163): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23771/usr/lib/snapd/snap-confine" pid=1921 comm="snap-confine" capability=12  capname="net_admin"
[   13.695050] audit: type=1400 audit(1742165156.242:164): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23771/usr/lib/snapd/snap-confine" pid=1921 comm="snap-confine" capability=38  capname="perfmon"
[   13.948965] rfkill: input handler enabled
[   14.760577] rfkill: input handler disabled
[   14.875586] FAT-fs (sda1): Volume was not properly unmounted. Some data may be corrupt. Please run fsck.
[   16.725090] audit: type=1400 audit(1742165159.272:165): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23771/usr/lib/snapd/snap-confine" pid=2820 comm="snap-confine" capability=12  capname="net_admin"
[   16.725102] audit: type=1400 audit(1742165159.272:166): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23771/usr/lib/snapd/snap-confine" pid=2820 comm="snap-confine" capability=38  capname="perfmon"
[   28.241185] warning: `ThreadPoolForeg' uses wireless extensions which will stop working for Wi-Fi 7 hardware; use nl80211
[   93.756813] audit: type=1400 audit(1742165236.280:167): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23771/usr/lib/snapd/snap-confine" pid=4300 comm="snap-confine" capability=4  capname="fsetid"
[   93.807433] audit: type=1400 audit(1742165236.331:168): apparmor="DENIED" operation="open" class="file" profile="snap-update-ns.firefox" name="/usr/local/share/" pid=4332 comm="5" requested_mask="r" denied_mask="r" fsuid=0 ouid=0
[   94.740318] audit: type=1107 audit(1742165237.264:169): pid=918 uid=101 auid=4294967295 ses=4294967295 subj=unconfined msg='apparmor="DENIED" operation="dbus_method_call"  bus="system" path="/org/freedesktop/timedate1" interface="org.freedesktop.DBus.Properties" member="GetAll" mask="send" name=":1.111" pid=4300 label="snap.firefox.firefox" peer_pid=4449 peer_label="unconfined"
                exe="/usr/bin/dbus-daemon" sauid=101 hostname=? addr=? terminal=?'
[   94.742104] audit: type=1107 audit(1742165237.265:170): pid=918 uid=101 auid=4294967295 ses=4294967295 subj=unconfined msg='apparmor="DENIED" operation="dbus_method_call"  bus="system" path="/org/freedesktop/timedate1" interface="org.freedesktop.DBus.Properties" member="GetAll" mask="send" name=":1.111" pid=4300 label="snap.firefox.firefox" peer_pid=4449 peer_label="unconfined"
                exe="/usr/bin/dbus-daemon" sauid=101 hostname=? addr=? terminal=?'
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 











