
nc -l 12345

nc 127.0.0.1 12345

bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


60: sk_lookup  name look_up  tag 74506662657241b3  gpl
        loaded_at 2025-03-19T23:09:40+0300  uid 0
        xlated 320B  jited 204B  memlock 4096B  map_ids 8,9
        btf_id 191
        pids bpfgo(3782)






