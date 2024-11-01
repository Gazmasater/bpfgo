bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "bpf/bpf.h"
#include "bpf/bpf_tracing.h"
#include "common_new.h"
#include "linux/ptrace.h"

char __license[] SEC("license") = "GPL";

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);
  __type(value, struct accept_args_t);
  __uint(max_entries, 1024*128);
} active_accept4_args_map SEC(".maps");



// https://linux.die.net/man/3/accept
// int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
SEC("kprobe/accept4")
int probe_accept4(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    int should_intercept(); 

    if (!should_intercept()) {
        return 0;
    }

    bpf_printk("kprobe/accept entry: PID: %d\n", pid);
    struct pt_regs *ctx2 = (struct pt_regs *)PT_REGS_PARM1(ctx);

    struct sockaddr *saddr;
    bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));

    // Build the connect_event and save it to the map
    struct accept_args_t accept_args = {};
    accept_args.addr = (struct sockaddr_in *)saddr;
    bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);

    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc ebpf_test.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpZQrBiG/include", "-I/tmp/.tmpZQrBiG/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "ebpf_test.c", "-o", "ebpf_test.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] In file included from ebpf_test.c:2:
In file included from /tmp/.tmpZQrBiG/include/bpf/bpf_tracing.h:5:
In file included from /tmp/.tmpZQrBiG/include/bpf/bpf_helpers.h:11:
/tmp/.tmpZQrBiG/include/bpf/bpf_helper_defs.h:56:23: error: redefinition of 'bpf_map_lookup_elem' as different kind of symbol
   56 | static void *(* const bpf_map_lookup_elem)(void *map, const void *key) = (void *) 1;
      |                       ^
/tmp/.tmpZQrBiG/include/bpf/bpf.h:151:16: note: previous definition is here
  151 | LIBBPF_API int bpf_map_lookup_elem(int fd, const void *key, void *value);
      |                ^
In file included from ebpf_test.c:2:
In file included from /tmp/.tmpZQrBiG/include/bpf/bpf_tracing.h:5:
In file included from /tmp/.tmpZQrBiG/include/bpf/bpf_helpers.h:11:
/tmp/.tmpZQrBiG/include/bpf/bpf_helper_defs.h:78:22: error: redefinition of 'bpf_map_update_elem' as different kind of symbol
   78 | static long (* const bpf_map_update_elem)(void *map, const void *key, const void *value, __u64 flags) = (void *) 2;
      |                      ^
/tmp/.tmpZQrBiG/include/bpf/bpf.h:148:16: note: previous definition is here
  148 | LIBBPF_API int bpf_map_update_elem(int fd, const void *key, const void *value,
      |                ^
In file included from ebpf_test.c:2:
In file included from /tmp/.tmpZQrBiG/include/bpf/bpf_tracing.h:5:
In file included from /tmp/.tmpZQrBiG/include/bpf/bpf_helpers.h:11:
/tmp/.tmpZQrBiG/include/bpf/bpf_helper_defs.h:88:22: error: redefinition of 'bpf_map_delete_elem' as different kind of symbol
   88 | static long (* const bpf_map_delete_elem)(void *map, const void *key) = (void *) 3;
      |                      ^
/tmp/.tmpZQrBiG/include/bpf/bpf.h:158:16: note: previous definition is here
  158 | LIBBPF_API int bpf_map_delete_elem(int fd, const void *key);
      |                ^
ebpf_test.c:39:25: error: incompatible pointer to integer conversion passing 'struct (unnamed struct at ebpf_test.c:8:1) *' to parameter of type 'int' [-Wint-conversion]
   39 |     bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);
      |                         ^~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmpZQrBiG/include/bpf/bpf.h:148:40: note: passing argument to parameter 'fd' here
  148 | LIBBPF_API int bpf_map_update_elem(int fd, const void *key, const void *value,
      |                                        ^
4 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
