lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


ls /sys/kernel/debug/tracing/events/syscalls/ | grep getaddrinfo
ls /sys/kernel/debug/tracing/events/syscalls/ | grep socket


SEC("tracepoint/syscalls/trace_tcp_sendmsg")
int trace_tcp_sendmsg(struct trace_event_raw_tcp_sendmsg *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;  // Получаем PID
    u16 port = ctx->sport;  // Исходный порт
    u32 src_ip = ctx->saddr;  // Исходный IP

    bpf_printk("TCP sendmsg PID=%d src_ip=%x src_port=%d", pid, src_ip, port);

    return 0;
}








