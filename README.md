#include <linux/ptrace.h>
#include <bcc/proto.h>

BPF_HASH(conn_info_map, u32, struct conn_info_t);

struct conn_info_t {
    u32 pid;
    u32 dst_ip;
    u16 dport;
    struct sockaddr_in *sock_addr;
    char comm[TASK_COMM_LEN];
};

// Tracepoint для sys_bind (enter)
SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_enter(struct trace_event_raw_sys_enter *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    struct sockaddr_in addr;
    struct conn_info_t conn_info = {};

    // Получаем параметры системного вызова
    struct sockaddr *addr_ptr = (struct sockaddr *)PT_REGS_PARM2(ctx);

    // Проверяем, что адрес - это sockaddr_in
    if (!addr_ptr || addr_ptr->sa_family != AF_INET)
        return 0;

    // Сохраняем данные о сокете в мапу
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Сохраняем указатель на sockaddr_in
    conn_info.sock_addr = (struct sockaddr_in *)addr_ptr;
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

// Tracepoint для sys_bind (exit)
SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);  // Получаем результат вызова

    // Если произошла ошибка, удаляем запись из мапы
    if (ret < 0) {
        bpf_printk("sys_bind Failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
    if (!conn_info) {
        bpf_printk("sys_bind No connection info found for PID=%d\n", pid);
        return 0;
    }

    // Получаем IP и порт клиента из sockaddr
    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
        bpf_printk("sys_bind Failed to read sockaddr for PID=%d\n", pid);
        return 0;
    }

    // Извлекаем IP и порт из sockaddr_in
    if (addr.sin_family == AF_INET) {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);  // Преобразуем IP в порядок хоста
        conn_info->dport = bpf_ntohs(addr.sin_port);  // Преобразуем порт в порядок хоста

        bpf_printk("sys_bind Completed: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                    conn_info->pid, conn_info->comm,
                    (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
                    (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
    }

    // Удаляем информацию о соединении из мапы после обработки
    bpf_map_delete_elem(&conn_info_map, &pid);

    return 0;
}
