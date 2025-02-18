bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool perf show  trace_events
pid 3536  fd 3: prog_id 58  tracepoint  sys_enter_sendto
pid 3536  fd 5: prog_id 59  tracepoint  sys_exit_sendto
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool perf list  trace_events
pid 3536  fd 3: prog_id 58  tracepoint  sys_enter_sendto
pid 3536  fd 5: prog_id 59  tracepoint  sys_exit_sendto
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 





11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go



gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map dump name trace_events
key:
00 00 00 00
value:
Unknown error 524
key:
01 00 00 00
value:
Unknown error 524
key:
02 00 00 00
value:
Unknown error 524
key:
03 00 00 00
value:
Unknown error 524
key:
04 00 00 00
value:
Unknown error 524
key:
05 00 00 00
value:
Unknown error 524
key:
06 00 00 00
value:
Unknown error 524
key:
07 00 00 00
value:
Unknown error 524
Found 0 elements
