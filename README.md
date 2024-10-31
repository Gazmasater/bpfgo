truct {
  __uint(type, BPF_MAP_TYPE_HASH);
  __type(key, u64);
  __type(value, struct accept_args_t);
  __uint(max_entries, 1024*128);
} active_accept4_args_map SEC(".maps");

struct accept_args_t {
    struct sockaddr_in* addr;
    int fd;
};

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

    // Build the connect_event and save it to the map
    struct accept_args_t accept_args = {};
    accept_args.addr = (struct sockaddr_in *)saddr;
    bpf_map_update_elem(&active_accept4_args_map, &current_pid_tgid, &accept_args, BPF_ANY);

    return 0;
}

/home/gaz358/myprog/bpfgo/fentry.c:117:10: error: call to undeclared function 'should_intercept'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
  117 |     if (!should_intercept()) {
      |          ^
/home/gaz358/myprog/bpfgo/fentry.c:122:46: error: The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb
  122 |     struct pt_regs *ctx2 = (struct pt_regs *)PT_REGS_PARM1(ctx);
      |                                              ^
/usr/include/bpf/bpf_tracing.h:563:29: note: expanded from macro 'PT_REGS_PARM1'
  563 | #define PT_REGS_PARM1(x) ({ _Pragma(__BPF_TARGET_MISSING); 0l; })
      |                             ^
<scratch space>:44:6: note: expanded from here
   44 |  GCC error "The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb"
      |      ^
/home/gaz358/myprog/bpfgo/fentry.c:125:44: error: The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb
  125 |     bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));
      |                                            ^
/usr/include/bpf/bpf_tracing.h:564:29: note: expanded from macro 'PT_REGS_PARM2'
  564 | #define PT_REGS_PARM2(x) ({ _Pragma(__BPF_TARGET_MISSING); 0l; })
      |                             ^
<scratch space>:46:6: note: expanded from here
   46 |  GCC error "The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb"
      |      ^
/home/gaz358/myprog/bpfgo/fentry.c:125:43: error: cannot take the address of an rvalue of type 'long'
  125 |     bpf_probe_read(&saddr, sizeof(saddr), &PT_REGS_PARM2(ctx2));
      |                                           ^~~~~~~~~~~~~~~~~~~~
4 errors generated.
Error: compile: exit status 1
exit status 1
gen.go:3: running "go": exit status 1
