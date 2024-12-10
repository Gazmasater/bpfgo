#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 12); // 4 KB ring buffer
} ringbuf SEC(".maps");

SEC("tp/syscalls/sys_enter_write")
int handle_write(void *ctx) {
    const char msg[] = "Hello, ringbuf!";
    char *buf = bpf_ringbuf_reserve(&ringbuf, sizeof(msg), 0);
    if (!buf)
        return 0;

    __builtin_memcpy(buf, msg, sizeof(msg));
    bpf_ringbuf_submit(buf, 0);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
