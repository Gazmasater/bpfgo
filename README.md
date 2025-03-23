lsns -t net


sudo ip netns list


bpf2go -output-dir $(pwd)/generated -tags linux -type trace_info -go-package=load -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)


https://arthurchiao.art/blog/pidfd-and-socket-lookup-bpf-illustrated/


            curl-8049    [005] ...21  1845.700905: bpf_trace_printk: !!!sys_exit_connect FAMILY=2 ADDRESS=77.88.55.242:80 Comm=curl 
           <...>-8048    [002] ...21  1845.701183: bpf_trace_printk: !!!sys_exit_connect FAMILY=2 ADDRESS=5.255.255.242:80 Comm=curl 

           nslookup ya.ru

           gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$            nslookup ya.ru
Server:         127.0.0.53
Address:        127.0.0.53#53

Non-authoritative answer:
Name:   ya.ru
Address: 5.255.255.242
Name:   ya.ru
Address: 77.88.55.242
Name:   ya.ru
Address: 77.88.44.242
Name:   ya.ru
Address: 2a02:6b8::2:242

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ 




  // Исключаем 127.0.0.53:53 (локальный DNS)
    if (dstIP == bpf_ntohl(0x7F000035) && dstPort == 53) {
        return SK_PASS;
    }


