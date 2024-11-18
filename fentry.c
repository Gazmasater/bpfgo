#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t {
    u32 pid;
    u32 src_ip;
    u32 dst_ip;

    u16 sport;
    u16 dport;
    char comm[16];
    struct sockaddr *sock_addr;   
    u32 addrlen;           
};

struct sys_enter_accept4_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    int fd;
    struct sockaddr *upeer_sockaddr;
    int *upeer_addrlen;
    int flags;
};

struct sys_exit_accept4_args {
    unsigned short common_type;
    unsigned char common_flags;
    unsigned char common_preempt_count;
    int common_pid;
    int __syscall_nr;
    long ret;
};

struct sys_enter_bind_args{

        unsigned short common_type;
        unsigned char common_flags;
        unsigned char common_preempt_count;      
        int common_pid;        
        int __syscall_nr;     
        int fd;       
        struct sockaddr * umyaddr;      
        int addrlen;     
};

struct sys_exit_bind_args{

        unsigned short common_type;
        unsigned char common_flags;
        unsigned char common_preempt_count;
        int common_pid;
        int __syscall_nr; 
        long ret;  

};

struct  sys_enter_connec_argst {

        unsigned short common_type;
        unsigned char common_flags;
        unsigned char common_preempt_count;
        int common_pid;
        int __syscall_nr; 
        int fd;
        struct sockaddr * uservaddr;      
        int addrlen;

};

struct sys_exit_connect_args {

        unsigned short common_type;
        unsigned char common_flags;
        unsigned char common_preempt_count;
        int common_pid;
        int __syscall_nr; 
        int fd;
        struct sockaddr * uservaddr;
        int addrlen;    

};

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_accept SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_bind SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);
    __type(value, struct conn_info_t);
} conn_info_map_c SEC(".maps");

char LICENSE[] SEC("license") = "Dual BSD/GPL";
#define AF_INET 2

// static __always_inline int init_conn_info_accept(u32 pid, struct pt_regs *ctx) {
//     struct conn_info_t conn_info = {};
//     conn_info.pid = pid;
//     bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
//     conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
//     conn_info.addrlen = PT_REGS_PARM3(ctx);
//     bpf_map_update_elem(&conn_info_map_accept, &pid, &conn_info, BPF_ANY);
//     return 0;
// }

// static __always_inline int init_conn_info_accept(struct sys_enter_accept4_args *ctx) {
//     struct conn_info_t conn_info = {};
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     conn_info.pid = pid;
//     bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
//     conn_info.sock_addr = (struct sockaddr *)ctx->upeer_sockaddr;
//     bpf_map_update_elem(&conn_info_map_accept, &pid, &conn_info, BPF_ANY);
//     return 0;
// }


// static __always_inline int init_conn_info_bind(struct pt_regs *ctx) {
//     struct conn_info_t conn_info = {};
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     conn_info.pid = pid;
//     bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
//     conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
//     bpf_map_update_elem(&conn_info_map_bind, &pid, &conn_info, BPF_ANY);
//     return 0;
// }

static __always_inline int init_conn_info_bind(struct sys_enter_bind_args *ctx) {
    struct conn_info_t conn_info = {};
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = (struct sockaddr *)ctx->umyaddr;
    bpf_map_update_elem(&conn_info_map_bind, &pid, &conn_info, BPF_ANY);
    return 0;
}



// static __always_inline int init_conn_info_connect(struct pt_regs *ctx) {
//     struct conn_info_t conn_info = {};
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     conn_info.pid = pid;
//     bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
//     conn_info.sock_addr = (struct sockaddr *)PT_REGS_PARM2(ctx);
//     conn_info.addrlen = PT_REGS_PARM3(ctx);
//     bpf_map_update_elem(&conn_info_map_c, &pid, &conn_info, BPF_ANY);
//     return 0;
// }

// static __always_inline int init_conn_info_connect(struct sys_enter_connec_argst *ctx) {
//     struct conn_info_t conn_info = {};
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     conn_info.pid = pid;
//     bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
//     conn_info.sock_addr = (struct sockaddr *)ctx->uservaddr;
//     bpf_map_update_elem(&conn_info_map_c, &pid, &conn_info, BPF_ANY);
//     return 0;
// }



// SEC("kprobe/__sys_accept4")
// int trace_accept4_entry(struct pt_regs *ctx) {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     init_conn_info_accept(ctx);

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid); 
//     if (conn_info) 
//     { bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", pid, conn_info->comm); }
    
//     return 0;
// }





// SEC("kprobe/__sys_accept4")
// int trace_accept4_entry(struct pt_regs *ctx) {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     init_conn_info_ab(pid , ctx);

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid); 
//     if (conn_info) 
//   //  { bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", pid, conn_info->comm); }
    
//     return 0;
// }

// // kretprobe для завершения извлечения информации о соединении
// SEC("kretprobe/__sys_accept4")
// int trace_accept4_ret(struct pt_regs *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = PT_REGS_RC(ctx); // Получаем результат вызова

//     // Если результат отрицательный, значит произошла ошибка
//     if (ret < 0) {
//         bpf_printk("Accept4 failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map_ab, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_ab, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for PID=%d\n", pid);
//         return 0;
//     }

