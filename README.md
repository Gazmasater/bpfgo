lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};
    int fd = ctx->fd;

    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем fd, но не сохраняем указатель uservaddr
    bpf_map_update_elem(&fd_map, &pid, &fd, BPF_ANY);
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int *fd = bpf_map_lookup_elem(&fd_map, &pid);
    if (!fd) {
        return 0;
    }

    // Получаем conn_info
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (conn_info) {
        conn_info->retval = ctx->ret;
        bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);
    }

    // Удаляем fd из карты, чтобы не засорять память
    bpf_map_delete_elem(&fd_map, &pid);
    
    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir "$(pwd)" -tags linux -type trace_info -go-package=main -target amd64 bpf "$(pwd)/trace.c" -- -I"$(pwd)"
/home/gaz358/myprog/bpfgo/trace.c:426:14: warning: expression result unused [-Wunused-value]
  426 |         info.fd_connect;
      |         ~~~~ ^~~~~~~~~~
1 warning generated.
Compiled /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.go





