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


static __always_inline int save_sockaddr(struct sockaddr *sock_addr, u32 pid) {
	bpf_map_update_elem(&sockaddr_map, &pid, sock_addr, BPF_ANY);
	return 0;
}

static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
	struct conn_info_t conn_info = {};
	conn_info.pid = pid;
	bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
	bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
	save_sockaddr(sock_addr, pid);
	return 0;
}

struct sockaddr_in addr;
struct sockaddr *sock_addr = bpf_map_lookup_elem(&sockaddr_map, &pid);
if (!sock_addr) {
	bpf_printk("EXIT_accept4 Failed to find sockaddr for PID=%d\n", pid);
	bpf_map_delete_elem(&conn_info_map_ra, &pid);
	return 0;
}

if (bpf_probe_read(&addr, sizeof(addr), sock_addr) != 0) {
	bpf_printk("EXIT_accept4 Failed to read sockaddr for PID=%d\n", pid);
	bpf_map_delete_elem(&conn_info_map_ra, &pid);
	return 0;
}

if (addr.sin_family == AF_INET) {
	conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
	conn_info->sport = bpf_ntohs(addr.sin_port);

	bpf_printk("EXIT_accept4 Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
		conn_info->pid, conn_info->comm,
		(conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
		(conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
}

bpf_map_update_elem(&conn_info_map_ra, &pid, conn_info, BPF_ANY);
bpf_map_delete_elem(&sockaddr_map, &pid); // Удаляем sockaddr после использования


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc tracepoint.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "clang" CommandArgs { inner: ["-g", "-O2", "-target", "bpf", "-Wno-unknown-attributes", "-D__TARGET_ARCH_x86", "-idirafter", "/usr/lib/llvm-18/lib/clang/18/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-I/tmp/.tmpMNflp0/include", "-I/tmp/.tmpMNflp0/include/vmlinux/x86", "-I/home/gaz358/myprog/bpfgo", "-c", "tracepoint.c", "-o", "tracepoint.bpf.o"] }
INFO [ecc_rs::bpf_compiler] 
ERROR [ecc_rs::bpf_compiler] tracepoint.c:108:78: warning: declaration of 'struct bpf_map_def' will not be visible outside of this function [-Wvisibility]
  108 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
      |                                                                              ^
tracepoint.c:113:2: error: call to undeclared function 'save_sockaddr'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  113 |         save_sockaddr(sock_addr, pid);
      |         ^
tracepoint.c:117:28: error: static declaration of 'save_sockaddr' follows non-static declaration
  117 | static __always_inline int save_sockaddr(struct sockaddr *sock_addr, u32 pid) {
      |                            ^
tracepoint.c:113:2: note: previous implicit declaration is here
  113 |         save_sockaddr(sock_addr, pid);
      |         ^
tracepoint.c:124:67: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:87:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  124 |     return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_ra, bpf_get_current_pid_tgid() >> 32);
      |                                                                   ^~~~~~~~~~~~~~~~~
tracepoint.c:108:91: note: passing argument to parameter 'map' here
  108 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
      |                                                                                           ^
tracepoint.c:130:62: warning: incompatible pointer types passing 'struct (unnamed struct at tracepoint.c:78:1) *' to parameter of type 'struct bpf_map_def *' [-Wincompatible-pointer-types]
  130 |     return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map_sc, bpf_get_current_pid_tgid() >> 32);
      |                                                              ^~~~~~~~~~~~~~~~~
tracepoint.c:108:91: note: passing argument to parameter 'map' here
  108 | static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid) {
      |                                                                                           ^
3 warnings and 2 errors generated.

Error: Failed to compile

Caused by:
    Failed to run clang(exit code = Some(1))
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ git add .
