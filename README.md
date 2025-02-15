clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

    void *addr_ptr = (void *)ctx->addr;
    bpf_map_update_elem(&addr_map, &pid, &addr_ptr, BPF_ANY);


#include <linux/ptrace.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/sched.h>

struct conn_info_t {
    u32 pid;
    char comm[TASK_COMM_LEN];
    u32 src_ip;
    u16 sport;
    struct sockaddr *sock_addr;
};

BPF_HASH(conn_info_map, u32, struct conn_info_t);
BPF_HASH(addr_map, u32, struct sockaddr *);

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    struct sockaddr *sock_addr = ctx->addr;
    bpf_map_update_elem(&addr_map, &pid, &sock_addr, BPF_ANY);

    // Store connection info in the map
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    // Log the event for debugging
    struct conn_info_t *info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (info) {
        bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", info->pid, info->comm);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    // If there is an error in sending, clean up
    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    // Retrieve connection info from the map
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    // Retrieve sockaddr info
    void *addr_ptr = bpf_map_lookup_elem(&addr_map, &pid);
    if (!addr_ptr) {
        bpf_printk("UDP sys_exit_sendto: No sockaddr found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);  // Clean up addr_map
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), addr_ptr) != 0) {
        bpf_printk("UDP sys_exit_sendto: Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        bpf_map_delete_elem(&addr_map, &pid);  // Clean up addr_map
        return 0;
    }

    // Print the details of the connection
    bpf_printk("Comm=%s FAMILY=%d", conn_info->comm, addr.sin_family);

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        // Log the connection information
        bpf_printk("UDP sys_exit_sendto: Connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    // Update the connection info in the map
    bpf_map_update_elem(&conn_info_map, &pid, conn_info, BPF_ANY);

    return 0;
}
