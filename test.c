#include <vmlinux.h>
#include <stdbool.h>
#include "bpf/bpf_core_read.h"
#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include "common.h"
//#include "go_arg.h"
//#include "openssl_uprobes.h"
//#include "go_tls_uprobes.h"
//#include "node_openssl_trace.h"

char __license[] SEC("license") = "GPL";

struct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);
  __type(value, struct accept_args_t);
  __uint(max_entries, 1024*128);
} active_accept4_args_map SEC(".maps");



SEC("kprobe/accept4")
int probe_accept4(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    

    bpf_printk("kprobe/accept entry: PID: %d\n", pid);
    struct pt_regs *ctx2 = (struct pt_regs *)PT_REGS_PARM1(ctx);

    struct sockaddr *saddr;
    bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));

    //Build the connect_event and save it to the map
  struct accept_args_t accept_args = {};
  accept_args.addr = (struct sockaddr_in *)saddr;
  bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);

    return 0;
}

// SEC("kretprobe/accept4")
// int probe_ret_accept4(struct pt_regs *ctx) {
//     u64 current_pid_tgid = bpf_get_current_pid_tgid();
//     u32 pid = current_pid_tgid >> 32;

//     struct accept_args_t* accept_args = bpf_map_lookup_elem(&active_accept4_args_map, &current_pid_tgid);
//     if (accept_args == NULL) {
//         return 0;
//     }

//     // Get the FD and check the call to accept4() was successful
//     int fd = (int)PT_REGS_RC(ctx);
//     if (fd < 0) {
//         bpf_printk("kprobe/accept return: failed: PID: %d, FD: %d\n", pid, fd);
//         return 0;
//     }

//     // Get the source IP & port
//     struct addr_t src_addr = {};
//     parse_address(&src_addr, accept_args);

//     // Get the cgroup name
//     struct task_struct *cur_tsk = (struct task_struct *)bpf_get_current_task();
//     if (cur_tsk == NULL) {
//         bpf_printk("kprobe/accept return: failed to get cur task PID: %d, FD: %d\n", pid, fd);
//         return -1;
//     }
    // int cgrp_id = memory_cgrp_id;
    // const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);

    // Build the connect_event and save it to the map
  //  struct connect_event_t conn_event;
  //  __builtin_memset(&conn_event, 0, sizeof(conn_event));
  //  conn_event.eventtype = eConnect;
  //  conn_event.type = kAccept;
  //  conn_event.timestamp_ns = bpf_ktime_get_ns();
  //  conn_event.pid = pid;
  //  conn_event.tid = current_pid_tgid;
  //  conn_event.fd = fd;
  //  conn_event.src_host = src_addr.ip;
  //  conn_event.src_port = src_addr.port;
  //  conn_event.dest_host = 0;
  //  conn_event.dest_port = 0;
  //  bpf_probe_read_str(&conn_event.cgroup, sizeof(conn_event.cgroup), name);
//
   // bpf_map_delete_elem(&active_accept4_args_map, &current_pid_tgid);
   // bpf_printk("kprobe/accept return: PID: %d, FD: %d", pid, fd);
   // bpf_ringbuf_output(&data_events, &conn_event, sizeof(struct connect_event_t), 0);

  //  return 0;
//}