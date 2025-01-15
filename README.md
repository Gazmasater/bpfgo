struct sockaddr_in addr;
if (BPF_CORE_READ(&addr, sin_family) != AF_INET) {
    bpf_printk("UDP sys_exit_sendto: Invalid address family for PID=%d\n", pid);
    bpf_map_delete_elem(&conn_info_map_sc, &pid);
    return 0;
}
