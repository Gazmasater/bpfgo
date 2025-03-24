lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


systemd-timesyn-814     [005] ...21  1026.549144: bpf_trace_printk: !!!lookup sys_exit_sendto FAMILY=2 ADDRESS=185.125.190.58:123 Comm=systemd-timesyn

irq/145-iwlwifi-522     [002] ..s21  1026.630511: bpf_trace_printk: lookup src=185.125.190.58:123 dst=192.168.1.71:56952 protocol=17 FAMILY=2 


