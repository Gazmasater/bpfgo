#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

struct conn_info_t
{
	u32 pid;
	u32 src_ip;
	u32 dst_ip;

	u16 sport;
	u16 dport;
	char comm[16];
	struct sockaddr *sock_addr;
	u32 addrlen;
};

struct socket_info {
    int domain;
    int type;
    int protocol;
    char comm[16];
};

struct sys_enter_socket_args

{

        unsigned short common_type;   //2   
        unsigned char common_flags;    //1   
        unsigned char common_preempt_count;  //1  
        int common_pid;   //4
        int __syscall_nr; //4s
		int pad1;//4
        int family;    //4  
        int type; //4
        int protocol;    //4 

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

struct sys_enter_recvfrom_args {
    unsigned short common_type;           
    unsigned char common_flags;           
    unsigned char common_preempt_count;   
    int common_pid;                       
    int __syscall_nr;                     
    int pad1;                            
    int fd;                               
    void *ubuf;                           
    size_t size;                          
    unsigned int flags;                                              
    struct sockaddr *addr;                
    int *addr_len;                        
} ;

struct sys_exit_recvfrom_args {
    unsigned short common_type;           // offset: 0, size: 2s
    unsigned char common_flags;           // offset: 2, size: 1
    unsigned char common_preempt_count;   // offset: 3, size: 1
    int common_pid;                       // offset: 4, size: 4
    int __syscall_nr;                     // offset: 8, size: 4
    int pad1;                             // padding to align the next field
    int ret;                             // offset: 16, size: 8
} ;


// struct sys_enter_accept_args
// {
// 	unsigned short common_type;
// 	unsigned char common_flags;
// 	unsigned char common_preempt_count;
// 	int common_pid;
// 	int __syscall_nr;
// 	int fd;
// 	int __padding;
// 	struct sockaddr *upeer_sockaddr;
// 	int *upeer_addrlen;
// };


struct sys_enter_accept4_args
{
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	int fd;
	int pad1;
	struct sockaddr *upeer_sockaddr;
	int *upeer_addrlen;
	int flags;
};

struct sys_exit_accept4_args
{
	unsigned short common_type;//2
	unsigned char common_flags;//1
	unsigned char common_preempt_count;//1
	int common_pid;//4
	int __syscall_nr;//4
	int pad1;
	long ret;//8
};

struct sys_enter_bind_args
{

	unsigned short common_type;//2
	unsigned char common_flags;//1
	unsigned char common_preempt_count;//1
	int common_pid;//4
	int __syscall_nr;//4
	int fd;//4
	int pad1;//4
	struct sockaddr *umyaddr;//8
	int addrlen;//4

};

struct sys_exit_bind_args
{

	unsigned short common_type;//2
	unsigned char common_flags;//1
	unsigned char common_preempt_count;//1
	int common_pid;//4
	int __syscall_nr;//4
	int pad1;
	int ret;//4
};

struct sys_enter_connect_args
{

	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
	int __syscall_nr;
	long fd;
	struct sockaddr *uservaddr;
	int addrlen;
};

struct sys_exit_connect_args
{

unsigned short common_type;      
unsigned char common_flags;      
unsigned char common_preempt_count;    
int common_pid; 
int __syscall_nr; 
int ret; 

};

struct {
	__uint(type, BPF_MAP_TYPE_HASH);         
	__uint(max_entries, 1024);              
	__type(key, u32);                       
	__type(value, struct socket_info );      
} socket_map SEC(".maps");

struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_sendto SEC(".maps");



struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_recvfrom SEC(".maps");


// struct
// {
// 	__uint(type, BPF_MAP_TYPE_PERCPU_HASH);
// 	__uint(max_entries, 1024);
// 	__type(key, u32);
// 	__type(value, struct conn_info_t);
// } conn_info_map_accept SEC(".maps");

struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_accept_four SEC(".maps");

struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_bind SEC(".maps");

struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_bind_udp SEC(".maps");


struct
{
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 1024);
	__type(key, u32);
	__type(value, struct conn_info_t);
} conn_info_map_connect SEC(".maps");

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
    return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map_sendto, bpf_get_current_pid_tgid() >> 32);
}


