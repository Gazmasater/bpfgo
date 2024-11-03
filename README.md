bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "bpf/bpf_helpers.h"
#include "bpf/bpf_core_read.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf.h"
#include "netinet/in.h"
#define TASK_COMM_LEN  16


SEC("kprobe/do_accept")
int bpf_prog(struct pt_regs *ctx, int sockfd, struct sockaddr __user *addr, int __user *addrlen) {
    struct task_struct *task = (struct task_struct *)bpf_get_current_task();
    
    // Получаем информацию о задаче
    char comm[TASK_COMM_LEN];
    bpf_get_current_comm(&comm, sizeof(comm));

    // Здесь вы можете логировать информацию или делать что-то другое
    bpf_trace_printk("Process %s accepted a connection on sockfd: %d\n", comm, sockfd);

    return 0;
}

char _license[] SEC("license") = "GPL";

[{
	"resource": "/home/gaz358/myprog/bpfgo/fentry.c",
	"owner": "C/C++: IntelliSense",
	"code": "18",
	"severity": 8,
	"message": "expected a ')'",
	"source": "C/C++",
	"startLineNumber": 10,
	"startColumn": 70,
	"endLineNumber": 10,
	"endColumn": 71
}]
