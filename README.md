bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang fentry fentry.c -- -target x86_64 -g -O2 -D __TARGET_ARCH_x86

go run github.com/cilium/ebpf/cmd/bpf2go -cc clang fentry fentry.c -- -target x86_64-pc-linux-gnu -g -O2 -D __TARGET_ARCH_x86


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -s
Linux
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -n
gaz358-BOD-WXX9
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -r
6.8.0-47-generic
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -v
#47-Ubuntu SMP PREEMPT_DYNAMIC Fri Sep 27 21:40:26 UTC 2024
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -m
x86_64
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -p
x86_64
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -i
x86_64
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ uname -o
GNU/Linux


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ go generate
Compiled /home/gaz358/myprog/bpfgo/fentry_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/fentry_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/fentry_bpfel.go
Compiled /home/gaz358/myprog/bpfgo/fentry_bpfeb.o
Stripped /home/gaz358/myprog/bpfgo/fentry_bpfeb.o
Wrote /home/gaz358/myprog/bpfgo/fentry_bpfeb.go
/home/gaz358/myprog/bpfgo/fentry.c:56:5: error: The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb
   56 | int BPF_KPROBE(tcp_connect, struct sock *sk) {
      |     ^
/usr/include/bpf/bpf_tracing.h:817:20: note: expanded from macro 'BPF_KPROBE'
  817 |         return ____##name(___bpf_kprobe_args(args));                        \
      |                           ^
/usr/include/bpf/bpf_tracing.h:797:41: note: expanded from macro '___bpf_kprobe_args'
  797 | #define ___bpf_kprobe_args(args...)     ___bpf_apply(___bpf_kprobe_args, ___bpf_narg(args))(args)
      |                                         ^
/usr/include/bpf/bpf_helpers.h:195:29: note: expanded from macro '___bpf_apply'
  195 | #define ___bpf_apply(fn, n) ___bpf_concat(fn, n)
      |                             ^
note: (skipping 2 expansions in backtrace; use -fmacro-backtrace-limit=0 to see all)
/usr/include/bpf/bpf_tracing.h:789:72: note: expanded from macro '___bpf_kprobe_args1'
  789 | #define ___bpf_kprobe_args1(x)          ___bpf_kprobe_args0(), (void *)PT_REGS_PARM1(ctx)
      |                                                                        ^
/usr/include/bpf/bpf_tracing.h:563:29: note: expanded from macro 'PT_REGS_PARM1'
  563 | #define PT_REGS_PARM1(x) ({ _Pragma(__BPF_TARGET_MISSING); 0l; })
      |                             ^
<scratch space>:27:6: note: expanded from here
   27 |  GCC error "The eBPF is using target specific macros, please provide -target that is not bpf, bpfel or bpfeb"
      |      ^
1 error generated.
Error: compile: exit status 1
exit status 1
main.go:31: running "go": exit status 1
