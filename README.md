lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


SEC("kprobe/__x64_sys_getaddrinfo")
int trace_getaddrinfo(struct pt_regs *ctx) {
    char query_data[256]; // Буфер для хранения имени хоста
    const char __user *user_ptr;

    // Получаем первый аргумент системного вызова (имя хоста)
    user_ptr = (const char __user *)PT_REGS_PARM1(ctx);

    // Считываем строку из пространства пользователя
    if (bpf_probe_read_user_str(query_data, sizeof(query_data), user_ptr) > 0) {
        bpf_printk("getaddrinfo called with query: %s\n", query_data);
    }

    return 0;
}



SEC("kprobe/__x64_sys_getaddrinfo")
int trace_getaddrinfo(struct pt_regs *ctx) {
    char query_data[256]; // Буфер для хранения имени хоста
    const char __user *user_ptr; // Объявляем user_ptr

    // Получаем первый аргумент системного вызова (имя хоста)
    user_ptr = (const char __user *)PT_REGS_PARM1(ctx);

    // Проверяем, что указатель не NULL
    if (user_ptr) {
        // Читаем строку из пользовательского пространства
        if (bpf_probe_read_user_str(query_data, sizeof(query_data), user_ptr) > 0) {
            bpf_printk("getaddrinfo called with query: %s\n", query_data);
        }
    }

    return 0;
}


SEC("kprobe/__x64_sys_getaddrinfo")
int trace_getaddrinfo(struct pt_regs *ctx) {
    char query_data[256]; 
    const char  *user_ptr; 

     user_ptr = (const char  *)PT_REGS_PARM1(ctx);

    if (user_ptr) {
        if (bpf_probe_read_user_str(query_data, sizeof(query_data), user_ptr) > 0) {
            bpf_printk("getaddrinfo called with query: %s\n", query_data);
        }
    }

    return 0;
}















