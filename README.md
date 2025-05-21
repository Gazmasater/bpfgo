
struct trace_info {
    union {
        struct in_addr srcIP;
        __u32 srcIP6[4];
    };
    union {
        struct in_addr dstIP;
        __u32 dstIP6[4];
    };
    __u32 pid;
    __u32 proto;
    char comm[16];
    // объединяем 5 u16 полей в два 32-битных
    struct {
        __u32 sysexit : 10; // 10 бит — обычно хватит
        __u32 sport   : 16;
        __u32 dport   : 16;
        __u32 state   : 8;
        __u32 family  : 8;
    };
};

