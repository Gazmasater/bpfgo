bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc ebpf_test.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmp8Htfer/include", "-I/tmp/.tmp8Htfer/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "ebpf_test.c", "-o", "ebpf_test.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] In file included from ebpf_test.c:4:
In file included from /tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:5:
In file included from /tmp/.tmp8Htfer/include/bpf/bpf_helpers.h:11:
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:56:23: error: redefinition of 'bpf_map_lookup_elem' as different kind of symbol
   56 | static void *(* const bpf_map_lookup_elem)(void *map, const void *key) = (void *) 1;
      |                       ^
/tmp/.tmp8Htfer/include/bpf/bpf.h:151:16: note: previous definition is here
  151 | LIBBPF_API int bpf_map_lookup_elem(int fd, const void *key, void *value);
      |                ^
In file included from ebpf_test.c:4:
In file included from /tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:5:
In file included from /tmp/.tmp8Htfer/include/bpf/bpf_helpers.h:11:
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:78:22: error: redefinition of 'bpf_map_update_elem' as different kind of symbol
   78 | static long (* const bpf_map_update_elem)(void *map, const void *key, const void *value, __u64 flags) = (void *) 2;
      |                      ^
/tmp/.tmp8Htfer/include/bpf/bpf.h:148:16: note: previous definition is here
  148 | LIBBPF_API int bpf_map_update_elem(int fd, const void *key, const void *value,
      |                ^
In file included from ebpf_test.c:4:
In file included from /tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:5:
In file included from /tmp/.tmp8Htfer/include/bpf/bpf_helpers.h:11:
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:88:22: error: redefinition of 'bpf_map_delete_elem' as different kind of symbol
   88 | static long (* const bpf_map_delete_elem)(void *map, const void *key) = (void *) 3;
      |                      ^
/tmp/.tmp8Htfer/include/bpf/bpf.h:158:16: note: previous definition is here
  158 | LIBBPF_API int bpf_map_delete_elem(int fd, const void *key);
      |                ^
ebpf_test.c:32:10: error: call to undeclared function 'should_intercept'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
   32 |     if (!should_intercept()) {
      |          ^
ebpf_test.c:45:25: error: incompatible pointer to integer conversion passing 'struct (unnamed struct at ebpf_test.c:15:1) *' to parameter of type 'int' [-Wint-conversion]
   45 |     bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);
      |                         ^~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf.h:148:40: note: passing argument to parameter 'fd' here
  148 | LIBBPF_API int bpf_map_update_elem(int fd, const void *key, const void *value,
      |                                        ^
ebpf_test.c:55:104: error: too few arguments to function call, expected 3, have 2
   55 |     struct accept_args_t* accept_args = bpf_map_lookup_elem(&active_accept4_args_map, &current_pid_tgid);
      |                                         ~~~~~~~~~~~~~~~~~~~                                            ^
/tmp/.tmp8Htfer/include/bpf/bpf.h:151:16: note: 'bpf_map_lookup_elem' declared here
  151 | LIBBPF_API int bpf_map_lookup_elem(int fd, const void *key, void *value);
      |                ^                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ebpf_test.c:69:5: error: call to undeclared function 'parse_address'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
   69 |     parse_address(&src_addr, accept_args);
      |     ^
ebpf_test.c:78:24: error: incomplete definition of type 'struct task_struct'
   78 |     const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:520:2: note: expanded from macro 'BPF_CORE_READ'
  520 |         ___type((src), a, ##__VA_ARGS__) __r;                               \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:391:29: note: expanded from macro '___type'
  391 | #define ___type(...) typeof(___arrow(__VA_ARGS__))
      |                             ^~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:389:23: note: expanded from macro '___arrow'
  389 | #define ___arrow(...) ___apply(___arrow, ___narg(__VA_ARGS__))(__VA_ARGS__)
      |                       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
note: (skipping 1 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:340:25: note: expanded from macro '___concat'
  340 | #define ___concat(a, b) a ## b
      |                         ^
<scratch space>:144:1: note: expanded from here
  144 | ___arrow6
      | ^
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:384:38: note: expanded from macro '___arrow6'
  384 | #define ___arrow6(a, b, c, d, e, f) a->b->c->d->e->f
      |                                     ~^
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:31:8: note: forward declaration of 'struct task_struct'
   31 | struct task_struct;
      |        ^
ebpf_test.c:78:24: error: incomplete definition of type 'struct task_struct'
   78 |     const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:521:2: note: expanded from macro 'BPF_CORE_READ'
  521 |         BPF_CORE_READ_INTO(&__r, (src), a, ##__VA_ARGS__);                  \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:428:2: note: expanded from macro 'BPF_CORE_READ_INTO'
  428 |         ___core_read(bpf_core_read, bpf_core_read,                          \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  429 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:419:2: note: expanded from macro '___core_read'
  419 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  420 |                                                       src, a, ##__VA_ARGS__)
      |                                                       ~~~~~~~~~~~~~~~~~~~~~~
note: (skipping 12 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:397:33: note: expanded from macro '___rd_first'
  397 | #define ___rd_first(fn, src, a) ___read(fn, &__t, ___type(src), src, a);
      |                                 ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:394:59: note: expanded from macro '___read'
  394 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:312:79: note: expanded from macro 'bpf_core_read'
  312 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                                                                      ^~~
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:31:8: note: forward declaration of 'struct task_struct'
   31 | struct task_struct;
      |        ^
ebpf_test.c:78:24: error: incomplete definition of type 'struct task_struct'
   78 |     const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:521:2: note: expanded from macro 'BPF_CORE_READ'
  521 |         BPF_CORE_READ_INTO(&__r, (src), a, ##__VA_ARGS__);                  \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:428:2: note: expanded from macro 'BPF_CORE_READ_INTO'
  428 |         ___core_read(bpf_core_read, bpf_core_read,                          \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  429 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:419:2: note: expanded from macro '___core_read'
  419 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  420 |                                                       src, a, ##__VA_ARGS__)
      |                                                       ~~~~~~~~~~~~~~~~~~~~~~
note: (skipping 17 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:380:26: note: expanded from macro '___arrow2'
  380 | #define ___arrow2(a, b) a->b
      |                          ^
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:394:44: note: expanded from macro '___read'
  394 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:312:79: note: expanded from macro 'bpf_core_read'
  312 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                                                                      ^~~
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:31:8: note: forward declaration of 'struct task_struct'
   31 | struct task_struct;
      |        ^
ebpf_test.c:78:24: error: incomplete definition of type 'struct task_struct'
   78 |     const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:521:2: note: expanded from macro 'BPF_CORE_READ'
  521 |         BPF_CORE_READ_INTO(&__r, (src), a, ##__VA_ARGS__);                  \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:428:2: note: expanded from macro 'BPF_CORE_READ_INTO'
  428 |         ___core_read(bpf_core_read, bpf_core_read,                          \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  429 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:419:2: note: expanded from macro '___core_read'
  419 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  420 |                                                       src, a, ##__VA_ARGS__)
      |                                                       ~~~~~~~~~~~~~~~~~~~~~~
note: (skipping 16 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:381:29: note: expanded from macro '___arrow3'
  381 | #define ___arrow3(a, b, c) a->b->c
      |                             ^
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:394:44: note: expanded from macro '___read'
  394 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:312:79: note: expanded from macro 'bpf_core_read'
  312 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                                                                      ^~~
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:31:8: note: forward declaration of 'struct task_struct'
   31 | struct task_struct;
      |        ^
ebpf_test.c:78:24: error: incomplete definition of type 'struct task_struct'
   78 |     const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:521:2: note: expanded from macro 'BPF_CORE_READ'
  521 |         BPF_CORE_READ_INTO(&__r, (src), a, ##__VA_ARGS__);                  \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:428:2: note: expanded from macro 'BPF_CORE_READ_INTO'
  428 |         ___core_read(bpf_core_read, bpf_core_read,                          \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  429 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:419:2: note: expanded from macro '___core_read'
  419 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  420 |                                                       src, a, ##__VA_ARGS__)
      |                                                       ~~~~~~~~~~~~~~~~~~~~~~
note: (skipping 15 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:382:32: note: expanded from macro '___arrow4'
  382 | #define ___arrow4(a, b, c, d) a->b->c->d
      |                                ^
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:394:44: note: expanded from macro '___read'
  394 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:312:79: note: expanded from macro 'bpf_core_read'
  312 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                                                                      ^~~
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:31:8: note: forward declaration of 'struct task_struct'
   31 | struct task_struct;
      |        ^
ebpf_test.c:78:24: error: incomplete definition of type 'struct task_struct'
   78 |     const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:521:2: note: expanded from macro 'BPF_CORE_READ'
  521 |         BPF_CORE_READ_INTO(&__r, (src), a, ##__VA_ARGS__);                  \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:428:2: note: expanded from macro 'BPF_CORE_READ_INTO'
  428 |         ___core_read(bpf_core_read, bpf_core_read,                          \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  429 |                      dst, (src), a, ##__VA_ARGS__)                          \
      |                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:419:2: note: expanded from macro '___core_read'
  419 |         ___apply(___core_read, ___empty(__VA_ARGS__))(fn, fn_ptr, dst,      \
      |         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  420 |                                                       src, a, ##__VA_ARGS__)
      |                                                       ~~~~~~~~~~~~~~~~~~~~~~
note: (skipping 9 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:383:35: note: expanded from macro '___arrow5'
  383 | #define ___arrow5(a, b, c, d, e) a->b->c->d->e
      |                                   ^
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:394:44: note: expanded from macro '___read'
  394 |         read_fn((void *)(dst), sizeof(*(dst)), &((src_type)(src))->accessor)
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf_core_read.h:312:79: note: expanded from macro 'bpf_core_read'
  312 |         bpf_probe_read_kernel(dst, sz, (const void *)__builtin_preserve_access_index(src))
      |                                                                                      ^~~
/tmp/.tmp8Htfer/include/bpf/bpf_helper_defs.h:31:8: note: forward declaration of 'struct task_struct'
   31 | struct task_struct;
      |        ^
ebpf_test.c:95:25: error: incompatible pointer to integer conversion passing 'struct (unnamed struct at ebpf_test.c:15:1) *' to parameter of type 'int' [-Wint-conversion]
   95 |     bpf_map_delete_elem(&active_accept4_args_map, &current_pid_tgid);
      |                         ^~~~~~~~~~~~~~~~~~~~~~~~
/tmp/.tmp8Htfer/include/bpf/bpf.h:158:40: note: passing argument to parameter 'fd' here
  158 | LIBBPF_API int bpf_map_delete_elem(int fd, const void *key);
      |                                        ^
14 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 




