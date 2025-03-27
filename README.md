lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


SEC("kprobe/__x64_sys_getaddrinfo")
int trace_getaddrinfo(struct pt_regs *ctx) {
   
return 0;
    
}














