
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

