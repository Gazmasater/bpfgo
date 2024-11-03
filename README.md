bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "vmlinux.h"
//#include "bpf/bpf_core_read.h"
#include "bpf/bpf_tracing.h"
//#include "bpf/bpf.h"
//#include "netinet/in.h"

#define TASK_COMM_LEN  16


SEC("kprobe/__x64_sys_accept")
int bpf_prog(struct pt_regs *ctx) {
    struct task_struct *task = (struct task_struct *)bpf_get_current_task();
    
    // Получаем информацию о задаче
    char comm[TASK_COMM_LEN];
    bpf_get_current_comm(&comm, sizeof(comm));

    // Здесь вы можете логировать информацию или делать что-то другое
    bpf_trace_printk("Process accepted a connection on  sockfd: %s\n", comm);

    return 0;
}

char _license[] SEC("license") = "GPL";


az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc fentry.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmp7Zw0YR/include", "-I/tmp/.tmp7Zw0YR/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "fentry.c", "-o", "fentry.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] fentry.c:19:72: error: incompatible pointer to integer conversion passing 'char[16]' to parameter of type '__u32' (aka 'unsigned int') [-Wint-conversion]
   19 |     bpf_trace_printk("Process accepted a connection on  sockfd: %d\n", comm);
      |                                                                        ^~~~
1 error generated.

Error: Failed to compile
