
struct trace_info {
    union {
        struct {
            struct in_addr srcIP;
            struct in_addr dstIP;
        };
        struct {
            __u32 srcIP6[4];
            __u32 dstIP6[4];
        };
    };
    __u32 pid;
    __u32 proto;
    __u16 sport;
    __u16 dport;
    __u16 family;
    __u8 sysexit;
    __u8 state;
    char comm[32];
};

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)
Compiled /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.o
Error: can't write /home/gaz358/myprog/bpfgo/bpf_x86_bpfel.go: can't generate types: template: common:17:4: executing "common" at <$.TypeDeclaration>: error calling TypeDeclaration: Struct:"trace_info": field 0: anonymous fields are not supported
