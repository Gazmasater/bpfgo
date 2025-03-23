lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


            curl-8049    [005] ...21  1845.700905: bpf_trace_printk: !!!sys_exit_connect FAMILY=2 ADDRESS=77.88.55.242:80 Comm=curl 
           <...>-8048    [002] ...21  1845.701183: bpf_trace_printk: !!!sys_exit_connect FAMILY=2 ADDRESS=5.255.255.242:80 Comm=curl 

           nslookup ya.ru




  // Исключаем 127.0.0.53:53 (локальный DNS)
    if (dstIP == bpf_ntohl(0x7F000035) && dstPort == 53) {
        return SK_PASS;
    }


