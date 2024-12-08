gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./ecli run package.json
INFO [faerie::elf] strtab: 0x246c symtab 0x24a8 relocs 0x24f0 sh_offset 0x24f0
libbpf: map 'output': failed to create: Invalid argument(-22)
libbpf: failed to load object 'tracepoint_bpf'
Error: Failed to run native eBPF program

Caused by:
    Bpf error: Failed to start polling: Bpf("Failed to load and attach: Failed to load bpf object\n\nCaused by:\n    System error, errno: 22"), RecvError
gaz358@gaz358-BOD-WXX9:~/myprog/b
