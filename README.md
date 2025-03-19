
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



	// Привязываем eBPF-программу к `sk_lookup` в network namespace
	skLookupLink, err := link.AttachNetNs(link.NetNsOptions{
		Program: objs.SkLookup,
		Target:  targetFD,
		Attach:  ebpf.AttachSkLookup,
	})
	if err != nil {
		log.Fatalf("failed to attach sk_lookup program: %v", err)
	}
	defer skLookupLink.Close()

 func link.AttachNetNs(ns int, prog *ebpf.Program) (*link.NetNsLink, error)




