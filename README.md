lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


#include <linux/ptrace.h>
#include <linux/socket.h>
#include <linux/fs.h>

int trace_connect(struct pt_regs *ctx, struct socket *sock, struct sockaddr *addr, int addrlen) {
    u32 pid = bpf_get_current_pid_tgid();  // Получаем PID текущего процесса
    if (!sock) {
        return 0;
    }

    // Получаем файловый дескриптор, используя sock->sk и sock->sk_socket
    struct file *file = sock->sk->sk_socket->file;
    if (file) {
        u32 fd = file->f_inode->i_ino;  // Извлекаем inode из файла
        // Логика работы с fd, например, сохраняем в хэш-таблицу или используем в других вычислениях
    }

    return 0;
}



