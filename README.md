gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map show
[sudo] password for gaz358: 
2: prog_array  name hid_jmp_table  flags 0x0
        key 4B  value 4B  max_entries 1024  memlock 8512B
        owner_prog_type tracing  owner jited
3: hash  flags 0x0
        key 9B  value 1B  max_entries 500  memlock 45984B
4: hash  flags 0x0
        key 9B  value 1B  max_entries 500  memlock 45984B
6: array  name libbpf_global  flags 0x0
        key 4B  value 32B  max_entries 1  memlock 352B
7: array  name pid_iter.rodata  flags 0x480
        key 4B  value 4B  max_entries 1  memlock 8192B
        btf_id 211  frozen
        pids bpftool(4796)
8: array  name libbpf_det_bind  flags 0x0
        key 4B  value 32B  max_entries 1  memlock 352B
