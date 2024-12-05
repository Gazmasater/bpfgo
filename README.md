struct conn_info_key {
    u32 pid;          // Идентификатор процесса (PID)
    u8 op_type;       // Тип операции: 0 — для sendto, 1 — для connect
};

struct
{
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, struct conn_info_key); // Используем новую структуру ключа
    __type(value, struct conn_info_t);
} conn_info_map SEC(".maps");

static __always_inline int init_conn_info(struct sockaddr *sock_addr, struct bpf_map_def *map, u32 pid, u8 op_type)
{
    struct conn_info_t conn_info = {};
    conn_info.pid = pid;
    bpf_get_current_comm(&conn_info.comm, sizeof(conn_info.comm));
    conn_info.sock_addr = sock_addr;

    struct conn_info_key key = {};
    key.pid = pid;
    key.op_type = op_type;

    bpf_map_update_elem(map, &key, &conn_info, BPF_ANY);
    return 0;
}

static __always_inline int init_conn_info_sendto(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    return init_conn_info((struct sockaddr *)ctx->addr, &conn_info_map, pid, 0); // 0 для sendto
}

static __always_inline int init_conn_info_connect(struct sys_enter_connect_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    return init_conn_info((struct sockaddr *)ctx->uservaddr, &conn_info_map, pid, 1); // 1 для connect
}

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto_enter(struct sys_enter_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info_sendto(ctx);

    struct conn_info_key key = { .pid = pid, .op_type = 0 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (conn_info && conn_info->comm[0] == 'u')
    {
        bpf_printk("SERVER sys_enter_sendto: PID=%d, Comm=%s\n", conn_info->pid, conn_info->comm);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_sendto")
int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    long ret = ctx->ret;

    if (ret < 0)
    {
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_key key = { .pid = pid, .op_type = 0 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (!conn_info)
    {
        bpf_printk("UDP sys_exit_sendto No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        bpf_printk("UDP sys_exit_sendto Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->src_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->sport = bpf_ntohs(addr.sin_port);

        bpf_printk("UDP sys_exit_sendto  connection: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->src_ip >> 24) & 0xFF, (conn_info->src_ip >> 16) & 0xFF,
                   (conn_info->src_ip >> 8) & 0xFF, conn_info->src_ip & 0xFF, conn_info->sport);
    }

    bpf_map_update_elem(&conn_info_map, &key, conn_info, BPF_ANY);
    return 0;
}

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect_enter(struct sys_enter_connect_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    init_conn_info_connect(ctx);

    struct conn_info_key key = { .pid = pid, .op_type = 1 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (conn_info && conn_info->comm[0] == 'n')
    {
        bpf_printk("enter_connect CLIENT Connect entry: PID=%d, Comm=%s\n", pid, conn_info->comm);
    }

    return 0;
}

SEC("tracepoint/syscalls/sys_exit_connect")
int trace_connect_exit(struct sys_exit_connect_args *ctx)
{
    u32 pid = bpf_get_current_pid_tgid() >> 32;
    int ret = ctx->ret;

    if (ret < 0)
    {
        bpf_printk("exit_connect Connect failed for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    struct conn_info_key key = { .pid = pid, .op_type = 1 };
    struct conn_info_t *conn_info = bpf_map_lookup_elem(&conn_info_map, &key);
    if (!conn_info)
    {
        bpf_printk("exit_connect No connection info found for PID=%d\n", pid);
        return 0;
    }

    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), conn_info->sock_addr) != 0)
    {
        bpf_printk("exit_connect Failed to read sockaddr for PID=%d\n", pid);
        bpf_map_delete_elem(&conn_info_map, &pid);
        return 0;
    }

    if (addr.sin_family == AF_INET)
    {
        conn_info->dst_ip = bpf_ntohl(addr.sin_addr.s_addr);
        conn_info->dport = bpf_ntohs(addr.sin_port);

        bpf_printk("exit_connect CLIENT Connected to server: PID=%d, Comm=%s, IP=%d.%d.%d.%d, Port=%d\n",
                   conn_info->pid, conn_info->comm,
                   (conn_info->dst_ip >> 24) & 0xFF, (conn_info->dst_ip >> 16) & 0xFF,
                   (conn_info->dst_ip >> 8) & 0xFF, conn_info->dst_ip & 0xFF, conn_info->dport);
    }

    bpf_map_update_elem(&conn_info_map, &key, conn_info, BPF_ANY);
    return 0;
}


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
[sudo] password for gaz358: 
INFO [faerie::elf] strtab: 0x3937 symtab 0x3970 relocs 0x39b8 sh_offset 0x39b8
libbpf: prog 'trace_sendto_exit': BPF program load failed: Invalid argument
libbpf: prog 'trace_sendto_exit': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; int trace_sendto_exit(struct sys_exit_sendto_args *ctx)
0: (bf) r6 = r1                       ; R1=ctx() R6_w=ctx()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
1: (85) call bpf_get_current_pid_tgid#14      ; R0_w=scalar()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
2: (77) r0 >>= 32                     ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff))
; u32 pid = bpf_get_current_pid_tgid() >> 32;
3: (63) *(u32 *)(r10 -4) = r0         ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R10=fp0 fp-8=mmmm????
; long ret = ctx->ret;
4: (79) r1 = *(u64 *)(r6 +16)         ; R1_w=scalar() R6_w=ctx()
; if (ret < 0)
5: (65) if r1 s> 0xffffffff goto pc+6         ; R1_w=scalar(smax=-1,umin=0x8000000000000000,var_off=(0x8000000000000000; 0x7fffffffffffffff))
6: (bf) r2 = r10                      ; R2_w=fp0 R10=fp0
; 
7: (07) r2 += -4                      ; R2_w=fp-4
8: (18) r1 = 0xffff8ab1d852c800       ; R1_w=map_ptr(map=conn_info_map_s,ks=8,vs=48)
10: (85) call bpf_map_delete_elem#3
invalid indirect access to stack R2 off=-4 size=8
processed 10 insns (limit 1000000) max_states_per_insn 0 total_states 0 peak_states 0 mark_read 0
-- END PROG LOAD LOG --
libbpf: prog 'trace_sendto_exit': failed to load: -22
libbpf: failed to load object 'tracepoint_bpf'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 22"), RecvError
