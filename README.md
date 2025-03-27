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

	kp, err := link.Kprobe("__x64_sys_getaddrinfo", objs.TraceGetaddrinfo, nil)
	if err != nil {
		log.Fatalf("Ошибка привязки kprobe: %v", err)
	}
	defer kp.Close()

 func link.Kprobe(symbol string, prog *ebpf.Program, opts *link.KprobeOptions) (link.Link, error)
Kprobe attaches the given eBPF program to a perf event that fires when the given kernel symbol starts executing. See /proc/kallsyms for available symbols. For example, printk():

kp, err := Kprobe("printk", prog, nil)
Losing the reference to the resulting Link (kp) will close the Kprobe and prevent further execution of prog. The Link must be Closed during program shutdown to avoid leaking system resources.

If attaching to symbol fails, automatically retries with the running platform's syscall prefix (e.g. __x64_) to support attaching to syscalls in a portable fashion.

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
[sudo] password for gaz358: 
Дескриптор нового namespace: 7
2025/03/27 13:55:40 Ошибка привязки kprobe: creating perf_kprobe PMU (arch-specific fallback for "__x64_sys_getaddrinfo"): token __x64___x64_sys_getaddrinfo: not found: no such file or directory
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
















