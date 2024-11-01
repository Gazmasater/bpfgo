bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "vmlinux.h"
#include "bpf/bpf_core_read.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_helpers.h"
#include "bpf/bpf_tracing.h"
#include "common.h"
#include "stdbool.h"

char __license[] SEC("license") = "GPL";

int should_intercept(void) {
    // Your logic here
    return 1;  // For testing, always return true
}

SEC("kprobe/__x64_sys_accept")
int probe_accept4(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    if (!should_intercept()) {
        return 0;
    }

    // Логируем PID процесса, вызывающего accept
    bpf_printk("kprobe/accept entry: PID: %d\n", pid);

    // Считываем аргументы
    struct pt_regs *ctx2 = (struct pt_regs *)PT_REGS_PARM1(ctx);
    struct sockaddr *saddr;
    bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));

    // Здесь можно добавить любую логику обработки, если это необходимо,
    // но мы не сохраняем данные в карту

    return 0;
}

