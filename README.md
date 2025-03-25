lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


tcp   ESTAB 0      0                  192.168.1.71:48402     34.107.243.93:443   users:(("firefox",pid=2876,fd=133))

#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/types.h>

BPF_HASH(socket_fds, u32, u32);  // Словарь для хранения fd по PID

int trace_connect(struct pt_regs *ctx, struct socket *sock, struct sockaddr *addr, int addrlen) {
    u32 pid = bpf_get_current_pid_tgid();  // Получаем PID текущего процесса
    u32 fd = sock->file->f_inode->i_ino;  // Получаем fd через inode

    socket_fds.update(&pid, &fd);  // Сохраняем fd для текущего PID

    return 0;
}


