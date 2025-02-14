gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo dmesg | tail -n 50
[    4.023534] iwlwifi 0000:00:14.3: Registered PHC clock: iwlwifi-PTP, with index: 0
[    4.550255] Bluetooth: hci0: Waiting for firmware download to complete
[    4.551144] Bluetooth: hci0: Firmware loaded in 1706705 usecs
[    4.551373] Bluetooth: hci0: Waiting for device to boot
[    4.556356] loop19: detected capacity change from 0 to 8
[    4.566159] Bluetooth: hci0: Device booted in 14633 usecs
[    4.566169] Bluetooth: hci0: Malformed MSFT vendor event: 0x02
[    4.566406] Bluetooth: hci0: Found Intel DDC parameters: intel/ibt-19-0-4.ddc
[    4.568250] Bluetooth: hci0: Applying Intel DDC parameters completed
[    4.569297] Bluetooth: hci0: Firmware revision 0.4 build 249 week 27 2023
[    4.571179] Bluetooth: hci0: HCI LE Coded PHY feature bit is set, but its usage is not supported.
[    4.636275] Bluetooth: MGMT ver 1.22
[    4.639043] NET: Registered PF_ALG protocol family
[    4.681804] Bluetooth: RFCOMM TTY layer initialized
[    4.681823] Bluetooth: RFCOMM socket layer initialized
[    4.681835] Bluetooth: RFCOMM ver 1.11
[    5.895712] rfkill: input handler disabled
[    7.565394] wlp0s20f3: authenticate with 0c:73:29:b7:d6:ed (local address=e4:fd:45:a8:ec:0b)
[    7.566459] wlp0s20f3: send auth to 0c:73:29:b7:d6:ed (try 1/3)
[    7.595848] wlp0s20f3: authenticated
[    7.597242] wlp0s20f3: associate with 0c:73:29:b7:d6:ed (try 1/3)
[    7.600428] wlp0s20f3: RX AssocResp from 0c:73:29:b7:d6:ed (capab=0x1031 status=0 aid=3)
[    7.609856] wlp0s20f3: associated
[   26.054064] systemd-journald[313]: /var/log/journal/1326c6779edf4f3ea9509f461f818711/user-1000.journal: Journal file uses a different sequence number ID, rotating.
[   26.209594] kauditd_printk_skb: 152 callbacks suppressed
[   26.209604] audit: type=1400 audit(1739559760.753:163): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23545/usr/lib/snapd/snap-confine" pid=1915 comm="snap-confine" capability=12  capname="net_admin"
[   26.209621] audit: type=1400 audit(1739559760.753:164): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23545/usr/lib/snapd/snap-confine" pid=1915 comm="snap-confine" capability=38  capname="perfmon"
[   26.377795] rfkill: input handler enabled
[   27.485383] rfkill: input handler disabled
[   27.713136] FAT-fs (sda1): Volume was not properly unmounted. Some data may be corrupt. Please run fsck.
[   34.921110] audit: type=1400 audit(1739559769.524:165): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/23545/usr/lib/snapd/snap-confine" pid=2834 comm="snap-confine" capability=4  capname="fsetid"
[   35.037168] audit: type=1400 audit(1739559769.646:166): apparmor="DENIED" operation="open" class="file" profile="snap-update-ns.firefox" name="/usr/local/share/" pid=2864 comm="5" requested_mask="r" denied_mask="r" fsuid=0 ouid=0
[   36.866861] audit: type=1107 audit(1739559771.534:167): pid=935 uid=101 auid=4294967295 ses=4294967295 subj=unconfined msg='apparmor="DENIED" operation="dbus_method_call"  bus="system" path="/org/freedesktop/timedate1" interface="org.freedesktop.DBus.Properties" member="GetAll" mask="send" name=":1.104" pid=2834 label="snap.firefox.firefox" peer_pid=3039 peer_label="unconfined"
                exe="/usr/bin/dbus-daemon" sauid=101 hostname=? addr=? terminal=?'
