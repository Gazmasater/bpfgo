nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

sudo bpftool map dump id 11




11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


bpf2go -output-dir $(pwd) \
  -tags linux \
  -type trace_info \
  -go-package main \
  target_amd64_bpf \
  $(pwd)/trace.c -- -I$(pwd)


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc tracepoint.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpCocf9h/include", "-I/tmp/.tmpCocf9h/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "tracepoint.c", "-o", "tracepoint.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] tracepoint.c:101:78: warning: declaration of 'struct bpf_map_def' will not be visible outside of this function [-Wvisibility]
  101 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
      |                                                                              ^
tracepoint.c:106:2: error: call to undeclared function 'save_sockaddr'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  106 |         save_sockaddr(sock_addr, pid);
      |         ^
tracepoint.c:110:28: error: static declaration of 'save_sockaddr' follows non-static declaration
  110 | static __always_inline int save_sockaddr(struct sockaddr *sock_addr, u32 pid) {
      |                            ^
tracepoint.c:106:2: note: previous implicit declaration is here
  106 |         save_sockaddr(sock_addr, pid);
      |         ^
tracepoint.c:121:67: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:80:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  121 |     return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map, bpf_get_current_pid_tgid() >> 32);
      |                                                                   ^~~~~~~~~~~~~~
tracepoint.c:101:91: note: passing argument to parameter 'map' here
  101 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
      |                                                                                           ^
tracepoint.c:127:62: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:80:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  127 |     return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map, bpf_get_current_pid_tgid() >> 32);
      |                                                              ^~~~~~~~~~~~~~
tracepoint.c:101:91: note: passing argument to parameter 'map' here
  101 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
      |                                                                                           ^
3 warnings and 2 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
