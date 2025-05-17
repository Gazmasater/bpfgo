struct trace_info {
    // IPv4
    struct sockaddr_in ssrcIP;
    struct sockaddr_in ddstIP; 
    struct in_addr srcIP;
    struct in_addr dstIP;

    __u32 srcIP6[4];    
    __u32 dstIP6[4];   
    //  __u16 ssrcIP6[8];
    // __u16 ddstIP6[8];

    __u16 sport;
    __u16 dport;

    __u32 pid;
    __u32 proto;
    __u32 sysexit;
    __u32 state;

    __u16 family;

    char comm[32];
} ;



