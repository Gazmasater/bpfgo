

typedef unsigned char __u8;
typedef short int __s16;
typedef short unsigned int __u16;
typedef int __s32;
typedef unsigned int __u32;
typedef long long int __s64;
typedef long long unsigned int __u64;
typedef __u8 u8;
typedef __s16 s16;
typedef __u16 u16;
typedef __s32 s32;
typedef __u32 u32;
typedef __s64 s64;
typedef __u64 u64;
typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __be32;
typedef __u64 __be64;
typedef __u32 __wsum;

#define kConnect 0
#define kAccept 1
#define CGROUP_LEN 128
#define memory_cgrp_id 4

struct connect_event_t {
    u64 eventtype;
    u64 type;
    u64 timestamp_ns;
    u32 pid;
    u32 tid;
    u32 fd;
    u32 src_host;
    u32 dest_host;
    u16 src_port;
    u16 dest_port; // the ports need to be next to each other so C doesn't zero-pad them to 4 bytes each
    char cgroup[CGROUP_LEN];
};

struct accept_args_t {
    struct sockaddr_in* addr;
    int fd;
};

struct addr_t {
    u32 ip;
    u16 port;
};

enum event_type { eConnect, eData, eClose, eDebug, eGetsockname };

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1024 * 1024 * 128); // Important that its big enough otherwise events will be dropped and cause weird behaviour
} data_events SEC(".maps");




