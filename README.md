struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(max_entries, 128);
} trace_events SEC(".maps");



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

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0xe79 symtab 0xeb8 relocs 0xf00 sh_offset 0xf00
thread '<unnamed>' panicked at ecli-lib/src/runner/task_manager/mod.rs:252:22:
called `Result::unwrap()` on an `Err` value: Export map named `trace_events` found, but no export type is provided
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace
