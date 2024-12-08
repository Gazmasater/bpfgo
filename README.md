BPF_PERF_OUTPUT(events); // карта для передачи данных в юзерспейс


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc tracepoint.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpzY6oSK/include", "-I/tmp/.tmpzY6oSK/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "tracepoint.c", "-o", "tracepoint.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] tracepoint.c:166:1: error: type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
  166 | BPF_PERF_OUTPUT(events); 
      | ^
      | int
tracepoint.c:166:17: error: a parameter list without types is only allowed in a function definition
  166 | BPF_PERF_OUTPUT(events); 
      |                 ^
tracepoint.c:181:78: warning: declaration of 'struct bpf_map_def' will not be visible outside of this function [-Wvisibility]
  181 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 call)
      |                                                                              ^
tracepoint.c:202:57: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:148:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  202 |     return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map_sc, bpf_get_current_pid_tgid() >> 32,4);
      |                                                         ^~~~~~~~~~~~~~~~~
tracepoint.c:181:91: note: passing argument to parameter 'map' here
  181 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 call)
      |                                                                                           ^
tracepoint.c:208:57: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:157:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  208 |     return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map_ra, bpf_get_current_pid_tgid() >> 32,3);
      |                                                         ^~~~~~~~~~~~~~~~~
tracepoint.c:181:91: note: passing argument to parameter 'map' here
  181 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 call)
      |                                                                                           ^
tracepoint.c:221:67: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:157:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  221 |     return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_ra, bpf_get_current_pid_tgid() >> 32,1);
      |                                                                   ^~~~~~~~~~~~~~~~~
tracepoint.c:181:91: note: passing argument to parameter 'map' here
  181 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 call)
      |                                                                                           ^
tracepoint.c:227:62: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:148:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  227 |     return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map_sc, bpf_get_current_pid_tgid() >> 32,2);
      |                                                              ^~~~~~~~~~~~~~~~~
tracepoint.c:181:91: note: passing argument to parameter 'map' here
  181 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 call)
      |                                                                                           ^
5 warnings and 2 errors generated.
