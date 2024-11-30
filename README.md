sudo bpftrace -l


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ strace nc -l -u 12345
execve("/usr/bin/nc", ["nc", "-l", "-u", "12345"], 0x7ffc44c291f8 /* 80 vars */) = 0
brk(NULL)                               = 0x60bc6fc6f000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ea6a4a89000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=61431, ...}) = 0
mmap(NULL, 61431, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7ea6a4a7a000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libbsd.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=80888, ...}) = 0
mmap(NULL, 86208, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ea6a4a64000
mmap(0x7ea6a4a68000, 49152, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x4000) = 0x7ea6a4a68000
mmap(0x7ea6a4a74000, 12288, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x10000) = 0x7ea6a4a74000
mmap(0x7ea6a4a77000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x12000) = 0x7ea6a4a77000
mmap(0x7ea6a4a79000, 192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ea6a4a79000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libresolv.so.2", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=68104, ...}) = 0
mmap(NULL, 75912, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ea6a4a51000
mmap(0x7ea6a4a54000, 40960, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7ea6a4a54000
mmap(0x7ea6a4a5e000, 8192, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xd000) = 0x7ea6a4a5e000
mmap(0x7ea6a4a60000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xf000) = 0x7ea6a4a60000
mmap(0x7ea6a4a62000, 6280, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ea6a4a62000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220\243\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
fstat(3, {st_mode=S_IFREG|0755, st_size=2125328, ...}) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2170256, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ea6a4800000
mmap(0x7ea6a4828000, 1605632, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7ea6a4828000
mmap(0x7ea6a49b0000, 323584, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1b0000) = 0x7ea6a49b0000
mmap(0x7ea6a49ff000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1fe000) = 0x7ea6a49ff000
mmap(0x7ea6a4a05000, 52624, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ea6a4a05000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libmd.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=55536, ...}) = 0
mmap(NULL, 57448, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ea6a4a42000
mmap(0x7ea6a4a44000, 36864, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7ea6a4a44000
mmap(0x7ea6a4a4d000, 8192, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xb000) = 0x7ea6a4a4d000
mmap(0x7ea6a4a4f000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xc000) = 0x7ea6a4a4f000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ea6a4a40000
arch_prctl(ARCH_SET_FS, 0x7ea6a4a41140) = 0
set_tid_address(0x7ea6a4a41410)         = 3596
set_robust_list(0x7ea6a4a41420, 24)     = 0
rseq(0x7ea6a4a41a60, 0x20, 0, 0x53053053) = 0
mprotect(0x7ea6a49ff000, 16384, PROT_READ) = 0
mprotect(0x7ea6a4a4f000, 4096, PROT_READ) = 0
mprotect(0x7ea6a4a60000, 4096, PROT_READ) = 0
mprotect(0x7ea6a4a77000, 4096, PROT_READ) = 0
mprotect(0x60bc6f324000, 4096, PROT_READ) = 0
mprotect(0x7ea6a4ac1000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7ea6a4a7a000, 61431)           = 0
rt_sigaction(SIGPIPE, {sa_handler=SIG_IGN, sa_mask=[PIPE], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7ea6a4845320}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
getrandom("\x1f\x36\x1d\x2b\x54\x75\x23\xdc", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x60bc6fc6f000
brk(0x60bc6fc90000)                     = 0x60bc6fc90000
socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) = 3
setsockopt(3, SOL_SOCKET, SO_REUSEADDR, [1], 4) = 0
setsockopt(3, SOL_SOCKET, SO_REUSEPORT, [1], 4) = 0
bind(3, {sa_family=AF_INET, sin_port=htons(12345), sin_addr=inet_addr("0.0.0.0")}, 16) = 0
recvfrom(3, 
0x7ffe6c39dc40, 2048, MSG_PEEK, 0x7ffe6c39d840, [128]) = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGWINCH {si_signo=SIGWINCH, si_code=SI_KERNEL} ---
recvfrom(3, 0x7ffe6c39dc40, 2048, MSG_PEEK, 0x7ffe6c39d840, [128]) = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGWINCH {si_signo=SIGWINCH, si_code=SI_KERNEL} ---
recvfrom(3, 0x7ffe6c39dc40, 2048, MSG_PEEK, 0x7ffe6c39d840, [128]) = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGWINCH {si_signo=SIGWINCH, si_code=SI_KERNEL} ---
recvfrom(3, "Hello UDP Server8899!", 2048, MSG_PEEK, {sa_family=AF_INET, sin_port=htons(38914), sin_addr=inet_addr("127.0.0.1")}, [128 => 16]) = 21
connect(3, {sa_family=AF_INET, sin_port=htons(38914), sin_addr=inet_addr("127.0.0.1")}, 16) = 0
poll([{fd=0, events=POLLIN}, {fd=3, events=0}, {fd=3, events=POLLIN}, {fd=1, events=0}], 4, -1) = 2 ([{fd=0, revents=POLLIN}, {fd=3, revents=POLLIN}])
read(0, "\n", 16384)                    = 1
read(3, "Hello UDP Server8899!", 16384) = 21
poll([{fd=0, events=POLLIN}, {fd=3, events=POLLOUT}, {fd=3, events=POLLIN}, {fd=1, events=POLLOUT}], 4, -1) = 2 ([{fd=3, revents=POLLOUT}, {fd=1, revents=POLLOUT}])
write(3, "\n", 1)                       = 1
write(1, "Hello UDP Server8899!", 21Hello UDP Server8899!)   = 21
poll([{fd=0, events=POLLIN}, {fd=3, events=0}, {fd=3, events=POLLIN}, {fd=1, events=0}], 4, -1
