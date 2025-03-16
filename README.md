
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

cmd := exec.Command("sudo", "-S", "/usr/sbin/tc", "qdisc", "add", "dev", "lo", "clsact")
cmd.Stdin = strings.NewReader("qwert\n") // Передаем пароль
err := cmd.Run()
if err != nil {
    log.Fatalf("failed to execute tc command: %v", err)
}


echo "qwert" | sudo -S tc qdisc add dev lo clsact


