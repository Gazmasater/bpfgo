bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf

//go:generate go run github.com/cilium/ebpf/cmd/bpf2go -cc clang fentry fentry.c -- -target x86_64 -g -O2 -D __TARGET_ARCH_x86

grep KPROBES /boot/config-$(uname -r)



CONFIG_KPROBES=y




++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecli run package.json
INFO [faerie::elf] strtab: 0x56b symtab 0x5a8 relocs 0x5f0 sh_offset 0x5f0
libbpf: Failed to bump RLIMIT_MEMLOCK (err = -1), you might need to do it explicitly!
libbpf: Error in bpf_object__probe_loading():Operation not permitted(1). Couldn't load trivial BPF program. Make sure your kernel supports BPF (CONFIG_BPF_SYSCALL=y) and/or that RLIMIT_MEMLOCK is set to big enough value.
libbpf: failed to load object 'fentry_bpf�97p'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 1"), RecvError
