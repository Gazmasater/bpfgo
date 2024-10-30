#include <linux/bpf.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/bpf_helpers.h>
#include <linux/ip.h>
#include <linux/ptrace.h>

SEC("fentry/__x64_sys_recvfrom")
int BPF_PROG(recvfrom_hook, int sockfd, void __user *buf, size_t len, unsigned int flags, struct sockaddr __user *src_addr, int __user *addrlen) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)src_addr;

    if (addr_in) {
        bpf_printk("UDP packet received from %pI4:%u\n", &addr_in->sin_addr, ntohs(addr_in->sin_port));
    }

    return 0; // возвращаем 0 для продолжения выполнения функции
}

char _license[] SEC("license") = "GPL";




#include <linux/bpf.h>
#include <linux/socket.h>
#include <linux/inet.h>
#include <linux/bpf_helpers.h>

SEC("fentry/__x64_sys_accept4")
int BPF_PROG(accept_hook, int sockfd, struct sockaddr __user *addr, int __user *addrlen, int flags) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;

    // Проверяем, что указатель адреса валиден
    if (addr_in) {
        bpf_printk("New TCP connection accepted from %pI4:%u\n", &addr_in->sin_addr, ntohs(addr_in->sin_port));
    }

    return 0; // возвращаем 0 для продолжения выполнения функции
}

char _license[] SEC("license") = "GPL";



#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u32);    // UID процесса
    __type(value, u32);  // ID сокета
    __uint(max_entries, 1024);
} socket_map SEC(".maps");

SEC("fentry/__sys_socket")
int handle_socket_creation(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    u32 socket_type = PT_REGS_PARM1(ctx);  // Тип сокета, например SOCK_STREAM
    u32 protocol = PT_REGS_PARM3(ctx);     // Протокол, например IPPROTO_TCP

    // Логика фильтрации: захватываем только TCP-сокеты
    if (protocol == IPPROTO_TCP) {
        bpf_map_update_elem(&socket_map, &pid, &socket_type, BPF_ANY);
    }

    return 0;
}

char _license[] SEC("license") = "GPL";


