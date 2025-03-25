lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


127.0.0.53:53 dst=127.0.0.1:5500 protocol=17 FAMILY=2 
 systemd-resolve-789     [001] ..s21   625.819110: bpf_trace_printk: lookup src=127.0.0.1:5500 dst=127.0.0.53:53 protocol=17 FAMILY=2 

 


