bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc fentry.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpHUkMRR/include", "-I/tmp/.tmpHUkMRR/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "fentry.c", "-o", "fentry.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] In file included from fentry.c:6:
./common.h:10:6: error: redefinition of 'bpf_map_type'
   10 | enum bpf_map_type {
      |      ^
./vmlinux.h:12368:6: note: previous definition is here
 12368 | enum bpf_map_type {
       |      ^
In file included from fentry.c:6:
./common.h:11:2: error: redefinition of enumerator 'BPF_MAP_TYPE_UNSPEC'
   11 |         BPF_MAP_TYPE_UNSPEC                = 0,
      |         ^
./vmlinux.h:12369:2: note: previous definition is here
 12369 |         BPF_MAP_TYPE_UNSPEC = 0,
       |         ^
In file included from fentry.c:6:
./common.h:12:2: error: redefinition of enumerator 'BPF_MAP_TYPE_HASH'
   12 |         BPF_MAP_TYPE_HASH                  = 1,
      |         ^
./vmlinux.h:12370:2: note: previous definition is here
 12370 |         BPF_MAP_TYPE_HASH = 1,
       |         ^
In file included from fentry.c:6:
./common.h:13:2: error: redefinition of enumerator 'BPF_MAP_TYPE_ARRAY'
   13 |         BPF_MAP_TYPE_ARRAY                 = 2,
      |         ^
./vmlinux.h:12371:2: note: previous definition is here
 12371 |         BPF_MAP_TYPE_ARRAY = 2,
       |         ^
In file included from fentry.c:6:
./common.h:14:2: error: redefinition of enumerator 'BPF_MAP_TYPE_PROG_ARRAY'
   14 |         BPF_MAP_TYPE_PROG_ARRAY            = 3,
      |         ^
./vmlinux.h:12372:2: note: previous definition is here
 12372 |         BPF_MAP_TYPE_PROG_ARRAY = 3,
       |         ^
In file included from fentry.c:6:
./common.h:15:2: error: redefinition of enumerator 'BPF_MAP_TYPE_PERF_EVENT_ARRAY'
   15 |         BPF_MAP_TYPE_PERF_EVENT_ARRAY      = 4,
      |         ^
./vmlinux.h:12373:2: note: previous definition is here
 12373 |         BPF_MAP_TYPE_PERF_EVENT_ARRAY = 4,
       |         ^
In file included from fentry.c:6:
./common.h:16:2: error: redefinition of enumerator 'BPF_MAP_TYPE_PERCPU_HASH'
   16 |         BPF_MAP_TYPE_PERCPU_HASH           = 5,
      |         ^
./vmlinux.h:12374:2: note: previous definition is here
 12374 |         BPF_MAP_TYPE_PERCPU_HASH = 5,
       |         ^
In file included from fentry.c:6:
./common.h:17:2: error: redefinition of enumerator 'BPF_MAP_TYPE_PERCPU_ARRAY'
   17 |         BPF_MAP_TYPE_PERCPU_ARRAY          = 6,
      |         ^
./vmlinux.h:12375:2: note: previous definition is here
 12375 |         BPF_MAP_TYPE_PERCPU_ARRAY = 6,
       |         ^
In file included from fentry.c:6:
./common.h:18:2: error: redefinition of enumerator 'BPF_MAP_TYPE_STACK_TRACE'
   18 |         BPF_MAP_TYPE_STACK_TRACE           = 7,
      |         ^
./vmlinux.h:12376:2: note: previous definition is here
 12376 |         BPF_MAP_TYPE_STACK_TRACE = 7,
       |         ^
In file included from fentry.c:6:
./common.h:19:2: error: redefinition of enumerator 'BPF_MAP_TYPE_CGROUP_ARRAY'
   19 |         BPF_MAP_TYPE_CGROUP_ARRAY          = 8,
      |         ^
./vmlinux.h:12377:2: note: previous definition is here
 12377 |         BPF_MAP_TYPE_CGROUP_ARRAY = 8,
       |         ^
In file included from fentry.c:6:
./common.h:20:2: error: redefinition of enumerator 'BPF_MAP_TYPE_LRU_HASH'
   20 |         BPF_MAP_TYPE_LRU_HASH              = 9,
      |         ^
./vmlinux.h:12378:2: note: previous definition is here
 12378 |         BPF_MAP_TYPE_LRU_HASH = 9,
       |         ^
In file included from fentry.c:6:
./common.h:21:2: error: redefinition of enumerator 'BPF_MAP_TYPE_LRU_PERCPU_HASH'
   21 |         BPF_MAP_TYPE_LRU_PERCPU_HASH       = 10,
      |         ^
./vmlinux.h:12379:2: note: previous definition is here
 12379 |         BPF_MAP_TYPE_LRU_PERCPU_HASH = 10,
       |         ^
In file included from fentry.c:6:
./common.h:22:2: error: redefinition of enumerator 'BPF_MAP_TYPE_LPM_TRIE'
   22 |         BPF_MAP_TYPE_LPM_TRIE              = 11,
      |         ^
./vmlinux.h:12380:2: note: previous definition is here
 12380 |         BPF_MAP_TYPE_LPM_TRIE = 11,
       |         ^
In file included from fentry.c:6:
./common.h:23:2: error: redefinition of enumerator 'BPF_MAP_TYPE_ARRAY_OF_MAPS'
   23 |         BPF_MAP_TYPE_ARRAY_OF_MAPS         = 12,
      |         ^
./vmlinux.h:12381:2: note: previous definition is here
 12381 |         BPF_MAP_TYPE_ARRAY_OF_MAPS = 12,
       |         ^
In file included from fentry.c:6:
./common.h:24:2: error: redefinition of enumerator 'BPF_MAP_TYPE_HASH_OF_MAPS'
   24 |         BPF_MAP_TYPE_HASH_OF_MAPS          = 13,
      |         ^
./vmlinux.h:12382:2: note: previous definition is here
 12382 |         BPF_MAP_TYPE_HASH_OF_MAPS = 13,
       |         ^
In file included from fentry.c:6:
./common.h:25:2: error: redefinition of enumerator 'BPF_MAP_TYPE_DEVMAP'
   25 |         BPF_MAP_TYPE_DEVMAP                = 14,
      |         ^
./vmlinux.h:12383:2: note: previous definition is here
 12383 |         BPF_MAP_TYPE_DEVMAP = 14,
       |         ^
In file included from fentry.c:6:
./common.h:26:2: error: redefinition of enumerator 'BPF_MAP_TYPE_SOCKMAP'
   26 |         BPF_MAP_TYPE_SOCKMAP               = 15,
      |         ^
./vmlinux.h:12384:2: note: previous definition is here
 12384 |         BPF_MAP_TYPE_SOCKMAP = 15,
       |         ^
In file included from fentry.c:6:
./common.h:27:2: error: redefinition of enumerator 'BPF_MAP_TYPE_CPUMAP'
   27 |         BPF_MAP_TYPE_CPUMAP                = 16,
      |         ^
./vmlinux.h:12385:2: note: previous definition is here
 12385 |         BPF_MAP_TYPE_CPUMAP = 16,
       |         ^
In file included from fentry.c:6:
./common.h:28:2: error: redefinition of enumerator 'BPF_MAP_TYPE_XSKMAP'
   28 |         BPF_MAP_TYPE_XSKMAP                = 17,
      |         ^
./vmlinux.h:12386:2: note: previous definition is here
 12386 |         BPF_MAP_TYPE_XSKMAP = 17,
       |         ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
