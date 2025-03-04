gcc udp_server.c -o udp_server

struct dst_info_t {
    u32 dst_ip;
    u16 dport;
    char comm[64];  // Это поле будет хранить имя процесса
};


struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, u32);  // Используем хеш имени процесса или PID как ключ
    __type(value, struct dst_info_t);  // Структура для хранения IP, порта и имени процесса
} dst_info_map SEC(".maps");

struct dst_info_t info = {};
info.dst_ip = ip;
info.dport = port;
__builtin_memcpy(info.comm, conn_info->comm, sizeof(info.comm));

bpf_map_update_elem(&dst_info_map, &pid, &info, BPF_ANY);


