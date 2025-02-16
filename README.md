bpf2go -o trace_bpf.go trace_bpf.c

bpf2go -output-stem trace_bpf trace.c




clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

