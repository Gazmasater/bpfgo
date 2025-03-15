

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

	echoDispatchLink, err := link.AttachKprobe("echo_dispatch", bpfObjs.EchoDispatch, nil)
	if err != nil {
		log.Fatalf("failed to attach echo_dispatch program: %v", err)
	}
	defer echoDispatchLink.Close()


































