lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


   Socket Thread-17385   [005] ...21 55648.683483: bpf_trace_printk: !!!lookup sys_exit_sendto FAMILY=2 ADDRESS=34.117.188.166:443 Comm=Socket Thread 
   Socket Thread-17385   [005] ...21 55648.683545: bpf_trace_printk: !!!lookup sys_exit_sendto FAMILY=2 ADDRESS=34.117.188.166:443 Comm=Socket Thread 
           bpfgo-28323   [007] ..s21 55648.683833: bpf_trace_printk: lookup src=127.0.0.1:34529 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-28323   [002] ...21 55648.684007: bpf_trace_printk: #####lookup sys_exit_recvfrom ADDRESS=127.0.0.53:53 Comm=bpfgo 
           bpfgo-28323   [002] ..s21 55648.684242: bpf_trace_printk: lookup src=127.0.0.1:41141 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-28323   [002] ...21 55648.684579: bpf_trace_printk: #####lookup sys_exit_recvfrom ADDRESS=127.0.0.53:53 Comm=bpfgo 
           bpfgo-28323   [002] ..s21 55648.684752: bpf_trace_printk: lookup src=127.0.0.1:40172 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-28323   [002] ...21 55648.684950: bpf_trace_printk: #####lookup sys_exit_recvfrom ADDRESS=127.0.0.53:53 Comm=bpfgo 
           bpfgo-28323   [002] ..s21 55648.685064: bpf_trace_printk: lookup src=127.0.0.1:41450 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-28323   [007] ...21 55648.685436: bpf_trace_printk: #####lookup sys_exit_recvfrom ADDRESS=127.0.0.53:53 Comm=bpfgo 
 irq/146-iwlwifi-554     [000] ..s21 55648.712325: bpf_trace_printk: lookup src=34.117.188.166:443 dst=192.168.1.71:36237 protocol=17 FAMILY=2 
   Socket Thread-17385   [005] ...21 55648.712411: bpf_trace_printk: #####lookup sys_exit_recvfrom ADDRESS=34.117.188.166:443 Comm=Socket Thread 
 irq/146-iwlwifi-554     [000] ..s21 55648.712516: bpf_trace_printk: lookup src=34.117.188.166:443 dst=192.168.1.71:36237 protocol=17 FAMILY=2 

 


