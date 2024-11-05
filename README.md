#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include  "bpf/bpf_endian.h"
#define __TARGET_ARCH_x86_64  // Указываем целевую архитектуру


struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 port;
};

char LICENSE[] SEC("license") = "Dual BSD/GPL";

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
    info.port = __bpf_ntohs(dport);
        //bpf_printk("Connection accepted: PID=%d, IP=%x, PORT=%d\n", info.pid, info.ip, info.port);
      bpf_printk("Connection accepted: PID=%d\n", pid);



    return 0;
}


#define __TARGET_ARCH_x86_64 // Для x86_64

clang -O2 -target bpf -c ваш_файл.c -o ваш_файл.o
clang -O2 -target bpf -D__TARGET_ARCH_x86_64 -c fentry.c -o bpf_fentry.o



sudo bpftool prog load ваш_файл.o /sys/fs/bpf/trace_accept
sudo bpftool prog attach /sys/fs/bpf/trace_accept /sys/kernel/debug/tracing/kprobes/inet_accept

lang -O2 -target bpf -c fentry.c -o bpf_fen
try.o
fentry.c:15:38: error: Must specify a BPF target arch via __TARGET_ARCH_xxx
   15 |     struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
      |                                      ^
/usr/include/bpf/bpf_tracing.h:563:29: note: expanded from macro 'PT_REGS_PARM1'
  563 | #define PT_REGS_PARM1(x) ({ _Pragma(__BPF_TARGET_MISSING); 0l; })
      |                             ^
<scratch space>:15:6: note: expanded from here
   15 |  GCC error "Must specify a BPF target arch via __TARGET_ARCH_xxx"
      |      ^
1 error generated.


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BEBRA
chmod +x BebraAppimage

./BebraAppimage

sudo apt update
sudo apt install fuse

./Bebra.AppImage --appimage-extract

sudo apt update
sudo apt upgrade -y
sudo apt full-upgrade -y

sudo apt autoremove -y
sudo apt autoclean

sudo reboot




az358@gaz358-BOD-WXX9:~/Downloads$ ./Bebra.AppImage 
dlopen(): error loading libfuse.so.2

AppImages require FUSE to run. 
You might still be able to extract the contents of this AppImage 
if you run it with the --appimage-extract option. 
See https://github.com/AppImage/AppImageKit/wiki/FUSE 
for more information

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc fentry.c
fuse: failed to exec fusermount3: No such file or directory

Cannot mount AppImage, please check your FUSE setup.
You might still be able to extract the contents of this AppImage 
if you run it with the --appimage-extract option. 
See https://github.com/AppImage/AppImageKit/wiki/FUSE 
for more information
open dir error: No such file or directory
