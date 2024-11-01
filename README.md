bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SEC("kretprobe/__x64_sys_accept")
int probe_ret_accept4(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    struct accept_args_t* accept_args = bpf_map_lookup_elem(&active_accept4_args_map, &current_pid_tgid);
    if (accept_args == NULL) {
        return 0;
    }

    // Get the FD and check the call to accept4() was successful
    int fd = (int)PT_REGS_RC(ctx);
    if (fd < 0) {
        bpf_printk("kprobe/accept return: failed: PID: %d, FD: %d\n", pid, fd);
        return 0;
    }

    // Get the source IP & port
    struct addr_t src_addr = {};
    parse_address(&src_addr, accept_args);

    // Get the cgroup name
    struct task_struct *cur_tsk = (struct task_struct *)bpf_get_current_task();
    if (cur_tsk == NULL) {
        bpf_printk("kprobe/accept return: failed to get cur task PID: %d, FD: %d\n", pid, fd);
        return -1;
    }
    int cgrp_id = memory_cgrp_id;
    const char *name = BPF_CORE_READ(cur_tsk, cgroups, subsys[cgrp_id], cgroup, kn, name);

    // Build the connect_event and save it to the map
    struct connect_event_t conn_event;
    __builtin_memset(&conn_event, 0, sizeof(conn_event));
    conn_event.eventtype = eConnect;
    conn_event.type = kAccept;
    conn_event.timestamp_ns = bpf_ktime_get_ns();
    conn_event.pid = pid;
    conn_event.tid = current_pid_tgid;
    conn_event.fd = fd;
    conn_event.src_host = src_addr.ip;
    conn_event.src_port = src_addr.port;
    conn_event.dest_host = 0;
    conn_event.dest_port = 0;
    bpf_probe_read_str(&conn_event.cgroup, sizeof(conn_event.cgroup), name);

    bpf_map_delete_elem(&active_accept4_args_map, &current_pid_tgid);
    bpf_printk("kprobe/accept return: PID: %d, FD: %d", pid, fd);
    bpf_ringbuf_output(&data_events, &conn_event, sizeof(struct connect_event_t), 0);

    return 0;
}
