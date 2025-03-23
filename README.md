lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo cat /sys/kernel/debug/tracing/trace_pipe|grep "lookup"
            curl-6984    [006] ..s21   786.234367: bpf_trace_printk: sk_lookup src=127.0.0.1:58870 dst=127.0.0.53:53 protocol=17 FAMILY=2
            curl-6984    [006] ..s21   786.234383: bpf_trace_printk: sk_lookup src=127.0.0.1:58870 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.234748: bpf_trace_printk: sk_lookup src=127.0.0.1:43549 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.235354: bpf_trace_printk: sk_lookup src=127.0.0.1:32906 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.235812: bpf_trace_printk: sk_lookup src=127.0.0.1:56205 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.236088: bpf_trace_printk: sk_lookup src=127.0.0.1:41998 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.248203: bpf_trace_printk: sk_lookup src=127.0.0.1:48585 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.249428: bpf_trace_printk: sk_lookup src=127.0.0.1:58015 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] .Ns21   786.250741: bpf_trace_printk: sk_lookup src=127.0.0.1:36410 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [003] ..s21   786.251917: bpf_trace_printk: sk_lookup src=127.0.0.1:50811 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [007] ..s21   786.252923: bpf_trace_printk: sk_lookup src=127.0.0.1:59010 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [007] ..s21   786.253479: bpf_trace_printk: sk_lookup src=127.0.0.1:45044 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [007] ..s21   786.254582: bpf_trace_printk: sk_lookup src=127.0.0.1:36129 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [007] ..s21   786.255004: bpf_trace_printk: sk_lookup src=127.0.0.1:43300 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [005] ..s21   786.267972: bpf_trace_printk: sk_lookup src=127.0.0.1:55955 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [005] ..s21   786.268258: bpf_trace_printk: sk_lookup src=127.0.0.1:49149 dst=127.0.0.53:53 protocol=17 FAMILY=2
 irq/149-iwlwifi-489     [006] ..s21   799.857541: bpf_trace_printk: sk_lookup src=0.0.0.0:47029 dst=0.0.0.0:546 protocol=17 FAMILY=10
           bpfgo-3744    [005] ..s21   817.794864: bpf_trace_printk: sk_lookup src=127.0.0.1:51477 dst=127.0.0.53:53 protocol=17 FAMILY=2
           bpfgo-3744    [005] ..s21   817.795161: bpf_trace_printk: sk_lookup src=127.0.0.1:60306 dst=127.0.0.53:53 protocol=17 FAMILY=2

