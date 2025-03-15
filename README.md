

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


	cgroupPath := "/sys/fs/cgroup/net_cls/my_cgroup"

	// Шаг 1: Создайте cgroup (если его нет)
	err := exec.Command("mkdir", "-p", cgroupPath).Run()
	if err != nil {
		log.Fatalf("failed to create cgroup: %v", err)
	}

	// Шаг 2: Установите classid для cgroup с использованием sudo
	err = exec.Command("sudo", "tc", "qdisc", "add", "dev", "lo", "clsact").Run()
	if err != nil {
		log.Fatalf("failed to execute tc command: %v", err)
	}

	fmt.Println("Successfully added clsact qdisc to lo interface")

	// Шаг 3: Открыть cgroup для получения дескриптора
	f, err := os.Open(cgroupPath)
	if err != nil {
		log.Fatalf("failed to open cgroup path: %v", err)
	}
	defer f.Close()

	// Получаем дескриптор cgroup (это поле 'Target' в RawAttachProgramOptions)
	cgroupFd := int(f.Fd())

	// Шаг 4: Привязать программу BPF
	err = link.RawAttachProgram(link.RawAttachProgramOptions{
		Program: objs.EchoDispatch,   // Программа BPF
		Attach:  ebpf.AttachSkLookup, // Тип привязки sk_lookup
		Target:  cgroupFd,            // Дескриптор cgroup
	})
	if err != nil {
		log.Fatalf("failed to attach sk_lookup: %v", err)
	}


