//     // Получаем IP и порт клиента из sockaddr, используя сохраненный указатель
//     struct sockaddr_in addr;

//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     // Извлекаем IP и порт из sockaddr_in, если это IPv4-соединение
//     if (addr.sin_family == AF_INET) {
//         conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr); // Преобразуем IP к порядку хоста
//         conn_info->sport = bpf_ntohs(addr.sin_port);      // Преобразуем порт к порядку хоста
        
//         bpf_printk("SERVER Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
//             (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
//     }




//     return 0;
// }


// // kprobe для фиксации начальных данных процесса и дескриптора файла
// SEC("kprobe/__sys_bind")
// int trace_bind_entry(struct pt_regs *ctx) {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;
//     init_conn_info_bind(ctx);

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid); 
//     if (conn_info) 
//     { bpf_printk("SERVER Bind entry: PID=%d, Comm=%s\n", pid, conn_info->comm); }

//     return 0;
// }


// kprobe для фиксации начальных данных процесса и дескриптора файла
SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_entry(struct sys_enter_accept4_args *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;
    init_conn_info_bind(ctx);

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid); 
    if (conn_info) 
    {
        bpf_printk("sys_enter_bind  sock_addr=%d",conn_info->sock_addr);

        bpf_printk("trace SERVER Bind entry: PID=%d, Comm=%s\n", pid, *conn_info->comm); }
    return 0;
}


// // kretprobe для завершения извлечения информации о соединении
// SEC("kretprobe/__sys_bind")
// int trace_bind_ret(struct pt_regs *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = PT_REGS_RC(ctx); // Получаем результат вызова

//     // Если результат отрицательный, значит произошла ошибка
//     if (ret < 0) {
//         bpf_printk("__sys_bind Accept4 failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map_accept, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
//     if (!conn_info) {
//         bpf_printk("__sys_bind No connection info found for PID=%d\n", pid);
//         return 0;
//     }

//     // Получаем IP и порт клиента из sockaddr, используя сохраненный указатель
//     struct sockaddr_in addr;

//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("__sys_bind Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     // Извлекаем IP и порт из sockaddr_in, если это IPv4-соединение
//     if (addr.sin_family == AF_INET) {
//         conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr); // Преобразуем IP к порядку хоста
//         conn_info->dport = bpf_ntohs(addr.sin_port);      // Преобразуем порт к порядку хоста
        
//         bpf_printk("__sys_bind SERVER Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
//             (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
//     }


//     return 0;
// }


// kretprobe для завершения извлечения информации о соединении
SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_ret(struct sys_exit_bind_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret ;

    // Если результат отрицательный, значит произошла ошибка
    if (ret < 0) {
        bpf_printk("trace __sys_bind Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
    if (!conn_info) {
        bpf_printk("trace __sys_bind No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Получаем IP и порт клиента из sockaddr, используя сохраненный указатель
    struct sockaddr_in addr;

        bpf_printk("trace sys_exit_bind  sock_addr=%d",conn_info->sock_addr);


    if (bpf_probe_read_user(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("trace __sys_bind Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем IP и порт из sockaddr_in, если это IPv4-соединение
    if (addr.sin_family == AF_INET) {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr); // Преобразуем IP к порядку хоста
        conn_info->dport = bpf_ntohs(addr.sin_port);      // Преобразуем порт к порядку хоста
        
        bpf_printk("trace __sys_bind SERVER Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
            conn_info->pid, conn_info->comm,
            (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
            (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
    }


    return 0;
}





// kprobe для фиксации начальных данных процесса при вызове connect
// SEC("kprobe/__sys_connect")
// int trace_connect_entry(struct pt_regs *ctx) {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;

//     init_conn_info_c(ctx);

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_c, &pid);
//     if (conn_info) {
//   //    bpf_printk("!!!!!!CLIENT Connect entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
//     }

//     return 0;
// }

// // kretprobe для завершения обработки соединения
// SEC("kretprobe/__sys_connect")
// int trace_connect_ret(struct pt_regs *ctx) {
//     u32 pid = bpf_get_current_pid_tgid() >> 32;
//     long ret = PT_REGS_RC(ctx); // Получаем результат вызова

//     // Если результат отрицательный, значит произошла ошибка
//     if (ret < 0) {
//         bpf_printk("Connect failed for PID=%d\n", pid);
//         bpf_map_delete_elem(&conn_info_map_c, &pid);
//         return 0;
//     }

//     struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_c, &pid);
//     if (!conn_info) {
//         bpf_printk("No connection info found for PID=%d\n", pid);
//         return 0;
//     }

//     struct sockaddr_in addr;
//     if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
//         bpf_printk("Failed to read sockaddr for PID=%d\n", pid);
//         return 0;
//     }

//     if (addr.sin_family == AF_INET) {
//         conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
//         conn_info->dport = bpf_ntohs(addr.sin_port);

//         bpf_printk("!!!!!CLIENT Connected to server: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
//             conn_info->pid, conn_info->comm,
//             (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
//             (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
//     }

//     return 0;
// }










