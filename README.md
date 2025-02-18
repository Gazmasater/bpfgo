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


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map dump /sys/fs/bpf/trace_events
[sudo] password for gaz358: 
Usage: bpftool map { show | list }   [MAP]
       bpftool map create     FILE type TYPE key KEY_SIZE value VALUE_SIZE \
                                  entries MAX_ENTRIES name NAME [flags FLAGS] \
                                  [inner_map MAP] [offload_dev NAME]
       bpftool map dump       MAP
       bpftool map update     MAP [key DATA] [value VALUE] [UPDATE_FLAGS]
       bpftool map lookup     MAP [key DATA]
       bpftool map getnext    MAP [key DATA]
       bpftool map delete     MAP  key DATA
       bpftool map pin        MAP  FILE
       bpftool map event_pipe MAP [cpu N index M]
       bpftool map peek       MAP
       bpftool map push       MAP value VALUE
       bpftool map pop        MAP
       bpftool map enqueue    MAP value VALUE
       bpftool map dequeue    MAP
       bpftool map freeze     MAP
       bpftool map help

       MAP := { id MAP_ID | pinned FILE | name MAP_NAME }
       DATA := { [hex] BYTES }
       PROG := { id PROG_ID | pinned FILE | tag PROG_TAG | name PROG_NAME }
       VALUE := { DATA | MAP | PROG }
       UPDATE_FLAGS := { any | exist | noexist }
       TYPE := { hash | array | prog_array | perf_event_array | percpu_hash |
                 percpu_array | stack_trace | cgroup_array | lru_hash |
                 lru_percpu_hash | lpm_trie | array_of_maps | hash_of_maps |
                 devmap | devmap_hash | sockmap | cpumap | xskmap | sockhash |
                 cgroup_storage | reuseport_sockarray | percpu_cgroup_storage |
                 queue | stack | sk_storage | struct_ops | ringbuf | inode_storage |
                 task_storage | bloom_filter | user_ringbuf | cgrp_storage | arena }
       OPTIONS := { {-j|--json} [{-p|--pretty}] | {-d|--debug} |
                    {-f|--bpffs} | {-n|--nomount} }
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool map pin id 11 /sys/fs/bpf/trace_events
Error: get map by id (11): No such file or directory
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 
