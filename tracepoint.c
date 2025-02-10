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

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10240);
    __type(key, u32);
    __type(value, int);
} fd_map SEC(".maps");





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
    int fd = ctx->fd;  // fd передается первым аргументом в sendto()
    
    bpf_map_update_elem(&fd_map, &pid, &fd, BPF_ANY);

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
     struct task_struct *task;
     //struct socket *sock;
     struct sock *sk;
     struct inet_sock *inet;
     __be32 src_ip;
     __be16 sport;
     int *fd_ptr;
     char comm[16];
     u32 pid = bpf_get_current_pid_tgid() >> 32;
 
     // Получаем fd из eBPF-карты
     fd_ptr = bpf_map_lookup_elem(&fd_map, &pid);
     if (!fd_ptr) return 0;
     
     int fd = *fd_ptr;
     bpf_map_delete_elem(&fd_map, &pid); // Удаляем запись, чтобы не засорять карту
 
     // Получаем текущий процесс
     task = (struct task_struct *)bpf_get_current_task();
     if (!task) return 0;

     u32 pid2=BPF_CORE_READ(task,pid);

     // Получаем files_struct
    struct files_struct *files = BPF_CORE_READ(task, files);
    if (!files) return 0;

    struct fdtable *fdt = BPF_CORE_READ(files, fdt);
    if (!fdt) return 0;

    struct file **file_ptr = BPF_CORE_READ(fdt, fd);
    if (!file_ptr) return 0;
    
    struct file *file;
    bpf_core_read_user(&file, sizeof(file), file_ptr); // Читаем указатель из пользовательской памяти
    if (!file) return 0;

    // struct socket *sock;
    // if (bpf_core_read(&sock, sizeof(sock), &file->private_data))
    //     return 0;
    
    


     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
     if (!conn_info) {
         bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
         bpf_map_delete_elem(&conn_info_map_sc, &pid);
         return 0;
     }

 
     //bpf_printk("sys_exit_sendto Private_data=%p NAME=%s PID=%d PID2=%d src_ip=%x sport=%d\n",file->private_data,conn_info->comm, pid, pid2,src_ip, sport);
     bpf_printk("sys_exit_sendto  NAME=%s PID=%d PID2=%d src_ip=%x sport=%d\n",conn_info->comm, pid, pid2,src_ip, sport);

     return 0;
 }
 