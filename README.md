sudo bpftrace -l


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ strace nc -6 -l -u 1234
execve("/usr/bin/nc", ["nc", "-6", "-l", "-u", "1234"], 0x7ffebd36bf30 /* 80 vars */) = 0
brk(NULL)                               = 0x6364d16fc000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7cdfc7cc2000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=61431, ...}) = 0
mmap(NULL, 61431, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7cdfc7cb3000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libbsd.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=80888, ...}) = 0
mmap(NULL, 86208, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7cdfc7c9d000
mmap(0x7cdfc7ca1000, 49152, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x4000) = 0x7cdfc7ca1000
mmap(0x7cdfc7cad000, 12288, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x10000) = 0x7cdfc7cad000
mmap(0x7cdfc7cb0000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x12000) = 0x7cdfc7cb0000
mmap(0x7cdfc7cb2000, 192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7cdfc7cb2000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libresolv.so.2", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=68104, ...}) = 0
mmap(NULL, 75912, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7cdfc7c8a000
mmap(0x7cdfc7c8d000, 40960, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7cdfc7c8d000
mmap(0x7cdfc7c97000, 8192, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xd000) = 0x7cdfc7c97000
mmap(0x7cdfc7c99000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xf000) = 0x7cdfc7c99000
mmap(0x7cdfc7c9b000, 6280, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7cdfc7c9b000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220\243\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
fstat(3, {st_mode=S_IFREG|0755, st_size=2125328, ...}) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2170256, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7cdfc7a00000
mmap(0x7cdfc7a28000, 1605632, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7cdfc7a28000
mmap(0x7cdfc7bb0000, 323584, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1b0000) = 0x7cdfc7bb0000
mmap(0x7cdfc7bff000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1fe000) = 0x7cdfc7bff000
mmap(0x7cdfc7c05000, 52624, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7cdfc7c05000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libmd.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=55536, ...}) = 0
mmap(NULL, 57448, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7cdfc7c7b000
mmap(0x7cdfc7c7d000, 36864, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7cdfc7c7d000
mmap(0x7cdfc7c86000, 8192, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xb000) = 0x7cdfc7c86000
mmap(0x7cdfc7c88000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xc000) = 0x7cdfc7c88000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7cdfc7c79000
arch_prctl(ARCH_SET_FS, 0x7cdfc7c7a140) = 0
set_tid_address(0x7cdfc7c7a410)         = 6522
set_robust_list(0x7cdfc7c7a420, 24)     = 0
rseq(0x7cdfc7c7aa60, 0x20, 0, 0x53053053) = 0
mprotect(0x7cdfc7bff000, 16384, PROT_READ) = 0
mprotect(0x7cdfc7c88000, 4096, PROT_READ) = 0
mprotect(0x7cdfc7c99000, 4096, PROT_READ) = 0
mprotect(0x7cdfc7cb0000, 4096, PROT_READ) = 0
mprotect(0x6364d0548000, 4096, PROT_READ) = 0
mprotect(0x7cdfc7cfa000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7cdfc7cb3000, 61431)           = 0
rt_sigaction(SIGPIPE, {sa_handler=SIG_IGN, sa_mask=[PIPE], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7cdfc7a45320}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
getrandom("\x3e\x26\xcc\x85\x22\xbd\x5b\x08", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x6364d16fc000
brk(0x6364d171d000)                     = 0x6364d171d000
socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP) = 3
setsockopt(3, SOL_SOCKET, SO_REUSEADDR, [1], 4) = 0
setsockopt(3, SOL_SOCKET, SO_REUSEPORT, [1], 4) = 0
bind(3, {sa_family=AF_INET6, sin6_port=htons(1234), sin6_flowinfo=htonl(0), inet_pton(AF_INET6, "::", &sin6_addr), sin6_scope_id=0}, 28) = 0
recvfrom(3, 
"Hello UDP Server8899!", 2048, MSG_PEEK, {sa_family=AF_INET6, sin6_port=htons(35485), sin6_flowinfo=htonl(0), inet_pton(AF_INET6, "::ffff:127.0.0.1", &sin6_addr), sin6_scope_id=0}, [128 => 28]) = 21
connect(3, {sa_family=AF_INET6, sin6_port=htons(35485), sin6_flowinfo=htonl(0), inet_pton(AF_INET6, "::ffff:127.0.0.1", &sin6_addr), sin6_scope_id=0}, 28) = 0
poll([{fd=0, events=POLLIN}, {fd=3, events=0}, {fd=3, events=POLLIN}, {fd=1, events=0}], 4, -1) = 2 ([{fd=0, revents=POLLIN}, {fd=3, revents=POLLIN}])
read(0, "\n", 16384)                    = 1
read(3, "Hello UDP Server8899!", 16384) = 21
poll([{fd=0, events=POLLIN}, {fd=3, events=POLLOUT}, {fd=3, events=POLLIN}, {fd=1, events=POLLOUT}], 4, -1) = 2 ([{fd=3, revents=POLLOUT}, {fd=1, revents=POLLOUT}])
write(3, "\n", 1)                       = 1
write(1, "Hello UDP Server8899!", 21Hello UDP Server8899!)   = 21
poll([{fd=0, events=POLLIN}, {fd=3, events=0}, {fd=3, events=POLLIN}, {fd=1, events=0}], 4, -1
