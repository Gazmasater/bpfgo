// go:build exclude
// This is a compact version of `vmlinux.h` to be used in the examples using C code.

#pragma once

#define memory_cgrp_id 
#define CGROUP_LEN 128
#define kAccept 1


typedef __u16 __sum16;

#define ETH_P_IP 0x0800


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

struct addr_t {
    u32 ip;
    u16 port;
};

struct accept_args_t {
    struct sockaddr_in* addr;
    int fd;
};


enum event_type { eConnect, eData, eClose, eDebug, eGetsockname };


/* BPF_FUNC_perf_event_output, BPF_FUNC_perf_event_read and
 * BPF_FUNC_perf_event_read_value flags.
 */
#define BPF_F_INDEX_MASK 0xffffffffULL
#define BPF_F_CURRENT_CPU BPF_F_INDEX_MASK

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);
  __type(value, struct accept_args_t);
  __uint(max_entries, 1024*128);
} active_accept4_args_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1024 * 1024 * 128); // Important that its big enough otherwise events will be dropped and cause weird behaviour
} data_events SEC(".maps");

