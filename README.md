
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


	if err := syscall.Unshare(syscall.CLONE_NEWNET); err != nil {
		log.Fatalf("Ошибка создания нового network namespace: %v", err)
	}
	fmt.Println("Создано новое сетевое пространство")

	// Открываем дескриптор нового namespace
	newNS, err := os.Open("/proc/self/ns/net")
	if err != nil {
		log.Fatalf("Ошибка открытия дескриптора нового namespace: %v", err)
	}
	defer newNS.Close()

	fmt.Printf("Дескриптор нового namespace: %d\n", newNS.Fd())

	skLookupLink, err := link.AttachNetNs(int(newNS.Fd()), objs.LookUp)
	if err != nil {
		log.Fatalf("failed to attach sk_lookup program: %v", err)
	}
	defer skLookupLink.Close()


 ip netns add testns
ip netns exec testns ip link set lo up
ip netns exec testns nc -u 127.0.0.1 12345







