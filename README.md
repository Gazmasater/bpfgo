az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
[sudo] password for gaz358: 
INFO [faerie::elf] strtab: 0xc7e symtab 0xcb8 relocs 0xd00 sh_offset 0xd00
libbpf: prog 'trace_accept4_entry': BPF program load failed: Permission denied
libbpf: prog 'trace_accept4_entry': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; int trace_accept4_entry(struct sys_enter_accept_args *ctx) {
0: (bf) r6 = r1                       ; R1=ctx() R6_w=ctx()
; bpf_printk("HELLO sys_enter_accept");
1: (18) r1 = 0xffff9f6c05ab1710       ; R1_w=map_value(map=fentry_b.rodata,ks=4,vs=145)
3: (b7) r2 = 23                       ; R2_w=23
4: (85) call bpf_trace_printk#6       ; R0_w=scalar()
; bpf_printk("HELLO init_conn_info_accept");
5: (18) r1 = 0xffff9f6c05ab176d       ; R1_w=map_value(map=fentry_b.rodata,ks=4,vs=145,off=93)
7: (b7) r2 = 28                       ; R2_w=28
8: (85) call bpf_trace_printk#6       ; R0=scalar()
; int pid = ctx->common_pid;
9: (61) r1 = *(u32 *)(r6 +4)
invalid bpf_context access off=4 size=4
processed 8 insns (limit 1000000) max_states_per_insn 0 total_states 1 peak_states 1 mark_read 1
-- END PROG LOAD LOG --
libbpf: prog 'trace_accept4_entry': failed to load: -13
libbpf: failed to load object 'fentry_bpf�iw'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 13"), RecvError