static __always_inline int init_conn_info_recvfrom(struct sys_enter_recvfrom_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map_recvfrom, bpf_get_current_pid_tgid() >> 32);
}


// static __always_inline int init_conn_info_accept(struct sys_enter_accept_args *ctx)
// {
//     return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_accept, bpf_get_current_pid_tgid() >> 32);
// }



static __always_inline int init_conn_info_accept4(struct sys_enter_accept4_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->upeer_sockaddr, &conn_info_map_accept_four, bpf_get_current_pid_tgid() >> 32);
}


static __always_inline int init_conn_info_bind(struct sys_enter_bind_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->umyaddr, &conn_info_map_bind, bpf_get_current_pid_tgid() >> 32);
}

static __always_inline int init_conn_info_bind_udp(struct sys_enter_bind_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->umyaddr, &conn_info_map_bind_udp, bpf_get_current_pid_tgid() >> 32);
}



static __always_inline int init_conn_info_connect(struct sys_enter_connect_args *ctx)
{
    return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map_connect, bpf_get_current_pid_tgid() >> 32);
}


SEC("tracepoint/syscalls/sys_enter_socket")
int trace_socket(struct sys_enter_socket_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct socket_info sock_info = {};
    sock_info.type = ctx->type;
    sock_info.protocol = ctx->protocol;


    bpf_get_current_comm(sock_info.comm, sizeof(sock_info.comm));

    
    bpf_map_update_elem(&socket_map, &pid, &sock_info, BPF_ANY);

    return 0;
}


 SEC("tracepoint/syscalls/sys_enter_sendto")
 int trace_sendto_enter(struct sys_enter_sendto_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_recvfrom(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sendto, &pid);
	if (conn_info&&conn_info->comm[0]=='u')
	{
		
		bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);

	}

 	return 0;
 }


 SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	long ret = ctx->ret;

	if (ret < 0)
	{
		//bpf_printk("RET UDP SYS_exit_recvfrom failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_recvfrom, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_recvfrom, &pid);
	if (!conn_info)
	{
		bpf_printk("UDP sys_exit_sendto No connection info found for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_recvfrom, &pid);

		return 0;
	}

if (conn_info->comm[0]=='u'&&conn_info->comm[1]=='d') {
	bpf_printk("UDP sys_exit_sendto sockaddr=%p",conn_info->sock_addr);
	}

if (conn_info->comm[0]!='u'&&conn_info->comm[1]!='d'){

	return 0;
}


	struct sockaddr_in addr;

	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("UDP sys_exit_sendto Failed to read sockaddr for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_recvfrom, &pid);

		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->sport = bpf_ntohs(addr.sin_port);

		bpf_printk("UDP sys_exit_sendto  connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
				   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
	}

	    bpf_map_update_elem(&conn_info_map_recvfrom, &pid, conn_info, BPF_ANY);


	return 0;
}


 SEC("tracepoint/syscalls/sys_enter_recvfrom")
 int trace_recvfrom_enter(struct sys_enter_recvfrom_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_recvfrom(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_recvfrom, &pid);
	if (conn_info&&conn_info->comm[0]=='u')
	{
		
		bpf_printk("SERVER sys_enter_recvfrom: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);

	}

 	return 0;
 }


SEC("tracepoint/syscalls/sys_exit_recvfrom")
int trace_recvfrom_exit(struct sys_exit_recvfrom_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	long ret = ctx->ret;

	if (ret < 0)
	{
		//bpf_printk("RET UDP SYS_exit_recvfrom failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_recvfrom, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_recvfrom, &pid);
	if (!conn_info)
	{

		bpf_map_delete_elem(&conn_info_map_recvfrom, &pid);

		bpf_printk("UDP SYS_exit_recvfrom No connection info found for PID=%d\n", pid);
		return 0;
	}

if (conn_info->comm[0]=='u'&&conn_info->comm[1]=='d') {
	bpf_printk("UDP SYS_exit_recvfrom sockaddr=%p",conn_info->sock_addr);
	}



	struct sockaddr_in addr;

	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("UDP SYS_exit_recvfrom Failed to read sockaddr for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_recvfrom, &pid);

		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->sport = bpf_ntohs(addr.sin_port);

		bpf_printk("UDP SYS_exit_recvfrom  connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
				   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
	}

	    bpf_map_update_elem(&conn_info_map_recvfrom, &pid, conn_info, BPF_ANY);


	return 0;
}



// SEC("tracepoint/syscalls/sys_enter_accept")
// int trace_accept_enter(struct sys_enter_accept_args *ctx){
// 	u32 pid = bpf_get_current_pid_tgid() >> 32;
// 	init_conn_info_accept(ctx);

// 	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
// 	if (conn_info)
// 	{
		
// 		bpf_printk("SERVER accept entry: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);

// 	}

// 	return 0;
// }

// SEC("tracepoint/syscalls/sys_exit_accept")
// int trace_accept_exit(struct sys_exit_accept4_args *ctx){
// 	u32 pid = bpf_get_current_pid_tgid() >> 32;
// 	long ret = ctx->ret;

// 	if (ret < 0)
// 	{
// 		bpf_printk("EXIT_accept Accept failed for PID=%d\n", pid);
// 		bpf_map_delete_elem(&conn_info_map_accept, &pid);
// 		return 0;
// 	}

// 	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
// 	if (!conn_info)
// 	{
// 		bpf_printk("EXIT_accept No connection info found for PID=%d\n", pid);
// 		return 0;
// 	}

// 	bpf_printk("EXIT sockaddr=%p",conn_info->sock_addr);


// 	struct sockaddr_in addr;

// 	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
// 	{
// 		bpf_printk("EXIT_accept Failed to read sockaddr for PID=%d\n", pid);
// 		bpf_map_delete_elem(&conn_info_map_accept, &pid);

// 		return 0;
// 	}

// 	if (addr.sin_family == AF_INET)
// 	{
// 		conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
// 		conn_info->sport = bpf_ntohs(addr.sin_port);

// 		bpf_printk("EXIT_accept Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
// 				   conn_info->pid, conn_info->comm,
// 				   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
// 				   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
// 	}

//     bpf_map_update_elem(&conn_info_map_accept, &pid, conn_info, BPF_ANY);


// 	return 0;
// }



SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_bind_ret(struct sys_enter_accept4_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	init_conn_info_accept4(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
	if (conn_info&&conn_info->comm[0]=='n')
	{
		
		bpf_printk("SERVER accept4 entry: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);

	}

	return 0;
}


SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx){
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	long ret = ctx->ret;

	if (ret < 0)
	{
	//	bpf_printk("EXIT_accept Accept4 failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
	if (!conn_info)
	{
		bpf_printk("EXIT_accept4 No connection info found for PID=%d\n", pid);
		return 0;
	}

	if (conn_info->comm[0]=='n') {

	bpf_printk("EXIT_accept4 sockaddr=%p",conn_info->sock_addr);

	}


	struct sockaddr_in addr;

	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("EXIT_accept4 Failed to read sockaddr for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_accept_four, &pid);

		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->sport = bpf_ntohs(addr.sin_port);

		bpf_printk("EXIT_accept4 Accepted connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
				   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
	}

	    bpf_map_update_elem(&conn_info_map_accept_four, &pid, conn_info, BPF_ANY);


	return 0;
}

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct sys_enter_bind_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;

  // Проверяем, есть ли информация о сокете для текущего процесса
    struct socket_info *sock_info = bpf_map_lookup_elem(&socket_map, &pid);
    if (!sock_info) {
		bpf_map_delete_elem(&socket_map, &pid);

        return 0;
    }

    if (sock_info->type==2) {
        init_conn_info_bind_udp(ctx);
    } else {
        init_conn_info_bind(ctx);
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
    if (conn_info&&conn_info->comm[0]=='n') {
         bpf_printk("TCP Bind entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
    }

    struct conn_info_t *conn_info_udp = bpf_map_lookup_elem(&conn_info_map_bind_udp, &pid);
    if (conn_info_udp) {
        bpf_printk("UDP Bind entry: PID=%d, Comm=%s\n", pid, conn_info_udp->comm);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct sys_exit_bind_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_printk("EXIT_bind failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_bind, &pid);
        bpf_map_delete_elem(&conn_info_map_bind_udp, &pid); // Удаляем для UDP тоже
        return 0;
    }

    struct socket_info *sock_info = bpf_map_lookup_elem(&socket_map, &pid);
    if (!sock_info)
    {
        bpf_printk("EXIT_bind No socket info found for PID=%d\n", pid);
        return 0;
    }

    if (sock_info->type == 2) // Для UDP
    {
        struct conn_info_t *conn_info_udp = bpf_map_lookup_elem(&conn_info_map_bind_udp, &pid);
        if (!conn_info_udp)
        {
            bpf_printk("EXIT_bind No UDP connection info found for PID=%d\n", pid);
            return 0;
        }

        struct sockaddr_in addr;
        if (bpf_probe_read(&addr, sizeof(addr), conn_info_udp->sock_addr) != 0)
        {
            bpf_printk("EXIT_bind Failed to read sockaddr for UDP PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map_bind_udp, &pid);
            return 0;
        }

        if (addr.sin_family == AF_INET)
        {
            conn_info_udp->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info_udp->dport = bpf_ntohs(addr.sin_port);

            bpf_printk("EXIT_bind UDP connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                       conn_info_udp->pid, conn_info_udp->comm,
                       (conn_info_udp->dst_ip >> 24) & 0xFF, (conn_info_udp->dst_ip >> 16) & 0xFF,
                       (conn_info_udp->dst_ip >> 8) & 0xFF, conn_info_udp->dst_ip & 0xFF,
                       conn_info_udp->dport);
        }

        bpf_map_update_elem(&conn_info_map_bind_udp, &pid, conn_info_udp, BPF_ANY);
    }
    else // Для TCP
    {
        struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_bind, &pid);
        if (!conn_info)
        {
            bpf_printk("EXIT_bind No TCP connection info found for PID=%d\n", pid);
            return 0;
        }

        struct sockaddr_in addr;
        if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
        {
            bpf_printk("EXIT_bind Failed to read sockaddr for TCP PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map_bind, &pid);
            return 0;
        }

        if (addr.sin_family == AF_INET)
        {
            conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
            conn_info->dport = bpf_ntohs(addr.sin_port);

            bpf_printk("EXIT_bind TCP connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                       conn_info->pid, conn_info->comm,
                       (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
                       (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF,
                       conn_info->dport);
        }

        bpf_map_update_elem(&conn_info_map_bind, &pid, conn_info, BPF_ANY);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx)
{
	u32 pid = bpf_get_current_pid_tgid() >> 32;

	init_conn_info_connect(ctx);

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_connect, &pid);
	if (conn_info&&conn_info->comm[0]=='n')
	{
		bpf_printk("enter_connect CLIENT Connect entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
	}

	return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx)
{
	u32 pid = bpf_get_current_pid_tgid() >> 32;
	int ret = ctx->ret;


	if (ret < 0)
	{
		bpf_printk("exit_connect Connect failed for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_connect, &pid);
		return 0;
	}

	struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_connect, &pid);
	if (!conn_info)
	{
		bpf_printk("exit_connect No connection info found for PID=%d\n", pid);
		return 0;
	}

	struct sockaddr_in addr;
	if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
	{
		bpf_printk("exit_connect Failed to read sockaddr for PID=%d\n", pid);
		bpf_map_delete_elem(&conn_info_map_connect, &pid);

		return 0;
	}

	if (addr.sin_family == AF_INET)
	{
		conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
		conn_info->dport = bpf_ntohs(addr.sin_port);

		bpf_printk("exit_connect CLIENT Connected to server: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
				   conn_info->pid, conn_info->comm,
				   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
				   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
	}

	bpf_map_update_elem(&conn_info_map_connect, &pid, conn_info, BPF_ANY);


	return 0;
}