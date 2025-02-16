export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c




clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ export GOPACKAGE=send             
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ bpf2go -output-stem trace_bpf trace.c
Error: expected at least two arguments




