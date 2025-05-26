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


gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule ip6 test prerouting exthdr type routing accept
Error: syntax error, unexpected type
add rule ip6 test prerouting exthdr type routing accept
                                    ^^^^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ sudo nft add rule ip6 test prerouting exthdr  
routing accept
Error: syntax error, unexpected string
add rule ip6 test prerouting exthdr routing accept
                                    ^^^^^^^
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ nft --version
nftables v1.0.9 (Old Doc Yak #3)
gaz358@gaz358-BOD-WXX9:~/myprog/nft-go/internal/expr-encoders$ 







