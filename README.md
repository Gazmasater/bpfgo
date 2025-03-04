gcc udp_server.c -o udp_server

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_accept/format
sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_bind/format

// Объявление карты BPF для хранения информации о сокетах
struct {
    __uint(type, BPF_MAP_TYPE_HASH);  // Тип карты: хеш-таблица
    __uint(max_entries, 1024);  // Максимальное количество записей
    __type(key, int);  // Тип ключа: дескриптор сокета (fd)
    __type(value, struct bind_info);  // Тип значения: структура bind_info
} bind_map SEC(".maps");


struct bind_info {
    int fd;  // Дескриптор сокета
    u16 port;  // Порт, на который привязан сокет
};

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind(struct sys_enter_bind_args *ctx) {
    int fd = ctx->fd;
    struct sockaddr_in *addr = (struct sockaddr_in *)ctx->addr;
    
    u16 port = bpf_ntohs(addr->sin_port);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Сохраняем информацию о сокете и порте в карту
    struct bind_info bind = {};
    bind.fd = fd;
    bind.port = port;

    bpf_map_update_elem(&bind_map, &fd, &bind, BPF_ANY);
    
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept4_enter(struct sys_enter_accept4_args *ctx) {
    int fd = ctx->fd;
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    // Получаем информацию о сокете из карты
    struct bind_info *bind = bpf_map_lookup_elem(&bind_map, &fd);
    if (bind == NULL) {
        return 0; // Нет информации о сокете
    }

    u16 bind_port = bind->port;
    bpf_printk("accept4 called on socket %d, port=%d\n", fd, bind_port);

    // Логика для работы с принятым соединением

    return 0;
}


struct sys_enter_bind_args{


    unsigned short common_type;       offset:0;       size:2; signed:0;
    unsigned char common_flags;       offset:2;       size:1; signed:0;
    unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
    int common_pid;   offset:4;       size:4; signed:1;

    int __syscall_nr; offset:8;       size:4; signed:1;

    int fd;   offset:16;      size:8; signed:0;
    struct sockaddr * umyaddr;        offset:24;      size:8; signed:0;
    int addrlen;      offset:32;      size:8; signed:0;




};




