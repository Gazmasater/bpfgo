
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

err = link.RawAttachProgram(link.RawAttachProgramOptions{
		//ProgFD:     progFD,              // Файловый дескриптор программы BPF
		//TargetFD:   int(targetFD.Fd()),  // Файловый дескриптор целевого сетевого пространства
		Attach:     ebpf.AttachSkLookup, // Тип привязки для sk_lookup
		//AttachType: ebpf.AttachSkLookup,
	})



