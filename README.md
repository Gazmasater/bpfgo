bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc ebpf_test.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpbHLzR3/include", "-I/tmp/.tmpbHLzR3/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "ebpf_test.c", "-o", "ebpf_test.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] In file included from ebpf_test.c:4:
In file included from /tmp/.tmpbHLzR3/include/bpf/bpf_core_read.h:5:
In file included from /tmp/.tmpbHLzR3/include/bpf/bpf_helpers.h:11:
/tmp/.tmpbHLzR3/include/bpf/bpf_helper_defs.h:56:23: error: redefinition of 'bpf_map_lookup_elem' as different kind of symbol
   56 | static void *(* const bpf_map_lookup_elem)(void *map, const void *key) = (void *) 1;
      |                       ^
/tmp/.tmpbHLzR3/include/bpf/bpf.h:151:16: note: previous definition is here
  151 | LIBBPF_API int bpf_map_lookup_elem(int fd, const void *key, void *value);
      |                ^
In file included from ebpf_test.c:4:
In file included from /tmp/.tmpbHLzR3/include/bpf/bpf_core_read.h:5:
In file included from /tmp/.tmpbHLzR3/include/bpf/bpf_helpers.h:11:
/tmp/.tmpbHLzR3/include/bpf/bpf_helper_defs.h:78:22: error: redefinition of 'bpf_map_update_elem' as different kind of symbol
   78 | static long (* const bpf_map_update_elem)(void *map, const void *key, const void *value, __u64 flags) = (void *) 2;
      |                      ^
/tmp/.tmpbHLzR3/include/bpf/bpf.h:148:16: note: previous definition is here
  148 | LIBBPF_API int bpf_map_update_elem(int fd, const void *key, const void *value,
      |                ^
In file included from ebpf_test.c:4:
In file included from /tmp/.tmpbHLzR3/include/bpf/bpf_core_read.h:5:
In file included from /tmp/.tmpbHLzR3/include/bpf/bpf_helpers.h:11:
/tmp/.tmpbHLzR3/include/bpf/bpf_helper_defs.h:88:22: error: redefinition of 'bpf_map_delete_elem' as different kind of symbol
   88 | static long (* const bpf_map_delete_elem)(void *map, const void *key) = (void *) 3;
      |                      ^
/tmp/.tmpbHLzR3/include/bpf/bpf.h:158:16: note: previous definition is here
  158 | LIBBPF_API int bpf_map_delete_elem(int fd, const void *key);
      |                ^
ebpf_test.c:32:10: error: call to undeclared function 'should_intercept'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
   32 |     if (!should_intercept()) {
      |          ^
ebpf_test.c:45:25: error: incompatible pointer to integer conversion passing 'struct (unnamed struct at ebpf_test.c:15:1) *' to parameter of type 'int' [-Wint-conversion]
   45 |     bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);
      |                         ^~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmpbHLzR3/include/bpf/bpf.h:148:40: note: passing argument to parameter 'fd' here
  148 | LIBBPF_API int bpf_map_update_elem(int fd, const void *key, const void *value,
      |                                        ^
5 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
