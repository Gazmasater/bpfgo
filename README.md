#define BPF_CORE_READ(src,a,__VA_ARGS__...) ({ ___type((src), a, ## __VA_ARGS__) __r; BPF_CORE_READ_INTO(&__r, (src), a, ## __VA_ARGS__); __r; })
BPF_CORE_READ() is used to simplify BPF CO-RE relocatable read, especially
when there are few pointer chasing steps.
E.g., what in non-BPF world (or in BPF w/ BCC) would be something like:
int x = s->a.b.c->d.e->f->g;
can be succinctly achieved using BPF_CORE_READ as:
int x = BPF_CORE_READ(s, a.b.c, d.e, f, g);

BPF_CORE_READ will decompose above statement into 4 bpf_core_read (BPF
CO-RE relocatable bpf_probe_read_kernel() wrapper) calls, logically
equivalent to:

const void *__t = s->a.b.c;
__t = __t->d.e;
__t = __t->f;
return __t->g;
Equivalence is logical, because there is a heavy type casting/preservation
involved, as well as all the reads are happening through
bpf_probe_read_kernel() calls using __builtin_preserve_access_index() to
emit CO-RE relocations.

N.B. Only up to 9 "field accessors" are supported, which should be more
than enough for any practical purpose.
Expands to:

({ typeof((conn_info)->sock_addr) __r; ({ bpf_probe_read_kernel((void *)(&__r), sizeof(*(&__r)), (const void *)__builtin_preserve_access_index(&((typeof(((conn_info))))(((conn_info))))->sock_addr)); }); __r; })



struct conn_info_t
{
	struct sockaddr *sock_addr;
	u32 pid;
	u32 src_ip;
	u32 dst_ip;
	u32 addrlen;
	u16 sport;
	u16 dport;
	char comm[16];
	
};

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx) {
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

if (ret < 0) {
    bpf_map_delete_elem(&conn_info_map_sc, &pid);
    return 0;
}

struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
if (!conn_info) {
    bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
    bpf_map_delete_elem(&conn_info_map_sc, &pid);
    return 0;
}



struct sockaddr_in *addr;
addr = BPF_CORE_READ(conn_info, sock_addr);

if (!addr) {
    bpf_printk("Failed to read sock_addr using BPF_CORE_READ\n");
    return 0;
}

    bpf_map_update_elem(&conn_info_map_sc, &pid, conn_info, BPF_ANY);

    return 0;
}

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x21b0 symtab 0x21e8 relocs 0x2230 sh_offset 0x2230
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
5: (65) if r1 s> 0xffffffff goto pc+6 12: R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R1_w=scalar(smin=0,umax=0x7fffffffffffffff,var_off=(0x0; 0x7fffffffffffffff)) R6_w=ctx() R10=fp0 fp-8=mmmm????
; 
12: (bf) r2 = r10                     ; R2_w=fp0 R10=fp0
; struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map_sc, &pid);
13: (07) r2 += -4                     ; R2_w=fp-4
14: (18) r1 = 0xffff9ff2f4587400      ; R1_w=map_ptr(map=conn_info_map_s,ks=4,vs=48)
16: (85) call bpf_map_lookup_elem#1   ; R0_w=map_value_or_null(id=1,map=conn_info_map_s,ks=4,vs=48)
17: (bf) r6 = r0                      ; R0=map_value_or_null(id=1,map=conn_info_map_s,ks=4,vs=48) R6=map_value_or_null(id=1,map=conn_info_map_s,ks=4,vs=48)
; if (!conn_info) {
18: (55) if r6 != 0x0 goto pc+6 25: R0=map_value(map=conn_info_map_s,ks=4,vs=48) R6=map_value(map=conn_info_map_s,ks=4,vs=48) R10=fp0 fp-8=mmmm????
; bpf_printk("UDP sys_exit_sendto: No connection info found for PID=%d\n", pid);
25: <invalid CO-RE relocation>
failed to resolve CO-RE relocation <byte_off> [12] struct conn_info_t.sock_addr (0:0 @ offset 0)
processed 30 insns (limit 1000000) max_states_per_insn 0 total_states 3 peak_states 3 mark_read 1
-- END PROG LOAD LOG --
libbpf: prog 'trace_sendto_exit': failed to load: -22
libbpf: failed to load object 'tracepoint_bpf'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 22"), RecvError
