bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

BPF_PROG_TYPE_FLOW_DISSECTOR — анализирует содержимое пакета, чтобы извлечь такие данные, как IP-адрес или порт, что помогает в маршрутизации и фильтрации.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <uapi/linux/ptrace.h>
#include <net/sock.h>
#include <bcc/proto.h>

struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 port;
};

// Создаем BPF-карту для хранения информации о соединениях
BPF_HASH(conn_map, u32, struct conn_info_t);

// Привязываем функцию к kprobe для inet_accept
SEC("kprobe/inet_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    
    // Получаем IP-адрес и порт из структуры sock
    u16 dport = sk->__sk_common.skc_dport;
    u32 dip = sk->__sk_common.skc_daddr;
    
    info.ip = dip;
    info.port = ntohs(dport);

    // Сохраняем информацию о соединении в BPF-карте
    conn_map.update(&pid, &info);

    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc fentry.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpg3fdO1/include", "-I/tmp/.tmpg3fdO1/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "fentry.c", "-o", "fentry.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] fentry.c:30:17: error: call to undeclared function 'ntohs'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
   30 |     info.port = ntohs(dport);
      |                 ^
1 error generated.

Error: Failed to compile
