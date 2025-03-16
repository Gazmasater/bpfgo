
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

	cgroupPath := "/sys/fs/cgroup/net_cls/my_cgroup"

	// Шаг 1: Создайте cgroup (если его нет)
	err := exec.Command("sudo", "mkdir", "-p", cgroupPath).Run()
	if err != nil {
		log.Fatalf("failed to create cgroup: %v", err)
	}

	// Шаг 2: Установите classid для cgroup с использованием sudo
	err = exec.Command("sudo", "tc", "qdisc", "add", "dev", "lo", "clsact").Run()
	if err != nil {
		log.Fatalf("failed to execute tc command: %v", err)
	}

