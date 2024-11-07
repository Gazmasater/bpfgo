#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

// Структура для хранения информации о соединении
struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 sport;
    u16 dport;
    char comm[16];
    struct file *file;
};

// Хеш-карта для хранения информации о соединениях
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// kprobe для фиксации PID и указателя на file при вызове accept
SEC("kprobe/__sys_accept4")
int trace_accept(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct file *file = (struct file *)PT_REGS_PARM1(ctx);
    if (!file) {
        bpf_printk("trace_accept: file is NULL\n");
        return 0;
    }

    // Создаем и заполняем структуру conn_info_t
    struct conn_info_t info = {};
    info.pid = pid;
    info.file = file;
    bpf_get_current_comm(info.comm, sizeof(info.comm));

    // Сохраняем информацию о процессе в карту
    int res = bpf_map_update_elem(&conn_info_map, &pid, &info, BPF_ANY);
    if (res == 0) {
        bpf_printk("trace_accept: PID=%d, Comm=%s\n", pid, info.comm);
    } else {
        bpf_printk("Failed to update conn_info_map for PID=%d\n", pid);
    }

    return 0;
}

// kretprobe для извлечения информации о соединении после успешного вызова accept
SEC("kretprobe/__sys_accept4")
int trace_accept_ret(struct pt_regs *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);

    // Проверяем результат вызова accept
    if (ret < 0) {
        // Ошибка соединения, удаляем запись из карты
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_printk("Accept failed, removed connection info for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем сохраненную информацию из карты
    struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!info || !info->file) {
        bpf_printk("No connection info found for PID=%d or file is NULL\n", pid);
        return 0;
    }

    // Получаем указатель на socket и структуру sock
    struct socket *sock;
    if (bpf_core_read(&sock, sizeof(sock), &info->file->private_data) != 0 || !sock) {
        bpf_printk("Failed to read file->private_data in kretprobe\n");
        return 0;
    }

    struct sock *sk;
    if (bpf_core_read(&sk, sizeof(sk), &sock->sk) != 0 || !sk) {
        bpf_printk("Failed to read sock->sk in kretprobe\n");
        return 0;
    }

    // Извлекаем IP и порты
    u32 dip;
    u32 portpair;
    if (bpf_core_read(&dip, sizeof(dip), &sk->__sk_common.skc_rcv_saddr) != 0) {
        bpf_printk("Failed to read sk->__sk_common.skc_rcv_saddr in kretprobe\n");
        return 0;
    }
    if (bpf_core_read(&portpair, sizeof(portpair), &sk->__sk_common.skc_portpair) != 0) {
        bpf_printk("Failed to read sk->__sk_common.skc_portpair in kretprobe\n");
        return 0;
    }

    // Заполняем информацию о IP и портах
    info->ip = dip;
    info->sport = bpf_ntohs(portpair >> 16);
    info->dport = bpf_ntohs(portpair & 0xFFFF);

    // Выводим информацию о соединении
    bpf_printk("Connection established: PID=%d, Comm=%s, IP=%u.%u.%u.%u, Sport=%u, Dport=%u\n",
               info->pid, info->comm,
               (info->ip >> 0) & 0xFF, (info->ip >> 8) & 0xFF,
               (info->ip >> 16) & 0xFF, (info->ip >> 24) & 0xFF,
               info->sport, info->dport);

    // Удаляем информацию из карты после использования
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}


________________________________________________________________________________

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x9109 symtab 0x9148 relocs 0x9190 sh_offset 0x9190
libbpf: prog 'trace_accept_ret': BPF program load failed: Invalid argument
libbpf: prog 'trace_accept_ret': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; int trace_accept_ret(struct pt_regs *ctx) {
0: (bf) r6 = r1                       ; R1=ctx() R6_w=ctx()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
1: (85) call bpf_get_current_pid_tgid#14      ; R0_w=scalar()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
2: (77) r0 >>= 32                     ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff))
; u32 pid = bpf_get_current_pid_tgid() >> 32;
3: (63) *(u32 *)(r10 -4) = r0         ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R10=fp0 fp-8=mmmm????
; long ret = PT_REGS_RC(ctx);
4: (79) r1 = *(u64 *)(r6 +80)         ; R1_w=scalar() R6_w=ctx()
; if (ret < 0) {
5: (65) if r1 s> 0xffffffff goto pc+11 17: R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R1_w=scalar(smin=0,umax=0x7fffffffffffffff,var_off=(0x0; 0x7fffffffffffffff)) R6_w=ctx() R10=fp0 fp-8=mmmm????
; bpf_printk("Accept failed, removed connection info for PID=%d\n", pid);
17: (bf) r2 = r10                     ; R2_w=fp0 R10=fp0
; struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
18: (07) r2 += -4                     ; R2_w=fp-4
19: (18) r1 = 0xffff970e0c38ec00      ; R1_w=map_ptr(map=conn_info_map,ks=4,vs=40)
21: (85) call bpf_map_lookup_elem#1   ; R0=map_value_or_null(id=1,map=conn_info_map,ks=4,vs=40)
; if (!info || !info->file) {
22: (15) if r0 == 0x0 goto pc+2       ; R0=map_value(map=conn_info_map,ks=4,vs=40)
; if (!info || !info->file) {
23: (79) r1 = *(u64 *)(r0 +32)        ; R0=map_value(map=conn_info_map,ks=4,vs=40) R1_w=scalar()
; if (!info || !info->file) {
24: (55) if r1 != 0x0 goto pc+6 31: R0=map_value(map=conn_info_map,ks=4,vs=40) R1_w=scalar(umin=1) R6=ctx() R10=fp0 fp-8=mmmm????
