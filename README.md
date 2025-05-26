sudo nft add table inet test
sudo nft add chain inet test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule inet test prerouting exthdr dst exists accept
sudo nft add rule inet test prerouting ip6 exthdr hdrlength 60 counter log prefix "dstopt match" accept


sudo nft list ruleset




gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule inet test prerouting ip6 nexthdr 60 accept
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft list ruleset
table inet test {
        chain prerouting {
                type filter hook prerouting priority filter; policy accept;
                ip6 nexthdr ipv6-opts accept
        }
}
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 


















