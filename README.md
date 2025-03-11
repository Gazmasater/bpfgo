nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

struct sys_enter_socket_args {


    unsigned short common_type;    
    unsigned char common_flags;       
    unsigned char common_preempt_count;    
    int common_pid;  
    int __syscall_nr;
    int family;     
    int type;
    int protocol;    


};

struct sys_exit_socket_args {


    unsigned short common_type;       
    unsigned char common_flags;    
    unsigned char common_preempt_count; 
    int common_pid;   
    int __syscall_nr;
    int pad;
    long ret; 

};

SEC("tracepoint/syscalls/sys_enter_socket")
int trace_socket_enter(struct sys_enter_socket_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct conn_info_t conn_info = {};

    bpf_printk("sys_enter_socket sys_exit_bind PID=%d",pid);





    return 0;
}


SEC("tracepoint/syscalls/sys_exit_socket")
int trace_socket_exit(struct sys_exit_socket_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    bpf_printk("!!!!sys_exit_socket sys_exit_bind PID=%d",pid);


    if (ret < 0) {
        bpf_printk("sys_exit_socket failed for PID=%d\n", pid);
        return 0;
    }

    return 0;
}





      |                    ^     ~~~~~~~~~~~~~~~~~~~~~~~

