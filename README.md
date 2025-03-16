
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

	targetFD, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("failed to open network namespace: %v", err)
	}
	defer targetFD.Close()

	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Target:  int(targetFD.Fd()),
		Program: objs.EchoDispatch,
		Attach:  ebpf.AttachSkLookup,
	})

	if err != nil {
		log.Fatalf("failed to attach program: %v", err)
	}







