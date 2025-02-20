bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


sudo bpftool perf show  trace_events
sudo bpftool perf list  trace_events

sudo bpftool map dump id 11




11: perf_event_array  name trace_events  flags 0x0
        key 4B  value 4B  max_entries 8  memlock 384B
        pids Load(4849)


export GOPACKAGE=your_package_name
bpf2go -output-stem trace_bpf trace.c

bpf2go trace_bpf trace.c



clang -O2 -g -target bpf -c trace.bpf.c -o trace.bpf.o

go get github.com/cilium/ebpf/cmd/bpf2go

which bpf2go


bpf2go -output-dir $(pwd) \
  -tags linux \
  -type trace_info \
  -go-package main \
  target_amd64_bpf \
  $(pwd)/trace.c -- -I$(pwd)


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/trace_pipe|grep "Comm=udp"
      udp_server-6628    [004] ...21   718.155286: bpf_trace_printk: SERVER sys_enter_recvfrom:FAMILY=49725 PID=6628, Comm=udp_server
           <...>-6669    [006] ...21   722.664758: bpf_trace_printk: SERVER sys_enter_sendto: PID=6669, Comm=udp_client
           <...>-6669    [006] ...21   722.664826: bpf_trace_printk: UDP sys_exit_sendto: Connection: PID=6669, Comm=udp_client, IP=0.0.0.0, Port=33333
      udp_server-6628    [002] ...21   722.664863: bpf_trace_printk: SERVER sys_enter_recvfrom:FAMILY=2 PID=6628, Comm=udp_server

