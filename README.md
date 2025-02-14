bpf2go -target native -pkg main trace bpf/trace.c -- -I/usr/include/


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -target native -pkg main trace bpf/trace.c -- -I/usr/include/
flag provided but not defined: -pkg
Usage: bpf2go [options] <ident> <source file> [-- <C flags>]

ident is used as the stem of all generated Go types and functions, and
must be a valid Go identifier.

source is a single C file that is compiled using the specified compiler
(usually some version of clang).

You can pass options to the compiler by appending them after a '--' argument
or by supplying -cflags. Flags passed as arguments take precedence
over flags passed via -cflags. Additionally, the program expands quotation
marks in -cflags. This means that -cflags 'foo "bar baz"' is passed to the
compiler as two arguments "foo" and "bar baz".

The program expects GOPACKAGE to be set in the environment, and should be invoked
via go generate. The generated files are written to the current directory.

Some options take defaults from the environment. Variable name is mentioned
next to the respective option.

Options:

  -cc binary
        binary used to compile C to BPF ($BPF2GO_CC) (default "clang")
  -cflags string
        flags passed to the compiler, may contain quoted arguments ($BPF2GO_CFLAGS)
  -go-package string
        package for output go file (default as ENV GOPACKAGE)
  -makebase directory
        write make compatible depinfo files relative to directory ($BPF2GO_MAKEBASE)
  -no-global-types
        Skip generating types for map keys and values, etc.
  -no-strip
        disable stripping of DWARF
  -output-dir string
        target directory of generated files (defaults to current directory)
  -output-stem string
        alternative stem for names of generated files (defaults to ident)
  -output-suffix string
        suffix in generated file names such as _test (default based on $GOFILE)
  -strip binary
        binary used to strip DWARF from compiled BPF ($BPF2GO_STRIP)
  -tags value
        Comma-separated list of Go build tags to include in generated files
  -target string
        clang target(s) to compile for (comma separated) (default "bpfel,bpfeb")
  -type Name
        Name of a type to generate a Go declaration for, may be repeated

Supported targets:
        bpf
        bpfel
        bpfeb
        386
        amd64
        arm
        arm64
        loong64
        mips
        ppc64
        ppc64le
        riscv64
        s390x
Error: flag provided but not defined: -pkg
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 





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







