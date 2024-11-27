    u16 family;
    if (bpf_probe_read(&family, sizeof(family), conn_info->sock_addr) != 0) {
        bpf_printk("EXIT_accept4 Failed to read address family for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
        return 0;
    }
