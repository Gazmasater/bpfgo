clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

    void *addr_ptr = (void *)ctx->addr;
    bpf_map_update_elem(&addr_map, &pid, &addr_ptr, BPF_ANY);


    struct sockaddr_in addr;
    if (bpf_probe_read(&addr, sizeof(addr), ctx->addr) != 0) {
        return 0; // Ошибка при чтении данных
    }

    bpf_map_update_elem(&addr_map, &pid, &addr, BPF_ANY);
