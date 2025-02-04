//go:build ignore

#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"


#include <bpf/bpf_helpers.h>

struct conn_info_t
{
	struct sockaddr *sock_addr;
	u32 pid;
	u32 src_ip;
	u32 dst_ip;
	u32 addrlen;
	u16 sport;
	u16 dport;
	u8 protocol; 
	char comm[16];
	
};



struct sys_enter_sendto_args
{

        unsigned short common_type;       
        unsigned char common_flags;       
        unsigned char common_preempt_count;      
        int common_pid;   
        int __syscall_nr; 
        int fd;  
		int pad1;
        void * buff;     
        size_t len;      
        unsigned int flags;       
        struct sockaddr * addr;   
        int addr_len;   

};

struct sys_exit_sendto_args
{

		unsigned short common_type;      
        unsigned char common_flags;     
        unsigned char common_preempt_count;   
        int common_pid;   
        int __syscall_nr; 
		int pad1;
        long ret; 

};


struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_sc SEC(".maps");


struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_ra SEC(".maps");




// struct
// {
// 	__uint(type, BPF_MAP_TYPE_PERCPU_HASH);
// 	__uint(max_entries, 1024);
// 	__type(key, u32);
// 	__type(value, struct conn_info_t);
// } conn_info_map_accept SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2


static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;
    bpf_map_update_elem(map, &pid, &conn_info, BPF_ANY);
    return 0;
}

static __always_inline int init_conn_info_sendto(struct sys_enter_sendto_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map_sc, bpf_get_current_pid_tgid() >> 32);
}




 SEC("tracepoint/syscalls/sys_enter_sendto")
 int trace_sendto_enter(struct sys_enter_sendto_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_sendto(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
	if (conn_info)
	{
		
		bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);

	}

 	return 0;
 }


SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    bpf_printk("sys_exit_sendto PID=%d\n", pid);


    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct task_struct *task;
    struct files_struct *files;
    struct fdtable *fdt;
    struct file **fd_array;
    struct file *file;
    struct socket *sock;
    struct sock *sk;

    // Получаем task_struct
    bpf_core_read(&task, sizeof(task), (void *)bpf_get_current_task());
    if (!task) {
        bpf_printk("UDP sys_exit_sendto: Failed to read task_struct for PID=%d\n", pid);
        return 0;
    }

    // Достаем files_struct
    bpf_core_read(&files, sizeof(files), &task->files);
    if (!files) {
        bpf_printk("UDP sys_exit_sendto: Failed to read files_struct for PID=%d\n", pid);
        return 0;
    }

    // Получаем fdtable
    bpf_core_read(&fdt, sizeof(fdt), &files->fdt);
    if (!fdt) {
        bpf_printk("UDP sys_exit_sendto: Failed to read fdtable for PID=%d\n", pid);
        return 0;
    }

    // Читаем массив файловых дескрипторов
    bpf_core_read(&fd_array, sizeof(fd_array), &fdt->fd);
    if (!fd_array) {
        bpf_printk("UDP sys_exit_sendto: Failed to read fd_array for PID=%d\n", pid);
        return 0;
    }

    // Берем file по дескриптору (например, 3)
    bpf_core_read(&file, sizeof(file), &fd_array[3]);
    if (!file) {
        bpf_printk("UDP sys_exit_sendto: Failed to read file from fd_array for PID=%d\n", pid);
        return 0;
    }

    // Получаем socket
    bpf_core_read(&sock, sizeof(sock), &file->private_data);
    if (!sock) {
        bpf_printk("UDP sys_exit_sendto: Failed to read socket for PID=%d\n", pid);
        return 0;
    }

    // Получаем sock
    bpf_core_read(&sk, sizeof(sk), &sock->sk);
    if (!sk) {
        bpf_printk("UDP sys_exit_sendto: sk is NULL for PID=%d\n", pid);
        return 0;
    }

    // Читаем src_ip и src_port
    struct inet_sock *inet = (struct inet_sock *)sk;
    if (!inet) {
        bpf_printk("UDP sys_exit_sendto: inet is NULL for PID=%d\n", pid);
        return 0;
    }

    u32 src_ip = 0;
    u16 src_port = 0;

    bpf_core_read(&src_ip, sizeof(src_ip), &inet->inet_saddr);
    bpf_core_read(&src_port, sizeof(src_port), &inet->inet_sport);

    // Проверяем, что IP считался корректно
    bpf_printk("UDP sys_exit_sendto: Read src_ip raw = 0x%x\n", src_ip);

    // Выводим IP в человекочитаемом формате
    bpf_printk("UDP sys_exit_sendto: PID=%d, src_ip=%d.%d.%d.%d\n", 
           pid, 
           (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF, 
           (src_ip >> 8) & 0xFF, src_ip & 0xFF); // Преобразуем порт в host byte order

    // Обновляем данные
    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);

    return 0;
}
