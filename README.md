sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept





sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept


sudo nft list ruleset

table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                exthdr dst exists accept
        }
}


sudo nft add rule ip6 test prerouting exthdr dst exists accept
sudo nft add rule ip6 test prerouting exthdr hopopts exists accept
sudo nft add rule ip6 test prerouting exthdr frag exists accept
sudo nft add rule ip6 test prerouting exthdr routing exists accept



nft add rule ip filter input ip daddr & 0xffffff00 == 192.168.1.0/24 accept


az358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule ip filter input ip daddr & 0
xffffff00 == 192.168.1.0/24 accept
[1] 5009
Error: syntax error, unexpected newline
add rule ip filter input ip daddr
                                 ^
0xffffff00: command not found
[1]+  Exit 1                  sudo nft add rule ip filter input ip daddr
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 







