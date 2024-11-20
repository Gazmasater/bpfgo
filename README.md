#include <linux/ptrace.h>
#include <linux/socket.h>
#include <linux/in.h>

struct conn_info_t {
    u32 pid;
    u32 dst_ip;
    u16 dst_port;
    char comm[TASK_COMM_LEN];
    u32 fd;  // ФД сокета
};

BPF_HASH(conn_info_map, u32, struct conn_info_t);

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind_entry(struct trace_event_raw_sys_enter *ctx) {
    // Получаем PID процесса
    u64 pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = pid_tgid >> 32;

    // Получаем информацию о процессе
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));

    // Получаем аргументы sys_enter_bind (fd и sockaddr) с использованием PT_REGS
    int fd = PT_REGS_PARM1(ctx);  // Получаем fd сокета из первого параметра
    struct sockaddr_in *sock_addr = (struct sockaddr_in *)PT_REGS_PARM2(ctx); // sockaddr из второго параметра

    // Запоминаем только fd и pid для будущего использования
    conn_info.fd = fd;

    // Сохраняем информацию в карте
    bpf_map_update_elem(&conn_info_map, &pid, &conn_info, BPF_ANY);

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_bind")
int trace_bind_exit(struct trace_event_raw_sys_exit *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = PT_REGS_RC(ctx);  // Получаем результат системного вызова

    if (ret < 0) {
        // Обработка ошибки
        bpf_printk("Bind failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
    } else {
        // Логирование успешной привязки
        struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &pid);
        if (conn_info) {
            // Получаем sockaddr после того как системный вызов выполнен
            struct sockaddr_in *sock_addr = (struct sockaddr_in *)PT_REGS_PARM2(ctx); // sockaddr из второго параметра

            if (sock_addr) {
                conn_info->dst_ip = bpf_ntohl(sock_addr->sin_addr.s_addr); // Преобразуем IP в порядок хоста
                conn_info->dst_port = bpf_ntohs(sock_addr->sin_port);     // Преобразуем порт в порядок хоста

                bpf_printk("Bind success for PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                           conn_info->pid,
                           conn_info->comm,
                           (conn_info->dst_ip >> 24) & 0xFF,
                           (conn_info->dst_ip >> 16) & 0xFF,
                           (conn_info->dst_ip >> 8) & 0xFF,
                           conn_info->dst_ip & 0xFF,
                           conn_info->dst_port);
            }
            // Удаляем информацию о соединении после использования
            bpf_map_delete_elem(&conn_info_map, &pid);
        }
    }
    return 0;
}
