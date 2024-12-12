#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_core_read.h"

// Структура события для отправки в userspace
struct event {
    u32 pid;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 1024);
} perf_event_array SEC(".maps");

SEC("tracepoint/syscalls/sys_enter_open")
int tracepoint_handler(struct sys_enter_open_args *ctx) {
    struct event evt = {};
    
    // Получение PID
    evt.pid = bpf_get_current_pid_tgid() >> 32;
    
    // Получение имени процесса
    bpf_get_current_comm(&evt.comm, sizeof(evt.comm));
    
    // Отправка данных в userspace через perf_event_array
    bpf_perf_event_output(ctx, &perf_event_array, BPF_F_CURRENT_CPU, &evt, sizeof(evt));
    
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
