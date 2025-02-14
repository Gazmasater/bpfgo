clang -target bpf -O2 -c program.c -o program.o



package main

import (
    "log"
    "os"
    "os/signal"
    "syscall"

    "github.com/cilium/ebpf"
    "github.com/cilium/ebpf/link"
    "github.com/cilium/ebpf/rlimit"
)

func main() {
    // Снимаем ограничения на использование памяти для eBPF
    if err := rlimit.RemoveMemlock(); err != nil {
        log.Fatalf("не удалось снять ограничение на память: %v", err)
    }

    // Загружаем eBPF-объект из файла
    obj, err := ebpf.LoadCollection("trace.bpf.o")
    if err != nil {
        log.Fatalf("ошибка при загрузке eBPF-объекта: %v", err)
    }
    defer obj.Close()

    // Получаем eBPF-программы из загруженного объекта
    progEnter := obj.Programs["trace_sendto_enter"]
    if progEnter == nil {
        log.Fatalf("программа trace_sendto_enter не найдена")
    }

    progExit := obj.Programs["trace_sendto_exit"]
    if progExit == nil {
        log.Fatalf("программа trace_sendto_exit не найдена")
    }

    // Привязываем программы к соответствующим tracepoint'ам
    linkEnter, err := link.Tracepoint("syscalls", "sys_enter_sendto", progEnter, nil)
    if err != nil {
        log.Fatalf("ошибка при привязке trace_sendto_enter: %v", err)
    }
    defer linkEnter.Close()

    linkExit, err := link.Tracepoint("syscalls", "sys_exit_sendto", progExit, nil)
    if err != nil {
        log.Fatalf("ошибка при привязке trace_sendto_exit: %v", err)
    }
    defer linkExit.Close()

    // Ожидаем сигнала завершения
    sigs := make(chan os.Signal, 1)
    signal.Notify(sigs, os.Interrupt, syscall.SIGTERM)
    <-sigs

    log.Println("Завершение работы программы")
}




//go:build ignore

#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

#include <bpf/bpf_helpers.h>

struct event_t {
    u32 pid;
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
    __type(value, struct sockaddr_in);
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
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sockaddr_in conn_info = {};

    conn_info.sin_family = AF_INET;  // IPv4 address family
    bpf_get_current_comm(&conn_info.sin_zero, sizeof(conn_info.sin_zero));

    if (ctx->addr) {
        bpf_probe_read(&conn_info, sizeof(conn_info), ctx->addr);
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

    struct sockaddr_in *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        return 0;
    }

    struct sockaddr_in *addr = conn_info;
    if (addr->sin_family == AF_INET) {
        u32 src_ip = bpf_ntohl(addr->sin_addr.s_addr);
        u16 sport = bpf_ntohs(addr->sin_port);
        struct event_t event = {
            .pid = conn_info->sin_family,
        };

        bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, &event, sizeof(event));
    }

    bpf_map_delete_elem(&conn_info_map, &pid);
    return 0;
}









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







