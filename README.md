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











