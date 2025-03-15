

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


	err := exec.Command("sudo", "tc", "qdisc", "add", "dev", "lo", "clsact").Run()
	if err != nil {
		log.Fatalf("failed to execute tc command: %v", err)
	}

	fmt.Println("Successfully added clsact qdisc to lo interface")

































