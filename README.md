export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c





clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ bpf2go trace_bpf trace.c
Compiled /home/gaz358/myprog/bpfgo/bpf/trace_bpf_bpfel.o
Stripped /home/gaz358/myprog/bpfgo/bpf/trace_bpf_bpfel.o
Wrote /home/gaz358/myprog/bpfgo/bpf/trace_bpf_bpfel.go
Compiled /home/gaz358/myprog/bpfgo/bpf/trace_bpf_bpfeb.o
Stripped /home/gaz358/myprog/bpfgo/bpf/trace_bpf_bpfeb.o
Wrote /home/gaz358/myprog/bpfgo/bpf/trace_bpf_bpfeb.go
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo/bpf$ 


