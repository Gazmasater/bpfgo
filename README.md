lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l



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











