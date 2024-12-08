#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <linux/inet.h>

struct conn_info_t {
    u32 pid;
    char comm[16];
    u32 src_ip;
    u16 sport;
    void *sock_addr;
};

struct bpf_map_def SEC("maps") conn_info_map_sc = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u32),
    .value_size = sizeof(struct conn_info_t),
    .max_entries = 1024,
};

struct bpf_map_def SEC("maps") output = {
    .type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
    .max_entries = 1024,
};

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0) {
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
    if (!conn_info) {
        bpf_printk("UDP sys_exit_sendto No connection info found for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    if (conn_info->comm[0] == 'u' && conn_info->comm[1] == 'd') {
        bpf_printk("UDP sys_exit_sendto sockaddr=%p", conn_info->sock_addr);
    } else {
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("UDP sys_exit_sendto Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map_sc, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET) {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("UDP sys_exit_sendto connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);

        // Отправляем данные в юзерспейс
        bpf_perf_event_output(ctx, &output, BPF_F_CURRENT_CPU, conn_info, sizeof(*conn_info));
    }

    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);
    return 0;
}
