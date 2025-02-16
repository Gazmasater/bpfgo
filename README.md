clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

    void *addr_ptr = (void *)ctx->addr;
    bpf_map_update_elem(&addr_map, &pid, &addr_ptr, BPF_ANY);


az358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ go generate
Compiled /home/gaz358/myprog/bpfgo/bpf/trace_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/bpf/trace_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/bpf/trace_bpfel.go
Compiled /home/gaz358/myprog/bpfgo/bpf/trace_bpfeb.o
Stripped /home/gaz358/myprog/bpfgo/bpf/trace_bpfeb.o
Wrote /home/gaz358/myprog/bpfgo/bpf/trace_bpfeb.go
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ 
