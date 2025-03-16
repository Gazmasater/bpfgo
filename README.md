
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

targetFD, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("failed to open network namespace: %v", err)
	}
	defer targetFD.Close()

	// Создаем ссылку для привязки программы с типом BPF_SK_LOOKUP
	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  int(targetFD.Fd()),  // Файловый дескриптор целевого объекта (например, сетевого пространства имен)
		Program: objs.EchoDispatch,   // Указатель на загруженную программу BPF
		Attach:  ebpf.AttachSkLookup, // Тип привязки, соответствующий BPF_SK_LOOKUP
		// Flags:   0,                   // Необязательные флаги
		// Anchor:  nil,                 // Если необходимо, можете указать якорь (можно оставить пустым)
	})

	if err != nil {
		log.Fatalf("failed to attach program: %v", err)
	}

progFD := objs.EchoDispatch.FD()

	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:           int(targetFD.Fd()),
		Program:          objs.EchoDispatch,
		Attach:           ebpf.AttachSkLookup,
		Anchor:           nil,
		Flags:            0,
		ExpectedRevision: 0,
	})

	if err != nil {
		log.Fatalf("failed to attach program: %v", err)
	}


type RawAttachProgramOptions struct {
    // Target to query. This is usually a file descriptor but may refer to
    // something else based on the attach type.
    Target int
    // Program to attach.
    Program *ebpf.Program
    // Attach must match the attach type of Program.
    Attach ebpf.AttachType
    // Attach relative to an anchor. Optional.
    Anchor Anchor
    // Flags control the attach behaviour. Specify an Anchor instead of
    // F_LINK, F_ID, F_BEFORE, F_AFTER and F_REPLACE. Optional.
    Flags uint32
    // Only attach if the internal revision matches the given value.
    ExpectedRevision uint64
}






