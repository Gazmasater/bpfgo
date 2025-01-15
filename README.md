struct sockaddr_in addr;
if (BPF_CORE_READ(&addr, sin_family) != AF_INET) {
    bpf_printk("UDP sys_exit_sendto: Invalid address family for PID=%d\n", pid);
    bpf_map_delete_elem(&conn_info_map_sc, &pid);
    return 0;
}

u32 src_ip = BPF_CORE_READ(&addr, sin_addr.s_addr);
u16 src_port = BPF_CORE_READ(&addr, sin_port);

bpf_printk("UDP sys_exit_sendto: IP=%d.%d.%d.%d, Port=%d\n",
           (src_ip >> 24) & 0xFF, (src_ip >> 16) & 0xFF,
           (src_ip >> 8) & 0xFF, src_ip & 0xFF,
           bpf_ntohs(src_port));


