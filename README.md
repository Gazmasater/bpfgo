struct trace_info {
    // IPv4
    struct in_addr srcIP;
    struct in_addr dstIP;
    __u32 srcIP6[4];    
    __u32 dstIP6[4];   
    
    __u32 pid;
    __u32 proto;
    __u16 sysexit;
    __u16 sport;
    __u16 dport;
    __u16 state;
    __u16 family;
    char comm[32];
} ;






