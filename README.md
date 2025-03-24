lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


1:39092 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   261.955912: bpf_trace_printk: lookup src=127.0.0.1:47796 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-1243    [007] ..s21   305.362172: bpf_trace_printk: lookup src=127.0.0.1:46439 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-1243    [007] ..s21   305.362184: bpf_trace_printk: lookup src=127.0.0.1:46439 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.362516: bpf_trace_printk: lookup src=127.0.0.1:59787 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.362972: bpf_trace_printk: lookup src=127.0.0.1:56693 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.363239: bpf_trace_printk: lookup src=127.0.0.1:60442 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.363349: bpf_trace_printk: lookup src=127.0.0.1:37246 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.363549: bpf_trace_printk: lookup src=127.0.0.1:41235 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.363645: bpf_trace_printk: lookup src=127.0.0.1:45080 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.376433: bpf_trace_printk: lookup src=127.0.0.1:51699 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.376636: bpf_trace_printk: lookup src=127.0.0.1:37457 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.492149: bpf_trace_printk: lookup src=127.0.0.1:38073 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.492405: bpf_trace_printk: lookup src=127.0.0.1:44902 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.726139: bpf_trace_printk: lookup src=127.0.0.1:57888 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.726468: bpf_trace_printk: lookup src=127.0.0.1:43596 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.973178: bpf_trace_printk: lookup src=127.0.0.1:47255 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   305.973429: bpf_trace_printk: lookup src=127.0.0.1:33267 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.145376: bpf_trace_printk: lookup src=127.0.0.1:43275 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.145595: bpf_trace_printk: lookup src=127.0.0.1:40996 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.146064: bpf_trace_printk: lookup src=127.0.0.1:59281 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.146284: bpf_trace_printk: lookup src=127.0.0.1:60799 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.146575: bpf_trace_printk: lookup src=127.0.0.1:58644 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.146859: bpf_trace_printk: lookup src=127.0.0.1:51735 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [004] ..s21   306.147278: bpf_trace_printk: lookup src=127.0.0.1:43441 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.147640: bpf_trace_printk: lookup src=127.0.0.1:52660 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.147996: bpf_trace_printk: lookup src=127.0.0.1:51618 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.148205: bpf_trace_printk: lookup src=127.0.0.1:33513 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-1244    [004] ..s21   306.333287: bpf_trace_printk: lookup src=127.0.0.1:34951 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-1244    [004] ..s21   306.333301: bpf_trace_printk: lookup src=127.0.0.1:34951 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.333465: bpf_trace_printk: lookup src=127.0.0.1:54126 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.334086: bpf_trace_printk: lookup src=127.0.0.1:45483 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.334447: bpf_trace_printk: lookup src=127.0.0.1:41048 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.334604: bpf_trace_printk: lookup src=127.0.0.1:46888 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.334851: bpf_trace_printk: lookup src=127.0.0.1:58585 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.334986: bpf_trace_printk: lookup src=127.0.0.1:50696 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.335201: bpf_trace_printk: lookup src=127.0.0.1:44194 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.335322: bpf_trace_printk: lookup src=127.0.0.1:54359 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.335531: bpf_trace_printk: lookup src=127.0.0.1:54240 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.335736: bpf_trace_printk: lookup src=127.0.0.1:58330 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.335978: bpf_trace_printk: lookup src=127.0.0.1:33945 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.336116: bpf_trace_printk: lookup src=127.0.0.1:33002 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-4188    [004] ..s21   306.617644: bpf_trace_printk: lookup src=127.0.0.1:57155 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-4188    [004] ..s21   306.617660: bpf_trace_printk: lookup src=127.0.0.1:57155 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.617828: bpf_trace_printk: lookup src=127.0.0.1:44897 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.618587: bpf_trace_printk: lookup src=127.0.0.1:35951 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.618988: bpf_trace_printk: lookup src=127.0.0.1:44015 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.619136: bpf_trace_printk: lookup src=127.0.0.1:42131 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.619398: bpf_trace_printk: lookup src=127.0.0.1:40427 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.619538: bpf_trace_printk: lookup src=127.0.0.1:41420 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.629968: bpf_trace_printk: lookup src=127.0.0.1:48091 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.630278: bpf_trace_printk: lookup src=127.0.0.1:39278 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.741513: bpf_trace_printk: lookup src=127.0.0.1:36264 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.741778: bpf_trace_printk: lookup src=127.0.0.1:49892 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.869402: bpf_trace_printk: lookup src=127.0.0.1:49648 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.869823: bpf_trace_printk: lookup src=127.0.0.1:45219 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.987057: bpf_trace_printk: lookup src=127.0.0.1:56425 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.987306: bpf_trace_printk: lookup src=127.0.0.1:57558 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [000] ..s21   306.987654: bpf_trace_printk: lookup src=127.0.0.1:52109 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.987999: bpf_trace_printk: lookup src=127.0.0.1:45254 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.988328: bpf_trace_printk: lookup src=127.0.0.1:60277 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.988497: bpf_trace_printk: lookup src=127.0.0.1:45792 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.988805: bpf_trace_printk: lookup src=127.0.0.1:60808 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   306.988971: bpf_trace_printk: lookup src=127.0.0.1:40598 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   307.952730: bpf_trace_printk: lookup src=127.0.0.1:44451 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   307.952988: bpf_trace_printk: lookup src=127.0.0.1:58806 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   307.982909: bpf_trace_printk: lookup src=127.0.0.1:36427 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   307.983079: bpf_trace_printk: lookup src=127.0.0.1:54687 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   308.122821: bpf_trace_printk: lookup src=127.0.0.1:54964 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   308.122948: bpf_trace_printk: lookup src=127.0.0.1:44822 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-1245    [006] ..s21   311.086350: bpf_trace_printk: lookup src=127.0.0.1:38054 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           snapd-1245    [006] ..s21   311.086358: bpf_trace_printk: lookup src=127.0.0.1:38054 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.086508: bpf_trace_printk: lookup src=127.0.0.1:54368 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.086734: bpf_trace_printk: lookup src=127.0.0.1:40600 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.086957: bpf_trace_printk: lookup src=127.0.0.1:57248 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087038: bpf_trace_printk: lookup src=127.0.0.1:56008 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087209: bpf_trace_printk: lookup src=127.0.0.1:55966 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087286: bpf_trace_printk: lookup src=127.0.0.1:45206 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087415: bpf_trace_printk: lookup src=127.0.0.1:41848 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087485: bpf_trace_printk: lookup src=127.0.0.1:32788 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087608: bpf_trace_printk: lookup src=127.0.0.1:33761 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087680: bpf_trace_printk: lookup src=127.0.0.1:53654 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087815: bpf_trace_printk: lookup src=127.0.0.1:51273 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3462    [003] ..s21   311.087882: bpf_trace_printk: lookup src=127.0.0.1:33634 dst=127.0.0.53:53 protocol=17 FAMILY=2 
 irq/144-iwlwifi-494     [001] ..s21   311.159231: bpf_trace_printk: lookup src=185.125.188.58:443 dst=192.168.1.71:44266 protocol=6 FAMILY=2 
 irq/144-iwlwifi-494     [001] ..s21   311.159245: bpf_trace_printk: lookup src=185.125.188.58:443 dst=192.168.1.71:44266 protocol=6 FAMILY=2 
 irq/150-iwlwifi-501     [007] ..s21   311.818576: bpf_trace_printk: lookup src=185.125.188.59:443 dst=192.168.1.71:52020 protocol=6 FAMILY=2 
 irq/143-iwlwifi-492     [000] ..s21   318.446613: bpf_trace_printk: lookup src=185.125.188.58:443 dst=192.168.1.71:44270 protocol=6 FAMILY=2 
           <...>-3114    [004] ..s21   333.071893: bpf_trace_printk: lookup src=127.0.0.1:60765 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           <...>-3114    [004] ..s21   333.072004: bpf_trace_printk: lookup src=127.0.0.1:62379 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           <...>-3464    [006] ..s21   333.072783: bpf_trace_printk: lookup src=127.0.0.1:43107 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.073509: bpf_trace_printk: lookup src=127.0.0.1:39304 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.074035: bpf_trace_printk: lookup src=127.0.0.1:38953 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.074327: bpf_trace_printk: lookup src=127.0.0.1:43402 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.074786: bpf_trace_printk: lookup src=127.0.0.1:38516 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.075114: bpf_trace_printk: lookup src=127.0.0.1:43273 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.075569: bpf_trace_printk: lookup src=127.0.0.1:35212 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.075803: bpf_trace_printk: lookup src=127.0.0.1:55817 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.087037: bpf_trace_printk: lookup src=127.0.0.1:40051 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.087479: bpf_trace_printk: lookup src=127.0.0.1:57766 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.100088: bpf_trace_printk: lookup src=127.0.0.1:54635 dst=127.0.0.53:53 protocol=17 FAMILY=2 
           bpfgo-3464    [006] ..s21   333.100297: bpf_trace_printk: lookup src=127.0.0.1:60881 dst=127.0.0.53:53 protocol=17 FAMILY=2 
 irq/148-iwlwifi-499     [005] ..s21   360.080480: bpf_trace_printk: lookup src=185.125.190.27:443 dst=192.168.1.71:42598 protocol=6 FAMILY=2 
 irq/147-iwlwifi-497     [004] ..s21   367.184222: bpf_trace_printk: lookup src=0.0.0.0:47029 dst=0.0.0.0:546 protocol=17 FAMILY=10 



PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.189.173.1(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=802 Comm=systemd-timesyn ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.56(prod-ntp-3.ntp1.ps5.canonical.com.), DstPort: 123 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.189.173.12(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.42.65.94(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.42.65.94(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.42.65.94(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.42.65.94(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=802 Comm=systemd-timesyn ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.56(prod-ntp-3.ntp1.ps5.canonical.com.), DstPort: 123 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3200 Comm=code ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 20.42.65.94(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=802 Comm=systemd-timesyn ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.56(prod-ntp-3.ntp1.ps5.canonical.com.), DstPort: 123 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.58(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.55(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.59(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.54(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.58(api.snapcraft.io.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.59(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.55(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.58(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.54(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.59(api.snapcraft.io.), DstPort: 443 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.27(snapstore-content-cache-2.ps5.canonical.com.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.28(snapstore-content-cache-3.ps5.canonical.com.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.26(snapstore-content-cache-1.ps5.canonical.com.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.27(snapstore-content-cache-2.ps5.canonical.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=994 Comm=NetworkManager ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.190.48(gladys.canonical.com.), DstPort: 80 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.58(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.54(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.59(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.55(api.snapcraft.io.), DstPort: 0 PROTO: 0
PID=956 Comm=snapd ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.125.188.58(api.snapcraft.io.), DstPort: 443 PROTO: 0
PID=3100 Comm=Chrome_ChildIOT ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=3100 Comm=Chrome_ChildIOT ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3100 Comm=Chrome_ChildIOT ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 13.107.246.45(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3100 Comm=Chrome_ChildIOT ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=3100 Comm=Chrome_ChildIOT ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=3100 Comm=Chrome_ChildIOT ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 51.104.15.253(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 172.64.41.4(Unknown), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 162.159.61.4(Unknown), DstPort: 0 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 172.64.41.4(Unknown), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 172.64.41.4(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.160.144.191(191.144.160.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.107.221.82(82.221.107.34.bc.googleusercontent.com.), DstPort: 0 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.107.221.82(82.221.107.34.bc.googleusercontent.com.), DstPort: 80 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.117.188.166(166.188.117.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.107.221.82(82.221.107.34.bc.googleusercontent.com.), DstPort: 80 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 23.215.0.133(a23-215-0-133.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 96.7.128.186(a96-7-128-186.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 172.64.41.4(Unknown), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 172.217.20.170(waw02s07-in-f170.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 216.58.215.42(par21s17-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 216.58.213.74(lhr25s01-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.250.179.74(par21s19-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.250.179.106(par21s20-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.250.178.138(par21s22-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.250.201.170(par21s23-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 216.58.214.74(fra15s10-in-f74.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.250.75.234(par10s41-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 216.58.214.170(mad01s26-in-f10.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #3 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #5 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #7 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.100(lf-in-f100.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.138(lf-in-f138.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.139(lf-in-f139.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.101(lf-in-f101.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.113(lf-in-f113.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.102(lf-in-f102.1e100.net.), DstPort: 0 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 172.64.41.4(Unknown), DstPort: 443 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.139(lg-in-f139.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.100(lg-in-f100.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.113(lg-in-f113.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.101(lg-in-f101.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.138(lg-in-f138.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.102(lg-in-f102.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.0.0.170(Unknown), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.0.0.171(Unknown), DstPort: 0 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.160.144.191(191.144.160.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.149.100.209(209.100.149.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #4 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Res~ver #10 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #9 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Res~ver #12 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.147(lb-in-f147.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.103(lb-in-f103.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.104(lb-in-f104.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.105(lb-in-f105.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.106(lb-in-f106.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.99(lb-in-f99.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.165.95(lg-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 209.85.233.95(lr-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 108.177.14.95(lt-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 74.125.131.95(lu-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.161.95(lh-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.162.95(li-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.251.1.95(lb-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.163.95(lj-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 173.194.220.95(lk-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 173.194.221.95(lm-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 173.194.222.95(lo-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 74.125.205.95(le-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 173.194.73.95(lq-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 142.250.150.95(la-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 64.233.164.95(lf-in-f95.1e100.net.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 96.7.128.192(a96-7-128-192.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 23.215.0.132(a23-215-0-132.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 23.215.0.132(a23-215-0-132.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 96.7.128.192(a96-7-128-192.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 23.215.0.133(a23-215-0-133.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 96.7.128.186(a96-7-128-186.deploy.static.akamaitechnologies.com.), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #6 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Res~ver #10 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Res~ver #11 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #2 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #3 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #8 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.107.243.93(93.243.107.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
2025/03/25 00:14:45 invalid event size
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 127.0.0.53(_localdnsstub), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.120.208.123(123.208.120.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 34.149.100.209(209.100.149.34.bc.googleusercontent.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
2025/03/25 00:14:45 invalid event size
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
2025/03/25 00:14:45 invalid event size
2025/03/25 00:14:45 invalid event size
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.121.3(lb-140-82-121-3-fra.github.com.), DstPort: 443 PROTO: 0
2025/03/25 00:14:47 invalid event size
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.133(cdn-185-199-108-133.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.111.133(cdn-185-199-111-133.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.133(cdn-185-199-108-133.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.133(cdn-185-199-108-133.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.133(cdn-185-199-108-133.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.121.6(lb-140-82-121-6-fra.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.121.6(lb-140-82-121-6-fra.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.113.22(lb-140-82-113-22-iad.github.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.113.22(lb-140-82-113-22-iad.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.113.25(lb-140-82-113-25-iad.github.com.), DstPort: 443 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.0.0.171(Unknown), DstPort: 0 PROTO: 0
PID=4972 Comm=DNS Resolver #1 ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.0.0.170(Unknown), DstPort: 0 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=798 Comm=systemd-resolve ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 192.168.1.1(beelinerouter.net.), DstPort: 53 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.108.154(cdn-185-199-108-154.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 185.199.111.133(cdn-185-199-111-133.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 140.82.113.25(lb-140-82-113-25-iad.github.com.), DstPort: 443 PROTO: 0
PID=4972 Comm=Socket Thread ,SrcIP: 0.0.0.0(Unknown), SrcPort: 0 -> DstIP: 173.194.73.95(lq-in-f95.1e100.net.), DstPort: 443 PROTO: 0

 


