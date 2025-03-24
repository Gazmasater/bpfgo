lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


bpfgo-21357   [006] ..s21  5046.280491: bpf_trace_printk: lookup src=127.0.0.1:34329 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-21357   [006] ..s21  5046.281131: bpf_trace_printk: lookup src=127.0.0.1:47709 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-21357   [000] ..s21  5046.281844: bpf_trace_printk: lookup src=127.0.0.1:46854 dst=127.0.0.53:53 protocol=17 FAMILY=2 



