lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


ls /sys/kernel/debug/tracing/events/syscalls/ | grep getaddrinfo
ls /sys/kernel/debug/tracing/events/syscalls/ | grep socket


SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    conn_info.fd=ctx->fd;

    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
  
    struct sockaddr *addr = (struct sockaddr *)ctx->uservaddr; 

    struct sockaddr_in *addrin = (struct sockaddr_in *)ctx->uservaddr;
    conn_info.sport=bpf_ntohs(conn_info.sport);
    
    bpf_printk("sys_enter_connect PID=%d FD=%d PORT=%d",conn_info.pid,conn_info.fd,conn_info.sport);


    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);

    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;







