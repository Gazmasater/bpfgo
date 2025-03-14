

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

grep CONFIG_BPF_SK_LOOKUP /boot/config-$(uname -r)


bpftool feature probe | grep sk_lookup






