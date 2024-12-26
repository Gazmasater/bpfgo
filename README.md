gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
[sudo] password for gaz358: 
INFO [faerie::elf] strtab: 0x237b symtab 0x23b8 relocs 0x2400 sh_offset 0x2400
libbpf: prog 'trace_sendto_exit': BPF program load failed: Invalid argument
libbpf: prog 'trace_sendto_exit': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
0: (bf) r6 = r1                       ; R1=ctx() R6_w=ctx()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
1: (85) call bpf_get_current_pid_tgid#14      ; R0_w=scalar()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
2: (77) r0 >>= 32                     ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff))
; u32 pid = bpf_get_current_pid_tgid() >> 32;
3: (63) *(u32 *)(r10 -4) = r0         ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R10=fp0 fp-8=mmmm????
; long ret = ctx->ret;
4: (79) r1 = *(u64 *)(r6 +16)         ; R1_w=scalar() R6_w=ctx()
; if (ret < 0) {
5: (65) if r1 s> 0xffffffff goto pc+7 13: R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R1_w=scalar(smin=0,umax=0x7fffffffffffffff,var_off=(0x0; 0x7fffffffffffffff)) R6_w=ctx() R10=fp0 fp-8=mmmm????
13: (bf) r2 = r10                     ; R2_w=fp0 R10=fp0
; struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
14: (07) r2 += -4                     ; R2_w=fp-4
15: (18) r1 = 0xffff98facaaad000      ; R1_w=map_ptr(map=conn_info_map_s,ks=4,vs=48)
17: (85) call bpf_map_lookup_elem#1   ; R0_w=map_value_or_null(id=1,map=conn_info_map_s,ks=4,vs=48)
18: (bf) r6 = r0                      ; R0_w=map_value_or_null(id=1,map=conn_info_map_s,ks=4,vs=48) R6_w=map_value_or_null(id=1,map=conn_info_map_s,ks=4,vs=48)
; if (!conn_info) {
19: (55) if r6 != 0x0 goto pc+6 26: R0_w=map_value(map=conn_info_map_s,ks=4,vs=48) R6_w=map_value(map=conn_info_map_s,ks=4,vs=48) R10=fp0 fp-8=mmmm????
; if (bpf_core_read(&addr, sizeof(addr), conn_info->sock_addr) != 0) {
26: <invalid CO-RE relocation>
failed to resolve CO-RE relocation <byte_off> [12] struct conn_info_t.sock_addr (0:0 @ offset 0)
processed 29 insns (limit 1000000) max_states_per_insn 0 total_states 2 peak_states 2 mark_read 1
-- END PROG LOAD LOG --
libbpf: prog 'trace_sendto_exit': failed to load: -22
libbpf: failed to load object 'tracepoint_bpf'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 22"), RecvError
