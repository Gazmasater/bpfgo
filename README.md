

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


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo ./bpfgo
2025/03/16 01:28:26 failed to attach sk_lookup: attach program: invalid argument
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 

































