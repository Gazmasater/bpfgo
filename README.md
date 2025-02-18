bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftool map pin id <map_id> /sys/fs/bpf/trace_events

bpftool map pin id 11 /sys/fs/bpf/trace_events

bpftool map dump /sys/fs/bpf/trace_events



11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go



sudo bpftool map dump pinned /sys/fs/bpf/trace_events


ls /sys/fs/bpf

sudo mount -t bpf bpf /sys/fs/bpf


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map dump pinned /sys/fs/bpf/trace_events
Error: bpf obj get (/sys/fs/bpf/trace_events): No such file or directory
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ls /sys/fs/bpf
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
