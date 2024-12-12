#include "vmlinux.h"
#include "bpf/bpf_tracing.h"
#include "bpf/bpf_endian.h"
#include "bpf/bpf_core_read.h"

// Определение структуры данных, передаваемой в userspace
struct event {
    u32 pid;
    char comm[16];
};

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY); // Карта для perf
    __uint(max_entries, 1024);
} perf_event_array SEC(".maps");



SEC("tracepoint/syscalls/sys_enter_open")
int tracepoint_handler(struct tracepoint__syscalls__sys_enter_open *ctx) {
    char msg[] = "sys_open called";
    bpf_perf_event_output(ctx, &perf_event_array, BPF_F_CURRENT_CPU, &msg, sizeof(msg));
    return 0;
}
