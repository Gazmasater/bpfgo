// Структура для хранения информации о соединении
struct conn_info_t {
    int pid;
    char comm[TASK_COMM_LEN];  // Буфер для хранения имени процесса
};

// Карты для хранения информации о соединениях и сокетах
struct bpf_map_def SEC("maps") conn_info_map_accept = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(int),    // PID
    .value_size = sizeof(struct conn_info_t),
    .max_entries = 1024,
};

struct bpf_map_def SEC("maps") sock_addr_map = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(int),    // PID
    .value_size = sizeof(struct sockaddr),
    .max_entries = 1024,
};

// Функция для инициализации информации о соединении
static __always_inline int init_conn_info_accept(struct bpf_raw_tracepoint_args *ctx) {
    bpf_printk("HELLO init_conn_info_accept");

    // Используем PID из структуры bpf_raw_tracepoint_args
    int pid = ctx->common_pid;

    // Инициализируем структуру conn_info
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));  // Получаем имя процесса

    // Читаем sockaddr из аргумента upeer_sockaddr
    struct sockaddr addr = {};
    if (ctx->args[1] && bpf_probe_read_user(&addr, sizeof(addr), (void *)ctx->args[1]) == 0) {
        // Сохраняем sockaddr в карту
        bpf_map_update_elem(&sock_addr_map, &pid, &addr, BPF_ANY);
    } else {
        bpf_printk("no save sockaddr in map");
    }

    // Сохраняем информацию о соединении в карту
    bpf_map_update_elem(&conn_info_map_accept, &pid, &conn_info, BPF_ANY);

    return 0;
}

// Трейс для системного вызова accept
SEC("tracepoint/syscalls/sys_enter_accept")
int trace_accept4_entry(struct bpf_raw_tracepoint_args *ctx) {
    bpf_printk("HELLO sys_enter_accept");

    // Инициализация информации о соединении
    init_conn_info_accept(ctx);

    // Получаем PID из структуры bpf_raw_tracepoint_args
    int pid = ctx->common_pid;

    // Извлекаем информацию о соединении из карты
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_accept, &pid);
    if (conn_info) {
        bpf_printk("SERVER accept entry: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);
    } else {
        bpf_printk("conn_info not found for PID=%d\n", pid);
    }

    return 0;
}
