SEC("tracepoint/syscalls/sys_exit_accept4")
int trace_accept4_exit(struct sys_exit_accept4_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_printk("EXIT_accept4 Accept4 failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept_four, &pid);
    if (!conn_info) {
        bpf_printk("EXIT_accept4 No connection info found for PID=%d\n", pid);
        return 0;
    }

    bpf_printk("EXIT_accept4 sockaddr=%p", conn_info->sock_addr);

    // Определяем семейство адресов
    u16 family;
    if (bpf_probe_read(&family, sizeof(family), conn_info->sock_addr) != 0) {
        bpf_printk("EXIT_accept4 Failed to read sockaddr family for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
        return 0;
    }

    if (family == AF_INET) {
        // IPv4
        struct sockaddr_in addr;
        if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
            bpf_printk("EXIT_accept4 Failed to read sockaddr for PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
            return 0;
        }

        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("EXIT_accept4 Accepted IPv4 connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

    } else if (family == AF_INET6) {
        // IPv6
        struct sockaddr_in6 addr6;
        if (bpf_probe_read(&addr6, sizeof(addr6), conn_info->sock_addr) != 0) {
            bpf_printk("EXIT_accept4 Failed to read sockaddr_in6 for PID=%d\n", pid);
            bpf_map_delete_elem(&conn_info_map_accept_four, &pid);
            return 0;
        }

        bpf_probe_read(&conn_info->src_ip6, sizeof(conn_info->src_ip6), &addr6.sin6_addr);
        conn_info->sport = bpf_ntohs(addr6.sin6_port);

        bpf_printk("EXIT_accept4 Accepted IPv6 connection: PID=%d, Comm=%s, IP=%x:%x:%x:%x:%x:%x:%x:%x, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   ntohs(addr6.sin6_addr.s6_addr16[0]), ntohs(addr6.sin6_addr.s6_addr16[1]),
                   ntohs(addr6.sin6_addr.s6_addr16[2]), ntohs(addr6.sin6_addr.s6_addr16[3]),
                   ntohs(addr6.sin6_addr.s6_addr16[4]), ntohs(addr6.sin6_addr.s6_addr16[5]),
                   ntohs(addr6.sin6_addr.s6_addr16[6]), ntohs(addr6.sin6_addr.s6_addr16[7]), conn_info->sport);
    } else {
        bpf_printk("EXIT_accept4 Unknown address family for PID=%d\n", pid);
    }

    bpf_map_update_elem(&conn_info_map_accept_four, &pid, conn_info, BPF_ANY);

    return 0;
}
