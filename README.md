lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


tcp   ESTAB 0      0                  192.168.1.71:48402     34.107.243.93:443   users:(("firefox",pid=2876,fd=133))



