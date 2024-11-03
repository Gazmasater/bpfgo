bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

BPF_PROG_TYPE_FLOW_DISSECTOR — анализирует содержимое пакета, чтобы извлечь такие данные, как IP-адрес или порт, что помогает в маршрутизации и фильтрации.

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "vmlinux.h"

#include "bpf/bpf_tracing.h"
#include  "bpf/bpf_endian.h"



struct conn_info_t {
    u32 pid;
    u32 ip;
    u16 port;
};

// Создаем BPF-карту для хранения информации о соединениях
//BPF_HASH(conn_map, u32, struct conn_info_t);

// Привязываем функцию к kprobe для inet_accept
SEC("kprobe/inet_accept")
int trace_accept(struct pt_regs *ctx) {
    struct sock *sk = (struct sock *)PT_REGS_PARM1(ctx);
    u32 pid = bpf_get_current_pid_tgid() >> 32;

    struct conn_info_t info = {};
    info.pid = pid;
    
    // Получаем IP-адрес и порт из структуры sock
    u16 dport = sk->__sk_common.skc_dport;
    u32 dip = sk->__sk_common.skc_daddr;
    
    info.ip = dip;
    info.port = __bpf_ntohs(dport);
  //bpf_printk("Connection accepted: PID=%d, IP=%x, PORT=%d\n", info.pid, info.ip, info.port);
      bpf_printk("Connection accepted: PID=%d\n", pid);
    // Сохраняем информацию о соединении в BPF-карте
    //conn_map.update(&pid, &info);

    return 0;
}



az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x373 symtab 0x3b0 relocs 0x3f8 sh_offset 0x3f8
libbpf: prog 'trace_accept': BPF program load failed: Invalid argument
libbpf: prog 'trace_accept': -- BEGIN PROG LOAD LOG --
0: R1=ctx() R10=fp0
; u32 pid = bpf_get_current_pid_tgid() >> 32;
0: (85) call bpf_get_current_pid_tgid#14      ; R0_w=scalar()
; u32 pid = bpf_get_current_pid_tgid() >> 32;
1: (77) r0 >>= 32                     ; R0_w=scalar(smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff))
; bpf_printk("Connection accepted: PID=%d\n", pid);
2: (18) r1 = 0xffff9713cca36910       ; R1_w=map_value(map=fentry_b.rodata,ks=4,vs=29)
4: (b7) r2 = 29                       ; R2_w=29
5: (bf) r3 = r0                       ; R0_w=scalar(id=1,smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff)) R3_w=scalar(id=1,smin=0,smax=umax=0xffffffff,var_off=(0x0; 0xffffffff))
6: (85) call bpf_trace_printk#6
cannot call GPL-restricted function from non-GPL compatible program
processed 6 insns (limit 1000000) max_states_per_insn 0 total_states 0 peak_states 0 mark_read 0
-- END PROG LOAD LOG --
libbpf: prog 'trace_accept': failed to load: -22
libbpf: failed to load object 'fentry_bpf���s'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 22"), RecvError
