//go:build ignore

#include "common.h"

#ifndef TASK_COMM_LEN
#define TASK_COMM_LEN 16
#endif

char __license[] SEC("license") = "Dual MIT/GPL";

struct event {
	u32 pid;
	u8 comm[TASK_COMM_LEN];
};

struct {
	__uint(type, BPF_MAP_TYPE_RINGBUF);
	__uint(max_entries, 1 << 24);
} events SEC(".maps");

// Force emitting struct event into the ELF.
const struct event *unused __attribute__((unused));

SEC("kprobe/sys_execve")
int kprobe_execve(struct pt_regs *ctx) {
	u64 id   = bpf_get_current_pid_tgid();
	u32 tgid = id >> 32;
	struct event *task_info;

	task_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
	if (!task_info) {
		return 0;
	}

	task_info->pid = tgid;
	bpf_get_current_comm(&task_info->comm, TASK_COMM_LEN);

	bpf_ringbuf_submit(task_info, 0);

	return 0;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x5ba symtab 0x5f8 relocs 0x640 sh_offset 0x640
INFO [bpf_loader_lib::skeleton::preload::section_loader] User didn't specify custom value for variable unused, use the default one in ELF
libbpf: prog 'kprobe_execve': BPF program load failed: Permission denied
libbpf: prog 'kprobe_execve': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; u64 id   = bpf_get_current_pid_tgid();
0: (85) call bpf_get_current_pid_tgid#14      ; R0_w=scalar()
1: (bf) r6 = r0                       ; R0_w=scalar(id=1) R6_w=scalar(id=1)
; task_info = bpf_ringbuf_reserve(&events, sizeof(struct event), 0);
2: (18) r1 = 0xffff8921dc456400       ; R1_w=map_ptr(map=events,ks=0,vs=0)
4: (b7) r2 = 16                       ; R2_w=16
5: (b7) r3 = 0                        ; R3_w=0
6: (85) call bpf_ringbuf_reserve#131          ; R0_w=ringbuf_mem_or_null(id=3,ref_obj_id=3,sz=16) refs=3
; if (!task_info) {
7: (15) if r0 == 0x0 goto pc+10       ; R0_w=ringbuf_mem(ref_obj_id=3,sz=16) refs=3
; u32 tgid = id >> 32;
8: (77) r6 >>= 32                     ; R6_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) refs=3
; task_info->pid = tgid;
9: (63) *(u32 *)(r0 +0) = r6          ; R0_w=ringbuf_mem(ref_obj_id=3,sz=16) R6_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) refs=3
; bpf_get_current_comm(&task_info->comm, TASK_COMM_LEN);
10: (bf) r1 = r0                      ; R0_w=ringbuf_mem(ref_obj_id=3,sz=16) R1_w=ringbuf_mem(ref_obj_id=3,sz=16) refs=3
11: (07) r1 += 4                      ; R1_w=ringbuf_mem(ref_obj_id=3,off=4,sz=16) refs=3
; bpf_get_current_comm(&task_info->comm, TASK_COMM_LEN);
12: (b7) r2 = 16                      ; R2_w=16 refs=3
13: (bf) r6 = r0                      ; R0_w=ringbuf_mem(ref_obj_id=3,sz=16) R6_w=ringbuf_mem(ref_obj_id=3,sz=16) refs=3
14: (85) call bpf_get_current_comm#16
invalid access to memory, mem_size=16 off=4 size=16
R1 min value is outside of the allowed memory range
processed 14 insns (limit 1000000) max_states_per_insn 0 total_states 0 peak_states 0 mark_read 0
-- END PROG LOAD LOG --
libbpf: prog 'kprobe_execve': failed to load: -13
libbpf: failed to load object 'tracepoint_bpf'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 13"), RecvError
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
