bpf2go -o trace_bpf.go trace_bpf.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

    void *addr_ptr = (void *)ctx->addr;
    bpf_map_update_elem(&addr_map, &pid, &addr_ptr, BPF_ANY);


