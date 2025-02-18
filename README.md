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



// Получаем путь до корня проекта (можно использовать путь до Perf как базу)
wd, err := os.Getwd()
if err != nil {
    log.Fatalf("failed to get current working directory: %v", err)
}

// Строим путь к файлу eBPF объекта, начиная с корня проекта
eBpfFilePath := filepath.Join(filepath.Dir(wd), "generated", "bpf_x86_bpfel.o")