[   36.886991] audit: type=1107 audit(1739559771.555:168): pid=935 uid=101 auid=4294967295 ses=4294967295 subj=unconfined msg='apparmor="DENIED" operation="dbus_method_call"  bus="system" path="/org/freedesktop/timedate1" interface="org.freedesktop.DBus.Properties" member="GetAll" mask="send" name=":1.104" pid=2834 label="snap.firefox.firefox" peer_pid=3039 peer_label="unconfined"
                exe="/usr/bin/dbus-daemon" sauid=101 hostname=? addr=? terminal=?'
[   98.440281] audit: type=1326 audit(1739559833.317:169): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.440289] audit: type=1326 audit(1739559833.317:170): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=141 compat=0 ip=0x7a88880b378b code=0x50000
[   98.440409] audit: type=1326 audit(1739559833.317:171): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.440437] audit: type=1326 audit(1739559833.317:172): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.440470] audit: type=1326 audit(1739559833.317:174): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.440474] audit: type=1326 audit(1739559833.317:173): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.440481] audit: type=1326 audit(1739559833.317:175): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.440508] audit: type=1326 audit(1739559833.317:176): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=203 compat=0 ip=0x7a8888034531 code=0x50000
[   98.444840] audit: type=1326 audit(1739559833.321:177): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=141 compat=0 ip=0x7a88880b378b code=0x50000
[   98.445459] audit: type=1326 audit(1739559833.322:178): auid=1000 uid=1000 gid=1000 ses=3 subj=snap.snap-store.snap-store pid=3885 comm="snap-store" exe="/snap/snap-store/1248/bin/snap-store" sig=0 arch=c000003e syscall=141 compat=0 ip=0x7a88880b378b code=0x50000
[  108.024195] warning: `ThreadPoolForeg' uses wireless extensions which will stop working for Wi-Fi 7 hardware; use nl80211
[  305.208464] kauditd_printk_skb: 6 callbacks suppressed
[  305.208468] audit: type=1107 audit(1739560040.097:185): pid=935 uid=101 auid=4294967295 ses=4294967295 subj=unconfined msg='apparmor="DENIED" operation="dbus_method_call"  bus="system" path="/org/freedesktop/timedate1" interface="org.freedesktop.DBus.Properties" member="GetAll" mask="send" name=":1.120" pid=2834 label="snap.firefox.firefox" peer_pid=4950 peer_label="unconfined"
                exe="/usr/bin/dbus-daemon" sauid=101 hostname=? addr=? terminal=?'
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 








sudo bpftool prog load bpf_prog.o /sys/fs/bpf/prog
sudo dmesg | tail -n 50







dmesg | tail -n 20 



package main

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"os"
	"os/signal"
	"reflect"

	"github.com/cilium/ebpf"
	"github.com/cilium/ebpf/perf"
)

// Event структура должна совпадать с event_t в BPF-коде
type Event struct {
	PID   uint32
	SrcIP uint32
	Sport uint16
	Comm  [16]byte
}

func loadTraceObject(objs interface{}) error {
	// Загружаем скомпилированную BPF-программу
	spec, err := ebpf.LoadCollectionSpec("trace.bpf.o") 
	if err != nil {
		return fmt.Errorf("loading BPF program: %v", err)
	}

	// Создаем коллекцию объектов из BPF-спецификации
	coll, err := ebpf.NewCollection(spec)
	if err != nil {
		return fmt.Errorf("creating BPF collection: %v", err)
	}

	// Привязываем карту trace_events
	traceEventsMap, found := coll.Maps["trace_events"]
	if !found {
		return fmt.Errorf("trace_events map not found in BPF program")
	}

	// Записываем карту в структуру
	v := reflect.ValueOf(objs).Elem()
	v.FieldByName("TraceEvents").Set(reflect.ValueOf(traceEventsMap))

	return nil
}

func main() {
	// Загружаем скомпилированную BPF-программу
	objs := struct {
		TraceEvents *ebpf.Map
	}{}

	if err := loadTraceObject(&objs); err != nil {
		log.Fatalf("loading BPF objects: %v", err)
	}
	defer objs.TraceEvents.Close()

	// Открываем канал для чтения perf-событий
	reader, err := perf.NewReader(objs.TraceEvents, 4096)
	if err != nil {
		log.Fatalf("creating perf event reader: %v", err)
	}
	defer reader.Close()

	fmt.Println("Listening for UDP sys_exit_sendto events...")

	// Обработка прерываний (Ctrl+C)
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt)

	go func() {
		for {
			record, err := reader.Read()
			if err != nil {
				log.Printf("reading perf event: %v", err)
				continue
			}

			// Декодируем данные
			var event Event
			if err := binary.Read(bytes.NewBuffer(record.RawSample), binary.LittleEndian, &event); err != nil {
				log.Printf("parsing perf event: %v", err)
				continue
			}

			// Вывод в консоль
			fmt.Printf("UDP sys_exit_sendto: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				event.PID, bytes.Trim(event.Comm[:], "\x00"),
				(event.SrcIP>>24)&0xFF, (event.SrcIP>>16)&0xFF, (event.SrcIP>>8)&0xFF, event.SrcIP&0xFF,
				event.Sport)
		}
	}()

	<-sigChan
	fmt.Println("\nExiting...")
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ go run .
2025/02/14 15:45:13 loading BPF objects: creating BPF collection: map conn_info_map: map create: operation not permitted (MEMLOCK may be too low, consider rlimit.RemoveMemlock)
exit status 1
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 







bpf2go -target native -pkg main trace bpf/trace.c -- -I/usr/include/


GOPACKAGE=main bpf2go -target native -go-package main trace bpf/trace.c -- -I/usr/include/


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ GOPACKAGE=main bpf2go -target native -go-package main trace bpf/trace.c -- -I/usr/include/
Compiled /home/gaz358/myprog/bpfgo/trace_x86_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/trace_x86_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/trace_x86_bpfel.go
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 



SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    
    if (ctx->addr) {
        bpf_probe_read(&conn_info.sock_addr, sizeof(conn_info.sock_addr), ctx->addr);
    }
    
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        return 0;
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)&conn_info->sock_addr;
    if (addr->sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr->sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr->sin_port);
    }

    struct event_t event = {
        .pid = conn_info->pid,
        .src_ip = conn_info->src_ip,
        .sport = conn_info->sport,
    };

    __builtin_memcpy(event.comm, conn_info->comm, sizeof(event.comm));
    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &event, sizeof(event));

    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}












//go:build ignore

#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

struct conn_info_t
{
    struct sockaddr *sock_addr;
    u32 pid;
    u32 src_ip;
    u32 dst_ip;
    u32 addrlen;
    u16 sport;
    u16 dport;
    char comm[64];
};

struct event_t {
    u32 pid;
    u32 src_ip;
    u16 sport;
    u16 pad;
    char comm[64];
};

struct sys_enter_sendto_args
{
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
    int pad1;
    void *buff;
    size_t len;
    unsigned int flags;
    struct sockaddr *addr;
    int addr_len;
};

struct sys_exit_sendto_args
{
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    long ret;
};

struct sys_enter_recvfrom_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    int fd;
    void *ubuf;
    size_t size;
    unsigned int flags;
    struct sockaddr *addr;
    int *addr_len;
};

struct sys_exit_recvfrom_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int pad1;
    int ret;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");




struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); 
} trace_events SEC(".maps");


char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    conn_info.sock_addr = ctx->addr;

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
    {
        return 0;
    }

    struct sockaddr_in addr = {};
    if (conn_info->sock_addr)
    {
        bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr);
        if (addr.sin_family == AF_INET)
        {
            conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info->sport = bpf_ntohs(addr.sin_port);
        }
    }

    struct event_t event = {
        .pid = conn_info->pid,
        .src_ip = conn_info->src_ip,
        .sport = conn_info->sport,
    };

    __builtin_memcpy(event.comm, conn_info->comm, sizeof(event.comm));




    bpf_printk("UDP sys_exit_sendto: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
        conn_info->pid, conn_info->comm,
        (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
        (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);


    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, event, sizeof(event));



    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom_enter(struct sys_enter_recvfrom_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    conn_info.sock_addr = ctx->addr;

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info)
    {
        return 0;
    }

    struct sockaddr_in addr = {};
    if (conn_info->sock_addr)
    {
        bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr);
        if (addr.sin_family == AF_INET)
        {
            conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info->sport = bpf_ntohs(addr.sin_port);
        }
    }


    bpf_printk("UDP sys_exit_recvfrom: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
        conn_info->pid, conn_info->comm,
        (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
        (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

    //bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, conn_info, sizeof(*conn_info));



    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}





bpf2go — это утилита из пакета github.com/cilium/ebpf/cmd/bpf2go, которая компилирует eBPF-программы в Go-код. Она позволяет встраивать скомпилированный eBPF-байт-код в Go-приложение.

Шаги для генерации Go-файла с помощью bpf2go
1. Установите bpf2go (если не установлен)
sh
Копировать
Редактировать
go install github.com/cilium/ebpf/cmd/bpf2go@latest
2. Скомпилируйте eBPF-программу в Go-код
Предположим, ваш eBPF-код находится в файле trace.c. Выполните команду:

sh
Копировать
Редактировать
bpf2go -target native trace trace.c -- -I/usr/include/
✅ Это создаст два файла:

trace_bpfel.go (или trace_bpfeb.go, в зависимости от архитектуры)
trace_bpfel.o (скомпилированный eBPF-байт-код)
3. Используйте сгенерированный код в Go
Создайте main.go и добавьте следующий код:

go
Копировать
Редактировать
package main

import (
	"fmt"
	"log"

	"github.com/cilium/ebpf"
)

func main() {
	// Загрузим скомпилированную eBPF-программу
	objs := traceObjects{}
	if err := loadTraceObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load BPF objects: %v", err)
	}
	defer objs.Close()

	fmt.Println("eBPF программа загружена!")
}
4. Скомпилируйте и запустите Go-программу
sh
Копировать
Редактировать
go run main.go
Теперь Go-код использует скомпилированную eBPF-программу! 🚀








Вы сказали:
trace.c в папке bpf
ChatGPT сказал:
Если ваш trace.c находится в папке bpf, то запустите bpf2go с учетом пути:

sh
Копировать
Редактировать
bpf2go -target native trace bpf/trace.c -- -I/usr/include/
Что произойдет после выполнения этой команды?
Будут созданы файлы в текущей директории:
trace_bpfel.go (или trace_bpfeb.go, в зависимости от архитектуры)
trace_bpfel.o (скомпилированный eBPF-байт-код)
Если хотите, чтобы файлы создавались в папке bpf, укажите -output:

sh
Копировать
Редактировать
bpf2go -target native -output bpf/ trace bpf/trace.c -- -I/usr/include/
Теперь в main.go подключайте сгенерированный пакет:

go
Копировать
Редактировать
package main

import (
	"fmt"
	"log"
)

func main() {
	objs := bpf.TraceObjects{} // Имя структуры зависит от префикса в bpf2go
	if err := bpf.LoadTraceObjects(&objs, nil); err != nil {
		log.Fatalf("failed to load BPF objects: %v", err)
	}
	defer objs.Close()

	fmt.Println("eBPF программа загружена!")
}
Затем выполните:

sh
Копировать
Редактировать
go run main.go
Если возникают ошибки, убедитесь, что у вас установлен clang и llvm:

sh
Копировать
Редактировать
sudo apt install clang llvm
Готово! 🚀 Теперь ваш Go-код использует eBPF.







