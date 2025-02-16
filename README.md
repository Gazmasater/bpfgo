export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c




clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go




