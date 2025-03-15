

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

err := link.RawAttachProgram(link.RawAttachProgramOptions{
    Program: objs.EchoDispatch,   // Загрузка программы BPF
    Attach:  ebpf.AttachSkLookup, // Тип привязки для sk_lookup
})
if err != nil {
    log.Fatalf("failed to attach sk_lookup: %v", err)
}


































