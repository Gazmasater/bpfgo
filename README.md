SEC("lsm/socket_sendmsg")
int BPF_PROG(socket_sendmsg, struct socket *sock, struct msghdr *msg, int size)
{
    bpf_printk("Sending message of size %d\n", size);
    return 0; // вернуть ненулевое значение, чтобы заблокировать операцию
}

