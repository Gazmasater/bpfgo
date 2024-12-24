gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x2508 symtab 0x2540 relocs 0x2588 sh_offset 0x2588
thread '<unnamed>' panicked at ecli-lib/src/runner/task_manager/mod.rs:252:22:
called `Result::unwrap()` on an `Err` value: Export map named `events` found, but no export type is provided
note: run with `RUST_BACKTRACE=1` environment variable to display a backtrace
