struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); // number of CPUs
} trace_events SEC(".maps");


struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); // количество CPU
} trace_events SEC(".maps.export");


    bpf_perf_event_output(ctx, &trace_events, BPF_F_CURRENT_CPU, conn_info, sizeof(*conn_info));




gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ./ecc tracepoint.c
INFO [ecc_rs::bpf_compiler] Compiling bpf object...
INFO [ecc_rs::bpf_compiler] $ "/tmp/.tmpbTxAAV/bin/bpftool" CommandArgs { inner: ["gen", "skeleton", "tracepoint.bpf.o", "-j"] }
INFO [ecc_rs::bpf_compiler] {"error":"failed to open BPF object file: Relocation failed"}

ERROR [ecc_rs::bpf_compiler] libbpf: elf: skipping unrecognized data section(13) .maps.export
libbpf: prog 'trace_sendto_exit': bad map relo against 'trace_events' in section '.maps.export'

Error: Failed to compile

Caused by:
    Failed to generate skeleton json(exit code = Some(91))
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
