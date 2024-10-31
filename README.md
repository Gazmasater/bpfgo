bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h


INCLUDES := -D__TARGET_ARCH_$(ARCH) -I$(OUTPUT) -I../third_party/libbpf-bootstrap/libbpf/include/uapi -I$(dir $(VMLINUX)) -I$(LIBBLAZESYM_INC) -I/usr/include/bpf


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc ebpf_test.c
thread 'main' panicked at src/config/mod.rs:100:87:
called `Result::unwrap()` on an `Err` value: eunomia data home not found, try setting `EUNOMIA_HOME`
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace


