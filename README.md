lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sys_enter_getsockname
sys_exit_getsockname

SEC("tracepoint/syscalls/sys_enter_getsockname")
int trace_enter_getsockname(struct pt_regs *ctx) {
    int sockfd = (int)PT_REGS_PARM1(ctx);  // Дескриптор сокета
    struct sockaddr *addr = (struct sockaddr *)PT_REGS_PARM2(ctx);  // Указатель на sockaddr

    // Печатаем дескриптор сокета, но не локальный IP/порт
    bpf_printk("Entering getsockname syscall: sockfd=%d\n", sockfd);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_getsockname")
int trace_exit_getsockname(struct pt_regs *ctx) {
    int sockfd = (int)PT_REGS_PARM1(ctx);  // Дескриптор сокета
    struct sockaddr *addr = (struct sockaddr *)PT_REGS_PARM2(ctx);  // Указатель на sockaddr
    int addrlen = (int)PT_REGS_PARM3(ctx);  // Длина структуры sockaddr

    if (addr) {
        if (addr->sa_family == AF_INET) {  // Проверяем IPv4
            struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
            u32 ip = addr_in->sin_addr.s_addr;
            u16 port = bpf_ntohs(addr_in->sin_port);

            // Печатаем локальный IP и порт
            bpf_printk("Exit getsockname syscall: sockfd=%d, local ip=%x, local port=%d\n", sockfd, ip, port);
        }
    }

    return 0;
}

	SockNameEnter, err := link.Tracepoint("syscalls", "sys_enter_getsockname", objs.TraceEnterGetsockname, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_getsockname: %s", err)
	}
	defer SockNameEnter.Close()


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=4"
[sudo] password for gaz358: 
2025/03/27 18:26:54 opening tracepoint sys_enter_getsockname: cannot create bpf perf link: permission denied
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 




ls /sys/kernel/debug/tracing/events/syscalls/ | grep getaddrinfo
ls /sys/kernel/debug/tracing/events/syscalls/ | grep socket

SEC("tracepoint/syscalls/sys_enter_sockname")
int trace_enter_sockname(struct pt_regs *ctx) {
    // Получаем дескриптор сокета
    int sockfd = (int)PT_REGS_PARM1(ctx);

    // Получаем указатель на структуру sockaddr, в которой будет храниться локальный адрес
    addr = (struct sockaddr_in *)PT_REGS_PARM2(ctx);

    // Проверяем, что адрес не пустой
    if (addr) {
        // Получаем IP-адрес и порт
        u32 local_ip = addr->sin_addr.s_addr;
        u16 local_port = bpf_ntohs(addr->sin_port);

        // Печатаем информацию
        bpf_printk("Entering sockname syscall: sockfd=%d, local ip=%x, local port=%d\n", sockfd, local_ip, local_port);
    }

    return 0;
}


SEC("tracepoint/syscalls/sys_exit_sockname")
int trace_exit_sockname(struct pt_regs *ctx) {
    // Получаем дескриптор сокета
    int sockfd = (int)PT_REGS_PARM1(ctx);

    // Получаем локальный адрес, возвращаемый системой
    addr = (struct sockaddr_in *)PT_REGS_PARM2(ctx);

    // Проверяем, что адрес не пустой
    if (addr) {
        // Получаем IP-адрес и порт
        u32 local_ip = addr->sin_addr.s_addr;
        u16 local_port = bpf_ntohs(addr->sin_port);

        // Печатаем информацию
        bpf_printk("sockfd=%d, local ip=%x, local port=%d\n", sockfd, local_ip, local_port);
    }

    return 0;
}




	SockNameEnter, err := link.Tracepoint("syscalls", "sys_enter_getsockname", objs.TraceEnterGetsockname, nil)
	if err != nil {
		log.Fatalf("opening tracepoint sys_enter_getsockname: %s", err)
	}
	defer SockNameEnter.Close()

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo|grep "SYSCALL=4"
[sudo] password for gaz358: 
2025/03/27 18:26:54 opening tracepoint sys_enter_getsockname: cannot create bpf perf link: permission denied
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

sudo strace -e bpf,perf_event_open ./bpfgo


--- SIGURG {si_signo=SIGURG, si_code=SI_TKILL, si_pid=4858, si_uid=0} ---
--- SIGURG {si_signo=SIGURG, si_code=SI_TKILL, si_pid=4858, si_uid=0} ---
--- SIGURG {si_signo=SIGURG, si_code=SI_TKILL, si_pid=4858, si_uid=0} ---
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_SOCKET_FILTER, insn_cnt=2, insns=0xc002874fa0, license="MIT", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=-1, func_info_rec_size=0, func_info=NULL, func_info_cnt=1, line_info_rec_size=0, line_info=NULL, line_info_cnt=0, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = -1 EBADF (Bad file descriptor)
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\234\1\0\0\234\1\0\0i\3\0\0{\2\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1309, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_KPROBE, insn_cnt=6, insns=0xc0006648a0, license="GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=0, func_info_rec_size=0, func_info=NULL, func_info_cnt=0, line_info_rec_size=0, line_info=NULL, line_info_cnt=0, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 8
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_SK_LOOKUP, insn_cnt=89, insns=0xc002cce000, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="look_up", prog_ifindex=0, expected_attach_type=BPF_SK_LOOKUP, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000688d40, func_info_cnt=1, line_info_rec_size=16, line_info=0xc002cc4000, line_info_cnt=27, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 8
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\30\1\0\0\30\1\0\0\230\0\0\0\1\0\0\0\0\0\0\10"..., btf_log_buf=NULL, btf_size=456, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_HASH, key_size=4, value_size=16, max_entries=1024, map_flags=0, inner_map_fd=0, map_name="addr_map", map_ifindex=0, btf_fd=6, btf_key_type_id=1, btf_value_type_id=2, btf_vmlinux_value_type_id=0, map_extra=0}, 80) = 9
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\330\1\0\0\330\1\0\0\230\2\0\0\177\1\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1160, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=44, insns=0xc002cd4000, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_bind_ente", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000688dc0, func_info_cnt=1, line_info_rec_size=16, line_info=0xc00000e700, line_info_cnt=13, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 10
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0P\1\0\0P\1\0\0\240\0\0\0\1\0\0\0\0\0\0\10"..., btf_log_buf=NULL, btf_size=520, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_HASH, key_size=4, value_size=92, max_entries=1024, map_flags=0, inner_map_fd=0, map_name="conn_info_map", map_ifindex=0, btf_fd=6, btf_key_type_id=1, btf_value_type_id=2, btf_vmlinux_value_type_id=0, map_extra=0}, 80) = 11
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\20\1\0\0\20\1\0\0\260\0\0\0\1\0\0\0\0\0\0\10"..., btf_log_buf=NULL, btf_size=472, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_HASH, key_size=4, value_size=16, max_entries=1024, map_flags=0, inner_map_fd=0, map_name="bind_map", map_ifindex=0, btf_fd=6, btf_key_type_id=1, btf_value_type_id=2, btf_vmlinux_value_type_id=0, map_extra=0}, 80) = 12
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0|\1\0\0|\1\0\0\327\0\0\0\1\0\0\0\0\0\0\10"..., btf_log_buf=NULL, btf_size=619, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_HASH, key_size=4, value_size=28, max_entries=1024, map_flags=0, inner_map_fd=0, map_name="bind6_map", map_ifindex=0, btf_fd=6, btf_key_type_id=1, btf_value_type_id=2, btf_vmlinux_value_type_id=0, map_extra=0}, 80) = 13
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\4\1\0\0\4\1\0\0\343\6\0\0.\6\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=2047, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=194, insns=0xc00018c700, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_bind_exit", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000688f00, func_info_cnt=1, line_info_rec_size=16, line_info=0xc0000aa400, line_info_cnt=63, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 14
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\304\1\0\0\304\1\0\0P\3\0\0/\2\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1324, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=60, insns=0xc002ce8000, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_connect_e", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000688f80, func_info_cnt=1, line_info_rec_size=16, line_info=0xc002cc4400, line_info_cnt=18, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 15
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\260\0\0\0\260\0\0\0\300\5\0\0(\5\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1672, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=354, insns=0xc002cf2000, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_connect_e", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000689080, func_info_cnt=1, line_info_rec_size=16, line_info=0xc0000aa800, line_info_cnt=52, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 16
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0T\1\0\0T\1\0\0001\1\0\0\256\0\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=669, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=7, insns=0xc000689180, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_enter_get", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000689100, func_info_cnt=1, line_info_rec_size=16, line_info=0xc000689140, line_info_cnt=3, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 17
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0T\1\0\0T\1\0\0\231\0\0\0\31\0\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=517, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=2, insns=0xc002ce7e80, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_enter_soc", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc0006891c0, func_info_cnt=1, line_info_rec_size=16, line_info=0xc000689200, line_info_cnt=1, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 18
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0@\2\0\0@\2\0\08\3\0\0\306\1\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1424, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=52, insns=0xc002d00000, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_recvfrom_", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000689240, func_info_cnt=1, line_info_rec_size=16, line_info=0xc00000ef00, line_info_cnt=15, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 19
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\274\0\0\0\274\0\0\0\254\5\0\0\r\5\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1664, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=354, insns=0xc002cf2c00, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_recvfrom_", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000689300, func_info_cnt=1, line_info_rec_size=16, line_info=0xc0000aac00, line_info_cnt=49, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 20
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0004\2\0\0004\2\0\0/\3\0\0\302\1\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1403, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=52, insns=0xc002d001a0, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_sendto_en", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000689380, func_info_cnt=1, line_info_rec_size=16, line_info=0xc00000f100, line_info_cnt=15, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 21
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0\314\0\0\0\314\0\0\0\241\5\0\0\1\5\0\0\1\0\0\f"..., btf_log_buf=NULL, btf_size=1669, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_PROG_LOAD, {prog_type=BPF_PROG_TYPE_TRACEPOINT, insn_cnt=350, insns=0xc002cf3800, license="Dual BSD/GPL", log_level=0, log_size=0, log_buf=NULL, kern_version=KERNEL_VERSION(0, 0, 0), prog_flags=0, prog_name="trace_sendto_ex", prog_ifindex=0, expected_attach_type=BPF_CGROUP_INET_INGRESS, prog_btf_fd=6, func_info_rec_size=8, func_info=0xc000689480, func_info_cnt=1, line_info_rec_size=16, line_info=0xc0000ab000, line_info_cnt=47, attach_btf_id=0, attach_prog_fd=0, fd_array=NULL}, 152) = 22
bpf(BPF_BTF_LOAD, {btf="\237\353\1\0\30\0\0\0\0\0\0\0@\1\0\0@\1\0\0\225\0\0\0\1\0\0\0\1\0\0\17"..., btf_log_buf=NULL, btf_size=493, btf_log_size=0, btf_log_level=0}, 40) = 6
bpf(BPF_MAP_CREATE, {map_type=BPF_MAP_TYPE_ARRAY, key_size=4, value_size=8, max_entries=1, map_flags=BPF_F_MMAPABLE, inner_map_fd=0, map_name=".bss", map_ifindex=0, btf_fd=6, btf_key_type_id=0, btf_value_type_id=1, btf_vmlinux_value_type_id=0, map_extra=0}, 80) = 23
bpf(BPF_MAP_UPDATE_ELEM, {map_fd=23, key=0xc002d1a5d0, value=0xc0000a54a0, flags=BPF_ANY}, 32) = 0
Дескриптор нового namespace: 6
bpf(BPF_LINK_CREATE, {link_create={prog_fd=-1, target_fd=-1, attach_type=BPF_CGROUP_INET_INGRESS, flags=0}}, 64) = -1 EBADF (Bad file descriptor)
bpf(BPF_LINK_CREATE, {link_create={prog_fd=8, target_fd=6, attach_type=BPF_SK_LOOKUP, flags=0}}, 64) = 7
--- SIGURG {si_signo=SIGURG, si_code=SI_TKILL, si_pid=4858, si_uid=0} ---
2025/03/27 18:38:22 opening tracepoint sys_enter_getsockname: cannot create bpf perf link: permission denied
+++ exited with 1 +++
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 







