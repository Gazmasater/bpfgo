#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/ptrace.h>

// Определение структуры данных, передаваемой в userspace
struct event {
    u32 pid;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY); // Карта для perf
} events SEC(".maps");

SEC("tp/syscalls/sys_enter_write")
int handle_write(struct trace_event_raw_sys_enter *ctx) {
    struct event e = {};
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    e.pid = pid;
    bpf_get_current_comm(&e.comm, sizeof(e.comm));

    // Отправка данных в perf
    bpf_perf_event_output(ctx, &events, BPF_F_CURRENT_CPU, &e, sizeof(e));
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
