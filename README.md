bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

bpftool map pin id <map_id> /sys/fs/bpf/trace_events

bpftool map pin id 11 /sys/fs/bpf/trace_events


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


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map show
2: prog_array  name hid_jmp_table  flags 0x0
        key 4B  value 4B  max_entries 1024  memlock 8512B
        owner_prog_type tracing  owner jited
3: hash  flags 0x0
        key 9B  value 1B  max_entries 500  memlock 45984B
4: hash  flags 0x0
        key 9B  value 1B  max_entries 500  memlock 45984B
5: hash  flags 0x0
        key 9B  value 1B  max_entries 500  memlock 45984B
9: hash  name addr_map  flags 0x0
        key 4B  value 4B  max_entries 1024  memlock 83648B
        btf_id 190
10: hash  name conn_info_map  flags 0x0
        key 4B  value 36B  max_entries 1024  memlock 116672B
        btf_id 191
12: array  name .rodata  flags 0x480
        key 4B  value 301B  max_entries 1  memlock 8192B
        btf_id 192  frozen
13: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
17: hash  name conn_info_map  flags 0x0
        key 4B  value 36B  max_entries 1024  memlock 116672B
        btf_id 201
        pids Perf(4934)
18: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Perf(4934)
20: array  name .rodata  flags 0x480
        key 4B  value 301B  max_entries 1  memlock 8192B
        btf_id 202  frozen
        pids Perf(4934)
21: array  name .bss  flags 0x400
        key 4B  value 8B  max_entries 1  memlock 8192B
        btf_id 203
        pids Perf(4934)
22: hash  name addr_map  flags 0x0
        key 4B  value 4B  max_entries 1024  memlock 83648B
        btf_id 204
        pids Perf(4934)
24: array  name libbpf_global  flags 0x0
        key 4B  value 32B  max_entries 1  memlock 352B
25: array  name pid_iter.rodata  flags 0x480
        key 4B  value 4B  max_entries 1  memlock 8192B
        btf_id 216  frozen
        pids bpftool(5162)
26: array  name libbpf_det_bind  flags 0x0
        key 4B  value 32B  max_entries 1  memlock 352B

        gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map dump pinned /sys/fs/bpf/trace_events
[sudo] password for gaz358: 
Error: bpf obj get (/sys/fs/bpf/trace_events): No such file or directory
