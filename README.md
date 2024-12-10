RUST_BACKTRACE=1 sudo ./ecli run package.json

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ RUST_BACKTRACE=1 sudo ./ecli run package.json
[sudo] password for gaz358: 
INFO [faerie::elf] strtab: 0x2e3 symtab 0x320 relocs 0x368 sh_offset 0x368
thread '<unnamed>' panicked at ecli-lib/src/runner/task_manager/mod.rs:252:22:
called `Result::unwrap()` on an `Err` value: Export map named `ringbuf` found, but no export type is provided
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace
