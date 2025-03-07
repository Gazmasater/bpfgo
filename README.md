gcc udp_server.c -o udp_server

ip addr show

sudo tcpdump -i any udp port 5353

gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ ip addr show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute 
       valid_lft forever preferred_lft forever
2: wlp0s20f3: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether e4:fd:45:a8:ec:0b brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.71/24 brd 192.168.1.255 scope global dynamic noprefixroute wlp0s20f3
       valid_lft 86333sec preferred_lft 86333sec
    inet6 fe80::d6b2:9200:15bb:a0e8/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
gaz358@gaz358-BOD-WXX9:~/myprog/bpfgo$ sudo tcpdump -i any udp port 5353
[sudo] password for gaz358: 
tcpdump: data link type LINUX_SLL2
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on any, link-type LINUX_SLL2 (Linux cooked v2), snapshot length 262144 bytes
18:00:16.942866 wlp0s20f3 Out IP6 gaz358-BOD-WXX9.mdns > ff02::fb.mdns: 0 [2q] PTR (QM)? _ipp._tcp.local. PTR (QM)? _ipps._tcp.local. (45)
18:00:16.942975 wlp0s20f3 Out IP gaz358-BOD-WXX9.mdns > mdns.mcast.net.mdns: 0 [2q] PTR (QM)? _ipp._tcp.local. PTR (QM)? _ipps._tcp.local. (45)
18:00:16.943055 lo    In  IP localhost.mdns > mdns.mcast.net.mdns: 0 [2q] PTR (QM)? _ipp._tcp.local. PTR (QM)? _ipps._tcp.local. (45)

