
https://platform-balun.ru/teach/control/stream/view/id/930914027
user: mat3x@mail.ru
pass: Vkalashnikov00f!



bpf2go -output-dir . -tags linux -type trace_info -go-package=main -target amd64 bpf $(pwd)/trace.c -- -I$(pwd)



sudo nft add table ip6 test
sudo nft add chain ip6 test prerouting '{ type filter hook prerouting priority 0; }'
sudo nft add rule ip6 test prerouting exthdr type routing accept

chain prerouting {
    type nat hook prerouting priority 0;
}
sudo nft add rule ip test prerouting ip daddr 192.168.1.0/24 accept




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



git rebase -i db77c5b 
Первый из них оставляешь как pick, остальные 9 меняешь на squash (или s):

git rebase --abort


git push -u origin trace_core1 --force




_______________________________________________________________________________________________


struct trace_info {
    /*  offset=0: */
    struct in_addr   srcIP;   //  4 байта, выровнено по 4
    /*  offset=4: */
    struct in_addr   dstIP;   //  4 байта, выровнено по 4
    /*  offset=8 (уже кратно 4): */
    struct in6_addr  srcIP6;  // 16 байт, выровнено по 4
    /*  offset=24: */
    struct in6_addr  dstIP6;  // 16 байт, выровнено по 4
    /*  offset=40: */
    __u32            pid;     //  4 байта, выровнено по 4
    /*  offset=44: */
    __u32            proto;   //  4 байта, выровнено по 4
    /*  offset=48: */
    __u16            sport;   //  2 байта, выровнено по 2
    /*  offset=50: */
    __u16            dport;   //  2 байта, выровнено по 2
    /*  offset=52: */
    __u16            family;  //  2 байта, выровнено по 2
    /*  offset=54: */
    __u8             sysexit; //  1 байт, выровнено по 1
    /*  offset=55: */
    __u8             state;   //  1 байт, выровнено по 1
    /*  offset=56 (следующий после 55 + 0 padding, потому что uint8+uint8 = 2 байта, 
       а общий offset 56 уже кратен 2): */
    char             comm[32];// 32 байта, выровнено по 1
    /*  offset=88 -> конец */
};












