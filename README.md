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

[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "65",
	"severity": 8,
	"message": "expected a ';'",
	"source": "C/C++",
	"startLineNumber": 798,
	"startColumn": 23,
	"endLineNumber": 798,
	"endColumn": 24
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "20",
	"severity": 8,
	"message": "identifier \"user_ptr\" is undefined",
	"source": "C/C++",
	"startLineNumber": 800,
	"startColumn": 5,
	"endLineNumber": 800,
	"endColumn": 13
}]

[{
	"resource": "/home/gaz358/myprog/bpfgo/trace.c",
	"owner": "C/C++: IntelliSense",
	"code": "18",
	"severity": 8,
	"message": "expected a ')'",
	"source": "C/C++",
	"startLineNumber": 800,
	"startColumn": 28,
	"endLineNumber": 800,
	"endColumn": 34
}]















