sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
sudo nft add rule ip test prerouting dup to 10.1.2.3
sudo nft list table ip test
sudo nft add rule ip test prerouting oifname "lo" ip daddr 192.168.1.10 counter log



sudo nft add rule inet filter input exthdr hdtype dst option 23 accept

gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add table ip test
sudo nft add chain ip test prerouting '{ type nat hook prerouting priority 0; }'
[sudo] password for gaz358: 
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ nft add rule inet filter input exthdr hdtype dst option 23 accept
Operation not permitted (you must be root)
Error: syntax error, unexpected string
add rule inet filter input exthdr hdtype dst option 23 accept
                                  ^^^^^^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule inet filter input exthdr hdt
ype dst option 23 accept
Error: syntax error, unexpected string
add rule inet filter input exthdr hdtype dst option 23 accept
                                  ^^^^^^





