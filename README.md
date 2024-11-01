bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "bpf/bpf_tracing.h" // bpf_helpers.h included within
#include "bpf/bpf.h"
#include "common_new.h"
#include "linux/ptrace.h"

char __license[] SEC("license") = "GPL";

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __type(key, u64);
    __type(value, struct accept_args_t);
    __uint(max_entries, 1024*128);
} active_accept4_args_map SEC(".maps");

// https://linux.die.net/man/3/accept
// int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
SEC("kprobe/accept4")
int probe_accept4(struct pt_regs *ctx) {
    u64 current_pid_tgid = bpf_get_current_pid_tgid();
    u32 pid = current_pid_tgid >> 32;

    if (!should_intercept()) {
        return 0;
    }

    bpf_printk("kprobe/accept entry: PID: %d\n", pid);
    struct pt_regs *ctx2 = (struct pt_regs *)PT_REGS_PARM1(ctx);

    struct sockaddr *saddr;
    bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));

    // Save to the map
    struct accept_args_t accept_args = {};
    accept_args.addr = (struct sockaddr_in *)saddr;
    int map_fd = bpf_map_fd_by_name("active_accept4_args_map"); // Получить дескриптор карты
    bpf_map_update_elem(map_fd, &current_pid_tgid, &accept_args, BPF_ANY);

    return 0;
}
