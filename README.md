

nc -l 12345

nc 127.0.0.1 12345



bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)

grep CONFIG_BPF_SK_LOOKUP /boot/config-$(uname -r)


bpftool feature probe | grep sk_lookup

az358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo bpftool feature probe | grep sk_lookup
[sudo] password for gaz358: 
eBPF program_type sk_lookup is available
        - bpf_sk_lookup_tcp
        - bpf_sk_lookup_udp
        - bpf_sk_lookup_tcp
        - bpf_sk_lookup_udp
        - bpf_sk_lookup_tcp
        - bpf_sk_lookup_udp
        - bpf_sk_lookup_tcp
        - bpf_sk_lookup_udp
        - bpf_sk_lookup_tcp
        - bpf_sk_lookup_udp
        - bpf_sk_lookup_tcp
        - bpf_sk_lookup_udp
eBPF helpers supported for program type sk_lookup:
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 






