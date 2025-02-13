struct
{
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(u32));
    __uint(value_size, sizeof(u32));
    __uint(max_entries, 128); // number of CPUs
} trace_events SEC(".maps");





gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
[sudo] password for gaz358: 
INFO [faerie::elf] strtab: 0xe18 symtab 0xe50 relocs 0xe98 sh_offset 0xe98
thread '<unnamed>' panicked at ecli-lib/src/runner/task_manager/mod.rs:252:22:
called `Result::unwrap()` on an `Err` value: Export map named `trace_events` found, but no export type is provided
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace
