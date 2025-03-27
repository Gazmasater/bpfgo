lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftrace -l

sudo ss -tunp | grep 192.0.73.2:443


struct sys_enter_getsockname_args {

    unsigned short common_type;      
    unsigned char common_flags;    
    unsigned char common_preempt_count;  
    int common_pid;  
    int __syscall_nr;
    int fd;  
    struct sockaddr * usockaddr;     
    int * usockaddr_len;    
    

};

SEC("tracepoint/syscalls/sys_enter_getsockname")
int trace_enter_getsockname(struct sys_enter_getsockname_args *ctx) {

    


    return 0;
}


PID=3678 srcAddr=34.117.188.166:443  SYSCALL=2
PID=3678 dstAddr=34.117.188.166:443  SYSCALL=1









