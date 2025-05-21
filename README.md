
#include <linux/in6.h> // для struct in6_addr

struct trace_info {
    // IPv4
    struct in_addr srcIP;
    struct in_addr dstIP;

    // IPv6
    struct in6_addr srcIP6;
    struct in6_addr dstIP6;

    __u32 pid;
    __u32 proto;
    __u16 sport;
    __u16 dport;
    __u16 family;
    __u8 sysexit;
    __u8 state;
    char comm[32];
};


[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "137",
	"severity": 8,
	"message": "expression must be a modifiable lvalue",
	"source": "C/C++",
	"startLineNumber": 573,
	"startColumn": 9,
	"endLineNumber": 573,
	"endColumn": 13
}]


