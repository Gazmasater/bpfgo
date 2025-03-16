
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

err := link.RawAttachProgram(link.RawAttachProgramOptions{
    Target:   int(targetFD.Fd()),   // Файловый дескриптор целевого объекта (например, сетевого пространства имен)
    Program:  myBpfProgram,         // Указатель на загруженную программу BPF
    Attach:   ebpf.AttachSkLookup,  // Тип привязки, соответствующий BPF_SK_LOOKUP
    Flags:    0,                    // Необязательные флаги
    Anchor:   Anchor{},             // Если необходимо, можете указать якорь (можно оставить пустым)
})
if err != nil {
    log.Fatalf("failed to attach program: %v", err)
}



